////////////////////////////////////////////////////////////////////////////////
// $Id: AmpServerProADC.cpp 7942 2024-03-15 08:04:26Z mellinger $
// Description: A source module that interfaces with EGI's Amp Server Pro SDK.
// Authors: Joshua Fialkoff, Robert Bell (EGI),
//          juergen.mellinger@uni-tuebingen.de
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
////////////////////////////////////////////////////////////////////////////////
#include "AmpServerProADC.h"

#include "BinaryData.h"
#include "GenericADC.h"
#include "BCIStream.h"
#include "PrecisionTime.h"

//#define bcidbgOn
#define SCALING_FACTOR 0.000155220429


// Size of each packet in bytes
#define SIZEOF_PACKETTYPE_1 1152
#define SIZEOF_PACKETTYPE_2 1264

using namespace std;
using namespace tinyxml2;

RegisterFilter(AmpServerProADC, 1);



namespace {

  class ResponseNode
  {
  public:
    typedef list<ResponseNode> List;

    ResponseNode()
    {}
    explicit ResponseNode(istream& is)
    {
      Parse(is);
    }
    const string& Name() const
    {
      return mName;
    }
    const string& Value() const
    {
      return mValue;
    }
    List Leaves() const;
    istream& Parse(istream&);
    ostream& Print(ostream&) const;
  private:
    string mName, mValue;
    List mChildren;
  };

  ResponseNode::List
    ResponseNode::Leaves() const
  {
    List result;
    if (mChildren.empty())
      result.push_back(*this);
    else for (List::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i)
    {
      List leaves = i->Leaves();
      result.insert(result.end(), leaves.begin(), leaves.end());
    }
    return result;
  }

  istream&
    ResponseNode::Parse(istream& is)
  {
    if ((is >> ws).peek() != '(')
      is.setstate(ios::failbit);
    is.ignore();
    is >> mName;
    while ((is >> ws).peek() == '(')
      mChildren.push_back(ResponseNode(is));
    if (mChildren.empty())
      getline(is, mValue, ')');
    else
    {
      if ((is >> ws).peek() != ')')
        is.setstate(ios::failbit);
      is.ignore();
    }
    return is;
  }

  ostream&
    ResponseNode::Print(ostream& os) const
  {
    os << "( " << mName;
    if (!mValue.empty())
      os << "=" << mValue;
    for (List::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i)
      i->Print(os << " ");
    return os << " )";
  }

  ostream& operator<<(ostream& os, const ResponseNode& r)
  {
    return r.Print(os);
  }

} // namespace

AmpServerProADC::AmpServerProADC() :
  mAmpState(0),
  mEnableStim(false),
  mStartRequested(false),
  mStimAlreadyRunning(false),
  mUploadInProgress(0),
  mUploadCompleted(0),
  mAutomaticTrigger(false),
  mUploadExpressionAlreadyFulfilled(0),
  mStartExpressionAlreadyFulfilled(false),
  mAbortExpressionAlreadyFulfilled(false),
  mStimThreadIsRunning(false),
  mKillThreads(false),
  mThreadMessage(DO_NOTHING),
  mThreadLock(NULL),
  mMillisecondsPerSampleBlock(0),
  mStimDelayInMilliseconds(0)
{
}

AmpServerProADC::~AmpServerProADC()
{
  mKillThreads = true;
  SendMessageToStimThread( EXIT_THREAD );
  if (mThreadLock) CloseHandle(mThreadLock);
  mThreadLock = NULL;

  if (mStimThread.joinable())
      mStimThread.join();
  Halt();
}

void
AmpServerProADC::SendMessageToStimThread( int messageValue )
{
  mThreadMessage = messageValue;
  if (mThreadLock) SetEvent(mThreadLock);
}


void
AmpServerProADC::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "Source:Signal%20Properties list ChannelNames= 1 auto // list of channel names",

    "Source int SourceCh= auto auto % % ",
    "Source list SourceChOffset= 1 auto % % % % ",
    "Source list SourceChGain= 1 auto % % % % ",

    "Source:PacketType2 int SampleBlockSize= 40 40 1 % ",

    "Source int SamplingRate= auto auto % % ",
    "Source string ServerIP= localhost localhost % % // IP address of amplifier",
    "Source int CommandPort= auto auto 1 65535 // port number corresponding to the command layer",
    "Source int NotificationPort= auto auto 1 65535 // port number corresponding to the notification layer",
    "Source int StreamPort= auto auto 1 65535 // port number corresponding to the stream layer",
    "Source string AmplifierID=  auto auto 0 % // the ID of the Amplifier from which data should be collected",
    "Source int AmpState= 0 0 0 1 // set amp state to: 0 DefaultAcquisitionState, 1 DefaultSignalGeneration (enumeration)",


    "Source:GTEN int    RunTranscranialStim= 0 0 0 1 // Perform Transcranial Stimulation (boolean)",
    "Source:GTEN matrix UploadExpressions= 1 { Condition PlanFile TrainID } State(StimulusCode)>0 plan.mcf % // Expression to trigger upload of each train, with the .mcf file name and <train> id where the train is defined (empty id means first <train> in the file)",
    "Source:GTEN string StartExpression= % % % %  // Expression to trigger stimulation start (if empty, stimulate as soon as the train is uploaded)",
    "Source:GTEN string DelayExpression= 0 % % %  // Evaluated when StartExpression becomes true and delays the instruction by the resulting number of milliseconds",
    "Source:GTEN string AbortExpression= State(StimulusCode)==0 State(StimulusCode)==0 % %  // Expression to abort stimulation",
    END_PARAMETER_DEFINITIONS

    BEGIN_STREAM_DEFINITIONS
    "EGIDigitalInputStream 16 0",
    "GTENTrainRunning 8 0",
    "GTENStimulationTriggered 8 0",
    "GTENStimulationOn 1 0", // = !(States("GTENTrainRunning")&4)
    "DigitalInput1  1 0",
    "DigitalInput2  1 0",
    "DigitalInput3  1 0",
    "DigitalInput4  1 0",
    "DigitalInput5  1 0",
    "DigitalInput6  1 0",
    "DigitalInput7  1 0",
    "DigitalInput8  1 0",
    "DigitalInput9  1 0",
    "DigitalInput10 1 0",
    "DigitalInput11 1 0",
    "DigitalInput12 1 0",
    "DigitalInput13 1 0",
    "DigitalInput14 1 0",
    "DigitalInput15 1 0",
    "DigitalInput16 1 0",
    
    "GTENUploading  8 0",
    "GTENUploaded   8 0",
    END_STREAM_DEFINITIONS
}

void
AmpServerProADC::OnHalt()
{
  mConnection.Close();
}

void
AmpServerProADC::OnAutoConfig()
{
  Parameter("CommandPort") = 9877;
  Parameter("NotificationPort") = 9878;
  Parameter("AmplifierID") = 0;
  Parameter("SamplingRate") << 1000 << "Hz";
  Parameter("SampleBlockSize") = 40;

  int ampID = ActualParameter("AmplifierID");
  if (!mConnection.IsOpen(ampID))
  {
    mConnection.Open(
      ActualParameter("ServerIP"),
      ActualParameter("CommandPort"),
      ampID
    );
    mConnection.SendCommand("SetPower 1");

    if (!mConnection.SendCommand("Start"))
    {
      bcierr << "Unable to start the amplifier";
      return;
    }
  }

  if (mConnection.IsOpen(ampID))
  {
    if (mConnection.Amps() < 1)
      bcierr << "No amplifiers available";
    else if (ampID >= mConnection.Amps() || ampID < 0)
      bcierr << "Invalid amplifier ID: " << ampID;

    Parameter("NotificationPort") = mConnection.NotificationPort();
    Parameter("StreamPort") = mConnection.StreamPort();

    Parameter("SourceCh") = mConnection.Channels();
    Parameter("SourceChOffset")->SetNumValues(mConnection.Channels());
    Parameter("ChannelNames")->SetNumValues(mConnection.Channels());
    Parameter("SourceChGain")->SetNumValues(mConnection.Channels());
    for (int ch = 0; ch < mConnection.Channels(); ++ch)
    {
      Parameter("SourceChOffset")(ch) = 0;
      Parameter("ChannelNames")(ch) << "Ch" << (ch + 1);
      Parameter("SourceChGain")(ch) << mConnection.Gain_muV() << "muV";
    }
    Parameter("SamplingRate") << mConnection.SamplingRate() << "Hz";
  }
  else
  {
    bcierr << "Could not connect to AmpServerPro at " << mConnection.Address()
      << ", " << mConnection.NotificationPort()
      << ", or " << mConnection.StreamPort();
  }
}

void
AmpServerProADC::ParseUploadExpressions( std::vector<Expression> & expressions, std::vector<std::string> & trains, int expectedNumberOfChannels ) const
{
  ParamRef uploadExpressions = Parameter( "UploadExpressions" );
  if( uploadExpressions->NumColumns() != 3 && !( uploadExpressions->NumColumns() == 0 && uploadExpressions->NumRows() == 0 ) )
  {
    bcierr << "UploadExpressions parameter must have 3 columns";
    return;
  }
  expressions.clear();
  trains.clear();
  for (int iRow = 0; iRow < uploadExpressions->NumRows(); iRow++)
  {
    Expression expression = uploadExpressions(iRow, 0);
    expressions.push_back( expression );
    std::string mcfFileName = uploadExpressions(iRow, 1);
    std::string trainID = uploadExpressions(iRow, 2);
    std::string train = ReadTrain(mcfFileName, trainID, expectedNumberOfChannels);
    if( train != "" ) trains.push_back( train );
  }
}

std::string
AmpServerProADC::ReadTrain( const std::string & fileName, const std::string & targetTrainID, int expectedNumberOfChannels ) const
{
  tinyxml2::XMLDocument xml;
  xml.LoadFile(fileName.c_str());
  if (xml.ErrorID() == XML_SUCCESS) bciout << "Read XML file " << fileName;
  else bcierr << "Failed to load stimulation config from " << fileName << " - check that the file exists and that its content is valid XML.";

  XMLElement* root = xml.FirstChildElement("mcf");
  if( !root ) { bcierr << "no root <mcf> element in " << fileName; return ""; }
  
  XMLElement* channelCount = root->FirstChildElement("channelCount");
  int nChannelsConfiguredInXML = atoi(channelCount->GetText());
  if (channelCount && expectedNumberOfChannels && nChannelsConfiguredInXML != expectedNumberOfChannels)
      bcierr << "Stimulation Error: Number of channels defined in " << fileName << " (" << nChannelsConfiguredInXML << ") not equal to number of device channels (" << expectedNumberOfChannels << ")";

  XMLElement* trains = root->FirstChildElement("trains");
  if (!trains) { bcierr << "no <trains> element"; return ""; }

  XMLElement* train = trains->FirstChildElement("train");
  while (train)
  {
    const char* thisTrainID = train->Attribute("id");
    if (targetTrainID == "" || targetTrainID == thisTrainID) break;
    train = train->NextSiblingElement( "train" );
  }
  if (!train) { bcierr << "could not find <train> element in " << fileName << (targetTrainID=="" ? "" : ( " with id=\"" + targetTrainID + "\"" )); return ""; }
  
  XMLElement* blocks = train->FirstChildElement("blocks");
  XMLElement* firstBlock = blocks ? blocks->FirstChildElement("block") : NULL;
  if( !blocks ) bcierr << "<train> from file " << fileName << " contains no <blocks> element";
  else if( !firstBlock ) bcierr << "<blocks> from file " << fileName << " contains no <block> element";
  XMLElement* waveforms = train->FirstChildElement("waveforms");
  if( !waveforms ) bcierr << "<train> from file " << fileName << " contains no <waveforms> element";

  // delete train attribute 
  train->DeleteAttribute("id");

  // delete iterations element
  XMLNode* iterations = train->FirstChildElement("iterations");
  if (iterations) train->DeleteChild(iterations);

  //change to arbitrary if sine waveform
  XMLElement* sineWaveform = waveforms ? waveforms->FirstChildElement("sineWaveform") : NULL;
  while(sineWaveform)
  {
    XMLElement* freq = sineWaveform->FirstChildElement("frequency");
    if (freq)
    {
      double freqNum = stod(freq->GetText());
      const char* idNum = sineWaveform->Attribute("id");
      waveforms->DeleteChild(sineWaveform);
      XMLElement* arbitraryWaveform = waveforms->InsertNewChildElement("arbitraryWaveform");
      arbitraryWaveform->SetAttribute("id", idNum);
      //do a different method based on how fast the freq is
      int fNumer, fDenom;
      const char* sampArr[1024];
      ostringstream ss1;
      //will always start at 0
      ss1 << 0;
      if (freqNum < 50)
      {
        //generate samples for the desired freq, then specify freq as 1
        for (int i = 1; i < 1024; i++)
        {
          double thisNum = sin(i * 2 * Pi() * freqNum / 1024);
          ss1 << " " << thisNum;
        }
        fDenom = 1;
        fNumer = 1;
      }
      else
      {
        //generate samples for 1 Hz, then specify freq
        for (int i = 1; i < 1024; i++)
        {
          double thisNum = sin(i * 2 * Pi() / 1024);
          ss1 << " " << thisNum;
        }
        fDenom = 1000;
        fNumer = freqNum * fDenom;
      }
      arbitraryWaveform->InsertNewChildElement("samples");
      arbitraryWaveform->FirstChildElement("samples")->SetText(ss1.str().c_str());
      arbitraryWaveform->InsertNewChildElement("frequencyNumerator");
      arbitraryWaveform->FirstChildElement("frequencyNumerator")->SetText(fNumer);
      arbitraryWaveform->InsertNewChildElement("frequencyDenominator");
      arbitraryWaveform->FirstChildElement("frequencyDenominator")->SetText(fDenom);
    }
    sineWaveform = sineWaveform->NextSiblingElement( "sineWaveform" );
  }

  XMLPrinter trainRenderer(NULL, true);
  train->Accept(&trainRenderer);
  return trainRenderer.CStr();
}

void
AmpServerProADC::OnPreflight(SignalProperties& outputProperties) const
{
  const Connection& AmpServer = mConnection;
  if (mConnection.PacketType() == 1)
    PreflightCondition(Parameter("SampleBlockSize") == 40);
  if ((bool)Parameter("RunTranscranialStim"))
  {
    if(mConnection.PacketType() == 2)
    {
      Expression startExpression = Parameter("StartExpression"); startExpression.Evaluate();
      Expression delayExpression = Parameter("DelayExpression"); delayExpression.Evaluate();
      Expression abortExpression = Parameter("AbortExpression"); abortExpression.Evaluate();
      std::vector<Expression> uploadExpressions;
      std::vector<std::string> trains;
      ParseUploadExpressions( uploadExpressions, trains, Parameter( "SourceCh" ) );
      for( int i = 0; i < uploadExpressions.size(); i++ ) uploadExpressions[i].Evaluate();
    }
    else
      bcierr << "Stimulation not possible on requested device. Device must be a GTEN200 and use packet type 2";
  }
  PreflightCondition(Parameter("SourceCh") <= AmpServer.Channels());
  PreflightCondition(Parameter("SamplingRate").InHertz() == AmpServer.SamplingRate());
  if (Parameter("ChannelNames")->NumValues() < Parameter("SourceCh"))
  {
    bcierr << "ChannelNames parameter length (" << Parameter("ChannelNames")->NumValues() << ") differs from the SourceCh";
  }
  for (int ch = 0; ch < Parameter("SourceCh"); ++ch)
  {
    outputProperties.ChannelLabels()[ch] = Parameter("ChannelNames")(ch);
    PreflightCondition(Parameter("SourceChOffset")(ch) == 0);
    double diff = Parameter("SourceChGain")(ch).InMicrovolts() - AmpServer.Gain_muV();
    if (::fabs(diff) > 1e-3)
      bcierr << "Wrong SourceChGain: differs by more than 1/1000";
  }
  outputProperties = SignalProperties(Parameter("SourceCh") + 19, Parameter("SampleBlockSize"), SignalType::float32);
  int extraChannelIndex = Parameter("SourceCh");
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@EGIDigitalInputStream";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput1";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput2";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput3";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput4";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput5";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput6";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput7";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput8";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput9";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput10";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput11";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput12";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput13";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput14";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput15";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@DigitalInput16";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@GTENTrainRunning";
  outputProperties.ChannelLabels()[extraChannelIndex++] = "@GTENStimulationOn"; // = !(States("GTENTrainRunning")&4)

  State("GTENStimulationTriggered");
  State("GTENUploading");
  State("GTENUploaded");
}

void
AmpServerProADC::OnInitialize(const SignalProperties&)
{
  //mConnection.SendCommand ("SetPower 1");
  mAmpState = Parameter("AmpState");
  mEnableStim = (bool)Parameter("RunTranscranialStim");
  if (mEnableStim)
  {
    ParseUploadExpressions( mUploadExpressions, mTrains );
    mAutomaticTrigger = Parameter("StartExpression") == "";
    mStartExpression = (Expression)Parameter("StartExpression");
    mDelayExpression = (Expression)Parameter("DelayExpression");
    mAbortExpression = (Expression)Parameter("AbortExpression");
    mMillisecondsPerSampleBlock = 1000 * Parameter("SampleBlockSize").ToNumber() / Parameter("SamplingRate").InHertz();
    
    mStartRequested = false;                  //
    mStimAlreadyRunning = false;              //
    mUploadExpressionAlreadyFulfilled = 0;   // TODO: maybe put these in StartRun() as well?
    mStartExpressionAlreadyFulfilled = false; //
    mAbortExpressionAlreadyFulfilled = false; //
    
    if (!mStimThreadIsRunning)
    {
      mThreadLock = CreateEvent(NULL, false, false, NULL);
      mStimThread = thread(&AmpServerProADC::StimThread, this);
    }
  }
}

void
AmpServerProADC::OnStartAcquisition()
{/*
  if( !mConnection.SendCommand( "Start" ) )
  {
    bcierr << "Unable to start the amplifier";
    return;
  }*/

  // Always set the amp to the default acquisition state first.
  mConnection.SendCommand("DefaultAcquisitionState");
  // Now send the desired start state (if not the already set default acquisition state).
  switch (mAmpState)
  {
  case 0:
    break;
  case 1:
    mConnection.SendCommand("DefaultSignalGeneration");
    break;
  }
  if (!mConnection.StartStreaming())
    bcierr << "Unable to start data streaming";
}

void
AmpServerProADC::OnStopAcquisition()
{
  if (!mConnection.StopStreaming())
    bcierr << "Unable to stop data streaming";
  else
    mConnection.SendCommand("Stop");
}

void
AmpServerProADC::DoAcquire(GenericSignal& outputSignal)
{
  if (!mConnection.ReadData(outputSignal))
    throw bcierr << mConnection.Error();
}


void
AmpServerProADC::OnProcess()
{
  if (mEnableStim)
  {
    bool stimRunning = State("GTENStimulationOn");
    bool justStopped = mStimAlreadyRunning && !stimRunning;
    mStimAlreadyRunning = stimRunning;
    if( stimRunning ) mUploadCompleted = 0;

    if (State("Running"))
    {
      int uploadExpressionFulfilled = 0;
      for (int iTrain = 0; iTrain < mUploadExpressions.size(); iTrain++)
      {
        if (mUploadExpressions[iTrain].Evaluate() != 0)
        {
          uploadExpressionFulfilled = iTrain + 1;
          break;
        }
      }
      bool triggerUpload = ( uploadExpressionFulfilled && uploadExpressionFulfilled != mUploadExpressionAlreadyFulfilled );
      mUploadExpressionAlreadyFulfilled = uploadExpressionFulfilled;
      
      bool startExpressionFulfilled = ( mStartExpression.Evaluate() != 0 );
      bool triggerStart = startExpressionFulfilled && !mStartExpressionAlreadyFulfilled;
      mStartExpressionAlreadyFulfilled = startExpressionFulfilled;

      bool abortExpressionFulfilled = ( mAbortExpression.Evaluate() != 0 );
      bool triggerAbort = abortExpressionFulfilled && !mAbortExpressionAlreadyFulfilled;
      mAbortExpressionAlreadyFulfilled = abortExpressionFulfilled;

      if( triggerUpload )
      {
        if( stimRunning ) bcierr << "GTEN: could not upload stim - stimulator is busy delivering stimulation";
        else if( mStartRequested ) bcierr << "GTEN: could not upload stim - stimulator start command is already queued";
        else
        {
          State( "GTENUploading" ) = mUploadInProgress = uploadExpressionFulfilled;
          mConnection.ForgetTrain();
          SendMessageToStimThread( uploadExpressionFulfilled ); // !! NB: the stimulator creates a small transient artifact about 1s after the upload is initiated
          if( mAutomaticTrigger )
          {
            // mAutomaticTrigger will be detected in the StimThread, which will fall through to sending a start command immediately after it sends the upload command
            State("GTENStimulationTriggered") = mUploadInProgress;
            mStartRequested = true; 
          }
        }
      }
      
      if( triggerStart && !mStartRequested)
      {
        // If you arrive here, that means you're using a StartExpression, and presumably you're doing that to improve timing precision...
        if(            stimRunning ) bcierr << "GTEN: could not start stim - already running";
        else if( !mUploadCompleted ) bcierr << "GTEN: could not start stim - train not uploaded";
        else if( mUploadInProgress ) bcierr << "GTEN: could not start stim - busy uploading"; // ...therefore, do not allow the start command to be queued up behind an ongoing upload, because that would lower precision again.
        else
        {
          State("GTENStimulationTriggered") = mUploadCompleted;
          mStimDelayInMilliseconds = mDelayExpression.Evaluate();
          if( mStimDelayInMilliseconds ) bciout << "GTEN: triggering stimulation with " << mStimDelayInMilliseconds << "ms additional delay";
          else                           bciout << "GTEN: triggering stimulation";
          mStartRequested = true;
          SendMessageToStimThread( START_STIM );
        }
      }
      else if ((State("GTENStimulationTriggered") && justStopped) )
      {
        bciout << "GTEN: stimulation finished";
        mStartRequested = false;
        State("GTENStimulationTriggered") = 0;
      }
      else if (triggerAbort && stimRunning)
      {
        bciwarn << "GTEN: aborting the stimulation";
        mStartRequested = false;
        State("GTENStimulationTriggered") = 0;
        SendMessageToStimThread( STOP_STIM );
      }
    }
    else
    { // handle the case where State("Running") is 0
      if (State("GTENTrainRunning") == 1) // TODO: will this ever change, though, if Running is 0?
      {
        //stop stimulation if not in run
        mConnection.EndStimulation();
      }
    }
    if( mConnection.TrainUploaded() && mUploadInProgress ) { mUploadCompleted = mUploadInProgress; mUploadInProgress = 0; }
    State( "GTENUploading" ) = mUploadInProgress;
    State( "GTENUploaded" ) = mUploadCompleted;
  } // if( mEnableStim)
}
void
AmpServerProADC::StimThread()
{
  mStimThreadIsRunning = true;
  bool keepGoing = true;
  while( keepGoing && !mKillThreads )
  {
    DWORD res = WaitForSingleObject(mThreadLock, 100 * mMillisecondsPerSampleBlock); // wait for message from main thread
    if(res != WAIT_OBJECT_0) continue;
    
    if( mThreadMessage > 0 ) // interpret message as a 1-based index into mTrains, for upload
    {
      if( mThreadMessage > mTrains.size() ) bcierr << "upload condition #" << mThreadMessage << " is out of range";
      else
      {
        mUploadInProgress = mThreadMessage;
        bciout << "GTEN: uploading stimulation train corresponding to UploadExpressions row #" << mThreadMessage;
        if( !mConnection.UploadTrain( mTrains[ mThreadMessage - 1 ] ) ) bcierr << "GTEN: failed to upload stim train";
        if( mAutomaticTrigger && !mConnection.BeginStimulation() ) bcierr << "GTEN: failed to schedule stim train to start after upload";
      }
    }
    else switch( mThreadMessage )
    {
      case START_STIM:
        if (mStimDelayInMilliseconds > 0)
        {
          bciout << "GTEN: waiting for " << mStimDelayInMilliseconds << "ms";
          PrecisionTime::NumType t0 = PrecisionTime::Now();
          mClock.SetInterval(Time::Seconds(mStimDelayInMilliseconds / 1000.0));
          mClock.Start();
          WithThreadPriority(ThreadUtils::Priority::Maximum - 1) mClock.Wait();
          mClock.Stop();
          PrecisionTime::NumType t1 = PrecisionTime::Now();
          bciout << "GTEN: finished waiting after " << PrecisionTime::UnsignedDiff(t1, t0) << "ms";
        }
        if( !mUploadCompleted ) bcierr << "GTEN: cannot start stim train because none is uploaded";
        else if (!mConnection.BeginStimulation()) bcierr << "GTEN: failed to start stim train";
        break;
        
      case STOP_STIM:
        if (!mConnection.EndStimulation()) bciwarn << "GTEN: failed to stop stim train";
        break;
      
      case EXIT_THREAD:
        keepGoing = false;
        break;
    } // end of switch(mThreadMessage)
    
    mThreadMessage = DO_NOTHING; // message handled, so put the flag down
  } // end of main while(keepGoing) loop
  mStimThreadIsRunning = false;
}

AmpServerProADC::Connection::Connection()
  : mSamplesInStream(0),
    mSamplesInOutput(0),
    mFirstRead(false),
    mNotificationPort(0),
    mStreamPort(0),
    mTimeout(0),
    mInitialTimeout(0),
    mAmpId(0),
    mAmps(0),
    mChannels(0),
    mGain_muV(0),
    mSamplingRate(0),
    mPacketType(0),
    mUseNotification(false),
    mNumberOfPendingUploads(0),
    mTrainUploaded(false),
    mKillThreads(false)
{
  mNotificationThread = thread(&AmpServerProADC::Connection::NotificationThread, this);
}

AmpServerProADC::Connection::~Connection()
{
  mKillThreads = true;
  mNotificationThread.join();
  Close();
}

void 
AmpServerProADC::Connection::NotificationThread( void )
{
  const std::string trainStarted  = "ntn_GTENStartTrainSucceeded";
  const std::string trainUploaded = "ntn_GTENSetTrainSucceeded";
  while( !mKillThreads )
  {
    while( !mKillThreads && !mNotifications.is_open() ) ::Sleep( 50 );
    while( !mKillThreads &&  mNotifications.is_open() )
    {
      if( !mNotificationSocket.wait_for_read( 50 ) ) continue;
      ResponseNode parsedResponse( mNotifications );
      ResponseNode::List leaves = parsedResponse.Leaves();
      ResponseNode::List::const_iterator itr = leaves.begin();
      if( itr == leaves.end() ) continue;
      const ResponseNode& node = *itr;
      if(      node.Value().substr( 0, trainStarted.length()  ) == trainStarted  ) { mNumberOfPendingUploads = 0; mTrainUploaded = false; }
      else if( node.Value().substr( 0, trainUploaded.length() ) == trainUploaded )
      {
        if( mNumberOfPendingUploads > 0 ) mNumberOfPendingUploads--;
        if( mNumberOfPendingUploads == 0 ) mTrainUploaded = true;
      }
      bcidbg << parsedResponse;
    }
  }
}

bool
AmpServerProADC::Connection::Open(const std::string& inIP, int inPort, int inAmpId)
{
  Close();

  mInitialTimeout = 7000;
  mTimeout = 1000;

  ostringstream oss;
  oss << inIP << ":" << inPort;
  mAddress = oss.str();
  mCommandSocket.open(mAddress);
  mCommands.open(mCommandSocket);

  mNotificationPort = 9878;
  mNotificationSocket.open(inIP, mNotificationPort);
  mNotifications.open(mNotificationSocket);

  mStreamPort = 9879;
  mStreamSocket.open(inIP, mStreamPort);
  mStream.open(mStreamSocket);

  mAmps = 0;
  std::map<std::string, std::string> results;
  if (SendCommand("NumberOfAmps", results))
    mAmps = ::atoi(results["number_of_amps"].c_str());
  mAmpId = inAmpId;

  return IsOpen(inAmpId) && ParseAmpDetails();
}

void
AmpServerProADC::Connection::Close()
{
  if( mCommands.is_open() ) SendCommand("SetPower 0");
  mStreamSocket.close();
  mStream.close();
  mStream.clear();
  mNotificationSocket.close();
  mNotifications.close();
  mNotifications.clear();
  mCommandSocket.close();
  mCommands.close();
  mCommands.clear();
}

bool
AmpServerProADC::Connection::IsOpen(int ampID) const
{
  return mCommands.is_open() && mStream.is_open() && ampID == mAmpId;
}


bool
AmpServerProADC::Connection::UploadTrain(const std::string & trainXML)
{
  string cmd = "(sendCommand cmd_GTENSetTrain " + to_string(mAmpId) + " 0 <?xml>" + trainXML + "</xml>)";
  bcidbg << cmd;  
  mNumberOfPendingUploads++;
  if (!(mCommands << cmd << endl)) { bciwarn << "GTENSetTrain command failed to send"; return false; }
  map<string,string> discardedOutputs;
  return GetCommandResponse( "GTENSetTrain", discardedOutputs, mInitialTimeout ); // Note the non-standard timeout value
}

bool AmpServerProADC::Connection::BeginStimulation()
{
  return SendCommand( "GTENStartTrain" );
}

bool
AmpServerProADC::Connection::EndStimulation()
{
  return SendCommand( "GTENAbortTrain" );
}

bool
AmpServerProADC::Connection::ParseAmpDetails()
{
  map<string,string> results;
  if( !SendCommand( "GetAmpDetails", results ) ) return false;
  mPacketType = stoi(results["packet_format"]);
  mChannels = stoi(results["number_of_channels"]);
  mAmpType = results["amp_type"];

  mSamplingRate = 1000;
  // EGI NA300 amps have a range of +/- 200,000uV that is digitized into
  // a 24bit signed value. The values received in BCI2000 are in float,
  // but use these values when converting back to uV.ss
  mGain_muV = 2e5 / (1 << 23);

  bciout << "Amp Type: " << mAmpType
    << "\nPacket Type: " << mPacketType
    << "\nNumber of Channels: " << mChannels;
  return true;
}

string
AmpServerProADC::Connection::BuildCommand(const string& inCommand)
{
  // inCommand could be for example "GTENStartTrain $val1 $val2" and that
  // would get translated into  "cmd_GTENStartTrain $ampId $val1 $val2"
  // with $val1 and $val2 defaulting to 0 if not supplied
  string name, val1, val2;
  istringstream iss(inCommand);
  if (!(iss >> name))
    return false;
  if (!(iss >> val1))
    val1 = "";
  if (!(iss >> val2))
  {
    val2 = val1;
    val1 = "";
  }
  if (val1.empty())
    val1 = "0";               // if val1 was not supplied, make it default to 0
  if (val2.empty())
    val2 = "0";               // if val2 was not supplied, make it default to 0

  ostringstream ampId;
  ampId << mAmpId; // to be inserted before the other two arguments
  string result = "(sendCommand cmd_" + name + " " + ampId.str() + " " + val1 + " " + val2 + ")";
  
  //bciout << result;
  return result;
}

bool
AmpServerProADC::Connection::SendCommand(const string& inCommand, int timeoutInMilliseconds)
{
  static map<string,string> discardedOutputs;
  return SendCommand( inCommand, discardedOutputs, timeoutInMilliseconds );
}
bool
AmpServerProADC::Connection::SendCommand(const string& inCommand, map<string,string>& outputs, int timeoutInMilliseconds)
{
  std::string cmd = BuildCommand(inCommand);
  if (!(mCommands << cmd << endl))
  {
    bciwarn << inCommand << " command failed to send";
    return false;
  }
  return GetCommandResponse( inCommand, outputs, timeoutInMilliseconds );
}

bool
AmpServerProADC::Connection::GetCommandResponse( const string & inCommand, map<string,string>& outputs, int timeoutInMilliseconds )
{
  if( timeoutInMilliseconds < 0 ) timeoutInMilliseconds = mTimeout;
  outputs.clear();
  if( !mCommandSocket.wait_for_read( timeoutInMilliseconds ) )
  {
    bciwarn << inCommand << " command timed out after " << timeoutInMilliseconds << "ms";
    return false;
  }
  bool success = false;
  ResponseNode parsedResponse(mCommands);
  if (parsedResponse.Name() != "sendCommand_return")
  {
    bciwarn << "unexpected response type \"" << parsedResponse.Name() << "\" to " << inCommand << " command";
    return false;
  }
  ResponseNode::List leaves = parsedResponse.Leaves();
  ResponseNode::List::const_iterator itr = leaves.begin();
  if( itr == leaves.end() )
  {
    bciwarn << "response to " << inCommand << " command was empty";
    return false;
  }
  const ResponseNode& status = *itr;
  if( status.Name() != "status" || status.Value() != "complete" )
  {
    bcierr << "GTEN: " << inCommand << " command not valid. " << status.Name() << "   " << status.Value();
    return false;
  }
  // return the result of the command if necessary
  while (++itr != leaves.end())
    outputs[ itr->Name() ] = itr->Value();
  return true;
}

bool
AmpServerProADC::Connection::StartStreaming()
{
  mFirstRead = true;
  mSamplesInStream = 0;
  mSamplesInOutput = 0;
  return (bool)(mStream << BuildCommand("ListenToAmp") << endl);
}

bool
AmpServerProADC::Connection::StopStreaming()
{
  bool result = !!(mStream << BuildCommand("StopListeningToAmp") << endl);
  while (mStream.rdbuf()->in_avail())
    mStream.ignore();
  return result;
}

bool
AmpServerProADC::Connection::ReadData(GenericSignal& outputSignal)
{
  if (mFirstRead)
  {
    mFirstRead = false;
    if (!mStreamSocket.wait_for_read(mInitialTimeout))
      mError = "AmpServer is available, but does not stream live EEG data. "
      "Please turn on the data streaming in Net Station Acquisition. After this, if it still not works, "
      "you need to obtain an AmpServerPro SDK license from EGI, "
      "and update EGI's HASP key.\n"
      "For more information, see http://www.egi.com.";
  }
  else if (!DoRead(outputSignal))
    mError = "Lost connection to AmpServerPro";
  return mError.empty();
}

bool
AmpServerProADC::Connection::DoRead(GenericSignal& outputSignal)
{
  if (mPacketType == 2)
  {
    while (!DoRead2(outputSignal))
    {
      if (!mStreamSocket.wait_for_read())
        return false;
      BinaryData<int64_t, BigEndian> ampId;
      if (!ampId.Get(mStream) || ampId != mAmpId)
        return false;
      BinaryData<uint64_t, BigEndian> packetSize;
      if (!packetSize.Get(mStream))
        return false;
      const int Packet2Size = SIZEOF_PACKETTYPE_2;
      if (packetSize % Packet2Size != 0)
        return false;
      mSamplesInStream = packetSize / Packet2Size;
      // AmpServerPro will crash if packets are not read immediately in full
      std::string buf(packetSize, '\0');
      mStream.read(const_cast<char*>(buf.data()), packetSize);
      mStringstream.str(buf);
      mStringstream.clear();
    }
  }
  else {
    while (!DoRead1(outputSignal))
    {
      if (!mStreamSocket.wait_for_read(mTimeout))
        return false;
      BinaryData<int64_t, BigEndian> ampId;
      if (!ampId.Get(mStream) || ampId != mAmpId)
        return false;
      BinaryData<uint64_t, BigEndian> packetSize;
      if (!packetSize.Get(mStream))
        return false;
      const int Packet1Size = SIZEOF_PACKETTYPE_1;
      if (packetSize % Packet1Size != 0)
        return false;
      mSamplesInStream = packetSize / Packet1Size;
      // AmpServerPro will crash if packets are not read immediately in full
      std::string buf(packetSize, '\0');
      mStream.read(const_cast<char*>(buf.data()), packetSize);
      mStringstream.str(buf);
      mStringstream.clear();
    }
  }
  return !!mStream;
}

bool
AmpServerProADC::Connection::DoRead2(GenericSignal& outputSignal)
{
  BinaryData<int32_t, LittleEndian, ArrayYes> data(256);
  BinaryData<uint8_t, LittleEndian>           trainData;
  BinaryData<uint16_t, LittleEndian>          digitalInputs;
  while (mSamplesInOutput < outputSignal.Elements() && mSamplesInStream > 0)
  {
    digitalInputs.Get(mStringstream);
    trainData.Get(mStringstream);
    mStringstream.ignore(sizeof(PacketFormat2Header) - sizeof(uint16_t) /*digitalInputs*/ - sizeof(uint8_t) /*tr*/);
    data.Get(mStringstream);
    mStringstream.ignore(sizeof(PacketFormat2Footer));

    for (int ch = 0; outputSignal.Properties().ChannelLabels()[ch].front() != '@'; ++ch)
      outputSignal(ch, mSamplesInOutput) = data[ch] * (float)SCALING_FACTOR;

    int streamChannelIndex = outputSignal.Channels() - 19;
    outputSignal(streamChannelIndex++, mSamplesInOutput) = digitalInputs;  // EGIDigitalInputStream stream
    for (int i = 0; i < 16; i++)
      outputSignal(streamChannelIndex++, mSamplesInOutput) = (digitalInputs >> i) & 1; // streams DigitalInput1 thru DigitalInput16  (NB: if disconnected, the voltages will be high and each corresponding bit value will be 1)
    outputSignal(streamChannelIndex++, mSamplesInOutput) = trainData;      // GTENTrainRunning stream
    outputSignal(streamChannelIndex++, mSamplesInOutput) = !(trainData&4); // GTENStimulationOn stream

    mSamplesInOutput += 1;
    mSamplesInStream -= 1;
  }
  bool done = (mSamplesInOutput >= outputSignal.Elements());
  if (done) mSamplesInOutput = 0;
  return done || !mStream;
}

bool
AmpServerProADC::Connection::DoRead1(GenericSignal& outputSignal)
{
  BinaryData<float, BigEndian> data;
  while (mSamplesInOutput < outputSignal.Elements() && mSamplesInStream > 0)
  {
    const int Packet1HeaderLength = 8 * sizeof(uint32_t);
    mStringstream.ignore(Packet1HeaderLength);
    const int Packet1ChannelCount = 256;
    for (int ch = 0; ch < std::min(outputSignal.Channels() - 1, Packet1ChannelCount); ++ch)
    {
      data.Get(mStringstream);
      outputSignal(ch, mSamplesInOutput) = data;
    }
    for (int ch = std::min(outputSignal.Channels() - 1, Packet1ChannelCount); ch < Packet1ChannelCount; ++ch)
      data.Get(mStringstream);
    ++mSamplesInOutput;
    const int Packet1FooterLength = 24 * sizeof(float);
    mStringstream.ignore(Packet1FooterLength);
    --mSamplesInStream;
  }
  bool done = (mSamplesInOutput >= outputSignal.Elements());
  if (done) mSamplesInOutput = 0;
  return done || !mStream;
}
