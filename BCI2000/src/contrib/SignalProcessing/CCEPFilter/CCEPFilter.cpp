////////////////////////////////////////////////////////////////////////////////
// Authors: Will Engelhardt
// Description: CCEPFilter implementation
////////////////////////////////////////////////////////////////////////////////

#include "CCEPFilter.h"
#include "BCIStream.h"
//#include "BitmapImage.h"
#include "WorkerThread.h"
#include "GenericVisualization.h"
#include <iomanip>

using namespace std;
RegisterFilter( CCEPFilter, 2.A );


CCEPFilter::CCEPFilter()
: mVisualize(false),
  mNumChannels(0),
  mStimulator(nullptr),
  mEpochSums(nullptr),
  mBaselineSums(nullptr),
  mBaselineBuffer(nullptr)
{
}

CCEPFilter::~CCEPFilter()
{
  if (mStimulator != nullptr)
  {
    delete mStimulator;
    mStimulator = nullptr;
  }
  Halt();
}

void
CCEPFilter::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
   //General configuration settings
   "Filtering:CCEP%20General%20Settings string OnsetExpression= 0 % % % "
      "// Expression that defines stimulus onset",

   //Duration parameters
   "Filtering:CCEP%20Duration%20Settings int BaselineEpochLength= 50ms 50ms 0 % "
      "// Length of data epoch before stimulus onset",
   "Filtering:CCEP%20Duration%20Settings int CCEPEpochLength= 500ms 500ms 0 % "
      "// Length of data epoch after stimulus onset and latency",

   //Quantity parameters
   "Filtering:CCEP%20Counting%20Settings int EpochsToAverage= 1 1 0 % "
      "// Number of epochs to average",
   "Filtering:CCEP%20Counting%20Settings int OnsetPeriod= 1 1 1 % "
      "// Only count every number of onset expressions ",

 END_PARAMETER_DEFINITIONS

   BEGIN_STATE_DEFINITIONS
   "CCEPTriggered 16 0 0 0",
   "StimulatingChannel 32 0 0 0",
   "ResetOnsetPeriod 8 0 0 0"
   END_STATE_DEFINITIONS
}

void
CCEPFilter::AutoConfig(const SignalProperties& Input)
{
  mStimulator = new StimulatorConfiguration();
  Parameter("CCEPEpochLength") = "500ms";
  Parameter("BaselineEpochLength") = "50ms";
}

void
CCEPFilter::Preflight( const SignalProperties& Input, SignalProperties& Output ) const
{
  Output = Input; // this simply passes information through about SampleBlock dimensions, etc....

  //stimulation parameters
  mStimulator->checkParameters(Input);

  Expression onsetExpression = Parameter("OnsetExpression").ToString();
  GenericSignal input(Input);
  onsetExpression.Evaluate(&input, 0);
  Expression t = Expression(Parameter("OnsetExpression"));
  t.Compile();

  int epochsToAverage = Parameter("EpochsToAverage");

  double outputSamples = Parameter("CCEPEpochLength").InSampleBlocks();
  double baselineSamples = Parameter("BaselineEpochLength").InSampleBlocks();
  //double outputSamples = mStimulator->getStimulationLength();
  outputSamples *= Input.Elements();
  baselineSamples *= Input.Elements();
  outputSamples = ::ceil(outputSamples);
  baselineSamples = ::ceil(baselineSamples);
  bciout << "Number of samples " << outputSamples << std::endl;
  bciout << "Number of baseline samples " << baselineSamples << std::endl;
  // Requested output signal properties.
  Output = Input;
  Output.SetChannels(Input.Channels())
    .SetElements(static_cast<size_t>(outputSamples + baselineSamples))
    .SetType(SignalType::float32)
    .SetIsStream(false)
    .ElementUnit()
    .SetRawMin(0)
    .SetRawMax(outputSamples + baselineSamples - 1);

  Parameter("OnsetPeriod");

  State("CCEPTriggered");
  State("StimulatingChannel");
  State("ResetOnsetPeriod");
}


void
CCEPFilter::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  mEpochs.clear();
  mBaselineEpochs.clear();
  if (mEpochSums != nullptr)
  {
    delete mEpochSums;
    mEpochSums = nullptr;
  }
  if (mBaselineSums != nullptr)
  {
    delete mBaselineSums;
    mBaselineSums = nullptr;
  }

  mOnsetExpression = Expression(Parameter("OnsetExpression"));
  mOnsetExpression.Compile();

  mOutputProperties = Output;
  mEpochsToAverage = Parameter("EpochsToAverage");

  double baselineSamples = Parameter("BaselineEpochLength").InSampleBlocks();
  baselineSamples *= Input.Elements();
  mBaselineSamples = ::ceil(baselineSamples);

  //initialize stimulator with set parameters
  mStimulator->initializeStimulator();  

  if (mBaselineBuffer != nullptr)
  {
    delete mBaselineBuffer;
    mBaselineBuffer = nullptr;
  }
  mBaselineProperties = mOutputProperties;
  mBaselineProperties.SetElements(mBaselineSamples);

  mEpochProperties = mOutputProperties;
  double outputSamples = Parameter("CCEPEpochLength").InSampleBlocks();
  outputSamples *= Input.Elements();
  outputSamples = ::ceil(outputSamples);
  mEpochProperties.SetElements(outputSamples);

  //mBaselineBuffer = new EpochBuffer(sp, 0);
  mBaselineBuffer = new ReverseBuffer(mBaselineProperties);

  mOnsetPeriod = Parameter("OnsetPeriod").ToNumber();

  mAllData = GenericSignal(mOutputProperties);
}

void
CCEPFilter::StartRun()
{
  // The user has just pressed "Start" (or "Resume")
  mPreviousExpressionValue = false;
  mEpochs.clear();
  mBaselineEpochs.clear();
  if (mEpochSums != nullptr)
  {
    delete mEpochSums;
    mEpochSums = nullptr;
  }
  if (mBaselineSums != nullptr)
  {
    delete mBaselineSums;
    mBaselineSums = nullptr;
  }
  //p->mVisSignal = GenericSignal(mOutputProperties.Channels() * p->mHoldMax, sp.Elements());

  mOnsetCounter = 0;
  mEpochMap.clear();

  State("CCEPTriggered") = 0;
  State("StimulatingChannel") = 0;
  State("ResetOnsetPeriod") = mOnsetPeriod;
}


void
CCEPFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{

  // And now we're processing a single SampleBlock of data.
  // Remember not to take too much CPU time here, or you will break the real-time constraint.

  //Output = GenericSignal(mOutputProperties);
  //Only a filter for visualizing, not changing data
  //Output = Input;
  if (mEpochsToAverage > 0)
  {
    int stimulusOnset = -1;
    set<int> stimulusOnsets;
    for (int i = 0; i < Statevector->Samples() - 1; ++i)
    {
      bool value = mOnsetExpression.Evaluate(&Input, i);
      if (value && !mPreviousExpressionValue)
      {
        stimulusOnset = i;
        stimulusOnsets.insert(i);
      }
      mPreviousExpressionValue = value;
    }
    //check if reset period is flagged from remote application
    if (State("ResetOnsetPeriod") != mOnsetPeriod)
    {
      mOnsetCounter = 0;
      mOnsetPeriod = State("ResetOnsetPeriod");
      //State("ResetOnsetPeriod") = 0;
    }

    if (!stimulusOnsets.empty())
    {
      for (auto it = stimulusOnsets.begin(); it != stimulusOnsets.end(); it++)
      {
        mOnsetCounter++;
        if ((mOnsetCounter-1) % mOnsetPeriod == 0) //only save a periodic number of onsets
        {
          mOnsetCounter = 1;
          // First block of stimulus presentation -- create a new epoch buffer.
          EpochBuffer* eBuf = new EpochBuffer(mEpochProperties, *it);
          mEpochs.insert(eBuf);
          auto s = mStimulator->getChannels(&Input);
          uint32_t stimChs = 0;
          for (auto it = s.begin(); it != s.end(); it++) {
            stimChs += pow(2, *it); //encode by bits
          }
          State("StimulatingChannel") = stimChs;

          //mBaselineBuffer->Finalize(Input, *it);
          ReverseBuffer* finalBuf = new ReverseBuffer(*mBaselineBuffer); //make a copy (hopefully deep?)
          finalBuf->Finalize(Input, *it);
          mEpochMap[eBuf] = finalBuf;
        }
      }
    }
    mBaselineBuffer->Process(Input);

    if (!mEpochs.empty())
    {
      EpochSet obsoleteEpochs;
      ReverseEpochSet obsoleteBaselineEpochs;
      for (auto& pBuffer : mEpochs)
      {
        pBuffer->Process(Input);
        if (pBuffer->EpochDone())
        { // Move buffer data into the epoch sum associated with the stimulus code,
            // and note the epoch buffer for later disposal.
          bcidbg << "Epoch done";
          if (mEpochSums == nullptr)
          {
            bcidbg << "Allocating result buffer";
            mEpochSums = new DataSum(mEpochProperties);
          }
          mEpochSums->Add(pBuffer->Data());
          obsoleteEpochs.insert(pBuffer);
          //baseline
          auto bBuf = mEpochMap.at(pBuffer);
          GenericSignal d = bBuf->PopData();
          obsoleteBaselineEpochs.insert(bBuf);
          //GenericSignal d = mBaselineBuffer->PopData();
          if (mBaselineSums == nullptr)
          {
            bcidbg << "Allocating baseline buffer";
            mBaselineSums = new DataSum(d.Properties());
          }
          mBaselineSums->Add(d);
          if (mEpochSums->Count() == mEpochsToAverage)
          { // When the number of required epochs is reached, copy the buffer average
            // into the output signal, and set states appropriately.
            for (int channel = 0; channel < Output.Channels(); ++channel)
            {
              for (int sample = 0; sample < mBaselineSamples; ++sample)
              {
                mAllData(channel, sample) = (*mBaselineSums)(channel, sample) / mEpochsToAverage;
              }
              for (int sample = mBaselineSamples; sample < Output.Elements(); ++sample)
              {
                mAllData(channel, sample) = (*mEpochSums)(channel, sample - mBaselineSamples) / mEpochsToAverage;
              }
            }
            State("CCEPTriggered") = (uint16_t)(State("CCEPTriggered") + 1);

          }
        }
      }
      for (auto& pBuffer : obsoleteEpochs)
        mEpochs.erase(pBuffer);
      for (auto& pBuffer : obsoleteBaselineEpochs)
        mBaselineEpochs.erase(pBuffer);
      // Epochs will be deallocated from the obsoleteEpochs destructor.
    }

    if (mEpochSums != nullptr && mEpochSums->Count() >= mEpochsToAverage)
    { // Reset the data sum buffer.
      bcidbg << "Clearing buffer";
      *mEpochSums = DataSum(mEpochProperties);
      *mBaselineSums = DataSum(mBaselineProperties);
    }
  }
  Output = mAllData;
}

void
CCEPFilter::StopRun()
{
}

void
CCEPFilter::Halt()
{

}