////////////////////////////////////////////////////////////////////////////////
// $Id: AmpServerProADC.h 7334 2023-04-30 15:51:03Z jhill $
// Description: A source module that interfaces withEGI's Amp Server Pro SDK.
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
#ifndef AmpServerProADCH
#define AmpServerProADCH

#include <sstream>
#include "BufferedADC.h"
#include "Expression/Expression.h"
#include "Clock.h"
#include "SockStream.h"
#include "NetworkPacketTypes.h"
#include <Win32Defs.h>
#include <tinyxml2.h>
#include <sstream>


// Message values for StimThread  (integer values >= 0 will indicate which stim train to upload):
#define START_STIM  -1
#define STOP_STIM   -2
#define EXIT_THREAD -3
#define DO_NOTHING  -4


class AmpServerProADC : public BufferedADC
{
public:
  AmpServerProADC();
  ~AmpServerProADC();

protected:
  void Publish() override;
  void OnHalt() override;
  void OnAutoConfig() override;
  void OnPreflight(SignalProperties&) const override;
  void OnInitialize(const SignalProperties&) override;
  void OnStartAcquisition() override;
  void OnStopAcquisition() override;
  void OnProcess() override;
  void DoAcquire(GenericSignal&) override;
  void StimThread();

private:
  void        ParseUploadExpressions( std::vector<Expression> & expressions, std::vector<std::string> & trains, int expectedNumberOfChannels=0 ) const;
  std::string ReadTrain( const std::string & fileName, const std::string & targetTrainID, int expectedNumberOfChannels ) const;
  void        SendMessageToStimThread( int messageValue );

  int                      mAmpState;
  bool                     mEnableStim;
  bool                     mStartRequested;
  bool                     mStimAlreadyRunning;
  int                      mUploadExpressionAlreadyFulfilled;
  bool                     mStartExpressionAlreadyFulfilled;
  bool                     mAbortExpressionAlreadyFulfilled;
  std::vector<Expression>  mUploadExpressions;
  std::vector<std::string> mTrains;
  int                      mUploadInProgress;
  int                      mUploadCompleted;
  bool                     mAutomaticTrigger;
  Expression               mStartExpression;
  Expression               mDelayExpression;
  Expression               mAbortExpression;
  std::thread              mStimThread;
  bool                     mStimThreadIsRunning;
  bool                     mKillThreads;
  int                      mThreadMessage;
  HANDLE                   mThreadLock;
  Clock                    mClock;
  int                      mMillisecondsPerSampleBlock;
  UINT16                   mStimDelayInMilliseconds;

  class Connection
  {

  public:
    Connection();
    Connection(AmpServerProADC* p);
    ~Connection();

    const std::string& Error() const { return mError; }
    bool   Open(const std::string& inIP, int inPort, int inAmpId);
    void   Close();

    const std::string& Address() const { return mAddress; }
    bool   IsOpen(int ampID) const;
    int    NotificationPort() const { return mNotificationPort; }
    int    StreamPort() const { return mStreamPort; }
    
    int    Amps() const { return mAmps; }
    int    Channels() const { return mChannels; }
    int    PacketType() const { return mPacketType; }
    double Gain_muV() const { return mGain_muV; }
    double SamplingRate() const { return mSamplingRate; }
    bool   StimEnabled() { return mStimulationEnabled; }
    bool   TrainUploaded() { return mTrainUploaded; }
    void   ForgetTrain() { mTrainUploaded = false; }

    bool   EnableStimulation(const std::string& file);
    bool   BeginStimulation();
    bool   EndStimulation();
    bool   SendCommand( const std::string & inCommand, int timeoutInMilliseconds=-1 );
    bool   SendCommand( const std::string & inCommand, std::map<std::string,std::string>& outputs, int timeoutInMilliseconds=-1 );
    bool   GetCommandResponse( const std::string & inCommand, std::map<std::string,std::string>& outputs, int timeoutInMilliseconds );
    bool   StartStreaming();
    bool   StopStreaming();
    bool   ReadData(GenericSignal&);
    bool   LoadGTENPattern(const std::string& name, int id);
    bool   UploadTrain(const std::string & trainXML);

  private:
    std::string BuildCommand(const std::string&);
    bool        ParseAmpDetails();
    bool        LoadXMLPattern(const std::string& file);
    bool        DoRead1(GenericSignal&);
    bool        DoRead2(GenericSignal&);
    bool        DoRead(GenericSignal&);
    void        NotificationThread();

    std::thread        mNotificationThread;
    bool               mKillThreads;
    AmpServerProADC *  parent;
    std::string        mError;
    int64_t            mSamplesInStream;
    int                mSamplesInOutput;
    bool               mFirstRead;
    bool               mStimulationEnabled;
    bool               mUseNotification;
    int                mNumberOfPendingUploads;
    bool               mTrainUploaded;
    int                mTimeout;
    int                mInitialTimeout;
    std::string        mAddress;
    std::string        mAmpType;
    int                mNotificationPort;
    int                mStreamPort;
    int                mAmpId;
    int                mAmps;
    int                mChannels;
    int                mPacketType;
    double             mGain_muV;
    double             mSamplingRate;
    sockstream         mCommands;
    sockstream         mStream;
    sockstream         mNotifications;
    client_tcpsocket   mCommandSocket;
    client_tcpsocket   mStreamSocket;
    client_tcpsocket   mNotificationSocket;
    std::istringstream mStringstream;
  } mConnection;
};

#endif // AmpServerProADCH

