/////////////////////////////////////////////////////////////////////////////
// $Id: AudioExtension.h 7460 2023-06-29 17:21:57Z mellinger $
// Authors: griffin.milsap@gmail.com
// Description: The audio extension enables input and mixed output from/to 
//   sound cards on the machine. The extension also handles filtering of
//   audio input/output envelope to states
//
// Documentation located at: 
//   http://www.bci2000.org/wiki/index.php/Contributions:AudioExtension
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
/////////////////////////////////////////////////////////////////////////////
#ifndef AUDIO_EXTENSION_H
#define AUDIO_EXTENSION_H

#include <string>
#include <iostream>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <portaudio.h>
#include <sndfile.h>
#include "Environment.h"
#include "Thread.h"
#include "RandomGenerator.h"
#include "Expression/Expression.h"
#include "IIRFilter.h"
#include "WaitableEvent.h"


class AudioExtension : public EnvironmentExtension, public Thread
{
 public:
  // Constructors
  AudioExtension();
  virtual ~AudioExtension();

  // EnvironmentExtension interface
  void Publish() override;
  void Preflight() const override;
  void Initialize() override;
  void Process() override;
  void StartRun() override;
  void StopRun() override;
  
  // Thread interface
  int OnExecute() override;

 private:
  typedef std::vector< Expression > ExpressionVector;
  typedef std::vector< ExpressionVector > ExpressionMatrix;
  typedef std::vector< std::pair< std::string, int > > ChannelDef;
  typedef std::vector< IIRFilter< FilterDesign::Real > > FilterBank;
  ExpressionMatrix mMixer;
  std::vector< std::vector< float > > mGainMatrix;
  std::mutex mMixerMutex;
  
  // Internal configuration methods
  bool GetHostApiConfig( PaHostApiTypeId hostApi, const PaHostApiInfo* &apiInfo, PaHostApiIndex &hostIdx ) const;
  void GetDeviceConfig( const PaHostApiInfo* apiInfo, PaHostApiIndex hostIdx, 
    int &inputIdx, int &outputIdx, int &inputChannels, int &outputChannels ) const;
  void LoadMixer( const ParamRef &matrix, ExpressionMatrix &mixer,
    ChannelDef &channelDef, int audioIns, int audioOuts, int fileIns ) const;
  void EvaluateMixer( ExpressionMatrix &mixer, std::vector< std::vector< float > > &gainMatrix ) const;
  void DesignFilterbank( const ParamRef &matrix, IIRFilter< FilterDesign::Real > &filter, size_t numCh ) const;

  void OpenRecordingFiles();
  void CloseRecordingFiles();


  // Static audio callback
  static int AudioCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void *userData );
  
  // Member variables
  std::set< int >                    mSupportedAPIs;
  int                                mHostAPI;
  
  WaitableEvent                      mBufferProcessed;
  GenericSignal                      mAudioInputBlock,
                                     mAudioOutputBlock,
                                     mAudioInputEnvelope,
                                     mAudioOutputEnvelope;
  
  int64_t                            mFrameCount;
  unsigned int                       mFramesPerBuffer,
                                     mMaxInputEnvelopes,
                                     mMaxOutputEnvelopes;
  
  int                                mInputChannels,
                                     mOutputChannels,
                                     mFileChannels;
  
  ChannelDef                         mChannelDef;
  RandomGenerator                    mRand;
  
  PaStream                          *mpAudioStream;
  
  SNDFILE                           *mpAudioInputFile,
                                    *mpAudioRecInputFile,
                                    *mpAudioRecOutputFile;
  
  FilterBank                         mInputEnvelopeFilter,
                                     mOutputEnvelopeFilter;
  IIRFilter< FilterDesign::Real >    mInputFilter,
                                     mOutputFilter;

  int                                mRecordingFormat = 0,
                                     mFilePart = 0;
  std::string                        mFileBase, mFileExtension;
  bool                               mAudioRecordInput = false,
                                     mAudioRecordOutput = false;
  int64_t                            mFrameCountInFile = 0;
  std::recursive_mutex               mRecordingFilesMutex;
};

#endif // AUDIO_EXTENSION_H
