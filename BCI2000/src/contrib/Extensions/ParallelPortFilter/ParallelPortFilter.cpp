////////////////////////////////////////////////////////////////////////////////
// $Id: ParallelPortFilter.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Author: kaleb.goering@gmail.com, juergen.mellinger@uni-tuebingen.de
// Description: A filter which controls parallel ports
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
#include "ParallelPortFilter.h"

#include "BCIStream.h"
#include "Expression.h"

RegisterFilter( ParallelPortFilter, 3.B );

struct LPTPort {
    ParallelPort::Ptr p;
    char value;
    int location;
};

struct ParallelPortFilter::Private
{
  ParallelPort::List mPorts;
  std::vector< std::pair<Expression, ParallelPort::Ptr> > mOutputs;
  std::map<std::string, LPTPort> mOutputMap;
};

ParallelPortFilter::ParallelPortFilter()
: p( new Private ),
  bitMode( false )
{
}

ParallelPortFilter::~ParallelPortFilter()
{
  delete p;
}

void
ParallelPortFilter::Publish()
{
  BEGIN_PARAMETER_DEFINITIONS
    "ParallelPort:Output int OutputMode= 1 1 1 2 "
      " // ParallelPortOutput cell interpret mode: 1 Word, 2 Bit (enumeration)",
    "ParallelPort:Output matrix ParallelPortOutput= 0 {Device%20Name Expression} % % %"
      " // Matrix of Expressions for parallel port output",
  END_PARAMETER_DEFINITIONS
}

void
ParallelPortFilter::AutoConfig( const SignalProperties& )
{
  p->mOutputs.clear();
  p->mPorts.clear();
  p->mOutputMap.clear();

  ParallelPort::List portsDetected;
  if( ParallelPort::Enumerate( portsDetected ) )
  {
    if( portsDetected.empty() )
      bciout << "No parallel ports detected.";
    else
    {
      std::ostringstream ports;
      for( const auto& p : portsDetected )
        ports << *p << "\n";
      bciout << "Parallel ports detected:\n" << ports.str();
    }
  }

  if (Parameter ("OutputMode") == 1)
      bitMode = false;
  else
      bitMode = true;
   
  ParamRef ParallelPortOutput = Parameter( "ParallelPortOutput" );
  if( ParallelPortOutput->NumRows() > 0 && ParallelPortOutput->NumColumns() != 2 )
    throw bcierr << "Expected two columns in ParallelPortOutput matrix: Device name, and Expression";

  std::vector<std::string> missingPorts;
  for( int i = 0; i < ParallelPortOutput->NumRows(); ++i )
  {
    std::string portName = ParallelPortOutput( i, 0 );
    ParallelPort::Ptr port = portsDetected.Find( portName );
    if( !port )
      missingPorts.push_back( portName );
    else
      p->mPorts.push_back( port );
  }
  for( const auto& portName : missingPorts )
    bcierr << "Parallel port device \"" << portName << "\" is not present in the system";
  for (auto& port : p->mPorts) {
    port->Open ();
    LPTPort temp_port;
    temp_port.location = 0;
    temp_port.value = 0x00;
    temp_port.p = port;
    p->mOutputMap[port->Name ()] = temp_port;
  }
}

void
ParallelPortFilter::Preflight( const SignalProperties &Input, SignalProperties &Output ) const
{
  GenericSignal preflightSignal( Input );
  ParamRef ParallelPortOutput = Parameter( "ParallelPortOutput" );
  for( int i = 0; i < ParallelPortOutput->NumRows(); ++i )
    Expression( ParallelPortOutput( i, 1 ) ).Evaluate( &preflightSignal );
  Output = Input;
}

void
ParallelPortFilter::Initialize( const SignalProperties &Input, const SignalProperties &Output )
{
  ParamRef ParallelPortOutput = Parameter( "ParallelPortOutput" );
  for( int i = 0; i < ParallelPortOutput->NumRows(); ++i )
  {
    std::string portName = ParallelPortOutput( i, 0 );
    auto port = p->mPorts.Find( portName );
    port->Write( ParallelPort::DCR, 0x00 ); // output mode
    p->mOutputs.push_back( std::make_pair( Expression( ParallelPortOutput( i, 1 ) ), port ) );
    p->mOutputs.back().first.Compile();
  }
}

void
ParallelPortFilter::StartRun()
{
  for( auto& output : p->mOutputs )
    output.second->Write( ParallelPort::Data, 0 );
}

void
ParallelPortFilter::StopRun()
{
  for( auto& output : p->mOutputs )
    output.second->Write( ParallelPort::Data, 0 );
}

void
ParallelPortFilter::Process( const GenericSignal &Input, GenericSignal &Output )
{
  if (bitMode)
  {
    for (auto& output : p->mOutputs)
    {
      auto output_itr = p->mOutputMap.find (output.second->Name ());
      output_itr->second.value += (((char)(output.first.Evaluate (&Input))) << output_itr->second.location);
      output_itr->second.location += 1;        
    }
    for (auto& output : p->mOutputMap)
    {
      output.second.p->Write (ParallelPort::Data, output.second.value);
      output.second.value = 0;
      output.second.location = 0;
    }
  }
  else 
  {
    for (auto& output : p->mOutputs)
      output.second->Write (ParallelPort::Data, output.first.Evaluate (&Input));
  }
  Output = Input;
}

