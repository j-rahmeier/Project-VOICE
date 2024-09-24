////////////////////////////////////////////////////////////////////////////////
// Authors: engelhardt@neurotechcenter.org
// Description: PegasusAstroFilter header
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_PEGASUSASTROFILTER_H  // makes sure this header is not included more than once
#define INCLUDED_PEGASUSASTROFILTER_H

#include "GenericFilter.h"
#include "Expression/Expression.h"
#include "PegasusAstroUSB.h"


class PegasusAstroFilter : public GenericFilter
{
 public:
  PegasusAstroFilter();
  ~PegasusAstroFilter();
  void Publish() override;
  void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
  void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
  void StartRun() override;
  void Process( const GenericSignal& Input, GenericSignal& Output ) override;
  void StopRun() override;
  void Halt() override;

 private:
   bool             mEnable,
                    mEmptyParam,
                    mPastStatus[6];

   char             mUSBNames[6],
                    mIndices[6] = { '1', '2', '3', '4', '5', '6' };

   int              mCounter;

   Expression       mTemp,
                    mActivate[6];

   std::string      mTempString,
                    mPastCOM,
                    mPortName;
   
   PegasusAstroUSB* mpDevice;
};

#endif // INCLUDED_PEGASUSASTROFILTER_H
