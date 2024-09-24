////////////////////////////////////////////////////////////////////////////////
// $Id: IIRFilter_ML.cpp 7463 2023-06-30 14:59:17Z mellinger $
// Authors: juergen.mellinger@neurotechcenter.org
// Description: An IIR filter that behaves like Matlab's filter() function.
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
#include "IIRFilter_ML.h"
#include <vector>

RegisterFilter( IIRFilter_ML, 2.C1 );

struct IIRFilter_ML::Private
{
  std::vector<double> NumeratorB, DenominatorA;
  std::vector<std::vector<double>> Delays;
};


IIRFilter_ML::IIRFilter_ML()
  : p(new Private)
{
}

IIRFilter_ML::~IIRFilter_ML()
{
  delete p;
}

void
IIRFilter_ML::Publish()
{
 BEGIN_PARAMETER_DEFINITIONS
   "Filtering:IIRFilter_ML list NumeratorB= 1 1 % % // numerator coefficients",
   "Filtering:IIRFilter_ML list DenominatorA= 1 1 % % // denominator coefficients",
   "Filtering:IIRFilter_ML matrix DelaysZi= 0 0 % % % // filter delays, columns correspond to channels",
 END_PARAMETER_DEFINITIONS
}

void
IIRFilter_ML::Preflight(const SignalProperties& Input, SignalProperties& Output) const
{
  Output = Input;
  PreflightCondition(Parameter("DenominatorA")->NumValues() > 0);
  PreflightCondition(Parameter("DenominatorA")(0) != 0);
  int numElements = std::max(Parameter("DenominatorA")->NumValues(), Parameter("NumeratorB")->NumValues());
  if (Parameter("DelaysZi")->NumValues() != 0)
  {
    if (Parameter("DelaysZi")->NumColumns() != Input.Channels())
      bcierr << "If nonempty, DelaysZi->NumColumns() must match number of input channels";
    if (Parameter("DelaysZi")->NumRows() != numElements - 1)
      bcierr << "If nonempty, DelaysZi->NumRows() must match the larger of DenominatorA->NumValues() and NumeratorB->NumValues() minus 1";
  }
}

void
IIRFilter_ML::Initialize( const SignalProperties& Input, const SignalProperties& Output )
{
  int numElements = std::max(Parameter("DenominatorA")->NumValues(), Parameter("NumeratorB")->NumValues());
  ParamRef NumeratorB = Parameter("NumeratorB");
  p->NumeratorB.resize(numElements + 1);
  for (auto i = 0; i < NumeratorB->NumValues(); ++i)
    p->NumeratorB[i+1] = NumeratorB(i);
  for (auto i = NumeratorB->NumValues(); i < numElements; ++i)
    p->NumeratorB[i+1] = 0;
  ParamRef DenominatorA = Parameter("DenominatorA");
  p->DenominatorA.resize(numElements + 1);
  for (auto i = 0; i < DenominatorA->NumValues(); ++i)
    p->DenominatorA[i+1] = DenominatorA(i);
  for (auto i = DenominatorA->NumValues(); i < numElements; ++i)
    p->DenominatorA[i+1] = 0;
  auto a1 = p->DenominatorA[1];
  for (auto i = 1; i < numElements + 1; ++i)
  {
    p->DenominatorA[i] /= a1;
    p->NumeratorB[i] /= a1;
  }
  ParamRef DelaysZi = Parameter("DelaysZi");
  p->Delays.clear();
  p->Delays.resize(Input.Channels(), std::vector<double>(numElements));
  if (DelaysZi->NumColumns() == Input.Channels() && DelaysZi->NumRows() == numElements - 1)
    for (auto ch = 0; ch < Input.Channels(); ++ch)
    {
      p->Delays[ch][0] = 0;
      for (auto n = 1; n < p->Delays[ch].size(); ++n)
        p->Delays[ch][n] = DelaysZi(n - 1, ch);
    }
}

void
IIRFilter_ML::Process( const GenericSignal& Input, GenericSignal& Output )
{
  for (int ch = 0; ch < Input.Channels(); ++ch)
  {
    for (int s = 0; s < Input.Elements(); ++s)
    {
      double xm = Input(ch, s);
      double ym = p->NumeratorB[1] * xm + p->Delays[ch][1];
      for (int n = 1; n < p->Delays[ch].size() - 1; ++n)
        p->Delays[ch][n] = p->NumeratorB[n+1] * xm + p->Delays[ch][n+1] - p->DenominatorA[n+1] * ym;
      p->Delays[ch].back() = p->NumeratorB.back() * xm - p->DenominatorA.back() * ym;
      Output(ch, s) = ym;
    }
  }
}

void
IIRFilter_ML::StopRun()
{
  MutableParamRef DelaysZi = Parameter("DelaysZi");
  DelaysZi->SetNumColumns(p->Delays.size());
  DelaysZi->SetNumRows(p->Delays[0].size() - 1);
  for (auto ch = 0; ch < p->Delays.size(); ++ch)
    for (auto n = 1; n < p->Delays[ch].size(); ++n)
      DelaysZi(n - 1, ch) = p->Delays[ch][n];
}
