////////////////////////////////////////////////////////////////////
// $Id: bci_stream2hybrid.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: jezhill@gmail.com
// Description: See the ToolInfo definition below.
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
////////////////////////////////////////////////////////////////////
#include "BCIStream.h"
#include "Files.h"
#include "GenericVisualization.h"
#include "MessageChannel.h"
#include "Param.h"
#include "ParamList.h"
#include "State.h"
#include "StateList.h"
#include "StateVector.h"
#include "SysCommand.h"
#include "bci_tool.h"

#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <set>
#include <string>

std::string ToolInfo[] = {
  "bci_stream2hybrid",
  "Convert a binary BCI2000 stream into a machine- and mostly-human-readable text/binary hybrid format",
  "Reads a BCI2000 compliant binary stream from standard input, and writes it to stdout with plain text headers.",
  "binary",
  "",
};

class StreamRenderer : public MessageChannel
{   

  public:
    StreamRenderer( Streambuf &in, Streambuf &out )
    : MessageChannel( in ), mOutput( out ), mpStatevector( NULL ), mSignalProperties( 0, 0 ),
      mParamsDumped( false ), mDataBuffer( NULL ), mEventSamplesPerBlock( 0 ), mOriginalSamplesPerBlock( 0 ), mOriginalSamplesPerSecond( 0.0 ), mSecondsPerBlock( 0.0 ), mBlocksProcessed( 0 )
    {
      mEndianness = ( ( BYTE_ORDER == LITTLE_ENDIAN ) ? '<' : '>' );
    }
    ~StreamRenderer()
    {
      delete mpStatevector; mpStatevector = NULL;
      delete [] mDataBuffer; mDataBuffer = NULL;
      for( EventBufferMap::iterator it = mEventBuffers.begin(); it != mEventBuffers.end(); it++ ) delete[] it->second;
    }

  private:
    Tiny::OStream mOutput;

    std::stringstream mParamsOut;
    bool mParamsDumped;
    char mEndianness;
    SignalProperties mSignalProperties;
    StateVector *mpStatevector;
    typedef std::map<std::string, State::ValueType> StateValueMap;
    StateValueMap mStateValues;
    typedef std::map<std::string, State::ValueType *> EventBufferMap;
    EventBufferMap mEventBuffers;
    StateList mStatelist;
    float * mDataBuffer;
    int mEventSamplesPerBlock;
    int mOriginalSamplesPerBlock;
    double mOriginalSamplesPerSecond, mSecondsPerBlock;
    size_t mBlocksProcessed;

    std::ostream & Output() { return mOutput; }
    void WriteStringLiteral( const std::string &str );
    void WriteHeader( void );
    void FinishHeader( void );
    void WriteData( const GenericSignal & );

    bool OnState( std::istream & ) override;
    bool OnParam( std::istream & ) override;
    bool OnVisSignalProperties( std::istream & ) override;
    bool OnStateVector( std::istream & ) override;
    bool OnVisSignal( std::istream & ) override;
    bool OnSysCommand(std::istream &) override;
};

ToolResult ToolInit()
{
  return noError;
}

bool gDebug = false;
ToolResult ToolMain( Streambuf &in, Streambuf &out, OptionSet &arOptions )
{
  gDebug = ( arOptions.findopt( "-d|--debug" ) != 0 ); // TODO: not documented in the usage that appears...
  if( arOptions.size() > ( int )gDebug) return illegalOption; // ...when this happens
  StreamRenderer converter( in, out );
  do {} while ( converter.HandleMessage() );
  if( in.Input()->Failed() ) return illegalInput;
  return noError;
}

bool StreamRenderer::OnState( std::istream &arIn ) // called at the beginning, once for each state definition
{
  State s;
  s.Unserialize( arIn );
  if( !mStatelist.Size() ) bciout << "bci_stream2hybrid: CONFIGURATION ACCEPTED";
  if( arIn )
  {
    mStatelist.Delete( s.Name() );
    mStatelist.Add( s );

    mEventSamplesPerBlock = 0;
    mOriginalSamplesPerBlock = 0;
    mOriginalSamplesPerSecond = 0.0;
    if ( mpStatevector )
    {
      delete mpStatevector;
      mpStatevector = nullptr;
    }
  }
  return true;
}

bool StreamRenderer::OnParam( std::istream & arIn ) // called at the beginning, once for each parameter
{
  Param p;
  if( p.Unserialize( arIn ) )
  {
    if( mParamsDumped )
    {
      throw bcierr << "Additional parameter arrived after parameters were written";
    }
    else
    {
      mParamsOut << "Parameter\n" << p << "\n";
      if( p.Name() == "SampleBlockSize" ) { std::stringstream ss( p.Value() ); ss >> mOriginalSamplesPerBlock; } // neither p nor p.Value() appears to have any NumericValue() or similar conversion method
      else if( p.Name() == "SamplingRate" )
      {
        try { mOriginalSamplesPerSecond = PhysicalUnit().SetSymbol( "Hz" ).PhysicalToRaw( p.Value() ); } // in case the user has written "1kHz" or similar. But this will throw an exception if they haven't written "Hz" at all, so...
        catch( const std::exception & ) { std::stringstream ss(p.Value()); ss >> mOriginalSamplesPerSecond; }
      }
    }
  }
  return true;
}

bool StreamRenderer::OnVisSignalProperties( std::istream & arIn ) // called once at the beginning
{
  VisSignalProperties vsp;
  vsp.Unserialize( arIn );
  mSignalProperties = vsp.SignalProperties();
  WriteHeader();
  return true;
}

bool StreamRenderer::OnStateVector( std::istream & arIn ) // called on every sample block
{
  if (mpStatevector == NULL)
  {
    // mStatelist was populated by multiple OnState() calls 
    mpStatevector = new StateVector( mStatelist );
    mpStatevector->Unserialize( arIn );
    mEventSamplesPerBlock = mpStatevector->Samples() - 1;
    mStateValues.clear();
    mEventBuffers.clear();
    for( const auto& s : mStatelist )
    {
      if( s.Kind() == State::Padding || s.Name().empty() || !::isalpha( s.Name().front() ) ) continue;
      // Ideally we would like to create an event-buffer only for states whose Kind() == State::EventKind,
      // (each event-buffer would have length mOriginalSamplesPerBlock) and record scalar values for other states.
      // However, in a command-line tool, or indeed anywhere outside a SignalSource module, that comparison
      // always evaluates to false, and *all* states will be treated the same. The number of samples per
      // cycle will only be known when the first OnStateVector call comes in (although, as of r6624, it
      // *should* always be mOriginalSamplesPerBlock).
      // Without breaking backward-compatibility, there is no way for the stream protocol to convey a
      // state's Kind, and no way for a .dat file to record it.
      // So we will create a scalar state value *and* an event buffer for *every* state.
      mStateValues[ s.Name() ] = 0;
      mEventBuffers[ s.Name() ] = new State::ValueType[ mEventSamplesPerBlock ];
    }
    FinishHeader();
  }
  else
    mpStatevector->Unserialize( arIn );
  return true;
}

bool StreamRenderer::OnVisSignal( std::istream & arIn ) // called on every sample block
{
  VisSignal v;
  v.Unserialize( arIn );
  const GenericSignal &s = v;
  if( mSignalProperties.IsEmpty() )
    throw bcierr << "Internal error: HandleVisSignalProperties should have written the header already, but has not";
  if( s.Properties() != mSignalProperties )
    throw bcierr << "Ignored signal with inconsistent properties";
  WriteData( s );
  return true;
}

bool StreamRenderer::OnSysCommand( std::istream & arIn )
{
  SysCommand s;
  if( s.Unserialize( arIn ) )
  {
    if( s == SysCommand::EndOfParameter )
      ;
    else
      throw bcierr << "Unexpected SysCommand";
  }
  return true;
}

void StreamRenderer::WriteStringLiteral( const std::string & str )
{
  Output() << '"';
  for( const char * s = str.c_str(); *s; s++ )
  {
    if( *s == '\n' ) Output() << "\\n";
    else if( *s == '\t' ) Output() << "\\t";
    else if( !::isprint( *s ) ) Output() << "\\x" << std::setfill( '0' ) << std::setw( 2 ) << std::hex << std::uppercase << ( int )*s;
    else
    {
      if( *s == '"' || *s == '\\' )  Output() << "\\";
      Output() << *s;
    }
  }
  Output() << '"';
}

void StreamRenderer::WriteHeader( void )
{
  std::ostream& out = Output();

  out << "\n";
  out << mParamsOut.str() << "\n";
  mParamsDumped = true;

  int nChannels = mSignalProperties.Channels();
  int nElements = mSignalProperties.Elements();
  delete[] mDataBuffer;
  mDataBuffer = new float[ ( size_t )( nChannels ? nChannels : 1 ) * ( size_t )( nElements ? nElements : 1 ) ];

  mSecondsPerBlock = ( double )mOriginalSamplesPerBlock / mOriginalSamplesPerSecond;
  out << "SamplesPerBlock =\n\t" << mOriginalSamplesPerBlock << "\n";
  out << "SamplesPerSecond =\n\t" << mOriginalSamplesPerSecond << "\n";
  out << "States =\n\t{}\n";
  out << "Events =\n\t{}\n";

  out << "NumberOfChannels =\n\t" << nChannels << "\n";

  out << "ChannelLabels =\n\t[";
  for( int iChannel = 0; iChannel < nChannels; iChannel++ )
  {
    if( iChannel ) out << ", ";
    WriteStringLiteral( mSignalProperties.ChannelLabels()[ iChannel ] );
  }
  out << "]\n";

  out << "NumberOfElements =\n\t" << nElements << "\n";

  out << "ElementLabels =\n\t[";
  for( int iElement = 0; iElement < nElements; iElement++ )
  {
    if( iElement ) out << ", ";
    WriteStringLiteral( mSignalProperties.ElementLabels()[ iElement ] );
  }
  out << "]\n";

  out << "ElementValues " << mEndianness << nElements << "f :\n";
  for( int iElement = 0; iElement < nElements; iElement++ ) mDataBuffer[ iElement ] = static_cast< float >( ( iElement - mSignalProperties.ElementUnit().Offset() ) * mSignalProperties.ElementUnit().Gain() );
  if( !gDebug ) out.write( ( const char * )mDataBuffer, sizeof( *mDataBuffer ) * nElements );
  out << "\n";

  out << "ElementUnit =\n\t";
  WriteStringLiteral( mSignalProperties.ElementUnit().Symbol() );
  out << "\n";

  mBlocksProcessed = 0;
  out << "NumberOfBlocksProcessed =\n\t0\n\n"; // remove this line if you want to use FinishHeader()
}

void StreamRenderer::FinishHeader( void )
{
  // This method is left intentionally blank. Or commented out, whatever.
  // It is called the first time a state vector is received, and hence it
  // would be useful if you wanted the header to include the following info: 

  //Output() << "EventSamplesPerBlock =\n\t" << mEventSamplesPerBlock << "\n";

  // But unless we really want that piece of information (which we'll see the
  // first time an event update gets sent anyway), it's not really necessary
  // to split this off from WriteHeader(), and it's neater not to do so, because
  // then the edge-case of transmitting 0 blocks would still result in a
  // completed header---which in any event must end with:

  //Output() << "NumberOfBlocksProcessed =\n\t0\n\n";
}

void StreamRenderer::WriteData( const GenericSignal & s )
{
  std::ostream & out = Output();
  int nChannels = s.Channels();
  int nElements = s.Elements();
  bool isFirstBlock = ( mBlocksProcessed == 0 );
  

  if( mpStatevector == NULL )
  {
    // NB: a similar clause was present in bci_stream2mat.cpp, but this code never seems to get run
    throw bcierr << "mpStatevector is NULL";
    //for( StateValueMap::iterator it = mStateValues.begin(); it != mStateValues.end(); it++ ) it->second = 0; // mStateValues was composed during OnVisSignalProperties()
      
  }
  else
  {
    for( StateValueMap::iterator it = mStateValues.begin(); it != mStateValues.end(); it++ ) // mStateValues was composed during OnVisSignalProperties()
    {
        const std::string& stateName = it->first;
        State::ValueType oldValue = it->second;
        bool stateValueHasChanged = isFirstBlock;
        if( ( it->second = mpStatevector->StateValue( stateName ) ) != oldValue ) stateValueHasChanged = true;
        if( stateValueHasChanged ) out << "States " << stateName << " =\n\t" << it->second << "\n";
    }
    for( EventBufferMap::iterator it = mEventBuffers.begin(); it != mEventBuffers.end(); it++ ) // mEventBuffers was composed during OnVisSignalProperties()
    {
      const std::string & eventName = it->first;
      State::ValueType* eventBuffer = it->second;
      bool eventBufferHasChanged = isFirstBlock;
      for( int iSample = 0; iSample < mEventSamplesPerBlock; iSample++ )
      {
        State::ValueType oldValue = eventBuffer[ iSample ];
        if( ( eventBuffer[ iSample ] = mpStatevector->StateValue( eventName, iSample ) ) != oldValue ) eventBufferHasChanged = true;
      }
      if( eventBufferHasChanged )
      {
        out << "Events " << eventName << " " << mEndianness << mEventSamplesPerBlock << "L :\n"; // NB: here we are trusting that (a) State::ValueType has remained defined as uint32_t, and (b) 'L' mapping to uint32_t (e.g. in Python's struct.unpack) on the architecture we're using
        if( !gDebug ) out.write( ( const char * )eventBuffer, sizeof( *eventBuffer ) * mEventSamplesPerBlock );
        out << "\n";
      }
    }
  }
  bool signalHasChanged = isFirstBlock;
  float* packed = mDataBuffer;
  for( int iElement = 0; iElement < nElements; ++iElement )
  {
    for( int iChannel = 0; iChannel < nChannels; ++iChannel )
    {
      float oldValue = *packed;
      if( ( *packed++ = s( iChannel, iElement ) ) != oldValue ) signalHasChanged = true;
    }
  }
  if( signalHasChanged )
  {
    int nFloats = nChannels * nElements;
    out << "Signal " << mEndianness << nFloats << "f :\n";
    if( !gDebug ) out.write( ( const char * )mDataBuffer, sizeof( *mDataBuffer ) * nFloats );
    out << "\n";
  }
  out << "NumberOfBlocksProcessed =\n\t" << ++mBlocksProcessed << "\n\n";
}
