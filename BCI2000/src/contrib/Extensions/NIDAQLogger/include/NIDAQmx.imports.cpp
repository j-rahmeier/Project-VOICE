// Import definitions for nicaiu library
// generated Wed Nov  3 17:09:51 2021 by DylibTool
#ifdef STATIC_LIBNICAIU

namespace Dylib { bool nicaiu_Loaded() { return true; } }


#else // STATIC_LIBNICAIU

#include "NIDAQmx.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( nicaiu, "NIDAQmx", functionImports, notFoundMsg, notFoundURL );

decltype(&__18217c42_DAQmxAOSeriesCalAdjust) DAQmxAOSeriesCalAdjust = 0;
decltype(&__655f003c_DAQmxAddCDAQSyncConnection) DAQmxAddCDAQSyncConnection = 0;
decltype(&__7f3a0b24_DAQmxAddGlobalChansToTask) DAQmxAddGlobalChansToTask = 0;
decltype(&__1d614359_DAQmxAddNetworkDevice) DAQmxAddNetworkDevice = 0;
decltype(&__192f7375_DAQmxAdjust1102Cal) DAQmxAdjust1102Cal = 0;
decltype(&__192f7575_DAQmxAdjust1104Cal) DAQmxAdjust1104Cal = 0;
decltype(&__192e7375_DAQmxAdjust1112Cal) DAQmxAdjust1112Cal = 0;
decltype(&__192d7375_DAQmxAdjust1122Cal) DAQmxAdjust1122Cal = 0;
decltype(&__192d7575_DAQmxAdjust1124Cal) DAQmxAdjust1124Cal = 0;
decltype(&__192d7475_DAQmxAdjust1125Cal) DAQmxAdjust1125Cal = 0;
decltype(&__192d7775_DAQmxAdjust1126Cal) DAQmxAdjust1126Cal = 0;
decltype(&__192b7075_DAQmxAdjust1141Cal) DAQmxAdjust1141Cal = 0;
decltype(&__192b7375_DAQmxAdjust1142Cal) DAQmxAdjust1142Cal = 0;
decltype(&__192b7275_DAQmxAdjust1143Cal) DAQmxAdjust1143Cal = 0;
decltype(&__3b241d07_DAQmxAdjust11601Cal) DAQmxAdjust11601Cal = 0;
decltype(&__3b241d05_DAQmxAdjust11603Cal) DAQmxAdjust11603Cal = 0;
decltype(&__3b241d03_DAQmxAdjust11605Cal) DAQmxAdjust11605Cal = 0;
decltype(&__3b241c05_DAQmxAdjust11613Cal) DAQmxAdjust11613Cal = 0;
decltype(&__3b241c02_DAQmxAdjust11614Cal) DAQmxAdjust11614Cal = 0;
decltype(&__3b241e02_DAQmxAdjust11634Cal) DAQmxAdjust11634Cal = 0;
decltype(&__3b241e01_DAQmxAdjust11637Cal) DAQmxAdjust11637Cal = 0;
decltype(&__1d2f7375_DAQmxAdjust1502Cal) DAQmxAdjust1502Cal = 0;
decltype(&__1d2f7275_DAQmxAdjust1503Cal) DAQmxAdjust1503Cal = 0;
decltype(&__0b454371_DAQmxAdjust1503CurrentCal) DAQmxAdjust1503CurrentCal = 0;
decltype(&__1d2d7175_DAQmxAdjust1520Cal) DAQmxAdjust1520Cal = 0;
decltype(&__1d2d7075_DAQmxAdjust1521Cal) DAQmxAdjust1521Cal = 0;
decltype(&__1d2c3975_DAQmxAdjust153xCal) DAQmxAdjust153xCal = 0;
decltype(&__1d2b7175_DAQmxAdjust1540Cal) DAQmxAdjust1540Cal = 0;
decltype(&__1a2f7570_DAQmxAdjust4204Cal) DAQmxAdjust4204Cal = 0;
decltype(&__1a2d7170_DAQmxAdjust4220Cal) DAQmxAdjust4220Cal = 0;
decltype(&__1a2d7570_DAQmxAdjust4224Cal) DAQmxAdjust4224Cal = 0;
decltype(&__1a2d7470_DAQmxAdjust4225Cal) DAQmxAdjust4225Cal = 0;
decltype(&__1b2f7170_DAQmxAdjust4300Cal) DAQmxAdjust4300Cal = 0;
decltype(&__1b2f7370_DAQmxAdjust4302Cal) DAQmxAdjust4302Cal = 0;
decltype(&__1b2f7270_DAQmxAdjust4303Cal) DAQmxAdjust4303Cal = 0;
decltype(&__1b2f7570_DAQmxAdjust4304Cal) DAQmxAdjust4304Cal = 0;
decltype(&__1b2f7470_DAQmxAdjust4305Cal) DAQmxAdjust4305Cal = 0;
decltype(&__1b2f7870_DAQmxAdjust4309Cal) DAQmxAdjust4309Cal = 0;
decltype(&__1b2e7170_DAQmxAdjust4310Cal) DAQmxAdjust4310Cal = 0;
decltype(&__1b2d7370_DAQmxAdjust4322Cal) DAQmxAdjust4322Cal = 0;
decltype(&__1b2c7870_DAQmxAdjust4339Cal) DAQmxAdjust4339Cal = 0;
decltype(&__1b2c3970_DAQmxAdjust433xCal) DAQmxAdjust433xCal = 0;
decltype(&__1b2a7270_DAQmxAdjust4353Cal) DAQmxAdjust4353Cal = 0;
decltype(&__1b2a7670_DAQmxAdjust4357Cal) DAQmxAdjust4357Cal = 0;
decltype(&__1c297270_DAQmxAdjust4463Cal) DAQmxAdjust4463Cal = 0;
decltype(&__1e2e7170_DAQmxAdjust4610Cal) DAQmxAdjust4610Cal = 0;
decltype(&__1a2f707d_DAQmxAdjust9201Cal) DAQmxAdjust9201Cal = 0;
decltype(&__1a2f737d_DAQmxAdjust9202Cal) DAQmxAdjust9202Cal = 0;
decltype(&__7b461c3a_DAQmxAdjust9203GainCal) DAQmxAdjust9203GainCal = 0;
decltype(&__69666078_DAQmxAdjust9203OffsetCal) DAQmxAdjust9203OffsetCal = 0;
decltype(&__1a2f747d_DAQmxAdjust9205Cal) DAQmxAdjust9205Cal = 0;
decltype(&__1a2f777d_DAQmxAdjust9206Cal) DAQmxAdjust9206Cal = 0;
decltype(&__7b46183a_DAQmxAdjust9207GainCal) DAQmxAdjust9207GainCal = 0;
decltype(&__69666478_DAQmxAdjust9207OffsetCal) DAQmxAdjust9207OffsetCal = 0;
decltype(&__7b46173a_DAQmxAdjust9208GainCal) DAQmxAdjust9208GainCal = 0;
decltype(&__69666b78_DAQmxAdjust9208OffsetCal) DAQmxAdjust9208OffsetCal = 0;
decltype(&__7b46163a_DAQmxAdjust9209GainCal) DAQmxAdjust9209GainCal = 0;
decltype(&__69666a78_DAQmxAdjust9209OffsetCal) DAQmxAdjust9209OffsetCal = 0;
decltype(&__1a2e717d_DAQmxAdjust9210Cal) DAQmxAdjust9210Cal = 0;
decltype(&__1a2e707d_DAQmxAdjust9211Cal) DAQmxAdjust9211Cal = 0;
decltype(&__1a2e737d_DAQmxAdjust9212Cal) DAQmxAdjust9212Cal = 0;
decltype(&__1a2e727d_DAQmxAdjust9213Cal) DAQmxAdjust9213Cal = 0;
decltype(&__1a2e757d_DAQmxAdjust9214Cal) DAQmxAdjust9214Cal = 0;
decltype(&__1a2e747d_DAQmxAdjust9215Cal) DAQmxAdjust9215Cal = 0;
decltype(&__1a2e777d_DAQmxAdjust9216Cal) DAQmxAdjust9216Cal = 0;
decltype(&__1a2e767d_DAQmxAdjust9217Cal) DAQmxAdjust9217Cal = 0;
decltype(&__1a2e797d_DAQmxAdjust9218Cal) DAQmxAdjust9218Cal = 0;
decltype(&__1a2e787d_DAQmxAdjust9219Cal) DAQmxAdjust9219Cal = 0;
decltype(&__1a2d717d_DAQmxAdjust9220Cal) DAQmxAdjust9220Cal = 0;
decltype(&__1a2d707d_DAQmxAdjust9221Cal) DAQmxAdjust9221Cal = 0;
decltype(&__1a2d737d_DAQmxAdjust9222Cal) DAQmxAdjust9222Cal = 0;
decltype(&__1a2d727d_DAQmxAdjust9223Cal) DAQmxAdjust9223Cal = 0;
decltype(&__1a2d757d_DAQmxAdjust9224Cal) DAQmxAdjust9224Cal = 0;
decltype(&__1a2d747d_DAQmxAdjust9225Cal) DAQmxAdjust9225Cal = 0;
decltype(&__1a2d777d_DAQmxAdjust9226Cal) DAQmxAdjust9226Cal = 0;
decltype(&__1a2d767d_DAQmxAdjust9227Cal) DAQmxAdjust9227Cal = 0;
decltype(&__1a2d797d_DAQmxAdjust9228Cal) DAQmxAdjust9228Cal = 0;
decltype(&__1a2d787d_DAQmxAdjust9229Cal) DAQmxAdjust9229Cal = 0;
decltype(&__1a2c717d_DAQmxAdjust9230Cal) DAQmxAdjust9230Cal = 0;
decltype(&__1a2c707d_DAQmxAdjust9231Cal) DAQmxAdjust9231Cal = 0;
decltype(&__1a2c737d_DAQmxAdjust9232Cal) DAQmxAdjust9232Cal = 0;
decltype(&__7b451b3a_DAQmxAdjust9234GainCal) DAQmxAdjust9234GainCal = 0;
decltype(&__69656778_DAQmxAdjust9234OffsetCal) DAQmxAdjust9234OffsetCal = 0;
decltype(&__1a2c797d_DAQmxAdjust9238Cal) DAQmxAdjust9238Cal = 0;
decltype(&__1a2c787d_DAQmxAdjust9239Cal) DAQmxAdjust9239Cal = 0;
decltype(&__1a2b737d_DAQmxAdjust9242Cal) DAQmxAdjust9242Cal = 0;
decltype(&__1a2b757d_DAQmxAdjust9244Cal) DAQmxAdjust9244Cal = 0;
decltype(&__1a2b777d_DAQmxAdjust9246Cal) DAQmxAdjust9246Cal = 0;
decltype(&__1a2b767d_DAQmxAdjust9247Cal) DAQmxAdjust9247Cal = 0;
decltype(&__1a2a717d_DAQmxAdjust9250Cal) DAQmxAdjust9250Cal = 0;
decltype(&__1a2a707d_DAQmxAdjust9251Cal) DAQmxAdjust9251Cal = 0;
decltype(&__1a2a737d_DAQmxAdjust9252Cal) DAQmxAdjust9252Cal = 0;
decltype(&__1a2a727d_DAQmxAdjust9253Cal) DAQmxAdjust9253Cal = 0;
decltype(&__1a29717d_DAQmxAdjust9260Cal) DAQmxAdjust9260Cal = 0;
decltype(&__1a29737d_DAQmxAdjust9262Cal) DAQmxAdjust9262Cal = 0;
decltype(&__1a29727d_DAQmxAdjust9263Cal) DAQmxAdjust9263Cal = 0;
decltype(&__1a29757d_DAQmxAdjust9264Cal) DAQmxAdjust9264Cal = 0;
decltype(&__1a29747d_DAQmxAdjust9265Cal) DAQmxAdjust9265Cal = 0;
decltype(&__1a29777d_DAQmxAdjust9266Cal) DAQmxAdjust9266Cal = 0;
decltype(&__1a29787d_DAQmxAdjust9269Cal) DAQmxAdjust9269Cal = 0;
decltype(&__36021813_DAQmxAdjust9628AICal) DAQmxAdjust9628AICal = 0;
decltype(&__30021813_DAQmxAdjust9628AOCal) DAQmxAdjust9628AOCal = 0;
decltype(&__36021913_DAQmxAdjust9629AICal) DAQmxAdjust9629AICal = 0;
decltype(&__30021913_DAQmxAdjust9629AOCal) DAQmxAdjust9629AOCal = 0;
decltype(&__36031813_DAQmxAdjust9638AICal) DAQmxAdjust9638AICal = 0;
decltype(&__30031813_DAQmxAdjust9638AOCal) DAQmxAdjust9638AOCal = 0;
decltype(&__1f28747d_DAQmxAdjust9775Cal) DAQmxAdjust9775Cal = 0;
decltype(&__59536c0a_DAQmxAdjustDSAAICal) DAQmxAdjustDSAAICal = 0;
decltype(&__21536c4f_DAQmxAdjustDSAAICalEx) DAQmxAdjustDSAAICalEx = 0;
decltype(&__243d455d_DAQmxAdjustDSAAICalWithGainAndCoupling) DAQmxAdjustDSAAICalWithGainAndCoupling = 0;
decltype(&__59536c0c_DAQmxAdjustDSAAOCal) DAQmxAdjustDSAAOCal = 0;
decltype(&__6f5b720c_DAQmxAdjustDSAAOTimebaseCal) DAQmxAdjustDSAAOTimebaseCal = 0;
decltype(&__6832342a_DAQmxAdjustDSATimebaseCal) DAQmxAdjustDSATimebaseCal = 0;
decltype(&__723c343a_DAQmxAdjustTIOTimebaseCal) DAQmxAdjustTIOTimebaseCal = 0;
decltype(&__5d526906_DAQmxAreConfiguredCDAQSyncPortsDisconnected) DAQmxAreConfiguredCDAQSyncPortsDisconnected = 0;
decltype(&__017a755d_DAQmxAutoConfigureCDAQSyncConnections) DAQmxAutoConfigureCDAQSyncConnections = 0;
decltype(&__1112475d_DAQmxCSeriesSetCalTemp) DAQmxCSeriesSetCalTemp = 0;
decltype(&__27027c7f_DAQmxCalculateReversePolyCoeff) DAQmxCalculateReversePolyCoeff = 0;
decltype(&__18043d5c_DAQmxCfgAnlgEdgeRefTrig) DAQmxCfgAnlgEdgeRefTrig = 0;
decltype(&__78282f13_DAQmxCfgAnlgEdgeStartTrig) DAQmxCfgAnlgEdgeStartTrig = 0;
decltype(&__687c5f7f_DAQmxCfgAnlgMultiEdgeRefTrig) DAQmxCfgAnlgMultiEdgeRefTrig = 0;
decltype(&__271c736d_DAQmxCfgAnlgMultiEdgeStartTrig) DAQmxCfgAnlgMultiEdgeStartTrig = 0;
decltype(&__44261505_DAQmxCfgAnlgWindowRefTrig) DAQmxCfgAnlgWindowRefTrig = 0;
decltype(&__56697529_DAQmxCfgAnlgWindowStartTrig) DAQmxCfgAnlgWindowStartTrig = 0;
decltype(&__67281717_DAQmxCfgBurstHandshakingTimingExportClock) DAQmxCfgBurstHandshakingTimingExportClock = 0;
decltype(&__67281b02_DAQmxCfgBurstHandshakingTimingImportClock) DAQmxCfgBurstHandshakingTimingImportClock = 0;
decltype(&__0b68764f_DAQmxCfgChangeDetectionTiming) DAQmxCfgChangeDetectionTiming = 0;
decltype(&__111d617c_DAQmxCfgDigEdgeAdvTrig) DAQmxCfgDigEdgeAdvTrig = 0;
decltype(&__100d616f_DAQmxCfgDigEdgeRefTrig) DAQmxCfgDigEdgeRefTrig = 0;
decltype(&__3c1f2e0f_DAQmxCfgDigEdgeStartTrig) DAQmxCfgDigEdgeStartTrig = 0;
decltype(&__6a250433_DAQmxCfgDigPatternRefTrig) DAQmxCfgDigPatternRefTrig = 0;
decltype(&__786a641f_DAQmxCfgDigPatternStartTrig) DAQmxCfgDigPatternStartTrig = 0;
decltype(&__6008363f_DAQmxCfgHandshakingTiming) DAQmxCfgHandshakingTiming = 0;
decltype(&__2602437b_DAQmxCfgImplicitTiming) DAQmxCfgImplicitTiming = 0;
decltype(&__674b4019_DAQmxCfgInputBuffer) DAQmxCfgInputBuffer = 0;
decltype(&__6065647d_DAQmxCfgOutputBuffer) DAQmxCfgOutputBuffer = 0;
decltype(&__2e1c4965_DAQmxCfgPipelinedSampClkTiming) DAQmxCfgPipelinedSampClkTiming = 0;
decltype(&__22625840_DAQmxCfgSampClkTiming) DAQmxCfgSampClkTiming = 0;
decltype(&__284b4974_DAQmxCfgTimeStartTrig) DAQmxCfgTimeStartTrig = 0;
decltype(&__35406f40_DAQmxCfgWatchdogAOExpirStates) DAQmxCfgWatchdogAOExpirStates = 0;
decltype(&__37406f40_DAQmxCfgWatchdogCOExpirStates) DAQmxCfgWatchdogCOExpirStates = 0;
decltype(&__30406f40_DAQmxCfgWatchdogDOExpirStates) DAQmxCfgWatchdogDOExpirStates = 0;
decltype(&__51361129_DAQmxChangeExtCalPassword) DAQmxChangeExtCalPassword = 0;
decltype(&__1923694d_DAQmxClearTEDS) DAQmxClearTEDS = 0;
decltype(&__2e1b6969_DAQmxClearTask) DAQmxClearTask = 0;
decltype(&__3b241916_DAQmxCloseExtCal) DAQmxCloseExtCal = 0;
decltype(&__28697c77_DAQmxConfigureLogging) DAQmxConfigureLogging = 0;
decltype(&__6c5d0d33_DAQmxConfigureTEDS) DAQmxConfigureTEDS = 0;
decltype(&__2b171120_DAQmxConnectSCExpressCalAccChans) DAQmxConnectSCExpressCalAccChans = 0;
decltype(&__75022f1a_DAQmxConnectTerms) DAQmxConnectTerms = 0;
decltype(&__23143900_DAQmxControlWatchdogTask) DAQmxControlWatchdogTask = 0;
decltype(&__6c761d5e_DAQmxCreateAIAccel4WireDCVoltageChan) DAQmxCreateAIAccel4WireDCVoltageChan = 0;
decltype(&__10356642_DAQmxCreateAIAccelChan) DAQmxCreateAIAccelChan = 0;
decltype(&__53416049_DAQmxCreateAIAccelChargeChan) DAQmxCreateAIAccelChargeChan = 0;
decltype(&__18323f63_DAQmxCreateAIBridgeChan) DAQmxCreateAIBridgeChan = 0;
decltype(&__0e33256b_DAQmxCreateAIChargeChan) DAQmxCreateAIChargeChan = 0;
decltype(&__25383c21_DAQmxCreateAICurrentChan) DAQmxCreateAICurrentChan = 0;
decltype(&__5c7c600c_DAQmxCreateAICurrentRMSChan) DAQmxCreateAICurrentRMSChan = 0;
decltype(&__705d5828_DAQmxCreateAIDeviceTempChan) DAQmxCreateAIDeviceTempChan = 0;
decltype(&__20334e54_DAQmxCreateAIForceBridgePolynomialChan) DAQmxCreateAIForceBridgePolynomialChan = 0;
decltype(&__41130621_DAQmxCreateAIForceBridgeTableChan) DAQmxCreateAIForceBridgeTableChan = 0;
decltype(&__01270d4b_DAQmxCreateAIForceBridgeTwoPointLinChan) DAQmxCreateAIForceBridgeTwoPointLinChan = 0;
decltype(&__467e2316_DAQmxCreateAIForceIEPEChan) DAQmxCreateAIForceIEPEChan = 0;
decltype(&__526f5d4b_DAQmxCreateAIFreqVoltageChan) DAQmxCreateAIFreqVoltageChan = 0;
decltype(&__615b5401_DAQmxCreateAIMicrophoneChan) DAQmxCreateAIMicrophoneChan = 0;
decltype(&__1974574a_DAQmxCreateAIPosEddyCurrProxProbeChan) DAQmxCreateAIPosEddyCurrProxProbeChan = 0;
decltype(&__1b180c00_DAQmxCreateAIPosLVDTChan) DAQmxCreateAIPosLVDTChan = 0;
decltype(&__05180c00_DAQmxCreateAIPosRVDTChan) DAQmxCreateAIPosRVDTChan = 0;
decltype(&__64322e09_DAQmxCreateAIPressureBridgePolynomialChan) DAQmxCreateAIPressureBridgePolynomialChan = 0;
decltype(&__447a5b68_DAQmxCreateAIPressureBridgeTableChan) DAQmxCreateAIPressureBridgeTableChan = 0;
decltype(&__70713128_DAQmxCreateAIPressureBridgeTwoPointLinChan) DAQmxCreateAIPressureBridgeTwoPointLinChan = 0;
decltype(&__574c7363_DAQmxCreateAIRTDChan) DAQmxCreateAIRTDChan = 0;
decltype(&__7b555c18_DAQmxCreateAIResistanceChan) DAQmxCreateAIResistanceChan = 0;
decltype(&__4d412b37_DAQmxCreateAIRosetteStrainGageChan) DAQmxCreateAIRosetteStrainGageChan = 0;
decltype(&__7c4a573f_DAQmxCreateAIStrainGageChan) DAQmxCreateAIStrainGageChan = 0;
decltype(&__556d3238_DAQmxCreateAITempBuiltInSensorChan) DAQmxCreateAITempBuiltInSensorChan = 0;
decltype(&__3a293f39_DAQmxCreateAIThrmcplChan) DAQmxCreateAIThrmcplChan = 0;
decltype(&__735c4327_DAQmxCreateAIThrmstrChanIex) DAQmxCreateAIThrmstrChanIex = 0;
decltype(&__6c5c4327_DAQmxCreateAIThrmstrChanVex) DAQmxCreateAIThrmstrChanVex = 0;
decltype(&__0a002a5c_DAQmxCreateAITorqueBridgePolynomialChan) DAQmxCreateAITorqueBridgePolynomialChan = 0;
decltype(&__7f610a14_DAQmxCreateAITorqueBridgeTableChan) DAQmxCreateAITorqueBridgeTableChan = 0;
decltype(&__15213e1f_DAQmxCreateAITorqueBridgeTwoPointLinChan) DAQmxCreateAITorqueBridgeTwoPointLinChan = 0;
decltype(&__29496760_DAQmxCreateAIVelocityIEPEChan) DAQmxCreateAIVelocityIEPEChan = 0;
decltype(&__23292f2e_DAQmxCreateAIVoltageChan) DAQmxCreateAIVoltageChan = 0;
decltype(&__4538382f_DAQmxCreateAIVoltageChanWithExcit) DAQmxCreateAIVoltageChanWithExcit = 0;
decltype(&__5a6d7303_DAQmxCreateAIVoltageRMSChan) DAQmxCreateAIVoltageRMSChan = 0;
decltype(&__23383c21_DAQmxCreateAOCurrentChan) DAQmxCreateAOCurrentChan = 0;
decltype(&__321f3727_DAQmxCreateAOFuncGenChan) DAQmxCreateAOFuncGenChan = 0;
decltype(&__25292f2e_DAQmxCreateAOVoltageChan) DAQmxCreateAOVoltageChan = 0;
decltype(&__5d5d5700_DAQmxCreateCIAngEncoderChan) DAQmxCreateCIAngEncoderChan = 0;
decltype(&__62535154_DAQmxCreateCIAngVelocityChan) DAQmxCreateCIAngVelocityChan = 0;
decltype(&__75457119_DAQmxCreateCICountEdgesChan) DAQmxCreateCICountEdgesChan = 0;
decltype(&__607a0934_DAQmxCreateCIDutyCycleChan) DAQmxCreateCIDutyCycleChan = 0;
decltype(&__0b735c4f_DAQmxCreateCIFreqChan) DAQmxCreateCIFreqChan = 0;
decltype(&__2d6f7271_DAQmxCreateCIGPSTimestampChan) DAQmxCreateCIGPSTimestampChan = 0;
decltype(&__5d505009_DAQmxCreateCILinEncoderChan) DAQmxCreateCILinEncoderChan = 0;
decltype(&__625e565d_DAQmxCreateCILinVelocityChan) DAQmxCreateCILinVelocityChan = 0;
decltype(&__1528297a_DAQmxCreateCIPeriodChan) DAQmxCreateCIPeriodChan = 0;
decltype(&__635c363d_DAQmxCreateCIPulseChanFreq) DAQmxCreateCIPulseChanFreq = 0;
decltype(&__65465726_DAQmxCreateCIPulseChanTicks) DAQmxCreateCIPulseChanTicks = 0;
decltype(&__6b482426_DAQmxCreateCIPulseChanTime) DAQmxCreateCIPulseChanTime = 0;
decltype(&__6b56620d_DAQmxCreateCIPulseWidthChan) DAQmxCreateCIPulseWidthChan = 0;
decltype(&__7c7b4c17_DAQmxCreateCISemiPeriodChan) DAQmxCreateCISemiPeriodChan = 0;
decltype(&__6a424802_DAQmxCreateCITwoEdgeSepChan) DAQmxCreateCITwoEdgeSepChan = 0;
decltype(&__655c363d_DAQmxCreateCOPulseChanFreq) DAQmxCreateCOPulseChanFreq = 0;
decltype(&__63465726_DAQmxCreateCOPulseChanTicks) DAQmxCreateCOPulseChanTicks = 0;
decltype(&__6d482426_DAQmxCreateCOPulseChanTime) DAQmxCreateCOPulseChanTime = 0;
decltype(&__7a352e2d_DAQmxCreateDIChan) DAQmxCreateDIChan = 0;
decltype(&__7c352e2d_DAQmxCreateDOChan) DAQmxCreateDOChan = 0;
decltype(&__55747027_DAQmxCreateLinScale) DAQmxCreateLinScale = 0;
decltype(&__5d6a7026_DAQmxCreateMapScale) DAQmxCreateMapScale = 0;
decltype(&__5d413536_DAQmxCreatePolynomialScale) DAQmxCreatePolynomialScale = 0;
decltype(&__55713516_DAQmxCreateTEDSAIAccelChan) DAQmxCreateTEDSAIAccelChan = 0;
decltype(&__5d766c37_DAQmxCreateTEDSAIBridgeChan) DAQmxCreateTEDSAIBridgeChan = 0;
decltype(&__607c6f75_DAQmxCreateTEDSAICurrentChan) DAQmxCreateTEDSAICurrentChan = 0;
decltype(&__181d3e16_DAQmxCreateTEDSAIForceBridgeChan) DAQmxCreateTEDSAIForceBridgeChan = 0;
decltype(&__033a7042_DAQmxCreateTEDSAIForceIEPEChan) DAQmxCreateTEDSAIForceIEPEChan = 0;
decltype(&__241f0755_DAQmxCreateTEDSAIMicrophoneChan) DAQmxCreateTEDSAIMicrophoneChan = 0;
decltype(&__5e5c5f54_DAQmxCreateTEDSAIPosLVDTChan) DAQmxCreateTEDSAIPosLVDTChan = 0;
decltype(&__405c5f54_DAQmxCreateTEDSAIPosRVDTChan) DAQmxCreateTEDSAIPosRVDTChan = 0;
decltype(&__4b556b20_DAQmxCreateTEDSAIPressureBridgeChan) DAQmxCreateTEDSAIPressureBridgeChan = 0;
decltype(&__12082037_DAQmxCreateTEDSAIRTDChan) DAQmxCreateTEDSAIRTDChan = 0;
decltype(&__3e110f4c_DAQmxCreateTEDSAIResistanceChan) DAQmxCreateTEDSAIResistanceChan = 0;
decltype(&__390e046b_DAQmxCreateTEDSAIStrainGageChan) DAQmxCreateTEDSAIStrainGageChan = 0;
decltype(&__7f6d6c6d_DAQmxCreateTEDSAIThrmcplChan) DAQmxCreateTEDSAIThrmcplChan = 0;
decltype(&__36181073_DAQmxCreateTEDSAIThrmstrChanIex) DAQmxCreateTEDSAIThrmstrChanIex = 0;
decltype(&__29181073_DAQmxCreateTEDSAIThrmstrChanVex) DAQmxCreateTEDSAIThrmstrChanVex = 0;
decltype(&__5939132b_DAQmxCreateTEDSAITorqueBridgeChan) DAQmxCreateTEDSAITorqueBridgeChan = 0;
decltype(&__666d7c7a_DAQmxCreateTEDSAIVoltageChan) DAQmxCreateTEDSAIVoltageChan = 0;
decltype(&__007c6b7b_DAQmxCreateTEDSAIVoltageChanWithExcit) DAQmxCreateTEDSAIVoltageChanWithExcit = 0;
decltype(&__0a774b55_DAQmxCreateTableScale) DAQmxCreateTableScale = 0;
decltype(&__3c052d5c_DAQmxCreateTask) DAQmxCreateTask = 0;
decltype(&__6561547a_DAQmxCreateWatchdogTimerTask) DAQmxCreateWatchdogTimerTask = 0;
decltype(&__2019547a_DAQmxCreateWatchdogTimerTaskEx) DAQmxCreateWatchdogTimerTaskEx = 0;
decltype(&__637c220a_DAQmxDSASetCalTemp) DAQmxDSASetCalTemp = 0;
decltype(&__38070120_DAQmxDeleteNetworkDevice) DAQmxDeleteNetworkDevice = 0;
decltype(&__7f69183c_DAQmxDeleteSavedGlobalChan) DAQmxDeleteSavedGlobalChan = 0;
decltype(&__0e64555a_DAQmxDeleteSavedScale) DAQmxDeleteSavedScale = 0;
decltype(&__6c66475d_DAQmxDeleteSavedTask) DAQmxDeleteSavedTask = 0;
decltype(&__330e5264_DAQmxDeviceSupportsCal) DAQmxDeviceSupportsCal = 0;
decltype(&__6e6a452f_DAQmxDisableAdvTrig) DAQmxDisableAdvTrig = 0;
decltype(&__7d6b552f_DAQmxDisableRefTrig) DAQmxDisableRefTrig = 0;
decltype(&__1d474760_DAQmxDisableStartTrig) DAQmxDisableStartTrig = 0;
decltype(&__4a457509_DAQmxDisconnectSCExpressCalAccChans) DAQmxDisconnectSCExpressCalAccChans = 0;
decltype(&__5f7b4f57_DAQmxDisconnectTerms) DAQmxDisconnectTerms = 0;
decltype(&__1d667e49_DAQmxESeriesCalAdjust) DAQmxESeriesCalAdjust = 0;
decltype(&__5611332f_DAQmxExportSignal) DAQmxExportSignal = 0;
decltype(&__273c0569_DAQmxFieldDAQSetCalTemp) DAQmxFieldDAQSetCalTemp = 0;
decltype(&__670a2d2c_DAQmxGet11601CalAdjustPoints) DAQmxGet11601CalAdjustPoints = 0;
decltype(&__650a2d2c_DAQmxGet11603CalAdjustPoints) DAQmxGet11603CalAdjustPoints = 0;
decltype(&__630a2d2c_DAQmxGet11605CalAdjustPoints) DAQmxGet11605CalAdjustPoints = 0;
decltype(&__650a2d2d_DAQmxGet11613CalAdjustPoints) DAQmxGet11613CalAdjustPoints = 0;
decltype(&__620a2d2d_DAQmxGet11614CalAdjustPoints) DAQmxGet11614CalAdjustPoints = 0;
decltype(&__620a2d2f_DAQmxGet11634CalAdjustPoints) DAQmxGet11634CalAdjustPoints = 0;
decltype(&__351a0170_DAQmxGet4302CalAdjustPoints) DAQmxGet4302CalAdjustPoints = 0;
decltype(&__351a0171_DAQmxGet4303CalAdjustPoints) DAQmxGet4303CalAdjustPoints = 0;
decltype(&__351a0176_DAQmxGet4304CalAdjustPoints) DAQmxGet4304CalAdjustPoints = 0;
decltype(&__351a0177_DAQmxGet4305CalAdjustPoints) DAQmxGet4305CalAdjustPoints = 0;
decltype(&__351a0370_DAQmxGet4322CalAdjustPoints) DAQmxGet4322CalAdjustPoints = 0;
decltype(&__351a027b_DAQmxGet4339CalAdjustPoints) DAQmxGet4339CalAdjustPoints = 0;
decltype(&__534c4c43_DAQmxGet4463AdjustPoints) DAQmxGet4463AdjustPoints = 0;
decltype(&__381b0173_DAQmxGet9201CalAdjustPoints) DAQmxGet9201CalAdjustPoints = 0;
decltype(&__381b0170_DAQmxGet9202CalAdjustPoints) DAQmxGet9202CalAdjustPoints = 0;
decltype(&__381b0171_DAQmxGet9203CalAdjustPoints) DAQmxGet9203CalAdjustPoints = 0;
decltype(&__381b0175_DAQmxGet9207CalAdjustPoints) DAQmxGet9207CalAdjustPoints = 0;
decltype(&__381b017a_DAQmxGet9208CalAdjustPoints) DAQmxGet9208CalAdjustPoints = 0;
decltype(&__381b017b_DAQmxGet9209CalAdjustPoints) DAQmxGet9209CalAdjustPoints = 0;
decltype(&__381b0070_DAQmxGet9212CalAdjustPoints) DAQmxGet9212CalAdjustPoints = 0;
decltype(&__381b0071_DAQmxGet9213CalAdjustPoints) DAQmxGet9213CalAdjustPoints = 0;
decltype(&__381b0076_DAQmxGet9214CalAdjustPoints) DAQmxGet9214CalAdjustPoints = 0;
decltype(&__381b0077_DAQmxGet9215CalAdjustPoints) DAQmxGet9215CalAdjustPoints = 0;
decltype(&__381b0074_DAQmxGet9216CalAdjustPoints) DAQmxGet9216CalAdjustPoints = 0;
decltype(&__381b0075_DAQmxGet9217CalAdjustPoints) DAQmxGet9217CalAdjustPoints = 0;
decltype(&__381b007a_DAQmxGet9218CalAdjustPoints) DAQmxGet9218CalAdjustPoints = 0;
decltype(&__381b007b_DAQmxGet9219CalAdjustPoints) DAQmxGet9219CalAdjustPoints = 0;
decltype(&__381b0372_DAQmxGet9220CalAdjustPoints) DAQmxGet9220CalAdjustPoints = 0;
decltype(&__381b0373_DAQmxGet9221CalAdjustPoints) DAQmxGet9221CalAdjustPoints = 0;
decltype(&__381b0370_DAQmxGet9222CalAdjustPoints) DAQmxGet9222CalAdjustPoints = 0;
decltype(&__381b0371_DAQmxGet9223CalAdjustPoints) DAQmxGet9223CalAdjustPoints = 0;
decltype(&__381b0376_DAQmxGet9224CalAdjustPoints) DAQmxGet9224CalAdjustPoints = 0;
decltype(&__381b0377_DAQmxGet9225CalAdjustPoints) DAQmxGet9225CalAdjustPoints = 0;
decltype(&__381b0374_DAQmxGet9226CalAdjustPoints) DAQmxGet9226CalAdjustPoints = 0;
decltype(&__381b0375_DAQmxGet9227CalAdjustPoints) DAQmxGet9227CalAdjustPoints = 0;
decltype(&__381b037a_DAQmxGet9228CalAdjustPoints) DAQmxGet9228CalAdjustPoints = 0;
decltype(&__381b037b_DAQmxGet9229CalAdjustPoints) DAQmxGet9229CalAdjustPoints = 0;
decltype(&__381b0272_DAQmxGet9230CalAdjustPoints) DAQmxGet9230CalAdjustPoints = 0;
decltype(&__381b0273_DAQmxGet9231CalAdjustPoints) DAQmxGet9231CalAdjustPoints = 0;
decltype(&__381b0270_DAQmxGet9232CalAdjustPoints) DAQmxGet9232CalAdjustPoints = 0;
decltype(&__381b0276_DAQmxGet9234CalAdjustPoints) DAQmxGet9234CalAdjustPoints = 0;
decltype(&__381b027a_DAQmxGet9238CalAdjustPoints) DAQmxGet9238CalAdjustPoints = 0;
decltype(&__381b027b_DAQmxGet9239CalAdjustPoints) DAQmxGet9239CalAdjustPoints = 0;
decltype(&__381b0570_DAQmxGet9242CalAdjustPoints) DAQmxGet9242CalAdjustPoints = 0;
decltype(&__381b0576_DAQmxGet9244CalAdjustPoints) DAQmxGet9244CalAdjustPoints = 0;
decltype(&__381b0574_DAQmxGet9246CalAdjustPoints) DAQmxGet9246CalAdjustPoints = 0;
decltype(&__381b0575_DAQmxGet9247CalAdjustPoints) DAQmxGet9247CalAdjustPoints = 0;
decltype(&__381b0472_DAQmxGet9250CalAdjustPoints) DAQmxGet9250CalAdjustPoints = 0;
decltype(&__381b0473_DAQmxGet9251CalAdjustPoints) DAQmxGet9251CalAdjustPoints = 0;
decltype(&__381b0470_DAQmxGet9252CalAdjustPoints) DAQmxGet9252CalAdjustPoints = 0;
decltype(&__381b0471_DAQmxGet9253CalAdjustPoints) DAQmxGet9253CalAdjustPoints = 0;
decltype(&__381b0772_DAQmxGet9260CalAdjustPoints) DAQmxGet9260CalAdjustPoints = 0;
decltype(&__381b0770_DAQmxGet9262CalAdjustPoints) DAQmxGet9262CalAdjustPoints = 0;
decltype(&__381b0771_DAQmxGet9263CalAdjustPoints) DAQmxGet9263CalAdjustPoints = 0;
decltype(&__381b0776_DAQmxGet9264CalAdjustPoints) DAQmxGet9264CalAdjustPoints = 0;
decltype(&__381b0777_DAQmxGet9265CalAdjustPoints) DAQmxGet9265CalAdjustPoints = 0;
decltype(&__381b0774_DAQmxGet9266CalAdjustPoints) DAQmxGet9266CalAdjustPoints = 0;
decltype(&__381b077b_DAQmxGet9269CalAdjustPoints) DAQmxGet9269CalAdjustPoints = 0;
decltype(&__41223b0e_DAQmxGet9628AICalAdjustPoints) DAQmxGet9628AICalAdjustPoints = 0;
decltype(&__41243b0e_DAQmxGet9628AOCalAdjustPoints) DAQmxGet9628AOCalAdjustPoints = 0;
decltype(&__41223b0f_DAQmxGet9629AICalAdjustPoints) DAQmxGet9629AICalAdjustPoints = 0;
decltype(&__41243b0f_DAQmxGet9629AOCalAdjustPoints) DAQmxGet9629AOCalAdjustPoints = 0;
decltype(&__41223a0e_DAQmxGet9638AICalAdjustPoints) DAQmxGet9638AICalAdjustPoints = 0;
decltype(&__41243a0e_DAQmxGet9638AOCalAdjustPoints) DAQmxGet9638AOCalAdjustPoints = 0;
decltype(&__381e0677_DAQmxGet9775CalAdjustPoints) DAQmxGet9775CalAdjustPoints = 0;
decltype(&__3d716456_DAQmxGetAIACExcitFreq) DAQmxGetAIACExcitFreq = 0;
decltype(&__4d4d643c_DAQmxGetAIACExcitSyncEnable) DAQmxGetAIACExcitSyncEnable = 0;
decltype(&__4c2d1025_DAQmxGetAIACExcitWireMode) DAQmxGetAIACExcitWireMode = 0;
decltype(&__21436b70_DAQmxGetAIADCCustomTimingMode) DAQmxGetAIADCCustomTimingMode = 0;
decltype(&__38111e7d_DAQmxGetAIADCTimingMode) DAQmxGetAIADCTimingMode = 0;
decltype(&__0127395d_DAQmxGetAIAccel4WireDCVoltageSensitivity) DAQmxGetAIAccel4WireDCVoltageSensitivity = 0;
decltype(&__27495029_DAQmxGetAIAccel4WireDCVoltageSensitivityUnits) DAQmxGetAIAccel4WireDCVoltageSensitivityUnits = 0;
decltype(&__16180e20_DAQmxGetAIAccelChargeSensitivity) DAQmxGetAIAccelChargeSensitivity = 0;
decltype(&__30766754_DAQmxGetAIAccelChargeSensitivityUnits) DAQmxGetAIAccelChargeSensitivityUnits = 0;
decltype(&__7b541c29_DAQmxGetAIAccelSensitivity) DAQmxGetAIAccelSensitivity = 0;
decltype(&__12203a47_DAQmxGetAIAccelSensitivityUnits) DAQmxGetAIAccelSensitivityUnits = 0;
decltype(&__70436d5c_DAQmxGetAIAccelUnits) DAQmxGetAIAccelUnits = 0;
decltype(&__5c787c78_DAQmxGetAIAcceldBRef) DAQmxGetAIAcceldBRef = 0;
decltype(&__092a1b6d_DAQmxGetAIAtten) DAQmxGetAIAtten = 0;
decltype(&__1f2a6266_DAQmxGetAIAutoZeroMode) DAQmxGetAIAutoZeroMode = 0;
decltype(&__62582036_DAQmxGetAIAveragingWinSize) DAQmxGetAIAveragingWinSize = 0;
decltype(&__3218052b_DAQmxGetAIBridgeBalanceCoarsePot) DAQmxGetAIBridgeBalanceCoarsePot = 0;
decltype(&__3e337d79_DAQmxGetAIBridgeBalanceFinePot) DAQmxGetAIBridgeBalanceFinePot = 0;
decltype(&__574d760e_DAQmxGetAIBridgeCfg) DAQmxGetAIBridgeCfg = 0;
decltype(&__2d2d3b60_DAQmxGetAIBridgeElectricalUnits) DAQmxGetAIBridgeElectricalUnits = 0;
decltype(&__55507d47_DAQmxGetAIBridgeInitialRatio) DAQmxGetAIBridgeInitialRatio = 0;
decltype(&__3c2d604d_DAQmxGetAIBridgeInitialVoltage) DAQmxGetAIBridgeInitialVoltage = 0;
decltype(&__2e567b4c_DAQmxGetAIBridgeNomResistance) DAQmxGetAIBridgeNomResistance = 0;
decltype(&__0b4e6065_DAQmxGetAIBridgePhysicalUnits) DAQmxGetAIBridgePhysicalUnits = 0;
decltype(&__0c3c0d25_DAQmxGetAIBridgePolyForwardCoeff) DAQmxGetAIBridgePolyForwardCoeff = 0;
decltype(&__0b370837_DAQmxGetAIBridgePolyReverseCoeff) DAQmxGetAIBridgePolyReverseCoeff = 0;
decltype(&__471c0912_DAQmxGetAIBridgeScaleType) DAQmxGetAIBridgeScaleType = 0;
decltype(&__1a0b646e_DAQmxGetAIBridgeShuntCalEnable) DAQmxGetAIBridgeShuntCalEnable = 0;
decltype(&__46710617_DAQmxGetAIBridgeShuntCalGainAdjust) DAQmxGetAIBridgeShuntCalGainAdjust = 0;
decltype(&__03116969_DAQmxGetAIBridgeShuntCalSelect) DAQmxGetAIBridgeShuntCalSelect = 0;
decltype(&__54191938_DAQmxGetAIBridgeShuntCalShuntCalAActualResistance) DAQmxGetAIBridgeShuntCalShuntCalAActualResistance = 0;
decltype(&__5269651c_DAQmxGetAIBridgeShuntCalShuntCalAResistance) DAQmxGetAIBridgeShuntCalShuntCalAResistance = 0;
decltype(&__5578636d_DAQmxGetAIBridgeShuntCalShuntCalASrc) DAQmxGetAIBridgeShuntCalShuntCalASrc = 0;
decltype(&__57191938_DAQmxGetAIBridgeShuntCalShuntCalBActualResistance) DAQmxGetAIBridgeShuntCalShuntCalBActualResistance = 0;
decltype(&__5169651c_DAQmxGetAIBridgeShuntCalShuntCalBResistance) DAQmxGetAIBridgeShuntCalShuntCalBResistance = 0;
decltype(&__44767238_DAQmxGetAIBridgeTableElectricalVals) DAQmxGetAIBridgeTableElectricalVals = 0;
decltype(&__49251916_DAQmxGetAIBridgeTablePhysicalVals) DAQmxGetAIBridgeTablePhysicalVals = 0;
decltype(&__365c5779_DAQmxGetAIBridgeTwoPointLinFirstElectricalVal) DAQmxGetAIBridgeTwoPointLinFirstElectricalVal = 0;
decltype(&__65447907_DAQmxGetAIBridgeTwoPointLinFirstPhysicalVal) DAQmxGetAIBridgeTwoPointLinFirstPhysicalVal = 0;
decltype(&__0303634d_DAQmxGetAIBridgeTwoPointLinSecondElectricalVal) DAQmxGetAIBridgeTwoPointLinSecondElectricalVal = 0;
decltype(&__7d507b63_DAQmxGetAIBridgeTwoPointLinSecondPhysicalVal) DAQmxGetAIBridgeTwoPointLinSecondPhysicalVal = 0;
decltype(&__3245787a_DAQmxGetAIBridgeUnits) DAQmxGetAIBridgeUnits = 0;
decltype(&__08206144_DAQmxGetAIChanCalApplyCalIfExp) DAQmxGetAIChanCalApplyCalIfExp = 0;
decltype(&__34032119_DAQmxGetAIChanCalCalDate) DAQmxGetAIChanCalCalDate = 0;
decltype(&__13655163_DAQmxGetAIChanCalDesc) DAQmxGetAIChanCalDesc = 0;
decltype(&__73643f32_DAQmxGetAIChanCalEnableCal) DAQmxGetAIChanCalEnableCal = 0;
decltype(&__34053805_DAQmxGetAIChanCalExpDate) DAQmxGetAIChanCalExpDate = 0;
decltype(&__0b253e09_DAQmxGetAIChanCalHasValidCalInfo) DAQmxGetAIChanCalHasValidCalInfo = 0;
decltype(&__15415177_DAQmxGetAIChanCalOperatorName) DAQmxGetAIChanCalOperatorName = 0;
decltype(&__5b39230c_DAQmxGetAIChanCalPolyForwardCoeff) DAQmxGetAIChanCalPolyForwardCoeff = 0;
decltype(&__493e2809_DAQmxGetAIChanCalPolyReverseCoeff) DAQmxGetAIChanCalPolyReverseCoeff = 0;
decltype(&__6c720308_DAQmxGetAIChanCalScaleType) DAQmxGetAIChanCalScaleType = 0;
decltype(&__744a7137_DAQmxGetAIChanCalTablePreScaledVals) DAQmxGetAIChanCalTablePreScaledVals = 0;
decltype(&__2b1d0f06_DAQmxGetAIChanCalTableScaledVals) DAQmxGetAIChanCalTableScaledVals = 0;
decltype(&__1b47716d_DAQmxGetAIChanCalVerifAcqVals) DAQmxGetAIChanCalVerifAcqVals = 0;
decltype(&__0c47626b_DAQmxGetAIChanCalVerifRefVals) DAQmxGetAIChanCalVerifRefVals = 0;
decltype(&__3a537960_DAQmxGetAIChargeUnits) DAQmxGetAIChargeUnits = 0;
decltype(&__735d5e7a_DAQmxGetAIChopEnable) DAQmxGetAIChopEnable = 0;
decltype(&__22342715_DAQmxGetAIConvActiveEdge) DAQmxGetAIConvActiveEdge = 0;
decltype(&__674c2715_DAQmxGetAIConvActiveEdgeEx) DAQmxGetAIConvActiveEdgeEx = 0;
decltype(&__6456540a_DAQmxGetAIConvDigFltrEnable) DAQmxGetAIConvDigFltrEnable = 0;
decltype(&__1c56544f_DAQmxGetAIConvDigFltrEnableEx) DAQmxGetAIConvDigFltrEnableEx = 0;
decltype(&__47783312_DAQmxGetAIConvDigFltrMinPulseWidth) DAQmxGetAIConvDigFltrMinPulseWidth = 0;
decltype(&__4778766a_DAQmxGetAIConvDigFltrMinPulseWidthEx) DAQmxGetAIConvDigFltrMinPulseWidthEx = 0;
decltype(&__631b3601_DAQmxGetAIConvDigFltrTimebaseRate) DAQmxGetAIConvDigFltrTimebaseRate = 0;
decltype(&__635e4e01_DAQmxGetAIConvDigFltrTimebaseRateEx) DAQmxGetAIConvDigFltrTimebaseRateEx = 0;
decltype(&__061a2516_DAQmxGetAIConvDigFltrTimebaseSrc) DAQmxGetAIConvDigFltrTimebaseSrc = 0;
decltype(&__43622516_DAQmxGetAIConvDigFltrTimebaseSrcEx) DAQmxGetAIConvDigFltrTimebaseSrcEx = 0;
decltype(&__75434110_DAQmxGetAIConvDigSyncEnable) DAQmxGetAIConvDigSyncEnable = 0;
decltype(&__0d434155_DAQmxGetAIConvDigSyncEnableEx) DAQmxGetAIConvDigSyncEnableEx = 0;
decltype(&__0e6b5b63_DAQmxGetAIConvMaxRate) DAQmxGetAIConvMaxRate = 0;
decltype(&__0e2e2363_DAQmxGetAIConvMaxRateEx) DAQmxGetAIConvMaxRateEx = 0;
decltype(&__675c2517_DAQmxGetAIConvRate) DAQmxGetAIConvRate = 0;
decltype(&__675c606f_DAQmxGetAIConvRateEx) DAQmxGetAIConvRateEx = 0;
decltype(&__70392404_DAQmxGetAIConvSrc) DAQmxGetAIConvSrc = 0;
decltype(&__707c5c04_DAQmxGetAIConvSrcEx) DAQmxGetAIConvSrcEx = 0;
decltype(&__7b390517_DAQmxGetAIConvTimebaseDiv) DAQmxGetAIConvTimebaseDiv = 0;
decltype(&__7b7c7d17_DAQmxGetAIConvTimebaseDivEx) DAQmxGetAIConvTimebaseDivEx = 0;
decltype(&__6e39120c_DAQmxGetAIConvTimebaseSrc) DAQmxGetAIConvTimebaseSrc = 0;
decltype(&__6e7c6a0c_DAQmxGetAIConvTimebaseSrcEx) DAQmxGetAIConvTimebaseSrcEx = 0;
decltype(&__66581b1f_DAQmxGetAICoupling) DAQmxGetAICoupling = 0;
decltype(&__5f5b773e_DAQmxGetAICurrentACRMSUnits) DAQmxGetAICurrentACRMSUnits = 0;
decltype(&__761a3618_DAQmxGetAICurrentShuntLoc) DAQmxGetAICurrentShuntLoc = 0;
decltype(&__071d3803_DAQmxGetAICurrentShuntResistance) DAQmxGetAICurrentShuntResistance = 0;
decltype(&__0f1b7c6b_DAQmxGetAICurrentUnits) DAQmxGetAICurrentUnits = 0;
decltype(&__5d161800_DAQmxGetAICustomScaleName) DAQmxGetAICustomScaleName = 0;
decltype(&__575d1629_DAQmxGetAIDCOffset) DAQmxGetAIDCOffset = 0;
decltype(&__5c28190f_DAQmxGetAIDataXferCustomThreshold) DAQmxGetAIDataXferCustomThreshold = 0;
decltype(&__710e040b_DAQmxGetAIDataXferMaxRate) DAQmxGetAIDataXferMaxRate = 0;
decltype(&__0f34657b_DAQmxGetAIDataXferMech) DAQmxGetAIDataXferMech = 0;
decltype(&__791f1515_DAQmxGetAIDataXferReqCond) DAQmxGetAIDataXferReqCond = 0;
decltype(&__421a181c_DAQmxGetAIDevScalingCoeff) DAQmxGetAIDevScalingCoeff = 0;
decltype(&__79787a31_DAQmxGetAIDigFltrBandpassCenterFreq) DAQmxGetAIDigFltrBandpassCenterFreq = 0;
decltype(&__0b04757d_DAQmxGetAIDigFltrBandpassWidth) DAQmxGetAIDigFltrBandpassWidth = 0;
decltype(&__0e2c7361_DAQmxGetAIDigFltrCoeff) DAQmxGetAIDigFltrCoeff = 0;
decltype(&__0a201765_DAQmxGetAIDigFltrEnable) DAQmxGetAIDigFltrEnable = 0;
decltype(&__6e717622_DAQmxGetAIDigFltrHighpassCutoffFreq) DAQmxGetAIDigFltrHighpassCutoffFreq = 0;
decltype(&__5846321d_DAQmxGetAIDigFltrLowpassCutoffFreq) DAQmxGetAIDigFltrLowpassCutoffFreq = 0;
decltype(&__23293016_DAQmxGetAIDigFltrNotchCenterFreq) DAQmxGetAIDigFltrNotchCenterFreq = 0;
decltype(&__6f5b4c19_DAQmxGetAIDigFltrNotchWidth) DAQmxGetAIDigFltrNotchWidth = 0;
decltype(&__0d346e60_DAQmxGetAIDigFltrOrder) DAQmxGetAIDigFltrOrder = 0;
decltype(&__7d341704_DAQmxGetAIDigFltrResponse) DAQmxGetAIDigFltrResponse = 0;
decltype(&__0d5d6574_DAQmxGetAIDigFltrType) DAQmxGetAIDigFltrType = 0;
decltype(&__0d2e6574_DAQmxGetAIDigFltrTypes) DAQmxGetAIDigFltrTypes = 0;
decltype(&__202c7460_DAQmxGetAIDitherEnable) DAQmxGetAIDitherEnable = 0;
decltype(&__7b240403_DAQmxGetAIEddyCurrentProxProbeSensitivity) DAQmxGetAIEddyCurrentProxProbeSensitivity = 0;
decltype(&__0f026a6a_DAQmxGetAIEddyCurrentProxProbeSensitivityUnits) DAQmxGetAIEddyCurrentProxProbeSensitivityUnits = 0;
decltype(&__6c557108_DAQmxGetAIEddyCurrentProxProbeUnits) DAQmxGetAIEddyCurrentProxProbeUnits = 0;
decltype(&__21234645_DAQmxGetAIEnhancedAliasRejectionEnable) DAQmxGetAIEnhancedAliasRejectionEnable = 0;
decltype(&__1104112d_DAQmxGetAIExcitActualVal) DAQmxGetAIExcitActualVal = 0;
decltype(&__1e497764_DAQmxGetAIExcitDCorAC) DAQmxGetAIExcitDCorAC = 0;
decltype(&__680a1c15_DAQmxGetAIExcitIdleOutputBehavior) DAQmxGetAIExcitIdleOutputBehavior = 0;
decltype(&__7b487657_DAQmxGetAIExcitSense) DAQmxGetAIExcitSense = 0;
decltype(&__6c450532_DAQmxGetAIExcitSrc) DAQmxGetAIExcitSrc = 0;
decltype(&__73794e5d_DAQmxGetAIExcitUseForScaling) DAQmxGetAIExcitUseForScaling = 0;
decltype(&__09525954_DAQmxGetAIExcitUseMultiplexed) DAQmxGetAIExcitUseMultiplexed = 0;
decltype(&__7f4a0537_DAQmxGetAIExcitVal) DAQmxGetAIExcitVal = 0;
decltype(&__30343856_DAQmxGetAIExcitVoltageOrCurrent) DAQmxGetAIExcitVoltageOrCurrent = 0;
decltype(&__2c5e7b63_DAQmxGetAIFilterDelay) DAQmxGetAIFilterDelay = 0;
decltype(&__3c021f64_DAQmxGetAIFilterDelayAdjustment) DAQmxGetAIFilterDelayAdjustment = 0;
decltype(&__5878150a_DAQmxGetAIFilterDelayUnits) DAQmxGetAIFilterDelayUnits = 0;
decltype(&__38307660_DAQmxGetAIFilterEnable) DAQmxGetAIFilterEnable = 0;
decltype(&__57497273_DAQmxGetAIFilterFreq) DAQmxGetAIFilterFreq = 0;
decltype(&__2c497367_DAQmxGetAIFilterOrder) DAQmxGetAIFilterOrder = 0;
decltype(&__2c301717_DAQmxGetAIFilterResponse) DAQmxGetAIFilterResponse = 0;
decltype(&__5841447d_DAQmxGetAIForceIEPESensorSensitivity) DAQmxGetAIForceIEPESensorSensitivity = 0;
decltype(&__7e2f2d09_DAQmxGetAIForceIEPESensorSensitivityUnits) DAQmxGetAIForceIEPESensorSensitivityUnits = 0;
decltype(&__70437021_DAQmxGetAIForceReadFromChan) DAQmxGetAIForceReadFromChan = 0;
decltype(&__61456350_DAQmxGetAIForceUnits) DAQmxGetAIForceUnits = 0;
decltype(&__6b4a3a12_DAQmxGetAIFreqHyst) DAQmxGetAIFreqHyst = 0;
decltype(&__5d535c1f_DAQmxGetAIFreqThreshVoltage) DAQmxGetAIFreqThreshVoltage = 0;
decltype(&__714a5405_DAQmxGetAIFreqUnits) DAQmxGetAIFreqUnits = 0;
decltype(&__14217378_DAQmxGetAIGain) DAQmxGetAIGain = 0;
decltype(&__63497c15_DAQmxGetAIImpedance) DAQmxGetAIImpedance = 0;
decltype(&__260b4749_DAQmxGetAIInputLimitsFaultDetectEnable) DAQmxGetAIInputLimitsFaultDetectEnable = 0;
decltype(&__58573823_DAQmxGetAIInputLimitsFaultDetectLowerLimit) DAQmxGetAIInputLimitsFaultDetectLowerLimit = 0;
decltype(&__41483f23_DAQmxGetAIInputLimitsFaultDetectUpperLimit) DAQmxGetAIInputLimitsFaultDetectUpperLimit = 0;
decltype(&__7f590924_DAQmxGetAIInputSrc) DAQmxGetAIInputSrc = 0;
decltype(&__290a3939_DAQmxGetAIIsTEDS) DAQmxGetAIIsTEDS = 0;
decltype(&__471e2b2a_DAQmxGetAILVDTSensitivity) DAQmxGetAILVDTSensitivity = 0;
decltype(&__33384543_DAQmxGetAILVDTSensitivityUnits) DAQmxGetAILVDTSensitivityUnits = 0;
decltype(&__506f5e21_DAQmxGetAILVDTUnits) DAQmxGetAILVDTUnits = 0;
decltype(&__7c496d61_DAQmxGetAILeadWireResistance) DAQmxGetAILeadWireResistance = 0;
decltype(&__57525e20_DAQmxGetAILossyLSBRemovalCompressedSampSize) DAQmxGetAILossyLSBRemovalCompressedSampSize = 0;
decltype(&__647f7b37_DAQmxGetAILowpassCutoffFreq) DAQmxGetAILowpassCutoffFreq = 0;
decltype(&__1b161264_DAQmxGetAILowpassEnable) DAQmxGetAILowpassEnable = 0;
decltype(&__072c3720_DAQmxGetAILowpassSwitchCapClkSrc) DAQmxGetAILowpassSwitchCapClkSrc = 0;
decltype(&__5c55593c_DAQmxGetAILowpassSwitchCapExtClkDiv) DAQmxGetAILowpassSwitchCapExtClkDiv = 0;
decltype(&__5e4e4a4d_DAQmxGetAILowpassSwitchCapExtClkFreq) DAQmxGetAILowpassSwitchCapExtClkFreq = 0;
decltype(&__5c555331_DAQmxGetAILowpassSwitchCapOutClkDiv) DAQmxGetAILowpassSwitchCapOutClkDiv = 0;
decltype(&__054f7978_DAQmxGetAIMax) DAQmxGetAIMax = 0;
decltype(&__6c592d05_DAQmxGetAIMeasType) DAQmxGetAIMeasType = 0;
decltype(&__3909796e_DAQmxGetAIMemMapEnable) DAQmxGetAIMemMapEnable = 0;
decltype(&__25370660_DAQmxGetAIMicrophoneSensitivity) DAQmxGetAIMicrophoneSensitivity = 0;
decltype(&__134f7970_DAQmxGetAIMin) DAQmxGetAIMin = 0;
decltype(&__24217e72_DAQmxGetAIOpenChanDetectEnable) DAQmxGetAIOpenChanDetectEnable = 0;
decltype(&__680a3f2c_DAQmxGetAIOpenThrmcplDetectEnable) DAQmxGetAIOpenThrmcplDetectEnable = 0;
decltype(&__70090e29_DAQmxGetAIOvercurrentDetectEnable) DAQmxGetAIOvercurrentDetectEnable = 0;
decltype(&__3706414f_DAQmxGetAIPowerSupplyFaultDetectEnable) DAQmxGetAIPowerSupplyFaultDetectEnable = 0;
decltype(&__032d3170_DAQmxGetAIPressureUnits) DAQmxGetAIPressureUnits = 0;
decltype(&__66596444_DAQmxGetAIProbeAtten) DAQmxGetAIProbeAtten = 0;
decltype(&__390e664d_DAQmxGetAIRTDA) DAQmxGetAIRTDA = 0;
decltype(&__390d664d_DAQmxGetAIRTDB) DAQmxGetAIRTDB = 0;
decltype(&__390c664d_DAQmxGetAIRTDC) DAQmxGetAIRTDC = 0;
decltype(&__391d564d_DAQmxGetAIRTDR0) DAQmxGetAIRTDR0 = 0;
decltype(&__5c1b1f3d_DAQmxGetAIRTDType) DAQmxGetAIRTDType = 0;
decltype(&__471e352a_DAQmxGetAIRVDTSensitivity) DAQmxGetAIRVDTSensitivity = 0;
decltype(&__33385b43_DAQmxGetAIRVDTSensitivityUnits) DAQmxGetAIRVDTSensitivityUnits = 0;
decltype(&__506f4021_DAQmxGetAIRVDTUnits) DAQmxGetAIRVDTUnits = 0;
decltype(&__3c2a1219_DAQmxGetAIRawDataCompressionType) DAQmxGetAIRawDataCompressionType = 0;
decltype(&__02306066_DAQmxGetAIRawSampJustification) DAQmxGetAIRawSampJustification = 0;
decltype(&__1f4f6e6f_DAQmxGetAIRawSampSize) DAQmxGetAIRawSampSize = 0;
decltype(&__5f5a4108_DAQmxGetAIRemoveFilterDelay) DAQmxGetAIRemoveFilterDelay = 0;
decltype(&__2c2e116d_DAQmxGetAIResistanceCfg) DAQmxGetAIResistanceCfg = 0;
decltype(&__49261f19_DAQmxGetAIResistanceUnits) DAQmxGetAIResistanceUnits = 0;
decltype(&__7a496567_DAQmxGetAIResolution) DAQmxGetAIResolution = 0;
decltype(&__5c270c13_DAQmxGetAIResolutionUnits) DAQmxGetAIResolutionUnits = 0;
decltype(&__72070f10_DAQmxGetAIRngHigh) DAQmxGetAIRngHigh = 0;
decltype(&__1a030900_DAQmxGetAIRngLow) DAQmxGetAIRngLow = 0;
decltype(&__02117c7f_DAQmxGetAIRosetteStrainGageOrientation) DAQmxGetAIRosetteStrainGageOrientation = 0;
decltype(&__6177110d_DAQmxGetAIRosetteStrainGageRosetteMeasType) DAQmxGetAIRosetteStrainGageRosetteMeasType = 0;
decltype(&__00045c68_DAQmxGetAIRosetteStrainGageRosetteType) DAQmxGetAIRosetteStrainGageRosetteType = 0;
decltype(&__1f35646c_DAQmxGetAIRosetteStrainGageStrainChans) DAQmxGetAIRosetteStrainGageStrainChans = 0;
decltype(&__725e421b_DAQmxGetAISampAndHoldEnable) DAQmxGetAISampAndHoldEnable = 0;
decltype(&__3110190c_DAQmxGetAISensorPowerCfg) DAQmxGetAISensorPowerCfg = 0;
decltype(&__5407061b_DAQmxGetAISensorPowerType) DAQmxGetAISensorPowerType = 0;
decltype(&__45647762_DAQmxGetAISensorPowerVoltage) DAQmxGetAISensorPowerVoltage = 0;
decltype(&__6a5b3418_DAQmxGetAISoundPressureMaxSoundPressureLvl) DAQmxGetAISoundPressureMaxSoundPressureLvl = 0;
decltype(&__615f6173_DAQmxGetAISoundPressureUnits) DAQmxGetAISoundPressureUnits = 0;
decltype(&__4d647057_DAQmxGetAISoundPressuredBRef) DAQmxGetAISoundPressuredBRef = 0;
decltype(&__0b290e66_DAQmxGetAIStrainGageCfg) DAQmxGetAIStrainGageCfg = 0;
decltype(&__0c77646a_DAQmxGetAIStrainGageForceReadFromChan) DAQmxGetAIStrainGageForceReadFromChan = 0;
decltype(&__263d6d77_DAQmxGetAIStrainGageGageFactor) DAQmxGetAIStrainGageGageFactor = 0;
decltype(&__0a3b0728_DAQmxGetAIStrainGagePoissonRatio) DAQmxGetAIStrainGagePoissonRatio = 0;
decltype(&__29406777_DAQmxGetAIStrainUnits) DAQmxGetAIStrainUnits = 0;
decltype(&__50684632_DAQmxGetAITEDSUnits) DAQmxGetAITEDSUnits = 0;
decltype(&__794b4612_DAQmxGetAITempUnits) DAQmxGetAITempUnits = 0;
decltype(&__6822231a_DAQmxGetAITermCfg) DAQmxGetAITermCfg = 0;
decltype(&__2009282c_DAQmxGetAIThrmcplCJCChan) DAQmxGetAIThrmcplCJCChan = 0;
decltype(&__30132a42_DAQmxGetAIThrmcplCJCSrc) DAQmxGetAIThrmcplCJCSrc = 0;
decltype(&__35002542_DAQmxGetAIThrmcplCJCVal) DAQmxGetAIThrmcplCJCVal = 0;
decltype(&__7c4d0031_DAQmxGetAIThrmcplLeadOffsetVoltage) DAQmxGetAIThrmcplLeadOffsetVoltage = 0;
decltype(&__7f713419_DAQmxGetAIThrmcplScaleType) DAQmxGetAIThrmcplScaleType = 0;
decltype(&__06767a71_DAQmxGetAIThrmcplType) DAQmxGetAIThrmcplType = 0;
decltype(&__7d631305_DAQmxGetAIThrmstrA) DAQmxGetAIThrmstrA = 0;
decltype(&__7d601305_DAQmxGetAIThrmstrB) DAQmxGetAIThrmstrB = 0;
decltype(&__7d611305_DAQmxGetAIThrmstrC) DAQmxGetAIThrmstrC = 0;
decltype(&__7d702205_DAQmxGetAIThrmstrR1) DAQmxGetAIThrmstrR1 = 0;
decltype(&__29507c67_DAQmxGetAITorqueUnits) DAQmxGetAITorqueUnits = 0;
decltype(&__5a2a1710_DAQmxGetAIUsbXferReqCount) DAQmxGetAIUsbXferReqCount = 0;
decltype(&__3e2c181b_DAQmxGetAIUsbXferReqSize) DAQmxGetAIUsbXferReqSize = 0;
decltype(&__080a0a42_DAQmxGetAIVelocityIEPESensorSensitivity) DAQmxGetAIVelocityIEPESensorSensitivity = 0;
decltype(&__66637e64_DAQmxGetAIVelocityIEPESensorSensitivityUnits) DAQmxGetAIVelocityIEPESensorSensitivityUnits = 0;
decltype(&__592d2622_DAQmxGetAIVelocityIEPESensordBRef) DAQmxGetAIVelocityIEPESensordBRef = 0;
decltype(&__0c2d277b_DAQmxGetAIVelocityUnits) DAQmxGetAIVelocityUnits = 0;
decltype(&__505d662d_DAQmxGetAIVoltageACRMSUnits) DAQmxGetAIVoltageACRMSUnits = 0;
decltype(&__001d6d78_DAQmxGetAIVoltageUnits) DAQmxGetAIVoltageUnits = 0;
decltype(&__11394143_DAQmxGetAIVoltagedBRef) DAQmxGetAIVoltagedBRef = 0;
decltype(&__77591a0e_DAQmxGetAOCommonModeOffset) DAQmxGetAOCommonModeOffset = 0;
decltype(&__0f1d7c6b_DAQmxGetAOCurrentUnits) DAQmxGetAOCurrentUnits = 0;
decltype(&__5d101800_DAQmxGetAOCustomScaleName) DAQmxGetAOCustomScaleName = 0;
decltype(&__56173109_DAQmxGetAODACOffsetExtSrc) DAQmxGetAODACOffsetExtSrc = 0;
decltype(&__3f00626d_DAQmxGetAODACOffsetSrc) DAQmxGetAODACOffsetSrc = 0;
decltype(&__2c0f6268_DAQmxGetAODACOffsetVal) DAQmxGetAODACOffsetVal = 0;
decltype(&__08047978_DAQmxGetAODACRefAllowConnToGnd) DAQmxGetAODACRefAllowConnToGnd = 0;
decltype(&__4d1b3c3e_DAQmxGetAODACRefConnToGnd) DAQmxGetAODACRefConnToGnd = 0;
decltype(&__0900616d_DAQmxGetAODACRefExtSrc) DAQmxGetAODACRefExtSrc = 0;
decltype(&__6d69763e_DAQmxGetAODACRefSrc) DAQmxGetAODACRefSrc = 0;
decltype(&__687a793e_DAQmxGetAODACRefVal) DAQmxGetAODACRefVal = 0;
decltype(&__76727957_DAQmxGetAODACRngHigh) DAQmxGetAODACRngHigh = 0;
decltype(&__7274693f_DAQmxGetAODACRngLow) DAQmxGetAODACRngLow = 0;
decltype(&__0f32657b_DAQmxGetAODataXferMech) DAQmxGetAODataXferMech = 0;
decltype(&__79191515_DAQmxGetAODataXferReqCond) DAQmxGetAODataXferReqCond = 0;
decltype(&__421c181c_DAQmxGetAODevScalingCoeff) DAQmxGetAODevScalingCoeff = 0;
decltype(&__29235844_DAQmxGetAOEnhancedImageRejectionEnable) DAQmxGetAOEnhancedImageRejectionEnable = 0;
decltype(&__2c587b63_DAQmxGetAOFilterDelay) DAQmxGetAOFilterDelay = 0;
decltype(&__3c041f64_DAQmxGetAOFilterDelayAdjustment) DAQmxGetAOFilterDelayAdjustment = 0;
decltype(&__587e150a_DAQmxGetAOFilterDelayUnits) DAQmxGetAOFilterDelayUnits = 0;
decltype(&__75672c0c_DAQmxGetAOFuncGenAmplitude) DAQmxGetAOFuncGenAmplitude = 0;
decltype(&__6c737e42_DAQmxGetAOFuncGenFMDeviation) DAQmxGetAOFuncGenFMDeviation = 0;
decltype(&__0c6c476c_DAQmxGetAOFuncGenFreq) DAQmxGetAOFuncGenFreq = 0;
decltype(&__1814304d_DAQmxGetAOFuncGenModulationType) DAQmxGetAOFuncGenModulationType = 0;
decltype(&__0e00276f_DAQmxGetAOFuncGenOffset) DAQmxGetAOFuncGenOffset = 0;
decltype(&__101c341e_DAQmxGetAOFuncGenSquareDutyCycle) DAQmxGetAOFuncGenSquareDutyCycle = 0;
decltype(&__6e7e7400_DAQmxGetAOFuncGenStartPhase) DAQmxGetAOFuncGenStartPhase = 0;
decltype(&__187e4c79_DAQmxGetAOFuncGenType) DAQmxGetAOFuncGenType = 0;
decltype(&__14277378_DAQmxGetAOGain) DAQmxGetAOGain = 0;
decltype(&__5150406f_DAQmxGetAOIdleOutputBehavior) DAQmxGetAOIdleOutputBehavior = 0;
decltype(&__022b307a_DAQmxGetAOLoadImpedance) DAQmxGetAOLoadImpedance = 0;
decltype(&__05497978_DAQmxGetAOMax) DAQmxGetAOMax = 0;
decltype(&__390f796e_DAQmxGetAOMemMapEnable) DAQmxGetAOMemMapEnable = 0;
decltype(&__13497970_DAQmxGetAOMin) DAQmxGetAOMin = 0;
decltype(&__4135101e_DAQmxGetAOOutputImpedance) DAQmxGetAOOutputImpedance = 0;
decltype(&__5d407e7d_DAQmxGetAOOutputType) DAQmxGetAOOutputType = 0;
decltype(&__081b305b_DAQmxGetAOPowerAmpChannelEnable) DAQmxGetAOPowerAmpChannelEnable = 0;
decltype(&__0e325156_DAQmxGetAOPowerAmpGain) DAQmxGetAOPowerAmpGain = 0;
decltype(&__012f3c25_DAQmxGetAOPowerAmpOffset) DAQmxGetAOPowerAmpOffset = 0;
decltype(&__045c5f5a_DAQmxGetAOPowerAmpOvercurrent) DAQmxGetAOPowerAmpOvercurrent = 0;
decltype(&__0715435f_DAQmxGetAOPowerAmpScalingCoeff) DAQmxGetAOPowerAmpScalingCoeff = 0;
decltype(&__182f2603_DAQmxGetAOReglitchEnable) DAQmxGetAOReglitchEnable = 0;
decltype(&__7a4f6567_DAQmxGetAOResolution) DAQmxGetAOResolution = 0;
decltype(&__5c210c13_DAQmxGetAOResolutionUnits) DAQmxGetAOResolutionUnits = 0;
decltype(&__6824231a_DAQmxGetAOTermCfg) DAQmxGetAOTermCfg = 0;
decltype(&__5a2c1710_DAQmxGetAOUsbXferReqCount) DAQmxGetAOUsbXferReqCount = 0;
decltype(&__3e2a181b_DAQmxGetAOUsbXferReqSize) DAQmxGetAOUsbXferReqSize = 0;
decltype(&__7e2d2c21_DAQmxGetAOUseOnlyOnBrdMem) DAQmxGetAOUseOnlyOnBrdMem = 0;
decltype(&__3e72757e_DAQmxGetAOVoltageCurrentLimit) DAQmxGetAOVoltageCurrentLimit = 0;
decltype(&__001b6d78_DAQmxGetAOVoltageUnits) DAQmxGetAOVoltageUnits = 0;
decltype(&__767b4019_DAQmxGetAdvTrigType) DAQmxGetAdvTrigType = 0;
decltype(&__615c702e_DAQmxGetAnalogPowerUpStates) DAQmxGetAnalogPowerUpStates = 0;
decltype(&__6c081133_DAQmxGetAnalogPowerUpStatesWithOutputType) DAQmxGetAnalogPowerUpStatesWithOutputType = 0;
decltype(&__1e1b2960_DAQmxGetAnlgEdgeRefTrigCoupling) DAQmxGetAnlgEdgeRefTrigCoupling = 0;
decltype(&__6477516c_DAQmxGetAnlgEdgeRefTrigDigFltrEnable) DAQmxGetAnlgEdgeRefTrigDigFltrEnable = 0;
decltype(&__7c547f0b_DAQmxGetAnlgEdgeRefTrigDigFltrMinPulseWidth) DAQmxGetAnlgEdgeRefTrigDigFltrMinPulseWidth = 0;
decltype(&__6f701c0e_DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseRate) DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseRate = 0;
decltype(&__78151d1d_DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseSrc) DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseSrc = 0;
decltype(&__7e664479_DAQmxGetAnlgEdgeRefTrigDigSyncEnable) DAQmxGetAnlgEdgeRefTrigDigSyncEnable = 0;
decltype(&__61734a07_DAQmxGetAnlgEdgeRefTrigHyst) DAQmxGetAnlgEdgeRefTrigHyst = 0;
decltype(&__6e6c3e03_DAQmxGetAnlgEdgeRefTrigLvl) DAQmxGetAnlgEdgeRefTrigLvl = 0;
decltype(&__746f4e79_DAQmxGetAnlgEdgeRefTrigSlope) DAQmxGetAnlgEdgeRefTrigSlope = 0;
decltype(&__6a633e1c_DAQmxGetAnlgEdgeRefTrigSrc) DAQmxGetAnlgEdgeRefTrigSrc = 0;
decltype(&__6f032a1b_DAQmxGetAnlgEdgeStartTrigCoupling) DAQmxGetAnlgEdgeStartTrigCoupling = 0;
decltype(&__170f5077_DAQmxGetAnlgEdgeStartTrigDigFltrEnable) DAQmxGetAnlgEdgeStartTrigDigFltrEnable = 0;
decltype(&__39684854_DAQmxGetAnlgEdgeStartTrigDigFltrMinPulseWidth) DAQmxGetAnlgEdgeStartTrigDigFltrMinPulseWidth = 0;
decltype(&__5a6d5b70_DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseRate) DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseRate = 0;
decltype(&__5b7e4c15_DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseSrc) DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseSrc = 0;
decltype(&__021a4a66_DAQmxGetAnlgEdgeStartTrigDigSyncEnable) DAQmxGetAnlgEdgeStartTrigDigSyncEnable = 0;
decltype(&__0c645573_DAQmxGetAnlgEdgeStartTrigHyst) DAQmxGetAnlgEdgeStartTrigHyst = 0;
decltype(&__78605a6c_DAQmxGetAnlgEdgeStartTrigLvl) DAQmxGetAnlgEdgeStartTrigLvl = 0;
decltype(&__081a406f_DAQmxGetAnlgEdgeStartTrigSlope) DAQmxGetAnlgEdgeStartTrigSlope = 0;
decltype(&__787f5e63_DAQmxGetAnlgEdgeStartTrigSrc) DAQmxGetAnlgEdgeStartTrigSrc = 0;
decltype(&__2b1f353b_DAQmxGetAnlgLvlPauseTrigCoupling) DAQmxGetAnlgLvlPauseTrigCoupling = 0;
decltype(&__27655943_DAQmxGetAnlgLvlPauseTrigDigFltrEnable) DAQmxGetAnlgLvlPauseTrigDigFltrEnable = 0;
decltype(&__407d7a6d_DAQmxGetAnlgLvlPauseTrigDigFltrMinPulseWidth) DAQmxGetAnlgLvlPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__456e5e0e_DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseRate) DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseRate = 0;
decltype(&__56793b0f_DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseSrc) DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__327f4856_DAQmxGetAnlgLvlPauseTrigDigSyncEnable) DAQmxGetAnlgLvlPauseTrigDigSyncEnable = 0;
decltype(&__4c605d58_DAQmxGetAnlgLvlPauseTrigHyst) DAQmxGetAnlgLvlPauseTrigHyst = 0;
decltype(&__486f422c_DAQmxGetAnlgLvlPauseTrigLvl) DAQmxGetAnlgLvlPauseTrigLvl = 0;
decltype(&__576b4d2c_DAQmxGetAnlgLvlPauseTrigSrc) DAQmxGetAnlgLvlPauseTrigSrc = 0;
decltype(&__53714b42_DAQmxGetAnlgLvlPauseTrigWhen) DAQmxGetAnlgLvlPauseTrigWhen = 0;
decltype(&__347e477b_DAQmxGetAnlgMultiEdgeRefTrigCouplings) DAQmxGetAnlgMultiEdgeRefTrigCouplings = 0;
decltype(&__53012f18_DAQmxGetAnlgMultiEdgeRefTrigHysts) DAQmxGetAnlgMultiEdgeRefTrigHysts = 0;
decltype(&__240e301f_DAQmxGetAnlgMultiEdgeRefTrigLvls) DAQmxGetAnlgMultiEdgeRefTrigLvls = 0;
decltype(&__5e67331c_DAQmxGetAnlgMultiEdgeRefTrigSlopes) DAQmxGetAnlgMultiEdgeRefTrigSlopes = 0;
decltype(&__3b0a3f1f_DAQmxGetAnlgMultiEdgeRefTrigSrcs) DAQmxGetAnlgMultiEdgeRefTrigSrcs = 0;
decltype(&__3c0f2c78_DAQmxGetAnlgMultiEdgeStartTrigCouplings) DAQmxGetAnlgMultiEdgeStartTrigCouplings = 0;
decltype(&__546c4b07_DAQmxGetAnlgMultiEdgeStartTrigHysts) DAQmxGetAnlgMultiEdgeStartTrigHysts = 0;
decltype(&__4b6b3c08_DAQmxGetAnlgMultiEdgeStartTrigLvls) DAQmxGetAnlgMultiEdgeStartTrigLvls = 0;
decltype(&__48684661_DAQmxGetAnlgMultiEdgeStartTrigSlopes) DAQmxGetAnlgMultiEdgeStartTrigSlopes = 0;
decltype(&__446b230c_DAQmxGetAnlgMultiEdgeStartTrigSrcs) DAQmxGetAnlgMultiEdgeStartTrigSrcs = 0;
decltype(&__5d72412c_DAQmxGetAnlgWinPauseTrigBtm) DAQmxGetAnlgWinPauseTrigBtm = 0;
decltype(&__3000373b_DAQmxGetAnlgWinPauseTrigCoupling) DAQmxGetAnlgWinPauseTrigCoupling = 0;
decltype(&__3c7a5b43_DAQmxGetAnlgWinPauseTrigDigFltrEnable) DAQmxGetAnlgWinPauseTrigDigFltrEnable = 0;
decltype(&__5b62786d_DAQmxGetAnlgWinPauseTrigDigFltrMinPulseWidth) DAQmxGetAnlgWinPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__5e715c0e_DAQmxGetAnlgWinPauseTrigDigFltrTimebaseRate) DAQmxGetAnlgWinPauseTrigDigFltrTimebaseRate = 0;
decltype(&__4d66390f_DAQmxGetAnlgWinPauseTrigDigFltrTimebaseSrc) DAQmxGetAnlgWinPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__29604a56_DAQmxGetAnlgWinPauseTrigDigSyncEnable) DAQmxGetAnlgWinPauseTrigDigSyncEnable = 0;
decltype(&__4c744f2c_DAQmxGetAnlgWinPauseTrigSrc) DAQmxGetAnlgWinPauseTrigSrc = 0;
decltype(&__4b695c2c_DAQmxGetAnlgWinPauseTrigTop) DAQmxGetAnlgWinPauseTrigTop = 0;
decltype(&__486e4942_DAQmxGetAnlgWinPauseTrigWhen) DAQmxGetAnlgWinPauseTrigWhen = 0;
decltype(&__4b00202a_DAQmxGetAnlgWinRefTrigBtm) DAQmxGetAnlgWinRefTrigBtm = 0;
decltype(&__3d174d58_DAQmxGetAnlgWinRefTrigCoupling) DAQmxGetAnlgWinRefTrigCoupling = 0;
decltype(&__516f4122_DAQmxGetAnlgWinRefTrigDigFltrEnable) DAQmxGetAnlgWinRefTrigDigFltrEnable = 0;
decltype(&__7241263a_DAQmxGetAnlgWinRefTrigDigFltrMinPulseWidth) DAQmxGetAnlgWinRefTrigDigFltrMinPulseWidth = 0;
decltype(&__56222329_DAQmxGetAnlgWinRefTrigDigFltrTimebaseRate) DAQmxGetAnlgWinRefTrigDigFltrTimebaseRate = 0;
decltype(&__3323303e_DAQmxGetAnlgWinRefTrigDigFltrTimebaseSrc) DAQmxGetAnlgWinRefTrigDigFltrTimebaseSrc = 0;
decltype(&__407a5438_DAQmxGetAnlgWinRefTrigDigSyncEnable) DAQmxGetAnlgWinRefTrigDigSyncEnable = 0;
decltype(&__4500312c_DAQmxGetAnlgWinRefTrigSrc) DAQmxGetAnlgWinRefTrigSrc = 0;
decltype(&__56003631_DAQmxGetAnlgWinRefTrigTop) DAQmxGetAnlgWinRefTrigTop = 0;
decltype(&__436e3536_DAQmxGetAnlgWinRefTrigWhen) DAQmxGetAnlgWinRefTrigWhen = 0;
decltype(&__4866403e_DAQmxGetAnlgWinStartTrigBtm) DAQmxGetAnlgWinStartTrigBtm = 0;
decltype(&__25143629_DAQmxGetAnlgWinStartTrigCoupling) DAQmxGetAnlgWinStartTrigCoupling = 0;
decltype(&__296e5a51_DAQmxGetAnlgWinStartTrigDigFltrEnable) DAQmxGetAnlgWinStartTrigDigFltrEnable = 0;
decltype(&__4e76797f_DAQmxGetAnlgWinStartTrigDigFltrMinPulseWidth) DAQmxGetAnlgWinStartTrigDigFltrMinPulseWidth = 0;
decltype(&__4b655d1c_DAQmxGetAnlgWinStartTrigDigFltrTimebaseRate) DAQmxGetAnlgWinStartTrigDigFltrTimebaseRate = 0;
decltype(&__5872381d_DAQmxGetAnlgWinStartTrigDigFltrTimebaseSrc) DAQmxGetAnlgWinStartTrigDigFltrTimebaseSrc = 0;
decltype(&__3c744b44_DAQmxGetAnlgWinStartTrigDigSyncEnable) DAQmxGetAnlgWinStartTrigDigSyncEnable = 0;
decltype(&__59604e3e_DAQmxGetAnlgWinStartTrigSrc) DAQmxGetAnlgWinStartTrigSrc = 0;
decltype(&__5e7d5d3e_DAQmxGetAnlgWinStartTrigTop) DAQmxGetAnlgWinStartTrigTop = 0;
decltype(&__5d7a4850_DAQmxGetAnlgWinStartTrigWhen) DAQmxGetAnlgWinStartTrigWhen = 0;
decltype(&__5d705953_DAQmxGetArmStartTerm) DAQmxGetArmStartTerm = 0;
decltype(&__1f6d4e50_DAQmxGetArmStartTrigTimescale) DAQmxGetArmStartTrigTimescale = 0;
decltype(&__68534530_DAQmxGetArmStartTrigTimestampEnable) DAQmxGetArmStartTrigTimestampEnable = 0;
decltype(&__0338445d_DAQmxGetArmStartTrigTimestampTimescale) DAQmxGetArmStartTrigTimestampTimescale = 0;
decltype(&__0a2c2f3d_DAQmxGetArmStartTrigTimestampVal) DAQmxGetArmStartTrigTimestampVal = 0;
decltype(&__5e7d4e50_DAQmxGetArmStartTrigTrigWhen) DAQmxGetArmStartTrigTrigWhen = 0;
decltype(&__091e323c_DAQmxGetArmStartTrigType) DAQmxGetArmStartTrigType = 0;
decltype(&__742a023d_DAQmxGetAutoConfiguredCDAQSyncConnections) DAQmxGetAutoConfiguredCDAQSyncConnections = 0;
decltype(&__3b0c2477_DAQmxGetBufInputBufSize) DAQmxGetBufInputBufSize = 0;
decltype(&__68464a55_DAQmxGetBufInputOnbrdBufSize) DAQmxGetBufInputOnbrdBufSize = 0;
decltype(&__2c2c2d20_DAQmxGetBufOutputBufSize) DAQmxGetBufOutputBufSize = 0;
decltype(&__0e7f674e_DAQmxGetBufOutputOnbrdBufSize) DAQmxGetBufOutputOnbrdBufSize = 0;
decltype(&__1a071f69_DAQmxGetBufferAttribute) DAQmxGetBufferAttribute = 0;
decltype(&__39061736_DAQmxGetCIAngEncoderInitialAngle) DAQmxGetCIAngEncoderInitialAngle = 0;
decltype(&__303a2706_DAQmxGetCIAngEncoderPulsesPerRev) DAQmxGetCIAngEncoderPulsesPerRev = 0;
decltype(&__51001712_DAQmxGetCIAngEncoderUnits) DAQmxGetCIAngEncoderUnits = 0;
decltype(&__0a210376_DAQmxGetCICount) DAQmxGetCICount = 0;
decltype(&__3e25574d_DAQmxGetCICountEdgesActiveEdge) DAQmxGetCICountEdgesActiveEdge = 0;
decltype(&__7a110d33_DAQmxGetCICountEdgesCountDirDigFltrEnable) DAQmxGetCICountEdgesCountDirDigFltrEnable = 0;
decltype(&__1d092e1d_DAQmxGetCICountEdgesCountDirDigFltrMinPulseWidth) DAQmxGetCICountEdgesCountDirDigFltrMinPulseWidth = 0;
decltype(&__181a0a7e_DAQmxGetCICountEdgesCountDirDigFltrTimebaseRate) DAQmxGetCICountEdgesCountDirDigFltrTimebaseRate = 0;
decltype(&__0b0d6f7f_DAQmxGetCICountEdgesCountDirDigFltrTimebaseSrc) DAQmxGetCICountEdgesCountDirDigFltrTimebaseSrc = 0;
decltype(&__6f0b1c26_DAQmxGetCICountEdgesCountDirDigSyncEnable) DAQmxGetCICountEdgesCountDirDigSyncEnable = 0;
decltype(&__11140928_DAQmxGetCICountEdgesCountDirHyst) DAQmxGetCICountEdgesCountDirHyst = 0;
decltype(&__42426c4a_DAQmxGetCICountEdgesCountDirLogicLvlBehavior) DAQmxGetCICountEdgesCountDirLogicLvlBehavior = 0;
decltype(&__4e6e6f31_DAQmxGetCICountEdgesCountDirTermCfg) DAQmxGetCICountEdgesCountDirTermCfg = 0;
decltype(&__77193f31_DAQmxGetCICountEdgesCountDirThreshVoltage) DAQmxGetCICountEdgesCountDirThreshVoltage = 0;
decltype(&__0d02263e_DAQmxGetCICountEdgesCountResetActiveEdge) DAQmxGetCICountEdgesCountResetActiveEdge = 0;
decltype(&__4b605521_DAQmxGetCICountEdgesCountResetDigFltrEnable) DAQmxGetCICountEdgesCountResetDigFltrEnable = 0;
decltype(&__684e3239_DAQmxGetCICountEdgesCountResetDigFltrMinPulseWidth) DAQmxGetCICountEdgesCountResetDigFltrMinPulseWidth = 0;
decltype(&__4c2d372a_DAQmxGetCICountEdgesCountResetDigFltrTimebaseRate) DAQmxGetCICountEdgesCountResetDigFltrTimebaseRate = 0;
decltype(&__292c243d_DAQmxGetCICountEdgesCountResetDigFltrTimebaseSrc) DAQmxGetCICountEdgesCountResetDigFltrTimebaseSrc = 0;
decltype(&__5a75403b_DAQmxGetCICountEdgesCountResetDigSyncEnable) DAQmxGetCICountEdgesCountResetDigSyncEnable = 0;
decltype(&__5d6d5f56_DAQmxGetCICountEdgesCountResetEnable) DAQmxGetCICountEdgesCountResetEnable = 0;
decltype(&__4f7b3e24_DAQmxGetCICountEdgesCountResetHyst) DAQmxGetCICountEdgesCountResetHyst = 0;
decltype(&__2a196d72_DAQmxGetCICountEdgesCountResetLogicLvlBehavior) DAQmxGetCICountEdgesCountResetLogicLvlBehavior = 0;
decltype(&__201f3e0f_DAQmxGetCICountEdgesCountResetResetCount) DAQmxGetCICountEdgesCountResetResetCount = 0;
decltype(&__4e622238_DAQmxGetCICountEdgesCountResetTerm) DAQmxGetCICountEdgesCountResetTerm = 0;
decltype(&__2962615e_DAQmxGetCICountEdgesCountResetTermCfg) DAQmxGetCICountEdgesCountResetTermCfg = 0;
decltype(&__79625829_DAQmxGetCICountEdgesCountResetThreshVoltage) DAQmxGetCICountEdgesCountResetThreshVoltage = 0;
decltype(&__4d3a112f_DAQmxGetCICountEdgesDigFltrEnable) DAQmxGetCICountEdgesDigFltrEnable = 0;
decltype(&__2a223201_DAQmxGetCICountEdgesDigFltrMinPulseWidth) DAQmxGetCICountEdgesDigFltrMinPulseWidth = 0;
decltype(&__2f311662_DAQmxGetCICountEdgesDigFltrTimebaseRate) DAQmxGetCICountEdgesDigFltrTimebaseRate = 0;
decltype(&__3c267363_DAQmxGetCICountEdgesDigFltrTimebaseSrc) DAQmxGetCICountEdgesDigFltrTimebaseSrc = 0;
decltype(&__5820003a_DAQmxGetCICountEdgesDigSyncEnable) DAQmxGetCICountEdgesDigSyncEnable = 0;
decltype(&__2a2f1440_DAQmxGetCICountEdgesDir) DAQmxGetCICountEdgesDir = 0;
decltype(&__4f5d7914_DAQmxGetCICountEdgesDirTerm) DAQmxGetCICountEdgesDirTerm = 0;
decltype(&__0a5b654a_DAQmxGetCICountEdgesGateDigFltrEnable) DAQmxGetCICountEdgesGateDigFltrEnable = 0;
decltype(&__6d434664_DAQmxGetCICountEdgesGateDigFltrMinPulseWidth) DAQmxGetCICountEdgesGateDigFltrMinPulseWidth = 0;
decltype(&__68506207_DAQmxGetCICountEdgesGateDigFltrTimebaseRate) DAQmxGetCICountEdgesGateDigFltrTimebaseRate = 0;
decltype(&__7b470706_DAQmxGetCICountEdgesGateDigFltrTimebaseSrc) DAQmxGetCICountEdgesGateDigFltrTimebaseSrc = 0;
decltype(&__002c7347_DAQmxGetCICountEdgesGateEnable) DAQmxGetCICountEdgesGateEnable = 0;
decltype(&__615e6151_DAQmxGetCICountEdgesGateHyst) DAQmxGetCICountEdgesGateHyst = 0;
decltype(&__32080433_DAQmxGetCICountEdgesGateLogicLvlBehavior) DAQmxGetCICountEdgesGateLogicLvlBehavior = 0;
decltype(&__7d426048_DAQmxGetCICountEdgesGateTerm) DAQmxGetCICountEdgesGateTerm = 0;
decltype(&__3e240748_DAQmxGetCICountEdgesGateTermCfg) DAQmxGetCICountEdgesGateTermCfg = 0;
decltype(&__07535748_DAQmxGetCICountEdgesGateThreshVoltage) DAQmxGetCICountEdgesGateThreshVoltage = 0;
decltype(&__7e4f774b_DAQmxGetCICountEdgesGateWhen) DAQmxGetCICountEdgesGateWhen = 0;
decltype(&__263f1534_DAQmxGetCICountEdgesHyst) DAQmxGetCICountEdgesHyst = 0;
decltype(&__203d6377_DAQmxGetCICountEdgesInitialCnt) DAQmxGetCICountEdgesInitialCnt = 0;
decltype(&__75697056_DAQmxGetCICountEdgesLogicLvlBehavior) DAQmxGetCICountEdgesLogicLvlBehavior = 0;
decltype(&__3a23142d_DAQmxGetCICountEdgesTerm) DAQmxGetCICountEdgesTerm = 0;
decltype(&__7945732d_DAQmxGetCICountEdgesTermCfg) DAQmxGetCICountEdgesTermCfg = 0;
decltype(&__4032232d_DAQmxGetCICountEdgesThreshVoltage) DAQmxGetCICountEdgesThreshVoltage = 0;
decltype(&__00291c42_DAQmxGetCICtrTimebaseActiveEdge) DAQmxGetCICtrTimebaseActiveEdge = 0;
decltype(&__625a0304_DAQmxGetCICtrTimebaseDigFltrEnable) DAQmxGetCICtrTimebaseDigFltrEnable = 0;
decltype(&__4c3d1b27_DAQmxGetCICtrTimebaseDigFltrMinPulseWidth) DAQmxGetCICtrTimebaseDigFltrMinPulseWidth = 0;
decltype(&__2f380803_DAQmxGetCICtrTimebaseDigFltrTimebaseRate) DAQmxGetCICtrTimebaseDigFltrTimebaseRate = 0;
decltype(&__2e2b1f66_DAQmxGetCICtrTimebaseDigFltrTimebaseSrc) DAQmxGetCICtrTimebaseDigFltrTimebaseSrc = 0;
decltype(&__774f1915_DAQmxGetCICtrTimebaseDigSyncEnable) DAQmxGetCICtrTimebaseDigSyncEnable = 0;
decltype(&__18396c72_DAQmxGetCICtrTimebaseMasterTimebaseDiv) DAQmxGetCICtrTimebaseMasterTimebaseDiv = 0;
decltype(&__682b1e07_DAQmxGetCICtrTimebaseRate) DAQmxGetCICtrTimebaseRate = 0;
decltype(&__0d2a0d10_DAQmxGetCICtrTimebaseSrc) DAQmxGetCICtrTimebaseSrc = 0;
decltype(&__5f161800_DAQmxGetCICustomScaleName) DAQmxGetCICustomScaleName = 0;
decltype(&__0d34657b_DAQmxGetCIDataXferMech) DAQmxGetCIDataXferMech = 0;
decltype(&__7b1f1515_DAQmxGetCIDataXferReqCond) DAQmxGetCIDataXferReqCond = 0;
decltype(&__700e2a05_DAQmxGetCIDupCountPrevent) DAQmxGetCIDupCountPrevent = 0;
decltype(&__142d3936_DAQmxGetCIDutyCycleDigFltrEnable) DAQmxGetCIDutyCycleDigFltrEnable = 0;
decltype(&__0c0e1751_DAQmxGetCIDutyCycleDigFltrMinPulseWidth) DAQmxGetCIDutyCycleDigFltrMinPulseWidth = 0;
decltype(&__1f2a7454_DAQmxGetCIDutyCycleDigFltrTimebaseRate) DAQmxGetCIDutyCycleDigFltrTimebaseRate = 0;
decltype(&__084f7547_DAQmxGetCIDutyCycleDigFltrTimebaseSrc) DAQmxGetCIDutyCycleDigFltrTimebaseSrc = 0;
decltype(&__474c400e_DAQmxGetCIDutyCycleLogicLvlBehavior) DAQmxGetCIDutyCycleLogicLvlBehavior = 0;
decltype(&__11322677_DAQmxGetCIDutyCycleStartingEdge) DAQmxGetCIDutyCycleStartingEdge = 0;
decltype(&__0d283b41_DAQmxGetCIDutyCycleTerm) DAQmxGetCIDutyCycleTerm = 0;
decltype(&__6b4f3b02_DAQmxGetCIDutyCycleTermCfg) DAQmxGetCIDutyCycleTermCfg = 0;
decltype(&__6263475f_DAQmxGetCIEncoderAInputDigFltrEnable) DAQmxGetCIEncoderAInputDigFltrEnable = 0;
decltype(&__7a406938_DAQmxGetCIEncoderAInputDigFltrMinPulseWidth) DAQmxGetCIEncoderAInputDigFltrMinPulseWidth = 0;
decltype(&__69640a3d_DAQmxGetCIEncoderAInputDigFltrTimebaseRate) DAQmxGetCIEncoderAInputDigFltrTimebaseRate = 0;
decltype(&__7e010b2e_DAQmxGetCIEncoderAInputDigFltrTimebaseSrc) DAQmxGetCIEncoderAInputDigFltrTimebaseSrc = 0;
decltype(&__7872524a_DAQmxGetCIEncoderAInputDigSyncEnable) DAQmxGetCIEncoderAInputDigSyncEnable = 0;
decltype(&__31023e67_DAQmxGetCIEncoderAInputLogicLvlBehavior) DAQmxGetCIEncoderAInputLogicLvlBehavior = 0;
decltype(&__7b664528_DAQmxGetCIEncoderAInputTerm) DAQmxGetCIEncoderAInputTerm = 0;
decltype(&__1d01456b_DAQmxGetCIEncoderAInputTermCfg) DAQmxGetCIEncoderAInputTermCfg = 0;
decltype(&__6260475f_DAQmxGetCIEncoderBInputDigFltrEnable) DAQmxGetCIEncoderBInputDigFltrEnable = 0;
decltype(&__7a436938_DAQmxGetCIEncoderBInputDigFltrMinPulseWidth) DAQmxGetCIEncoderBInputDigFltrMinPulseWidth = 0;
decltype(&__69670a3d_DAQmxGetCIEncoderBInputDigFltrTimebaseRate) DAQmxGetCIEncoderBInputDigFltrTimebaseRate = 0;
decltype(&__7e020b2e_DAQmxGetCIEncoderBInputDigFltrTimebaseSrc) DAQmxGetCIEncoderBInputDigFltrTimebaseSrc = 0;
decltype(&__7871524a_DAQmxGetCIEncoderBInputDigSyncEnable) DAQmxGetCIEncoderBInputDigSyncEnable = 0;
decltype(&__31013e67_DAQmxGetCIEncoderBInputLogicLvlBehavior) DAQmxGetCIEncoderBInputLogicLvlBehavior = 0;
decltype(&__7b654528_DAQmxGetCIEncoderBInputTerm) DAQmxGetCIEncoderBInputTerm = 0;
decltype(&__1d02456b_DAQmxGetCIEncoderBInputTermCfg) DAQmxGetCIEncoderBInputTermCfg = 0;
decltype(&__0354606f_DAQmxGetCIEncoderDecodingType) DAQmxGetCIEncoderDecodingType = 0;
decltype(&__017e4655_DAQmxGetCIEncoderZIndexEnable) DAQmxGetCIEncoderZIndexEnable = 0;
decltype(&__627e5749_DAQmxGetCIEncoderZIndexPhase) DAQmxGetCIEncoderZIndexPhase = 0;
decltype(&__6b73242a_DAQmxGetCIEncoderZIndexVal) DAQmxGetCIEncoderZIndexVal = 0;
decltype(&__6278475f_DAQmxGetCIEncoderZInputDigFltrEnable) DAQmxGetCIEncoderZInputDigFltrEnable = 0;
decltype(&__7a5b6938_DAQmxGetCIEncoderZInputDigFltrMinPulseWidth) DAQmxGetCIEncoderZInputDigFltrMinPulseWidth = 0;
decltype(&__697f0a3d_DAQmxGetCIEncoderZInputDigFltrTimebaseRate) DAQmxGetCIEncoderZInputDigFltrTimebaseRate = 0;
decltype(&__7e1a0b2e_DAQmxGetCIEncoderZInputDigFltrTimebaseSrc) DAQmxGetCIEncoderZInputDigFltrTimebaseSrc = 0;
decltype(&__7869524a_DAQmxGetCIEncoderZInputDigSyncEnable) DAQmxGetCIEncoderZInputDigSyncEnable = 0;
decltype(&__31193e67_DAQmxGetCIEncoderZInputLogicLvlBehavior) DAQmxGetCIEncoderZInputLogicLvlBehavior = 0;
decltype(&__7b7d4528_DAQmxGetCIEncoderZInputTerm) DAQmxGetCIEncoderZInputTerm = 0;
decltype(&__1d1a456b_DAQmxGetCIEncoderZInputTermCfg) DAQmxGetCIEncoderZInputTermCfg = 0;
decltype(&__2e5e7b63_DAQmxGetCIFilterDelay) DAQmxGetCIFilterDelay = 0;
decltype(&__5a78150a_DAQmxGetCIFilterDelayUnits) DAQmxGetCIFilterDelayUnits = 0;
decltype(&__3a307660_DAQmxGetCIFilterEnable) DAQmxGetCIFilterEnable = 0;
decltype(&__55497273_DAQmxGetCIFilterFreq) DAQmxGetCIFilterFreq = 0;
decltype(&__2e497367_DAQmxGetCIFilterOrder) DAQmxGetCIFilterOrder = 0;
decltype(&__2e301717_DAQmxGetCIFilterResponse) DAQmxGetCIFilterResponse = 0;
decltype(&__6d515117_DAQmxGetCIFreqDigFltrEnable) DAQmxGetCIFreqDigFltrEnable = 0;
decltype(&__4e7f360f_DAQmxGetCIFreqDigFltrMinPulseWidth) DAQmxGetCIFreqDigFltrMinPulseWidth = 0;
decltype(&__6a1c331c_DAQmxGetCIFreqDigFltrTimebaseRate) DAQmxGetCIFreqDigFltrTimebaseRate = 0;
decltype(&__0f1d200b_DAQmxGetCIFreqDigFltrTimebaseSrc) DAQmxGetCIFreqDigFltrTimebaseSrc = 0;
decltype(&__7c44440d_DAQmxGetCIFreqDigSyncEnable) DAQmxGetCIFreqDigSyncEnable = 0;
decltype(&__6c3e3602_DAQmxGetCIFreqDiv) DAQmxGetCIFreqDiv = 0;
decltype(&__3c4d577c_DAQmxGetCIFreqEnableAveraging) DAQmxGetCIFreqEnableAveraging = 0;
decltype(&__694a3a12_DAQmxGetCIFreqHyst) DAQmxGetCIFreqHyst = 0;
decltype(&__0c286944_DAQmxGetCIFreqLogicLvlBehavior) DAQmxGetCIFreqLogicLvlBehavior = 0;
decltype(&__0f25726b_DAQmxGetCIFreqMeasMeth) DAQmxGetCIFreqMeasMeth = 0;
decltype(&__16286b67_DAQmxGetCIFreqMeasTime) DAQmxGetCIFreqMeasTime = 0;
decltype(&__724e1012_DAQmxGetCIFreqStartingEdge) DAQmxGetCIFreqStartingEdge = 0;
decltype(&__6853260e_DAQmxGetCIFreqTerm) DAQmxGetCIFreqTerm = 0;
decltype(&__0f536568_DAQmxGetCIFreqTermCfg) DAQmxGetCIFreqTermCfg = 0;
decltype(&__5f535c1f_DAQmxGetCIFreqThreshVoltage) DAQmxGetCIFreqThreshVoltage = 0;
decltype(&__734a5405_DAQmxGetCIFreqUnits) DAQmxGetCIFreqUnits = 0;
decltype(&__273e0b53_DAQmxGetCIGPSSyncMethod) DAQmxGetCIGPSSyncMethod = 0;
decltype(&__4f4f7844_DAQmxGetCIGPSSyncSrc) DAQmxGetCIGPSSyncSrc = 0;
decltype(&__1f0e0120_DAQmxGetCILinEncoderDistPerPulse) DAQmxGetCILinEncoderDistPerPulse = 0;
decltype(&__31127645_DAQmxGetCILinEncoderInitialPos) DAQmxGetCILinEncoderInitialPos = 0;
decltype(&__58001a15_DAQmxGetCILinEncoderUnits) DAQmxGetCILinEncoderUnits = 0;
decltype(&__074f7978_DAQmxGetCIMax) DAQmxGetCIMax = 0;
decltype(&__1d3d1d6b_DAQmxGetCIMaxMeasPeriod) DAQmxGetCIMaxMeasPeriod = 0;
decltype(&__6e592d05_DAQmxGetCIMeasType) DAQmxGetCIMeasType = 0;
decltype(&__3b09796e_DAQmxGetCIMemMapEnable) DAQmxGetCIMemMapEnable = 0;
decltype(&__114f7970_DAQmxGetCIMin) DAQmxGetCIMin = 0;
decltype(&__59321d11_DAQmxGetCINumPossiblyInvalidSamps) DAQmxGetCINumPossiblyInvalidSamps = 0;
decltype(&__3d4b6f6c_DAQmxGetCIOutputState) DAQmxGetCIOutputState = 0;
decltype(&__2e5a7c77_DAQmxGetCIPeriodDigFltrEnable) DAQmxGetCIPeriodDigFltrEnable = 0;
decltype(&__49425f59_DAQmxGetCIPeriodDigFltrMinPulseWidth) DAQmxGetCIPeriodDigFltrMinPulseWidth = 0;
decltype(&__4c517b3a_DAQmxGetCIPeriodDigFltrTimebaseRate) DAQmxGetCIPeriodDigFltrTimebaseRate = 0;
decltype(&__5f461e3b_DAQmxGetCIPeriodDigFltrTimebaseSrc) DAQmxGetCIPeriodDigFltrTimebaseSrc = 0;
decltype(&__3b406d62_DAQmxGetCIPeriodDigSyncEnable) DAQmxGetCIPeriodDigSyncEnable = 0;
decltype(&__494f7d18_DAQmxGetCIPeriodDiv) DAQmxGetCIPeriodDiv = 0;
decltype(&__28312d6b_DAQmxGetCIPeriodEnableAveraging) DAQmxGetCIPeriodEnableAveraging = 0;
decltype(&__455f786c_DAQmxGetCIPeriodHyst) DAQmxGetCIPeriodHyst = 0;
decltype(&__16091d0e_DAQmxGetCIPeriodLogicLvlBehavior) DAQmxGetCIPeriodLogicLvlBehavior = 0;
decltype(&__0d261e03_DAQmxGetCIPeriodMeasMeth) DAQmxGetCIPeriodMeasMeth = 0;
decltype(&__142a070e_DAQmxGetCIPeriodMeasTime) DAQmxGetCIPeriodMeasTime = 0;
decltype(&__6f5f6368_DAQmxGetCIPeriodStartingEdge) DAQmxGetCIPeriodStartingEdge = 0;
decltype(&__59437975_DAQmxGetCIPeriodTerm) DAQmxGetCIPeriodTerm = 0;
decltype(&__1a251e75_DAQmxGetCIPeriodTermCfg) DAQmxGetCIPeriodTermCfg = 0;
decltype(&__23524e75_DAQmxGetCIPeriodThreshVoltage) DAQmxGetCIPeriodThreshVoltage = 0;
decltype(&__2b48626c_DAQmxGetCIPeriodUnits) DAQmxGetCIPeriodUnits = 0;
decltype(&__7659750a_DAQmxGetCIPrescaler) DAQmxGetCIPrescaler = 0;
decltype(&__1d2e1f09_DAQmxGetCIPulseFreqDigFltrEnable) DAQmxGetCIPulseFreqDigFltrEnable = 0;
decltype(&__050d316e_DAQmxGetCIPulseFreqDigFltrMinPulseWidth) DAQmxGetCIPulseFreqDigFltrMinPulseWidth = 0;
decltype(&__1629526b_DAQmxGetCIPulseFreqDigFltrTimebaseRate) DAQmxGetCIPulseFreqDigFltrTimebaseRate = 0;
decltype(&__014c5378_DAQmxGetCIPulseFreqDigFltrTimebaseSrc) DAQmxGetCIPulseFreqDigFltrTimebaseSrc = 0;
decltype(&__073f0a1c_DAQmxGetCIPulseFreqDigSyncEnable) DAQmxGetCIPulseFreqDigSyncEnable = 0;
decltype(&__4e4f6631_DAQmxGetCIPulseFreqLogicLvlBehavior) DAQmxGetCIPulseFreqLogicLvlBehavior = 0;
decltype(&__505c6568_DAQmxGetCIPulseFreqStartEdge) DAQmxGetCIPulseFreqStartEdge = 0;
decltype(&__042b1d7e_DAQmxGetCIPulseFreqTerm) DAQmxGetCIPulseFreqTerm = 0;
decltype(&__624c1d3d_DAQmxGetCIPulseFreqTermCfg) DAQmxGetCIPulseFreqTermCfg = 0;
decltype(&__0f30040c_DAQmxGetCIPulseFreqUnits) DAQmxGetCIPulseFreqUnits = 0;
decltype(&__59231d24_DAQmxGetCIPulseTicksDigFltrEnable) DAQmxGetCIPulseTicksDigFltrEnable = 0;
decltype(&__3e3b3e0a_DAQmxGetCIPulseTicksDigFltrMinPulseWidth) DAQmxGetCIPulseTicksDigFltrMinPulseWidth = 0;
decltype(&__3b281a69_DAQmxGetCIPulseTicksDigFltrTimebaseRate) DAQmxGetCIPulseTicksDigFltrTimebaseRate = 0;
decltype(&__283f7f68_DAQmxGetCIPulseTicksDigFltrTimebaseSrc) DAQmxGetCIPulseTicksDigFltrTimebaseSrc = 0;
decltype(&__4c390c31_DAQmxGetCIPulseTicksDigSyncEnable) DAQmxGetCIPulseTicksDigSyncEnable = 0;
decltype(&__61707c5d_DAQmxGetCIPulseTicksLogicLvlBehavior) DAQmxGetCIPulseTicksLogicLvlBehavior = 0;
decltype(&__386e6f5e_DAQmxGetCIPulseTicksStartEdge) DAQmxGetCIPulseTicksStartEdge = 0;
decltype(&__2e3a1826_DAQmxGetCIPulseTicksTerm) DAQmxGetCIPulseTicksTerm = 0;
decltype(&__6d5c7f26_DAQmxGetCIPulseTicksTermCfg) DAQmxGetCIPulseTicksTermCfg = 0;
decltype(&__06260b1b_DAQmxGetCIPulseTimeDigFltrEnable) DAQmxGetCIPulseTimeDigFltrEnable = 0;
decltype(&__1e05257c_DAQmxGetCIPulseTimeDigFltrMinPulseWidth) DAQmxGetCIPulseTimeDigFltrMinPulseWidth = 0;
decltype(&__0d214679_DAQmxGetCIPulseTimeDigFltrTimebaseRate) DAQmxGetCIPulseTimeDigFltrTimebaseRate = 0;
decltype(&__1a44476a_DAQmxGetCIPulseTimeDigFltrTimebaseSrc) DAQmxGetCIPulseTimeDigFltrTimebaseSrc = 0;
decltype(&__1c371e0e_DAQmxGetCIPulseTimeDigSyncEnable) DAQmxGetCIPulseTimeDigSyncEnable = 0;
decltype(&__55477223_DAQmxGetCIPulseTimeLogicLvlBehavior) DAQmxGetCIPulseTimeLogicLvlBehavior = 0;
decltype(&__4b54717a_DAQmxGetCIPulseTimeStartEdge) DAQmxGetCIPulseTimeStartEdge = 0;
decltype(&__1f23096c_DAQmxGetCIPulseTimeTerm) DAQmxGetCIPulseTimeTerm = 0;
decltype(&__7944092f_DAQmxGetCIPulseTimeTermCfg) DAQmxGetCIPulseTimeTermCfg = 0;
decltype(&__1438101e_DAQmxGetCIPulseTimeUnits) DAQmxGetCIPulseTimeUnits = 0;
decltype(&__5924023c_DAQmxGetCIPulseWidthDigFltrEnable) DAQmxGetCIPulseWidthDigFltrEnable = 0;
decltype(&__3e3c2112_DAQmxGetCIPulseWidthDigFltrMinPulseWidth) DAQmxGetCIPulseWidthDigFltrMinPulseWidth = 0;
decltype(&__3b2f0571_DAQmxGetCIPulseWidthDigFltrTimebaseRate) DAQmxGetCIPulseWidthDigFltrTimebaseRate = 0;
decltype(&__28386070_DAQmxGetCIPulseWidthDigFltrTimebaseSrc) DAQmxGetCIPulseWidthDigFltrTimebaseSrc = 0;
decltype(&__4c3e1329_DAQmxGetCIPulseWidthDigSyncEnable) DAQmxGetCIPulseWidthDigSyncEnable = 0;
decltype(&__61776345_DAQmxGetCIPulseWidthLogicLvlBehavior) DAQmxGetCIPulseWidthLogicLvlBehavior = 0;
decltype(&__18211d23_DAQmxGetCIPulseWidthStartingEdge) DAQmxGetCIPulseWidthStartingEdge = 0;
decltype(&__2e3d073e_DAQmxGetCIPulseWidthTerm) DAQmxGetCIPulseWidthTerm = 0;
decltype(&__6d5b603e_DAQmxGetCIPulseWidthTermCfg) DAQmxGetCIPulseWidthTermCfg = 0;
decltype(&__5c361c27_DAQmxGetCIPulseWidthUnits) DAQmxGetCIPulseWidthUnits = 0;
decltype(&__3f0e200c_DAQmxGetCISampClkOverrunBehavior) DAQmxGetCISampClkOverrunBehavior = 0;
decltype(&__67684007_DAQmxGetCISampClkOverrunSentinelVal) DAQmxGetCISampClkOverrunSentinelVal = 0;
decltype(&__43332f12_DAQmxGetCISemiPeriodDigFltrEnable) DAQmxGetCISemiPeriodDigFltrEnable = 0;
decltype(&__242b0c3c_DAQmxGetCISemiPeriodDigFltrMinPulseWidth) DAQmxGetCISemiPeriodDigFltrMinPulseWidth = 0;
decltype(&__2138285f_DAQmxGetCISemiPeriodDigFltrTimebaseRate) DAQmxGetCISemiPeriodDigFltrTimebaseRate = 0;
decltype(&__322f4d5e_DAQmxGetCISemiPeriodDigFltrTimebaseSrc) DAQmxGetCISemiPeriodDigFltrTimebaseSrc = 0;
decltype(&__56293e07_DAQmxGetCISemiPeriodDigSyncEnable) DAQmxGetCISemiPeriodDigSyncEnable = 0;
decltype(&__7b604e6b_DAQmxGetCISemiPeriodLogicLvlBehavior) DAQmxGetCISemiPeriodLogicLvlBehavior = 0;
decltype(&__0236300d_DAQmxGetCISemiPeriodStartingEdge) DAQmxGetCISemiPeriodStartingEdge = 0;
decltype(&__342a2a10_DAQmxGetCISemiPeriodTerm) DAQmxGetCISemiPeriodTerm = 0;
decltype(&__774c4d10_DAQmxGetCISemiPeriodTermCfg) DAQmxGetCISemiPeriodTermCfg = 0;
decltype(&__46213109_DAQmxGetCISemiPeriodUnits) DAQmxGetCISemiPeriodUnits = 0;
decltype(&__454f6539_DAQmxGetCITCReached) DAQmxGetCITCReached = 0;
decltype(&__3a221a6d_DAQmxGetCIThreshVoltage) DAQmxGetCIThreshVoltage = 0;
decltype(&__6c2d2a25_DAQmxGetCITimestampInitialSeconds) DAQmxGetCITimestampInitialSeconds = 0;
decltype(&__1d2e1722_DAQmxGetCITimestampUnits) DAQmxGetCITimestampUnits = 0;
decltype(&__28136f7d_DAQmxGetCITwoEdgeSepFirstDigFltrEnable) DAQmxGetCITwoEdgeSepFirstDigFltrEnable = 0;
decltype(&__0674775e_DAQmxGetCITwoEdgeSepFirstDigFltrMinPulseWidth) DAQmxGetCITwoEdgeSepFirstDigFltrMinPulseWidth = 0;
decltype(&__6571647a_DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseRate) DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseRate = 0;
decltype(&__6462731f_DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseSrc) DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseSrc = 0;
decltype(&__3d06756c_DAQmxGetCITwoEdgeSepFirstDigSyncEnable) DAQmxGetCITwoEdgeSepFirstDigSyncEnable = 0;
decltype(&__2275776d_DAQmxGetCITwoEdgeSepFirstEdge) DAQmxGetCITwoEdgeSepFirstEdge = 0;
decltype(&__512b3c1c_DAQmxGetCITwoEdgeSepFirstLogicLvlBehavior) DAQmxGetCITwoEdgeSepFirstLogicLvlBehavior = 0;
decltype(&__2a647678_DAQmxGetCITwoEdgeSepFirstTerm) DAQmxGetCITwoEdgeSepFirstTerm = 0;
decltype(&__2a27101f_DAQmxGetCITwoEdgeSepFirstTermCfg) DAQmxGetCITwoEdgeSepFirstTermCfg = 0;
decltype(&__3f37216a_DAQmxGetCITwoEdgeSepSecondDigFltrEnable) DAQmxGetCITwoEdgeSepSecondDigFltrEnable = 0;
decltype(&__1c194672_DAQmxGetCITwoEdgeSepSecondDigFltrMinPulseWidth) DAQmxGetCITwoEdgeSepSecondDigFltrMinPulseWidth = 0;
decltype(&__387a4361_DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseRate) DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseRate = 0;
decltype(&__5d7b5076_DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseSrc) DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseSrc = 0;
decltype(&__2e223470_DAQmxGetCITwoEdgeSepSecondDigSyncEnable) DAQmxGetCITwoEdgeSepSecondDigSyncEnable = 0;
decltype(&__2f3d4772_DAQmxGetCITwoEdgeSepSecondEdge) DAQmxGetCITwoEdgeSepSecondEdge = 0;
decltype(&__5e4e1939_DAQmxGetCITwoEdgeSepSecondLogicLvlBehavior) DAQmxGetCITwoEdgeSepSecondLogicLvlBehavior = 0;
decltype(&__3a355673_DAQmxGetCITwoEdgeSepSecondTerm) DAQmxGetCITwoEdgeSepSecondTerm = 0;
decltype(&__5d351515_DAQmxGetCITwoEdgeSepSecondTermCfg) DAQmxGetCITwoEdgeSepSecondTermCfg = 0;
decltype(&__5337080d_DAQmxGetCITwoEdgeSepUnits) DAQmxGetCITwoEdgeSepUnits = 0;
decltype(&__582a1710_DAQmxGetCIUsbXferReqCount) DAQmxGetCIUsbXferReqCount = 0;
decltype(&__3c2c181b_DAQmxGetCIUsbXferReqSize) DAQmxGetCIUsbXferReqSize = 0;
decltype(&__282c120a_DAQmxGetCIVelocityAngEncoderPulsesPerRev) DAQmxGetCIVelocityAngEncoderPulsesPerRev = 0;
decltype(&__4916221e_DAQmxGetCIVelocityAngEncoderUnits) DAQmxGetCIVelocityAngEncoderUnits = 0;
decltype(&__1159457c_DAQmxGetCIVelocityDiv) DAQmxGetCIVelocityDiv = 0;
decltype(&__7a757253_DAQmxGetCIVelocityEncoderAInputDigFltrEnable) DAQmxGetCIVelocityEncoderAInputDigFltrEnable = 0;
decltype(&__62565c34_DAQmxGetCIVelocityEncoderAInputDigFltrMinPulseWidth) DAQmxGetCIVelocityEncoderAInputDigFltrMinPulseWidth = 0;
decltype(&__71723f31_DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseRate) DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseRate = 0;
decltype(&__66173e22_DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseSrc) DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseSrc = 0;
decltype(&__29140b6b_DAQmxGetCIVelocityEncoderAInputLogicLvlBehavior) DAQmxGetCIVelocityEncoderAInputLogicLvlBehavior = 0;
decltype(&__63707024_DAQmxGetCIVelocityEncoderAInputTerm) DAQmxGetCIVelocityEncoderAInputTerm = 0;
decltype(&__05177067_DAQmxGetCIVelocityEncoderAInputTermCfg) DAQmxGetCIVelocityEncoderAInputTermCfg = 0;
decltype(&__7a767253_DAQmxGetCIVelocityEncoderBInputDigFltrEnable) DAQmxGetCIVelocityEncoderBInputDigFltrEnable = 0;
decltype(&__62555c34_DAQmxGetCIVelocityEncoderBInputDigFltrMinPulseWidth) DAQmxGetCIVelocityEncoderBInputDigFltrMinPulseWidth = 0;
decltype(&__71713f31_DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseRate) DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseRate = 0;
decltype(&__66143e22_DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseSrc) DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseSrc = 0;
decltype(&__29170b6b_DAQmxGetCIVelocityEncoderBInputLogicLvlBehavior) DAQmxGetCIVelocityEncoderBInputLogicLvlBehavior = 0;
decltype(&__63737024_DAQmxGetCIVelocityEncoderBInputTerm) DAQmxGetCIVelocityEncoderBInputTerm = 0;
decltype(&__05147067_DAQmxGetCIVelocityEncoderBInputTermCfg) DAQmxGetCIVelocityEncoderBInputTermCfg = 0;
decltype(&__1b425563_DAQmxGetCIVelocityEncoderDecodingType) DAQmxGetCIVelocityEncoderDecodingType = 0;
decltype(&__0718342c_DAQmxGetCIVelocityLinEncoderDistPerPulse) DAQmxGetCIVelocityLinEncoderDistPerPulse = 0;
decltype(&__40162f19_DAQmxGetCIVelocityLinEncoderUnits) DAQmxGetCIVelocityLinEncoderUnits = 0;
decltype(&__6b4f1819_DAQmxGetCIVelocityMeasTime) DAQmxGetCIVelocityMeasTime = 0;
decltype(&__1c547f6c_DAQmxGetCOAutoIncrCnt) DAQmxGetCOAutoIncrCnt = 0;
decltype(&__597b6c6a_DAQmxGetCOConstrainedGenMode) DAQmxGetCOConstrainedGenMode = 0;
decltype(&__0a270376_DAQmxGetCOCount) DAQmxGetCOCount = 0;
decltype(&__002f1c42_DAQmxGetCOCtrTimebaseActiveEdge) DAQmxGetCOCtrTimebaseActiveEdge = 0;
decltype(&__625c0304_DAQmxGetCOCtrTimebaseDigFltrEnable) DAQmxGetCOCtrTimebaseDigFltrEnable = 0;
decltype(&__4c3b1b27_DAQmxGetCOCtrTimebaseDigFltrMinPulseWidth) DAQmxGetCOCtrTimebaseDigFltrMinPulseWidth = 0;
decltype(&__2f3e0803_DAQmxGetCOCtrTimebaseDigFltrTimebaseRate) DAQmxGetCOCtrTimebaseDigFltrTimebaseRate = 0;
decltype(&__2e2d1f66_DAQmxGetCOCtrTimebaseDigFltrTimebaseSrc) DAQmxGetCOCtrTimebaseDigFltrTimebaseSrc = 0;
decltype(&__77491915_DAQmxGetCOCtrTimebaseDigSyncEnable) DAQmxGetCOCtrTimebaseDigSyncEnable = 0;
decltype(&__183f6c72_DAQmxGetCOCtrTimebaseMasterTimebaseDiv) DAQmxGetCOCtrTimebaseMasterTimebaseDiv = 0;
decltype(&__682d1e07_DAQmxGetCOCtrTimebaseRate) DAQmxGetCOCtrTimebaseRate = 0;
decltype(&__0d2c0d10_DAQmxGetCOCtrTimebaseSrc) DAQmxGetCOCtrTimebaseSrc = 0;
decltype(&__0d32657b_DAQmxGetCODataXferMech) DAQmxGetCODataXferMech = 0;
decltype(&__7b191515_DAQmxGetCODataXferReqCond) DAQmxGetCODataXferReqCond = 0;
decltype(&__07313059_DAQmxGetCOEnableInitialDelayOnRetrigger) DAQmxGetCOEnableInitialDelayOnRetrigger = 0;
decltype(&__3b0f796e_DAQmxGetCOMemMapEnable) DAQmxGetCOMemMapEnable = 0;
decltype(&__3d4d6f6c_DAQmxGetCOOutputState) DAQmxGetCOOutputState = 0;
decltype(&__5f407e7d_DAQmxGetCOOutputType) DAQmxGetCOOutputType = 0;
decltype(&__765f750a_DAQmxGetCOPrescaler) DAQmxGetCOPrescaler = 0;
decltype(&__7c546428_DAQmxGetCOPulseDone) DAQmxGetCOPulseDone = 0;
decltype(&__1f2d786b_DAQmxGetCOPulseDutyCyc) DAQmxGetCOPulseDutyCyc = 0;
decltype(&__615f702a_DAQmxGetCOPulseFreq) DAQmxGetCOPulseFreq = 0;
decltype(&__023b396f_DAQmxGetCOPulseFreqInitialDelay) DAQmxGetCOPulseFreqInitialDelay = 0;
decltype(&__0f36040c_DAQmxGetCOPulseFreqUnits) DAQmxGetCOPulseFreqUnits = 0;
decltype(&__133e0203_DAQmxGetCOPulseHighTicks) DAQmxGetCOPulseHighTicks = 0;
decltype(&__13300c70_DAQmxGetCOPulseHighTime) DAQmxGetCOPulseHighTime = 0;
decltype(&__03371013_DAQmxGetCOPulseIdleState) DAQmxGetCOPulseIdleState = 0;
decltype(&__1f262649_DAQmxGetCOPulseLowTicks) DAQmxGetCOPulseLowTicks = 0;
decltype(&__11285549_DAQmxGetCOPulseLowTime) DAQmxGetCOPulseLowTime = 0;
decltype(&__76486c38_DAQmxGetCOPulseTerm) DAQmxGetCOPulseTerm = 0;
decltype(&__3f3a0e02_DAQmxGetCOPulseTicksInitialDelay) DAQmxGetCOPulseTicksInitialDelay = 0;
decltype(&__19332d7d_DAQmxGetCOPulseTimeInitialDelay) DAQmxGetCOPulseTimeInitialDelay = 0;
decltype(&__143e101e_DAQmxGetCOPulseTimeUnits) DAQmxGetCOPulseTimeUnits = 0;
decltype(&__29067e59_DAQmxGetCORdyForNewVal) DAQmxGetCORdyForNewVal = 0;
decltype(&__582c1710_DAQmxGetCOUsbXferReqCount) DAQmxGetCOUsbXferReqCount = 0;
decltype(&__3c2a181b_DAQmxGetCOUsbXferReqSize) DAQmxGetCOUsbXferReqSize = 0;
decltype(&__7c2d2c21_DAQmxGetCOUseOnlyOnBrdMem) DAQmxGetCOUseOnlyOnBrdMem = 0;
decltype(&__316c6454_DAQmxGetCalAccConnectionCount) DAQmxGetCalAccConnectionCount = 0;
decltype(&__77610c38_DAQmxGetCalDevTemp) DAQmxGetCalDevTemp = 0;
decltype(&__0700311d_DAQmxGetCalInfoAttribute) DAQmxGetCalInfoAttribute = 0;
decltype(&__21507965_DAQmxGetCalRecommendedAccConnectionCountLimit) DAQmxGetCalRecommendedAccConnectionCountLimit = 0;
decltype(&__6a6f0a08_DAQmxGetCalUserDefinedInfo) DAQmxGetCalUserDefinedInfo = 0;
decltype(&__773c2e13_DAQmxGetCalUserDefinedInfoMaxSize) DAQmxGetCalUserDefinedInfoMaxSize = 0;
decltype(&__1f3e2834_DAQmxGetCarrierSerialNum) DAQmxGetCarrierSerialNum = 0;
decltype(&__32785471_DAQmxGetChanAttribute) DAQmxGetChanAttribute = 0;
decltype(&__490b2614_DAQmxGetChanDescr) DAQmxGetChanDescr = 0;
decltype(&__597f7377_DAQmxGetChanIsGlobal) DAQmxGetChanIsGlobal = 0;
decltype(&__1d016377_DAQmxGetChanSyncUnlockBehavior) DAQmxGetChanSyncUnlockBehavior = 0;
decltype(&__2b172512_DAQmxGetChanType) DAQmxGetChanType = 0;
decltype(&__37075a77_DAQmxGetChangeDetectDIFallingEdgePhysicalChans) DAQmxGetChangeDetectDIFallingEdgePhysicalChans = 0;
decltype(&__1749507c_DAQmxGetChangeDetectDIRisingEdgePhysicalChans) DAQmxGetChangeDetectDIRisingEdgePhysicalChans = 0;
decltype(&__35315275_DAQmxGetChangeDetectDITristate) DAQmxGetChangeDetectDITristate = 0;
decltype(&__6c757208_DAQmxGetDIAcquireOn) DAQmxGetDIAcquireOn = 0;
decltype(&__0a34657b_DAQmxGetDIDataXferMech) DAQmxGetDIDataXferMech = 0;
decltype(&__7c1f1515_DAQmxGetDIDataXferReqCond) DAQmxGetDIDataXferReqCond = 0;
decltype(&__0f201765_DAQmxGetDIDigFltrEnable) DAQmxGetDIDigFltrEnable = 0;
decltype(&__1e365a48_DAQmxGetDIDigFltrEnableBusMode) DAQmxGetDIDigFltrEnableBusMode = 0;
decltype(&__2c0e707d_DAQmxGetDIDigFltrMinPulseWidth) DAQmxGetDIDigFltrMinPulseWidth = 0;
decltype(&__086d756e_DAQmxGetDIDigFltrTimebaseRate) DAQmxGetDIDigFltrTimebaseRate = 0;
decltype(&__6d6c6679_DAQmxGetDIDigFltrTimebaseSrc) DAQmxGetDIDigFltrTimebaseSrc = 0;
decltype(&__1e35027f_DAQmxGetDIDigSyncEnable) DAQmxGetDIDigSyncEnable = 0;
decltype(&__31436166_DAQmxGetDIInvertLines) DAQmxGetDIInvertLines = 0;
decltype(&__074b725c_DAQmxGetDILogicFamily) DAQmxGetDILogicFamily = 0;
decltype(&__3c09796e_DAQmxGetDIMemMapEnable) DAQmxGetDIMemMapEnable = 0;
decltype(&__70701302_DAQmxGetDINumLines) DAQmxGetDINumLines = 0;
decltype(&__6559140a_DAQmxGetDITristate) DAQmxGetDITristate = 0;
decltype(&__5f2a1710_DAQmxGetDIUsbXferReqCount) DAQmxGetDIUsbXferReqCount = 0;
decltype(&__3b2c181b_DAQmxGetDIUsbXferReqSize) DAQmxGetDIUsbXferReqSize = 0;
decltype(&__0a32657b_DAQmxGetDODataXferMech) DAQmxGetDODataXferMech = 0;
decltype(&__7c191515_DAQmxGetDODataXferReqCond) DAQmxGetDODataXferReqCond = 0;
decltype(&__62494e73_DAQmxGetDOGenerateOn) DAQmxGetDOGenerateOn = 0;
decltype(&__31456166_DAQmxGetDOInvertLines) DAQmxGetDOInvertLines = 0;
decltype(&__05434166_DAQmxGetDOLineStatesDoneState) DAQmxGetDOLineStatesDoneState = 0;
decltype(&__23290d70_DAQmxGetDOLineStatesPausedState) DAQmxGetDOLineStatesPausedState = 0;
decltype(&__241a5b64_DAQmxGetDOLineStatesStartState) DAQmxGetDOLineStatesStartState = 0;
decltype(&__074d725c_DAQmxGetDOLogicFamily) DAQmxGetDOLogicFamily = 0;
decltype(&__3c0f796e_DAQmxGetDOMemMapEnable) DAQmxGetDOMemMapEnable = 0;
decltype(&__70761302_DAQmxGetDONumLines) DAQmxGetDONumLines = 0;
decltype(&__481f1e1e_DAQmxGetDOOutputDriveType) DAQmxGetDOOutputDriveType = 0;
decltype(&__7f3b0f09_DAQmxGetDOOvercurrentAutoReenable) DAQmxGetDOOvercurrentAutoReenable = 0;
decltype(&__72711419_DAQmxGetDOOvercurrentLimit) DAQmxGetDOOvercurrentLimit = 0;
decltype(&__79457b0f_DAQmxGetDOOvercurrentReenablePeriod) DAQmxGetDOOvercurrentReenablePeriod = 0;
decltype(&__655f140a_DAQmxGetDOTristate) DAQmxGetDOTristate = 0;
decltype(&__5f2c1710_DAQmxGetDOUsbXferReqCount) DAQmxGetDOUsbXferReqCount = 0;
decltype(&__3b2a181b_DAQmxGetDOUsbXferReqSize) DAQmxGetDOUsbXferReqSize = 0;
decltype(&__7b2d2c21_DAQmxGetDOUseOnlyOnBrdMem) DAQmxGetDOUseOnlyOnBrdMem = 0;
decltype(&__21504b70_DAQmxGetDelayFromSampClkDelay) DAQmxGetDelayFromSampClkDelay = 0;
decltype(&__21153370_DAQmxGetDelayFromSampClkDelayEx) DAQmxGetDelayFromSampClkDelayEx = 0;
decltype(&__55762519_DAQmxGetDelayFromSampClkDelayUnits) DAQmxGetDelayFromSampClkDelayUnits = 0;
decltype(&__55766061_DAQmxGetDelayFromSampClkDelayUnitsEx) DAQmxGetDelayFromSampClkDelayUnitsEx = 0;
decltype(&__3b212663_DAQmxGetDevAIBridgeRngs) DAQmxGetDevAIBridgeRngs = 0;
decltype(&__2d203c6b_DAQmxGetDevAIChargeRngs) DAQmxGetDevAIChargeRngs = 0;
decltype(&__263f4443_DAQmxGetDevAICouplings) DAQmxGetDevAICouplings = 0;
decltype(&__12252f1a_DAQmxGetDevAICurrentIntExcitDiscreteVals) DAQmxGetDevAICurrentIntExcitDiscreteVals = 0;
decltype(&__112b3e2d_DAQmxGetDevAICurrentRngs) DAQmxGetDevAICurrentRngs = 0;
decltype(&__4d052b16_DAQmxGetDevAIDigFltrLowpassCutoffFreqDiscreteVals) DAQmxGetDevAIDigFltrLowpassCutoffFreqDiscreteVals = 0;
decltype(&__33217172_DAQmxGetDevAIDigFltrLowpassCutoffFreqRangeVals) DAQmxGetDevAIDigFltrLowpassCutoffFreqRangeVals = 0;
decltype(&__33775e5a_DAQmxGetDevAIFreqRngs) DAQmxGetDevAIFreqRngs = 0;
decltype(&__5f572331_DAQmxGetDevAIGains) DAQmxGetDevAIGains = 0;
decltype(&__6a590c2c_DAQmxGetDevAILowpassCutoffFreqDiscreteVals) DAQmxGetDevAILowpassCutoffFreqDiscreteVals = 0;
decltype(&__0e272876_DAQmxGetDevAILowpassCutoffFreqRangeVals) DAQmxGetDevAILowpassCutoffFreqRangeVals = 0;
decltype(&__1e4a4641_DAQmxGetDevAIMaxMultiChanRate) DAQmxGetDevAIMaxMultiChanRate = 0;
decltype(&__1b295c4e_DAQmxGetDevAIMaxSingleChanRate) DAQmxGetDevAIMaxSingleChanRate = 0;
decltype(&__634f5f53_DAQmxGetDevAIMinRate) DAQmxGetDevAIMinRate = 0;
decltype(&__4c2b1c27_DAQmxGetDevAINumSampTimingEngines) DAQmxGetDevAINumSampTimingEngines = 0;
decltype(&__24724746_DAQmxGetDevAINumSyncPulseSrcs) DAQmxGetDevAINumSyncPulseSrcs = 0;
decltype(&__406a2121_DAQmxGetDevAIPhysicalChans) DAQmxGetDevAIPhysicalChans = 0;
decltype(&__58464518_DAQmxGetDevAIResistanceRngs) DAQmxGetDevAIResistanceRngs = 0;
decltype(&__240e4c51_DAQmxGetDevAISampModes) DAQmxGetDevAISampModes = 0;
decltype(&__5b5e3f26_DAQmxGetDevAISimultaneousSamplingSupported) DAQmxGetDevAISimultaneousSamplingSupported = 0;
decltype(&__352d2f40_DAQmxGetDevAISupportedMeasTypes) DAQmxGetDevAISupportedMeasTypes = 0;
decltype(&__31074350_DAQmxGetDevAITrigUsage) DAQmxGetDevAITrigUsage = 0;
decltype(&__14343c15_DAQmxGetDevAIVoltageIntExcitDiscreteVals) DAQmxGetDevAIVoltageIntExcitDiscreteVals = 0;
decltype(&__306e586b_DAQmxGetDevAIVoltageIntExcitRangeVals) DAQmxGetDevAIVoltageIntExcitRangeVals = 0;
decltype(&__173a2d22_DAQmxGetDevAIVoltageRngs) DAQmxGetDevAIVoltageRngs = 0;
decltype(&__172b3e2d_DAQmxGetDevAOCurrentRngs) DAQmxGetDevAOCurrentRngs = 0;
decltype(&__59572331_DAQmxGetDevAOGains) DAQmxGetDevAOGains = 0;
decltype(&__654f5745_DAQmxGetDevAOMaxRate) DAQmxGetDevAOMaxRate = 0;
decltype(&__654f5f53_DAQmxGetDevAOMinRate) DAQmxGetDevAOMinRate = 0;
decltype(&__4a2b1c27_DAQmxGetDevAONumSampTimingEngines) DAQmxGetDevAONumSampTimingEngines = 0;
decltype(&__22724746_DAQmxGetDevAONumSyncPulseSrcs) DAQmxGetDevAONumSyncPulseSrcs = 0;
decltype(&__466a2121_DAQmxGetDevAOPhysicalChans) DAQmxGetDevAOPhysicalChans = 0;
decltype(&__1f744b4b_DAQmxGetDevAOSampClkSupported) DAQmxGetDevAOSampClkSupported = 0;
decltype(&__220e4c51_DAQmxGetDevAOSampModes) DAQmxGetDevAOSampModes = 0;
decltype(&__71320f38_DAQmxGetDevAOSupportedOutputTypes) DAQmxGetDevAOSupportedOutputTypes = 0;
decltype(&__37074350_DAQmxGetDevAOTrigUsage) DAQmxGetDevAOTrigUsage = 0;
decltype(&__113a2d22_DAQmxGetDevAOVoltageRngs) DAQmxGetDevAOVoltageRngs = 0;
decltype(&__38133d78_DAQmxGetDevAccessoryProductNums) DAQmxGetDevAccessoryProductNums = 0;
decltype(&__340e2b11_DAQmxGetDevAccessoryProductTypes) DAQmxGetDevAccessoryProductTypes = 0;
decltype(&__3715694e_DAQmxGetDevAccessorySerialNums) DAQmxGetDevAccessorySerialNums = 0;
decltype(&__63625754_DAQmxGetDevAnlgTrigSupported) DAQmxGetDevAnlgTrigSupported = 0;
decltype(&__7d751622_DAQmxGetDevBusType) DAQmxGetDevBusType = 0;
decltype(&__62475947_DAQmxGetDevCIMaxSize) DAQmxGetDevCIMaxSize = 0;
decltype(&__07263d22_DAQmxGetDevCIMaxTimebase) DAQmxGetDevCIMaxTimebase = 0;
decltype(&__406a2123_DAQmxGetDevCIPhysicalChans) DAQmxGetDevCIPhysicalChans = 0;
decltype(&__19744b49_DAQmxGetDevCISampClkSupported) DAQmxGetDevCISampClkSupported = 0;
decltype(&__240e4c53_DAQmxGetDevCISampModes) DAQmxGetDevCISampModes = 0;
decltype(&__352d2f42_DAQmxGetDevCISupportedMeasTypes) DAQmxGetDevCISupportedMeasTypes = 0;
decltype(&__31074352_DAQmxGetDevCITrigUsage) DAQmxGetDevCITrigUsage = 0;
decltype(&__64475947_DAQmxGetDevCOMaxSize) DAQmxGetDevCOMaxSize = 0;
decltype(&__01263d22_DAQmxGetDevCOMaxTimebase) DAQmxGetDevCOMaxTimebase = 0;
decltype(&__466a2123_DAQmxGetDevCOPhysicalChans) DAQmxGetDevCOPhysicalChans = 0;
decltype(&__1f744b49_DAQmxGetDevCOSampClkSupported) DAQmxGetDevCOSampClkSupported = 0;
decltype(&__220e4c53_DAQmxGetDevCOSampModes) DAQmxGetDevCOSampModes = 0;
decltype(&__71320f3a_DAQmxGetDevCOSupportedOutputTypes) DAQmxGetDevCOSupportedOutputTypes = 0;
decltype(&__37074352_DAQmxGetDevCOTrigUsage) DAQmxGetDevCOTrigUsage = 0;
decltype(&__380c031e_DAQmxGetDevChassisModuleDevNames) DAQmxGetDevChassisModuleDevNames = 0;
decltype(&__73426426_DAQmxGetDevCompactDAQChassisDevName) DAQmxGetDevCompactDAQChassisDevName = 0;
decltype(&__51676f78_DAQmxGetDevCompactDAQSlotNum) DAQmxGetDevCompactDAQSlotNum = 0;
decltype(&__6d42722e_DAQmxGetDevCompactRIOChassisDevName) DAQmxGetDevCompactRIOChassisDevName = 0;
decltype(&__4f677970_DAQmxGetDevCompactRIOSlotNum) DAQmxGetDevCompactRIOSlotNum = 0;
decltype(&__545c2b33_DAQmxGetDevDILines) DAQmxGetDevDILines = 0;
decltype(&__634f5740_DAQmxGetDevDIMaxRate) DAQmxGetDevDIMaxRate = 0;
decltype(&__4c2b1c22_DAQmxGetDevDINumSampTimingEngines) DAQmxGetDevDINumSampTimingEngines = 0;
decltype(&__45402d2f_DAQmxGetDevDIPorts) DAQmxGetDevDIPorts = 0;
decltype(&__31074355_DAQmxGetDevDITrigUsage) DAQmxGetDevDITrigUsage = 0;
decltype(&__525c2b33_DAQmxGetDevDOLines) DAQmxGetDevDOLines = 0;
decltype(&__654f5740_DAQmxGetDevDOMaxRate) DAQmxGetDevDOMaxRate = 0;
decltype(&__4a2b1c22_DAQmxGetDevDONumSampTimingEngines) DAQmxGetDevDONumSampTimingEngines = 0;
decltype(&__43402d2f_DAQmxGetDevDOPorts) DAQmxGetDevDOPorts = 0;
decltype(&__37074355_DAQmxGetDevDOTrigUsage) DAQmxGetDevDOTrigUsage = 0;
decltype(&__7c764e28_DAQmxGetDevDigTrigSupported) DAQmxGetDevDigTrigSupported = 0;
decltype(&__1b384a49_DAQmxGetDevIsSimulated) DAQmxGetDevIsSimulated = 0;
decltype(&__223e6e7b_DAQmxGetDevNumDMAChans) DAQmxGetDevNumDMAChans = 0;
decltype(&__090c314c_DAQmxGetDevNumTimeTrigs) DAQmxGetDevNumTimeTrigs = 0;
decltype(&__0a127c61_DAQmxGetDevNumTimestampEngines) DAQmxGetDevNumTimestampEngines = 0;
decltype(&__48647551_DAQmxGetDevPCIBusNum) DAQmxGetDevPCIBusNum = 0;
decltype(&__4d647341_DAQmxGetDevPCIDevNum) DAQmxGetDevPCIDevNum = 0;
decltype(&__32170725_DAQmxGetDevPXIChassisNum) DAQmxGetDevPXIChassisNum = 0;
decltype(&__225e5f50_DAQmxGetDevPXISlotNum) DAQmxGetDevPXISlotNum = 0;
decltype(&__6f640232_DAQmxGetDevProductCategory) DAQmxGetDevProductCategory = 0;
decltype(&__04786849_DAQmxGetDevProductNum) DAQmxGetDevProductNum = 0;
decltype(&__191d7245_DAQmxGetDevProductType) DAQmxGetDevProductType = 0;
decltype(&__715f5e46_DAQmxGetDevSerialNum) DAQmxGetDevSerialNum = 0;
decltype(&__4579060c_DAQmxGetDevTCPIPEthernetIP) DAQmxGetDevTCPIPEthernetIP = 0;
decltype(&__1d3d150c_DAQmxGetDevTCPIPHostname) DAQmxGetDevTCPIPHostname = 0;
decltype(&__496f0a0b_DAQmxGetDevTCPIPWirelessIP) DAQmxGetDevTCPIPWirelessIP = 0;
decltype(&__2c2b5345_DAQmxGetDevTEDSHWTEDSSupported) DAQmxGetDevTEDSHWTEDSSupported = 0;
decltype(&__73704357_DAQmxGetDevTerminals) DAQmxGetDevTerminals = 0;
decltype(&__64635541_DAQmxGetDevTimeTrigSupported) DAQmxGetDevTimeTrigSupported = 0;
decltype(&__1a000f66_DAQmxGetDeviceAttribute) DAQmxGetDeviceAttribute = 0;
decltype(&__66764113_DAQmxGetDigEdgeAdvTrigDigFltrEnable) DAQmxGetDigEdgeAdvTrigDigFltrEnable = 0;
decltype(&__767c270b_DAQmxGetDigEdgeAdvTrigEdge) DAQmxGetDigEdgeAdvTrigEdge = 0;
decltype(&__7219311d_DAQmxGetDigEdgeAdvTrigSrc) DAQmxGetDigEdgeAdvTrigSrc = 0;
decltype(&__0109191e_DAQmxGetDigEdgeArmStartTrigDigFltrEnable) DAQmxGetDigEdgeArmStartTrigDigFltrEnable = 0;
decltype(&__192a3779_DAQmxGetDigEdgeArmStartTrigDigFltrMinPulseWidth) DAQmxGetDigEdgeArmStartTrigDigFltrMinPulseWidth = 0;
decltype(&__0a0e547c_DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseRate) DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseRate = 0;
decltype(&__1d6b556f_DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseSrc) DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseSrc = 0;
decltype(&__1b180c0b_DAQmxGetDigEdgeArmStartTrigDigSyncEnable) DAQmxGetDigEdgeArmStartTrigDigSyncEnable = 0;
decltype(&__19191378_DAQmxGetDigEdgeArmStartTrigEdge) DAQmxGetDigEdgeArmStartTrigEdge = 0;
decltype(&__0f1d766e_DAQmxGetDigEdgeArmStartTrigSrc) DAQmxGetDigEdgeArmStartTrigSrc = 0;
decltype(&__67664100_DAQmxGetDigEdgeRefTrigDigFltrEnable) DAQmxGetDigEdgeRefTrigDigFltrEnable = 0;
decltype(&__44482618_DAQmxGetDigEdgeRefTrigDigFltrMinPulseWidth) DAQmxGetDigEdgeRefTrigDigFltrMinPulseWidth = 0;
decltype(&__602b230b_DAQmxGetDigEdgeRefTrigDigFltrTimebaseRate) DAQmxGetDigEdgeRefTrigDigFltrTimebaseRate = 0;
decltype(&__052a301c_DAQmxGetDigEdgeRefTrigDigFltrTimebaseSrc) DAQmxGetDigEdgeRefTrigDigFltrTimebaseSrc = 0;
decltype(&__7673541a_DAQmxGetDigEdgeRefTrigDigSyncEnable) DAQmxGetDigEdgeRefTrigDigSyncEnable = 0;
decltype(&__776c2718_DAQmxGetDigEdgeRefTrigEdge) DAQmxGetDigEdgeRefTrigEdge = 0;
decltype(&__7309310e_DAQmxGetDigEdgeRefTrigSrc) DAQmxGetDigEdgeRefTrigSrc = 0;
decltype(&__1f675a73_DAQmxGetDigEdgeStartTrigDigFltrEnable) DAQmxGetDigEdgeStartTrigDigFltrEnable = 0;
decltype(&__787f795d_DAQmxGetDigEdgeStartTrigDigFltrMinPulseWidth) DAQmxGetDigEdgeStartTrigDigFltrMinPulseWidth = 0;
decltype(&__7d6c5d3e_DAQmxGetDigEdgeStartTrigDigFltrTimebaseRate) DAQmxGetDigEdgeStartTrigDigFltrTimebaseRate = 0;
decltype(&__6e7b383f_DAQmxGetDigEdgeStartTrigDigFltrTimebaseSrc) DAQmxGetDigEdgeStartTrigDigFltrTimebaseSrc = 0;
decltype(&__0a7d4b66_DAQmxGetDigEdgeStartTrigDigSyncEnable) DAQmxGetDigEdgeStartTrigDigSyncEnable = 0;
decltype(&__797f4a79_DAQmxGetDigEdgeStartTrigEdge) DAQmxGetDigEdgeStartTrigEdge = 0;
decltype(&__6f694e1c_DAQmxGetDigEdgeStartTrigSrc) DAQmxGetDigEdgeStartTrigSrc = 0;
decltype(&__70755556_DAQmxGetDigEdgeWatchdogExpirTrigEdge) DAQmxGetDigEdgeWatchdogExpirTrigEdge = 0;
decltype(&__66635133_DAQmxGetDigEdgeWatchdogExpirTrigSrc) DAQmxGetDigEdgeWatchdogExpirTrigSrc = 0;
decltype(&__756e6e43_DAQmxGetDigLvlPauseTrigDigFltrEnable) DAQmxGetDigLvlPauseTrigDigFltrEnable = 0;
decltype(&__6d4d4024_DAQmxGetDigLvlPauseTrigDigFltrMinPulseWidth) DAQmxGetDigLvlPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__7e692321_DAQmxGetDigLvlPauseTrigDigFltrTimebaseRate) DAQmxGetDigLvlPauseTrigDigFltrTimebaseRate = 0;
decltype(&__690c2232_DAQmxGetDigLvlPauseTrigDigFltrTimebaseSrc) DAQmxGetDigLvlPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__6f7f7b56_DAQmxGetDigLvlPauseTrigDigSyncEnable) DAQmxGetDigLvlPauseTrigDigSyncEnable = 0;
decltype(&__7b7a0133_DAQmxGetDigLvlPauseTrigSrc) DAQmxGetDigLvlPauseTrigSrc = 0;
decltype(&__617c6f37_DAQmxGetDigLvlPauseTrigWhen) DAQmxGetDigLvlPauseTrigWhen = 0;
decltype(&__7f75012c_DAQmxGetDigPatternPauseTrigPattern) DAQmxGetDigPatternPauseTrigPattern = 0;
decltype(&__1e0c754a_DAQmxGetDigPatternPauseTrigSrc) DAQmxGetDigPatternPauseTrigSrc = 0;
decltype(&__040a1b4e_DAQmxGetDigPatternPauseTrigWhen) DAQmxGetDigPatternPauseTrigWhen = 0;
decltype(&__1e14143a_DAQmxGetDigPatternRefTrigPattern) DAQmxGetDigPatternRefTrigPattern = 0;
decltype(&__6a727543_DAQmxGetDigPatternRefTrigSrc) DAQmxGetDigPatternRefTrigSrc = 0;
decltype(&__04766f45_DAQmxGetDigPatternRefTrigWhen) DAQmxGetDigPatternRefTrigWhen = 0;
decltype(&__6b741339_DAQmxGetDigPatternStartTrigPattern) DAQmxGetDigPatternStartTrigPattern = 0;
decltype(&__0a0d675f_DAQmxGetDigPatternStartTrigSrc) DAQmxGetDigPatternStartTrigSrc = 0;
decltype(&__100b095b_DAQmxGetDigPatternStartTrigWhen) DAQmxGetDigPatternStartTrigWhen = 0;
decltype(&__3a226d6d_DAQmxGetDigitalLogicFamilyPowerUpState) DAQmxGetDigitalLogicFamilyPowerUpState = 0;
decltype(&__577d6c55_DAQmxGetDigitalPowerUpStates) DAQmxGetDigitalPowerUpStates = 0;
decltype(&__7f404e0f_DAQmxGetDigitalPullUpPullDownStates) DAQmxGetDigitalPullUpPullDownStates = 0;
decltype(&__4727113d_DAQmxGetDisconnectedCDAQSyncPorts) DAQmxGetDisconnectedCDAQSyncPorts = 0;
decltype(&__62495504_DAQmxGetErrorString) DAQmxGetErrorString = 0;
decltype(&__6f247774_DAQmxGetExported10MHzRefClkOutputTerm) DAQmxGetExported10MHzRefClkOutputTerm = 0;
decltype(&__56440864_DAQmxGetExported20MHzTimebaseOutputTerm) DAQmxGetExported20MHzTimebaseOutputTerm = 0;
decltype(&__5a7d4d31_DAQmxGetExportedAIConvClkOutputTerm) DAQmxGetExportedAIConvClkOutputTerm = 0;
decltype(&__2418767b_DAQmxGetExportedAIConvClkPulsePolarity) DAQmxGetExportedAIConvClkPulsePolarity = 0;
decltype(&__68421034_DAQmxGetExportedAIHoldCmpltEventOutputTerm) DAQmxGetExportedAIHoldCmpltEventOutputTerm = 0;
decltype(&__0d795a4a_DAQmxGetExportedAIHoldCmpltEventPulsePolarity) DAQmxGetExportedAIHoldCmpltEventPulsePolarity = 0;
decltype(&__77553b27_DAQmxGetExportedAdvCmpltEventDelay) DAQmxGetExportedAdvCmpltEventDelay = 0;
decltype(&__0320326b_DAQmxGetExportedAdvCmpltEventOutputTerm) DAQmxGetExportedAdvCmpltEventOutputTerm = 0;
decltype(&__7d450921_DAQmxGetExportedAdvCmpltEventPulsePolarity) DAQmxGetExportedAdvCmpltEventPulsePolarity = 0;
decltype(&__0129144e_DAQmxGetExportedAdvCmpltEventPulseWidth) DAQmxGetExportedAdvCmpltEventPulseWidth = 0;
decltype(&__5427250e_DAQmxGetExportedAdvTrigOutputTerm) DAQmxGetExportedAdvTrigOutputTerm = 0;
decltype(&__6f6d5b6b_DAQmxGetExportedAdvTrigPulsePolarity) DAQmxGetExportedAdvTrigPulsePolarity = 0;
decltype(&__72022707_DAQmxGetExportedAdvTrigPulseWidth) DAQmxGetExportedAdvTrigPulseWidth = 0;
decltype(&__0624496e_DAQmxGetExportedAdvTrigPulseWidthUnits) DAQmxGetExportedAdvTrigPulseWidthUnits = 0;
decltype(&__7f5c6e23_DAQmxGetExportedChangeDetectEventOutputTerm) DAQmxGetExportedChangeDetectEventOutputTerm = 0;
decltype(&__01395569_DAQmxGetExportedChangeDetectEventPulsePolarity) DAQmxGetExportedChangeDetectEventPulsePolarity = 0;
decltype(&__4a241e16_DAQmxGetExportedCtrOutEventOutputBehavior) DAQmxGetExportedCtrOutEventOutputBehavior = 0;
decltype(&__34447763_DAQmxGetExportedCtrOutEventOutputTerm) DAQmxGetExportedCtrOutEventOutputTerm = 0;
decltype(&__0f0e0906_DAQmxGetExportedCtrOutEventPulsePolarity) DAQmxGetExportedCtrOutEventPulsePolarity = 0;
decltype(&__437c151e_DAQmxGetExportedCtrOutEventToggleIdleState) DAQmxGetExportedCtrOutEventToggleIdleState = 0;
decltype(&__53704b26_DAQmxGetExportedDataActiveEventLvlActiveLvl) DAQmxGetExportedDataActiveEventLvlActiveLvl = 0;
decltype(&__71230524_DAQmxGetExportedDataActiveEventOutputTerm) DAQmxGetExportedDataActiveEventOutputTerm = 0;
decltype(&__18153d0f_DAQmxGetExportedDividedSampClkTimebaseOutputTerm) DAQmxGetExportedDividedSampClkTimebaseOutputTerm = 0;
decltype(&__1332497b_DAQmxGetExportedHshkEventDelay) DAQmxGetExportedHshkEventDelay = 0;
decltype(&__602c1611_DAQmxGetExportedHshkEventInterlockedAssertOnStart) DAQmxGetExportedHshkEventInterlockedAssertOnStart = 0;
decltype(&__0b2e3169_DAQmxGetExportedHshkEventInterlockedAssertedLvl) DAQmxGetExportedHshkEventInterlockedAssertedLvl = 0;
decltype(&__7e3a340e_DAQmxGetExportedHshkEventInterlockedDeassertDelay) DAQmxGetExportedHshkEventInterlockedDeassertDelay = 0;
decltype(&__0e323e57_DAQmxGetExportedHshkEventOutputBehavior) DAQmxGetExportedHshkEventOutputBehavior = 0;
decltype(&__67474037_DAQmxGetExportedHshkEventOutputTerm) DAQmxGetExportedHshkEventOutputTerm = 0;
decltype(&__19227b7d_DAQmxGetExportedHshkEventPulsePolarity) DAQmxGetExportedHshkEventPulsePolarity = 0;
decltype(&__654e6612_DAQmxGetExportedHshkEventPulseWidth) DAQmxGetExportedHshkEventPulseWidth = 0;
decltype(&__0275687b_DAQmxGetExportedPauseTrigLvlActiveLvl) DAQmxGetExportedPauseTrigLvlActiveLvl = 0;
decltype(&__4c774a28_DAQmxGetExportedPauseTrigOutputTerm) DAQmxGetExportedPauseTrigOutputTerm = 0;
decltype(&__6b45624a_DAQmxGetExportedRdyForStartEventLvlActiveLvl) DAQmxGetExportedRdyForStartEventLvlActiveLvl = 0;
decltype(&__69673104_DAQmxGetExportedRdyForStartEventOutputTerm) DAQmxGetExportedRdyForStartEventOutputTerm = 0;
decltype(&__597d5230_DAQmxGetExportedRdyForXferEventDeassertCond) DAQmxGetExportedRdyForXferEventDeassertCond = 0;
decltype(&__484d2601_DAQmxGetExportedRdyForXferEventDeassertCondCustomThreshold) DAQmxGetExportedRdyForXferEventDeassertCondCustomThreshold = 0;
decltype(&__78736a0e_DAQmxGetExportedRdyForXferEventLvlActiveLvl) DAQmxGetExportedRdyForXferEventLvlActiveLvl = 0;
decltype(&__5a20240c_DAQmxGetExportedRdyForXferEventOutputTerm) DAQmxGetExportedRdyForXferEventOutputTerm = 0;
decltype(&__4726350e_DAQmxGetExportedRefTrigOutputTerm) DAQmxGetExportedRefTrigOutputTerm = 0;
decltype(&__7c6c4b6b_DAQmxGetExportedRefTrigPulsePolarity) DAQmxGetExportedRefTrigPulsePolarity = 0;
decltype(&__5479202c_DAQmxGetExportedSampClkDelayOffset) DAQmxGetExportedSampClkDelayOffset = 0;
decltype(&__09475b5f_DAQmxGetExportedSampClkOutputBehavior) DAQmxGetExportedSampClkOutputBehavior = 0;
decltype(&__7727322a_DAQmxGetExportedSampClkOutputTerm) DAQmxGetExportedSampClkOutputTerm = 0;
decltype(&__4c6d4c4f_DAQmxGetExportedSampClkPulsePolarity) DAQmxGetExportedSampClkPulsePolarity = 0;
decltype(&__7f39321c_DAQmxGetExportedSampClkTimebaseOutputTerm) DAQmxGetExportedSampClkTimebaseOutputTerm = 0;
decltype(&__38090b6a_DAQmxGetExportedSignalAttribute) DAQmxGetExportedSignalAttribute = 0;
decltype(&__5e625e29_DAQmxGetExportedStartTrigOutputTerm) DAQmxGetExportedStartTrigOutputTerm = 0;
decltype(&__20076563_DAQmxGetExportedStartTrigPulsePolarity) DAQmxGetExportedStartTrigPulsePolarity = 0;
decltype(&__23221d0b_DAQmxGetExportedSyncPulseEventOutputTerm) DAQmxGetExportedSyncPulseEventOutputTerm = 0;
decltype(&__3d1b605b_DAQmxGetExportedWatchdogExpiredEventOutputTerm) DAQmxGetExportedWatchdogExpiredEventOutputTerm = 0;
decltype(&__1e576059_DAQmxGetExtCalLastDateAndTime) DAQmxGetExtCalLastDateAndTime = 0;
decltype(&__0616585e_DAQmxGetExtCalLastTemp) DAQmxGetExtCalLastTemp = 0;
decltype(&__73280322_DAQmxGetExtCalRecommendedInterval) DAQmxGetExtCalRecommendedInterval = 0;
decltype(&__4f213911_DAQmxGetExtendedErrorInfo) DAQmxGetExtendedErrorInfo = 0;
decltype(&__79426d72_DAQmxGetFieldDAQBankDevNames) DAQmxGetFieldDAQBankDevNames = 0;
decltype(&__3b23036a_DAQmxGetFieldDAQDevName) DAQmxGetFieldDAQDevName = 0;
decltype(&__546d2d1f_DAQmxGetFirstSampClkOffset) DAQmxGetFirstSampClkOffset = 0;
decltype(&__3c754765_DAQmxGetFirstSampClkTimescale) DAQmxGetFirstSampClkTimescale = 0;
decltype(&__29172e02_DAQmxGetFirstSampClkWhen) DAQmxGetFirstSampClkWhen = 0;
decltype(&__17091300_DAQmxGetFirstSampTimestampEnable) DAQmxGetFirstSampTimestampEnable = 0;
decltype(&__7a627801_DAQmxGetFirstSampTimestampTimescale) DAQmxGetFirstSampTimestampTimescale = 0;
decltype(&__1a6b6c6a_DAQmxGetFirstSampTimestampVal) DAQmxGetFirstSampTimestampVal = 0;
decltype(&__49510e01_DAQmxGetHshkDelayAfterXfer) DAQmxGetHshkDelayAfterXfer = 0;
decltype(&__3215037f_DAQmxGetHshkSampleInputDataWhen) DAQmxGetHshkSampleInputDataWhen = 0;
decltype(&__3742526e_DAQmxGetHshkStartCond) DAQmxGetHshkStartCond = 0;
decltype(&__747e4570_DAQmxGetHshkTrigType) DAQmxGetHshkTrigType = 0;
decltype(&__5f34290c_DAQmxGetImplicitUnderflowBehavior) DAQmxGetImplicitUnderflowBehavior = 0;
decltype(&__1c1a467c_DAQmxGetInterlockedHshkTrigAssertedLvl) DAQmxGetInterlockedHshkTrigAssertedLvl = 0;
decltype(&__1f034750_DAQmxGetInterlockedHshkTrigSrc) DAQmxGetInterlockedHshkTrigSrc = 0;
decltype(&__111f3968_DAQmxGetLoggingFilePath) DAQmxGetLoggingFilePath = 0;
decltype(&__4961466e_DAQmxGetLoggingFilePreallocationSize) DAQmxGetLoggingFilePreallocationSize = 0;
decltype(&__516b5f6f_DAQmxGetLoggingFileWriteSize) DAQmxGetLoggingFileWriteSize = 0;
decltype(&__76635133_DAQmxGetLoggingMode) DAQmxGetLoggingMode = 0;
decltype(&__7872474b_DAQmxGetLoggingPause) DAQmxGetLoggingPause = 0;
decltype(&__41635318_DAQmxGetLoggingSampsPerFile) DAQmxGetLoggingSampsPerFile = 0;
decltype(&__61447f78_DAQmxGetLoggingTDMSGroupName) DAQmxGetLoggingTDMSGroupName = 0;
decltype(&__59467a6a_DAQmxGetLoggingTDMSOperation) DAQmxGetLoggingTDMSOperation = 0;
decltype(&__7e702304_DAQmxGetMasterTimebaseRate) DAQmxGetMasterTimebaseRate = 0;
decltype(&__69152217_DAQmxGetMasterTimebaseSrc) DAQmxGetMasterTimebaseSrc = 0;
decltype(&__1e0c5960_DAQmxGetNthTaskChannel) DAQmxGetNthTaskChannel = 0;
decltype(&__13775e6a_DAQmxGetNthTaskDevice) DAQmxGetNthTaskDevice = 0;
decltype(&__7b6d3d32_DAQmxGetNthTaskReadChannel) DAQmxGetNthTaskReadChannel = 0;
decltype(&__64564e7b_DAQmxGetOnDemandSimultaneousAOEnable) DAQmxGetOnDemandSimultaneousAOEnable = 0;
decltype(&__03675671_DAQmxGetPauseTrigTerm) DAQmxGetPauseTrigTerm = 0;
decltype(&__0b674a73_DAQmxGetPauseTrigType) DAQmxGetPauseTrigType = 0;
decltype(&__19446878_DAQmxGetPersistedChanAllowInteractiveDeletion) DAQmxGetPersistedChanAllowInteractiveDeletion = 0;
decltype(&__66586e75_DAQmxGetPersistedChanAllowInteractiveEditing) DAQmxGetPersistedChanAllowInteractiveEditing = 0;
decltype(&__091e4c6f_DAQmxGetPersistedChanAttribute) DAQmxGetPersistedChanAttribute = 0;
decltype(&__677d5d1a_DAQmxGetPersistedChanAuthor) DAQmxGetPersistedChanAuthor = 0;
decltype(&__1b30465c_DAQmxGetPersistedScaleAllowInteractiveDeletion) DAQmxGetPersistedScaleAllowInteractiveDeletion = 0;
decltype(&__164f5a5a_DAQmxGetPersistedScaleAllowInteractiveEditing) DAQmxGetPersistedScaleAllowInteractiveEditing = 0;
decltype(&__0c201c78_DAQmxGetPersistedScaleAttribute) DAQmxGetPersistedScaleAttribute = 0;
decltype(&__794e7f69_DAQmxGetPersistedScaleAuthor) DAQmxGetPersistedScaleAuthor = 0;
decltype(&__1c53616a_DAQmxGetPersistedTaskAllowInteractiveDeletion) DAQmxGetPersistedTaskAllowInteractiveDeletion = 0;
decltype(&__634f6767_DAQmxGetPersistedTaskAllowInteractiveEditing) DAQmxGetPersistedTaskAllowInteractiveEditing = 0;
decltype(&__0c09457d_DAQmxGetPersistedTaskAttribute) DAQmxGetPersistedTaskAttribute = 0;
decltype(&__626a5408_DAQmxGetPersistedTaskAuthor) DAQmxGetPersistedTaskAuthor = 0;
decltype(&__053a0355_DAQmxGetPhysicalChanAIInputSrcs) DAQmxGetPhysicalChanAIInputSrcs = 0;
decltype(&__1129323d_DAQmxGetPhysicalChanAIPowerControlEnable) DAQmxGetPhysicalChanAIPowerControlEnable = 0;
decltype(&__002e4f4f_DAQmxGetPhysicalChanAIPowerControlType) DAQmxGetPhysicalChanAIPowerControlType = 0;
decltype(&__793f2c3e_DAQmxGetPhysicalChanAIPowerControlVoltage) DAQmxGetPhysicalChanAIPowerControlVoltage = 0;
decltype(&__4b3c1e1e_DAQmxGetPhysicalChanAISensorPowerOpenChan) DAQmxGetPhysicalChanAISensorPowerOpenChan = 0;
decltype(&__4b796b79_DAQmxGetPhysicalChanAISensorPowerOvercurrent) DAQmxGetPhysicalChanAISensorPowerOvercurrent = 0;
decltype(&__2e177f6a_DAQmxGetPhysicalChanAISensorPowerTypes) DAQmxGetPhysicalChanAISensorPowerTypes = 0;
decltype(&__7b300118_DAQmxGetPhysicalChanAISensorPowerVoltageRangeVals) DAQmxGetPhysicalChanAISensorPowerVoltageRangeVals = 0;
decltype(&__1f280305_DAQmxGetPhysicalChanAISupportedMeasTypes) DAQmxGetPhysicalChanAISupportedMeasTypes = 0;
decltype(&__12325a6b_DAQmxGetPhysicalChanAITermCfgs) DAQmxGetPhysicalChanAITermCfgs = 0;
decltype(&__415a6b5c_DAQmxGetPhysicalChanAOManualControlAmplitude) DAQmxGetPhysicalChanAOManualControlAmplitude = 0;
decltype(&__533e0138_DAQmxGetPhysicalChanAOManualControlEnable) DAQmxGetPhysicalChanAOManualControlEnable = 0;
decltype(&__2a3a1257_DAQmxGetPhysicalChanAOManualControlFreq) DAQmxGetPhysicalChanAOManualControlFreq = 0;
decltype(&__17210037_DAQmxGetPhysicalChanAOManualControlShortDetected) DAQmxGetPhysicalChanAOManualControlShortDetected = 0;
decltype(&__676c1c25_DAQmxGetPhysicalChanAOSupportedOutputTypes) DAQmxGetPhysicalChanAOSupportedOutputTypes = 0;
decltype(&__6a313a29_DAQmxGetPhysicalChanAOSupportedPowerUpOutputTypes) DAQmxGetPhysicalChanAOSupportedPowerUpOutputTypes = 0;
decltype(&__12345a6b_DAQmxGetPhysicalChanAOTermCfgs) DAQmxGetPhysicalChanAOTermCfgs = 0;
decltype(&__0b734c6e_DAQmxGetPhysicalChanAttribute) DAQmxGetPhysicalChanAttribute = 0;
decltype(&__1d280305_DAQmxGetPhysicalChanCISupportedMeasTypes) DAQmxGetPhysicalChanCISupportedMeasTypes = 0;
decltype(&__656c1c25_DAQmxGetPhysicalChanCOSupportedOutputTypes) DAQmxGetPhysicalChanCOSupportedOutputTypes = 0;
decltype(&__40464507_DAQmxGetPhysicalChanDIChangeDetectSupported) DAQmxGetPhysicalChanDIChangeDetectSupported = 0;
decltype(&__142c226e_DAQmxGetPhysicalChanDIPortWidth) DAQmxGetPhysicalChanDIPortWidth = 0;
decltype(&__11045a61_DAQmxGetPhysicalChanDISampClkSupported) DAQmxGetPhysicalChanDISampClkSupported = 0;
decltype(&__0b392066_DAQmxGetPhysicalChanDISampModes) DAQmxGetPhysicalChanDISampModes = 0;
decltype(&__142a226e_DAQmxGetPhysicalChanDOPortWidth) DAQmxGetPhysicalChanDOPortWidth = 0;
decltype(&__11025a61_DAQmxGetPhysicalChanDOSampClkSupported) DAQmxGetPhysicalChanDOSampClkSupported = 0;
decltype(&__0b3f2066_DAQmxGetPhysicalChanDOSampModes) DAQmxGetPhysicalChanDOSampModes = 0;
decltype(&__0804200d_DAQmxGetPhysicalChanName) DAQmxGetPhysicalChanName = 0;
decltype(&__493b1809_DAQmxGetPhysicalChanTEDSBitStream) DAQmxGetPhysicalChanTEDSBitStream = 0;
decltype(&__1b466e72_DAQmxGetPhysicalChanTEDSMfgID) DAQmxGetPhysicalChanTEDSMfgID = 0;
decltype(&__33011833_DAQmxGetPhysicalChanTEDSModelNum) DAQmxGetPhysicalChanTEDSModelNum = 0;
decltype(&__4d293527_DAQmxGetPhysicalChanTEDSSerialNum) DAQmxGetPhysicalChanTEDSSerialNum = 0;
decltype(&__6360632e_DAQmxGetPhysicalChanTEDSTemplateIDs) DAQmxGetPhysicalChanTEDSTemplateIDs = 0;
decltype(&__3a5e6161_DAQmxGetPhysicalChanTEDSVersionLetter) DAQmxGetPhysicalChanTEDSVersionLetter = 0;
decltype(&__58471506_DAQmxGetPhysicalChanTEDSVersionNum) DAQmxGetPhysicalChanTEDSVersionNum = 0;
decltype(&__76730e21_DAQmxGetPossibleSCExpressCalAccConnections) DAQmxGetPossibleSCExpressCalAccConnections = 0;
decltype(&__1a330c4a_DAQmxGetReadAccessoryInsertionOrRemovalDetected) DAQmxGetReadAccessoryInsertionOrRemovalDetected = 0;
decltype(&__2375547b_DAQmxGetReadAttribute) DAQmxGetReadAttribute = 0;
decltype(&__08624060_DAQmxGetReadAutoStart) DAQmxGetReadAutoStart = 0;
decltype(&__707e5165_DAQmxGetReadAvailSampPerChan) DAQmxGetReadAvailSampPerChan = 0;
decltype(&__0860654e_DAQmxGetReadChangeDetectHasOverflowed) DAQmxGetReadChangeDetectHasOverflowed = 0;
decltype(&__76650a05_DAQmxGetReadChannelsToRead) DAQmxGetReadChannelsToRead = 0;
decltype(&__0a7a4c47_DAQmxGetReadCommonModeRangeErrorChans) DAQmxGetReadCommonModeRangeErrorChans = 0;
decltype(&__794b342e_DAQmxGetReadCommonModeRangeErrorChansExist) DAQmxGetReadCommonModeRangeErrorChansExist = 0;
decltype(&__2f1c356b_DAQmxGetReadCurrReadPos) DAQmxGetReadCurrReadPos = 0;
decltype(&__17331018_DAQmxGetReadDevsWithInsertedOrRemovedAccessories) DAQmxGetReadDevsWithInsertedOrRemovedAccessories = 0;
decltype(&__45444b52_DAQmxGetReadDigitalLinesBytesPerChan) DAQmxGetReadDigitalLinesBytesPerChan = 0;
decltype(&__52476709_DAQmxGetReadExcitFaultChans) DAQmxGetReadExcitFaultChans = 0;
decltype(&__2a2e1438_DAQmxGetReadExcitFaultChansExist) DAQmxGetReadExcitFaultChansExist = 0;
decltype(&__6b283239_DAQmxGetReadInputLimitsFaultChans) DAQmxGetReadInputLimitsFaultChans = 0;
decltype(&__18194a50_DAQmxGetReadInputLimitsFaultChansExist) DAQmxGetReadInputLimitsFaultChansExist = 0;
decltype(&__4877564d_DAQmxGetReadNumChans) DAQmxGetReadNumChans = 0;
decltype(&__4471330e_DAQmxGetReadOffset) DAQmxGetReadOffset = 0;
decltype(&__117b517d_DAQmxGetReadOpenChans) DAQmxGetReadOpenChans = 0;
decltype(&__7056587a_DAQmxGetReadOpenChansDetails) DAQmxGetReadOpenChansDetails = 0;
decltype(&__624a2914_DAQmxGetReadOpenChansExist) DAQmxGetReadOpenChansExist = 0;
decltype(&__0006281d_DAQmxGetReadOpenCurrentLoopChans) DAQmxGetReadOpenCurrentLoopChans = 0;
decltype(&__317e416e_DAQmxGetReadOpenCurrentLoopChansExist) DAQmxGetReadOpenCurrentLoopChansExist = 0;
decltype(&__7e6a404e_DAQmxGetReadOpenThrmcplChans) DAQmxGetReadOpenThrmcplChans = 0;
decltype(&__4f12293d_DAQmxGetReadOpenThrmcplChansExist) DAQmxGetReadOpenThrmcplChansExist = 0;
decltype(&__1367597b_DAQmxGetReadOverWrite) DAQmxGetReadOverWrite = 0;
decltype(&__4f6f584d_DAQmxGetReadOvercurrentChans) DAQmxGetReadOvercurrentChans = 0;
decltype(&__7e17313e_DAQmxGetReadOvercurrentChansExist) DAQmxGetReadOvercurrentChansExist = 0;
decltype(&__49706103_DAQmxGetReadOverloadedChans) DAQmxGetReadOverloadedChans = 0;
decltype(&__31191232_DAQmxGetReadOverloadedChansExist) DAQmxGetReadOverloadedChansExist = 0;
decltype(&__2d11373b_DAQmxGetReadOvertemperatureChans) DAQmxGetReadOvertemperatureChans = 0;
decltype(&__1c695e48_DAQmxGetReadOvertemperatureChansExist) DAQmxGetReadOvertemperatureChansExist = 0;
decltype(&__53477c7b_DAQmxGetReadPLLUnlockedChans) DAQmxGetReadPLLUnlockedChans = 0;
decltype(&__623f1508_DAQmxGetReadPLLUnlockedChansExist) DAQmxGetReadPLLUnlockedChansExist = 0;
decltype(&__6d2e2334_DAQmxGetReadPowerSupplyFaultChans) DAQmxGetReadPowerSupplyFaultChans = 0;
decltype(&__1e1f5b5d_DAQmxGetReadPowerSupplyFaultChansExist) DAQmxGetReadPowerSupplyFaultChansExist = 0;
decltype(&__34123706_DAQmxGetReadRawDataWidth) DAQmxGetReadRawDataWidth = 0;
decltype(&__586a5c44_DAQmxGetReadReadAllAvailSamp) DAQmxGetReadReadAllAvailSamp = 0;
decltype(&__1c004f79_DAQmxGetReadRelativeTo) DAQmxGetReadRelativeTo = 0;
decltype(&__285b5975_DAQmxGetReadSleepTime) DAQmxGetReadSleepTime = 0;
decltype(&__3b77537b_DAQmxGetReadSyncUnlockedChans) DAQmxGetReadSyncUnlockedChans = 0;
decltype(&__48462b12_DAQmxGetReadSyncUnlockedChansExist) DAQmxGetReadSyncUnlockedChansExist = 0;
decltype(&__4d76507c_DAQmxGetReadTotalSampPerChanAcquired) DAQmxGetReadTotalSampPerChanAcquired = 0;
decltype(&__746d586c_DAQmxGetReadWaitMode) DAQmxGetReadWaitMode = 0;
decltype(&__771c3916_DAQmxGetRealTimeAttribute) DAQmxGetRealTimeAttribute = 0;
decltype(&__3c0a111e_DAQmxGetRealTimeConvLateErrorsToWarnings) DAQmxGetRealTimeConvLateErrorsToWarnings = 0;
decltype(&__0b383617_DAQmxGetRealTimeNumOfWarmupIters) DAQmxGetRealTimeNumOfWarmupIters = 0;
decltype(&__3a090d02_DAQmxGetRealTimeReportMissedSamp) DAQmxGetRealTimeReportMissedSamp = 0;
decltype(&__4e6b3d34_DAQmxGetRealTimeWaitForNextSampClkWaitMode) DAQmxGetRealTimeWaitForNextSampClkWaitMode = 0;
decltype(&__7b113e11_DAQmxGetRealTimeWriteRecoveryMode) DAQmxGetRealTimeWriteRecoveryMode = 0;
decltype(&__766d003b_DAQmxGetRefClkRate) DAQmxGetRefClkRate = 0;
decltype(&__61080128_DAQmxGetRefClkSrc) DAQmxGetRefClkSrc = 0;
decltype(&__10765f71_DAQmxGetRefTrigAutoTrigEnable) DAQmxGetRefTrigAutoTrigEnable = 0;
decltype(&__7e65585b_DAQmxGetRefTrigAutoTriggered) DAQmxGetRefTrigAutoTriggered = 0;
decltype(&__79665470_DAQmxGetRefTrigDelay) DAQmxGetRefTrigDelay = 0;
decltype(&__50442916_DAQmxGetRefTrigMaxNumTrigsToDetect) DAQmxGetRefTrigMaxNumTrigsToDetect = 0;
decltype(&__19787e6b_DAQmxGetRefTrigPretrigSamples) DAQmxGetRefTrigPretrigSamples = 0;
decltype(&__49704c04_DAQmxGetRefTrigRetriggerWin) DAQmxGetRefTrigRetriggerWin = 0;
decltype(&__7f7b4e61_DAQmxGetRefTrigRetriggerable) DAQmxGetRefTrigRetriggerable = 0;
decltype(&__79785819_DAQmxGetRefTrigTerm) DAQmxGetRefTrigTerm = 0;
decltype(&__280d5c78_DAQmxGetRefTrigTimestampEnable) DAQmxGetRefTrigTimestampEnable = 0;
decltype(&__430c3113_DAQmxGetRefTrigTimestampTimescale) DAQmxGetRefTrigTimestampTimescale = 0;
decltype(&__5767511a_DAQmxGetRefTrigTimestampVal) DAQmxGetRefTrigTimestampVal = 0;
decltype(&__070d524e_DAQmxGetRefTrigTrigWin) DAQmxGetRefTrigTrigWin = 0;
decltype(&__657a5019_DAQmxGetRefTrigType) DAQmxGetRefTrigType = 0;
decltype(&__4f3a3f39_DAQmxGetSampClkActiveEdge) DAQmxGetSampClkActiveEdge = 0;
decltype(&__507c5d4a_DAQmxGetSampClkDigFltrEnable) DAQmxGetSampClkDigFltrEnable = 0;
decltype(&__485f732d_DAQmxGetSampClkDigFltrMinPulseWidth) DAQmxGetSampClkDigFltrMinPulseWidth = 0;
decltype(&__5b7b1028_DAQmxGetSampClkDigFltrTimebaseRate) DAQmxGetSampClkDigFltrTimebaseRate = 0;
decltype(&__4c1e113b_DAQmxGetSampClkDigFltrTimebaseSrc) DAQmxGetSampClkDigFltrTimebaseSrc = 0;
decltype(&__4a6d485f_DAQmxGetSampClkDigSyncEnable) DAQmxGetSampClkDigSyncEnable = 0;
decltype(&__39166045_DAQmxGetSampClkMaxRate) DAQmxGetSampClkMaxRate = 0;
decltype(&__28137458_DAQmxGetSampClkOverrunBehavior) DAQmxGetSampClkOverrunBehavior = 0;
decltype(&__4d7f573b_DAQmxGetSampClkRate) DAQmxGetSampClkRate = 0;
decltype(&__5e68323a_DAQmxGetSampClkSrc) DAQmxGetSampClkSrc = 0;
decltype(&__49795f3d_DAQmxGetSampClkTerm) DAQmxGetSampClkTerm = 0;
decltype(&__47243f0f_DAQmxGetSampClkTimebaseActiveEdge) DAQmxGetSampClkTimebaseActiveEdge = 0;
decltype(&__4d63321b_DAQmxGetSampClkTimebaseDiv) DAQmxGetSampClkTimebaseDiv = 0;
decltype(&__3714271f_DAQmxGetSampClkTimebaseMasterTimebaseDiv) DAQmxGetSampClkTimebaseMasterTimebaseDiv = 0;
decltype(&__4561570d_DAQmxGetSampClkTimebaseRate) DAQmxGetSampClkTimebaseRate = 0;
decltype(&__5676320c_DAQmxGetSampClkTimebaseSrc) DAQmxGetSampClkTimebaseSrc = 0;
decltype(&__41675f0b_DAQmxGetSampClkTimebaseTerm) DAQmxGetSampClkTimebaseTerm = 0;
decltype(&__52163f37_DAQmxGetSampClkTimingResponseMode) DAQmxGetSampClkTimingResponseMode = 0;
decltype(&__100f3f2a_DAQmxGetSampClkUnderflowBehavior) DAQmxGetSampClkUnderflowBehavior = 0;
decltype(&__3b166d6c_DAQmxGetSampClkWriteWfmUseInitialWfmDT) DAQmxGetSampClkWriteWfmUseInitialWfmDT = 0;
decltype(&__5f0c360e_DAQmxGetSampQuantSampMode) DAQmxGetSampQuantSampMode = 0;
decltype(&__79795d76_DAQmxGetSampQuantSampPerChan) DAQmxGetSampQuantSampPerChan = 0;
decltype(&__32001f0b_DAQmxGetSampTimingEngine) DAQmxGetSampTimingEngine = 0;
decltype(&__250c6079_DAQmxGetSampTimingType) DAQmxGetSampTimingType = 0;
decltype(&__0c284374_DAQmxGetScaleAttribute) DAQmxGetScaleAttribute = 0;
decltype(&__69533006_DAQmxGetScaleDescr) DAQmxGetScaleDescr = 0;
decltype(&__3c45536b_DAQmxGetScaleLinSlope) DAQmxGetScaleLinSlope = 0;
decltype(&__4666310a_DAQmxGetScaleLinYIntercept) DAQmxGetScaleLinYIntercept = 0;
decltype(&__5d765c4b_DAQmxGetScaleMapPreScaledMax) DAQmxGetScaleMapPreScaledMax = 0;
decltype(&__5d76545d_DAQmxGetScaleMapPreScaledMin) DAQmxGetScaleMapPreScaledMin = 0;
decltype(&__442f1808_DAQmxGetScaleMapScaledMax) DAQmxGetScaleMapScaledMax = 0;
decltype(&__522f1800_DAQmxGetScaleMapScaledMin) DAQmxGetScaleMapScaledMin = 0;
decltype(&__217d4269_DAQmxGetScalePolyForwardCoeff) DAQmxGetScalePolyForwardCoeff = 0;
decltype(&__337a496c_DAQmxGetScalePolyReverseCoeff) DAQmxGetScalePolyReverseCoeff = 0;
decltype(&__55466012_DAQmxGetScalePreScaledUnits) DAQmxGetScalePreScaledUnits = 0;
decltype(&__083a2632_DAQmxGetScaleScaledUnits) DAQmxGetScaleScaledUnits = 0;
decltype(&__0e0e1052_DAQmxGetScaleTablePreScaledVals) DAQmxGetScaleTablePreScaledVals = 0;
decltype(&__51596e63_DAQmxGetScaleTableScaledVals) DAQmxGetScaleTableScaledVals = 0;
decltype(&__6f312c05_DAQmxGetScaleType) DAQmxGetScaleType = 0;
decltype(&__2f04715f_DAQmxGetSelfCalLastDateAndTime) DAQmxGetSelfCalLastDateAndTime = 0;
decltype(&__281c3067_DAQmxGetSelfCalLastTemp) DAQmxGetSelfCalLastTemp = 0;
decltype(&__2b062127_DAQmxGetSelfCalSupported) DAQmxGetSelfCalSupported = 0;
decltype(&__1d1b426e_DAQmxGetStartTrigDelay) DAQmxGetStartTrigDelay = 0;
decltype(&__746f6400_DAQmxGetStartTrigDelayUnits) DAQmxGetStartTrigDelayUnits = 0;
decltype(&__607d6b4c_DAQmxGetStartTrigMaxNumTrigsToDetect) DAQmxGetStartTrigMaxNumTrigsToDetect = 0;
decltype(&__056f7278_DAQmxGetStartTrigRetriggerWin) DAQmxGetStartTrigRetriggerWin = 0;
decltype(&__070a4473_DAQmxGetStartTrigRetriggerable) DAQmxGetStartTrigRetriggerable = 0;
decltype(&__11724270_DAQmxGetStartTrigTerm) DAQmxGetStartTrigTerm = 0;
decltype(&__75642d0e_DAQmxGetStartTrigTimescale) DAQmxGetStartTrigTimescale = 0;
decltype(&__15131305_DAQmxGetStartTrigTimestampEnable) DAQmxGetStartTrigTimestampEnable = 0;
decltype(&__78787804_DAQmxGetStartTrigTimestampTimescale) DAQmxGetStartTrigTimestampTimescale = 0;
decltype(&__18716c6f_DAQmxGetStartTrigTimestampVal) DAQmxGetStartTrigTimestampVal = 0;
decltype(&__75253d0e_DAQmxGetStartTrigTrigWhen) DAQmxGetStartTrigTrigWhen = 0;
decltype(&__1b253c05_DAQmxGetStartTrigTrigWin) DAQmxGetStartTrigTrigWin = 0;
decltype(&__19725e72_DAQmxGetStartTrigType) DAQmxGetStartTrigType = 0;
decltype(&__29217a7a_DAQmxGetSwitchChanAnlgBusSharingEnable) DAQmxGetSwitchChanAnlgBusSharingEnable = 0;
decltype(&__6c715313_DAQmxGetSwitchChanAttribute) DAQmxGetSwitchChanAttribute = 0;
decltype(&__6767430d_DAQmxGetSwitchChanBandwidth) DAQmxGetSwitchChanBandwidth = 0;
decltype(&__73715609_DAQmxGetSwitchChanImpedance) DAQmxGetSwitchChanImpedance = 0;
decltype(&__525f642c_DAQmxGetSwitchChanMaxACCarryCurrent) DAQmxGetSwitchChanMaxACCarryCurrent = 0;
decltype(&__2433105e_DAQmxGetSwitchChanMaxACCarryPwr) DAQmxGetSwitchChanMaxACCarryPwr = 0;
decltype(&__78797f52_DAQmxGetSwitchChanMaxACSwitchCurrent) DAQmxGetSwitchChanMaxACSwitchCurrent = 0;
decltype(&__0a0f1326_DAQmxGetSwitchChanMaxACSwitchPwr) DAQmxGetSwitchChanMaxACSwitchPwr = 0;
decltype(&__1d3f6453_DAQmxGetSwitchChanMaxACVoltage) DAQmxGetSwitchChanMaxACVoltage = 0;
decltype(&__525a642c_DAQmxGetSwitchChanMaxDCCarryCurrent) DAQmxGetSwitchChanMaxDCCarryCurrent = 0;
decltype(&__2436105e_DAQmxGetSwitchChanMaxDCCarryPwr) DAQmxGetSwitchChanMaxDCCarryPwr = 0;
decltype(&__787c7f52_DAQmxGetSwitchChanMaxDCSwitchCurrent) DAQmxGetSwitchChanMaxDCSwitchCurrent = 0;
decltype(&__0a0a1326_DAQmxGetSwitchChanMaxDCSwitchPwr) DAQmxGetSwitchChanMaxDCSwitchPwr = 0;
decltype(&__1d3a6453_DAQmxGetSwitchChanMaxDCVoltage) DAQmxGetSwitchChanMaxDCVoltage = 0;
decltype(&__0c102e76_DAQmxGetSwitchChanUsage) DAQmxGetSwitchChanUsage = 0;
decltype(&__7b770403_DAQmxGetSwitchChanWireMode) DAQmxGetSwitchChanWireMode = 0;
decltype(&__1c18180d_DAQmxGetSwitchDevAutoConnAnlgBus) DAQmxGetSwitchDevAutoConnAnlgBus = 0;
decltype(&__54567310_DAQmxGetSwitchDevNumColumns) DAQmxGetSwitchDevNumColumns = 0;
decltype(&__51410004_DAQmxGetSwitchDevNumRelays) DAQmxGetSwitchDevNumRelays = 0;
decltype(&__28381b16_DAQmxGetSwitchDevNumRows) DAQmxGetSwitchDevNumRows = 0;
decltype(&__2b263579_DAQmxGetSwitchDevNumSwitchChans) DAQmxGetSwitchDevNumSwitchChans = 0;
decltype(&__2214043b_DAQmxGetSwitchDevPwrDownLatchRelaysAfterSettling) DAQmxGetSwitchDevPwrDownLatchRelaysAfterSettling = 0;
decltype(&__6846300d_DAQmxGetSwitchDevRelayList) DAQmxGetSwitchDevRelayList = 0;
decltype(&__0e261918_DAQmxGetSwitchDevSettled) DAQmxGetSwitchDevSettled = 0;
decltype(&__0c727c7f_DAQmxGetSwitchDevSettlingTime) DAQmxGetSwitchDevSettlingTime = 0;
decltype(&__0f3b1c6e_DAQmxGetSwitchDevSwitchChanList) DAQmxGetSwitchDevSwitchChanList = 0;
decltype(&__7e5d7c61_DAQmxGetSwitchDevTemperature) DAQmxGetSwitchDevTemperature = 0;
decltype(&__6c21191f_DAQmxGetSwitchDevTopology) DAQmxGetSwitchDevTopology = 0;
decltype(&__37667b6b_DAQmxGetSwitchDeviceAttribute) DAQmxGetSwitchDeviceAttribute = 0;
decltype(&__6c714318_DAQmxGetSwitchScanAttribute) DAQmxGetSwitchScanAttribute = 0;
decltype(&__67724231_DAQmxGetSwitchScanBreakMode) DAQmxGetSwitchScanBreakMode = 0;
decltype(&__507d597a_DAQmxGetSwitchScanRepeatMode) DAQmxGetSwitchScanRepeatMode = 0;
decltype(&__22212973_DAQmxGetSwitchScanWaitingForAdv) DAQmxGetSwitchScanWaitingForAdv = 0;
decltype(&__34063841_DAQmxGetSyncClkInterval) DAQmxGetSyncClkInterval = 0;
decltype(&__13172e21_DAQmxGetSyncPulseMinDelayToStart) DAQmxGetSyncPulseMinDelayToStart = 0;
decltype(&__534d6e1f_DAQmxGetSyncPulseResetDelay) DAQmxGetSyncPulseResetDelay = 0;
decltype(&__52490713_DAQmxGetSyncPulseResetTime) DAQmxGetSyncPulseResetTime = 0;
decltype(&__5a59446a_DAQmxGetSyncPulseSrc) DAQmxGetSyncPulseSrc = 0;
decltype(&__5c0d260a_DAQmxGetSyncPulseSyncTime) DAQmxGetSyncPulseSyncTime = 0;
decltype(&__375e537b_DAQmxGetSyncPulseTerm) DAQmxGetSyncPulseTerm = 0;
decltype(&__361c5568_DAQmxGetSyncPulseTimeTimescale) DAQmxGetSyncPulseTimeTimescale = 0;
decltype(&__51093701_DAQmxGetSyncPulseTimeWhen) DAQmxGetSyncPulseTimeWhen = 0;
decltype(&__3f5e4f79_DAQmxGetSyncPulseType) DAQmxGetSyncPulseType = 0;
decltype(&__676c7a3c_DAQmxGetSysDevNames) DAQmxGetSysDevNames = 0;
decltype(&__01204d5e_DAQmxGetSysGlobalChans) DAQmxGetSysGlobalChans = 0;
decltype(&__6a656675_DAQmxGetSysNIDAQMajorVersion) DAQmxGetSysNIDAQMajorVersion = 0;
decltype(&__6a6d6275_DAQmxGetSysNIDAQMinorVersion) DAQmxGetSysNIDAQMinorVersion = 0;
decltype(&__1b5d5d6d_DAQmxGetSysNIDAQUpdateVersion) DAQmxGetSysNIDAQUpdateVersion = 0;
decltype(&__7f1e2b2f_DAQmxGetSysScales) DAQmxGetSysScales = 0;
decltype(&__0e0c2c3e_DAQmxGetSysTasks) DAQmxGetSysTasks = 0;
decltype(&__6d7b4315_DAQmxGetSystemInfoAttribute) DAQmxGetSystemInfoAttribute = 0;
decltype(&__25714674_DAQmxGetTaskAttribute) DAQmxGetTaskAttribute = 0;
decltype(&__456a4a6f_DAQmxGetTaskChannels) DAQmxGetTaskChannels = 0;
decltype(&__476d5e67_DAQmxGetTaskComplete) DAQmxGetTaskComplete = 0;
decltype(&__4f67421b_DAQmxGetTaskDevices) DAQmxGetTaskDevices = 0;
decltype(&__26062a17_DAQmxGetTaskName) DAQmxGetTaskName = 0;
decltype(&__4e734442_DAQmxGetTaskNumChans) DAQmxGetTaskNumChans = 0;
decltype(&__26174355_DAQmxGetTaskNumDevices) DAQmxGetTaskNumDevices = 0;
decltype(&__070e1466_DAQmxGetTimingAttribute) DAQmxGetTimingAttribute = 0;
decltype(&__7f0e1423_DAQmxGetTimingAttributeEx) DAQmxGetTimingAttributeEx = 0;
decltype(&__25625c78_DAQmxGetTrigAttribute) DAQmxGetTrigAttribute = 0;
decltype(&__0f0f2979_DAQmxGetTriggerSyncType) DAQmxGetTriggerSyncType = 0;
decltype(&__46446c53_DAQmxGetWatchdogAOExpirState) DAQmxGetWatchdogAOExpirState = 0;
decltype(&__62456579_DAQmxGetWatchdogAOOutputType) DAQmxGetWatchdogAOOutputType = 0;
decltype(&__4e152e1b_DAQmxGetWatchdogAttribute) DAQmxGetWatchdogAttribute = 0;
decltype(&__44446c53_DAQmxGetWatchdogCOExpirState) DAQmxGetWatchdogCOExpirState = 0;
decltype(&__43446c53_DAQmxGetWatchdogDOExpirState) DAQmxGetWatchdogDOExpirState = 0;
decltype(&__3e364955_DAQmxGetWatchdogExpirTrigTrigOnNetworkConnLoss) DAQmxGetWatchdogExpirTrigTrigOnNetworkConnLoss = 0;
decltype(&__367b546d_DAQmxGetWatchdogExpirTrigType) DAQmxGetWatchdogExpirTrigType = 0;
decltype(&__5676352a_DAQmxGetWatchdogHasExpired) DAQmxGetWatchdogHasExpired = 0;
decltype(&__381f3678_DAQmxGetWatchdogTimeout) DAQmxGetWatchdogTimeout = 0;
decltype(&__39000d34_DAQmxGetWriteAccessoryInsertionOrRemovalDetected) DAQmxGetWriteAccessoryInsertionOrRemovalDetected = 0;
decltype(&__08394b6c_DAQmxGetWriteAttribute) DAQmxGetWriteAttribute = 0;
decltype(&__7b050a19_DAQmxGetWriteCurrWritePos) DAQmxGetWriteCurrWritePos = 0;
decltype(&__6b0d0d28_DAQmxGetWriteDevsWithInsertedOrRemovedAccessories) DAQmxGetWriteDevsWithInsertedOrRemovedAccessories = 0;
decltype(&__215f7a73_DAQmxGetWriteDigitalLinesBytesPerChan) DAQmxGetWriteDigitalLinesBytesPerChan = 0;
decltype(&__3173547a_DAQmxGetWriteExternalOvervoltageChans) DAQmxGetWriteExternalOvervoltageChans = 0;
decltype(&__42422c13_DAQmxGetWriteExternalOvervoltageChansExist) DAQmxGetWriteExternalOvervoltageChansExist = 0;
decltype(&__3e52496e_DAQmxGetWriteNumChans) DAQmxGetWriteNumChans = 0;
decltype(&__7d5e4f0b_DAQmxGetWriteOffset) DAQmxGetWriteOffset = 0;
decltype(&__6e1a3810_DAQmxGetWriteOpenCurrentLoopChans) DAQmxGetWriteOpenCurrentLoopChans = 0;
decltype(&__1d2b4079_DAQmxGetWriteOpenCurrentLoopChansExist) DAQmxGetWriteOpenCurrentLoopChansExist = 0;
decltype(&__3e555160_DAQmxGetWriteOvercurrentChans) DAQmxGetWriteOvercurrentChans = 0;
decltype(&__4d642909_DAQmxGetWriteOvercurrentChansExist) DAQmxGetWriteOvercurrentChansExist = 0;
decltype(&__70534e59_DAQmxGetWriteOverloadedChans) DAQmxGetWriteOverloadedChans = 0;
decltype(&__412b272a_DAQmxGetWriteOverloadedChansExist) DAQmxGetWriteOverloadedChansExist = 0;
decltype(&__48372f0f_DAQmxGetWriteOvertemperatureChans) DAQmxGetWriteOvertemperatureChans = 0;
decltype(&__3b065766_DAQmxGetWriteOvertemperatureChansExist) DAQmxGetWriteOvertemperatureChansExist = 0;
decltype(&__4777101b_DAQmxGetWritePowerSupplyFaultChans) DAQmxGetWritePowerSupplyFaultChans = 0;
decltype(&__2e042163_DAQmxGetWritePowerSupplyFaultChansExist) DAQmxGetWritePowerSupplyFaultChansExist = 0;
decltype(&__752e2c0f_DAQmxGetWriteRawDataWidth) DAQmxGetWriteRawDataWidth = 0;
decltype(&__0f2d7565_DAQmxGetWriteRegenMode) DAQmxGetWriteRegenMode = 0;
decltype(&__0a063e77_DAQmxGetWriteRelativeTo) DAQmxGetWriteRelativeTo = 0;
decltype(&__06326561_DAQmxGetWriteSleepTime) DAQmxGetWriteSleepTime = 0;
decltype(&__0c2b007a_DAQmxGetWriteSpaceAvail) DAQmxGetWriteSpaceAvail = 0;
decltype(&__0821496b_DAQmxGetWriteSyncUnlockedChans) DAQmxGetWriteSyncUnlockedChans = 0;
decltype(&__61527813_DAQmxGetWriteSyncUnlockedChansExist) DAQmxGetWriteSyncUnlockedChansExist = 0;
decltype(&__1e2e5d66_DAQmxGetWriteTotalSampPerChanGenerated) DAQmxGetWriteTotalSampPerChanGenerated = 0;
decltype(&__1f6e5360_DAQmxGetWriteWaitMode) DAQmxGetWriteWaitMode = 0;
decltype(&__0b2c2b70_DAQmxInitExtCal) DAQmxInitExtCal = 0;
decltype(&__2b3e7878_DAQmxIsReadOrWriteLate) DAQmxIsReadOrWriteLate = 0;
decltype(&__32152c7d_DAQmxIsTaskDone) DAQmxIsTaskDone = 0;
decltype(&__33595f7f_DAQmxLoadTask) DAQmxLoadTask = 0;
decltype(&__1d6e7e49_DAQmxMSeriesCalAdjust) DAQmxMSeriesCalAdjust = 0;
decltype(&__5f6d0e2b_DAQmxPerformBridgeOffsetNullingCal) DAQmxPerformBridgeOffsetNullingCal = 0;
decltype(&__5f6d4b53_DAQmxPerformBridgeOffsetNullingCalEx) DAQmxPerformBridgeOffsetNullingCalEx = 0;
decltype(&__6b6b083d_DAQmxPerformBridgeShuntCal) DAQmxPerformBridgeShuntCal = 0;
decltype(&__6b6b4d45_DAQmxPerformBridgeShuntCalEx) DAQmxPerformBridgeShuntCalEx = 0;
decltype(&__74661338_DAQmxPerformStrainShuntCal) DAQmxPerformStrainShuntCal = 0;
decltype(&__74665640_DAQmxPerformStrainShuntCalEx) DAQmxPerformStrainShuntCalEx = 0;
decltype(&__35273872_DAQmxPerformThrmcplLeadOffsetNullingCal) DAQmxPerformThrmcplLeadOffsetNullingCal = 0;
decltype(&__02093d2b_DAQmxReadAnalogF64) DAQmxReadAnalogF64 = 0;
decltype(&__251a676b_DAQmxReadAnalogScalarF64) DAQmxReadAnalogScalarF64 = 0;
decltype(&__0815242b_DAQmxReadBinaryI16) DAQmxReadBinaryI16 = 0;
decltype(&__0a11242b_DAQmxReadBinaryI32) DAQmxReadBinaryI32 = 0;
decltype(&__08152437_DAQmxReadBinaryU16) DAQmxReadBinaryU16 = 0;
decltype(&__0a112437_DAQmxReadBinaryU32) DAQmxReadBinaryU32 = 0;
decltype(&__70120a0b_DAQmxReadCounterF64) DAQmxReadCounterF64 = 0;
decltype(&__08120a4e_DAQmxReadCounterF64Ex) DAQmxReadCounterF64Ex = 0;
decltype(&__30351951_DAQmxReadCounterScalarF64) DAQmxReadCounterScalarF64 = 0;
decltype(&__36350a54_DAQmxReadCounterScalarU32) DAQmxReadCounterScalarU32 = 0;
decltype(&__63170c0b_DAQmxReadCounterU32) DAQmxReadCounterU32 = 0;
decltype(&__1b170c4e_DAQmxReadCounterU32Ex) DAQmxReadCounterU32Ex = 0;
decltype(&__1e22250f_DAQmxReadCtrFreq) DAQmxReadCtrFreq = 0;
decltype(&__2c334463_DAQmxReadCtrFreqScalar) DAQmxReadCtrFreqScalar = 0;
decltype(&__7f392315_DAQmxReadCtrTicks) DAQmxReadCtrTicks = 0;
decltype(&__130b3274_DAQmxReadCtrTicksScalar) DAQmxReadCtrTicksScalar = 0;
decltype(&__0c392d1b_DAQmxReadCtrTime) DAQmxReadCtrTime = 0;
decltype(&__3e284c77_DAQmxReadCtrTimeScalar) DAQmxReadCtrTimeScalar = 0;
decltype(&__0e4a5262_DAQmxReadDigitalLines) DAQmxReadDigitalLines = 0;
decltype(&__31320858_DAQmxReadDigitalScalarU32) DAQmxReadDigitalScalarU32 = 0;
decltype(&__64120a07_DAQmxReadDigitalU16) DAQmxReadDigitalU16 = 0;
decltype(&__64100e07_DAQmxReadDigitalU32) DAQmxReadDigitalU32 = 0;
decltype(&__641b3c07_DAQmxReadDigitalU8) DAQmxReadDigitalU8 = 0;
decltype(&__5841557b_DAQmxReadRaw) DAQmxReadRaw = 0;
decltype(&__2c155964_DAQmxRegisterDoneEvent) DAQmxRegisterDoneEvent = 0;
decltype(&__342c3943_DAQmxRegisterEveryNSamplesEvent) DAQmxRegisterEveryNSamplesEvent = 0;
decltype(&__3f372b39_DAQmxRegisterSignalEvent) DAQmxRegisterSignalEvent = 0;
decltype(&__0c506459_DAQmxRemoveCDAQSyncConnection) DAQmxRemoveCDAQSyncConnection = 0;
decltype(&__7800342b_DAQmxReserveNetworkDevice) DAQmxReserveNetworkDevice = 0;
decltype(&__09283569_DAQmxResetAIACExcitFreq) DAQmxResetAIACExcitFreq = 0;
decltype(&__09424555_DAQmxResetAIACExcitSyncEnable) DAQmxResetAIACExcitSyncEnable = 0;
decltype(&__7d5b4435_DAQmxResetAIACExcitWireMode) DAQmxResetAIACExcitWireMode = 0;
decltype(&__060e295b_DAQmxResetAIADCCustomTimingMode) DAQmxResetAIADCCustomTimingMode = 0;
decltype(&__73033009_DAQmxResetAIADCTimingMode) DAQmxResetAIADCTimingMode = 0;
decltype(&__5423093f_DAQmxResetAIAccel4WireDCVoltageSensitivity) DAQmxResetAIAccel4WireDCVoltageSensitivity = 0;
decltype(&__3d572f51_DAQmxResetAIAccel4WireDCVoltageSensitivityUnits) DAQmxResetAIAccel4WireDCVoltageSensitivityUnits = 0;
decltype(&__635e1e00_DAQmxResetAIAccelChargeSensitivity) DAQmxResetAIAccelChargeSensitivity = 0;
decltype(&__0a2a386e_DAQmxResetAIAccelChargeSensitivityUnits) DAQmxResetAIAccelChargeSensitivityUnits = 0;
decltype(&__7157734c_DAQmxResetAIAccelSensitivity) DAQmxResetAIAccelSensitivity = 0;
decltype(&__57391a38_DAQmxResetAIAccelSensitivityUnits) DAQmxResetAIAccelSensitivityUnits = 0;
decltype(&__0022785b_DAQmxResetAIAccelUnits) DAQmxResetAIAccelUnits = 0;
decltype(&__11065460_DAQmxResetAIAcceldBRef) DAQmxResetAIAcceldBRef = 0;
decltype(&__76130132_DAQmxResetAIAtten) DAQmxResetAIAtten = 0;
decltype(&__0f181732_DAQmxResetAIAutoZeroMode) DAQmxResetAIAutoZeroMode = 0;
decltype(&__4d486a40_DAQmxResetAIAveragingWinSize) DAQmxResetAIAveragingWinSize = 0;
decltype(&__68553a00_DAQmxResetAIBridgeBalanceCoarsePot) DAQmxResetAIBridgeBalanceCoarsePot = 0;
decltype(&__1007362b_DAQmxResetAIBridgeBalanceFinePot) DAQmxResetAIBridgeBalanceFinePot = 0;
decltype(&__1b705f55_DAQmxResetAIBridgeCfg) DAQmxResetAIBridgeCfg = 0;
decltype(&__561e2535_DAQmxResetAIBridgeElectricalUnits) DAQmxResetAIBridgeElectricalUnits = 0;
decltype(&__10395d48_DAQmxResetAIBridgeInitialRatio) DAQmxResetAIBridgeInitialRatio = 0;
decltype(&__0d333435_DAQmxResetAIBridgeInitialVoltage) DAQmxResetAIBridgeInitialVoltage = 0;
decltype(&__1632264e_DAQmxResetAIBridgeNomResistance) DAQmxResetAIBridgeNomResistance = 0;
decltype(&__0d1b0356_DAQmxResetAIBridgePhysicalUnits) DAQmxResetAIBridgePhysicalUnits = 0;
decltype(&__605b0424_DAQmxResetAIBridgePolyForwardCoeff) DAQmxResetAIBridgePolyForwardCoeff = 0;
decltype(&__6549032f_DAQmxResetAIBridgePolyReverseCoeff) DAQmxResetAIBridgePolyReverseCoeff = 0;
decltype(&__646c4f04_DAQmxResetAIBridgeScaleType) DAQmxResetAIBridgeScaleType = 0;
decltype(&__09101213_DAQmxResetAIBridgeShuntCalEnable) DAQmxResetAIBridgeShuntCalEnable = 0;
decltype(&__6b694e69_DAQmxResetAIBridgeShuntCalGainAdjust) DAQmxResetAIBridgeShuntCalGainAdjust = 0;
decltype(&__04170b09_DAQmxResetAIBridgeShuntCalSelect) DAQmxResetAIBridgeShuntCalSelect = 0;
decltype(&__74465c01_DAQmxResetAIBridgeShuntCalShuntCalAActualResistance) DAQmxResetAIBridgeShuntCalShuntCalAActualResistance = 0;
decltype(&__08625a71_DAQmxResetAIBridgeShuntCalShuntCalAResistance) DAQmxResetAIBridgeShuntCalShuntCalAResistance = 0;
decltype(&__0e135d60_DAQmxResetAIBridgeShuntCalShuntCalASrc) DAQmxResetAIBridgeShuntCalShuntCalASrc = 0;
decltype(&__74465f01_DAQmxResetAIBridgeShuntCalShuntCalBActualResistance) DAQmxResetAIBridgeShuntCalShuntCalBActualResistance = 0;
decltype(&__08625971_DAQmxResetAIBridgeShuntCalShuntCalBResistance) DAQmxResetAIBridgeShuntCalShuntCalBResistance = 0;
decltype(&__1f464c6e_DAQmxResetAIBridgeTableElectricalVals) DAQmxResetAIBridgeTableElectricalVals = 0;
decltype(&__7468413d_DAQmxResetAIBridgeTablePhysicalVals) DAQmxResetAIBridgeTablePhysicalVals = 0;
decltype(&__3a073e44_DAQmxResetAIBridgeTwoPointLinFirstElectricalVal) DAQmxResetAIBridgeTwoPointLinFirstElectricalVal = 0;
decltype(&__14796d5c_DAQmxResetAIBridgeTwoPointLinFirstPhysicalVal) DAQmxResetAIBridgeTwoPointLinFirstPhysicalVal = 0;
decltype(&__0e330b1b_DAQmxResetAIBridgeTwoPointLinSecondElectricalVal) DAQmxResetAIBridgeTwoPointLinSecondElectricalVal = 0;
decltype(&__161d7548_DAQmxResetAIBridgeTwoPointLinSecondPhysicalVal) DAQmxResetAIBridgeTwoPointLinSecondPhysicalVal = 0;
decltype(&__15043a5d_DAQmxResetAIBridgeUnits) DAQmxResetAIBridgeUnits = 0;
decltype(&__0c3a0038_DAQmxResetAIChanCalApplyCalIfExp) DAQmxResetAIChanCalApplyCalIfExp = 0;
decltype(&__3c1d1b7d_DAQmxResetAIChanCalDesc) DAQmxResetAIChanCalDesc = 0;
decltype(&__524c7b7c_DAQmxResetAIChanCalEnableCal) DAQmxResetAIChanCalEnableCal = 0;
decltype(&__3c091d59_DAQmxResetAIChanCalOperatorName) DAQmxResetAIChanCalOperatorName = 0;
decltype(&__4e725321_DAQmxResetAIChanCalPolyForwardCoeff) DAQmxResetAIChanCalPolyForwardCoeff = 0;
decltype(&__45774126_DAQmxResetAIChanCalPolyReverseCoeff) DAQmxResetAIChanCalPolyReverseCoeff = 0;
decltype(&__6e76646a_DAQmxResetAIChanCalScaleType) DAQmxResetAIChanCalScaleType = 0;
decltype(&__1c497c52_DAQmxResetAIChanCalTablePreScaledVals) DAQmxResetAIChanCalTablePreScaledVals = 0;
decltype(&__62782305_DAQmxResetAIChanCalTableScaledVals) DAQmxResetAIChanCalTableScaledVals = 0;
decltype(&__1c13135f_DAQmxResetAIChanCalVerifAcqVals) DAQmxResetAIChanCalVerifAcqVals = 0;
decltype(&__0f15045f_DAQmxResetAIChanCalVerifRefVals) DAQmxResetAIChanCalVerifRefVals = 0;
decltype(&__141e324b_DAQmxResetAIChargeUnits) DAQmxResetAIChargeUnits = 0;
decltype(&__33047b45_DAQmxResetAIChopEnable) DAQmxResetAIChopEnable = 0;
decltype(&__4a6b2a2c_DAQmxResetAIConvActiveEdge) DAQmxResetAIConvActiveEdge = 0;
decltype(&__4a6b6f54_DAQmxResetAIConvActiveEdgeEx) DAQmxResetAIConvActiveEdgeEx = 0;
decltype(&__39746c4e_DAQmxResetAIConvDigFltrEnable) DAQmxResetAIConvDigFltrEnable = 0;
decltype(&__3931144e_DAQmxResetAIConvDigFltrEnableEx) DAQmxResetAIConvDigFltrEnableEx = 0;
decltype(&__5e6c4f60_DAQmxResetAIConvDigFltrMinPulseWidth) DAQmxResetAIConvDigFltrMinPulseWidth = 0;
decltype(&__1b144f60_DAQmxResetAIConvDigFltrMinPulseWidthEx) DAQmxResetAIConvDigFltrMinPulseWidthEx = 0;
decltype(&__5b7f6b03_DAQmxResetAIConvDigFltrTimebaseRate) DAQmxResetAIConvDigFltrTimebaseRate = 0;
decltype(&__237f6b46_DAQmxResetAIConvDigFltrTimebaseRateEx) DAQmxResetAIConvDigFltrTimebaseRateEx = 0;
decltype(&__48680e02_DAQmxResetAIConvDigFltrTimebaseSrc) DAQmxResetAIConvDigFltrTimebaseSrc = 0;
decltype(&__48684b7a_DAQmxResetAIConvDigFltrTimebaseSrcEx) DAQmxResetAIConvDigFltrTimebaseSrcEx = 0;
decltype(&__2c6e7d5b_DAQmxResetAIConvDigSyncEnable) DAQmxResetAIConvDigSyncEnable = 0;
decltype(&__2c2b055b_DAQmxResetAIConvDigSyncEnableEx) DAQmxResetAIConvDigSyncEnableEx = 0;
decltype(&__48696f44_DAQmxResetAIConvRate) DAQmxResetAIConvRate = 0;
decltype(&__0d116f44_DAQmxResetAIConvRateEx) DAQmxResetAIConvRateEx = 0;
decltype(&__497a7821_DAQmxResetAIConvSrc) DAQmxResetAIConvSrc = 0;
decltype(&__317a7864_DAQmxResetAIConvSrcEx) DAQmxResetAIConvSrcEx = 0;
decltype(&__68697321_DAQmxResetAIConvTimebaseDiv) DAQmxResetAIConvTimebaseDiv = 0;
decltype(&__10697364_DAQmxResetAIConvTimebaseDivEx) DAQmxResetAIConvTimebaseDivEx = 0;
decltype(&__7f726621_DAQmxResetAIConvTimebaseSrc) DAQmxResetAIConvTimebaseSrc = 0;
decltype(&__07726664_DAQmxResetAIConvTimebaseSrcEx) DAQmxResetAIConvTimebaseSrcEx = 0;
decltype(&__76616e40_DAQmxResetAICoupling) DAQmxResetAICoupling = 0;
decltype(&__1a405743_DAQmxResetAICurrentACRMSUnits) DAQmxResetAICurrentACRMSUnits = 0;
decltype(&__5b667e02_DAQmxResetAICurrentShuntLoc) DAQmxResetAICurrentShuntLoc = 0;
decltype(&__557d0f05_DAQmxResetAICurrentShuntResistance) DAQmxResetAICurrentShuntResistance = 0;
decltype(&__11150703_DAQmxResetAICurrentUnits) DAQmxResetAICurrentUnits = 0;
decltype(&__757e550e_DAQmxResetAICustomScaleName) DAQmxResetAICustomScaleName = 0;
decltype(&__7b575f45_DAQmxResetAIDCOffset) DAQmxResetAIDCOffset = 0;
decltype(&__74715430_DAQmxResetAIDataXferCustomThreshold) DAQmxResetAIDataXferCustomThreshold = 0;
decltype(&__69757916_DAQmxResetAIDataXferMaxRate) DAQmxResetAIDataXferMaxRate = 0;
decltype(&__0805072c_DAQmxResetAIDataXferMech) DAQmxResetAIDataXferMech = 0;
decltype(&__786b7107_DAQmxResetAIDataXferReqCond) DAQmxResetAIDataXferReqCond = 0;
decltype(&__174f7160_DAQmxResetAIDigFltrBandpassCenterFreq) DAQmxResetAIDigFltrBandpassCenterFreq = 0;
decltype(&__1803031c_DAQmxResetAIDigFltrBandpassWidth) DAQmxResetAIDigFltrBandpassWidth = 0;
decltype(&__1e1f0634_DAQmxResetAIDigFltrCoeff) DAQmxResetAIDigFltrCoeff = 0;
decltype(&__7a1b0238_DAQmxResetAIDigFltrEnable) DAQmxResetAIDigFltrEnable = 0;
decltype(&__1b5c6669_DAQmxResetAIDigFltrHighpassCutoffFreq) DAQmxResetAIDigFltrHighpassCutoffFreq = 0;
decltype(&__5f63505e_DAQmxResetAIDigFltrLowpassCutoffFreq) DAQmxResetAIDigFltrLowpassCutoffFreq = 0;
decltype(&__5d682b31_DAQmxResetAIDigFltrNotchCenterFreq) DAQmxResetAIDigFltrNotchCenterFreq = 0;
decltype(&__21676743_DAQmxResetAIDigFltrNotchWidth) DAQmxResetAIDigFltrNotchWidth = 0;
decltype(&__031e052c_DAQmxResetAIDigFltrOrder) DAQmxResetAIDigFltrOrder = 0;
decltype(&__7a7a752c_DAQmxResetAIDigFltrResponse) DAQmxResetAIDigFltrResponse = 0;
decltype(&__080a0545_DAQmxResetAIDigFltrType) DAQmxResetAIDigFltrType = 0;
decltype(&__191e2834_DAQmxResetAIDitherEnable) DAQmxResetAIDitherEnable = 0;
decltype(&__697d733c_DAQmxResetAIEddyCurrentProxProbeSensitivity) DAQmxResetAIEddyCurrentProxProbeSensitivity = 0;
decltype(&__0714071a_DAQmxResetAIEddyCurrentProxProbeSensitivityUnits) DAQmxResetAIEddyCurrentProxProbeSensitivityUnits = 0;
decltype(&__1c76644d_DAQmxResetAIEddyCurrentProxProbeUnits) DAQmxResetAIEddyCurrentProxProbeUnits = 0;
decltype(&__2b3b293b_DAQmxResetAIEnhancedAliasRejectionEnable) DAQmxResetAIEnhancedAliasRejectionEnable = 0;
decltype(&__7c53191c_DAQmxResetAIExcitActualVal) DAQmxResetAIExcitActualVal = 0;
decltype(&__1a1a1651_DAQmxResetAIExcitDCorAC) DAQmxResetAIExcitDCorAC = 0;
decltype(&__716b6012_DAQmxResetAIExcitIdleOutputBehavior) DAQmxResetAIExcitIdleOutputBehavior = 0;
decltype(&__1b297350_DAQmxResetAIExcitSense) DAQmxResetAIExcitSense = 0;
decltype(&__684c645d_DAQmxResetAIExcitSrc) DAQmxResetAIExcitSrc = 0;
decltype(&__23237b61_DAQmxResetAIExcitUseForScaling) DAQmxResetAIExcitUseForScaling = 0;
decltype(&__342a014a_DAQmxResetAIExcitUseMultiplexed) DAQmxResetAIExcitUseMultiplexed = 0;
decltype(&__68497752_DAQmxResetAIExcitVal) DAQmxResetAIExcitVal = 0;
decltype(&__5528382c_DAQmxResetAIExcitVoltageOrCurrent) DAQmxResetAIExcitVoltageOrCurrent = 0;
decltype(&__721a341a_DAQmxResetAIFilterDelayAdjustment) DAQmxResetAIFilterDelayAdjustment = 0;
decltype(&__78745060_DAQmxResetAIFilterDelayUnits) DAQmxResetAIFilterDelayUnits = 0;
decltype(&__1b1e3028_DAQmxResetAIFilterEnable) DAQmxResetAIFilterEnable = 0;
decltype(&__1f0d5f51_DAQmxResetAIFilterFreq) DAQmxResetAIFilterFreq = 0;
decltype(&__1e192451_DAQmxResetAIFilterOrder) DAQmxResetAIFilterOrder = 0;
decltype(&__7a692428_DAQmxResetAIFilterResponse) DAQmxResetAIFilterResponse = 0;
decltype(&__29035059_DAQmxResetAIForceIEPESensorSensitivity) DAQmxResetAIForceIEPESensorSensitivity = 0;
decltype(&__40777637_DAQmxResetAIForceIEPESensorSensitivityUnits) DAQmxResetAIForceIEPESensorSensitivityUnits = 0;
decltype(&__1d5f785b_DAQmxResetAIForceReadFromChan) DAQmxResetAIForceReadFromChan = 0;
decltype(&__0e2e695d_DAQmxResetAIForceUnits) DAQmxResetAIForceUnits = 0;
decltype(&__576c6352_DAQmxResetAIFreqHyst) DAQmxResetAIFreqHyst = 0;
decltype(&__3161554b_DAQmxResetAIFreqThreshVoltage) DAQmxResetAIFreqThreshVoltage = 0;
decltype(&__397b7952_DAQmxResetAIFreqUnits) DAQmxResetAIFreqUnits = 0;
decltype(&__1e061c39_DAQmxResetAIGain) DAQmxResetAIGain = 0;
decltype(&__116b6b51_DAQmxResetAIImpedance) DAQmxResetAIImpedance = 0;
decltype(&__2a372e13_DAQmxResetAIInputLimitsFaultDetectEnable) DAQmxResetAIInputLimitsFaultDetectEnable = 0;
decltype(&__555d504f_DAQmxResetAIInputLimitsFaultDetectLowerLimit) DAQmxResetAIInputLimitsFaultDetectLowerLimit = 0;
decltype(&__525d4950_DAQmxResetAIInputLimitsFaultDetectUpperLimit) DAQmxResetAIInputLimitsFaultDetectUpperLimit = 0;
decltype(&__645a7741_DAQmxResetAIInputSrc) DAQmxResetAIInputSrc = 0;
decltype(&__46544f06_DAQmxResetAILVDTSensitivity) DAQmxResetAILVDTSensitivity = 0;
decltype(&__283d3b20_DAQmxResetAILVDTSensitivityUnits) DAQmxResetAILVDTSensitivityUnits = 0;
decltype(&__335f5877_DAQmxResetAILVDTUnits) DAQmxResetAILVDTUnits = 0;
decltype(&__001f7451_DAQmxResetAILeadWireResistance) DAQmxResetAILeadWireResistance = 0;
decltype(&__335e5f4a_DAQmxResetAILossyLSBRemovalCompressedSampSize) DAQmxResetAILossyLSBRemovalCompressedSampSize = 0;
decltype(&__16496c67_DAQmxResetAILowpassCutoffFreq) DAQmxResetAILowpassCutoffFreq = 0;
decltype(&__7f1a130e_DAQmxResetAILowpassEnable) DAQmxResetAILowpassEnable = 0;
decltype(&__5a5e0f34_DAQmxResetAILowpassSwitchCapClkSrc) DAQmxResetAILowpassSwitchCapClkSrc = 0;
decltype(&__3442544d_DAQmxResetAILowpassSwitchCapExtClkDiv) DAQmxResetAILowpassSwitchCapExtClkDiv = 0;
decltype(&__27335656_DAQmxResetAILowpassSwitchCapExtClkFreq) DAQmxResetAILowpassSwitchCapExtClkFreq = 0;
decltype(&__3e4f544d_DAQmxResetAILowpassSwitchCapOutClkDiv) DAQmxResetAILowpassSwitchCapOutClkDiv = 0;
decltype(&__14060d57_DAQmxResetAIMax) DAQmxResetAIMax = 0;
decltype(&__14103111_DAQmxResetAIMemMapEnable) DAQmxResetAIMemMapEnable = 0;
decltype(&__6b1e2d2f_DAQmxResetAIMicrophoneSensitivity) DAQmxResetAIMicrophoneSensitivity = 0;
decltype(&__140e1b57_DAQmxResetAIMin) DAQmxResetAIMin = 0;
decltype(&__130c2c39_DAQmxResetAIOpenChanDetectEnable) DAQmxResetAIOpenChanDetectEnable = 0;
decltype(&__52526012_DAQmxResetAIOpenThrmcplDetectEnable) DAQmxResetAIOpenThrmcplDetectEnable = 0;
decltype(&__63577811_DAQmxResetAIOvercurrentDetectEnable) DAQmxResetAIOvercurrentDetectEnable = 0;
decltype(&__2c313f1e_DAQmxResetAIPowerSupplyFaultDetectEnable) DAQmxResetAIPowerSupplyFaultDetectEnable = 0;
decltype(&__5c0e0b35_DAQmxResetAIPressureUnits) DAQmxResetAIPressureUnits = 0;
decltype(&__093a6e41_DAQmxResetAIProbeAtten) DAQmxResetAIProbeAtten = 0;
decltype(&__0b333116_DAQmxResetAIRTDA) DAQmxResetAIRTDA = 0;
decltype(&__0b333115_DAQmxResetAIRTDB) DAQmxResetAIRTDB = 0;
decltype(&__0b333114_DAQmxResetAIRTDC) DAQmxResetAIRTDC = 0;
decltype(&__3b333105_DAQmxResetAIRTDR0) DAQmxResetAIRTDR0 = 0;
decltype(&__72435403_DAQmxResetAIRTDType) DAQmxResetAIRTDType = 0;
decltype(&__58544f06_DAQmxResetAIRVDTSensitivity) DAQmxResetAIRVDTSensitivity = 0;
decltype(&__363d3b20_DAQmxResetAIRVDTSensitivityUnits) DAQmxResetAIRVDTSensitivityUnits = 0;
decltype(&__2d5f5877_DAQmxResetAIRVDTUnits) DAQmxResetAIRVDTUnits = 0;
decltype(&__7f673432_DAQmxResetAIRawDataCompressionType) DAQmxResetAIRawDataCompressionType = 0;
decltype(&__2c765742_DAQmxResetAIRemoveFilterDelay) DAQmxResetAIRemoveFilterDelay = 0;
decltype(&__7c132436_DAQmxResetAIResistanceCfg) DAQmxResetAIResistanceCfg = 0;
decltype(&__7267413e_DAQmxResetAIResistanceUnits) DAQmxResetAIResistanceUnits = 0;
decltype(&__626e7a1f_DAQmxResetAIRngHigh) DAQmxResetAIRngHigh = 0;
decltype(&__647e121b_DAQmxResetAIRngLow) DAQmxResetAIRngLow = 0;
decltype(&__11010a09_DAQmxResetAIRosetteStrainGageOrientation) DAQmxResetAIRosetteStrainGageOrientation = 0;
decltype(&__7c73696f_DAQmxResetAIRosetteStrainGageRosetteMeasType) DAQmxResetAIRosetteStrainGageRosetteMeasType = 0;
decltype(&__2f657a46_DAQmxResetAISampAndHoldEnable) DAQmxResetAISampAndHoldEnable = 0;
decltype(&__74723908_DAQmxResetAISensorPowerCfg) DAQmxResetAISensorPowerCfg = 0;
decltype(&__6b655c1f_DAQmxResetAISensorPowerType) DAQmxResetAISensorPowerType = 0;
decltype(&__1a1c4d7c_DAQmxResetAISensorPowerVoltage) DAQmxResetAISensorPowerVoltage = 0;
decltype(&__59666243_DAQmxResetAISoundPressureMaxSoundPressureLvl) DAQmxResetAISoundPressureMaxSoundPressureLvl = 0;
decltype(&__0c0d6947_DAQmxResetAISoundPressureUnits) DAQmxResetAISoundPressureUnits = 0;
decltype(&__1d29457c_DAQmxResetAISoundPressuredBRef) DAQmxResetAISoundPressuredBRef = 0;
decltype(&__63180331_DAQmxResetAIStrainGageCfg) DAQmxResetAIStrainGageCfg = 0;
decltype(&__0914046f_DAQmxResetAIStrainGageForceReadFromChan) DAQmxResetAIStrainGageForceReadFromChan = 0;
decltype(&__00092e25_DAQmxResetAIStrainGageGageFactor) DAQmxResetAIStrainGageGageFactor = 0;
decltype(&__6a560223_DAQmxResetAIStrainGagePoissonRatio) DAQmxResetAIStrainGagePoissonRatio = 0;
decltype(&__0a092158_DAQmxResetAIStrainUnits) DAQmxResetAIStrainUnits = 0;
decltype(&__2b6c7153_DAQmxResetAITempUnits) DAQmxResetAITempUnits = 0;
decltype(&__4e64603a_DAQmxResetAITermCfg) DAQmxResetAITermCfg = 0;
decltype(&__483c3d18_DAQmxResetAIThrmcplCJCVal) DAQmxResetAIThrmcplCJCVal = 0;
decltype(&__6d4f7455_DAQmxResetAIThrmcplLeadOffsetVoltage) DAQmxResetAIThrmcplLeadOffsetVoltage = 0;
decltype(&__59677769_DAQmxResetAIThrmcplScaleType) DAQmxResetAIThrmcplScaleType = 0;
decltype(&__170f0e6e_DAQmxResetAIThrmcplType) DAQmxResetAIThrmcplType = 0;
decltype(&__7e7b757b_DAQmxResetAIThrmstrA) DAQmxResetAIThrmstrA = 0;
decltype(&__7e7b7578_DAQmxResetAIThrmstrB) DAQmxResetAIThrmstrB = 0;
decltype(&__7e7b7579_DAQmxResetAIThrmstrC) DAQmxResetAIThrmstrC = 0;
decltype(&__4f7b7568_DAQmxResetAIThrmstrR1) DAQmxResetAIThrmstrR1 = 0;
decltype(&__11192148_DAQmxResetAITorqueUnits) DAQmxResetAITorqueUnits = 0;
decltype(&__7a6e5232_DAQmxResetAIUsbXferReqCount) DAQmxResetAIUsbXferReqCount = 0;
decltype(&__75653634_DAQmxResetAIUsbXferReqSize) DAQmxResetAIUsbXferReqSize = 0;
decltype(&__673c0012_DAQmxResetAIVelocityIEPESensorSensitivity) DAQmxResetAIVelocityIEPESensorSensitivity = 0;
decltype(&__131a6e7b_DAQmxResetAIVelocityIEPESensorSensitivityUnits) DAQmxResetAIVelocityIEPESensorSensitivityUnits = 0;
decltype(&__4b5c5135_DAQmxResetAIVelocityIEPESensordBRef) DAQmxResetAIVelocityIEPESensordBRef = 0;
decltype(&__4a050435_DAQmxResetAIVelocityUnits) DAQmxResetAIVelocityUnits = 0;
decltype(&__0b535845_DAQmxResetAIVoltageACRMSUnits) DAQmxResetAIVoltageACRMSUnits = 0;
decltype(&__00060805_DAQmxResetAIVoltageUnits) DAQmxResetAIVoltageUnits = 0;
decltype(&__2c3d1921_DAQmxResetAIVoltagedBRef) DAQmxResetAIVoltagedBRef = 0;
decltype(&__77707f41_DAQmxResetAOCommonModeOffset) DAQmxResetAOCommonModeOffset = 0;
decltype(&__11150705_DAQmxResetAOCurrentUnits) DAQmxResetAOCurrentUnits = 0;
decltype(&__757e5508_DAQmxResetAOCustomScaleName) DAQmxResetAOCustomScaleName = 0;
decltype(&__5c775e0f_DAQmxResetAODACOffsetExtSrc) DAQmxResetAODACOffsetExtSrc = 0;
decltype(&__0f133718_DAQmxResetAODACOffsetSrc) DAQmxResetAODACOffsetSrc = 0;
decltype(&__0f162417_DAQmxResetAODACOffsetVal) DAQmxResetAODACOffsetVal = 0;
decltype(&__1406001c_DAQmxResetAODACRefAllowConnToGnd) DAQmxResetAODACRefAllowConnToGnd = 0;
decltype(&__51404503_DAQmxResetAODACRefConnToGnd) DAQmxResetAODACRefConnToGnd = 0;
decltype(&__0c130118_DAQmxResetAODACRefExtSrc) DAQmxResetAODACRefExtSrc = 0;
decltype(&__1b406571_DAQmxResetAODACRefSrc) DAQmxResetAODACRefSrc = 0;
decltype(&__14406062_DAQmxResetAODACRefVal) DAQmxResetAODACRefVal = 0;
decltype(&__14297e6a_DAQmxResetAODACRngHigh) DAQmxResetAODACRngHigh = 0;
decltype(&__04417a6c_DAQmxResetAODACRngLow) DAQmxResetAODACRngLow = 0;
decltype(&__0805072a_DAQmxResetAODataXferMech) DAQmxResetAODataXferMech = 0;
decltype(&__786b7101_DAQmxResetAODataXferReqCond) DAQmxResetAODataXferReqCond = 0;
decltype(&__353a213b_DAQmxResetAOEnhancedImageRejectionEnable) DAQmxResetAOEnhancedImageRejectionEnable = 0;
decltype(&__161d2440_DAQmxResetAOFilterDelay) DAQmxResetAOFilterDelay = 0;
decltype(&__721a341c_DAQmxResetAOFilterDelayAdjustment) DAQmxResetAOFilterDelayAdjustment = 0;
decltype(&__78745066_DAQmxResetAOFilterDelayUnits) DAQmxResetAOFilterDelayUnits = 0;
decltype(&__41727d7f_DAQmxResetAOFuncGenAmplitude) DAQmxResetAOFuncGenAmplitude = 0;
decltype(&__133c646b_DAQmxResetAOFuncGenFMDeviation) DAQmxResetAOFuncGenFMDeviation = 0;
decltype(&__2a120474_DAQmxResetAOFuncGenFreq) DAQmxResetAOFuncGenFreq = 0;
decltype(&__5d33100c_DAQmxResetAOFuncGenModulationType) DAQmxResetAOFuncGenModulationType = 0;
decltype(&__4a110618_DAQmxResetAOFuncGenOffset) DAQmxResetAOFuncGenOffset = 0;
decltype(&__59601804_DAQmxResetAOFuncGenSquareDutyCycle) DAQmxResetAOFuncGenSquareDutyCycle = 0;
decltype(&__197e6666_DAQmxResetAOFuncGenStartPhase) DAQmxResetAOFuncGenStartPhase = 0;
decltype(&__21071066_DAQmxResetAOFuncGenType) DAQmxResetAOFuncGenType = 0;
decltype(&__1e061c3f_DAQmxResetAOGain) DAQmxResetAOGain = 0;
decltype(&__2d115948_DAQmxResetAOIdleOutputBehavior) DAQmxResetAOIdleOutputBehavior = 0;
decltype(&__5d040a33_DAQmxResetAOLoadImpedance) DAQmxResetAOLoadImpedance = 0;
decltype(&__14060d51_DAQmxResetAOMax) DAQmxResetAOMax = 0;
decltype(&__14103117_DAQmxResetAOMemMapEnable) DAQmxResetAOMemMapEnable = 0;
decltype(&__140e1b51_DAQmxResetAOMin) DAQmxResetAOMin = 0;
decltype(&__7d60492d_DAQmxResetAOOutputImpedance) DAQmxResetAOOutputImpedance = 0;
decltype(&__5d250003_DAQmxResetAOPowerAmpChannelEnable) DAQmxResetAOPowerAmpChannelEnable = 0;
decltype(&__4b7d1037_DAQmxResetAOReglitchEnable) DAQmxResetAOReglitchEnable = 0;
decltype(&__616d5439_DAQmxResetAOResolutionUnits) DAQmxResetAOResolutionUnits = 0;
decltype(&__4e64603c_DAQmxResetAOTermCfg) DAQmxResetAOTermCfg = 0;
decltype(&__7a6e5234_DAQmxResetAOUsbXferReqCount) DAQmxResetAOUsbXferReqCount = 0;
decltype(&__75653632_DAQmxResetAOUsbXferReqSize) DAQmxResetAOUsbXferReqSize = 0;
decltype(&__415f7635_DAQmxResetAOUseOnlyOnBrdMem) DAQmxResetAOUseOnlyOnBrdMem = 0;
decltype(&__1800366a_DAQmxResetAOVoltageCurrentLimit) DAQmxResetAOVoltageCurrentLimit = 0;
decltype(&__00060803_DAQmxResetAOVoltageUnits) DAQmxResetAOVoltageUnits = 0;
decltype(&__2d677e63_DAQmxResetAdvTrigType) DAQmxResetAdvTrigType = 0;
decltype(&__441e1603_DAQmxResetAnlgEdgeRefTrigCoupling) DAQmxResetAnlgEdgeRefTrigCoupling = 0;
decltype(&__3c126c6f_DAQmxResetAnlgEdgeRefTrigDigFltrEnable) DAQmxResetAnlgEdgeRefTrigDigFltrEnable = 0;
decltype(&__1275744c_DAQmxResetAnlgEdgeRefTrigDigFltrMinPulseWidth) DAQmxResetAnlgEdgeRefTrigDigFltrMinPulseWidth = 0;
decltype(&__71706768_DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseRate) DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseRate = 0;
decltype(&__7063700d_DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseSrc) DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseSrc = 0;
decltype(&__2907767e_DAQmxResetAnlgEdgeRefTrigDigSyncEnable) DAQmxResetAnlgEdgeRefTrigDigSyncEnable = 0;
decltype(&__2779696b_DAQmxResetAnlgEdgeRefTrigHyst) DAQmxResetAnlgEdgeRefTrigHyst = 0;
decltype(&__537d6674_DAQmxResetAnlgEdgeRefTrigLvl) DAQmxResetAnlgEdgeRefTrigLvl = 0;
decltype(&__23077c77_DAQmxResetAnlgEdgeRefTrigSlope) DAQmxResetAnlgEdgeRefTrigSlope = 0;
decltype(&__5362627b_DAQmxResetAnlgEdgeRefTrigSrc) DAQmxResetAnlgEdgeRefTrigSrc = 0;
decltype(&__4765671b_DAQmxResetAnlgEdgeStartTrigCoupling) DAQmxResetAnlgEdgeStartTrigCoupling = 0;
decltype(&__3d091f17_DAQmxResetAnlgEdgeStartTrigDigFltrEnable) DAQmxResetAnlgEdgeStartTrigDigFltrEnable = 0;
decltype(&__252a3170_DAQmxResetAnlgEdgeStartTrigDigFltrMinPulseWidth) DAQmxResetAnlgEdgeStartTrigDigFltrMinPulseWidth = 0;
decltype(&__360e5275_DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseRate) DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseRate = 0;
decltype(&__216b5366_DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseSrc) DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseSrc = 0;
decltype(&__27180a02_DAQmxResetAnlgEdgeStartTrigDigSyncEnable) DAQmxResetAnlgEdgeStartTrigDigSyncEnable = 0;
decltype(&__380d047c_DAQmxResetAnlgEdgeStartTrigHyst) DAQmxResetAnlgEdgeStartTrigHyst = 0;
decltype(&__37127078_DAQmxResetAnlgEdgeStartTrigLvl) DAQmxResetAnlgEdgeStartTrigLvl = 0;
decltype(&__2d110002_DAQmxResetAnlgEdgeStartTrigSlope) DAQmxResetAnlgEdgeStartTrigSlope = 0;
decltype(&__331d7067_DAQmxResetAnlgEdgeStartTrigSrc) DAQmxResetAnlgEdgeStartTrigSrc = 0;
decltype(&__58452307_DAQmxResetAnlgLvlPauseTrigCoupling) DAQmxResetAnlgLvlPauseTrigCoupling = 0;
decltype(&__343d2f7d_DAQmxResetAnlgLvlPauseTrigDigFltrEnable) DAQmxResetAnlgLvlPauseTrigDigFltrEnable = 0;
decltype(&__17134865_DAQmxResetAnlgLvlPauseTrigDigFltrMinPulseWidth) DAQmxResetAnlgLvlPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__33704d76_DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseRate) DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseRate = 0;
decltype(&__56715e61_DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseSrc) DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__25283a67_DAQmxResetAnlgLvlPauseTrigDigSyncEnable) DAQmxResetAnlgLvlPauseTrigDigSyncEnable = 0;
decltype(&__30264478_DAQmxResetAnlgLvlPauseTrigHyst) DAQmxResetAnlgLvlPauseTrigHyst = 0;
decltype(&__2f524077_DAQmxResetAnlgLvlPauseTrigLvl) DAQmxResetAnlgLvlPauseTrigLvl = 0;
decltype(&__20525f73_DAQmxResetAnlgLvlPauseTrigSrc) DAQmxResetAnlgLvlPauseTrigSrc = 0;
decltype(&__263c5b69_DAQmxResetAnlgLvlPauseTrigWhen) DAQmxResetAnlgLvlPauseTrigWhen = 0;
decltype(&__2a053c66_DAQmxResetAnlgMultiEdgeRefTrigCouplings) DAQmxResetAnlgMultiEdgeRefTrigCouplings = 0;
decltype(&__42665b19_DAQmxResetAnlgMultiEdgeRefTrigHysts) DAQmxResetAnlgMultiEdgeRefTrigHysts = 0;
decltype(&__5d612c16_DAQmxResetAnlgMultiEdgeRefTrigLvls) DAQmxResetAnlgMultiEdgeRefTrigLvls = 0;
decltype(&__5e62567f_DAQmxResetAnlgMultiEdgeRefTrigSlopes) DAQmxResetAnlgMultiEdgeRefTrigSlopes = 0;
decltype(&__52613312_DAQmxResetAnlgMultiEdgeRefTrigSrcs) DAQmxResetAnlgMultiEdgeRefTrigSrcs = 0;
decltype(&__41063417_DAQmxResetAnlgMultiEdgeStartTrigCouplings) DAQmxResetAnlgMultiEdgeStartTrigCouplings = 0;
decltype(&__26795c74_DAQmxResetAnlgMultiEdgeStartTrigHysts) DAQmxResetAnlgMultiEdgeStartTrigHysts = 0;
decltype(&__51764373_DAQmxResetAnlgMultiEdgeStartTrigLvls) DAQmxResetAnlgMultiEdgeStartTrigLvls = 0;
decltype(&__2b1f4070_DAQmxResetAnlgMultiEdgeStartTrigSlopes) DAQmxResetAnlgMultiEdgeStartTrigSlopes = 0;
decltype(&__4e724c73_DAQmxResetAnlgMultiEdgeStartTrigSrcs) DAQmxResetAnlgMultiEdgeStartTrigSrcs = 0;
decltype(&__2c52556a_DAQmxResetAnlgWinPauseTrigBtm) DAQmxResetAnlgWinPauseTrigBtm = 0;
decltype(&__5a453818_DAQmxResetAnlgWinPauseTrigCoupling) DAQmxResetAnlgWinPauseTrigCoupling = 0;
decltype(&__363d3462_DAQmxResetAnlgWinPauseTrigDigFltrEnable) DAQmxResetAnlgWinPauseTrigDigFltrEnable = 0;
decltype(&__1513537a_DAQmxResetAnlgWinPauseTrigDigFltrMinPulseWidth) DAQmxResetAnlgWinPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__31705669_DAQmxResetAnlgWinPauseTrigDigFltrTimebaseRate) DAQmxResetAnlgWinPauseTrigDigFltrTimebaseRate = 0;
decltype(&__5471457e_DAQmxResetAnlgWinPauseTrigDigFltrTimebaseSrc) DAQmxResetAnlgWinPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__27282178_DAQmxResetAnlgWinPauseTrigDigSyncEnable) DAQmxResetAnlgWinPauseTrigDigSyncEnable = 0;
decltype(&__2252446c_DAQmxResetAnlgWinPauseTrigSrc) DAQmxResetAnlgWinPauseTrigSrc = 0;
decltype(&__31524371_DAQmxResetAnlgWinPauseTrigTop) DAQmxResetAnlgWinPauseTrigTop = 0;
decltype(&__243c4076_DAQmxResetAnlgWinPauseTrigWhen) DAQmxResetAnlgWinPauseTrigWhen = 0;
decltype(&__4d544318_DAQmxResetAnlgWinRefTrigBtm) DAQmxResetAnlgWinRefTrigBtm = 0;
decltype(&__2026350f_DAQmxResetAnlgWinRefTrigCoupling) DAQmxResetAnlgWinRefTrigCoupling = 0;
decltype(&__2c5c5977_DAQmxResetAnlgWinRefTrigDigFltrEnable) DAQmxResetAnlgWinRefTrigDigFltrEnable = 0;
decltype(&__4b447a59_DAQmxResetAnlgWinRefTrigDigFltrMinPulseWidth) DAQmxResetAnlgWinRefTrigDigFltrMinPulseWidth = 0;
decltype(&__4e575e3a_DAQmxResetAnlgWinRefTrigDigFltrTimebaseRate) DAQmxResetAnlgWinRefTrigDigFltrTimebaseRate = 0;
decltype(&__5d403b3b_DAQmxResetAnlgWinRefTrigDigFltrTimebaseSrc) DAQmxResetAnlgWinRefTrigDigFltrTimebaseSrc = 0;
decltype(&__39464862_DAQmxResetAnlgWinRefTrigDigSyncEnable) DAQmxResetAnlgWinRefTrigDigSyncEnable = 0;
decltype(&__5c524d18_DAQmxResetAnlgWinRefTrigSrc) DAQmxResetAnlgWinRefTrigSrc = 0;
decltype(&__5b4f5e18_DAQmxResetAnlgWinRefTrigTop) DAQmxResetAnlgWinRefTrigTop = 0;
decltype(&__58484b76_DAQmxResetAnlgWinRefTrigWhen) DAQmxResetAnlgWinRefTrigWhen = 0;
decltype(&__2d40407e_DAQmxResetAnlgWinStartTrigBtm) DAQmxResetAnlgWinStartTrigBtm = 0;
decltype(&__5b572d0c_DAQmxResetAnlgWinStartTrigCoupling) DAQmxResetAnlgWinStartTrigCoupling = 0;
decltype(&__372f2176_DAQmxResetAnlgWinStartTrigDigFltrEnable) DAQmxResetAnlgWinStartTrigDigFltrEnable = 0;
decltype(&__1401466e_DAQmxResetAnlgWinStartTrigDigFltrMinPulseWidth) DAQmxResetAnlgWinStartTrigDigFltrMinPulseWidth = 0;
decltype(&__3062437d_DAQmxResetAnlgWinStartTrigDigFltrTimebaseRate) DAQmxResetAnlgWinStartTrigDigFltrTimebaseRate = 0;
decltype(&__5563506a_DAQmxResetAnlgWinStartTrigDigFltrTimebaseSrc) DAQmxResetAnlgWinStartTrigDigFltrTimebaseSrc = 0;
decltype(&__263a346c_DAQmxResetAnlgWinStartTrigDigSyncEnable) DAQmxResetAnlgWinStartTrigDigSyncEnable = 0;
decltype(&__23405178_DAQmxResetAnlgWinStartTrigSrc) DAQmxResetAnlgWinStartTrigSrc = 0;
decltype(&__30405665_DAQmxResetAnlgWinStartTrigTop) DAQmxResetAnlgWinStartTrigTop = 0;
decltype(&__252e5562_DAQmxResetAnlgWinStartTrigWhen) DAQmxResetAnlgWinStartTrigWhen = 0;
decltype(&__232e1775_DAQmxResetArmStartTrigTimescale) DAQmxResetArmStartTrigTimescale = 0;
decltype(&__284e604b_DAQmxResetArmStartTrigTimestampEnable) DAQmxResetArmStartTrigTimestampEnable = 0;
decltype(&__29230b20_DAQmxResetArmStartTrigTimestampTimescale) DAQmxResetArmStartTrigTimestampTimescale = 0;
decltype(&__232e5665_DAQmxResetArmStartTrigTrigWhen) DAQmxResetArmStartTrigTrigWhen = 0;
decltype(&__5f420106_DAQmxResetArmStartTrigType) DAQmxResetArmStartTrigType = 0;
decltype(&__49093314_DAQmxResetBufInputBufSize) DAQmxResetBufInputBufSize = 0;
decltype(&__405e2434_DAQmxResetBufOutputBufSize) DAQmxResetBufOutputBufSize = 0;
decltype(&__0a300667_DAQmxResetBufOutputOnbrdBufSize) DAQmxResetBufOutputOnbrdBufSize = 0;
decltype(&__7217121f_DAQmxResetBufferAttribute) DAQmxResetBufferAttribute = 0;
decltype(&__7a48311e_DAQmxResetCIAngEncoderInitialAngle) DAQmxResetCIAngEncoderInitialAngle = 0;
decltype(&__4a783822_DAQmxResetCIAngEncoderPulsesPerRev) DAQmxResetCIAngEncoderPulsesPerRev = 0;
decltype(&__7a6c5918_DAQmxResetCIAngEncoderUnits) DAQmxResetCIAngEncoderUnits = 0;
decltype(&__3a33363d_DAQmxResetCICountEdgesActiveEdge) DAQmxResetCICountEdgesActiveEdge = 0;
decltype(&__604d7209_DAQmxResetCICountEdgesCountDirDigFltrEnable) DAQmxResetCICountEdgesCountDirDigFltrEnable = 0;
decltype(&__43631511_DAQmxResetCICountEdgesCountDirDigFltrMinPulseWidth) DAQmxResetCICountEdgesCountDirDigFltrMinPulseWidth = 0;
decltype(&__67001002_DAQmxResetCICountEdgesCountDirDigFltrTimebaseRate) DAQmxResetCICountEdgesCountDirDigFltrTimebaseRate = 0;
decltype(&__02010315_DAQmxResetCICountEdgesCountDirDigFltrTimebaseSrc) DAQmxResetCICountEdgesCountDirDigFltrTimebaseSrc = 0;
decltype(&__71586713_DAQmxResetCICountEdgesCountDirDigSyncEnable) DAQmxResetCICountEdgesCountDirDigSyncEnable = 0;
decltype(&__6456190c_DAQmxResetCICountEdgesCountDirHyst) DAQmxResetCICountEdgesCountDirHyst = 0;
decltype(&__01344a5a_DAQmxResetCICountEdgesCountDirLogicLvlBehavior) DAQmxResetCICountEdgesCountDirLogicLvlBehavior = 0;
decltype(&__024f4676_DAQmxResetCICountEdgesCountDirTermCfg) DAQmxResetCICountEdgesCountDirTermCfg = 0;
decltype(&__524f7f01_DAQmxResetCICountEdgesCountDirThreshVoltage) DAQmxResetCICountEdgesCountDirThreshVoltage = 0;
decltype(&__4b40051a_DAQmxResetCICountEdgesCountResetActiveEdge) DAQmxResetCICountEdgesCountResetActiveEdge = 0;
decltype(&__385f4378_DAQmxResetCICountEdgesCountResetDigFltrEnable) DAQmxResetCICountEdgesCountResetDigFltrEnable = 0;
decltype(&__5f476056_DAQmxResetCICountEdgesCountResetDigFltrMinPulseWidth) DAQmxResetCICountEdgesCountResetDigFltrMinPulseWidth = 0;
decltype(&__5a544435_DAQmxResetCICountEdgesCountResetDigFltrTimebaseRate) DAQmxResetCICountEdgesCountResetDigFltrTimebaseRate = 0;
decltype(&__49432134_DAQmxResetCICountEdgesCountResetDigFltrTimebaseSrc) DAQmxResetCICountEdgesCountResetDigFltrTimebaseSrc = 0;
decltype(&__2d45526d_DAQmxResetCICountEdgesCountResetDigSyncEnable) DAQmxResetCICountEdgesCountResetDigSyncEnable = 0;
decltype(&__32285575_DAQmxResetCICountEdgesCountResetEnable) DAQmxResetCICountEdgesCountResetEnable = 0;
decltype(&__535a4763_DAQmxResetCICountEdgesCountResetHyst) DAQmxResetCICountEdgesCountResetHyst = 0;
decltype(&__000c2201_DAQmxResetCICountEdgesCountResetLogicLvlBehavior) DAQmxResetCICountEdgesCountResetLogicLvlBehavior = 0;
decltype(&__53712807_DAQmxResetCICountEdgesCountResetResetCount) DAQmxResetCICountEdgesCountResetResetCount = 0;
decltype(&__4f46467a_DAQmxResetCICountEdgesCountResetTerm) DAQmxResetCICountEdgesCountResetTerm = 0;
decltype(&__0c20217a_DAQmxResetCICountEdgesCountResetTermCfg) DAQmxResetCICountEdgesCountResetTermCfg = 0;
decltype(&__3557717a_DAQmxResetCICountEdgesCountResetThreshVoltage) DAQmxResetCICountEdgesCountResetThreshVoltage = 0;
decltype(&__7c514522_DAQmxResetCICountEdgesDigFltrEnable) DAQmxResetCICountEdgesDigFltrEnable = 0;
decltype(&__5f7f223a_DAQmxResetCICountEdgesDigFltrMinPulseWidth) DAQmxResetCICountEdgesDigFltrMinPulseWidth = 0;
decltype(&__7b1c2729_DAQmxResetCICountEdgesDigFltrTimebaseRate) DAQmxResetCICountEdgesDigFltrTimebaseRate = 0;
decltype(&__1e1d343e_DAQmxResetCICountEdgesDigFltrTimebaseSrc) DAQmxResetCICountEdgesDigFltrTimebaseSrc = 0;
decltype(&__6d445038_DAQmxResetCICountEdgesDigSyncEnable) DAQmxResetCICountEdgesDigSyncEnable = 0;
decltype(&__793e2237_DAQmxResetCICountEdgesDir) DAQmxResetCICountEdgesDir = 0;
decltype(&__146a4745_DAQmxResetCICountEdgesDirTerm) DAQmxResetCICountEdgesDirTerm = 0;
decltype(&__08340243_DAQmxResetCICountEdgesGateDigFltrEnable) DAQmxResetCICountEdgesGateDigFltrEnable = 0;
decltype(&__2b1a655b_DAQmxResetCICountEdgesGateDigFltrMinPulseWidth) DAQmxResetCICountEdgesGateDigFltrMinPulseWidth = 0;
decltype(&__0f796048_DAQmxResetCICountEdgesGateDigFltrTimebaseRate) DAQmxResetCICountEdgesGateDigFltrTimebaseRate = 0;
decltype(&__6a78735f_DAQmxResetCICountEdgesGateDigFltrTimebaseSrc) DAQmxResetCICountEdgesGateDigFltrTimebaseSrc = 0;
decltype(&__1e390834_DAQmxResetCICountEdgesGateEnable) DAQmxResetCICountEdgesGateEnable = 0;
decltype(&__0c2f6946_DAQmxResetCICountEdgesGateHyst) DAQmxResetCICountEdgesGateHyst = 0;
decltype(&__694d3a10_DAQmxResetCICountEdgesGateLogicLvlBehavior) DAQmxResetCICountEdgesGateLogicLvlBehavior = 0;
decltype(&__0d36755a_DAQmxResetCICountEdgesGateTerm) DAQmxResetCICountEdgesGateTerm = 0;
decltype(&__6a36363c_DAQmxResetCICountEdgesGateTermCfg) DAQmxResetCICountEdgesGateTermCfg = 0;
decltype(&__3a360f4b_DAQmxResetCICountEdgesGateThreshVoltage) DAQmxResetCICountEdgesGateThreshVoltage = 0;
decltype(&__1a357657_DAQmxResetCICountEdgesGateWhen) DAQmxResetCICountEdgesGateWhen = 0;
decltype(&__784a2e27_DAQmxResetCICountEdgesHyst) DAQmxResetCICountEdgesHyst = 0;
decltype(&__0e092825_DAQmxResetCICountEdgesInitialCnt) DAQmxResetCICountEdgesInitialCnt = 0;
decltype(&__1d287d71_DAQmxResetCICountEdgesLogicLvlBehavior) DAQmxResetCICountEdgesLogicLvlBehavior = 0;
decltype(&__7953323b_DAQmxResetCICountEdgesTerm) DAQmxResetCICountEdgesTerm = 0;
decltype(&__1e53715d_DAQmxResetCICountEdgesTermCfg) DAQmxResetCICountEdgesTermCfg = 0;
decltype(&__4e53482a_DAQmxResetCICountEdgesThreshVoltage) DAQmxResetCICountEdgesThreshVoltage = 0;
decltype(&__713c0831_DAQmxResetCICtrTimebaseActiveEdge) DAQmxResetCICtrTimebaseActiveEdge = 0;
decltype(&__6e7a6a42_DAQmxResetCICtrTimebaseDigFltrEnable) DAQmxResetCICtrTimebaseDigFltrEnable = 0;
decltype(&__76594425_DAQmxResetCICtrTimebaseDigFltrMinPulseWidth) DAQmxResetCICtrTimebaseDigFltrMinPulseWidth = 0;
decltype(&__657d2720_DAQmxResetCICtrTimebaseDigFltrTimebaseRate) DAQmxResetCICtrTimebaseDigFltrTimebaseRate = 0;
decltype(&__72182633_DAQmxResetCICtrTimebaseDigFltrTimebaseSrc) DAQmxResetCICtrTimebaseDigFltrTimebaseSrc = 0;
decltype(&__746b7f57_DAQmxResetCICtrTimebaseDigSyncEnable) DAQmxResetCICtrTimebaseDigSyncEnable = 0;
decltype(&__010c1021_DAQmxResetCICtrTimebaseMasterTimebaseDiv) DAQmxResetCICtrTimebaseMasterTimebaseDiv = 0;
decltype(&__73796033_DAQmxResetCICtrTimebaseRate) DAQmxResetCICtrTimebaseRate = 0;
decltype(&__606e0532_DAQmxResetCICtrTimebaseSrc) DAQmxResetCICtrTimebaseSrc = 0;
decltype(&__757e570e_DAQmxResetCICustomScaleName) DAQmxResetCICustomScaleName = 0;
decltype(&__0805052c_DAQmxResetCIDataXferMech) DAQmxResetCIDataXferMech = 0;
decltype(&__786b7307_DAQmxResetCIDataXferReqCond) DAQmxResetCIDataXferReqCond = 0;
decltype(&__477b7816_DAQmxResetCIDupCountPrevent) DAQmxResetCIDupCountPrevent = 0;
decltype(&__54481c35_DAQmxResetCIDutyCycleDigFltrEnable) DAQmxResetCIDutyCycleDigFltrEnable = 0;
decltype(&__7a2f0416_DAQmxResetCIDutyCycleDigFltrMinPulseWidth) DAQmxResetCIDutyCycleDigFltrMinPulseWidth = 0;
decltype(&__192a1732_DAQmxResetCIDutyCycleDigFltrTimebaseRate) DAQmxResetCIDutyCycleDigFltrTimebaseRate = 0;
decltype(&__18390057_DAQmxResetCIDutyCycleDigFltrTimebaseSrc) DAQmxResetCIDutyCycleDigFltrTimebaseSrc = 0;
decltype(&__2d704f54_DAQmxResetCIDutyCycleLogicLvlBehavior) DAQmxResetCIDutyCycleLogicLvlBehavior = 0;
decltype(&__4b09192a_DAQmxResetCIDutyCycleStartingEdge) DAQmxResetCIDutyCycleStartingEdge = 0;
decltype(&__563f0530_DAQmxResetCIDutyCycleTerm) DAQmxResetCIDutyCycleTerm = 0;
decltype(&__567c6357_DAQmxResetCIDutyCycleTermCfg) DAQmxResetCIDutyCycleTermCfg = 0;
decltype(&__2a216a7b_DAQmxResetCIEncoderAInputDigFltrEnable) DAQmxResetCIEncoderAInputDigFltrEnable = 0;
decltype(&__04467258_DAQmxResetCIEncoderAInputDigFltrMinPulseWidth) DAQmxResetCIEncoderAInputDigFltrMinPulseWidth = 0;
decltype(&__6743617c_DAQmxResetCIEncoderAInputDigFltrTimebaseRate) DAQmxResetCIEncoderAInputDigFltrTimebaseRate = 0;
decltype(&__66507619_DAQmxResetCIEncoderAInputDigFltrTimebaseSrc) DAQmxResetCIEncoderAInputDigFltrTimebaseSrc = 0;
decltype(&__3f34706a_DAQmxResetCIEncoderAInputDigSyncEnable) DAQmxResetCIEncoderAInputDigSyncEnable = 0;
decltype(&__5319391a_DAQmxResetCIEncoderAInputLogicLvlBehavior) DAQmxResetCIEncoderAInputLogicLvlBehavior = 0;
decltype(&__2856737e_DAQmxResetCIEncoderAInputTerm) DAQmxResetCIEncoderAInputTerm = 0;
decltype(&__28151519_DAQmxResetCIEncoderAInputTermCfg) DAQmxResetCIEncoderAInputTermCfg = 0;
decltype(&__2a216a78_DAQmxResetCIEncoderBInputDigFltrEnable) DAQmxResetCIEncoderBInputDigFltrEnable = 0;
decltype(&__0446725b_DAQmxResetCIEncoderBInputDigFltrMinPulseWidth) DAQmxResetCIEncoderBInputDigFltrMinPulseWidth = 0;
decltype(&__6743617f_DAQmxResetCIEncoderBInputDigFltrTimebaseRate) DAQmxResetCIEncoderBInputDigFltrTimebaseRate = 0;
decltype(&__6650761a_DAQmxResetCIEncoderBInputDigFltrTimebaseSrc) DAQmxResetCIEncoderBInputDigFltrTimebaseSrc = 0;
decltype(&__3f347069_DAQmxResetCIEncoderBInputDigSyncEnable) DAQmxResetCIEncoderBInputDigSyncEnable = 0;
decltype(&__53193919_DAQmxResetCIEncoderBInputLogicLvlBehavior) DAQmxResetCIEncoderBInputLogicLvlBehavior = 0;
decltype(&__2856737d_DAQmxResetCIEncoderBInputTerm) DAQmxResetCIEncoderBInputTerm = 0;
decltype(&__2815151a_DAQmxResetCIEncoderBInputTermCfg) DAQmxResetCIEncoderBInputTermCfg = 0;
decltype(&__0d110b4c_DAQmxResetCIEncoderDecodingType) DAQmxResetCIEncoderDecodingType = 0;
decltype(&__2b2b0966_DAQmxResetCIEncoderZIndexEnable) DAQmxResetCIEncoderZIndexEnable = 0;
decltype(&__3a376a66_DAQmxResetCIEncoderZIndexPhase) DAQmxResetCIEncoderZIndexPhase = 0;
decltype(&__4954636b_DAQmxResetCIEncoderZIndexVal) DAQmxResetCIEncoderZIndexVal = 0;
decltype(&__2a216a60_DAQmxResetCIEncoderZInputDigFltrEnable) DAQmxResetCIEncoderZInputDigFltrEnable = 0;
decltype(&__04467243_DAQmxResetCIEncoderZInputDigFltrMinPulseWidth) DAQmxResetCIEncoderZInputDigFltrMinPulseWidth = 0;
decltype(&__67436167_DAQmxResetCIEncoderZInputDigFltrTimebaseRate) DAQmxResetCIEncoderZInputDigFltrTimebaseRate = 0;
decltype(&__66507602_DAQmxResetCIEncoderZInputDigFltrTimebaseSrc) DAQmxResetCIEncoderZInputDigFltrTimebaseSrc = 0;
decltype(&__3f347071_DAQmxResetCIEncoderZInputDigSyncEnable) DAQmxResetCIEncoderZInputDigSyncEnable = 0;
decltype(&__53193901_DAQmxResetCIEncoderZInputLogicLvlBehavior) DAQmxResetCIEncoderZInputLogicLvlBehavior = 0;
decltype(&__28567365_DAQmxResetCIEncoderZInputTerm) DAQmxResetCIEncoderZInputTerm = 0;
decltype(&__28151502_DAQmxResetCIEncoderZInputTermCfg) DAQmxResetCIEncoderZInputTermCfg = 0;
decltype(&__78745260_DAQmxResetCIFilterDelayUnits) DAQmxResetCIFilterDelayUnits = 0;
decltype(&__1b1e3228_DAQmxResetCIFilterEnable) DAQmxResetCIFilterEnable = 0;
decltype(&__1f0d5d51_DAQmxResetCIFilterFreq) DAQmxResetCIFilterFreq = 0;
decltype(&__1e192651_DAQmxResetCIFilterOrder) DAQmxResetCIFilterOrder = 0;
decltype(&__7a692628_DAQmxResetCIFilterResponse) DAQmxResetCIFilterResponse = 0;
decltype(&__3c696549_DAQmxResetCIFreqDigFltrEnable) DAQmxResetCIFreqDigFltrEnable = 0;
decltype(&__5b714667_DAQmxResetCIFreqDigFltrMinPulseWidth) DAQmxResetCIFreqDigFltrMinPulseWidth = 0;
decltype(&__5e626204_DAQmxResetCIFreqDigFltrTimebaseRate) DAQmxResetCIFreqDigFltrTimebaseRate = 0;
decltype(&__4d750705_DAQmxResetCIFreqDigFltrTimebaseSrc) DAQmxResetCIFreqDigFltrTimebaseSrc = 0;
decltype(&__2973745c_DAQmxResetCIFreqDigSyncEnable) DAQmxResetCIFreqDigSyncEnable = 0;
decltype(&__5b7c6426_DAQmxResetCIFreqDiv) DAQmxResetCIFreqDiv = 0;
decltype(&__3a023455_DAQmxResetCIFreqEnableAveraging) DAQmxResetCIFreqEnableAveraging = 0;
decltype(&__576c6152_DAQmxResetCIFreqHyst) DAQmxResetCIFreqHyst = 0;
decltype(&__043a0430_DAQmxResetCIFreqLogicLvlBehavior) DAQmxResetCIFreqLogicLvlBehavior = 0;
decltype(&__1f15073d_DAQmxResetCIFreqMeasMeth) DAQmxResetCIFreqMeasMeth = 0;
decltype(&__06191e30_DAQmxResetCIFreqMeasTime) DAQmxResetCIFreqMeasTime = 0;
decltype(&__7d6c7a56_DAQmxResetCIFreqStartingEdge) DAQmxResetCIFreqStartingEdge = 0;
decltype(&__4b70604b_DAQmxResetCIFreqTerm) DAQmxResetCIFreqTerm = 0;
decltype(&__0816074b_DAQmxResetCIFreqTermCfg) DAQmxResetCIFreqTermCfg = 0;
decltype(&__3161574b_DAQmxResetCIFreqThreshVoltage) DAQmxResetCIFreqThreshVoltage = 0;
decltype(&__397b7b52_DAQmxResetCIFreqUnits) DAQmxResetCIFreqUnits = 0;
decltype(&__662d2f26_DAQmxResetCIGPSSyncMethod) DAQmxResetCIGPSSyncMethod = 0;
decltype(&__153a4757_DAQmxResetCIGPSSyncSrc) DAQmxResetCIGPSSyncSrc = 0;
decltype(&__6c5e1716_DAQmxResetCILinEncoderDistPerPulse) DAQmxResetCILinEncoderDistPerPulse = 0;
decltype(&__1b3b390a_DAQmxResetCILinEncoderInitialPos) DAQmxResetCILinEncoderInitialPos = 0;
decltype(&__776b5018_DAQmxResetCILinEncoderUnits) DAQmxResetCILinEncoderUnits = 0;
decltype(&__14060f57_DAQmxResetCIMax) DAQmxResetCIMax = 0;
decltype(&__70151525_DAQmxResetCIMaxMeasPeriod) DAQmxResetCIMaxMeasPeriod = 0;
decltype(&__14103311_DAQmxResetCIMemMapEnable) DAQmxResetCIMemMapEnable = 0;
decltype(&__140e1957_DAQmxResetCIMin) DAQmxResetCIMin = 0;
decltype(&__11092642_DAQmxResetCIPeriodDigFltrEnable) DAQmxResetCIPeriodDigFltrEnable = 0;
decltype(&__3227415a_DAQmxResetCIPeriodDigFltrMinPulseWidth) DAQmxResetCIPeriodDigFltrMinPulseWidth = 0;
decltype(&__16444449_DAQmxResetCIPeriodDigFltrTimebaseRate) DAQmxResetCIPeriodDigFltrTimebaseRate = 0;
decltype(&__7345575e_DAQmxResetCIPeriodDigFltrTimebaseSrc) DAQmxResetCIPeriodDigFltrTimebaseSrc = 0;
decltype(&__001c3358_DAQmxResetCIPeriodDigSyncEnable) DAQmxResetCIPeriodDigSyncEnable = 0;
decltype(&__10664157_DAQmxResetCIPeriodDiv) DAQmxResetCIPeriodDiv = 0;
decltype(&__40152029_DAQmxResetCIPeriodEnableAveraging) DAQmxResetCIPeriodEnableAveraging = 0;
decltype(&__15124d47_DAQmxResetCIPeriodHyst) DAQmxResetCIPeriodHyst = 0;
decltype(&__70701e11_DAQmxResetCIPeriodLogicLvlBehavior) DAQmxResetCIPeriodLogicLvlBehavior = 0;
decltype(&__737d053e_DAQmxResetCIPeriodMeasMeth) DAQmxResetCIPeriodMeasMeth = 0;
decltype(&__6a701c32_DAQmxResetCIPeriodMeasTime) DAQmxResetCIPeriodMeasTime = 0;
decltype(&__0e166747_DAQmxResetCIPeriodStartingEdge) DAQmxResetCIPeriodStartingEdge = 0;
decltype(&__140b515b_DAQmxResetCIPeriodTerm) DAQmxResetCIPeriodTerm = 0;
decltype(&__730b123d_DAQmxResetCIPeriodTermCfg) DAQmxResetCIPeriodTermCfg = 0;
decltype(&__230b2b4a_DAQmxResetCIPeriodThreshVoltage) DAQmxResetCIPeriodThreshVoltage = 0;
decltype(&__0f122350_DAQmxResetCIPeriodUnits) DAQmxResetCIPeriodUnits = 0;
decltype(&__18747e41_DAQmxResetCIPrescaler) DAQmxResetCIPrescaler = 0;
decltype(&__72771536_DAQmxResetCIPulseFreqDigFltrEnable) DAQmxResetCIPulseFreqDigFltrEnable = 0;
decltype(&__5c100d15_DAQmxResetCIPulseFreqDigFltrMinPulseWidth) DAQmxResetCIPulseFreqDigFltrMinPulseWidth = 0;
decltype(&__3f151e31_DAQmxResetCIPulseFreqDigFltrTimebaseRate) DAQmxResetCIPulseFreqDigFltrTimebaseRate = 0;
decltype(&__3e060954_DAQmxResetCIPulseFreqDigFltrTimebaseSrc) DAQmxResetCIPulseFreqDigFltrTimebaseSrc = 0;
decltype(&__67620f27_DAQmxResetCIPulseFreqDigSyncEnable) DAQmxResetCIPulseFreqDigSyncEnable = 0;
decltype(&__0b4f4657_DAQmxResetCIPulseFreqLogicLvlBehavior) DAQmxResetCIPulseFreqLogicLvlBehavior = 0;
decltype(&__08165844_DAQmxResetCIPulseFreqStartEdge) DAQmxResetCIPulseFreqStartEdge = 0;
decltype(&__70000c33_DAQmxResetCIPulseFreqTerm) DAQmxResetCIPulseFreqTerm = 0;
decltype(&__70436a54_DAQmxResetCIPulseFreqTermCfg) DAQmxResetCIPulseFreqTermCfg = 0;
decltype(&__69720728_DAQmxResetCIPulseFreqUnits) DAQmxResetCIPulseFreqUnits = 0;
decltype(&__705a513b_DAQmxResetCIPulseTicksDigFltrEnable) DAQmxResetCIPulseTicksDigFltrEnable = 0;
decltype(&__53743623_DAQmxResetCIPulseTicksDigFltrMinPulseWidth) DAQmxResetCIPulseTicksDigFltrMinPulseWidth = 0;
decltype(&__77173330_DAQmxResetCIPulseTicksDigFltrTimebaseRate) DAQmxResetCIPulseTicksDigFltrTimebaseRate = 0;
decltype(&__12162027_DAQmxResetCIPulseTicksDigFltrTimebaseSrc) DAQmxResetCIPulseTicksDigFltrTimebaseSrc = 0;
decltype(&__614f4421_DAQmxResetCIPulseTicksDigSyncEnable) DAQmxResetCIPulseTicksDigSyncEnable = 0;
decltype(&__11236968_DAQmxResetCIPulseTicksLogicLvlBehavior) DAQmxResetCIPulseTicksLogicLvlBehavior = 0;
decltype(&__02203076_DAQmxResetCIPulseTicksStartEdge) DAQmxResetCIPulseTicksStartEdge = 0;
decltype(&__75582622_DAQmxResetCIPulseTicksTerm) DAQmxResetCIPulseTicksTerm = 0;
decltype(&__12586544_DAQmxResetCIPulseTicksTermCfg) DAQmxResetCIPulseTicksTermCfg = 0;
decltype(&__66650e3e_DAQmxResetCIPulseTimeDigFltrEnable) DAQmxResetCIPulseTimeDigFltrEnable = 0;
decltype(&__4802161d_DAQmxResetCIPulseTimeDigFltrMinPulseWidth) DAQmxResetCIPulseTimeDigFltrMinPulseWidth = 0;
decltype(&__2b070539_DAQmxResetCIPulseTimeDigFltrTimebaseRate) DAQmxResetCIPulseTimeDigFltrTimebaseRate = 0;
decltype(&__2a14125c_DAQmxResetCIPulseTimeDigFltrTimebaseSrc) DAQmxResetCIPulseTimeDigFltrTimebaseSrc = 0;
decltype(&__7370142f_DAQmxResetCIPulseTimeDigSyncEnable) DAQmxResetCIPulseTimeDigSyncEnable = 0;
decltype(&__1f5d5d5f_DAQmxResetCIPulseTimeLogicLvlBehavior) DAQmxResetCIPulseTimeLogicLvlBehavior = 0;
decltype(&__1c04434c_DAQmxResetCIPulseTimeStartEdge) DAQmxResetCIPulseTimeStartEdge = 0;
decltype(&__6412173b_DAQmxResetCIPulseTimeTerm) DAQmxResetCIPulseTimeTerm = 0;
decltype(&__6451715c_DAQmxResetCIPulseTimeTermCfg) DAQmxResetCIPulseTimeTermCfg = 0;
decltype(&__7d601c20_DAQmxResetCIPulseTimeUnits) DAQmxResetCIPulseTimeUnits = 0;
decltype(&__6f42513c_DAQmxResetCIPulseWidthDigFltrEnable) DAQmxResetCIPulseWidthDigFltrEnable = 0;
decltype(&__4c6c3624_DAQmxResetCIPulseWidthDigFltrMinPulseWidth) DAQmxResetCIPulseWidthDigFltrMinPulseWidth = 0;
decltype(&__680f3337_DAQmxResetCIPulseWidthDigFltrTimebaseRate) DAQmxResetCIPulseWidthDigFltrTimebaseRate = 0;
decltype(&__0d0e2020_DAQmxResetCIPulseWidthDigFltrTimebaseSrc) DAQmxResetCIPulseWidthDigFltrTimebaseSrc = 0;
decltype(&__7e574426_DAQmxResetCIPulseWidthDigSyncEnable) DAQmxResetCIPulseWidthDigSyncEnable = 0;
decltype(&__0e3b696f_DAQmxResetCIPulseWidthLogicLvlBehavior) DAQmxResetCIPulseWidthLogicLvlBehavior = 0;
decltype(&__705d1039_DAQmxResetCIPulseWidthStartingEdge) DAQmxResetCIPulseWidthStartingEdge = 0;
decltype(&__6a402625_DAQmxResetCIPulseWidthTerm) DAQmxResetCIPulseWidthTerm = 0;
decltype(&__0d406543_DAQmxResetCIPulseWidthTermCfg) DAQmxResetCIPulseWidthTermCfg = 0;
decltype(&__7159542e_DAQmxResetCIPulseWidthUnits) DAQmxResetCIPulseWidthUnits = 0;
decltype(&__4d723716_DAQmxResetCISampClkOverrunBehavior) DAQmxResetCISampClkOverrunBehavior = 0;
decltype(&__2d796f70_DAQmxResetCISampClkOverrunSentinelVal) DAQmxResetCISampClkOverrunSentinelVal = 0;
decltype(&__426c4b2b_DAQmxResetCISemiPeriodDigFltrEnable) DAQmxResetCISemiPeriodDigFltrEnable = 0;
decltype(&__61422c33_DAQmxResetCISemiPeriodDigFltrMinPulseWidth) DAQmxResetCISemiPeriodDigFltrMinPulseWidth = 0;
decltype(&__45212920_DAQmxResetCISemiPeriodDigFltrTimebaseRate) DAQmxResetCISemiPeriodDigFltrTimebaseRate = 0;
decltype(&__20203a37_DAQmxResetCISemiPeriodDigFltrTimebaseSrc) DAQmxResetCISemiPeriodDigFltrTimebaseSrc = 0;
decltype(&__53795e31_DAQmxResetCISemiPeriodDigSyncEnable) DAQmxResetCISemiPeriodDigSyncEnable = 0;
decltype(&__23157378_DAQmxResetCISemiPeriodLogicLvlBehavior) DAQmxResetCISemiPeriodLogicLvlBehavior = 0;
decltype(&__5d730a2e_DAQmxResetCISemiPeriodStartingEdge) DAQmxResetCISemiPeriodStartingEdge = 0;
decltype(&__476e3c32_DAQmxResetCISemiPeriodTerm) DAQmxResetCISemiPeriodTerm = 0;
decltype(&__206e7f54_DAQmxResetCISemiPeriodTermCfg) DAQmxResetCISemiPeriodTermCfg = 0;
decltype(&__5c774e39_DAQmxResetCISemiPeriodUnits) DAQmxResetCISemiPeriodUnits = 0;
decltype(&__7713323a_DAQmxResetCIThreshVoltage) DAQmxResetCIThreshVoltage = 0;
decltype(&__475b6435_DAQmxResetCITimestampInitialSeconds) DAQmxResetCITimestampInitialSeconds = 0;
decltype(&__7a5c1536_DAQmxResetCITimestampUnits) DAQmxResetCITimestampUnits = 0;
decltype(&__0203200b_DAQmxResetCITwoEdgeSepFirstDigFltrEnable) DAQmxResetCITwoEdgeSepFirstDigFltrEnable = 0;
decltype(&__1a200e6c_DAQmxResetCITwoEdgeSepFirstDigFltrMinPulseWidth) DAQmxResetCITwoEdgeSepFirstDigFltrMinPulseWidth = 0;
decltype(&__09046d69_DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseRate) DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseRate = 0;
decltype(&__1e616c7a_DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseSrc) DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseSrc = 0;
decltype(&__1812351e_DAQmxResetCITwoEdgeSepFirstDigSyncEnable) DAQmxResetCITwoEdgeSepFirstDigSyncEnable = 0;
decltype(&__1a132a6d_DAQmxResetCITwoEdgeSepFirstEdge) DAQmxResetCITwoEdgeSepFirstEdge = 0;
decltype(&__51625933_DAQmxResetCITwoEdgeSepFirstLogicLvlBehavior) DAQmxResetCITwoEdgeSepFirstLogicLvlBehavior = 0;
decltype(&__1b06227c_DAQmxResetCITwoEdgeSepFirstTerm) DAQmxResetCITwoEdgeSepFirstTerm = 0;
decltype(&__7d61223f_DAQmxResetCITwoEdgeSepFirstTermCfg) DAQmxResetCITwoEdgeSepFirstTermCfg = 0;
decltype(&__4c14372f_DAQmxResetCITwoEdgeSepSecondDigFltrEnable) DAQmxResetCITwoEdgeSepSecondDigFltrEnable = 0;
decltype(&__2b0c1401_DAQmxResetCITwoEdgeSepSecondDigFltrMinPulseWidth) DAQmxResetCITwoEdgeSepSecondDigFltrMinPulseWidth = 0;
decltype(&__2e1f3062_DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseRate) DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseRate = 0;
decltype(&__3d085563_DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseSrc) DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseSrc = 0;
decltype(&__590e263a_DAQmxResetCITwoEdgeSepSecondDigSyncEnable) DAQmxResetCITwoEdgeSepSecondDigSyncEnable = 0;
decltype(&__2a0c2725_DAQmxResetCITwoEdgeSepSecondEdge) DAQmxResetCITwoEdgeSepSecondEdge = 0;
decltype(&__74475656_DAQmxResetCITwoEdgeSepSecondLogicLvlBehavior) DAQmxResetCITwoEdgeSepSecondLogicLvlBehavior = 0;
decltype(&__3b0d322d_DAQmxResetCITwoEdgeSepSecondTerm) DAQmxResetCITwoEdgeSepSecondTerm = 0;
decltype(&__786b552d_DAQmxResetCITwoEdgeSepSecondTermCfg) DAQmxResetCITwoEdgeSepSecondTermCfg = 0;
decltype(&__65735b2f_DAQmxResetCITwoEdgeSepUnits) DAQmxResetCITwoEdgeSepUnits = 0;
decltype(&__7a6e5032_DAQmxResetCIUsbXferReqCount) DAQmxResetCIUsbXferReqCount = 0;
decltype(&__75653434_DAQmxResetCIUsbXferReqSize) DAQmxResetCIUsbXferReqSize = 0;
decltype(&__7f742034_DAQmxResetCIVelocityAngEncoderPulsesPerRev) DAQmxResetCIVelocityAngEncoderPulsesPerRev = 0;
decltype(&__4f60410e_DAQmxResetCIVelocityAngEncoderUnits) DAQmxResetCIVelocityAngEncoderUnits = 0;
decltype(&__28021941_DAQmxResetCIVelocityDiv) DAQmxResetCIVelocityDiv = 0;
decltype(&__1f2d726d_DAQmxResetCIVelocityEncoderAInputDigFltrEnable) DAQmxResetCIVelocityEncoderAInputDigFltrEnable = 0;
decltype(&__314a6a4e_DAQmxResetCIVelocityEncoderAInputDigFltrMinPulseWidth) DAQmxResetCIVelocityEncoderAInputDigFltrMinPulseWidth = 0;
decltype(&__524f796a_DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseRate) DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseRate = 0;
decltype(&__535c6e0f_DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseSrc) DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseSrc = 0;
decltype(&__6615210c_DAQmxResetCIVelocityEncoderAInputLogicLvlBehavior) DAQmxResetCIVelocityEncoderAInputLogicLvlBehavior = 0;
decltype(&__1d5a6b68_DAQmxResetCIVelocityEncoderAInputTerm) DAQmxResetCIVelocityEncoderAInputTerm = 0;
decltype(&__1d190d0f_DAQmxResetCIVelocityEncoderAInputTermCfg) DAQmxResetCIVelocityEncoderAInputTermCfg = 0;
decltype(&__1f2d726e_DAQmxResetCIVelocityEncoderBInputDigFltrEnable) DAQmxResetCIVelocityEncoderBInputDigFltrEnable = 0;
decltype(&__314a6a4d_DAQmxResetCIVelocityEncoderBInputDigFltrMinPulseWidth) DAQmxResetCIVelocityEncoderBInputDigFltrMinPulseWidth = 0;
decltype(&__524f7969_DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseRate) DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseRate = 0;
decltype(&__535c6e0c_DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseSrc) DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseSrc = 0;
decltype(&__6615210f_DAQmxResetCIVelocityEncoderBInputLogicLvlBehavior) DAQmxResetCIVelocityEncoderBInputLogicLvlBehavior = 0;
decltype(&__1d5a6b6b_DAQmxResetCIVelocityEncoderBInputTerm) DAQmxResetCIVelocityEncoderBInputTerm = 0;
decltype(&__1d190d0c_DAQmxResetCIVelocityEncoderBInputTermCfg) DAQmxResetCIVelocityEncoderBInputTermCfg = 0;
decltype(&__381d135a_DAQmxResetCIVelocityEncoderDecodingType) DAQmxResetCIVelocityEncoderDecodingType = 0;
decltype(&__59520f00_DAQmxResetCIVelocityLinEncoderDistPerPulse) DAQmxResetCIVelocityLinEncoderDistPerPulse = 0;
decltype(&__4267480e_DAQmxResetCIVelocityLinEncoderUnits) DAQmxResetCIVelocityLinEncoderUnits = 0;
decltype(&__75676357_DAQmxResetCIVelocityMeasTime) DAQmxResetCIVelocityMeasTime = 0;
decltype(&__1212144c_DAQmxResetCOAutoIncrCnt) DAQmxResetCOAutoIncrCnt = 0;
decltype(&__01145163_DAQmxResetCOConstrainedGenMode) DAQmxResetCOConstrainedGenMode = 0;
decltype(&__713c0837_DAQmxResetCOCtrTimebaseActiveEdge) DAQmxResetCOCtrTimebaseActiveEdge = 0;
decltype(&__6e7a6a44_DAQmxResetCOCtrTimebaseDigFltrEnable) DAQmxResetCOCtrTimebaseDigFltrEnable = 0;
decltype(&__76594423_DAQmxResetCOCtrTimebaseDigFltrMinPulseWidth) DAQmxResetCOCtrTimebaseDigFltrMinPulseWidth = 0;
decltype(&__657d2726_DAQmxResetCOCtrTimebaseDigFltrTimebaseRate) DAQmxResetCOCtrTimebaseDigFltrTimebaseRate = 0;
decltype(&__72182635_DAQmxResetCOCtrTimebaseDigFltrTimebaseSrc) DAQmxResetCOCtrTimebaseDigFltrTimebaseSrc = 0;
decltype(&__746b7f51_DAQmxResetCOCtrTimebaseDigSyncEnable) DAQmxResetCOCtrTimebaseDigSyncEnable = 0;
decltype(&__010c1027_DAQmxResetCOCtrTimebaseMasterTimebaseDiv) DAQmxResetCOCtrTimebaseMasterTimebaseDiv = 0;
decltype(&__73796035_DAQmxResetCOCtrTimebaseRate) DAQmxResetCOCtrTimebaseRate = 0;
decltype(&__606e0534_DAQmxResetCOCtrTimebaseSrc) DAQmxResetCOCtrTimebaseSrc = 0;
decltype(&__0805052a_DAQmxResetCODataXferMech) DAQmxResetCODataXferMech = 0;
decltype(&__786b7301_DAQmxResetCODataXferReqCond) DAQmxResetCODataXferReqCond = 0;
decltype(&__5d270f29_DAQmxResetCOEnableInitialDelayOnRetrigger) DAQmxResetCOEnableInitialDelayOnRetrigger = 0;
decltype(&__14103317_DAQmxResetCOMemMapEnable) DAQmxResetCOMemMapEnable = 0;
decltype(&__18747e47_DAQmxResetCOPrescaler) DAQmxResetCOPrescaler = 0;
decltype(&__15151735_DAQmxResetCOPulseDutyCyc) DAQmxResetCOPulseDutyCyc = 0;
decltype(&__1d546947_DAQmxResetCOPulseFreq) DAQmxResetCOPulseFreq = 0;
decltype(&__54110a23_DAQmxResetCOPulseFreqInitialDelay) DAQmxResetCOPulseFreqInitialDelay = 0;
decltype(&__6972072e_DAQmxResetCOPulseFreqUnits) DAQmxResetCOPulseFreqUnits = 0;
decltype(&__6f7d1b26_DAQmxResetCOPulseHighTicks) DAQmxResetCOPulseHighTicks = 0;
decltype(&__610e1b28_DAQmxResetCOPulseHighTime) DAQmxResetCOPulseHighTime = 0;
decltype(&__7d6d0b2f_DAQmxResetCOPulseIdleState) DAQmxResetCOPulseIdleState = 0;
decltype(&__4b37173e_DAQmxResetCOPulseLowTicks) DAQmxResetCOPulseLowTicks = 0;
decltype(&__38371930_DAQmxResetCOPulseLowTime) DAQmxResetCOPulseLowTime = 0;
decltype(&__01467e50_DAQmxResetCOPulseTerm) DAQmxResetCOPulseTerm = 0;
decltype(&__637c3722_DAQmxResetCOPulseTicksInitialDelay) DAQmxResetCOPulseTicksInitialDelay = 0;
decltype(&__4003112b_DAQmxResetCOPulseTimeInitialDelay) DAQmxResetCOPulseTimeInitialDelay = 0;
decltype(&__7d601c26_DAQmxResetCOPulseTimeUnits) DAQmxResetCOPulseTimeUnits = 0;
decltype(&__7a6e5034_DAQmxResetCOUsbXferReqCount) DAQmxResetCOUsbXferReqCount = 0;
decltype(&__75653432_DAQmxResetCOUsbXferReqSize) DAQmxResetCOUsbXferReqSize = 0;
decltype(&__415f7435_DAQmxResetCOUseOnlyOnBrdMem) DAQmxResetCOUseOnlyOnBrdMem = 0;
decltype(&__390f3a60_DAQmxResetChanAttribute) DAQmxResetChanAttribute = 0;
decltype(&__4b6a4113_DAQmxResetChanDescr) DAQmxResetChanDescr = 0;
decltype(&__0e091519_DAQmxResetChanSyncUnlockBehavior) DAQmxResetChanSyncUnlockBehavior = 0;
decltype(&__37093f1f_DAQmxResetChangeDetectDIFallingEdgePhysicalChans) DAQmxResetChangeDetectDIFallingEdgePhysicalChans = 0;
decltype(&__3d021f51_DAQmxResetChangeDetectDIRisingEdgePhysicalChans) DAQmxResetChangeDetectDIRisingEdgePhysicalChans = 0;
decltype(&__3f0b3d29_DAQmxResetChangeDetectDITristate) DAQmxResetChangeDetectDITristate = 0;
decltype(&__1f76646d_DAQmxResetDIAcquireOn) DAQmxResetDIAcquireOn = 0;
decltype(&__0805022c_DAQmxResetDIDataXferMech) DAQmxResetDIDataXferMech = 0;
decltype(&__786b7407_DAQmxResetDIDataXferReqCond) DAQmxResetDIDataXferReqCond = 0;
decltype(&__7a1b0738_DAQmxResetDIDigFltrEnable) DAQmxResetDIDigFltrEnable = 0;
decltype(&__3736162e_DAQmxResetDIDigFltrEnableBusMode) DAQmxResetDIDigFltrEnableBusMode = 0;
decltype(&__1d032416_DAQmxResetDIDigFltrMinPulseWidth) DAQmxResetDIDigFltrMinPulseWidth = 0;
decltype(&__18100075_DAQmxResetDIDigFltrTimebaseRate) DAQmxResetDIDigFltrTimebaseRate = 0;
decltype(&__0b076574_DAQmxResetDIDigFltrTimebaseSrc) DAQmxResetDIDigFltrTimebaseSrc = 0;
decltype(&__6f01162d_DAQmxResetDIDigSyncEnable) DAQmxResetDIDigSyncEnable = 0;
decltype(&__0c18395b_DAQmxResetDIInvertLines) DAQmxResetDIInvertLines = 0;
decltype(&__1f220f53_DAQmxResetDILogicFamily) DAQmxResetDILogicFamily = 0;
decltype(&__14103411_DAQmxResetDIMemMapEnable) DAQmxResetDIMemMapEnable = 0;
decltype(&__79746d41_DAQmxResetDITristate) DAQmxResetDITristate = 0;
decltype(&__7a6e5732_DAQmxResetDIUsbXferReqCount) DAQmxResetDIUsbXferReqCount = 0;
decltype(&__75653334_DAQmxResetDIUsbXferReqSize) DAQmxResetDIUsbXferReqSize = 0;
decltype(&__0805022a_DAQmxResetDODataXferMech) DAQmxResetDODataXferMech = 0;
decltype(&__786b7401_DAQmxResetDODataXferReqCond) DAQmxResetDODataXferReqCond = 0;
decltype(&__230d6a51_DAQmxResetDOGenerateOn) DAQmxResetDOGenerateOn = 0;
decltype(&__0c18395d_DAQmxResetDOInvertLines) DAQmxResetDOInvertLines = 0;
decltype(&__2c180d5b_DAQmxResetDOLineStatesDoneState) DAQmxResetDOLineStatesDoneState = 0;
decltype(&__600e2b31_DAQmxResetDOLineStatesPausedState) DAQmxResetDOLineStatesPausedState = 0;
decltype(&__361a2c02_DAQmxResetDOLineStatesStartState) DAQmxResetDOLineStatesStartState = 0;
decltype(&__1f220f55_DAQmxResetDOLogicFamily) DAQmxResetDOLogicFamily = 0;
decltype(&__14103417_DAQmxResetDOMemMapEnable) DAQmxResetDOMemMapEnable = 0;
decltype(&__73604007_DAQmxResetDOOutputDriveType) DAQmxResetDOOutputDriveType = 0;
decltype(&__62777723_DAQmxResetDOOvercurrentAutoReenable) DAQmxResetDOOvercurrentAutoReenable = 0;
decltype(&__79677a69_DAQmxResetDOOvercurrentLimit) DAQmxResetDOOvercurrentLimit = 0;
decltype(&__1671715d_DAQmxResetDOOvercurrentReenablePeriod) DAQmxResetDOOvercurrentReenablePeriod = 0;
decltype(&__79746d47_DAQmxResetDOTristate) DAQmxResetDOTristate = 0;
decltype(&__7a6e5734_DAQmxResetDOUsbXferReqCount) DAQmxResetDOUsbXferReqCount = 0;
decltype(&__75653332_DAQmxResetDOUsbXferReqSize) DAQmxResetDOUsbXferReqSize = 0;
decltype(&__415f7335_DAQmxResetDOUseOnlyOnBrdMem) DAQmxResetDOUseOnlyOnBrdMem = 0;
decltype(&__260e2948_DAQmxResetDelayFromSampClkDelay) DAQmxResetDelayFromSampClkDelay = 0;
decltype(&__5e0e290d_DAQmxResetDelayFromSampClkDelayEx) DAQmxResetDelayFromSampClkDelayEx = 0;
decltype(&__48675d6e_DAQmxResetDelayFromSampClkDelayUnits) DAQmxResetDelayFromSampClkDelayUnits = 0;
decltype(&__0d1f5d6e_DAQmxResetDelayFromSampClkDelayUnitsEx) DAQmxResetDelayFromSampClkDelayUnitsEx = 0;
decltype(&__2f0e131e_DAQmxResetDevice) DAQmxResetDevice = 0;
decltype(&__2c6d6e6e_DAQmxResetDigEdgeAdvTrigDigFltrEnable) DAQmxResetDigEdgeAdvTrigDigFltrEnable = 0;
decltype(&__4a757e64_DAQmxResetDigEdgeAdvTrigEdge) DAQmxResetDigEdgeAdvTrigEdge = 0;
decltype(&__5c637a01_DAQmxResetDigEdgeAdvTrigSrc) DAQmxResetDigEdgeAdvTrigSrc = 0;
decltype(&__74600911_DAQmxResetDigEdgeArmStartTrigDigFltrEnable) DAQmxResetDigEdgeArmStartTrigDigFltrEnable = 0;
decltype(&__5a071132_DAQmxResetDigEdgeArmStartTrigDigFltrMinPulseWidth) DAQmxResetDigEdgeArmStartTrigDigFltrMinPulseWidth = 0;
decltype(&__39020216_DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseRate) DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseRate = 0;
decltype(&__38111573_DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseSrc) DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseSrc = 0;
decltype(&__61751300_DAQmxResetDigEdgeArmStartTrigDigSyncEnable) DAQmxResetDigEdgeArmStartTrigDigSyncEnable = 0;
decltype(&__7e061101_DAQmxResetDigEdgeArmStartTrigEdge) DAQmxResetDigEdgeArmStartTrigEdge = 0;
decltype(&__1b100705_DAQmxResetDigEdgeArmStartTrigSrc) DAQmxResetDigEdgeArmStartTrigSrc = 0;
decltype(&__2c7e6f7e_DAQmxResetDigEdgeRefTrigDigFltrEnable) DAQmxResetDigEdgeRefTrigDigFltrEnable = 0;
decltype(&__4b664c50_DAQmxResetDigEdgeRefTrigDigFltrMinPulseWidth) DAQmxResetDigEdgeRefTrigDigFltrMinPulseWidth = 0;
decltype(&__4e756833_DAQmxResetDigEdgeRefTrigDigFltrTimebaseRate) DAQmxResetDigEdgeRefTrigDigFltrTimebaseRate = 0;
decltype(&__5d620d32_DAQmxResetDigEdgeRefTrigDigFltrTimebaseSrc) DAQmxResetDigEdgeRefTrigDigFltrTimebaseSrc = 0;
decltype(&__39647e6b_DAQmxResetDigEdgeRefTrigDigSyncEnable) DAQmxResetDigEdgeRefTrigDigSyncEnable = 0;
decltype(&__4a667f74_DAQmxResetDigEdgeRefTrigEdge) DAQmxResetDigEdgeRefTrigEdge = 0;
decltype(&__5c707b11_DAQmxResetDigEdgeRefTrigSrc) DAQmxResetDigEdgeRefTrigSrc = 0;
decltype(&__370d177f_DAQmxResetDigEdgeStartTrigDigFltrEnable) DAQmxResetDigEdgeStartTrigDigFltrEnable = 0;
decltype(&__14237067_DAQmxResetDigEdgeStartTrigDigFltrMinPulseWidth) DAQmxResetDigEdgeStartTrigDigFltrMinPulseWidth = 0;
decltype(&__30407574_DAQmxResetDigEdgeStartTrigDigFltrTimebaseRate) DAQmxResetDigEdgeStartTrigDigFltrTimebaseRate = 0;
decltype(&__55416663_DAQmxResetDigEdgeStartTrigDigFltrTimebaseSrc) DAQmxResetDigEdgeStartTrigDigFltrTimebaseSrc = 0;
decltype(&__26180265_DAQmxResetDigEdgeStartTrigDigSyncEnable) DAQmxResetDigEdgeStartTrigDigSyncEnable = 0;
decltype(&__27077167_DAQmxResetDigEdgeStartTrigEdge) DAQmxResetDigEdgeStartTrigEdge = 0;
decltype(&__23626771_DAQmxResetDigEdgeStartTrigSrc) DAQmxResetDigEdgeStartTrigSrc = 0;
decltype(&__3828786d_DAQmxResetDigEdgeWatchdogExpirTrigEdge) DAQmxResetDigEdgeWatchdogExpirTrigEdge = 0;
decltype(&__3c4d6e7b_DAQmxResetDigEdgeWatchdogExpirTrigSrc) DAQmxResetDigEdgeWatchdogExpirTrigSrc = 0;
decltype(&__033d7d76_DAQmxResetDigLvlPauseTrigDigFltrEnable) DAQmxResetDigLvlPauseTrigDigFltrEnable = 0;
decltype(&__2d5a6555_DAQmxResetDigLvlPauseTrigDigFltrMinPulseWidth) DAQmxResetDigLvlPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__4e5f7671_DAQmxResetDigLvlPauseTrigDigFltrTimebaseRate) DAQmxResetDigLvlPauseTrigDigFltrTimebaseRate = 0;
decltype(&__4f4c6114_DAQmxResetDigLvlPauseTrigDigFltrTimebaseSrc) DAQmxResetDigLvlPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__16286767_DAQmxResetDigLvlPauseTrigDigSyncEnable) DAQmxResetDigLvlPauseTrigDigSyncEnable = 0;
decltype(&__6c4d7362_DAQmxResetDigLvlPauseTrigSrc) DAQmxResetDigLvlPauseTrigSrc = 0;
decltype(&__02496964_DAQmxResetDigLvlPauseTrigWhen) DAQmxResetDigLvlPauseTrigWhen = 0;
decltype(&__6c52776d_DAQmxResetDigPatternPauseTrigPattern) DAQmxResetDigPatternPauseTrigPattern = 0;
decltype(&__18341614_DAQmxResetDigPatternPauseTrigSrc) DAQmxResetDigPatternPauseTrigSrc = 0;
decltype(&__76300c12_DAQmxResetDigPatternPauseTrigWhen) DAQmxResetDigPatternPauseTrigWhen = 0;
decltype(&__7944160c_DAQmxResetDigPatternRefTrigPattern) DAQmxResetDigPatternRefTrigPattern = 0;
decltype(&__183d626a_DAQmxResetDigPatternRefTrigSrc) DAQmxResetDigPatternRefTrigSrc = 0;
decltype(&__023b0c6e_DAQmxResetDigPatternRefTrigWhen) DAQmxResetDigPatternRefTrigWhen = 0;
decltype(&__7e47636c_DAQmxResetDigPatternStartTrigPattern) DAQmxResetDigPatternStartTrigPattern = 0;
decltype(&__0a210215_DAQmxResetDigPatternStartTrigSrc) DAQmxResetDigPatternStartTrigSrc = 0;
decltype(&__64251813_DAQmxResetDigPatternStartTrigWhen) DAQmxResetDigPatternStartTrigWhen = 0;
decltype(&__1a0a673c_DAQmxResetExported10MHzRefClkOutputTerm) DAQmxResetExported10MHzRefClkOutputTerm = 0;
decltype(&__651a5e5c_DAQmxResetExported20MHzTimebaseOutputTerm) DAQmxResetExported20MHzTimebaseOutputTerm = 0;
decltype(&__204f5265_DAQmxResetExportedAIConvClkOutputTerm) DAQmxResetExportedAIConvClkOutputTerm = 0;
decltype(&__7d4a605a_DAQmxResetExportedAIHoldCmpltEventOutputTerm) DAQmxResetExportedAIHoldCmpltEventOutputTerm = 0;
decltype(&__37340561_DAQmxResetExportedAIHoldCmpltEventPulsePolarity) DAQmxResetExportedAIHoldCmpltEventPulsePolarity = 0;
decltype(&__56597f4d_DAQmxResetExportedAdvCmpltEventDelay) DAQmxResetExportedAdvCmpltEventDelay = 0;
decltype(&__5f150b38_DAQmxResetExportedAdvCmpltEventOutputTerm) DAQmxResetExportedAdvCmpltEventOutputTerm = 0;
decltype(&__645f755d_DAQmxResetExportedAdvCmpltEventPulsePolarity) DAQmxResetExportedAdvCmpltEventPulsePolarity = 0;
decltype(&__79300931_DAQmxResetExportedAdvCmpltEventPulseWidth) DAQmxResetExportedAdvCmpltEventPulseWidth = 0;
decltype(&__48705c3f_DAQmxResetExportedAdvTrigOutputTerm) DAQmxResetExportedAdvTrigOutputTerm = 0;
decltype(&__4a797a1a_DAQmxResetExportedAdvTrigPulseWidth) DAQmxResetExportedAdvTrigPulseWidth = 0;
decltype(&__24100e3c_DAQmxResetExportedAdvTrigPulseWidthUnits) DAQmxResetExportedAdvTrigPulseWidthUnits = 0;
decltype(&__035d7744_DAQmxResetExportedChangeDetectEventOutputTerm) DAQmxResetExportedChangeDetectEventOutputTerm = 0;
decltype(&__38170921_DAQmxResetExportedChangeDetectEventPulsePolarity) DAQmxResetExportedChangeDetectEventPulsePolarity = 0;
decltype(&__7368423c_DAQmxResetExportedCtrOutEventOutputBehavior) DAQmxResetExportedCtrOutEventOutputBehavior = 0;
decltype(&__1a1d3c5c_DAQmxResetExportedCtrOutEventOutputTerm) DAQmxResetExportedCtrOutEventOutputTerm = 0;
decltype(&__64780716_DAQmxResetExportedCtrOutEventPulsePolarity) DAQmxResetExportedCtrOutEventPulsePolarity = 0;
decltype(&__78604b64_DAQmxResetExportedCtrOutEventToggleIdleState) DAQmxResetExportedCtrOutEventToggleIdleState = 0;
decltype(&__26585b68_DAQmxResetExportedDataActiveEventLvlActiveLvl) DAQmxResetExportedDataActiveEventLvlActiveLvl = 0;
decltype(&__685a793b_DAQmxResetExportedDataActiveEventOutputTerm) DAQmxResetExportedDataActiveEventOutputTerm = 0;
decltype(&__5071100d_DAQmxResetExportedDividedSampClkTimebaseOutputTerm) DAQmxResetExportedDividedSampClkTimebaseOutputTerm = 0;
decltype(&__24051b2a_DAQmxResetExportedHshkEventDelay) DAQmxResetExportedHshkEventDelay = 0;
decltype(&__7b6f6834_DAQmxResetExportedHshkEventInterlockedAssertOnStart) DAQmxResetExportedHshkEventInterlockedAssertOnStart = 0;
decltype(&__5c170336_DAQmxResetExportedHshkEventInterlockedAssertedLvl) DAQmxResetExportedHshkEventInterlockedAssertedLvl = 0;
decltype(&__59707622_DAQmxResetExportedHshkEventInterlockedDeassertDelay) DAQmxResetExportedHshkEventInterlockedDeassertDelay = 0;
decltype(&__5329062a_DAQmxResetExportedHshkEventOutputBehavior) DAQmxResetExportedHshkEventOutputBehavior = 0;
decltype(&__2d496f5f_DAQmxResetExportedHshkEventOutputTerm) DAQmxResetExportedHshkEventOutputTerm = 0;
decltype(&__1603113a_DAQmxResetExportedHshkEventPulsePolarity) DAQmxResetExportedHshkEventPulsePolarity = 0;
decltype(&__0b6c6d56_DAQmxResetExportedHshkEventPulseWidth) DAQmxResetExportedHshkEventPulseWidth = 0;
decltype(&__05050a6d_DAQmxResetExportedPauseTrigLvlActiveLvl) DAQmxResetExportedPauseTrigLvlActiveLvl = 0;
decltype(&__2756446f_DAQmxResetExportedPauseTrigOutputTerm) DAQmxResetExportedPauseTrigOutputTerm = 0;
decltype(&__0f34635d_DAQmxResetExportedRdyForStartEventLvlActiveLvl) DAQmxResetExportedRdyForStartEventLvlActiveLvl = 0;
decltype(&__5c7a617f_DAQmxResetExportedRdyForStartEventOutputTerm) DAQmxResetExportedRdyForStartEventOutputTerm = 0;
decltype(&__3f4e5165_DAQmxResetExportedRdyForXferEventDeassertCond) DAQmxResetExportedRdyForXferEventDeassertCond = 0;
decltype(&__4b7f4055_DAQmxResetExportedRdyForXferEventDeassertCondCustomThreshold) DAQmxResetExportedRdyForXferEventDeassertCondCustomThreshold = 0;
decltype(&__0770706b_DAQmxResetExportedRdyForXferEventLvlActiveLvl) DAQmxResetExportedRdyForXferEventLvlActiveLvl = 0;
decltype(&__49725238_DAQmxResetExportedRdyForXferEventOutputTerm) DAQmxResetExportedRdyForXferEventOutputTerm = 0;
decltype(&__58704f3e_DAQmxResetExportedRefTrigOutputTerm) DAQmxResetExportedRefTrigOutputTerm = 0;
decltype(&__26157474_DAQmxResetExportedRefTrigPulsePolarity) DAQmxResetExportedRefTrigPulsePolarity = 0;
decltype(&__4d525c61_DAQmxResetExportedSampClkDelayOffset) DAQmxResetExportedSampClkDelayOffset = 0;
decltype(&__3621015f_DAQmxResetExportedSampClkOutputBehavior) DAQmxResetExportedSampClkOutputBehavior = 0;
decltype(&__5f547f3f_DAQmxResetExportedSampClkOutputTerm) DAQmxResetExportedSampClkOutputTerm = 0;
decltype(&__21314475_DAQmxResetExportedSampClkPulsePolarity) DAQmxResetExportedSampClkPulsePolarity = 0;
decltype(&__5f627721_DAQmxResetExportedSampClkTimebaseOutputTerm) DAQmxResetExportedSampClkTimebaseOutputTerm = 0;
decltype(&__66143011_DAQmxResetExportedSignalAttribute) DAQmxResetExportedSignalAttribute = 0;
decltype(&__3357567a_DAQmxResetExportedStartTrigOutputTerm) DAQmxResetExportedStartTrigOutputTerm = 0;
decltype(&__081d281f_DAQmxResetExportedStartTrigPulsePolarity) DAQmxResetExportedStartTrigPulsePolarity = 0;
decltype(&__70752b3a_DAQmxResetExportedSyncPulseEventOutputTerm) DAQmxResetExportedSyncPulseEventOutputTerm = 0;
decltype(&__0d253503_DAQmxResetExportedWatchdogExpiredEventOutputTerm) DAQmxResetExportedWatchdogExpiredEventOutputTerm = 0;
decltype(&__40615c75_DAQmxResetFirstSampClkOffset) DAQmxResetFirstSampClkOffset = 0;
decltype(&__2a1b346d_DAQmxResetFirstSampClkTimescale) DAQmxResetFirstSampClkTimescale = 0;
decltype(&__437c210f_DAQmxResetFirstSampClkWhen) DAQmxResetFirstSampClkWhen = 0;
decltype(&__7e7e1f11_DAQmxResetFirstSampTimestampEnable) DAQmxResetFirstSampTimestampEnable = 0;
decltype(&__157f727a_DAQmxResetFirstSampTimestampTimescale) DAQmxResetFirstSampTimestampTimescale = 0;
decltype(&__637f4149_DAQmxResetHshkDelayAfterXfer) DAQmxResetHshkDelayAfterXfer = 0;
decltype(&__6e013a0d_DAQmxResetHshkSampleInputDataWhen) DAQmxResetHshkSampleInputDataWhen = 0;
decltype(&__3f103f5a_DAQmxResetHshkStartCond) DAQmxResetHshkStartCond = 0;
decltype(&__280e7c66_DAQmxResetHshkTrigType) DAQmxResetHshkTrigType = 0;
decltype(&__4472572c_DAQmxResetImplicitUnderflowBehavior) DAQmxResetImplicitUnderflowBehavior = 0;
decltype(&__2b021402_DAQmxResetInterlockedHshkTrigAssertedLvl) DAQmxResetInterlockedHshkTrigAssertedLvl = 0;
decltype(&__2a2e171b_DAQmxResetInterlockedHshkTrigSrc) DAQmxResetInterlockedHshkTrigSrc = 0;
decltype(&__54161907_DAQmxResetLoggingFilePath) DAQmxResetLoggingFilePath = 0;
decltype(&__2b104179_DAQmxResetLoggingFilePreallocationSize) DAQmxResetLoggingFilePreallocationSize = 0;
decltype(&__32115973_DAQmxResetLoggingFileWriteSize) DAQmxResetLoggingFileWriteSize = 0;
decltype(&__3c4d7e7b_DAQmxResetLoggingMode) DAQmxResetLoggingMode = 0;
decltype(&__2a35706a_DAQmxResetLoggingPause) DAQmxResetLoggingPause = 0;
decltype(&__3e66497b_DAQmxResetLoggingSampsPerFile) DAQmxResetLoggingSampsPerFile = 0;
decltype(&__1206695c_DAQmxResetLoggingTDMSGroupName) DAQmxResetLoggingTDMSGroupName = 0;
decltype(&__1714515e_DAQmxResetLoggingTDMSOperation) DAQmxResetLoggingTDMSOperation = 0;
decltype(&__4e7a7668_DAQmxResetMasterTimebaseRate) DAQmxResetMasterTimebaseRate = 0;
decltype(&__4f69610d_DAQmxResetMasterTimebaseSrc) DAQmxResetMasterTimebaseSrc = 0;
decltype(&__23056c4e_DAQmxResetOnDemandSimultaneousAOEnable) DAQmxResetOnDemandSimultaneousAOEnable = 0;
decltype(&__270d037f_DAQmxResetPauseTrigType) DAQmxResetPauseTrigType = 0;
decltype(&__5f431931_DAQmxResetPhysicalChanAIPowerControlEnable) DAQmxResetPhysicalChanAIPowerControlEnable = 0;
decltype(&__22310836_DAQmxResetPhysicalChanAIPowerControlType) DAQmxResetPhysicalChanAIPowerControlType = 0;
decltype(&__41407127_DAQmxResetPhysicalChanAIPowerControlVoltage) DAQmxResetPhysicalChanAIPowerControlVoltage = 0;
decltype(&__6c465b26_DAQmxResetPhysicalChanAOManualControlEnable) DAQmxResetPhysicalChanAOManualControlEnable = 0;
decltype(&__39052b6d_DAQmxResetReadAttribute) DAQmxResetReadAttribute = 0;
decltype(&__2d1e007a_DAQmxResetReadAutoStart) DAQmxResetReadAutoStart = 0;
decltype(&__677b7e7d_DAQmxResetReadChannelsToRead) DAQmxResetReadChannelsToRead = 0;
decltype(&__5e704c69_DAQmxResetReadOffset) DAQmxResetReadOffset = 0;
decltype(&__34051b7f_DAQmxResetReadOverWrite) DAQmxResetReadOverWrite = 0;
decltype(&__313a5072_DAQmxResetReadReadAllAvailSamp) DAQmxResetReadReadAllAvailSamp = 0;
decltype(&__22071418_DAQmxResetReadRelativeTo) DAQmxResetReadRelativeTo = 0;
decltype(&__340b2043_DAQmxResetReadSleepTime) DAQmxResetReadSleepTime = 0;
decltype(&__35127c75_DAQmxResetReadWaitMode) DAQmxResetReadWaitMode = 0;
decltype(&__54687f04_DAQmxResetRealTimeAttribute) DAQmxResetRealTimeAttribute = 0;
decltype(&__7c603412_DAQmxResetRealTimeConvLateErrorsToWarnings) DAQmxResetRealTimeConvLateErrorsToWarnings = 0;
decltype(&__5b690320_DAQmxResetRealTimeNumOfWarmupIters) DAQmxResetRealTimeNumOfWarmupIters = 0;
decltype(&__607c3211_DAQmxResetRealTimeReportMissedSamp) DAQmxResetRealTimeReportMissedSamp = 0;
decltype(&__504a4673_DAQmxResetRealTimeWaitForNextSampClkWaitMode) DAQmxResetRealTimeWaitForNextSampClkWaitMode = 0;
decltype(&__536f7309_DAQmxResetRealTimeWriteRecoveryMode) DAQmxResetRealTimeWriteRecoveryMode = 0;
decltype(&__6d457e75_DAQmxResetRefClkRate) DAQmxResetRefClkRate = 0;
decltype(&__6c566910_DAQmxResetRefClkSrc) DAQmxResetRefClkSrc = 0;
decltype(&__320f186e_DAQmxResetRefTrigAutoTrigEnable) DAQmxResetRefTrigAutoTrigEnable = 0;
decltype(&__390e717e_DAQmxResetRefTrigDelay) DAQmxResetRefTrigDelay = 0;
decltype(&__4468585c_DAQmxResetRefTrigMaxNumTrigsToDetect) DAQmxResetRefTrigMaxNumTrigsToDetect = 0;
decltype(&__13151160_DAQmxResetRefTrigPretrigSamples) DAQmxResetRefTrigPretrigSamples = 0;
decltype(&__217a4168_DAQmxResetRefTrigRetriggerWin) DAQmxResetRefTrigRetriggerWin = 0;
decltype(&__231f7763_DAQmxResetRefTrigRetriggerable) DAQmxResetRefTrigRetriggerable = 0;
decltype(&__31062015_DAQmxResetRefTrigTimestampEnable) DAQmxResetRefTrigTimestampEnable = 0;
decltype(&__5c6d4b14_DAQmxResetRefTrigTimestampTimescale) DAQmxResetRefTrigTimestampTimescale = 0;
decltype(&__3f300f15_DAQmxResetRefTrigTrigWin) DAQmxResetRefTrigTrigWin = 0;
decltype(&__3d676d62_DAQmxResetRefTrigType) DAQmxResetRefTrigType = 0;
decltype(&__52474722_DAQmxResetSampClkActiveEdge) DAQmxResetSampClkActiveEdge = 0;
decltype(&__30345864_DAQmxResetSampClkDigFltrEnable) DAQmxResetSampClkDigFltrEnable = 0;
decltype(&__1e534047_DAQmxResetSampClkDigFltrMinPulseWidth) DAQmxResetSampClkDigFltrMinPulseWidth = 0;
decltype(&__7d565363_DAQmxResetSampClkDigFltrTimebaseRate) DAQmxResetSampClkDigFltrTimebaseRate = 0;
decltype(&__7c454406_DAQmxResetSampClkDigFltrTimebaseSrc) DAQmxResetSampClkDigFltrTimebaseSrc = 0;
decltype(&__25214275_DAQmxResetSampClkDigSyncEnable) DAQmxResetSampClkDigSyncEnable = 0;
decltype(&__1926200b_DAQmxResetSampClkOverrunBehavior) DAQmxResetSampClkOverrunBehavior = 0;
decltype(&__3a454567_DAQmxResetSampClkRate) DAQmxResetSampClkRate = 0;
decltype(&__5f445670_DAQmxResetSampClkSrc) DAQmxResetSampClkSrc = 0;
decltype(&__52714f3c_DAQmxResetSampClkTimebaseActiveEdge) DAQmxResetSampClkTimebaseActiveEdge = 0;
decltype(&__5f65457b_DAQmxResetSampClkTimebaseDiv) DAQmxResetSampClkTimebaseDiv = 0;
decltype(&__4a613f0c_DAQmxResetSampClkTimebaseMasterTimebaseDiv) DAQmxResetSampClkTimebaseMasterTimebaseDiv = 0;
decltype(&__3a734d79_DAQmxResetSampClkTimebaseRate) DAQmxResetSampClkTimebaseRate = 0;
decltype(&__5f725e6e_DAQmxResetSampClkTimebaseSrc) DAQmxResetSampClkTimebaseSrc = 0;
decltype(&__52495a0e_DAQmxResetSampClkTimingResponseMode) DAQmxResetSampClkTimingResponseMode = 0;
decltype(&__52541817_DAQmxResetSampClkUnderflowBehavior) DAQmxResetSampClkUnderflowBehavior = 0;
decltype(&__0012330e_DAQmxResetSampClkWriteWfmUseInitialWfmDT) DAQmxResetSampClkWriteWfmUseInitialWfmDT = 0;
decltype(&__5b705714_DAQmxResetSampQuantSampMode) DAQmxResetSampQuantSampMode = 0;
decltype(&__30087161_DAQmxResetSampQuantSampPerChan) DAQmxResetSampQuantSampPerChan = 0;
decltype(&__72753a18_DAQmxResetSampTimingEngine) DAQmxResetSampTimingEngine = 0;
decltype(&__0d072d14_DAQmxResetSampTimingType) DAQmxResetSampTimingType = 0;
decltype(&__2f101503_DAQmxResetStartTrigDelay) DAQmxResetStartTrigDelay = 0;
decltype(&__097e7c77_DAQmxResetStartTrigDelayUnits) DAQmxResetStartTrigDelayUnits = 0;
decltype(&__06326865_DAQmxResetStartTrigMaxNumTrigsToDetect) DAQmxResetStartTrigMaxNumTrigsToDetect = 0;
decltype(&__1f060d77_DAQmxResetStartTrigRetriggerWin) DAQmxResetStartTrigRetriggerWin = 0;
decltype(&__290d0f12_DAQmxResetStartTrigRetriggerable) DAQmxResetStartTrigRetriggerable = 0;
decltype(&__40707d7c_DAQmxResetStartTrigTimescale) DAQmxResetStartTrigTimescale = 0;
decltype(&__7e7b1d0b_DAQmxResetStartTrigTimestampEnable) DAQmxResetStartTrigTimestampEnable = 0;
decltype(&__157a7060_DAQmxResetStartTrigTimestampTimescale) DAQmxResetStartTrigTimestampTimescale = 0;
decltype(&__50707d3d_DAQmxResetStartTrigTrigWhen) DAQmxResetStartTrigTrigWhen = 0;
decltype(&__517b133d_DAQmxResetStartTrigTrigWin) DAQmxResetStartTrigTrigWin = 0;
decltype(&__330c116a_DAQmxResetStartTrigType) DAQmxResetStartTrigType = 0;
decltype(&__2e666469_DAQmxResetSwitchScanAttribute) DAQmxResetSwitchScanAttribute = 0;
decltype(&__2f4f6f6a_DAQmxResetSwitchScanBreakMode) DAQmxResetSwitchScanBreakMode = 0;
decltype(&__34045865_DAQmxResetSwitchScanRepeatMode) DAQmxResetSwitchScanRepeatMode = 0;
decltype(&__553f3c1e_DAQmxResetSyncClkInterval) DAQmxResetSyncClkInterval = 0;
decltype(&__435f1b0f_DAQmxResetSyncPulseMinDelayToStart) DAQmxResetSyncPulseMinDelayToStart = 0;
decltype(&__03615b55_DAQmxResetSyncPulseResetDelay) DAQmxResetSyncPulseResetDelay = 0;
decltype(&__29145241_DAQmxResetSyncPulseSrc) DAQmxResetSyncPulseSrc = 0;
decltype(&__38163e04_DAQmxResetSyncPulseTimeTimescale) DAQmxResetSyncPulseTimeTimescale = 0;
decltype(&__5a7f5911_DAQmxResetSyncPulseTimeWhen) DAQmxResetSyncPulseTimeWhen = 0;
decltype(&__22073746_DAQmxResetSyncPulseType) DAQmxResetSyncPulseType = 0;
decltype(&__79180f16_DAQmxResetTimingAttribute) DAQmxResetTimingAttribute = 0;
decltype(&__795d7716_DAQmxResetTimingAttributeEx) DAQmxResetTimingAttributeEx = 0;
decltype(&__31062d7a_DAQmxResetTrigAttribute) DAQmxResetTrigAttribute = 0;
decltype(&__44070717_DAQmxResetTriggerSyncType) DAQmxResetTriggerSyncType = 0;
decltype(&__012d4e5c_DAQmxResetWatchdogAOExpirState) DAQmxResetWatchdogAOExpirState = 0;
decltype(&__08076a5d_DAQmxResetWatchdogAOOutputType) DAQmxResetWatchdogAOOutputType = 0;
decltype(&__4365460d_DAQmxResetWatchdogAttribute) DAQmxResetWatchdogAttribute = 0;
decltype(&__012d4c5c_DAQmxResetWatchdogCOExpirState) DAQmxResetWatchdogCOExpirState = 0;
decltype(&__012d4b5c_DAQmxResetWatchdogDOExpirState) DAQmxResetWatchdogDOExpirState = 0;
decltype(&__242b362e_DAQmxResetWatchdogExpirTrigTrigOnNetworkConnLoss) DAQmxResetWatchdogExpirTrigTrigOnNetworkConnLoss = 0;
decltype(&__39133e63_DAQmxResetWatchdogExpirTrigType) DAQmxResetWatchdogExpirTrigType = 0;
decltype(&__5b063007_DAQmxResetWatchdogTimeout) DAQmxResetWatchdogTimeout = 0;
decltype(&__26120021_DAQmxResetWriteAttribute) DAQmxResetWriteAttribute = 0;
decltype(&__22757546_DAQmxResetWriteOffset) DAQmxResetWriteOffset = 0;
decltype(&__181b0735_DAQmxResetWriteRegenMode) DAQmxResetWriteRegenMode = 0;
decltype(&__5309021e_DAQmxResetWriteRelativeTo) DAQmxResetWriteRelativeTo = 0;
decltype(&__081f0e2a_DAQmxResetWriteSleepTime) DAQmxResetWriteSleepTime = 0;
decltype(&__3e1e1776_DAQmxResetWriteWaitMode) DAQmxResetWriteWaitMode = 0;
decltype(&__4e7a7623_DAQmxRestoreLastExtCalConst) DAQmxRestoreLastExtCalConst = 0;
decltype(&__610b1c1b_DAQmxSCBaseboardCalAdjust) DAQmxSCBaseboardCalAdjust = 0;
decltype(&__1d707e49_DAQmxSSeriesCalAdjust) DAQmxSSeriesCalAdjust = 0;
decltype(&__53555e37_DAQmxSaveGlobalChan) DAQmxSaveGlobalChan = 0;
decltype(&__3524537a_DAQmxSaveScale) DAQmxSaveScale = 0;
decltype(&__32465168_DAQmxSaveTask) DAQmxSaveTask = 0;
decltype(&__5a51556d_DAQmxSelfCal) DAQmxSelfCal = 0;
decltype(&__47615104_DAQmxSelfTestDevice) DAQmxSelfTestDevice = 0;
decltype(&__2e052d0c_DAQmxSendSoftwareTrigger) DAQmxSendSoftwareTrigger = 0;
decltype(&__3d656456_DAQmxSetAIACExcitFreq) DAQmxSetAIACExcitFreq = 0;
decltype(&__4d59643c_DAQmxSetAIACExcitSyncEnable) DAQmxSetAIACExcitSyncEnable = 0;
decltype(&__4c391025_DAQmxSetAIACExcitWireMode) DAQmxSetAIACExcitWireMode = 0;
decltype(&__21576b70_DAQmxSetAIADCCustomTimingMode) DAQmxSetAIADCCustomTimingMode = 0;
decltype(&__38051e7d_DAQmxSetAIADCTimingMode) DAQmxSetAIADCTimingMode = 0;
decltype(&__0133395d_DAQmxSetAIAccel4WireDCVoltageSensitivity) DAQmxSetAIAccel4WireDCVoltageSensitivity = 0;
decltype(&__275d5029_DAQmxSetAIAccel4WireDCVoltageSensitivityUnits) DAQmxSetAIAccel4WireDCVoltageSensitivityUnits = 0;
decltype(&__160c0e20_DAQmxSetAIAccelChargeSensitivity) DAQmxSetAIAccelChargeSensitivity = 0;
decltype(&__30626754_DAQmxSetAIAccelChargeSensitivityUnits) DAQmxSetAIAccelChargeSensitivityUnits = 0;
decltype(&__7b401c29_DAQmxSetAIAccelSensitivity) DAQmxSetAIAccelSensitivity = 0;
decltype(&__12343a47_DAQmxSetAIAccelSensitivityUnits) DAQmxSetAIAccelSensitivityUnits = 0;
decltype(&__70576d5c_DAQmxSetAIAccelUnits) DAQmxSetAIAccelUnits = 0;
decltype(&__5c6c7c78_DAQmxSetAIAcceldBRef) DAQmxSetAIAcceldBRef = 0;
decltype(&__093e1b6d_DAQmxSetAIAtten) DAQmxSetAIAtten = 0;
decltype(&__1f3e6266_DAQmxSetAIAutoZeroMode) DAQmxSetAIAutoZeroMode = 0;
decltype(&__624c2036_DAQmxSetAIAveragingWinSize) DAQmxSetAIAveragingWinSize = 0;
decltype(&__320c052b_DAQmxSetAIBridgeBalanceCoarsePot) DAQmxSetAIBridgeBalanceCoarsePot = 0;
decltype(&__3e277d79_DAQmxSetAIBridgeBalanceFinePot) DAQmxSetAIBridgeBalanceFinePot = 0;
decltype(&__5759760e_DAQmxSetAIBridgeCfg) DAQmxSetAIBridgeCfg = 0;
decltype(&__2d393b60_DAQmxSetAIBridgeElectricalUnits) DAQmxSetAIBridgeElectricalUnits = 0;
decltype(&__55447d47_DAQmxSetAIBridgeInitialRatio) DAQmxSetAIBridgeInitialRatio = 0;
decltype(&__3c39604d_DAQmxSetAIBridgeInitialVoltage) DAQmxSetAIBridgeInitialVoltage = 0;
decltype(&__2e427b4c_DAQmxSetAIBridgeNomResistance) DAQmxSetAIBridgeNomResistance = 0;
decltype(&__0b5a6065_DAQmxSetAIBridgePhysicalUnits) DAQmxSetAIBridgePhysicalUnits = 0;
decltype(&__0c280d25_DAQmxSetAIBridgePolyForwardCoeff) DAQmxSetAIBridgePolyForwardCoeff = 0;
decltype(&__0b230837_DAQmxSetAIBridgePolyReverseCoeff) DAQmxSetAIBridgePolyReverseCoeff = 0;
decltype(&__47080912_DAQmxSetAIBridgeScaleType) DAQmxSetAIBridgeScaleType = 0;
decltype(&__1a1f646e_DAQmxSetAIBridgeShuntCalEnable) DAQmxSetAIBridgeShuntCalEnable = 0;
decltype(&__46650617_DAQmxSetAIBridgeShuntCalGainAdjust) DAQmxSetAIBridgeShuntCalGainAdjust = 0;
decltype(&__03056969_DAQmxSetAIBridgeShuntCalSelect) DAQmxSetAIBridgeShuntCalSelect = 0;
decltype(&__540d1938_DAQmxSetAIBridgeShuntCalShuntCalAActualResistance) DAQmxSetAIBridgeShuntCalShuntCalAActualResistance = 0;
decltype(&__527d651c_DAQmxSetAIBridgeShuntCalShuntCalAResistance) DAQmxSetAIBridgeShuntCalShuntCalAResistance = 0;
decltype(&__556c636d_DAQmxSetAIBridgeShuntCalShuntCalASrc) DAQmxSetAIBridgeShuntCalShuntCalASrc = 0;
decltype(&__570d1938_DAQmxSetAIBridgeShuntCalShuntCalBActualResistance) DAQmxSetAIBridgeShuntCalShuntCalBActualResistance = 0;
decltype(&__517d651c_DAQmxSetAIBridgeShuntCalShuntCalBResistance) DAQmxSetAIBridgeShuntCalShuntCalBResistance = 0;
decltype(&__44627238_DAQmxSetAIBridgeTableElectricalVals) DAQmxSetAIBridgeTableElectricalVals = 0;
decltype(&__49311916_DAQmxSetAIBridgeTablePhysicalVals) DAQmxSetAIBridgeTablePhysicalVals = 0;
decltype(&__36485779_DAQmxSetAIBridgeTwoPointLinFirstElectricalVal) DAQmxSetAIBridgeTwoPointLinFirstElectricalVal = 0;
decltype(&__65507907_DAQmxSetAIBridgeTwoPointLinFirstPhysicalVal) DAQmxSetAIBridgeTwoPointLinFirstPhysicalVal = 0;
decltype(&__0317634d_DAQmxSetAIBridgeTwoPointLinSecondElectricalVal) DAQmxSetAIBridgeTwoPointLinSecondElectricalVal = 0;
decltype(&__7d447b63_DAQmxSetAIBridgeTwoPointLinSecondPhysicalVal) DAQmxSetAIBridgeTwoPointLinSecondPhysicalVal = 0;
decltype(&__3251787a_DAQmxSetAIBridgeUnits) DAQmxSetAIBridgeUnits = 0;
decltype(&__08346144_DAQmxSetAIChanCalApplyCalIfExp) DAQmxSetAIChanCalApplyCalIfExp = 0;
decltype(&__34172119_DAQmxSetAIChanCalCalDate) DAQmxSetAIChanCalCalDate = 0;
decltype(&__13715163_DAQmxSetAIChanCalDesc) DAQmxSetAIChanCalDesc = 0;
decltype(&__73703f32_DAQmxSetAIChanCalEnableCal) DAQmxSetAIChanCalEnableCal = 0;
decltype(&__34113805_DAQmxSetAIChanCalExpDate) DAQmxSetAIChanCalExpDate = 0;
decltype(&__15555177_DAQmxSetAIChanCalOperatorName) DAQmxSetAIChanCalOperatorName = 0;
decltype(&__5b2d230c_DAQmxSetAIChanCalPolyForwardCoeff) DAQmxSetAIChanCalPolyForwardCoeff = 0;
decltype(&__492a2809_DAQmxSetAIChanCalPolyReverseCoeff) DAQmxSetAIChanCalPolyReverseCoeff = 0;
decltype(&__6c660308_DAQmxSetAIChanCalScaleType) DAQmxSetAIChanCalScaleType = 0;
decltype(&__745e7137_DAQmxSetAIChanCalTablePreScaledVals) DAQmxSetAIChanCalTablePreScaledVals = 0;
decltype(&__2b090f06_DAQmxSetAIChanCalTableScaledVals) DAQmxSetAIChanCalTableScaledVals = 0;
decltype(&__1b53716d_DAQmxSetAIChanCalVerifAcqVals) DAQmxSetAIChanCalVerifAcqVals = 0;
decltype(&__0c53626b_DAQmxSetAIChanCalVerifRefVals) DAQmxSetAIChanCalVerifRefVals = 0;
decltype(&__3a477960_DAQmxSetAIChargeUnits) DAQmxSetAIChargeUnits = 0;
decltype(&__73495e7a_DAQmxSetAIChopEnable) DAQmxSetAIChopEnable = 0;
decltype(&__22202715_DAQmxSetAIConvActiveEdge) DAQmxSetAIConvActiveEdge = 0;
decltype(&__67582715_DAQmxSetAIConvActiveEdgeEx) DAQmxSetAIConvActiveEdgeEx = 0;
decltype(&__6442540a_DAQmxSetAIConvDigFltrEnable) DAQmxSetAIConvDigFltrEnable = 0;
decltype(&__1c42544f_DAQmxSetAIConvDigFltrEnableEx) DAQmxSetAIConvDigFltrEnableEx = 0;
decltype(&__476c3312_DAQmxSetAIConvDigFltrMinPulseWidth) DAQmxSetAIConvDigFltrMinPulseWidth = 0;
decltype(&__476c766a_DAQmxSetAIConvDigFltrMinPulseWidthEx) DAQmxSetAIConvDigFltrMinPulseWidthEx = 0;
decltype(&__630f3601_DAQmxSetAIConvDigFltrTimebaseRate) DAQmxSetAIConvDigFltrTimebaseRate = 0;
decltype(&__634a4e01_DAQmxSetAIConvDigFltrTimebaseRateEx) DAQmxSetAIConvDigFltrTimebaseRateEx = 0;
decltype(&__060e2516_DAQmxSetAIConvDigFltrTimebaseSrc) DAQmxSetAIConvDigFltrTimebaseSrc = 0;
decltype(&__43762516_DAQmxSetAIConvDigFltrTimebaseSrcEx) DAQmxSetAIConvDigFltrTimebaseSrcEx = 0;
decltype(&__75574110_DAQmxSetAIConvDigSyncEnable) DAQmxSetAIConvDigSyncEnable = 0;
decltype(&__0d574155_DAQmxSetAIConvDigSyncEnableEx) DAQmxSetAIConvDigSyncEnableEx = 0;
decltype(&__67482517_DAQmxSetAIConvRate) DAQmxSetAIConvRate = 0;
decltype(&__6748606f_DAQmxSetAIConvRateEx) DAQmxSetAIConvRateEx = 0;
decltype(&__702d2404_DAQmxSetAIConvSrc) DAQmxSetAIConvSrc = 0;
decltype(&__70685c04_DAQmxSetAIConvSrcEx) DAQmxSetAIConvSrcEx = 0;
decltype(&__7b2d0517_DAQmxSetAIConvTimebaseDiv) DAQmxSetAIConvTimebaseDiv = 0;
decltype(&__7b687d17_DAQmxSetAIConvTimebaseDivEx) DAQmxSetAIConvTimebaseDivEx = 0;
decltype(&__6e2d120c_DAQmxSetAIConvTimebaseSrc) DAQmxSetAIConvTimebaseSrc = 0;
decltype(&__6e686a0c_DAQmxSetAIConvTimebaseSrcEx) DAQmxSetAIConvTimebaseSrcEx = 0;
decltype(&__664c1b1f_DAQmxSetAICoupling) DAQmxSetAICoupling = 0;
decltype(&__5f4f773e_DAQmxSetAICurrentACRMSUnits) DAQmxSetAICurrentACRMSUnits = 0;
decltype(&__760e3618_DAQmxSetAICurrentShuntLoc) DAQmxSetAICurrentShuntLoc = 0;
decltype(&__07093803_DAQmxSetAICurrentShuntResistance) DAQmxSetAICurrentShuntResistance = 0;
decltype(&__0f0f7c6b_DAQmxSetAICurrentUnits) DAQmxSetAICurrentUnits = 0;
decltype(&__5d021800_DAQmxSetAICustomScaleName) DAQmxSetAICustomScaleName = 0;
decltype(&__57491629_DAQmxSetAIDCOffset) DAQmxSetAIDCOffset = 0;
decltype(&__5c3c190f_DAQmxSetAIDataXferCustomThreshold) DAQmxSetAIDataXferCustomThreshold = 0;
decltype(&__711a040b_DAQmxSetAIDataXferMaxRate) DAQmxSetAIDataXferMaxRate = 0;
decltype(&__0f20657b_DAQmxSetAIDataXferMech) DAQmxSetAIDataXferMech = 0;
decltype(&__790b1515_DAQmxSetAIDataXferReqCond) DAQmxSetAIDataXferReqCond = 0;
decltype(&__796c7a31_DAQmxSetAIDigFltrBandpassCenterFreq) DAQmxSetAIDigFltrBandpassCenterFreq = 0;
decltype(&__0b10757d_DAQmxSetAIDigFltrBandpassWidth) DAQmxSetAIDigFltrBandpassWidth = 0;
decltype(&__0e387361_DAQmxSetAIDigFltrCoeff) DAQmxSetAIDigFltrCoeff = 0;
decltype(&__0a341765_DAQmxSetAIDigFltrEnable) DAQmxSetAIDigFltrEnable = 0;
decltype(&__6e657622_DAQmxSetAIDigFltrHighpassCutoffFreq) DAQmxSetAIDigFltrHighpassCutoffFreq = 0;
decltype(&__5852321d_DAQmxSetAIDigFltrLowpassCutoffFreq) DAQmxSetAIDigFltrLowpassCutoffFreq = 0;
decltype(&__233d3016_DAQmxSetAIDigFltrNotchCenterFreq) DAQmxSetAIDigFltrNotchCenterFreq = 0;
decltype(&__6f4f4c19_DAQmxSetAIDigFltrNotchWidth) DAQmxSetAIDigFltrNotchWidth = 0;
decltype(&__0d206e60_DAQmxSetAIDigFltrOrder) DAQmxSetAIDigFltrOrder = 0;
decltype(&__7d201704_DAQmxSetAIDigFltrResponse) DAQmxSetAIDigFltrResponse = 0;
decltype(&__0d496574_DAQmxSetAIDigFltrType) DAQmxSetAIDigFltrType = 0;
decltype(&__20387460_DAQmxSetAIDitherEnable) DAQmxSetAIDitherEnable = 0;
decltype(&__7b300403_DAQmxSetAIEddyCurrentProxProbeSensitivity) DAQmxSetAIEddyCurrentProxProbeSensitivity = 0;
decltype(&__0f166a6a_DAQmxSetAIEddyCurrentProxProbeSensitivityUnits) DAQmxSetAIEddyCurrentProxProbeSensitivityUnits = 0;
decltype(&__6c417108_DAQmxSetAIEddyCurrentProxProbeUnits) DAQmxSetAIEddyCurrentProxProbeUnits = 0;
decltype(&__21374645_DAQmxSetAIEnhancedAliasRejectionEnable) DAQmxSetAIEnhancedAliasRejectionEnable = 0;
decltype(&__1110112d_DAQmxSetAIExcitActualVal) DAQmxSetAIExcitActualVal = 0;
decltype(&__1e5d7764_DAQmxSetAIExcitDCorAC) DAQmxSetAIExcitDCorAC = 0;
decltype(&__681e1c15_DAQmxSetAIExcitIdleOutputBehavior) DAQmxSetAIExcitIdleOutputBehavior = 0;
decltype(&__7b5c7657_DAQmxSetAIExcitSense) DAQmxSetAIExcitSense = 0;
decltype(&__6c510532_DAQmxSetAIExcitSrc) DAQmxSetAIExcitSrc = 0;
decltype(&__736d4e5d_DAQmxSetAIExcitUseForScaling) DAQmxSetAIExcitUseForScaling = 0;
decltype(&__09465954_DAQmxSetAIExcitUseMultiplexed) DAQmxSetAIExcitUseMultiplexed = 0;
decltype(&__7f5e0537_DAQmxSetAIExcitVal) DAQmxSetAIExcitVal = 0;
decltype(&__30203856_DAQmxSetAIExcitVoltageOrCurrent) DAQmxSetAIExcitVoltageOrCurrent = 0;
decltype(&__3c161f64_DAQmxSetAIFilterDelayAdjustment) DAQmxSetAIFilterDelayAdjustment = 0;
decltype(&__586c150a_DAQmxSetAIFilterDelayUnits) DAQmxSetAIFilterDelayUnits = 0;
decltype(&__38247660_DAQmxSetAIFilterEnable) DAQmxSetAIFilterEnable = 0;
decltype(&__575d7273_DAQmxSetAIFilterFreq) DAQmxSetAIFilterFreq = 0;
decltype(&__2c5d7367_DAQmxSetAIFilterOrder) DAQmxSetAIFilterOrder = 0;
decltype(&__2c241717_DAQmxSetAIFilterResponse) DAQmxSetAIFilterResponse = 0;
decltype(&__5855447d_DAQmxSetAIForceIEPESensorSensitivity) DAQmxSetAIForceIEPESensorSensitivity = 0;
decltype(&__7e3b2d09_DAQmxSetAIForceIEPESensorSensitivityUnits) DAQmxSetAIForceIEPESensorSensitivityUnits = 0;
decltype(&__70577021_DAQmxSetAIForceReadFromChan) DAQmxSetAIForceReadFromChan = 0;
decltype(&__61516350_DAQmxSetAIForceUnits) DAQmxSetAIForceUnits = 0;
decltype(&__6b5e3a12_DAQmxSetAIFreqHyst) DAQmxSetAIFreqHyst = 0;
decltype(&__5d475c1f_DAQmxSetAIFreqThreshVoltage) DAQmxSetAIFreqThreshVoltage = 0;
decltype(&__715e5405_DAQmxSetAIFreqUnits) DAQmxSetAIFreqUnits = 0;
decltype(&__14357378_DAQmxSetAIGain) DAQmxSetAIGain = 0;
decltype(&__635d7c15_DAQmxSetAIImpedance) DAQmxSetAIImpedance = 0;
decltype(&__261f4749_DAQmxSetAIInputLimitsFaultDetectEnable) DAQmxSetAIInputLimitsFaultDetectEnable = 0;
decltype(&__58433823_DAQmxSetAIInputLimitsFaultDetectLowerLimit) DAQmxSetAIInputLimitsFaultDetectLowerLimit = 0;
decltype(&__415c3f23_DAQmxSetAIInputLimitsFaultDetectUpperLimit) DAQmxSetAIInputLimitsFaultDetectUpperLimit = 0;
decltype(&__7f4d0924_DAQmxSetAIInputSrc) DAQmxSetAIInputSrc = 0;
decltype(&__470a2b2a_DAQmxSetAILVDTSensitivity) DAQmxSetAILVDTSensitivity = 0;
decltype(&__332c4543_DAQmxSetAILVDTSensitivityUnits) DAQmxSetAILVDTSensitivityUnits = 0;
decltype(&__507b5e21_DAQmxSetAILVDTUnits) DAQmxSetAILVDTUnits = 0;
decltype(&__7c5d6d61_DAQmxSetAILeadWireResistance) DAQmxSetAILeadWireResistance = 0;
decltype(&__57465e20_DAQmxSetAILossyLSBRemovalCompressedSampSize) DAQmxSetAILossyLSBRemovalCompressedSampSize = 0;
decltype(&__646b7b37_DAQmxSetAILowpassCutoffFreq) DAQmxSetAILowpassCutoffFreq = 0;
decltype(&__1b021264_DAQmxSetAILowpassEnable) DAQmxSetAILowpassEnable = 0;
decltype(&__07383720_DAQmxSetAILowpassSwitchCapClkSrc) DAQmxSetAILowpassSwitchCapClkSrc = 0;
decltype(&__5c41593c_DAQmxSetAILowpassSwitchCapExtClkDiv) DAQmxSetAILowpassSwitchCapExtClkDiv = 0;
decltype(&__5e5a4a4d_DAQmxSetAILowpassSwitchCapExtClkFreq) DAQmxSetAILowpassSwitchCapExtClkFreq = 0;
decltype(&__5c415331_DAQmxSetAILowpassSwitchCapOutClkDiv) DAQmxSetAILowpassSwitchCapOutClkDiv = 0;
decltype(&__055b7978_DAQmxSetAIMax) DAQmxSetAIMax = 0;
decltype(&__391d796e_DAQmxSetAIMemMapEnable) DAQmxSetAIMemMapEnable = 0;
decltype(&__25230660_DAQmxSetAIMicrophoneSensitivity) DAQmxSetAIMicrophoneSensitivity = 0;
decltype(&__135b7970_DAQmxSetAIMin) DAQmxSetAIMin = 0;
decltype(&__24357e72_DAQmxSetAIOpenChanDetectEnable) DAQmxSetAIOpenChanDetectEnable = 0;
decltype(&__681e3f2c_DAQmxSetAIOpenThrmcplDetectEnable) DAQmxSetAIOpenThrmcplDetectEnable = 0;
decltype(&__701d0e29_DAQmxSetAIOvercurrentDetectEnable) DAQmxSetAIOvercurrentDetectEnable = 0;
decltype(&__3712414f_DAQmxSetAIPowerSupplyFaultDetectEnable) DAQmxSetAIPowerSupplyFaultDetectEnable = 0;
decltype(&__03393170_DAQmxSetAIPressureUnits) DAQmxSetAIPressureUnits = 0;
decltype(&__664d6444_DAQmxSetAIProbeAtten) DAQmxSetAIProbeAtten = 0;
decltype(&__391a664d_DAQmxSetAIRTDA) DAQmxSetAIRTDA = 0;
decltype(&__3919664d_DAQmxSetAIRTDB) DAQmxSetAIRTDB = 0;
decltype(&__3918664d_DAQmxSetAIRTDC) DAQmxSetAIRTDC = 0;
decltype(&__3909564d_DAQmxSetAIRTDR0) DAQmxSetAIRTDR0 = 0;
decltype(&__5c0f1f3d_DAQmxSetAIRTDType) DAQmxSetAIRTDType = 0;
decltype(&__470a352a_DAQmxSetAIRVDTSensitivity) DAQmxSetAIRVDTSensitivity = 0;
decltype(&__332c5b43_DAQmxSetAIRVDTSensitivityUnits) DAQmxSetAIRVDTSensitivityUnits = 0;
decltype(&__507b4021_DAQmxSetAIRVDTUnits) DAQmxSetAIRVDTUnits = 0;
decltype(&__3c3e1219_DAQmxSetAIRawDataCompressionType) DAQmxSetAIRawDataCompressionType = 0;
decltype(&__5f4e4108_DAQmxSetAIRemoveFilterDelay) DAQmxSetAIRemoveFilterDelay = 0;
decltype(&__2c3a116d_DAQmxSetAIResistanceCfg) DAQmxSetAIResistanceCfg = 0;
decltype(&__49321f19_DAQmxSetAIResistanceUnits) DAQmxSetAIResistanceUnits = 0;
decltype(&__72130f10_DAQmxSetAIRngHigh) DAQmxSetAIRngHigh = 0;
decltype(&__1a170900_DAQmxSetAIRngLow) DAQmxSetAIRngLow = 0;
decltype(&__02057c7f_DAQmxSetAIRosetteStrainGageOrientation) DAQmxSetAIRosetteStrainGageOrientation = 0;
decltype(&__6163110d_DAQmxSetAIRosetteStrainGageRosetteMeasType) DAQmxSetAIRosetteStrainGageRosetteMeasType = 0;
decltype(&__724a421b_DAQmxSetAISampAndHoldEnable) DAQmxSetAISampAndHoldEnable = 0;
decltype(&__3104190c_DAQmxSetAISensorPowerCfg) DAQmxSetAISensorPowerCfg = 0;
decltype(&__5413061b_DAQmxSetAISensorPowerType) DAQmxSetAISensorPowerType = 0;
decltype(&__45707762_DAQmxSetAISensorPowerVoltage) DAQmxSetAISensorPowerVoltage = 0;
decltype(&__6a4f3418_DAQmxSetAISoundPressureMaxSoundPressureLvl) DAQmxSetAISoundPressureMaxSoundPressureLvl = 0;
decltype(&__614b6173_DAQmxSetAISoundPressureUnits) DAQmxSetAISoundPressureUnits = 0;
decltype(&__4d707057_DAQmxSetAISoundPressuredBRef) DAQmxSetAISoundPressuredBRef = 0;
decltype(&__0b3d0e66_DAQmxSetAIStrainGageCfg) DAQmxSetAIStrainGageCfg = 0;
decltype(&__0c63646a_DAQmxSetAIStrainGageForceReadFromChan) DAQmxSetAIStrainGageForceReadFromChan = 0;
decltype(&__26296d77_DAQmxSetAIStrainGageGageFactor) DAQmxSetAIStrainGageGageFactor = 0;
decltype(&__0a2f0728_DAQmxSetAIStrainGagePoissonRatio) DAQmxSetAIStrainGagePoissonRatio = 0;
decltype(&__29546777_DAQmxSetAIStrainUnits) DAQmxSetAIStrainUnits = 0;
decltype(&__795f4612_DAQmxSetAITempUnits) DAQmxSetAITempUnits = 0;
decltype(&__6836231a_DAQmxSetAITermCfg) DAQmxSetAITermCfg = 0;
decltype(&__35142542_DAQmxSetAIThrmcplCJCVal) DAQmxSetAIThrmcplCJCVal = 0;
decltype(&__7c590031_DAQmxSetAIThrmcplLeadOffsetVoltage) DAQmxSetAIThrmcplLeadOffsetVoltage = 0;
decltype(&__7f653419_DAQmxSetAIThrmcplScaleType) DAQmxSetAIThrmcplScaleType = 0;
decltype(&__06627a71_DAQmxSetAIThrmcplType) DAQmxSetAIThrmcplType = 0;
decltype(&__7d771305_DAQmxSetAIThrmstrA) DAQmxSetAIThrmstrA = 0;
decltype(&__7d741305_DAQmxSetAIThrmstrB) DAQmxSetAIThrmstrB = 0;
decltype(&__7d751305_DAQmxSetAIThrmstrC) DAQmxSetAIThrmstrC = 0;
decltype(&__7d642205_DAQmxSetAIThrmstrR1) DAQmxSetAIThrmstrR1 = 0;
decltype(&__29447c67_DAQmxSetAITorqueUnits) DAQmxSetAITorqueUnits = 0;
decltype(&__5a3e1710_DAQmxSetAIUsbXferReqCount) DAQmxSetAIUsbXferReqCount = 0;
decltype(&__3e38181b_DAQmxSetAIUsbXferReqSize) DAQmxSetAIUsbXferReqSize = 0;
decltype(&__081e0a42_DAQmxSetAIVelocityIEPESensorSensitivity) DAQmxSetAIVelocityIEPESensorSensitivity = 0;
decltype(&__66777e64_DAQmxSetAIVelocityIEPESensorSensitivityUnits) DAQmxSetAIVelocityIEPESensorSensitivityUnits = 0;
decltype(&__59392622_DAQmxSetAIVelocityIEPESensordBRef) DAQmxSetAIVelocityIEPESensordBRef = 0;
decltype(&__0c39277b_DAQmxSetAIVelocityUnits) DAQmxSetAIVelocityUnits = 0;
decltype(&__5049662d_DAQmxSetAIVoltageACRMSUnits) DAQmxSetAIVoltageACRMSUnits = 0;
decltype(&__00096d78_DAQmxSetAIVoltageUnits) DAQmxSetAIVoltageUnits = 0;
decltype(&__112d4143_DAQmxSetAIVoltagedBRef) DAQmxSetAIVoltagedBRef = 0;
decltype(&__774d1a0e_DAQmxSetAOCommonModeOffset) DAQmxSetAOCommonModeOffset = 0;
decltype(&__0f097c6b_DAQmxSetAOCurrentUnits) DAQmxSetAOCurrentUnits = 0;
decltype(&__5d041800_DAQmxSetAOCustomScaleName) DAQmxSetAOCustomScaleName = 0;
decltype(&__56033109_DAQmxSetAODACOffsetExtSrc) DAQmxSetAODACOffsetExtSrc = 0;
decltype(&__3f14626d_DAQmxSetAODACOffsetSrc) DAQmxSetAODACOffsetSrc = 0;
decltype(&__2c1b6268_DAQmxSetAODACOffsetVal) DAQmxSetAODACOffsetVal = 0;
decltype(&__08107978_DAQmxSetAODACRefAllowConnToGnd) DAQmxSetAODACRefAllowConnToGnd = 0;
decltype(&__4d0f3c3e_DAQmxSetAODACRefConnToGnd) DAQmxSetAODACRefConnToGnd = 0;
decltype(&__0914616d_DAQmxSetAODACRefExtSrc) DAQmxSetAODACRefExtSrc = 0;
decltype(&__6d7d763e_DAQmxSetAODACRefSrc) DAQmxSetAODACRefSrc = 0;
decltype(&__686e793e_DAQmxSetAODACRefVal) DAQmxSetAODACRefVal = 0;
decltype(&__76667957_DAQmxSetAODACRngHigh) DAQmxSetAODACRngHigh = 0;
decltype(&__7260693f_DAQmxSetAODACRngLow) DAQmxSetAODACRngLow = 0;
decltype(&__0f26657b_DAQmxSetAODataXferMech) DAQmxSetAODataXferMech = 0;
decltype(&__790d1515_DAQmxSetAODataXferReqCond) DAQmxSetAODataXferReqCond = 0;
decltype(&__29375844_DAQmxSetAOEnhancedImageRejectionEnable) DAQmxSetAOEnhancedImageRejectionEnable = 0;
decltype(&__2c4c7b63_DAQmxSetAOFilterDelay) DAQmxSetAOFilterDelay = 0;
decltype(&__3c101f64_DAQmxSetAOFilterDelayAdjustment) DAQmxSetAOFilterDelayAdjustment = 0;
decltype(&__586a150a_DAQmxSetAOFilterDelayUnits) DAQmxSetAOFilterDelayUnits = 0;
decltype(&__75732c0c_DAQmxSetAOFuncGenAmplitude) DAQmxSetAOFuncGenAmplitude = 0;
decltype(&__6c677e42_DAQmxSetAOFuncGenFMDeviation) DAQmxSetAOFuncGenFMDeviation = 0;
decltype(&__0c78476c_DAQmxSetAOFuncGenFreq) DAQmxSetAOFuncGenFreq = 0;
decltype(&__1800304d_DAQmxSetAOFuncGenModulationType) DAQmxSetAOFuncGenModulationType = 0;
decltype(&__0e14276f_DAQmxSetAOFuncGenOffset) DAQmxSetAOFuncGenOffset = 0;
decltype(&__1008341e_DAQmxSetAOFuncGenSquareDutyCycle) DAQmxSetAOFuncGenSquareDutyCycle = 0;
decltype(&__6e6a7400_DAQmxSetAOFuncGenStartPhase) DAQmxSetAOFuncGenStartPhase = 0;
decltype(&__186a4c79_DAQmxSetAOFuncGenType) DAQmxSetAOFuncGenType = 0;
decltype(&__14337378_DAQmxSetAOGain) DAQmxSetAOGain = 0;
decltype(&__5144406f_DAQmxSetAOIdleOutputBehavior) DAQmxSetAOIdleOutputBehavior = 0;
decltype(&__023f307a_DAQmxSetAOLoadImpedance) DAQmxSetAOLoadImpedance = 0;
decltype(&__055d7978_DAQmxSetAOMax) DAQmxSetAOMax = 0;
decltype(&__391b796e_DAQmxSetAOMemMapEnable) DAQmxSetAOMemMapEnable = 0;
decltype(&__135d7970_DAQmxSetAOMin) DAQmxSetAOMin = 0;
decltype(&__4121101e_DAQmxSetAOOutputImpedance) DAQmxSetAOOutputImpedance = 0;
decltype(&__080f305b_DAQmxSetAOPowerAmpChannelEnable) DAQmxSetAOPowerAmpChannelEnable = 0;
decltype(&__183b2603_DAQmxSetAOReglitchEnable) DAQmxSetAOReglitchEnable = 0;
decltype(&__5c350c13_DAQmxSetAOResolutionUnits) DAQmxSetAOResolutionUnits = 0;
decltype(&__6830231a_DAQmxSetAOTermCfg) DAQmxSetAOTermCfg = 0;
decltype(&__5a381710_DAQmxSetAOUsbXferReqCount) DAQmxSetAOUsbXferReqCount = 0;
decltype(&__3e3e181b_DAQmxSetAOUsbXferReqSize) DAQmxSetAOUsbXferReqSize = 0;
decltype(&__7e392c21_DAQmxSetAOUseOnlyOnBrdMem) DAQmxSetAOUseOnlyOnBrdMem = 0;
decltype(&__3e66757e_DAQmxSetAOVoltageCurrentLimit) DAQmxSetAOVoltageCurrentLimit = 0;
decltype(&__000f6d78_DAQmxSetAOVoltageUnits) DAQmxSetAOVoltageUnits = 0;
decltype(&__766f4019_DAQmxSetAdvTrigType) DAQmxSetAdvTrigType = 0;
decltype(&__6148702e_DAQmxSetAnalogPowerUpStates) DAQmxSetAnalogPowerUpStates = 0;
decltype(&__6c1c1133_DAQmxSetAnalogPowerUpStatesWithOutputType) DAQmxSetAnalogPowerUpStatesWithOutputType = 0;
decltype(&__1e0f2960_DAQmxSetAnlgEdgeRefTrigCoupling) DAQmxSetAnlgEdgeRefTrigCoupling = 0;
decltype(&__6463516c_DAQmxSetAnlgEdgeRefTrigDigFltrEnable) DAQmxSetAnlgEdgeRefTrigDigFltrEnable = 0;
decltype(&__7c407f0b_DAQmxSetAnlgEdgeRefTrigDigFltrMinPulseWidth) DAQmxSetAnlgEdgeRefTrigDigFltrMinPulseWidth = 0;
decltype(&__6f641c0e_DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseRate) DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseRate = 0;
decltype(&__78011d1d_DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseSrc) DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseSrc = 0;
decltype(&__7e724479_DAQmxSetAnlgEdgeRefTrigDigSyncEnable) DAQmxSetAnlgEdgeRefTrigDigSyncEnable = 0;
decltype(&__61674a07_DAQmxSetAnlgEdgeRefTrigHyst) DAQmxSetAnlgEdgeRefTrigHyst = 0;
decltype(&__6e783e03_DAQmxSetAnlgEdgeRefTrigLvl) DAQmxSetAnlgEdgeRefTrigLvl = 0;
decltype(&__747b4e79_DAQmxSetAnlgEdgeRefTrigSlope) DAQmxSetAnlgEdgeRefTrigSlope = 0;
decltype(&__6a773e1c_DAQmxSetAnlgEdgeRefTrigSrc) DAQmxSetAnlgEdgeRefTrigSrc = 0;
decltype(&__6f172a1b_DAQmxSetAnlgEdgeStartTrigCoupling) DAQmxSetAnlgEdgeStartTrigCoupling = 0;
decltype(&__171b5077_DAQmxSetAnlgEdgeStartTrigDigFltrEnable) DAQmxSetAnlgEdgeStartTrigDigFltrEnable = 0;
decltype(&__397c4854_DAQmxSetAnlgEdgeStartTrigDigFltrMinPulseWidth) DAQmxSetAnlgEdgeStartTrigDigFltrMinPulseWidth = 0;
decltype(&__5a795b70_DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseRate) DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseRate = 0;
decltype(&__5b6a4c15_DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseSrc) DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseSrc = 0;
decltype(&__020e4a66_DAQmxSetAnlgEdgeStartTrigDigSyncEnable) DAQmxSetAnlgEdgeStartTrigDigSyncEnable = 0;
decltype(&__0c705573_DAQmxSetAnlgEdgeStartTrigHyst) DAQmxSetAnlgEdgeStartTrigHyst = 0;
decltype(&__78745a6c_DAQmxSetAnlgEdgeStartTrigLvl) DAQmxSetAnlgEdgeStartTrigLvl = 0;
decltype(&__080e406f_DAQmxSetAnlgEdgeStartTrigSlope) DAQmxSetAnlgEdgeStartTrigSlope = 0;
decltype(&__786b5e63_DAQmxSetAnlgEdgeStartTrigSrc) DAQmxSetAnlgEdgeStartTrigSrc = 0;
decltype(&__2b0b353b_DAQmxSetAnlgLvlPauseTrigCoupling) DAQmxSetAnlgLvlPauseTrigCoupling = 0;
decltype(&__27715943_DAQmxSetAnlgLvlPauseTrigDigFltrEnable) DAQmxSetAnlgLvlPauseTrigDigFltrEnable = 0;
decltype(&__40697a6d_DAQmxSetAnlgLvlPauseTrigDigFltrMinPulseWidth) DAQmxSetAnlgLvlPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__457a5e0e_DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseRate) DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseRate = 0;
decltype(&__566d3b0f_DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseSrc) DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__326b4856_DAQmxSetAnlgLvlPauseTrigDigSyncEnable) DAQmxSetAnlgLvlPauseTrigDigSyncEnable = 0;
decltype(&__4c745d58_DAQmxSetAnlgLvlPauseTrigHyst) DAQmxSetAnlgLvlPauseTrigHyst = 0;
decltype(&__487b422c_DAQmxSetAnlgLvlPauseTrigLvl) DAQmxSetAnlgLvlPauseTrigLvl = 0;
decltype(&__577f4d2c_DAQmxSetAnlgLvlPauseTrigSrc) DAQmxSetAnlgLvlPauseTrigSrc = 0;
decltype(&__53654b42_DAQmxSetAnlgLvlPauseTrigWhen) DAQmxSetAnlgLvlPauseTrigWhen = 0;
decltype(&__346a477b_DAQmxSetAnlgMultiEdgeRefTrigCouplings) DAQmxSetAnlgMultiEdgeRefTrigCouplings = 0;
decltype(&__53152f18_DAQmxSetAnlgMultiEdgeRefTrigHysts) DAQmxSetAnlgMultiEdgeRefTrigHysts = 0;
decltype(&__241a301f_DAQmxSetAnlgMultiEdgeRefTrigLvls) DAQmxSetAnlgMultiEdgeRefTrigLvls = 0;
decltype(&__5e73331c_DAQmxSetAnlgMultiEdgeRefTrigSlopes) DAQmxSetAnlgMultiEdgeRefTrigSlopes = 0;
decltype(&__3b1e3f1f_DAQmxSetAnlgMultiEdgeRefTrigSrcs) DAQmxSetAnlgMultiEdgeRefTrigSrcs = 0;
decltype(&__3c1b2c78_DAQmxSetAnlgMultiEdgeStartTrigCouplings) DAQmxSetAnlgMultiEdgeStartTrigCouplings = 0;
decltype(&__54784b07_DAQmxSetAnlgMultiEdgeStartTrigHysts) DAQmxSetAnlgMultiEdgeStartTrigHysts = 0;
decltype(&__4b7f3c08_DAQmxSetAnlgMultiEdgeStartTrigLvls) DAQmxSetAnlgMultiEdgeStartTrigLvls = 0;
decltype(&__487c4661_DAQmxSetAnlgMultiEdgeStartTrigSlopes) DAQmxSetAnlgMultiEdgeStartTrigSlopes = 0;
decltype(&__447f230c_DAQmxSetAnlgMultiEdgeStartTrigSrcs) DAQmxSetAnlgMultiEdgeStartTrigSrcs = 0;
decltype(&__5d66412c_DAQmxSetAnlgWinPauseTrigBtm) DAQmxSetAnlgWinPauseTrigBtm = 0;
decltype(&__3014373b_DAQmxSetAnlgWinPauseTrigCoupling) DAQmxSetAnlgWinPauseTrigCoupling = 0;
decltype(&__3c6e5b43_DAQmxSetAnlgWinPauseTrigDigFltrEnable) DAQmxSetAnlgWinPauseTrigDigFltrEnable = 0;
decltype(&__5b76786d_DAQmxSetAnlgWinPauseTrigDigFltrMinPulseWidth) DAQmxSetAnlgWinPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__5e655c0e_DAQmxSetAnlgWinPauseTrigDigFltrTimebaseRate) DAQmxSetAnlgWinPauseTrigDigFltrTimebaseRate = 0;
decltype(&__4d72390f_DAQmxSetAnlgWinPauseTrigDigFltrTimebaseSrc) DAQmxSetAnlgWinPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__29744a56_DAQmxSetAnlgWinPauseTrigDigSyncEnable) DAQmxSetAnlgWinPauseTrigDigSyncEnable = 0;
decltype(&__4c604f2c_DAQmxSetAnlgWinPauseTrigSrc) DAQmxSetAnlgWinPauseTrigSrc = 0;
decltype(&__4b7d5c2c_DAQmxSetAnlgWinPauseTrigTop) DAQmxSetAnlgWinPauseTrigTop = 0;
decltype(&__487a4942_DAQmxSetAnlgWinPauseTrigWhen) DAQmxSetAnlgWinPauseTrigWhen = 0;
decltype(&__4b14202a_DAQmxSetAnlgWinRefTrigBtm) DAQmxSetAnlgWinRefTrigBtm = 0;
decltype(&__3d034d58_DAQmxSetAnlgWinRefTrigCoupling) DAQmxSetAnlgWinRefTrigCoupling = 0;
decltype(&__517b4122_DAQmxSetAnlgWinRefTrigDigFltrEnable) DAQmxSetAnlgWinRefTrigDigFltrEnable = 0;
decltype(&__7255263a_DAQmxSetAnlgWinRefTrigDigFltrMinPulseWidth) DAQmxSetAnlgWinRefTrigDigFltrMinPulseWidth = 0;
decltype(&__56362329_DAQmxSetAnlgWinRefTrigDigFltrTimebaseRate) DAQmxSetAnlgWinRefTrigDigFltrTimebaseRate = 0;
decltype(&__3337303e_DAQmxSetAnlgWinRefTrigDigFltrTimebaseSrc) DAQmxSetAnlgWinRefTrigDigFltrTimebaseSrc = 0;
decltype(&__406e5438_DAQmxSetAnlgWinRefTrigDigSyncEnable) DAQmxSetAnlgWinRefTrigDigSyncEnable = 0;
decltype(&__4514312c_DAQmxSetAnlgWinRefTrigSrc) DAQmxSetAnlgWinRefTrigSrc = 0;
decltype(&__56143631_DAQmxSetAnlgWinRefTrigTop) DAQmxSetAnlgWinRefTrigTop = 0;
decltype(&__437a3536_DAQmxSetAnlgWinRefTrigWhen) DAQmxSetAnlgWinRefTrigWhen = 0;
decltype(&__4872403e_DAQmxSetAnlgWinStartTrigBtm) DAQmxSetAnlgWinStartTrigBtm = 0;
decltype(&__25003629_DAQmxSetAnlgWinStartTrigCoupling) DAQmxSetAnlgWinStartTrigCoupling = 0;
decltype(&__297a5a51_DAQmxSetAnlgWinStartTrigDigFltrEnable) DAQmxSetAnlgWinStartTrigDigFltrEnable = 0;
decltype(&__4e62797f_DAQmxSetAnlgWinStartTrigDigFltrMinPulseWidth) DAQmxSetAnlgWinStartTrigDigFltrMinPulseWidth = 0;
decltype(&__4b715d1c_DAQmxSetAnlgWinStartTrigDigFltrTimebaseRate) DAQmxSetAnlgWinStartTrigDigFltrTimebaseRate = 0;
decltype(&__5866381d_DAQmxSetAnlgWinStartTrigDigFltrTimebaseSrc) DAQmxSetAnlgWinStartTrigDigFltrTimebaseSrc = 0;
decltype(&__3c604b44_DAQmxSetAnlgWinStartTrigDigSyncEnable) DAQmxSetAnlgWinStartTrigDigSyncEnable = 0;
decltype(&__59744e3e_DAQmxSetAnlgWinStartTrigSrc) DAQmxSetAnlgWinStartTrigSrc = 0;
decltype(&__5e695d3e_DAQmxSetAnlgWinStartTrigTop) DAQmxSetAnlgWinStartTrigTop = 0;
decltype(&__5d6e4850_DAQmxSetAnlgWinStartTrigWhen) DAQmxSetAnlgWinStartTrigWhen = 0;
decltype(&__1f794e50_DAQmxSetArmStartTrigTimescale) DAQmxSetArmStartTrigTimescale = 0;
decltype(&__68474530_DAQmxSetArmStartTrigTimestampEnable) DAQmxSetArmStartTrigTimestampEnable = 0;
decltype(&__032c445d_DAQmxSetArmStartTrigTimestampTimescale) DAQmxSetArmStartTrigTimestampTimescale = 0;
decltype(&__5e694e50_DAQmxSetArmStartTrigTrigWhen) DAQmxSetArmStartTrigTrigWhen = 0;
decltype(&__090a323c_DAQmxSetArmStartTrigType) DAQmxSetArmStartTrigType = 0;
decltype(&__3b182477_DAQmxSetBufInputBufSize) DAQmxSetBufInputBufSize = 0;
decltype(&__2c382d20_DAQmxSetBufOutputBufSize) DAQmxSetBufOutputBufSize = 0;
decltype(&__0e6b674e_DAQmxSetBufOutputOnbrdBufSize) DAQmxSetBufOutputOnbrdBufSize = 0;
decltype(&__1a131f69_DAQmxSetBufferAttribute) DAQmxSetBufferAttribute = 0;
decltype(&__39121736_DAQmxSetCIAngEncoderInitialAngle) DAQmxSetCIAngEncoderInitialAngle = 0;
decltype(&__302e2706_DAQmxSetCIAngEncoderPulsesPerRev) DAQmxSetCIAngEncoderPulsesPerRev = 0;
decltype(&__51141712_DAQmxSetCIAngEncoderUnits) DAQmxSetCIAngEncoderUnits = 0;
decltype(&__3e31574d_DAQmxSetCICountEdgesActiveEdge) DAQmxSetCICountEdgesActiveEdge = 0;
decltype(&__7a050d33_DAQmxSetCICountEdgesCountDirDigFltrEnable) DAQmxSetCICountEdgesCountDirDigFltrEnable = 0;
decltype(&__1d1d2e1d_DAQmxSetCICountEdgesCountDirDigFltrMinPulseWidth) DAQmxSetCICountEdgesCountDirDigFltrMinPulseWidth = 0;
decltype(&__180e0a7e_DAQmxSetCICountEdgesCountDirDigFltrTimebaseRate) DAQmxSetCICountEdgesCountDirDigFltrTimebaseRate = 0;
decltype(&__0b196f7f_DAQmxSetCICountEdgesCountDirDigFltrTimebaseSrc) DAQmxSetCICountEdgesCountDirDigFltrTimebaseSrc = 0;
decltype(&__6f1f1c26_DAQmxSetCICountEdgesCountDirDigSyncEnable) DAQmxSetCICountEdgesCountDirDigSyncEnable = 0;
decltype(&__11000928_DAQmxSetCICountEdgesCountDirHyst) DAQmxSetCICountEdgesCountDirHyst = 0;
decltype(&__42566c4a_DAQmxSetCICountEdgesCountDirLogicLvlBehavior) DAQmxSetCICountEdgesCountDirLogicLvlBehavior = 0;
decltype(&__4e7a6f31_DAQmxSetCICountEdgesCountDirTermCfg) DAQmxSetCICountEdgesCountDirTermCfg = 0;
decltype(&__770d3f31_DAQmxSetCICountEdgesCountDirThreshVoltage) DAQmxSetCICountEdgesCountDirThreshVoltage = 0;
decltype(&__0d16263e_DAQmxSetCICountEdgesCountResetActiveEdge) DAQmxSetCICountEdgesCountResetActiveEdge = 0;
decltype(&__4b745521_DAQmxSetCICountEdgesCountResetDigFltrEnable) DAQmxSetCICountEdgesCountResetDigFltrEnable = 0;
decltype(&__685a3239_DAQmxSetCICountEdgesCountResetDigFltrMinPulseWidth) DAQmxSetCICountEdgesCountResetDigFltrMinPulseWidth = 0;
decltype(&__4c39372a_DAQmxSetCICountEdgesCountResetDigFltrTimebaseRate) DAQmxSetCICountEdgesCountResetDigFltrTimebaseRate = 0;
decltype(&__2938243d_DAQmxSetCICountEdgesCountResetDigFltrTimebaseSrc) DAQmxSetCICountEdgesCountResetDigFltrTimebaseSrc = 0;
decltype(&__5a61403b_DAQmxSetCICountEdgesCountResetDigSyncEnable) DAQmxSetCICountEdgesCountResetDigSyncEnable = 0;
decltype(&__5d795f56_DAQmxSetCICountEdgesCountResetEnable) DAQmxSetCICountEdgesCountResetEnable = 0;
decltype(&__4f6f3e24_DAQmxSetCICountEdgesCountResetHyst) DAQmxSetCICountEdgesCountResetHyst = 0;
decltype(&__2a0d6d72_DAQmxSetCICountEdgesCountResetLogicLvlBehavior) DAQmxSetCICountEdgesCountResetLogicLvlBehavior = 0;
decltype(&__200b3e0f_DAQmxSetCICountEdgesCountResetResetCount) DAQmxSetCICountEdgesCountResetResetCount = 0;
decltype(&__4e762238_DAQmxSetCICountEdgesCountResetTerm) DAQmxSetCICountEdgesCountResetTerm = 0;
decltype(&__2976615e_DAQmxSetCICountEdgesCountResetTermCfg) DAQmxSetCICountEdgesCountResetTermCfg = 0;
decltype(&__79765829_DAQmxSetCICountEdgesCountResetThreshVoltage) DAQmxSetCICountEdgesCountResetThreshVoltage = 0;
decltype(&__4d2e112f_DAQmxSetCICountEdgesDigFltrEnable) DAQmxSetCICountEdgesDigFltrEnable = 0;
decltype(&__2a363201_DAQmxSetCICountEdgesDigFltrMinPulseWidth) DAQmxSetCICountEdgesDigFltrMinPulseWidth = 0;
decltype(&__2f251662_DAQmxSetCICountEdgesDigFltrTimebaseRate) DAQmxSetCICountEdgesDigFltrTimebaseRate = 0;
decltype(&__3c327363_DAQmxSetCICountEdgesDigFltrTimebaseSrc) DAQmxSetCICountEdgesDigFltrTimebaseSrc = 0;
decltype(&__5834003a_DAQmxSetCICountEdgesDigSyncEnable) DAQmxSetCICountEdgesDigSyncEnable = 0;
decltype(&__2a3b1440_DAQmxSetCICountEdgesDir) DAQmxSetCICountEdgesDir = 0;
decltype(&__4f497914_DAQmxSetCICountEdgesDirTerm) DAQmxSetCICountEdgesDirTerm = 0;
decltype(&__0a4f654a_DAQmxSetCICountEdgesGateDigFltrEnable) DAQmxSetCICountEdgesGateDigFltrEnable = 0;
decltype(&__6d574664_DAQmxSetCICountEdgesGateDigFltrMinPulseWidth) DAQmxSetCICountEdgesGateDigFltrMinPulseWidth = 0;
decltype(&__68446207_DAQmxSetCICountEdgesGateDigFltrTimebaseRate) DAQmxSetCICountEdgesGateDigFltrTimebaseRate = 0;
decltype(&__7b530706_DAQmxSetCICountEdgesGateDigFltrTimebaseSrc) DAQmxSetCICountEdgesGateDigFltrTimebaseSrc = 0;
decltype(&__00387347_DAQmxSetCICountEdgesGateEnable) DAQmxSetCICountEdgesGateEnable = 0;
decltype(&__614a6151_DAQmxSetCICountEdgesGateHyst) DAQmxSetCICountEdgesGateHyst = 0;
decltype(&__321c0433_DAQmxSetCICountEdgesGateLogicLvlBehavior) DAQmxSetCICountEdgesGateLogicLvlBehavior = 0;
decltype(&__7d566048_DAQmxSetCICountEdgesGateTerm) DAQmxSetCICountEdgesGateTerm = 0;
decltype(&__3e300748_DAQmxSetCICountEdgesGateTermCfg) DAQmxSetCICountEdgesGateTermCfg = 0;
decltype(&__07475748_DAQmxSetCICountEdgesGateThreshVoltage) DAQmxSetCICountEdgesGateThreshVoltage = 0;
decltype(&__7e5b774b_DAQmxSetCICountEdgesGateWhen) DAQmxSetCICountEdgesGateWhen = 0;
decltype(&__262b1534_DAQmxSetCICountEdgesHyst) DAQmxSetCICountEdgesHyst = 0;
decltype(&__20296377_DAQmxSetCICountEdgesInitialCnt) DAQmxSetCICountEdgesInitialCnt = 0;
decltype(&__757d7056_DAQmxSetCICountEdgesLogicLvlBehavior) DAQmxSetCICountEdgesLogicLvlBehavior = 0;
decltype(&__3a37142d_DAQmxSetCICountEdgesTerm) DAQmxSetCICountEdgesTerm = 0;
decltype(&__7951732d_DAQmxSetCICountEdgesTermCfg) DAQmxSetCICountEdgesTermCfg = 0;
decltype(&__4026232d_DAQmxSetCICountEdgesThreshVoltage) DAQmxSetCICountEdgesThreshVoltage = 0;
decltype(&__003d1c42_DAQmxSetCICtrTimebaseActiveEdge) DAQmxSetCICtrTimebaseActiveEdge = 0;
decltype(&__624e0304_DAQmxSetCICtrTimebaseDigFltrEnable) DAQmxSetCICtrTimebaseDigFltrEnable = 0;
decltype(&__4c291b27_DAQmxSetCICtrTimebaseDigFltrMinPulseWidth) DAQmxSetCICtrTimebaseDigFltrMinPulseWidth = 0;
decltype(&__2f2c0803_DAQmxSetCICtrTimebaseDigFltrTimebaseRate) DAQmxSetCICtrTimebaseDigFltrTimebaseRate = 0;
decltype(&__2e3f1f66_DAQmxSetCICtrTimebaseDigFltrTimebaseSrc) DAQmxSetCICtrTimebaseDigFltrTimebaseSrc = 0;
decltype(&__775b1915_DAQmxSetCICtrTimebaseDigSyncEnable) DAQmxSetCICtrTimebaseDigSyncEnable = 0;
decltype(&__182d6c72_DAQmxSetCICtrTimebaseMasterTimebaseDiv) DAQmxSetCICtrTimebaseMasterTimebaseDiv = 0;
decltype(&__683f1e07_DAQmxSetCICtrTimebaseRate) DAQmxSetCICtrTimebaseRate = 0;
decltype(&__0d3e0d10_DAQmxSetCICtrTimebaseSrc) DAQmxSetCICtrTimebaseSrc = 0;
decltype(&__5f021800_DAQmxSetCICustomScaleName) DAQmxSetCICustomScaleName = 0;
decltype(&__0d20657b_DAQmxSetCIDataXferMech) DAQmxSetCIDataXferMech = 0;
decltype(&__7b0b1515_DAQmxSetCIDataXferReqCond) DAQmxSetCIDataXferReqCond = 0;
decltype(&__701a2a05_DAQmxSetCIDupCountPrevent) DAQmxSetCIDupCountPrevent = 0;
decltype(&__14393936_DAQmxSetCIDutyCycleDigFltrEnable) DAQmxSetCIDutyCycleDigFltrEnable = 0;
decltype(&__0c1a1751_DAQmxSetCIDutyCycleDigFltrMinPulseWidth) DAQmxSetCIDutyCycleDigFltrMinPulseWidth = 0;
decltype(&__1f3e7454_DAQmxSetCIDutyCycleDigFltrTimebaseRate) DAQmxSetCIDutyCycleDigFltrTimebaseRate = 0;
decltype(&__085b7547_DAQmxSetCIDutyCycleDigFltrTimebaseSrc) DAQmxSetCIDutyCycleDigFltrTimebaseSrc = 0;
decltype(&__4758400e_DAQmxSetCIDutyCycleLogicLvlBehavior) DAQmxSetCIDutyCycleLogicLvlBehavior = 0;
decltype(&__11262677_DAQmxSetCIDutyCycleStartingEdge) DAQmxSetCIDutyCycleStartingEdge = 0;
decltype(&__0d3c3b41_DAQmxSetCIDutyCycleTerm) DAQmxSetCIDutyCycleTerm = 0;
decltype(&__6b5b3b02_DAQmxSetCIDutyCycleTermCfg) DAQmxSetCIDutyCycleTermCfg = 0;
decltype(&__6277475f_DAQmxSetCIEncoderAInputDigFltrEnable) DAQmxSetCIEncoderAInputDigFltrEnable = 0;
decltype(&__7a546938_DAQmxSetCIEncoderAInputDigFltrMinPulseWidth) DAQmxSetCIEncoderAInputDigFltrMinPulseWidth = 0;
decltype(&__69700a3d_DAQmxSetCIEncoderAInputDigFltrTimebaseRate) DAQmxSetCIEncoderAInputDigFltrTimebaseRate = 0;
decltype(&__7e150b2e_DAQmxSetCIEncoderAInputDigFltrTimebaseSrc) DAQmxSetCIEncoderAInputDigFltrTimebaseSrc = 0;
decltype(&__7866524a_DAQmxSetCIEncoderAInputDigSyncEnable) DAQmxSetCIEncoderAInputDigSyncEnable = 0;
decltype(&__31163e67_DAQmxSetCIEncoderAInputLogicLvlBehavior) DAQmxSetCIEncoderAInputLogicLvlBehavior = 0;
decltype(&__7b724528_DAQmxSetCIEncoderAInputTerm) DAQmxSetCIEncoderAInputTerm = 0;
decltype(&__1d15456b_DAQmxSetCIEncoderAInputTermCfg) DAQmxSetCIEncoderAInputTermCfg = 0;
decltype(&__6274475f_DAQmxSetCIEncoderBInputDigFltrEnable) DAQmxSetCIEncoderBInputDigFltrEnable = 0;
decltype(&__7a576938_DAQmxSetCIEncoderBInputDigFltrMinPulseWidth) DAQmxSetCIEncoderBInputDigFltrMinPulseWidth = 0;
decltype(&__69730a3d_DAQmxSetCIEncoderBInputDigFltrTimebaseRate) DAQmxSetCIEncoderBInputDigFltrTimebaseRate = 0;
decltype(&__7e160b2e_DAQmxSetCIEncoderBInputDigFltrTimebaseSrc) DAQmxSetCIEncoderBInputDigFltrTimebaseSrc = 0;
decltype(&__7865524a_DAQmxSetCIEncoderBInputDigSyncEnable) DAQmxSetCIEncoderBInputDigSyncEnable = 0;
decltype(&__31153e67_DAQmxSetCIEncoderBInputLogicLvlBehavior) DAQmxSetCIEncoderBInputLogicLvlBehavior = 0;
decltype(&__7b714528_DAQmxSetCIEncoderBInputTerm) DAQmxSetCIEncoderBInputTerm = 0;
decltype(&__1d16456b_DAQmxSetCIEncoderBInputTermCfg) DAQmxSetCIEncoderBInputTermCfg = 0;
decltype(&__0340606f_DAQmxSetCIEncoderDecodingType) DAQmxSetCIEncoderDecodingType = 0;
decltype(&__016a4655_DAQmxSetCIEncoderZIndexEnable) DAQmxSetCIEncoderZIndexEnable = 0;
decltype(&__626a5749_DAQmxSetCIEncoderZIndexPhase) DAQmxSetCIEncoderZIndexPhase = 0;
decltype(&__6b67242a_DAQmxSetCIEncoderZIndexVal) DAQmxSetCIEncoderZIndexVal = 0;
decltype(&__626c475f_DAQmxSetCIEncoderZInputDigFltrEnable) DAQmxSetCIEncoderZInputDigFltrEnable = 0;
decltype(&__7a4f6938_DAQmxSetCIEncoderZInputDigFltrMinPulseWidth) DAQmxSetCIEncoderZInputDigFltrMinPulseWidth = 0;
decltype(&__696b0a3d_DAQmxSetCIEncoderZInputDigFltrTimebaseRate) DAQmxSetCIEncoderZInputDigFltrTimebaseRate = 0;
decltype(&__7e0e0b2e_DAQmxSetCIEncoderZInputDigFltrTimebaseSrc) DAQmxSetCIEncoderZInputDigFltrTimebaseSrc = 0;
decltype(&__787d524a_DAQmxSetCIEncoderZInputDigSyncEnable) DAQmxSetCIEncoderZInputDigSyncEnable = 0;
decltype(&__310d3e67_DAQmxSetCIEncoderZInputLogicLvlBehavior) DAQmxSetCIEncoderZInputLogicLvlBehavior = 0;
decltype(&__7b694528_DAQmxSetCIEncoderZInputTerm) DAQmxSetCIEncoderZInputTerm = 0;
decltype(&__1d0e456b_DAQmxSetCIEncoderZInputTermCfg) DAQmxSetCIEncoderZInputTermCfg = 0;
decltype(&__5a6c150a_DAQmxSetCIFilterDelayUnits) DAQmxSetCIFilterDelayUnits = 0;
decltype(&__3a247660_DAQmxSetCIFilterEnable) DAQmxSetCIFilterEnable = 0;
decltype(&__555d7273_DAQmxSetCIFilterFreq) DAQmxSetCIFilterFreq = 0;
decltype(&__2e5d7367_DAQmxSetCIFilterOrder) DAQmxSetCIFilterOrder = 0;
decltype(&__2e241717_DAQmxSetCIFilterResponse) DAQmxSetCIFilterResponse = 0;
decltype(&__6d455117_DAQmxSetCIFreqDigFltrEnable) DAQmxSetCIFreqDigFltrEnable = 0;
decltype(&__4e6b360f_DAQmxSetCIFreqDigFltrMinPulseWidth) DAQmxSetCIFreqDigFltrMinPulseWidth = 0;
decltype(&__6a08331c_DAQmxSetCIFreqDigFltrTimebaseRate) DAQmxSetCIFreqDigFltrTimebaseRate = 0;
decltype(&__0f09200b_DAQmxSetCIFreqDigFltrTimebaseSrc) DAQmxSetCIFreqDigFltrTimebaseSrc = 0;
decltype(&__7c50440d_DAQmxSetCIFreqDigSyncEnable) DAQmxSetCIFreqDigSyncEnable = 0;
decltype(&__6c2a3602_DAQmxSetCIFreqDiv) DAQmxSetCIFreqDiv = 0;
decltype(&__3c59577c_DAQmxSetCIFreqEnableAveraging) DAQmxSetCIFreqEnableAveraging = 0;
decltype(&__695e3a12_DAQmxSetCIFreqHyst) DAQmxSetCIFreqHyst = 0;
decltype(&__0c3c6944_DAQmxSetCIFreqLogicLvlBehavior) DAQmxSetCIFreqLogicLvlBehavior = 0;
decltype(&__0f31726b_DAQmxSetCIFreqMeasMeth) DAQmxSetCIFreqMeasMeth = 0;
decltype(&__163c6b67_DAQmxSetCIFreqMeasTime) DAQmxSetCIFreqMeasTime = 0;
decltype(&__725a1012_DAQmxSetCIFreqStartingEdge) DAQmxSetCIFreqStartingEdge = 0;
decltype(&__6847260e_DAQmxSetCIFreqTerm) DAQmxSetCIFreqTerm = 0;
decltype(&__0f476568_DAQmxSetCIFreqTermCfg) DAQmxSetCIFreqTermCfg = 0;
decltype(&__5f475c1f_DAQmxSetCIFreqThreshVoltage) DAQmxSetCIFreqThreshVoltage = 0;
decltype(&__735e5405_DAQmxSetCIFreqUnits) DAQmxSetCIFreqUnits = 0;
decltype(&__272a0b53_DAQmxSetCIGPSSyncMethod) DAQmxSetCIGPSSyncMethod = 0;
decltype(&__4f5b7844_DAQmxSetCIGPSSyncSrc) DAQmxSetCIGPSSyncSrc = 0;
decltype(&__1f1a0120_DAQmxSetCILinEncoderDistPerPulse) DAQmxSetCILinEncoderDistPerPulse = 0;
decltype(&__31067645_DAQmxSetCILinEncoderInitialPos) DAQmxSetCILinEncoderInitialPos = 0;
decltype(&__58141a15_DAQmxSetCILinEncoderUnits) DAQmxSetCILinEncoderUnits = 0;
decltype(&__075b7978_DAQmxSetCIMax) DAQmxSetCIMax = 0;
decltype(&__1d291d6b_DAQmxSetCIMaxMeasPeriod) DAQmxSetCIMaxMeasPeriod = 0;
decltype(&__3b1d796e_DAQmxSetCIMemMapEnable) DAQmxSetCIMemMapEnable = 0;
decltype(&__115b7970_DAQmxSetCIMin) DAQmxSetCIMin = 0;
decltype(&__2e4e7c77_DAQmxSetCIPeriodDigFltrEnable) DAQmxSetCIPeriodDigFltrEnable = 0;
decltype(&__49565f59_DAQmxSetCIPeriodDigFltrMinPulseWidth) DAQmxSetCIPeriodDigFltrMinPulseWidth = 0;
decltype(&__4c457b3a_DAQmxSetCIPeriodDigFltrTimebaseRate) DAQmxSetCIPeriodDigFltrTimebaseRate = 0;
decltype(&__5f521e3b_DAQmxSetCIPeriodDigFltrTimebaseSrc) DAQmxSetCIPeriodDigFltrTimebaseSrc = 0;
decltype(&__3b546d62_DAQmxSetCIPeriodDigSyncEnable) DAQmxSetCIPeriodDigSyncEnable = 0;
decltype(&__495b7d18_DAQmxSetCIPeriodDiv) DAQmxSetCIPeriodDiv = 0;
decltype(&__28252d6b_DAQmxSetCIPeriodEnableAveraging) DAQmxSetCIPeriodEnableAveraging = 0;
decltype(&__454b786c_DAQmxSetCIPeriodHyst) DAQmxSetCIPeriodHyst = 0;
decltype(&__161d1d0e_DAQmxSetCIPeriodLogicLvlBehavior) DAQmxSetCIPeriodLogicLvlBehavior = 0;
decltype(&__0d321e03_DAQmxSetCIPeriodMeasMeth) DAQmxSetCIPeriodMeasMeth = 0;
decltype(&__143e070e_DAQmxSetCIPeriodMeasTime) DAQmxSetCIPeriodMeasTime = 0;
decltype(&__6f4b6368_DAQmxSetCIPeriodStartingEdge) DAQmxSetCIPeriodStartingEdge = 0;
decltype(&__59577975_DAQmxSetCIPeriodTerm) DAQmxSetCIPeriodTerm = 0;
decltype(&__1a311e75_DAQmxSetCIPeriodTermCfg) DAQmxSetCIPeriodTermCfg = 0;
decltype(&__23464e75_DAQmxSetCIPeriodThreshVoltage) DAQmxSetCIPeriodThreshVoltage = 0;
decltype(&__2b5c626c_DAQmxSetCIPeriodUnits) DAQmxSetCIPeriodUnits = 0;
decltype(&__764d750a_DAQmxSetCIPrescaler) DAQmxSetCIPrescaler = 0;
decltype(&__1d3a1f09_DAQmxSetCIPulseFreqDigFltrEnable) DAQmxSetCIPulseFreqDigFltrEnable = 0;
decltype(&__0519316e_DAQmxSetCIPulseFreqDigFltrMinPulseWidth) DAQmxSetCIPulseFreqDigFltrMinPulseWidth = 0;
decltype(&__163d526b_DAQmxSetCIPulseFreqDigFltrTimebaseRate) DAQmxSetCIPulseFreqDigFltrTimebaseRate = 0;
decltype(&__01585378_DAQmxSetCIPulseFreqDigFltrTimebaseSrc) DAQmxSetCIPulseFreqDigFltrTimebaseSrc = 0;
decltype(&__072b0a1c_DAQmxSetCIPulseFreqDigSyncEnable) DAQmxSetCIPulseFreqDigSyncEnable = 0;
decltype(&__4e5b6631_DAQmxSetCIPulseFreqLogicLvlBehavior) DAQmxSetCIPulseFreqLogicLvlBehavior = 0;
decltype(&__50486568_DAQmxSetCIPulseFreqStartEdge) DAQmxSetCIPulseFreqStartEdge = 0;
decltype(&__043f1d7e_DAQmxSetCIPulseFreqTerm) DAQmxSetCIPulseFreqTerm = 0;
decltype(&__62581d3d_DAQmxSetCIPulseFreqTermCfg) DAQmxSetCIPulseFreqTermCfg = 0;
decltype(&__0f24040c_DAQmxSetCIPulseFreqUnits) DAQmxSetCIPulseFreqUnits = 0;
decltype(&__59371d24_DAQmxSetCIPulseTicksDigFltrEnable) DAQmxSetCIPulseTicksDigFltrEnable = 0;
decltype(&__3e2f3e0a_DAQmxSetCIPulseTicksDigFltrMinPulseWidth) DAQmxSetCIPulseTicksDigFltrMinPulseWidth = 0;
decltype(&__3b3c1a69_DAQmxSetCIPulseTicksDigFltrTimebaseRate) DAQmxSetCIPulseTicksDigFltrTimebaseRate = 0;
decltype(&__282b7f68_DAQmxSetCIPulseTicksDigFltrTimebaseSrc) DAQmxSetCIPulseTicksDigFltrTimebaseSrc = 0;
decltype(&__4c2d0c31_DAQmxSetCIPulseTicksDigSyncEnable) DAQmxSetCIPulseTicksDigSyncEnable = 0;
decltype(&__61647c5d_DAQmxSetCIPulseTicksLogicLvlBehavior) DAQmxSetCIPulseTicksLogicLvlBehavior = 0;
decltype(&__387a6f5e_DAQmxSetCIPulseTicksStartEdge) DAQmxSetCIPulseTicksStartEdge = 0;
decltype(&__2e2e1826_DAQmxSetCIPulseTicksTerm) DAQmxSetCIPulseTicksTerm = 0;
decltype(&__6d487f26_DAQmxSetCIPulseTicksTermCfg) DAQmxSetCIPulseTicksTermCfg = 0;
decltype(&__06320b1b_DAQmxSetCIPulseTimeDigFltrEnable) DAQmxSetCIPulseTimeDigFltrEnable = 0;
decltype(&__1e11257c_DAQmxSetCIPulseTimeDigFltrMinPulseWidth) DAQmxSetCIPulseTimeDigFltrMinPulseWidth = 0;
decltype(&__0d354679_DAQmxSetCIPulseTimeDigFltrTimebaseRate) DAQmxSetCIPulseTimeDigFltrTimebaseRate = 0;
decltype(&__1a50476a_DAQmxSetCIPulseTimeDigFltrTimebaseSrc) DAQmxSetCIPulseTimeDigFltrTimebaseSrc = 0;
decltype(&__1c231e0e_DAQmxSetCIPulseTimeDigSyncEnable) DAQmxSetCIPulseTimeDigSyncEnable = 0;
decltype(&__55537223_DAQmxSetCIPulseTimeLogicLvlBehavior) DAQmxSetCIPulseTimeLogicLvlBehavior = 0;
decltype(&__4b40717a_DAQmxSetCIPulseTimeStartEdge) DAQmxSetCIPulseTimeStartEdge = 0;
decltype(&__1f37096c_DAQmxSetCIPulseTimeTerm) DAQmxSetCIPulseTimeTerm = 0;
decltype(&__7950092f_DAQmxSetCIPulseTimeTermCfg) DAQmxSetCIPulseTimeTermCfg = 0;
decltype(&__142c101e_DAQmxSetCIPulseTimeUnits) DAQmxSetCIPulseTimeUnits = 0;
decltype(&__5930023c_DAQmxSetCIPulseWidthDigFltrEnable) DAQmxSetCIPulseWidthDigFltrEnable = 0;
decltype(&__3e282112_DAQmxSetCIPulseWidthDigFltrMinPulseWidth) DAQmxSetCIPulseWidthDigFltrMinPulseWidth = 0;
decltype(&__3b3b0571_DAQmxSetCIPulseWidthDigFltrTimebaseRate) DAQmxSetCIPulseWidthDigFltrTimebaseRate = 0;
decltype(&__282c6070_DAQmxSetCIPulseWidthDigFltrTimebaseSrc) DAQmxSetCIPulseWidthDigFltrTimebaseSrc = 0;
decltype(&__4c2a1329_DAQmxSetCIPulseWidthDigSyncEnable) DAQmxSetCIPulseWidthDigSyncEnable = 0;
decltype(&__61636345_DAQmxSetCIPulseWidthLogicLvlBehavior) DAQmxSetCIPulseWidthLogicLvlBehavior = 0;
decltype(&__18351d23_DAQmxSetCIPulseWidthStartingEdge) DAQmxSetCIPulseWidthStartingEdge = 0;
decltype(&__2e29073e_DAQmxSetCIPulseWidthTerm) DAQmxSetCIPulseWidthTerm = 0;
decltype(&__6d4f603e_DAQmxSetCIPulseWidthTermCfg) DAQmxSetCIPulseWidthTermCfg = 0;
decltype(&__5c221c27_DAQmxSetCIPulseWidthUnits) DAQmxSetCIPulseWidthUnits = 0;
decltype(&__3f1a200c_DAQmxSetCISampClkOverrunBehavior) DAQmxSetCISampClkOverrunBehavior = 0;
decltype(&__677c4007_DAQmxSetCISampClkOverrunSentinelVal) DAQmxSetCISampClkOverrunSentinelVal = 0;
decltype(&__43272f12_DAQmxSetCISemiPeriodDigFltrEnable) DAQmxSetCISemiPeriodDigFltrEnable = 0;
decltype(&__243f0c3c_DAQmxSetCISemiPeriodDigFltrMinPulseWidth) DAQmxSetCISemiPeriodDigFltrMinPulseWidth = 0;
decltype(&__212c285f_DAQmxSetCISemiPeriodDigFltrTimebaseRate) DAQmxSetCISemiPeriodDigFltrTimebaseRate = 0;
decltype(&__323b4d5e_DAQmxSetCISemiPeriodDigFltrTimebaseSrc) DAQmxSetCISemiPeriodDigFltrTimebaseSrc = 0;
decltype(&__563d3e07_DAQmxSetCISemiPeriodDigSyncEnable) DAQmxSetCISemiPeriodDigSyncEnable = 0;
decltype(&__7b744e6b_DAQmxSetCISemiPeriodLogicLvlBehavior) DAQmxSetCISemiPeriodLogicLvlBehavior = 0;
decltype(&__0222300d_DAQmxSetCISemiPeriodStartingEdge) DAQmxSetCISemiPeriodStartingEdge = 0;
decltype(&__343e2a10_DAQmxSetCISemiPeriodTerm) DAQmxSetCISemiPeriodTerm = 0;
decltype(&__77584d10_DAQmxSetCISemiPeriodTermCfg) DAQmxSetCISemiPeriodTermCfg = 0;
decltype(&__46353109_DAQmxSetCISemiPeriodUnits) DAQmxSetCISemiPeriodUnits = 0;
decltype(&__3a361a6d_DAQmxSetCIThreshVoltage) DAQmxSetCIThreshVoltage = 0;
decltype(&__6c392a25_DAQmxSetCITimestampInitialSeconds) DAQmxSetCITimestampInitialSeconds = 0;
decltype(&__1d3a1722_DAQmxSetCITimestampUnits) DAQmxSetCITimestampUnits = 0;
decltype(&__28076f7d_DAQmxSetCITwoEdgeSepFirstDigFltrEnable) DAQmxSetCITwoEdgeSepFirstDigFltrEnable = 0;
decltype(&__0660775e_DAQmxSetCITwoEdgeSepFirstDigFltrMinPulseWidth) DAQmxSetCITwoEdgeSepFirstDigFltrMinPulseWidth = 0;
decltype(&__6565647a_DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseRate) DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseRate = 0;
decltype(&__6476731f_DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseSrc) DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseSrc = 0;
decltype(&__3d12756c_DAQmxSetCITwoEdgeSepFirstDigSyncEnable) DAQmxSetCITwoEdgeSepFirstDigSyncEnable = 0;
decltype(&__2261776d_DAQmxSetCITwoEdgeSepFirstEdge) DAQmxSetCITwoEdgeSepFirstEdge = 0;
decltype(&__513f3c1c_DAQmxSetCITwoEdgeSepFirstLogicLvlBehavior) DAQmxSetCITwoEdgeSepFirstLogicLvlBehavior = 0;
decltype(&__2a707678_DAQmxSetCITwoEdgeSepFirstTerm) DAQmxSetCITwoEdgeSepFirstTerm = 0;
decltype(&__2a33101f_DAQmxSetCITwoEdgeSepFirstTermCfg) DAQmxSetCITwoEdgeSepFirstTermCfg = 0;
decltype(&__3f23216a_DAQmxSetCITwoEdgeSepSecondDigFltrEnable) DAQmxSetCITwoEdgeSepSecondDigFltrEnable = 0;
decltype(&__1c0d4672_DAQmxSetCITwoEdgeSepSecondDigFltrMinPulseWidth) DAQmxSetCITwoEdgeSepSecondDigFltrMinPulseWidth = 0;
decltype(&__386e4361_DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseRate) DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseRate = 0;
decltype(&__5d6f5076_DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseSrc) DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseSrc = 0;
decltype(&__2e363470_DAQmxSetCITwoEdgeSepSecondDigSyncEnable) DAQmxSetCITwoEdgeSepSecondDigSyncEnable = 0;
decltype(&__2f294772_DAQmxSetCITwoEdgeSepSecondEdge) DAQmxSetCITwoEdgeSepSecondEdge = 0;
decltype(&__5e5a1939_DAQmxSetCITwoEdgeSepSecondLogicLvlBehavior) DAQmxSetCITwoEdgeSepSecondLogicLvlBehavior = 0;
decltype(&__3a215673_DAQmxSetCITwoEdgeSepSecondTerm) DAQmxSetCITwoEdgeSepSecondTerm = 0;
decltype(&__5d211515_DAQmxSetCITwoEdgeSepSecondTermCfg) DAQmxSetCITwoEdgeSepSecondTermCfg = 0;
decltype(&__5323080d_DAQmxSetCITwoEdgeSepUnits) DAQmxSetCITwoEdgeSepUnits = 0;
decltype(&__583e1710_DAQmxSetCIUsbXferReqCount) DAQmxSetCIUsbXferReqCount = 0;
decltype(&__3c38181b_DAQmxSetCIUsbXferReqSize) DAQmxSetCIUsbXferReqSize = 0;
decltype(&__2838120a_DAQmxSetCIVelocityAngEncoderPulsesPerRev) DAQmxSetCIVelocityAngEncoderPulsesPerRev = 0;
decltype(&__4902221e_DAQmxSetCIVelocityAngEncoderUnits) DAQmxSetCIVelocityAngEncoderUnits = 0;
decltype(&__114d457c_DAQmxSetCIVelocityDiv) DAQmxSetCIVelocityDiv = 0;
decltype(&__7a617253_DAQmxSetCIVelocityEncoderAInputDigFltrEnable) DAQmxSetCIVelocityEncoderAInputDigFltrEnable = 0;
decltype(&__62425c34_DAQmxSetCIVelocityEncoderAInputDigFltrMinPulseWidth) DAQmxSetCIVelocityEncoderAInputDigFltrMinPulseWidth = 0;
decltype(&__71663f31_DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseRate) DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseRate = 0;
decltype(&__66033e22_DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseSrc) DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseSrc = 0;
decltype(&__29000b6b_DAQmxSetCIVelocityEncoderAInputLogicLvlBehavior) DAQmxSetCIVelocityEncoderAInputLogicLvlBehavior = 0;
decltype(&__63647024_DAQmxSetCIVelocityEncoderAInputTerm) DAQmxSetCIVelocityEncoderAInputTerm = 0;
decltype(&__05037067_DAQmxSetCIVelocityEncoderAInputTermCfg) DAQmxSetCIVelocityEncoderAInputTermCfg = 0;
decltype(&__7a627253_DAQmxSetCIVelocityEncoderBInputDigFltrEnable) DAQmxSetCIVelocityEncoderBInputDigFltrEnable = 0;
decltype(&__62415c34_DAQmxSetCIVelocityEncoderBInputDigFltrMinPulseWidth) DAQmxSetCIVelocityEncoderBInputDigFltrMinPulseWidth = 0;
decltype(&__71653f31_DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseRate) DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseRate = 0;
decltype(&__66003e22_DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseSrc) DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseSrc = 0;
decltype(&__29030b6b_DAQmxSetCIVelocityEncoderBInputLogicLvlBehavior) DAQmxSetCIVelocityEncoderBInputLogicLvlBehavior = 0;
decltype(&__63677024_DAQmxSetCIVelocityEncoderBInputTerm) DAQmxSetCIVelocityEncoderBInputTerm = 0;
decltype(&__05007067_DAQmxSetCIVelocityEncoderBInputTermCfg) DAQmxSetCIVelocityEncoderBInputTermCfg = 0;
decltype(&__1b565563_DAQmxSetCIVelocityEncoderDecodingType) DAQmxSetCIVelocityEncoderDecodingType = 0;
decltype(&__070c342c_DAQmxSetCIVelocityLinEncoderDistPerPulse) DAQmxSetCIVelocityLinEncoderDistPerPulse = 0;
decltype(&__40022f19_DAQmxSetCIVelocityLinEncoderUnits) DAQmxSetCIVelocityLinEncoderUnits = 0;
decltype(&__6b5b1819_DAQmxSetCIVelocityMeasTime) DAQmxSetCIVelocityMeasTime = 0;
decltype(&__1c407f6c_DAQmxSetCOAutoIncrCnt) DAQmxSetCOAutoIncrCnt = 0;
decltype(&__596f6c6a_DAQmxSetCOConstrainedGenMode) DAQmxSetCOConstrainedGenMode = 0;
decltype(&__003b1c42_DAQmxSetCOCtrTimebaseActiveEdge) DAQmxSetCOCtrTimebaseActiveEdge = 0;
decltype(&__62480304_DAQmxSetCOCtrTimebaseDigFltrEnable) DAQmxSetCOCtrTimebaseDigFltrEnable = 0;
decltype(&__4c2f1b27_DAQmxSetCOCtrTimebaseDigFltrMinPulseWidth) DAQmxSetCOCtrTimebaseDigFltrMinPulseWidth = 0;
decltype(&__2f2a0803_DAQmxSetCOCtrTimebaseDigFltrTimebaseRate) DAQmxSetCOCtrTimebaseDigFltrTimebaseRate = 0;
decltype(&__2e391f66_DAQmxSetCOCtrTimebaseDigFltrTimebaseSrc) DAQmxSetCOCtrTimebaseDigFltrTimebaseSrc = 0;
decltype(&__775d1915_DAQmxSetCOCtrTimebaseDigSyncEnable) DAQmxSetCOCtrTimebaseDigSyncEnable = 0;
decltype(&__182b6c72_DAQmxSetCOCtrTimebaseMasterTimebaseDiv) DAQmxSetCOCtrTimebaseMasterTimebaseDiv = 0;
decltype(&__68391e07_DAQmxSetCOCtrTimebaseRate) DAQmxSetCOCtrTimebaseRate = 0;
decltype(&__0d380d10_DAQmxSetCOCtrTimebaseSrc) DAQmxSetCOCtrTimebaseSrc = 0;
decltype(&__0d26657b_DAQmxSetCODataXferMech) DAQmxSetCODataXferMech = 0;
decltype(&__7b0d1515_DAQmxSetCODataXferReqCond) DAQmxSetCODataXferReqCond = 0;
decltype(&__07253059_DAQmxSetCOEnableInitialDelayOnRetrigger) DAQmxSetCOEnableInitialDelayOnRetrigger = 0;
decltype(&__3b1b796e_DAQmxSetCOMemMapEnable) DAQmxSetCOMemMapEnable = 0;
decltype(&__764b750a_DAQmxSetCOPrescaler) DAQmxSetCOPrescaler = 0;
decltype(&__1f39786b_DAQmxSetCOPulseDutyCyc) DAQmxSetCOPulseDutyCyc = 0;
decltype(&__614b702a_DAQmxSetCOPulseFreq) DAQmxSetCOPulseFreq = 0;
decltype(&__022f396f_DAQmxSetCOPulseFreqInitialDelay) DAQmxSetCOPulseFreqInitialDelay = 0;
decltype(&__0f22040c_DAQmxSetCOPulseFreqUnits) DAQmxSetCOPulseFreqUnits = 0;
decltype(&__132a0203_DAQmxSetCOPulseHighTicks) DAQmxSetCOPulseHighTicks = 0;
decltype(&__13240c70_DAQmxSetCOPulseHighTime) DAQmxSetCOPulseHighTime = 0;
decltype(&__03231013_DAQmxSetCOPulseIdleState) DAQmxSetCOPulseIdleState = 0;
decltype(&__1f322649_DAQmxSetCOPulseLowTicks) DAQmxSetCOPulseLowTicks = 0;
decltype(&__113c5549_DAQmxSetCOPulseLowTime) DAQmxSetCOPulseLowTime = 0;
decltype(&__765c6c38_DAQmxSetCOPulseTerm) DAQmxSetCOPulseTerm = 0;
decltype(&__3f2e0e02_DAQmxSetCOPulseTicksInitialDelay) DAQmxSetCOPulseTicksInitialDelay = 0;
decltype(&__19272d7d_DAQmxSetCOPulseTimeInitialDelay) DAQmxSetCOPulseTimeInitialDelay = 0;
decltype(&__142a101e_DAQmxSetCOPulseTimeUnits) DAQmxSetCOPulseTimeUnits = 0;
decltype(&__58381710_DAQmxSetCOUsbXferReqCount) DAQmxSetCOUsbXferReqCount = 0;
decltype(&__3c3e181b_DAQmxSetCOUsbXferReqSize) DAQmxSetCOUsbXferReqSize = 0;
decltype(&__7c392c21_DAQmxSetCOUseOnlyOnBrdMem) DAQmxSetCOUseOnlyOnBrdMem = 0;
decltype(&__31786454_DAQmxSetCalAccConnectionCount) DAQmxSetCalAccConnectionCount = 0;
decltype(&__0714311d_DAQmxSetCalInfoAttribute) DAQmxSetCalInfoAttribute = 0;
decltype(&__6a7b0a08_DAQmxSetCalUserDefinedInfo) DAQmxSetCalUserDefinedInfo = 0;
decltype(&__326c5471_DAQmxSetChanAttribute) DAQmxSetChanAttribute = 0;
decltype(&__491f2614_DAQmxSetChanDescr) DAQmxSetChanDescr = 0;
decltype(&__1d156377_DAQmxSetChanSyncUnlockBehavior) DAQmxSetChanSyncUnlockBehavior = 0;
decltype(&__37135a77_DAQmxSetChangeDetectDIFallingEdgePhysicalChans) DAQmxSetChangeDetectDIFallingEdgePhysicalChans = 0;
decltype(&__175d507c_DAQmxSetChangeDetectDIRisingEdgePhysicalChans) DAQmxSetChangeDetectDIRisingEdgePhysicalChans = 0;
decltype(&__35255275_DAQmxSetChangeDetectDITristate) DAQmxSetChangeDetectDITristate = 0;
decltype(&__6c617208_DAQmxSetDIAcquireOn) DAQmxSetDIAcquireOn = 0;
decltype(&__0a20657b_DAQmxSetDIDataXferMech) DAQmxSetDIDataXferMech = 0;
decltype(&__7c0b1515_DAQmxSetDIDataXferReqCond) DAQmxSetDIDataXferReqCond = 0;
decltype(&__0f341765_DAQmxSetDIDigFltrEnable) DAQmxSetDIDigFltrEnable = 0;
decltype(&__1e225a48_DAQmxSetDIDigFltrEnableBusMode) DAQmxSetDIDigFltrEnableBusMode = 0;
decltype(&__2c1a707d_DAQmxSetDIDigFltrMinPulseWidth) DAQmxSetDIDigFltrMinPulseWidth = 0;
decltype(&__0879756e_DAQmxSetDIDigFltrTimebaseRate) DAQmxSetDIDigFltrTimebaseRate = 0;
decltype(&__6d786679_DAQmxSetDIDigFltrTimebaseSrc) DAQmxSetDIDigFltrTimebaseSrc = 0;
decltype(&__1e21027f_DAQmxSetDIDigSyncEnable) DAQmxSetDIDigSyncEnable = 0;
decltype(&__31576166_DAQmxSetDIInvertLines) DAQmxSetDIInvertLines = 0;
decltype(&__075f725c_DAQmxSetDILogicFamily) DAQmxSetDILogicFamily = 0;
decltype(&__3c1d796e_DAQmxSetDIMemMapEnable) DAQmxSetDIMemMapEnable = 0;
decltype(&__654d140a_DAQmxSetDITristate) DAQmxSetDITristate = 0;
decltype(&__5f3e1710_DAQmxSetDIUsbXferReqCount) DAQmxSetDIUsbXferReqCount = 0;
decltype(&__3b38181b_DAQmxSetDIUsbXferReqSize) DAQmxSetDIUsbXferReqSize = 0;
decltype(&__0a26657b_DAQmxSetDODataXferMech) DAQmxSetDODataXferMech = 0;
decltype(&__7c0d1515_DAQmxSetDODataXferReqCond) DAQmxSetDODataXferReqCond = 0;
decltype(&__625d4e73_DAQmxSetDOGenerateOn) DAQmxSetDOGenerateOn = 0;
decltype(&__31516166_DAQmxSetDOInvertLines) DAQmxSetDOInvertLines = 0;
decltype(&__05574166_DAQmxSetDOLineStatesDoneState) DAQmxSetDOLineStatesDoneState = 0;
decltype(&__233d0d70_DAQmxSetDOLineStatesPausedState) DAQmxSetDOLineStatesPausedState = 0;
decltype(&__240e5b64_DAQmxSetDOLineStatesStartState) DAQmxSetDOLineStatesStartState = 0;
decltype(&__0759725c_DAQmxSetDOLogicFamily) DAQmxSetDOLogicFamily = 0;
decltype(&__3c1b796e_DAQmxSetDOMemMapEnable) DAQmxSetDOMemMapEnable = 0;
decltype(&__480b1e1e_DAQmxSetDOOutputDriveType) DAQmxSetDOOutputDriveType = 0;
decltype(&__7f2f0f09_DAQmxSetDOOvercurrentAutoReenable) DAQmxSetDOOvercurrentAutoReenable = 0;
decltype(&__72651419_DAQmxSetDOOvercurrentLimit) DAQmxSetDOOvercurrentLimit = 0;
decltype(&__79517b0f_DAQmxSetDOOvercurrentReenablePeriod) DAQmxSetDOOvercurrentReenablePeriod = 0;
decltype(&__654b140a_DAQmxSetDOTristate) DAQmxSetDOTristate = 0;
decltype(&__5f381710_DAQmxSetDOUsbXferReqCount) DAQmxSetDOUsbXferReqCount = 0;
decltype(&__3b3e181b_DAQmxSetDOUsbXferReqSize) DAQmxSetDOUsbXferReqSize = 0;
decltype(&__7b392c21_DAQmxSetDOUseOnlyOnBrdMem) DAQmxSetDOUseOnlyOnBrdMem = 0;
decltype(&__21444b70_DAQmxSetDelayFromSampClkDelay) DAQmxSetDelayFromSampClkDelay = 0;
decltype(&__21013370_DAQmxSetDelayFromSampClkDelayEx) DAQmxSetDelayFromSampClkDelayEx = 0;
decltype(&__55622519_DAQmxSetDelayFromSampClkDelayUnits) DAQmxSetDelayFromSampClkDelayUnits = 0;
decltype(&__55626061_DAQmxSetDelayFromSampClkDelayUnitsEx) DAQmxSetDelayFromSampClkDelayUnitsEx = 0;
decltype(&__66624113_DAQmxSetDigEdgeAdvTrigDigFltrEnable) DAQmxSetDigEdgeAdvTrigDigFltrEnable = 0;
decltype(&__7668270b_DAQmxSetDigEdgeAdvTrigEdge) DAQmxSetDigEdgeAdvTrigEdge = 0;
decltype(&__720d311d_DAQmxSetDigEdgeAdvTrigSrc) DAQmxSetDigEdgeAdvTrigSrc = 0;
decltype(&__011d191e_DAQmxSetDigEdgeArmStartTrigDigFltrEnable) DAQmxSetDigEdgeArmStartTrigDigFltrEnable = 0;
decltype(&__193e3779_DAQmxSetDigEdgeArmStartTrigDigFltrMinPulseWidth) DAQmxSetDigEdgeArmStartTrigDigFltrMinPulseWidth = 0;
decltype(&__0a1a547c_DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseRate) DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseRate = 0;
decltype(&__1d7f556f_DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseSrc) DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseSrc = 0;
decltype(&__1b0c0c0b_DAQmxSetDigEdgeArmStartTrigDigSyncEnable) DAQmxSetDigEdgeArmStartTrigDigSyncEnable = 0;
decltype(&__190d1378_DAQmxSetDigEdgeArmStartTrigEdge) DAQmxSetDigEdgeArmStartTrigEdge = 0;
decltype(&__0f09766e_DAQmxSetDigEdgeArmStartTrigSrc) DAQmxSetDigEdgeArmStartTrigSrc = 0;
decltype(&__67724100_DAQmxSetDigEdgeRefTrigDigFltrEnable) DAQmxSetDigEdgeRefTrigDigFltrEnable = 0;
decltype(&__445c2618_DAQmxSetDigEdgeRefTrigDigFltrMinPulseWidth) DAQmxSetDigEdgeRefTrigDigFltrMinPulseWidth = 0;
decltype(&__603f230b_DAQmxSetDigEdgeRefTrigDigFltrTimebaseRate) DAQmxSetDigEdgeRefTrigDigFltrTimebaseRate = 0;
decltype(&__053e301c_DAQmxSetDigEdgeRefTrigDigFltrTimebaseSrc) DAQmxSetDigEdgeRefTrigDigFltrTimebaseSrc = 0;
decltype(&__7667541a_DAQmxSetDigEdgeRefTrigDigSyncEnable) DAQmxSetDigEdgeRefTrigDigSyncEnable = 0;
decltype(&__77782718_DAQmxSetDigEdgeRefTrigEdge) DAQmxSetDigEdgeRefTrigEdge = 0;
decltype(&__731d310e_DAQmxSetDigEdgeRefTrigSrc) DAQmxSetDigEdgeRefTrigSrc = 0;
decltype(&__1f735a73_DAQmxSetDigEdgeStartTrigDigFltrEnable) DAQmxSetDigEdgeStartTrigDigFltrEnable = 0;
decltype(&__786b795d_DAQmxSetDigEdgeStartTrigDigFltrMinPulseWidth) DAQmxSetDigEdgeStartTrigDigFltrMinPulseWidth = 0;
decltype(&__7d785d3e_DAQmxSetDigEdgeStartTrigDigFltrTimebaseRate) DAQmxSetDigEdgeStartTrigDigFltrTimebaseRate = 0;
decltype(&__6e6f383f_DAQmxSetDigEdgeStartTrigDigFltrTimebaseSrc) DAQmxSetDigEdgeStartTrigDigFltrTimebaseSrc = 0;
decltype(&__0a694b66_DAQmxSetDigEdgeStartTrigDigSyncEnable) DAQmxSetDigEdgeStartTrigDigSyncEnable = 0;
decltype(&__796b4a79_DAQmxSetDigEdgeStartTrigEdge) DAQmxSetDigEdgeStartTrigEdge = 0;
decltype(&__6f7d4e1c_DAQmxSetDigEdgeStartTrigSrc) DAQmxSetDigEdgeStartTrigSrc = 0;
decltype(&__70615556_DAQmxSetDigEdgeWatchdogExpirTrigEdge) DAQmxSetDigEdgeWatchdogExpirTrigEdge = 0;
decltype(&__66775133_DAQmxSetDigEdgeWatchdogExpirTrigSrc) DAQmxSetDigEdgeWatchdogExpirTrigSrc = 0;
decltype(&__757a6e43_DAQmxSetDigLvlPauseTrigDigFltrEnable) DAQmxSetDigLvlPauseTrigDigFltrEnable = 0;
decltype(&__6d594024_DAQmxSetDigLvlPauseTrigDigFltrMinPulseWidth) DAQmxSetDigLvlPauseTrigDigFltrMinPulseWidth = 0;
decltype(&__7e7d2321_DAQmxSetDigLvlPauseTrigDigFltrTimebaseRate) DAQmxSetDigLvlPauseTrigDigFltrTimebaseRate = 0;
decltype(&__69182232_DAQmxSetDigLvlPauseTrigDigFltrTimebaseSrc) DAQmxSetDigLvlPauseTrigDigFltrTimebaseSrc = 0;
decltype(&__6f6b7b56_DAQmxSetDigLvlPauseTrigDigSyncEnable) DAQmxSetDigLvlPauseTrigDigSyncEnable = 0;
decltype(&__7b6e0133_DAQmxSetDigLvlPauseTrigSrc) DAQmxSetDigLvlPauseTrigSrc = 0;
decltype(&__61686f37_DAQmxSetDigLvlPauseTrigWhen) DAQmxSetDigLvlPauseTrigWhen = 0;
decltype(&__7f61012c_DAQmxSetDigPatternPauseTrigPattern) DAQmxSetDigPatternPauseTrigPattern = 0;
decltype(&__1e18754a_DAQmxSetDigPatternPauseTrigSrc) DAQmxSetDigPatternPauseTrigSrc = 0;
decltype(&__041e1b4e_DAQmxSetDigPatternPauseTrigWhen) DAQmxSetDigPatternPauseTrigWhen = 0;
decltype(&__1e00143a_DAQmxSetDigPatternRefTrigPattern) DAQmxSetDigPatternRefTrigPattern = 0;
decltype(&__6a667543_DAQmxSetDigPatternRefTrigSrc) DAQmxSetDigPatternRefTrigSrc = 0;
decltype(&__04626f45_DAQmxSetDigPatternRefTrigWhen) DAQmxSetDigPatternRefTrigWhen = 0;
decltype(&__6b601339_DAQmxSetDigPatternStartTrigPattern) DAQmxSetDigPatternStartTrigPattern = 0;
decltype(&__0a19675f_DAQmxSetDigPatternStartTrigSrc) DAQmxSetDigPatternStartTrigSrc = 0;
decltype(&__101f095b_DAQmxSetDigPatternStartTrigWhen) DAQmxSetDigPatternStartTrigWhen = 0;
decltype(&__3a366d6d_DAQmxSetDigitalLogicFamilyPowerUpState) DAQmxSetDigitalLogicFamilyPowerUpState = 0;
decltype(&__57696c55_DAQmxSetDigitalPowerUpStates) DAQmxSetDigitalPowerUpStates = 0;
decltype(&__7f544e0f_DAQmxSetDigitalPullUpPullDownStates) DAQmxSetDigitalPullUpPullDownStates = 0;
decltype(&__6f307774_DAQmxSetExported10MHzRefClkOutputTerm) DAQmxSetExported10MHzRefClkOutputTerm = 0;
decltype(&__56500864_DAQmxSetExported20MHzTimebaseOutputTerm) DAQmxSetExported20MHzTimebaseOutputTerm = 0;
decltype(&__5a694d31_DAQmxSetExportedAIConvClkOutputTerm) DAQmxSetExportedAIConvClkOutputTerm = 0;
decltype(&__68561034_DAQmxSetExportedAIHoldCmpltEventOutputTerm) DAQmxSetExportedAIHoldCmpltEventOutputTerm = 0;
decltype(&__0d6d5a4a_DAQmxSetExportedAIHoldCmpltEventPulsePolarity) DAQmxSetExportedAIHoldCmpltEventPulsePolarity = 0;
decltype(&__77413b27_DAQmxSetExportedAdvCmpltEventDelay) DAQmxSetExportedAdvCmpltEventDelay = 0;
decltype(&__0334326b_DAQmxSetExportedAdvCmpltEventOutputTerm) DAQmxSetExportedAdvCmpltEventOutputTerm = 0;
decltype(&__7d510921_DAQmxSetExportedAdvCmpltEventPulsePolarity) DAQmxSetExportedAdvCmpltEventPulsePolarity = 0;
decltype(&__013d144e_DAQmxSetExportedAdvCmpltEventPulseWidth) DAQmxSetExportedAdvCmpltEventPulseWidth = 0;
decltype(&__5433250e_DAQmxSetExportedAdvTrigOutputTerm) DAQmxSetExportedAdvTrigOutputTerm = 0;
decltype(&__72162707_DAQmxSetExportedAdvTrigPulseWidth) DAQmxSetExportedAdvTrigPulseWidth = 0;
decltype(&__0630496e_DAQmxSetExportedAdvTrigPulseWidthUnits) DAQmxSetExportedAdvTrigPulseWidthUnits = 0;
decltype(&__7f486e23_DAQmxSetExportedChangeDetectEventOutputTerm) DAQmxSetExportedChangeDetectEventOutputTerm = 0;
decltype(&__012d5569_DAQmxSetExportedChangeDetectEventPulsePolarity) DAQmxSetExportedChangeDetectEventPulsePolarity = 0;
decltype(&__4a301e16_DAQmxSetExportedCtrOutEventOutputBehavior) DAQmxSetExportedCtrOutEventOutputBehavior = 0;
decltype(&__34507763_DAQmxSetExportedCtrOutEventOutputTerm) DAQmxSetExportedCtrOutEventOutputTerm = 0;
decltype(&__0f1a0906_DAQmxSetExportedCtrOutEventPulsePolarity) DAQmxSetExportedCtrOutEventPulsePolarity = 0;
decltype(&__4368151e_DAQmxSetExportedCtrOutEventToggleIdleState) DAQmxSetExportedCtrOutEventToggleIdleState = 0;
decltype(&__53644b26_DAQmxSetExportedDataActiveEventLvlActiveLvl) DAQmxSetExportedDataActiveEventLvlActiveLvl = 0;
decltype(&__71370524_DAQmxSetExportedDataActiveEventOutputTerm) DAQmxSetExportedDataActiveEventOutputTerm = 0;
decltype(&__18013d0f_DAQmxSetExportedDividedSampClkTimebaseOutputTerm) DAQmxSetExportedDividedSampClkTimebaseOutputTerm = 0;
decltype(&__1326497b_DAQmxSetExportedHshkEventDelay) DAQmxSetExportedHshkEventDelay = 0;
decltype(&__60381611_DAQmxSetExportedHshkEventInterlockedAssertOnStart) DAQmxSetExportedHshkEventInterlockedAssertOnStart = 0;
decltype(&__0b3a3169_DAQmxSetExportedHshkEventInterlockedAssertedLvl) DAQmxSetExportedHshkEventInterlockedAssertedLvl = 0;
decltype(&__7e2e340e_DAQmxSetExportedHshkEventInterlockedDeassertDelay) DAQmxSetExportedHshkEventInterlockedDeassertDelay = 0;
decltype(&__0e263e57_DAQmxSetExportedHshkEventOutputBehavior) DAQmxSetExportedHshkEventOutputBehavior = 0;
decltype(&__67534037_DAQmxSetExportedHshkEventOutputTerm) DAQmxSetExportedHshkEventOutputTerm = 0;
decltype(&__19367b7d_DAQmxSetExportedHshkEventPulsePolarity) DAQmxSetExportedHshkEventPulsePolarity = 0;
decltype(&__655a6612_DAQmxSetExportedHshkEventPulseWidth) DAQmxSetExportedHshkEventPulseWidth = 0;
decltype(&__0261687b_DAQmxSetExportedPauseTrigLvlActiveLvl) DAQmxSetExportedPauseTrigLvlActiveLvl = 0;
decltype(&__4c634a28_DAQmxSetExportedPauseTrigOutputTerm) DAQmxSetExportedPauseTrigOutputTerm = 0;
decltype(&__6b51624a_DAQmxSetExportedRdyForStartEventLvlActiveLvl) DAQmxSetExportedRdyForStartEventLvlActiveLvl = 0;
decltype(&__69733104_DAQmxSetExportedRdyForStartEventOutputTerm) DAQmxSetExportedRdyForStartEventOutputTerm = 0;
decltype(&__59695230_DAQmxSetExportedRdyForXferEventDeassertCond) DAQmxSetExportedRdyForXferEventDeassertCond = 0;
decltype(&__48592601_DAQmxSetExportedRdyForXferEventDeassertCondCustomThreshold) DAQmxSetExportedRdyForXferEventDeassertCondCustomThreshold = 0;
decltype(&__78676a0e_DAQmxSetExportedRdyForXferEventLvlActiveLvl) DAQmxSetExportedRdyForXferEventLvlActiveLvl = 0;
decltype(&__5a34240c_DAQmxSetExportedRdyForXferEventOutputTerm) DAQmxSetExportedRdyForXferEventOutputTerm = 0;
decltype(&__4732350e_DAQmxSetExportedRefTrigOutputTerm) DAQmxSetExportedRefTrigOutputTerm = 0;
decltype(&__7c784b6b_DAQmxSetExportedRefTrigPulsePolarity) DAQmxSetExportedRefTrigPulsePolarity = 0;
decltype(&__546d202c_DAQmxSetExportedSampClkDelayOffset) DAQmxSetExportedSampClkDelayOffset = 0;
decltype(&__09535b5f_DAQmxSetExportedSampClkOutputBehavior) DAQmxSetExportedSampClkOutputBehavior = 0;
decltype(&__7733322a_DAQmxSetExportedSampClkOutputTerm) DAQmxSetExportedSampClkOutputTerm = 0;
decltype(&__4c794c4f_DAQmxSetExportedSampClkPulsePolarity) DAQmxSetExportedSampClkPulsePolarity = 0;
decltype(&__7f2d321c_DAQmxSetExportedSampClkTimebaseOutputTerm) DAQmxSetExportedSampClkTimebaseOutputTerm = 0;
decltype(&__381d0b6a_DAQmxSetExportedSignalAttribute) DAQmxSetExportedSignalAttribute = 0;
decltype(&__5e765e29_DAQmxSetExportedStartTrigOutputTerm) DAQmxSetExportedStartTrigOutputTerm = 0;
decltype(&__20136563_DAQmxSetExportedStartTrigPulsePolarity) DAQmxSetExportedStartTrigPulsePolarity = 0;
decltype(&__23361d0b_DAQmxSetExportedSyncPulseEventOutputTerm) DAQmxSetExportedSyncPulseEventOutputTerm = 0;
decltype(&__3d0f605b_DAQmxSetExportedWatchdogExpiredEventOutputTerm) DAQmxSetExportedWatchdogExpiredEventOutputTerm = 0;
decltype(&__54792d1f_DAQmxSetFirstSampClkOffset) DAQmxSetFirstSampClkOffset = 0;
decltype(&__3c614765_DAQmxSetFirstSampClkTimescale) DAQmxSetFirstSampClkTimescale = 0;
decltype(&__29032e02_DAQmxSetFirstSampClkWhen) DAQmxSetFirstSampClkWhen = 0;
decltype(&__171d1300_DAQmxSetFirstSampTimestampEnable) DAQmxSetFirstSampTimestampEnable = 0;
decltype(&__7a767801_DAQmxSetFirstSampTimestampTimescale) DAQmxSetFirstSampTimestampTimescale = 0;
decltype(&__49450e01_DAQmxSetHshkDelayAfterXfer) DAQmxSetHshkDelayAfterXfer = 0;
decltype(&__3201037f_DAQmxSetHshkSampleInputDataWhen) DAQmxSetHshkSampleInputDataWhen = 0;
decltype(&__3756526e_DAQmxSetHshkStartCond) DAQmxSetHshkStartCond = 0;
decltype(&__746a4570_DAQmxSetHshkTrigType) DAQmxSetHshkTrigType = 0;
decltype(&__5f20290c_DAQmxSetImplicitUnderflowBehavior) DAQmxSetImplicitUnderflowBehavior = 0;
decltype(&__1c0e467c_DAQmxSetInterlockedHshkTrigAssertedLvl) DAQmxSetInterlockedHshkTrigAssertedLvl = 0;
decltype(&__1f174750_DAQmxSetInterlockedHshkTrigSrc) DAQmxSetInterlockedHshkTrigSrc = 0;
decltype(&__110b3968_DAQmxSetLoggingFilePath) DAQmxSetLoggingFilePath = 0;
decltype(&__4975466e_DAQmxSetLoggingFilePreallocationSize) DAQmxSetLoggingFilePreallocationSize = 0;
decltype(&__517f5f6f_DAQmxSetLoggingFileWriteSize) DAQmxSetLoggingFileWriteSize = 0;
decltype(&__76775133_DAQmxSetLoggingMode) DAQmxSetLoggingMode = 0;
decltype(&__7866474b_DAQmxSetLoggingPause) DAQmxSetLoggingPause = 0;
decltype(&__41775318_DAQmxSetLoggingSampsPerFile) DAQmxSetLoggingSampsPerFile = 0;
decltype(&__61507f78_DAQmxSetLoggingTDMSGroupName) DAQmxSetLoggingTDMSGroupName = 0;
decltype(&__59527a6a_DAQmxSetLoggingTDMSOperation) DAQmxSetLoggingTDMSOperation = 0;
decltype(&__7e642304_DAQmxSetMasterTimebaseRate) DAQmxSetMasterTimebaseRate = 0;
decltype(&__69012217_DAQmxSetMasterTimebaseSrc) DAQmxSetMasterTimebaseSrc = 0;
decltype(&__64424e7b_DAQmxSetOnDemandSimultaneousAOEnable) DAQmxSetOnDemandSimultaneousAOEnable = 0;
decltype(&__0b734a73_DAQmxSetPauseTrigType) DAQmxSetPauseTrigType = 0;
decltype(&__113d323d_DAQmxSetPhysicalChanAIPowerControlEnable) DAQmxSetPhysicalChanAIPowerControlEnable = 0;
decltype(&__003a4f4f_DAQmxSetPhysicalChanAIPowerControlType) DAQmxSetPhysicalChanAIPowerControlType = 0;
decltype(&__792b2c3e_DAQmxSetPhysicalChanAIPowerControlVoltage) DAQmxSetPhysicalChanAIPowerControlVoltage = 0;
decltype(&__532a0138_DAQmxSetPhysicalChanAOManualControlEnable) DAQmxSetPhysicalChanAOManualControlEnable = 0;
decltype(&__0810200d_DAQmxSetPhysicalChanName) DAQmxSetPhysicalChanName = 0;
decltype(&__2361547b_DAQmxSetReadAttribute) DAQmxSetReadAttribute = 0;
decltype(&__08764060_DAQmxSetReadAutoStart) DAQmxSetReadAutoStart = 0;
decltype(&__76710a05_DAQmxSetReadChannelsToRead) DAQmxSetReadChannelsToRead = 0;
decltype(&__4465330e_DAQmxSetReadOffset) DAQmxSetReadOffset = 0;
decltype(&__1373597b_DAQmxSetReadOverWrite) DAQmxSetReadOverWrite = 0;
decltype(&__587e5c44_DAQmxSetReadReadAllAvailSamp) DAQmxSetReadReadAllAvailSamp = 0;
decltype(&__1c144f79_DAQmxSetReadRelativeTo) DAQmxSetReadRelativeTo = 0;
decltype(&__284f5975_DAQmxSetReadSleepTime) DAQmxSetReadSleepTime = 0;
decltype(&__7479586c_DAQmxSetReadWaitMode) DAQmxSetReadWaitMode = 0;
decltype(&__77083916_DAQmxSetRealTimeAttribute) DAQmxSetRealTimeAttribute = 0;
decltype(&__3c1e111e_DAQmxSetRealTimeConvLateErrorsToWarnings) DAQmxSetRealTimeConvLateErrorsToWarnings = 0;
decltype(&__0b2c3617_DAQmxSetRealTimeNumOfWarmupIters) DAQmxSetRealTimeNumOfWarmupIters = 0;
decltype(&__3a1d0d02_DAQmxSetRealTimeReportMissedSamp) DAQmxSetRealTimeReportMissedSamp = 0;
decltype(&__4e7f3d34_DAQmxSetRealTimeWaitForNextSampClkWaitMode) DAQmxSetRealTimeWaitForNextSampClkWaitMode = 0;
decltype(&__7b053e11_DAQmxSetRealTimeWriteRecoveryMode) DAQmxSetRealTimeWriteRecoveryMode = 0;
decltype(&__7679003b_DAQmxSetRefClkRate) DAQmxSetRefClkRate = 0;
decltype(&__611c0128_DAQmxSetRefClkSrc) DAQmxSetRefClkSrc = 0;
decltype(&__10625f71_DAQmxSetRefTrigAutoTrigEnable) DAQmxSetRefTrigAutoTrigEnable = 0;
decltype(&__79725470_DAQmxSetRefTrigDelay) DAQmxSetRefTrigDelay = 0;
decltype(&__50502916_DAQmxSetRefTrigMaxNumTrigsToDetect) DAQmxSetRefTrigMaxNumTrigsToDetect = 0;
decltype(&__196c7e6b_DAQmxSetRefTrigPretrigSamples) DAQmxSetRefTrigPretrigSamples = 0;
decltype(&__49644c04_DAQmxSetRefTrigRetriggerWin) DAQmxSetRefTrigRetriggerWin = 0;
decltype(&__7f6f4e61_DAQmxSetRefTrigRetriggerable) DAQmxSetRefTrigRetriggerable = 0;
decltype(&__28195c78_DAQmxSetRefTrigTimestampEnable) DAQmxSetRefTrigTimestampEnable = 0;
decltype(&__43183113_DAQmxSetRefTrigTimestampTimescale) DAQmxSetRefTrigTimestampTimescale = 0;
decltype(&__0719524e_DAQmxSetRefTrigTrigWin) DAQmxSetRefTrigTrigWin = 0;
decltype(&__656e5019_DAQmxSetRefTrigType) DAQmxSetRefTrigType = 0;
decltype(&__4a50732f_DAQmxSetSCExpressCalAccBridgeOutput) DAQmxSetSCExpressCalAccBridgeOutput = 0;
decltype(&__4f2e3f39_DAQmxSetSampClkActiveEdge) DAQmxSetSampClkActiveEdge = 0;
decltype(&__50685d4a_DAQmxSetSampClkDigFltrEnable) DAQmxSetSampClkDigFltrEnable = 0;
decltype(&__484b732d_DAQmxSetSampClkDigFltrMinPulseWidth) DAQmxSetSampClkDigFltrMinPulseWidth = 0;
decltype(&__5b6f1028_DAQmxSetSampClkDigFltrTimebaseRate) DAQmxSetSampClkDigFltrTimebaseRate = 0;
decltype(&__4c0a113b_DAQmxSetSampClkDigFltrTimebaseSrc) DAQmxSetSampClkDigFltrTimebaseSrc = 0;
decltype(&__4a79485f_DAQmxSetSampClkDigSyncEnable) DAQmxSetSampClkDigSyncEnable = 0;
decltype(&__28077458_DAQmxSetSampClkOverrunBehavior) DAQmxSetSampClkOverrunBehavior = 0;
decltype(&__4d6b573b_DAQmxSetSampClkRate) DAQmxSetSampClkRate = 0;
decltype(&__5e7c323a_DAQmxSetSampClkSrc) DAQmxSetSampClkSrc = 0;
decltype(&__47303f0f_DAQmxSetSampClkTimebaseActiveEdge) DAQmxSetSampClkTimebaseActiveEdge = 0;
decltype(&__4d77321b_DAQmxSetSampClkTimebaseDiv) DAQmxSetSampClkTimebaseDiv = 0;
decltype(&__3700271f_DAQmxSetSampClkTimebaseMasterTimebaseDiv) DAQmxSetSampClkTimebaseMasterTimebaseDiv = 0;
decltype(&__4575570d_DAQmxSetSampClkTimebaseRate) DAQmxSetSampClkTimebaseRate = 0;
decltype(&__5662320c_DAQmxSetSampClkTimebaseSrc) DAQmxSetSampClkTimebaseSrc = 0;
decltype(&__52023f37_DAQmxSetSampClkTimingResponseMode) DAQmxSetSampClkTimingResponseMode = 0;
decltype(&__101b3f2a_DAQmxSetSampClkUnderflowBehavior) DAQmxSetSampClkUnderflowBehavior = 0;
decltype(&__3b026d6c_DAQmxSetSampClkWriteWfmUseInitialWfmDT) DAQmxSetSampClkWriteWfmUseInitialWfmDT = 0;
decltype(&__5f18360e_DAQmxSetSampQuantSampMode) DAQmxSetSampQuantSampMode = 0;
decltype(&__796d5d76_DAQmxSetSampQuantSampPerChan) DAQmxSetSampQuantSampPerChan = 0;
decltype(&__32141f0b_DAQmxSetSampTimingEngine) DAQmxSetSampTimingEngine = 0;
decltype(&__25186079_DAQmxSetSampTimingType) DAQmxSetSampTimingType = 0;
decltype(&__0c3c4374_DAQmxSetScaleAttribute) DAQmxSetScaleAttribute = 0;
decltype(&__69473006_DAQmxSetScaleDescr) DAQmxSetScaleDescr = 0;
decltype(&__3c51536b_DAQmxSetScaleLinSlope) DAQmxSetScaleLinSlope = 0;
decltype(&__4672310a_DAQmxSetScaleLinYIntercept) DAQmxSetScaleLinYIntercept = 0;
decltype(&__5d625c4b_DAQmxSetScaleMapPreScaledMax) DAQmxSetScaleMapPreScaledMax = 0;
decltype(&__5d62545d_DAQmxSetScaleMapPreScaledMin) DAQmxSetScaleMapPreScaledMin = 0;
decltype(&__443b1808_DAQmxSetScaleMapScaledMax) DAQmxSetScaleMapScaledMax = 0;
decltype(&__523b1800_DAQmxSetScaleMapScaledMin) DAQmxSetScaleMapScaledMin = 0;
decltype(&__21694269_DAQmxSetScalePolyForwardCoeff) DAQmxSetScalePolyForwardCoeff = 0;
decltype(&__336e496c_DAQmxSetScalePolyReverseCoeff) DAQmxSetScalePolyReverseCoeff = 0;
decltype(&__55526012_DAQmxSetScalePreScaledUnits) DAQmxSetScalePreScaledUnits = 0;
decltype(&__082e2632_DAQmxSetScaleScaledUnits) DAQmxSetScaleScaledUnits = 0;
decltype(&__0e1a1052_DAQmxSetScaleTablePreScaledVals) DAQmxSetScaleTablePreScaledVals = 0;
decltype(&__514d6e63_DAQmxSetScaleTableScaledVals) DAQmxSetScaleTableScaledVals = 0;
decltype(&__1d0f426e_DAQmxSetStartTrigDelay) DAQmxSetStartTrigDelay = 0;
decltype(&__747b6400_DAQmxSetStartTrigDelayUnits) DAQmxSetStartTrigDelayUnits = 0;
decltype(&__60696b4c_DAQmxSetStartTrigMaxNumTrigsToDetect) DAQmxSetStartTrigMaxNumTrigsToDetect = 0;
decltype(&__057b7278_DAQmxSetStartTrigRetriggerWin) DAQmxSetStartTrigRetriggerWin = 0;
decltype(&__071e4473_DAQmxSetStartTrigRetriggerable) DAQmxSetStartTrigRetriggerable = 0;
decltype(&__75702d0e_DAQmxSetStartTrigTimescale) DAQmxSetStartTrigTimescale = 0;
decltype(&__15071305_DAQmxSetStartTrigTimestampEnable) DAQmxSetStartTrigTimestampEnable = 0;
decltype(&__786c7804_DAQmxSetStartTrigTimestampTimescale) DAQmxSetStartTrigTimestampTimescale = 0;
decltype(&__75313d0e_DAQmxSetStartTrigTrigWhen) DAQmxSetStartTrigTrigWhen = 0;
decltype(&__1b313c05_DAQmxSetStartTrigTrigWin) DAQmxSetStartTrigTrigWin = 0;
decltype(&__19665e72_DAQmxSetStartTrigType) DAQmxSetStartTrigType = 0;
decltype(&__29357a7a_DAQmxSetSwitchChanAnlgBusSharingEnable) DAQmxSetSwitchChanAnlgBusSharingEnable = 0;
decltype(&__6c655313_DAQmxSetSwitchChanAttribute) DAQmxSetSwitchChanAttribute = 0;
decltype(&__0c042e76_DAQmxSetSwitchChanUsage) DAQmxSetSwitchChanUsage = 0;
decltype(&__1c0c180d_DAQmxSetSwitchDevAutoConnAnlgBus) DAQmxSetSwitchDevAutoConnAnlgBus = 0;
decltype(&__2200043b_DAQmxSetSwitchDevPwrDownLatchRelaysAfterSettling) DAQmxSetSwitchDevPwrDownLatchRelaysAfterSettling = 0;
decltype(&__0c667c7f_DAQmxSetSwitchDevSettlingTime) DAQmxSetSwitchDevSettlingTime = 0;
decltype(&__37727b6b_DAQmxSetSwitchDeviceAttribute) DAQmxSetSwitchDeviceAttribute = 0;
decltype(&__6c654318_DAQmxSetSwitchScanAttribute) DAQmxSetSwitchScanAttribute = 0;
decltype(&__67664231_DAQmxSetSwitchScanBreakMode) DAQmxSetSwitchScanBreakMode = 0;
decltype(&__5069597a_DAQmxSetSwitchScanRepeatMode) DAQmxSetSwitchScanRepeatMode = 0;
decltype(&__34123841_DAQmxSetSyncClkInterval) DAQmxSetSyncClkInterval = 0;
decltype(&__13032e21_DAQmxSetSyncPulseMinDelayToStart) DAQmxSetSyncPulseMinDelayToStart = 0;
decltype(&__53596e1f_DAQmxSetSyncPulseResetDelay) DAQmxSetSyncPulseResetDelay = 0;
decltype(&__5a4d446a_DAQmxSetSyncPulseSrc) DAQmxSetSyncPulseSrc = 0;
decltype(&__36085568_DAQmxSetSyncPulseTimeTimescale) DAQmxSetSyncPulseTimeTimescale = 0;
decltype(&__511d3701_DAQmxSetSyncPulseTimeWhen) DAQmxSetSyncPulseTimeWhen = 0;
decltype(&__3f4a4f79_DAQmxSetSyncPulseType) DAQmxSetSyncPulseType = 0;
decltype(&__071a1466_DAQmxSetTimingAttribute) DAQmxSetTimingAttribute = 0;
decltype(&__7f1a1423_DAQmxSetTimingAttributeEx) DAQmxSetTimingAttributeEx = 0;
decltype(&__25765c78_DAQmxSetTrigAttribute) DAQmxSetTrigAttribute = 0;
decltype(&__0f1b2979_DAQmxSetTriggerSyncType) DAQmxSetTriggerSyncType = 0;
decltype(&__46506c53_DAQmxSetWatchdogAOExpirState) DAQmxSetWatchdogAOExpirState = 0;
decltype(&__62516579_DAQmxSetWatchdogAOOutputType) DAQmxSetWatchdogAOOutputType = 0;
decltype(&__4e012e1b_DAQmxSetWatchdogAttribute) DAQmxSetWatchdogAttribute = 0;
decltype(&__44506c53_DAQmxSetWatchdogCOExpirState) DAQmxSetWatchdogCOExpirState = 0;
decltype(&__43506c53_DAQmxSetWatchdogDOExpirState) DAQmxSetWatchdogDOExpirState = 0;
decltype(&__3e224955_DAQmxSetWatchdogExpirTrigTrigOnNetworkConnLoss) DAQmxSetWatchdogExpirTrigTrigOnNetworkConnLoss = 0;
decltype(&__366f546d_DAQmxSetWatchdogExpirTrigType) DAQmxSetWatchdogExpirTrigType = 0;
decltype(&__380b3678_DAQmxSetWatchdogTimeout) DAQmxSetWatchdogTimeout = 0;
decltype(&__082d4b6c_DAQmxSetWriteAttribute) DAQmxSetWriteAttribute = 0;
decltype(&__7d4a4f0b_DAQmxSetWriteOffset) DAQmxSetWriteOffset = 0;
decltype(&__0f397565_DAQmxSetWriteRegenMode) DAQmxSetWriteRegenMode = 0;
decltype(&__0a123e77_DAQmxSetWriteRelativeTo) DAQmxSetWriteRelativeTo = 0;
decltype(&__06266561_DAQmxSetWriteSleepTime) DAQmxSetWriteSleepTime = 0;
decltype(&__1f7a5360_DAQmxSetWriteWaitMode) DAQmxSetWriteWaitMode = 0;
decltype(&__15504649_DAQmxSetup1102Cal) DAQmxSetup1102Cal = 0;
decltype(&__15564649_DAQmxSetup1104Cal) DAQmxSetup1104Cal = 0;
decltype(&__14504649_DAQmxSetup1112Cal) DAQmxSetup1112Cal = 0;
decltype(&__17504649_DAQmxSetup1122Cal) DAQmxSetup1122Cal = 0;
decltype(&__17564649_DAQmxSetup1124Cal) DAQmxSetup1124Cal = 0;
decltype(&__17574649_DAQmxSetup1125Cal) DAQmxSetup1125Cal = 0;
decltype(&__17544649_DAQmxSetup1126Cal) DAQmxSetup1126Cal = 0;
decltype(&__11534649_DAQmxSetup1141Cal) DAQmxSetup1141Cal = 0;
decltype(&__11504649_DAQmxSetup1142Cal) DAQmxSetup1142Cal = 0;
decltype(&__11514649_DAQmxSetup1143Cal) DAQmxSetup1143Cal = 0;
decltype(&__1e3e306b_DAQmxSetup11605Cal) DAQmxSetup11605Cal = 0;
decltype(&__1e3d316b_DAQmxSetup11634Cal) DAQmxSetup11634Cal = 0;
decltype(&__1e3d326b_DAQmxSetup11637Cal) DAQmxSetup11637Cal = 0;
decltype(&__1550464d_DAQmxSetup1502Cal) DAQmxSetup1502Cal = 0;
decltype(&__1551464d_DAQmxSetup1503Cal) DAQmxSetup1503Cal = 0;
decltype(&__1752464d_DAQmxSetup1520Cal) DAQmxSetup1520Cal = 0;
decltype(&__1753464d_DAQmxSetup1521Cal) DAQmxSetup1521Cal = 0;
decltype(&__161a464d_DAQmxSetup153xCal) DAQmxSetup153xCal = 0;
decltype(&__1152464d_DAQmxSetup1540Cal) DAQmxSetup1540Cal = 0;
decltype(&__1550434b_DAQmxSetup4302Cal) DAQmxSetup4302Cal = 0;
decltype(&__1551434b_DAQmxSetup4303Cal) DAQmxSetup4303Cal = 0;
decltype(&__1556434b_DAQmxSetup4304Cal) DAQmxSetup4304Cal = 0;
decltype(&__1557434b_DAQmxSetup4305Cal) DAQmxSetup4305Cal = 0;
decltype(&__1750434b_DAQmxSetup4322Cal) DAQmxSetup4322Cal = 0;
decltype(&__165b434b_DAQmxSetup4339Cal) DAQmxSetup4339Cal = 0;
decltype(&__161a434b_DAQmxSetup433xCal) DAQmxSetup433xCal = 0;
decltype(&__1351434c_DAQmxSetup4463Cal) DAQmxSetup4463Cal = 0;
decltype(&__1d52434c_DAQmxSetup4480Cal) DAQmxSetup4480Cal = 0;
decltype(&__145a4e4a_DAQmxSetup9218Cal) DAQmxSetup9218Cal = 0;
decltype(&__145b4e4a_DAQmxSetup9219Cal) DAQmxSetup9219Cal = 0;
decltype(&__11504e4a_DAQmxSetup9242Cal) DAQmxSetup9242Cal = 0;
decltype(&__11564e4a_DAQmxSetup9244Cal) DAQmxSetup9244Cal = 0;
decltype(&__13524e4a_DAQmxSetup9260Cal) DAQmxSetup9260Cal = 0;
decltype(&__13504e4a_DAQmxSetup9262Cal) DAQmxSetup9262Cal = 0;
decltype(&__13514e4a_DAQmxSetup9263Cal) DAQmxSetup9263Cal = 0;
decltype(&__13564e4a_DAQmxSetup9264Cal) DAQmxSetup9264Cal = 0;
decltype(&__13574e4a_DAQmxSetup9265Cal) DAQmxSetup9265Cal = 0;
decltype(&__13544e4a_DAQmxSetup9266Cal) DAQmxSetup9266Cal = 0;
decltype(&__135b4e4a_DAQmxSetup9269Cal) DAQmxSetup9269Cal = 0;
decltype(&__383b2060_DAQmxSetup9628AOCal) DAQmxSetup9628AOCal = 0;
decltype(&__383a2060_DAQmxSetup9629AOCal) DAQmxSetup9629AOCal = 0;
decltype(&__393b2060_DAQmxSetup9638AOCal) DAQmxSetup9638AOCal = 0;
decltype(&__61513f3f_DAQmxSetupDSAAOTimebaseCal) DAQmxSetupDSAAOTimebaseCal = 0;
decltype(&__5c200205_DAQmxStartNewFile) DAQmxStartNewFile = 0;
decltype(&__3d0d716d_DAQmxStartTask) DAQmxStartTask = 0;
decltype(&__27464471_DAQmxStopTask) DAQmxStopTask = 0;
decltype(&__0f085143_DAQmxSwitchCloseRelays) DAQmxSwitchCloseRelays = 0;
decltype(&__446b2022_DAQmxSwitchConnect) DAQmxSwitchConnect = 0;
decltype(&__281f0457_DAQmxSwitchConnectMulti) DAQmxSwitchConnectMulti = 0;
decltype(&__450b2521_DAQmxSwitchCreateScanList) DAQmxSwitchCreateScanList = 0;
decltype(&__3b6c484c_DAQmxSwitchDisconnect) DAQmxSwitchDisconnect = 0;
decltype(&__3b2d2420_DAQmxSwitchDisconnectAll) DAQmxSwitchDisconnectAll = 0;
decltype(&__4f483d20_DAQmxSwitchDisconnectMulti) DAQmxSwitchDisconnectMulti = 0;
decltype(&__406b4212_DAQmxSwitchFindPath) DAQmxSwitchFindPath = 0;
decltype(&__13727e73_DAQmxSwitchGetMultiRelayCount) DAQmxSwitchGetMultiRelayCount = 0;
decltype(&__7472781d_DAQmxSwitchGetMultiRelayPos) DAQmxSwitchGetMultiRelayPos = 0;
decltype(&__0630725b_DAQmxSwitchGetSingleRelayCount) DAQmxSwitchGetSingleRelayCount = 0;
decltype(&__6857725d_DAQmxSwitchGetSingleRelayPos) DAQmxSwitchGetSingleRelayPos = 0;
decltype(&__2e784160_DAQmxSwitchOpenRelays) DAQmxSwitchOpenRelays = 0;
decltype(&__3b35524a_DAQmxSwitchSetTopologyAndReset) DAQmxSwitchSetTopologyAndReset = 0;
decltype(&__5c790519_DAQmxSwitchWaitForSettling) DAQmxSwitchWaitForSettling = 0;
decltype(&__2324301c_DAQmxTaskControl) DAQmxTaskControl = 0;
decltype(&__3f292e50_DAQmxTristateOutputTerm) DAQmxTristateOutputTerm = 0;
decltype(&__59527b0c_DAQmxUnreserveNetworkDevice) DAQmxUnreserveNetworkDevice = 0;
decltype(&__5652621e_DAQmxWaitForNextSampleClock) DAQmxWaitForNextSampleClock = 0;
decltype(&__72662e13_DAQmxWaitForValidTimestamp) DAQmxWaitForValidTimestamp = 0;
decltype(&__3c214e7e_DAQmxWaitUntilTaskDone) DAQmxWaitUntilTaskDone = 0;
decltype(&__6f29390d_DAQmxWriteAnalogF64) DAQmxWriteAnalogF64 = 0;
decltype(&__2f0e2a57_DAQmxWriteAnalogScalarF64) DAQmxWriteAnalogScalarF64 = 0;
decltype(&__6f232514_DAQmxWriteBinaryI16) DAQmxWriteBinaryI16 = 0;
decltype(&__6f212114_DAQmxWriteBinaryI32) DAQmxWriteBinaryI32 = 0;
decltype(&__73232514_DAQmxWriteBinaryU16) DAQmxWriteBinaryU16 = 0;
decltype(&__73212114_DAQmxWriteBinaryU32) DAQmxWriteBinaryU32 = 0;
decltype(&__4b351215_DAQmxWriteCtrFreq) DAQmxWriteCtrFreq = 0;
decltype(&__27070374_DAQmxWriteCtrFreqScalar) DAQmxWriteCtrFreqScalar = 0;
decltype(&__51540913_DAQmxWriteCtrTicks) DAQmxWriteCtrTicks = 0;
decltype(&__30383b02_DAQmxWriteCtrTicksScalar) DAQmxWriteCtrTicksScalar = 0;
decltype(&__5f27091d_DAQmxWriteCtrTime) DAQmxWriteCtrTime = 0;
decltype(&__3315187c_DAQmxWriteCtrTimeScalar) DAQmxWriteCtrTimeScalar = 0;
decltype(&__26257a62_DAQmxWriteDigitalLines) DAQmxWriteDigitalLines = 0;
decltype(&__1c1a0238_DAQmxWriteDigitalScalarU32) DAQmxWriteDigitalScalarU32 = 0;
decltype(&__434f223a_DAQmxWriteDigitalU16) DAQmxWriteDigitalU16 = 0;
decltype(&__434f203e_DAQmxWriteDigitalU32) DAQmxWriteDigitalU32 = 0;
decltype(&__434f2b0c_DAQmxWriteDigitalU8) DAQmxWriteDigitalU8 = 0;
decltype(&__3f737165_DAQmxWriteRaw) DAQmxWriteRaw = 0;
decltype(&__62362e34_DAQmxWriteToTEDSFromArray) DAQmxWriteToTEDSFromArray = 0;
decltype(&__1c2d3030_DAQmxWriteToTEDSFromFile) DAQmxWriteToTEDSFromFile = 0;
decltype(&__1d7b7e49_DAQmxXSeriesCalAdjust) DAQmxXSeriesCalAdjust = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "DAQmxAOSeriesCalAdjust", (void**)&DAQmxAOSeriesCalAdjust, Dylib::Import::cMangled },
  { "DAQmxAddCDAQSyncConnection", (void**)&DAQmxAddCDAQSyncConnection, Dylib::Import::cMangled },
  { "DAQmxAddGlobalChansToTask", (void**)&DAQmxAddGlobalChansToTask, Dylib::Import::cMangled },
  { "DAQmxAddNetworkDevice", (void**)&DAQmxAddNetworkDevice, Dylib::Import::cMangled },
  { "DAQmxAdjust1102Cal", (void**)&DAQmxAdjust1102Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1104Cal", (void**)&DAQmxAdjust1104Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1112Cal", (void**)&DAQmxAdjust1112Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1122Cal", (void**)&DAQmxAdjust1122Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1124Cal", (void**)&DAQmxAdjust1124Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1125Cal", (void**)&DAQmxAdjust1125Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1126Cal", (void**)&DAQmxAdjust1126Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1141Cal", (void**)&DAQmxAdjust1141Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1142Cal", (void**)&DAQmxAdjust1142Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1143Cal", (void**)&DAQmxAdjust1143Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11601Cal", (void**)&DAQmxAdjust11601Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11603Cal", (void**)&DAQmxAdjust11603Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11605Cal", (void**)&DAQmxAdjust11605Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11613Cal", (void**)&DAQmxAdjust11613Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11614Cal", (void**)&DAQmxAdjust11614Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11634Cal", (void**)&DAQmxAdjust11634Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust11637Cal", (void**)&DAQmxAdjust11637Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1502Cal", (void**)&DAQmxAdjust1502Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1503Cal", (void**)&DAQmxAdjust1503Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1503CurrentCal", (void**)&DAQmxAdjust1503CurrentCal, Dylib::Import::cMangled },
  { "DAQmxAdjust1520Cal", (void**)&DAQmxAdjust1520Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust1521Cal", (void**)&DAQmxAdjust1521Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust153xCal", (void**)&DAQmxAdjust153xCal, Dylib::Import::cMangled },
  { "DAQmxAdjust1540Cal", (void**)&DAQmxAdjust1540Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4204Cal", (void**)&DAQmxAdjust4204Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4220Cal", (void**)&DAQmxAdjust4220Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4224Cal", (void**)&DAQmxAdjust4224Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4225Cal", (void**)&DAQmxAdjust4225Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4300Cal", (void**)&DAQmxAdjust4300Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4302Cal", (void**)&DAQmxAdjust4302Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4303Cal", (void**)&DAQmxAdjust4303Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4304Cal", (void**)&DAQmxAdjust4304Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4305Cal", (void**)&DAQmxAdjust4305Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4309Cal", (void**)&DAQmxAdjust4309Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4310Cal", (void**)&DAQmxAdjust4310Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4322Cal", (void**)&DAQmxAdjust4322Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4339Cal", (void**)&DAQmxAdjust4339Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust433xCal", (void**)&DAQmxAdjust433xCal, Dylib::Import::cMangled },
  { "DAQmxAdjust4353Cal", (void**)&DAQmxAdjust4353Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4357Cal", (void**)&DAQmxAdjust4357Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4463Cal", (void**)&DAQmxAdjust4463Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust4610Cal", (void**)&DAQmxAdjust4610Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9201Cal", (void**)&DAQmxAdjust9201Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9202Cal", (void**)&DAQmxAdjust9202Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9203GainCal", (void**)&DAQmxAdjust9203GainCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9203OffsetCal", (void**)&DAQmxAdjust9203OffsetCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9205Cal", (void**)&DAQmxAdjust9205Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9206Cal", (void**)&DAQmxAdjust9206Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9207GainCal", (void**)&DAQmxAdjust9207GainCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9207OffsetCal", (void**)&DAQmxAdjust9207OffsetCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9208GainCal", (void**)&DAQmxAdjust9208GainCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9208OffsetCal", (void**)&DAQmxAdjust9208OffsetCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9209GainCal", (void**)&DAQmxAdjust9209GainCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9209OffsetCal", (void**)&DAQmxAdjust9209OffsetCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9210Cal", (void**)&DAQmxAdjust9210Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9211Cal", (void**)&DAQmxAdjust9211Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9212Cal", (void**)&DAQmxAdjust9212Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9213Cal", (void**)&DAQmxAdjust9213Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9214Cal", (void**)&DAQmxAdjust9214Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9215Cal", (void**)&DAQmxAdjust9215Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9216Cal", (void**)&DAQmxAdjust9216Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9217Cal", (void**)&DAQmxAdjust9217Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9218Cal", (void**)&DAQmxAdjust9218Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9219Cal", (void**)&DAQmxAdjust9219Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9220Cal", (void**)&DAQmxAdjust9220Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9221Cal", (void**)&DAQmxAdjust9221Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9222Cal", (void**)&DAQmxAdjust9222Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9223Cal", (void**)&DAQmxAdjust9223Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9224Cal", (void**)&DAQmxAdjust9224Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9225Cal", (void**)&DAQmxAdjust9225Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9226Cal", (void**)&DAQmxAdjust9226Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9227Cal", (void**)&DAQmxAdjust9227Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9228Cal", (void**)&DAQmxAdjust9228Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9229Cal", (void**)&DAQmxAdjust9229Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9230Cal", (void**)&DAQmxAdjust9230Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9231Cal", (void**)&DAQmxAdjust9231Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9232Cal", (void**)&DAQmxAdjust9232Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9234GainCal", (void**)&DAQmxAdjust9234GainCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9234OffsetCal", (void**)&DAQmxAdjust9234OffsetCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9238Cal", (void**)&DAQmxAdjust9238Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9239Cal", (void**)&DAQmxAdjust9239Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9242Cal", (void**)&DAQmxAdjust9242Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9244Cal", (void**)&DAQmxAdjust9244Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9246Cal", (void**)&DAQmxAdjust9246Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9247Cal", (void**)&DAQmxAdjust9247Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9250Cal", (void**)&DAQmxAdjust9250Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9251Cal", (void**)&DAQmxAdjust9251Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9252Cal", (void**)&DAQmxAdjust9252Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9253Cal", (void**)&DAQmxAdjust9253Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9260Cal", (void**)&DAQmxAdjust9260Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9262Cal", (void**)&DAQmxAdjust9262Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9263Cal", (void**)&DAQmxAdjust9263Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9264Cal", (void**)&DAQmxAdjust9264Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9265Cal", (void**)&DAQmxAdjust9265Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9266Cal", (void**)&DAQmxAdjust9266Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9269Cal", (void**)&DAQmxAdjust9269Cal, Dylib::Import::cMangled },
  { "DAQmxAdjust9628AICal", (void**)&DAQmxAdjust9628AICal, Dylib::Import::cMangled },
  { "DAQmxAdjust9628AOCal", (void**)&DAQmxAdjust9628AOCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9629AICal", (void**)&DAQmxAdjust9629AICal, Dylib::Import::cMangled },
  { "DAQmxAdjust9629AOCal", (void**)&DAQmxAdjust9629AOCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9638AICal", (void**)&DAQmxAdjust9638AICal, Dylib::Import::cMangled },
  { "DAQmxAdjust9638AOCal", (void**)&DAQmxAdjust9638AOCal, Dylib::Import::cMangled },
  { "DAQmxAdjust9775Cal", (void**)&DAQmxAdjust9775Cal, Dylib::Import::cMangled },
  { "DAQmxAdjustDSAAICal", (void**)&DAQmxAdjustDSAAICal, Dylib::Import::cMangled },
  { "DAQmxAdjustDSAAICalEx", (void**)&DAQmxAdjustDSAAICalEx, Dylib::Import::cMangled },
  { "DAQmxAdjustDSAAICalWithGainAndCoupling", (void**)&DAQmxAdjustDSAAICalWithGainAndCoupling, Dylib::Import::cMangled },
  { "DAQmxAdjustDSAAOCal", (void**)&DAQmxAdjustDSAAOCal, Dylib::Import::cMangled },
  { "DAQmxAdjustDSAAOTimebaseCal", (void**)&DAQmxAdjustDSAAOTimebaseCal, Dylib::Import::cMangled },
  { "DAQmxAdjustDSATimebaseCal", (void**)&DAQmxAdjustDSATimebaseCal, Dylib::Import::cMangled },
  { "DAQmxAdjustTIOTimebaseCal", (void**)&DAQmxAdjustTIOTimebaseCal, Dylib::Import::cMangled },
  { "DAQmxAreConfiguredCDAQSyncPortsDisconnected", (void**)&DAQmxAreConfiguredCDAQSyncPortsDisconnected, Dylib::Import::cMangled },
  { "DAQmxAutoConfigureCDAQSyncConnections", (void**)&DAQmxAutoConfigureCDAQSyncConnections, Dylib::Import::cMangled },
  { "DAQmxCSeriesSetCalTemp", (void**)&DAQmxCSeriesSetCalTemp, Dylib::Import::cMangled },
  { "DAQmxCalculateReversePolyCoeff", (void**)&DAQmxCalculateReversePolyCoeff, Dylib::Import::cMangled },
  { "DAQmxCfgAnlgEdgeRefTrig", (void**)&DAQmxCfgAnlgEdgeRefTrig, Dylib::Import::cMangled },
  { "DAQmxCfgAnlgEdgeStartTrig", (void**)&DAQmxCfgAnlgEdgeStartTrig, Dylib::Import::cMangled },
  { "DAQmxCfgAnlgMultiEdgeRefTrig", (void**)&DAQmxCfgAnlgMultiEdgeRefTrig, Dylib::Import::cMangled },
  { "DAQmxCfgAnlgMultiEdgeStartTrig", (void**)&DAQmxCfgAnlgMultiEdgeStartTrig, Dylib::Import::cMangled },
  { "DAQmxCfgAnlgWindowRefTrig", (void**)&DAQmxCfgAnlgWindowRefTrig, Dylib::Import::cMangled },
  { "DAQmxCfgAnlgWindowStartTrig", (void**)&DAQmxCfgAnlgWindowStartTrig, Dylib::Import::cMangled },
  { "DAQmxCfgBurstHandshakingTimingExportClock", (void**)&DAQmxCfgBurstHandshakingTimingExportClock, Dylib::Import::cMangled },
  { "DAQmxCfgBurstHandshakingTimingImportClock", (void**)&DAQmxCfgBurstHandshakingTimingImportClock, Dylib::Import::cMangled },
  { "DAQmxCfgChangeDetectionTiming", (void**)&DAQmxCfgChangeDetectionTiming, Dylib::Import::cMangled },
  { "DAQmxCfgDigEdgeAdvTrig", (void**)&DAQmxCfgDigEdgeAdvTrig, Dylib::Import::cMangled },
  { "DAQmxCfgDigEdgeRefTrig", (void**)&DAQmxCfgDigEdgeRefTrig, Dylib::Import::cMangled },
  { "DAQmxCfgDigEdgeStartTrig", (void**)&DAQmxCfgDigEdgeStartTrig, Dylib::Import::cMangled },
  { "DAQmxCfgDigPatternRefTrig", (void**)&DAQmxCfgDigPatternRefTrig, Dylib::Import::cMangled },
  { "DAQmxCfgDigPatternStartTrig", (void**)&DAQmxCfgDigPatternStartTrig, Dylib::Import::cMangled },
  { "DAQmxCfgHandshakingTiming", (void**)&DAQmxCfgHandshakingTiming, Dylib::Import::cMangled },
  { "DAQmxCfgImplicitTiming", (void**)&DAQmxCfgImplicitTiming, Dylib::Import::cMangled },
  { "DAQmxCfgInputBuffer", (void**)&DAQmxCfgInputBuffer, Dylib::Import::cMangled },
  { "DAQmxCfgOutputBuffer", (void**)&DAQmxCfgOutputBuffer, Dylib::Import::cMangled },
  { "DAQmxCfgPipelinedSampClkTiming", (void**)&DAQmxCfgPipelinedSampClkTiming, Dylib::Import::cMangled },
  { "DAQmxCfgSampClkTiming", (void**)&DAQmxCfgSampClkTiming, Dylib::Import::cMangled },
  { "DAQmxCfgTimeStartTrig", (void**)&DAQmxCfgTimeStartTrig, Dylib::Import::cMangled },
  { "DAQmxCfgWatchdogAOExpirStates", (void**)&DAQmxCfgWatchdogAOExpirStates, Dylib::Import::cMangled },
  { "DAQmxCfgWatchdogCOExpirStates", (void**)&DAQmxCfgWatchdogCOExpirStates, Dylib::Import::cMangled },
  { "DAQmxCfgWatchdogDOExpirStates", (void**)&DAQmxCfgWatchdogDOExpirStates, Dylib::Import::cMangled },
  { "DAQmxChangeExtCalPassword", (void**)&DAQmxChangeExtCalPassword, Dylib::Import::cMangled },
  { "DAQmxClearTEDS", (void**)&DAQmxClearTEDS, Dylib::Import::cMangled },
  { "DAQmxClearTask", (void**)&DAQmxClearTask, Dylib::Import::cMangled },
  { "DAQmxCloseExtCal", (void**)&DAQmxCloseExtCal, Dylib::Import::cMangled },
  { "DAQmxConfigureLogging", (void**)&DAQmxConfigureLogging, Dylib::Import::cMangled },
  { "DAQmxConfigureTEDS", (void**)&DAQmxConfigureTEDS, Dylib::Import::cMangled },
  { "DAQmxConnectSCExpressCalAccChans", (void**)&DAQmxConnectSCExpressCalAccChans, Dylib::Import::cMangled },
  { "DAQmxConnectTerms", (void**)&DAQmxConnectTerms, Dylib::Import::cMangled },
  { "DAQmxControlWatchdogTask", (void**)&DAQmxControlWatchdogTask, Dylib::Import::cMangled },
  { "DAQmxCreateAIAccel4WireDCVoltageChan", (void**)&DAQmxCreateAIAccel4WireDCVoltageChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIAccelChan", (void**)&DAQmxCreateAIAccelChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIAccelChargeChan", (void**)&DAQmxCreateAIAccelChargeChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIBridgeChan", (void**)&DAQmxCreateAIBridgeChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIChargeChan", (void**)&DAQmxCreateAIChargeChan, Dylib::Import::cMangled },
  { "DAQmxCreateAICurrentChan", (void**)&DAQmxCreateAICurrentChan, Dylib::Import::cMangled },
  { "DAQmxCreateAICurrentRMSChan", (void**)&DAQmxCreateAICurrentRMSChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIDeviceTempChan", (void**)&DAQmxCreateAIDeviceTempChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIForceBridgePolynomialChan", (void**)&DAQmxCreateAIForceBridgePolynomialChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIForceBridgeTableChan", (void**)&DAQmxCreateAIForceBridgeTableChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIForceBridgeTwoPointLinChan", (void**)&DAQmxCreateAIForceBridgeTwoPointLinChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIForceIEPEChan", (void**)&DAQmxCreateAIForceIEPEChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIFreqVoltageChan", (void**)&DAQmxCreateAIFreqVoltageChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIMicrophoneChan", (void**)&DAQmxCreateAIMicrophoneChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIPosEddyCurrProxProbeChan", (void**)&DAQmxCreateAIPosEddyCurrProxProbeChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIPosLVDTChan", (void**)&DAQmxCreateAIPosLVDTChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIPosRVDTChan", (void**)&DAQmxCreateAIPosRVDTChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIPressureBridgePolynomialChan", (void**)&DAQmxCreateAIPressureBridgePolynomialChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIPressureBridgeTableChan", (void**)&DAQmxCreateAIPressureBridgeTableChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIPressureBridgeTwoPointLinChan", (void**)&DAQmxCreateAIPressureBridgeTwoPointLinChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIRTDChan", (void**)&DAQmxCreateAIRTDChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIResistanceChan", (void**)&DAQmxCreateAIResistanceChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIRosetteStrainGageChan", (void**)&DAQmxCreateAIRosetteStrainGageChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIStrainGageChan", (void**)&DAQmxCreateAIStrainGageChan, Dylib::Import::cMangled },
  { "DAQmxCreateAITempBuiltInSensorChan", (void**)&DAQmxCreateAITempBuiltInSensorChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIThrmcplChan", (void**)&DAQmxCreateAIThrmcplChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIThrmstrChanIex", (void**)&DAQmxCreateAIThrmstrChanIex, Dylib::Import::cMangled },
  { "DAQmxCreateAIThrmstrChanVex", (void**)&DAQmxCreateAIThrmstrChanVex, Dylib::Import::cMangled },
  { "DAQmxCreateAITorqueBridgePolynomialChan", (void**)&DAQmxCreateAITorqueBridgePolynomialChan, Dylib::Import::cMangled },
  { "DAQmxCreateAITorqueBridgeTableChan", (void**)&DAQmxCreateAITorqueBridgeTableChan, Dylib::Import::cMangled },
  { "DAQmxCreateAITorqueBridgeTwoPointLinChan", (void**)&DAQmxCreateAITorqueBridgeTwoPointLinChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIVelocityIEPEChan", (void**)&DAQmxCreateAIVelocityIEPEChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIVoltageChan", (void**)&DAQmxCreateAIVoltageChan, Dylib::Import::cMangled },
  { "DAQmxCreateAIVoltageChanWithExcit", (void**)&DAQmxCreateAIVoltageChanWithExcit, Dylib::Import::cMangled },
  { "DAQmxCreateAIVoltageRMSChan", (void**)&DAQmxCreateAIVoltageRMSChan, Dylib::Import::cMangled },
  { "DAQmxCreateAOCurrentChan", (void**)&DAQmxCreateAOCurrentChan, Dylib::Import::cMangled },
  { "DAQmxCreateAOFuncGenChan", (void**)&DAQmxCreateAOFuncGenChan, Dylib::Import::cMangled },
  { "DAQmxCreateAOVoltageChan", (void**)&DAQmxCreateAOVoltageChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIAngEncoderChan", (void**)&DAQmxCreateCIAngEncoderChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIAngVelocityChan", (void**)&DAQmxCreateCIAngVelocityChan, Dylib::Import::cMangled },
  { "DAQmxCreateCICountEdgesChan", (void**)&DAQmxCreateCICountEdgesChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIDutyCycleChan", (void**)&DAQmxCreateCIDutyCycleChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIFreqChan", (void**)&DAQmxCreateCIFreqChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIGPSTimestampChan", (void**)&DAQmxCreateCIGPSTimestampChan, Dylib::Import::cMangled },
  { "DAQmxCreateCILinEncoderChan", (void**)&DAQmxCreateCILinEncoderChan, Dylib::Import::cMangled },
  { "DAQmxCreateCILinVelocityChan", (void**)&DAQmxCreateCILinVelocityChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIPeriodChan", (void**)&DAQmxCreateCIPeriodChan, Dylib::Import::cMangled },
  { "DAQmxCreateCIPulseChanFreq", (void**)&DAQmxCreateCIPulseChanFreq, Dylib::Import::cMangled },
  { "DAQmxCreateCIPulseChanTicks", (void**)&DAQmxCreateCIPulseChanTicks, Dylib::Import::cMangled },
  { "DAQmxCreateCIPulseChanTime", (void**)&DAQmxCreateCIPulseChanTime, Dylib::Import::cMangled },
  { "DAQmxCreateCIPulseWidthChan", (void**)&DAQmxCreateCIPulseWidthChan, Dylib::Import::cMangled },
  { "DAQmxCreateCISemiPeriodChan", (void**)&DAQmxCreateCISemiPeriodChan, Dylib::Import::cMangled },
  { "DAQmxCreateCITwoEdgeSepChan", (void**)&DAQmxCreateCITwoEdgeSepChan, Dylib::Import::cMangled },
  { "DAQmxCreateCOPulseChanFreq", (void**)&DAQmxCreateCOPulseChanFreq, Dylib::Import::cMangled },
  { "DAQmxCreateCOPulseChanTicks", (void**)&DAQmxCreateCOPulseChanTicks, Dylib::Import::cMangled },
  { "DAQmxCreateCOPulseChanTime", (void**)&DAQmxCreateCOPulseChanTime, Dylib::Import::cMangled },
  { "DAQmxCreateDIChan", (void**)&DAQmxCreateDIChan, Dylib::Import::cMangled },
  { "DAQmxCreateDOChan", (void**)&DAQmxCreateDOChan, Dylib::Import::cMangled },
  { "DAQmxCreateLinScale", (void**)&DAQmxCreateLinScale, Dylib::Import::cMangled },
  { "DAQmxCreateMapScale", (void**)&DAQmxCreateMapScale, Dylib::Import::cMangled },
  { "DAQmxCreatePolynomialScale", (void**)&DAQmxCreatePolynomialScale, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIAccelChan", (void**)&DAQmxCreateTEDSAIAccelChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIBridgeChan", (void**)&DAQmxCreateTEDSAIBridgeChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAICurrentChan", (void**)&DAQmxCreateTEDSAICurrentChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIForceBridgeChan", (void**)&DAQmxCreateTEDSAIForceBridgeChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIForceIEPEChan", (void**)&DAQmxCreateTEDSAIForceIEPEChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIMicrophoneChan", (void**)&DAQmxCreateTEDSAIMicrophoneChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIPosLVDTChan", (void**)&DAQmxCreateTEDSAIPosLVDTChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIPosRVDTChan", (void**)&DAQmxCreateTEDSAIPosRVDTChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIPressureBridgeChan", (void**)&DAQmxCreateTEDSAIPressureBridgeChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIRTDChan", (void**)&DAQmxCreateTEDSAIRTDChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIResistanceChan", (void**)&DAQmxCreateTEDSAIResistanceChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIStrainGageChan", (void**)&DAQmxCreateTEDSAIStrainGageChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIThrmcplChan", (void**)&DAQmxCreateTEDSAIThrmcplChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIThrmstrChanIex", (void**)&DAQmxCreateTEDSAIThrmstrChanIex, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIThrmstrChanVex", (void**)&DAQmxCreateTEDSAIThrmstrChanVex, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAITorqueBridgeChan", (void**)&DAQmxCreateTEDSAITorqueBridgeChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIVoltageChan", (void**)&DAQmxCreateTEDSAIVoltageChan, Dylib::Import::cMangled },
  { "DAQmxCreateTEDSAIVoltageChanWithExcit", (void**)&DAQmxCreateTEDSAIVoltageChanWithExcit, Dylib::Import::cMangled },
  { "DAQmxCreateTableScale", (void**)&DAQmxCreateTableScale, Dylib::Import::cMangled },
  { "DAQmxCreateTask", (void**)&DAQmxCreateTask, Dylib::Import::cMangled },
  { "DAQmxCreateWatchdogTimerTask", (void**)&DAQmxCreateWatchdogTimerTask, Dylib::Import::cMangled },
  { "DAQmxCreateWatchdogTimerTaskEx", (void**)&DAQmxCreateWatchdogTimerTaskEx, Dylib::Import::cMangled },
  { "DAQmxDSASetCalTemp", (void**)&DAQmxDSASetCalTemp, Dylib::Import::cMangled },
  { "DAQmxDeleteNetworkDevice", (void**)&DAQmxDeleteNetworkDevice, Dylib::Import::cMangled },
  { "DAQmxDeleteSavedGlobalChan", (void**)&DAQmxDeleteSavedGlobalChan, Dylib::Import::cMangled },
  { "DAQmxDeleteSavedScale", (void**)&DAQmxDeleteSavedScale, Dylib::Import::cMangled },
  { "DAQmxDeleteSavedTask", (void**)&DAQmxDeleteSavedTask, Dylib::Import::cMangled },
  { "DAQmxDeviceSupportsCal", (void**)&DAQmxDeviceSupportsCal, Dylib::Import::cMangled },
  { "DAQmxDisableAdvTrig", (void**)&DAQmxDisableAdvTrig, Dylib::Import::cMangled },
  { "DAQmxDisableRefTrig", (void**)&DAQmxDisableRefTrig, Dylib::Import::cMangled },
  { "DAQmxDisableStartTrig", (void**)&DAQmxDisableStartTrig, Dylib::Import::cMangled },
  { "DAQmxDisconnectSCExpressCalAccChans", (void**)&DAQmxDisconnectSCExpressCalAccChans, Dylib::Import::cMangled },
  { "DAQmxDisconnectTerms", (void**)&DAQmxDisconnectTerms, Dylib::Import::cMangled },
  { "DAQmxESeriesCalAdjust", (void**)&DAQmxESeriesCalAdjust, Dylib::Import::cMangled },
  { "DAQmxExportSignal", (void**)&DAQmxExportSignal, Dylib::Import::cMangled },
  { "DAQmxFieldDAQSetCalTemp", (void**)&DAQmxFieldDAQSetCalTemp, Dylib::Import::cMangled },
  { "DAQmxGet11601CalAdjustPoints", (void**)&DAQmxGet11601CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet11603CalAdjustPoints", (void**)&DAQmxGet11603CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet11605CalAdjustPoints", (void**)&DAQmxGet11605CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet11613CalAdjustPoints", (void**)&DAQmxGet11613CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet11614CalAdjustPoints", (void**)&DAQmxGet11614CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet11634CalAdjustPoints", (void**)&DAQmxGet11634CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4302CalAdjustPoints", (void**)&DAQmxGet4302CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4303CalAdjustPoints", (void**)&DAQmxGet4303CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4304CalAdjustPoints", (void**)&DAQmxGet4304CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4305CalAdjustPoints", (void**)&DAQmxGet4305CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4322CalAdjustPoints", (void**)&DAQmxGet4322CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4339CalAdjustPoints", (void**)&DAQmxGet4339CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet4463AdjustPoints", (void**)&DAQmxGet4463AdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9201CalAdjustPoints", (void**)&DAQmxGet9201CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9202CalAdjustPoints", (void**)&DAQmxGet9202CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9203CalAdjustPoints", (void**)&DAQmxGet9203CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9207CalAdjustPoints", (void**)&DAQmxGet9207CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9208CalAdjustPoints", (void**)&DAQmxGet9208CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9209CalAdjustPoints", (void**)&DAQmxGet9209CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9212CalAdjustPoints", (void**)&DAQmxGet9212CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9213CalAdjustPoints", (void**)&DAQmxGet9213CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9214CalAdjustPoints", (void**)&DAQmxGet9214CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9215CalAdjustPoints", (void**)&DAQmxGet9215CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9216CalAdjustPoints", (void**)&DAQmxGet9216CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9217CalAdjustPoints", (void**)&DAQmxGet9217CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9218CalAdjustPoints", (void**)&DAQmxGet9218CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9219CalAdjustPoints", (void**)&DAQmxGet9219CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9220CalAdjustPoints", (void**)&DAQmxGet9220CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9221CalAdjustPoints", (void**)&DAQmxGet9221CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9222CalAdjustPoints", (void**)&DAQmxGet9222CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9223CalAdjustPoints", (void**)&DAQmxGet9223CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9224CalAdjustPoints", (void**)&DAQmxGet9224CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9225CalAdjustPoints", (void**)&DAQmxGet9225CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9226CalAdjustPoints", (void**)&DAQmxGet9226CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9227CalAdjustPoints", (void**)&DAQmxGet9227CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9228CalAdjustPoints", (void**)&DAQmxGet9228CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9229CalAdjustPoints", (void**)&DAQmxGet9229CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9230CalAdjustPoints", (void**)&DAQmxGet9230CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9231CalAdjustPoints", (void**)&DAQmxGet9231CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9232CalAdjustPoints", (void**)&DAQmxGet9232CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9234CalAdjustPoints", (void**)&DAQmxGet9234CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9238CalAdjustPoints", (void**)&DAQmxGet9238CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9239CalAdjustPoints", (void**)&DAQmxGet9239CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9242CalAdjustPoints", (void**)&DAQmxGet9242CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9244CalAdjustPoints", (void**)&DAQmxGet9244CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9246CalAdjustPoints", (void**)&DAQmxGet9246CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9247CalAdjustPoints", (void**)&DAQmxGet9247CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9250CalAdjustPoints", (void**)&DAQmxGet9250CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9251CalAdjustPoints", (void**)&DAQmxGet9251CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9252CalAdjustPoints", (void**)&DAQmxGet9252CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9253CalAdjustPoints", (void**)&DAQmxGet9253CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9260CalAdjustPoints", (void**)&DAQmxGet9260CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9262CalAdjustPoints", (void**)&DAQmxGet9262CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9263CalAdjustPoints", (void**)&DAQmxGet9263CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9264CalAdjustPoints", (void**)&DAQmxGet9264CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9265CalAdjustPoints", (void**)&DAQmxGet9265CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9266CalAdjustPoints", (void**)&DAQmxGet9266CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9269CalAdjustPoints", (void**)&DAQmxGet9269CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9628AICalAdjustPoints", (void**)&DAQmxGet9628AICalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9628AOCalAdjustPoints", (void**)&DAQmxGet9628AOCalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9629AICalAdjustPoints", (void**)&DAQmxGet9629AICalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9629AOCalAdjustPoints", (void**)&DAQmxGet9629AOCalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9638AICalAdjustPoints", (void**)&DAQmxGet9638AICalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9638AOCalAdjustPoints", (void**)&DAQmxGet9638AOCalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGet9775CalAdjustPoints", (void**)&DAQmxGet9775CalAdjustPoints, Dylib::Import::cMangled },
  { "DAQmxGetAIACExcitFreq", (void**)&DAQmxGetAIACExcitFreq, Dylib::Import::cMangled },
  { "DAQmxGetAIACExcitSyncEnable", (void**)&DAQmxGetAIACExcitSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIACExcitWireMode", (void**)&DAQmxGetAIACExcitWireMode, Dylib::Import::cMangled },
  { "DAQmxGetAIADCCustomTimingMode", (void**)&DAQmxGetAIADCCustomTimingMode, Dylib::Import::cMangled },
  { "DAQmxGetAIADCTimingMode", (void**)&DAQmxGetAIADCTimingMode, Dylib::Import::cMangled },
  { "DAQmxGetAIAccel4WireDCVoltageSensitivity", (void**)&DAQmxGetAIAccel4WireDCVoltageSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIAccel4WireDCVoltageSensitivityUnits", (void**)&DAQmxGetAIAccel4WireDCVoltageSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIAccelChargeSensitivity", (void**)&DAQmxGetAIAccelChargeSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIAccelChargeSensitivityUnits", (void**)&DAQmxGetAIAccelChargeSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIAccelSensitivity", (void**)&DAQmxGetAIAccelSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIAccelSensitivityUnits", (void**)&DAQmxGetAIAccelSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIAccelUnits", (void**)&DAQmxGetAIAccelUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIAcceldBRef", (void**)&DAQmxGetAIAcceldBRef, Dylib::Import::cMangled },
  { "DAQmxGetAIAtten", (void**)&DAQmxGetAIAtten, Dylib::Import::cMangled },
  { "DAQmxGetAIAutoZeroMode", (void**)&DAQmxGetAIAutoZeroMode, Dylib::Import::cMangled },
  { "DAQmxGetAIAveragingWinSize", (void**)&DAQmxGetAIAveragingWinSize, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeBalanceCoarsePot", (void**)&DAQmxGetAIBridgeBalanceCoarsePot, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeBalanceFinePot", (void**)&DAQmxGetAIBridgeBalanceFinePot, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeCfg", (void**)&DAQmxGetAIBridgeCfg, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeElectricalUnits", (void**)&DAQmxGetAIBridgeElectricalUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeInitialRatio", (void**)&DAQmxGetAIBridgeInitialRatio, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeInitialVoltage", (void**)&DAQmxGetAIBridgeInitialVoltage, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeNomResistance", (void**)&DAQmxGetAIBridgeNomResistance, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgePhysicalUnits", (void**)&DAQmxGetAIBridgePhysicalUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgePolyForwardCoeff", (void**)&DAQmxGetAIBridgePolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgePolyReverseCoeff", (void**)&DAQmxGetAIBridgePolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeScaleType", (void**)&DAQmxGetAIBridgeScaleType, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalEnable", (void**)&DAQmxGetAIBridgeShuntCalEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalGainAdjust", (void**)&DAQmxGetAIBridgeShuntCalGainAdjust, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalSelect", (void**)&DAQmxGetAIBridgeShuntCalSelect, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalShuntCalAActualResistance", (void**)&DAQmxGetAIBridgeShuntCalShuntCalAActualResistance, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalShuntCalAResistance", (void**)&DAQmxGetAIBridgeShuntCalShuntCalAResistance, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalShuntCalASrc", (void**)&DAQmxGetAIBridgeShuntCalShuntCalASrc, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalShuntCalBActualResistance", (void**)&DAQmxGetAIBridgeShuntCalShuntCalBActualResistance, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeShuntCalShuntCalBResistance", (void**)&DAQmxGetAIBridgeShuntCalShuntCalBResistance, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeTableElectricalVals", (void**)&DAQmxGetAIBridgeTableElectricalVals, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeTablePhysicalVals", (void**)&DAQmxGetAIBridgeTablePhysicalVals, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeTwoPointLinFirstElectricalVal", (void**)&DAQmxGetAIBridgeTwoPointLinFirstElectricalVal, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeTwoPointLinFirstPhysicalVal", (void**)&DAQmxGetAIBridgeTwoPointLinFirstPhysicalVal, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeTwoPointLinSecondElectricalVal", (void**)&DAQmxGetAIBridgeTwoPointLinSecondElectricalVal, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeTwoPointLinSecondPhysicalVal", (void**)&DAQmxGetAIBridgeTwoPointLinSecondPhysicalVal, Dylib::Import::cMangled },
  { "DAQmxGetAIBridgeUnits", (void**)&DAQmxGetAIBridgeUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalApplyCalIfExp", (void**)&DAQmxGetAIChanCalApplyCalIfExp, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalCalDate", (void**)&DAQmxGetAIChanCalCalDate, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalDesc", (void**)&DAQmxGetAIChanCalDesc, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalEnableCal", (void**)&DAQmxGetAIChanCalEnableCal, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalExpDate", (void**)&DAQmxGetAIChanCalExpDate, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalHasValidCalInfo", (void**)&DAQmxGetAIChanCalHasValidCalInfo, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalOperatorName", (void**)&DAQmxGetAIChanCalOperatorName, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalPolyForwardCoeff", (void**)&DAQmxGetAIChanCalPolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalPolyReverseCoeff", (void**)&DAQmxGetAIChanCalPolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalScaleType", (void**)&DAQmxGetAIChanCalScaleType, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalTablePreScaledVals", (void**)&DAQmxGetAIChanCalTablePreScaledVals, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalTableScaledVals", (void**)&DAQmxGetAIChanCalTableScaledVals, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalVerifAcqVals", (void**)&DAQmxGetAIChanCalVerifAcqVals, Dylib::Import::cMangled },
  { "DAQmxGetAIChanCalVerifRefVals", (void**)&DAQmxGetAIChanCalVerifRefVals, Dylib::Import::cMangled },
  { "DAQmxGetAIChargeUnits", (void**)&DAQmxGetAIChargeUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIChopEnable", (void**)&DAQmxGetAIChopEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIConvActiveEdge", (void**)&DAQmxGetAIConvActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetAIConvActiveEdgeEx", (void**)&DAQmxGetAIConvActiveEdgeEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrEnable", (void**)&DAQmxGetAIConvDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrEnableEx", (void**)&DAQmxGetAIConvDigFltrEnableEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrMinPulseWidth", (void**)&DAQmxGetAIConvDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrMinPulseWidthEx", (void**)&DAQmxGetAIConvDigFltrMinPulseWidthEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrTimebaseRate", (void**)&DAQmxGetAIConvDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrTimebaseRateEx", (void**)&DAQmxGetAIConvDigFltrTimebaseRateEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrTimebaseSrc", (void**)&DAQmxGetAIConvDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigFltrTimebaseSrcEx", (void**)&DAQmxGetAIConvDigFltrTimebaseSrcEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigSyncEnable", (void**)&DAQmxGetAIConvDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIConvDigSyncEnableEx", (void**)&DAQmxGetAIConvDigSyncEnableEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvMaxRate", (void**)&DAQmxGetAIConvMaxRate, Dylib::Import::cMangled },
  { "DAQmxGetAIConvMaxRateEx", (void**)&DAQmxGetAIConvMaxRateEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvRate", (void**)&DAQmxGetAIConvRate, Dylib::Import::cMangled },
  { "DAQmxGetAIConvRateEx", (void**)&DAQmxGetAIConvRateEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvSrc", (void**)&DAQmxGetAIConvSrc, Dylib::Import::cMangled },
  { "DAQmxGetAIConvSrcEx", (void**)&DAQmxGetAIConvSrcEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvTimebaseDiv", (void**)&DAQmxGetAIConvTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxGetAIConvTimebaseDivEx", (void**)&DAQmxGetAIConvTimebaseDivEx, Dylib::Import::cMangled },
  { "DAQmxGetAIConvTimebaseSrc", (void**)&DAQmxGetAIConvTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAIConvTimebaseSrcEx", (void**)&DAQmxGetAIConvTimebaseSrcEx, Dylib::Import::cMangled },
  { "DAQmxGetAICoupling", (void**)&DAQmxGetAICoupling, Dylib::Import::cMangled },
  { "DAQmxGetAICurrentACRMSUnits", (void**)&DAQmxGetAICurrentACRMSUnits, Dylib::Import::cMangled },
  { "DAQmxGetAICurrentShuntLoc", (void**)&DAQmxGetAICurrentShuntLoc, Dylib::Import::cMangled },
  { "DAQmxGetAICurrentShuntResistance", (void**)&DAQmxGetAICurrentShuntResistance, Dylib::Import::cMangled },
  { "DAQmxGetAICurrentUnits", (void**)&DAQmxGetAICurrentUnits, Dylib::Import::cMangled },
  { "DAQmxGetAICustomScaleName", (void**)&DAQmxGetAICustomScaleName, Dylib::Import::cMangled },
  { "DAQmxGetAIDCOffset", (void**)&DAQmxGetAIDCOffset, Dylib::Import::cMangled },
  { "DAQmxGetAIDataXferCustomThreshold", (void**)&DAQmxGetAIDataXferCustomThreshold, Dylib::Import::cMangled },
  { "DAQmxGetAIDataXferMaxRate", (void**)&DAQmxGetAIDataXferMaxRate, Dylib::Import::cMangled },
  { "DAQmxGetAIDataXferMech", (void**)&DAQmxGetAIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxGetAIDataXferReqCond", (void**)&DAQmxGetAIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxGetAIDevScalingCoeff", (void**)&DAQmxGetAIDevScalingCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrBandpassCenterFreq", (void**)&DAQmxGetAIDigFltrBandpassCenterFreq, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrBandpassWidth", (void**)&DAQmxGetAIDigFltrBandpassWidth, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrCoeff", (void**)&DAQmxGetAIDigFltrCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrEnable", (void**)&DAQmxGetAIDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrHighpassCutoffFreq", (void**)&DAQmxGetAIDigFltrHighpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrLowpassCutoffFreq", (void**)&DAQmxGetAIDigFltrLowpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrNotchCenterFreq", (void**)&DAQmxGetAIDigFltrNotchCenterFreq, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrNotchWidth", (void**)&DAQmxGetAIDigFltrNotchWidth, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrOrder", (void**)&DAQmxGetAIDigFltrOrder, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrResponse", (void**)&DAQmxGetAIDigFltrResponse, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrType", (void**)&DAQmxGetAIDigFltrType, Dylib::Import::cMangled },
  { "DAQmxGetAIDigFltrTypes", (void**)&DAQmxGetAIDigFltrTypes, Dylib::Import::cMangled },
  { "DAQmxGetAIDitherEnable", (void**)&DAQmxGetAIDitherEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIEddyCurrentProxProbeSensitivity", (void**)&DAQmxGetAIEddyCurrentProxProbeSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIEddyCurrentProxProbeSensitivityUnits", (void**)&DAQmxGetAIEddyCurrentProxProbeSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIEddyCurrentProxProbeUnits", (void**)&DAQmxGetAIEddyCurrentProxProbeUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIEnhancedAliasRejectionEnable", (void**)&DAQmxGetAIEnhancedAliasRejectionEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitActualVal", (void**)&DAQmxGetAIExcitActualVal, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitDCorAC", (void**)&DAQmxGetAIExcitDCorAC, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitIdleOutputBehavior", (void**)&DAQmxGetAIExcitIdleOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitSense", (void**)&DAQmxGetAIExcitSense, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitSrc", (void**)&DAQmxGetAIExcitSrc, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitUseForScaling", (void**)&DAQmxGetAIExcitUseForScaling, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitUseMultiplexed", (void**)&DAQmxGetAIExcitUseMultiplexed, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitVal", (void**)&DAQmxGetAIExcitVal, Dylib::Import::cMangled },
  { "DAQmxGetAIExcitVoltageOrCurrent", (void**)&DAQmxGetAIExcitVoltageOrCurrent, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterDelay", (void**)&DAQmxGetAIFilterDelay, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterDelayAdjustment", (void**)&DAQmxGetAIFilterDelayAdjustment, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterDelayUnits", (void**)&DAQmxGetAIFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterEnable", (void**)&DAQmxGetAIFilterEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterFreq", (void**)&DAQmxGetAIFilterFreq, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterOrder", (void**)&DAQmxGetAIFilterOrder, Dylib::Import::cMangled },
  { "DAQmxGetAIFilterResponse", (void**)&DAQmxGetAIFilterResponse, Dylib::Import::cMangled },
  { "DAQmxGetAIForceIEPESensorSensitivity", (void**)&DAQmxGetAIForceIEPESensorSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIForceIEPESensorSensitivityUnits", (void**)&DAQmxGetAIForceIEPESensorSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIForceReadFromChan", (void**)&DAQmxGetAIForceReadFromChan, Dylib::Import::cMangled },
  { "DAQmxGetAIForceUnits", (void**)&DAQmxGetAIForceUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIFreqHyst", (void**)&DAQmxGetAIFreqHyst, Dylib::Import::cMangled },
  { "DAQmxGetAIFreqThreshVoltage", (void**)&DAQmxGetAIFreqThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetAIFreqUnits", (void**)&DAQmxGetAIFreqUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIGain", (void**)&DAQmxGetAIGain, Dylib::Import::cMangled },
  { "DAQmxGetAIImpedance", (void**)&DAQmxGetAIImpedance, Dylib::Import::cMangled },
  { "DAQmxGetAIInputLimitsFaultDetectEnable", (void**)&DAQmxGetAIInputLimitsFaultDetectEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIInputLimitsFaultDetectLowerLimit", (void**)&DAQmxGetAIInputLimitsFaultDetectLowerLimit, Dylib::Import::cMangled },
  { "DAQmxGetAIInputLimitsFaultDetectUpperLimit", (void**)&DAQmxGetAIInputLimitsFaultDetectUpperLimit, Dylib::Import::cMangled },
  { "DAQmxGetAIInputSrc", (void**)&DAQmxGetAIInputSrc, Dylib::Import::cMangled },
  { "DAQmxGetAIIsTEDS", (void**)&DAQmxGetAIIsTEDS, Dylib::Import::cMangled },
  { "DAQmxGetAILVDTSensitivity", (void**)&DAQmxGetAILVDTSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAILVDTSensitivityUnits", (void**)&DAQmxGetAILVDTSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAILVDTUnits", (void**)&DAQmxGetAILVDTUnits, Dylib::Import::cMangled },
  { "DAQmxGetAILeadWireResistance", (void**)&DAQmxGetAILeadWireResistance, Dylib::Import::cMangled },
  { "DAQmxGetAILossyLSBRemovalCompressedSampSize", (void**)&DAQmxGetAILossyLSBRemovalCompressedSampSize, Dylib::Import::cMangled },
  { "DAQmxGetAILowpassCutoffFreq", (void**)&DAQmxGetAILowpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxGetAILowpassEnable", (void**)&DAQmxGetAILowpassEnable, Dylib::Import::cMangled },
  { "DAQmxGetAILowpassSwitchCapClkSrc", (void**)&DAQmxGetAILowpassSwitchCapClkSrc, Dylib::Import::cMangled },
  { "DAQmxGetAILowpassSwitchCapExtClkDiv", (void**)&DAQmxGetAILowpassSwitchCapExtClkDiv, Dylib::Import::cMangled },
  { "DAQmxGetAILowpassSwitchCapExtClkFreq", (void**)&DAQmxGetAILowpassSwitchCapExtClkFreq, Dylib::Import::cMangled },
  { "DAQmxGetAILowpassSwitchCapOutClkDiv", (void**)&DAQmxGetAILowpassSwitchCapOutClkDiv, Dylib::Import::cMangled },
  { "DAQmxGetAIMax", (void**)&DAQmxGetAIMax, Dylib::Import::cMangled },
  { "DAQmxGetAIMeasType", (void**)&DAQmxGetAIMeasType, Dylib::Import::cMangled },
  { "DAQmxGetAIMemMapEnable", (void**)&DAQmxGetAIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIMicrophoneSensitivity", (void**)&DAQmxGetAIMicrophoneSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIMin", (void**)&DAQmxGetAIMin, Dylib::Import::cMangled },
  { "DAQmxGetAIOpenChanDetectEnable", (void**)&DAQmxGetAIOpenChanDetectEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIOpenThrmcplDetectEnable", (void**)&DAQmxGetAIOpenThrmcplDetectEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIOvercurrentDetectEnable", (void**)&DAQmxGetAIOvercurrentDetectEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIPowerSupplyFaultDetectEnable", (void**)&DAQmxGetAIPowerSupplyFaultDetectEnable, Dylib::Import::cMangled },
  { "DAQmxGetAIPressureUnits", (void**)&DAQmxGetAIPressureUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIProbeAtten", (void**)&DAQmxGetAIProbeAtten, Dylib::Import::cMangled },
  { "DAQmxGetAIRTDA", (void**)&DAQmxGetAIRTDA, Dylib::Import::cMangled },
  { "DAQmxGetAIRTDB", (void**)&DAQmxGetAIRTDB, Dylib::Import::cMangled },
  { "DAQmxGetAIRTDC", (void**)&DAQmxGetAIRTDC, Dylib::Import::cMangled },
  { "DAQmxGetAIRTDR0", (void**)&DAQmxGetAIRTDR0, Dylib::Import::cMangled },
  { "DAQmxGetAIRTDType", (void**)&DAQmxGetAIRTDType, Dylib::Import::cMangled },
  { "DAQmxGetAIRVDTSensitivity", (void**)&DAQmxGetAIRVDTSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIRVDTSensitivityUnits", (void**)&DAQmxGetAIRVDTSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIRVDTUnits", (void**)&DAQmxGetAIRVDTUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIRawDataCompressionType", (void**)&DAQmxGetAIRawDataCompressionType, Dylib::Import::cMangled },
  { "DAQmxGetAIRawSampJustification", (void**)&DAQmxGetAIRawSampJustification, Dylib::Import::cMangled },
  { "DAQmxGetAIRawSampSize", (void**)&DAQmxGetAIRawSampSize, Dylib::Import::cMangled },
  { "DAQmxGetAIRemoveFilterDelay", (void**)&DAQmxGetAIRemoveFilterDelay, Dylib::Import::cMangled },
  { "DAQmxGetAIResistanceCfg", (void**)&DAQmxGetAIResistanceCfg, Dylib::Import::cMangled },
  { "DAQmxGetAIResistanceUnits", (void**)&DAQmxGetAIResistanceUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIResolution", (void**)&DAQmxGetAIResolution, Dylib::Import::cMangled },
  { "DAQmxGetAIResolutionUnits", (void**)&DAQmxGetAIResolutionUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIRngHigh", (void**)&DAQmxGetAIRngHigh, Dylib::Import::cMangled },
  { "DAQmxGetAIRngLow", (void**)&DAQmxGetAIRngLow, Dylib::Import::cMangled },
  { "DAQmxGetAIRosetteStrainGageOrientation", (void**)&DAQmxGetAIRosetteStrainGageOrientation, Dylib::Import::cMangled },
  { "DAQmxGetAIRosetteStrainGageRosetteMeasType", (void**)&DAQmxGetAIRosetteStrainGageRosetteMeasType, Dylib::Import::cMangled },
  { "DAQmxGetAIRosetteStrainGageRosetteType", (void**)&DAQmxGetAIRosetteStrainGageRosetteType, Dylib::Import::cMangled },
  { "DAQmxGetAIRosetteStrainGageStrainChans", (void**)&DAQmxGetAIRosetteStrainGageStrainChans, Dylib::Import::cMangled },
  { "DAQmxGetAISampAndHoldEnable", (void**)&DAQmxGetAISampAndHoldEnable, Dylib::Import::cMangled },
  { "DAQmxGetAISensorPowerCfg", (void**)&DAQmxGetAISensorPowerCfg, Dylib::Import::cMangled },
  { "DAQmxGetAISensorPowerType", (void**)&DAQmxGetAISensorPowerType, Dylib::Import::cMangled },
  { "DAQmxGetAISensorPowerVoltage", (void**)&DAQmxGetAISensorPowerVoltage, Dylib::Import::cMangled },
  { "DAQmxGetAISoundPressureMaxSoundPressureLvl", (void**)&DAQmxGetAISoundPressureMaxSoundPressureLvl, Dylib::Import::cMangled },
  { "DAQmxGetAISoundPressureUnits", (void**)&DAQmxGetAISoundPressureUnits, Dylib::Import::cMangled },
  { "DAQmxGetAISoundPressuredBRef", (void**)&DAQmxGetAISoundPressuredBRef, Dylib::Import::cMangled },
  { "DAQmxGetAIStrainGageCfg", (void**)&DAQmxGetAIStrainGageCfg, Dylib::Import::cMangled },
  { "DAQmxGetAIStrainGageForceReadFromChan", (void**)&DAQmxGetAIStrainGageForceReadFromChan, Dylib::Import::cMangled },
  { "DAQmxGetAIStrainGageGageFactor", (void**)&DAQmxGetAIStrainGageGageFactor, Dylib::Import::cMangled },
  { "DAQmxGetAIStrainGagePoissonRatio", (void**)&DAQmxGetAIStrainGagePoissonRatio, Dylib::Import::cMangled },
  { "DAQmxGetAIStrainUnits", (void**)&DAQmxGetAIStrainUnits, Dylib::Import::cMangled },
  { "DAQmxGetAITEDSUnits", (void**)&DAQmxGetAITEDSUnits, Dylib::Import::cMangled },
  { "DAQmxGetAITempUnits", (void**)&DAQmxGetAITempUnits, Dylib::Import::cMangled },
  { "DAQmxGetAITermCfg", (void**)&DAQmxGetAITermCfg, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmcplCJCChan", (void**)&DAQmxGetAIThrmcplCJCChan, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmcplCJCSrc", (void**)&DAQmxGetAIThrmcplCJCSrc, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmcplCJCVal", (void**)&DAQmxGetAIThrmcplCJCVal, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmcplLeadOffsetVoltage", (void**)&DAQmxGetAIThrmcplLeadOffsetVoltage, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmcplScaleType", (void**)&DAQmxGetAIThrmcplScaleType, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmcplType", (void**)&DAQmxGetAIThrmcplType, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmstrA", (void**)&DAQmxGetAIThrmstrA, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmstrB", (void**)&DAQmxGetAIThrmstrB, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmstrC", (void**)&DAQmxGetAIThrmstrC, Dylib::Import::cMangled },
  { "DAQmxGetAIThrmstrR1", (void**)&DAQmxGetAIThrmstrR1, Dylib::Import::cMangled },
  { "DAQmxGetAITorqueUnits", (void**)&DAQmxGetAITorqueUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIUsbXferReqCount", (void**)&DAQmxGetAIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxGetAIUsbXferReqSize", (void**)&DAQmxGetAIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxGetAIVelocityIEPESensorSensitivity", (void**)&DAQmxGetAIVelocityIEPESensorSensitivity, Dylib::Import::cMangled },
  { "DAQmxGetAIVelocityIEPESensorSensitivityUnits", (void**)&DAQmxGetAIVelocityIEPESensorSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIVelocityIEPESensordBRef", (void**)&DAQmxGetAIVelocityIEPESensordBRef, Dylib::Import::cMangled },
  { "DAQmxGetAIVelocityUnits", (void**)&DAQmxGetAIVelocityUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIVoltageACRMSUnits", (void**)&DAQmxGetAIVoltageACRMSUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIVoltageUnits", (void**)&DAQmxGetAIVoltageUnits, Dylib::Import::cMangled },
  { "DAQmxGetAIVoltagedBRef", (void**)&DAQmxGetAIVoltagedBRef, Dylib::Import::cMangled },
  { "DAQmxGetAOCommonModeOffset", (void**)&DAQmxGetAOCommonModeOffset, Dylib::Import::cMangled },
  { "DAQmxGetAOCurrentUnits", (void**)&DAQmxGetAOCurrentUnits, Dylib::Import::cMangled },
  { "DAQmxGetAOCustomScaleName", (void**)&DAQmxGetAOCustomScaleName, Dylib::Import::cMangled },
  { "DAQmxGetAODACOffsetExtSrc", (void**)&DAQmxGetAODACOffsetExtSrc, Dylib::Import::cMangled },
  { "DAQmxGetAODACOffsetSrc", (void**)&DAQmxGetAODACOffsetSrc, Dylib::Import::cMangled },
  { "DAQmxGetAODACOffsetVal", (void**)&DAQmxGetAODACOffsetVal, Dylib::Import::cMangled },
  { "DAQmxGetAODACRefAllowConnToGnd", (void**)&DAQmxGetAODACRefAllowConnToGnd, Dylib::Import::cMangled },
  { "DAQmxGetAODACRefConnToGnd", (void**)&DAQmxGetAODACRefConnToGnd, Dylib::Import::cMangled },
  { "DAQmxGetAODACRefExtSrc", (void**)&DAQmxGetAODACRefExtSrc, Dylib::Import::cMangled },
  { "DAQmxGetAODACRefSrc", (void**)&DAQmxGetAODACRefSrc, Dylib::Import::cMangled },
  { "DAQmxGetAODACRefVal", (void**)&DAQmxGetAODACRefVal, Dylib::Import::cMangled },
  { "DAQmxGetAODACRngHigh", (void**)&DAQmxGetAODACRngHigh, Dylib::Import::cMangled },
  { "DAQmxGetAODACRngLow", (void**)&DAQmxGetAODACRngLow, Dylib::Import::cMangled },
  { "DAQmxGetAODataXferMech", (void**)&DAQmxGetAODataXferMech, Dylib::Import::cMangled },
  { "DAQmxGetAODataXferReqCond", (void**)&DAQmxGetAODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxGetAODevScalingCoeff", (void**)&DAQmxGetAODevScalingCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAOEnhancedImageRejectionEnable", (void**)&DAQmxGetAOEnhancedImageRejectionEnable, Dylib::Import::cMangled },
  { "DAQmxGetAOFilterDelay", (void**)&DAQmxGetAOFilterDelay, Dylib::Import::cMangled },
  { "DAQmxGetAOFilterDelayAdjustment", (void**)&DAQmxGetAOFilterDelayAdjustment, Dylib::Import::cMangled },
  { "DAQmxGetAOFilterDelayUnits", (void**)&DAQmxGetAOFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenAmplitude", (void**)&DAQmxGetAOFuncGenAmplitude, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenFMDeviation", (void**)&DAQmxGetAOFuncGenFMDeviation, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenFreq", (void**)&DAQmxGetAOFuncGenFreq, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenModulationType", (void**)&DAQmxGetAOFuncGenModulationType, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenOffset", (void**)&DAQmxGetAOFuncGenOffset, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenSquareDutyCycle", (void**)&DAQmxGetAOFuncGenSquareDutyCycle, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenStartPhase", (void**)&DAQmxGetAOFuncGenStartPhase, Dylib::Import::cMangled },
  { "DAQmxGetAOFuncGenType", (void**)&DAQmxGetAOFuncGenType, Dylib::Import::cMangled },
  { "DAQmxGetAOGain", (void**)&DAQmxGetAOGain, Dylib::Import::cMangled },
  { "DAQmxGetAOIdleOutputBehavior", (void**)&DAQmxGetAOIdleOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxGetAOLoadImpedance", (void**)&DAQmxGetAOLoadImpedance, Dylib::Import::cMangled },
  { "DAQmxGetAOMax", (void**)&DAQmxGetAOMax, Dylib::Import::cMangled },
  { "DAQmxGetAOMemMapEnable", (void**)&DAQmxGetAOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxGetAOMin", (void**)&DAQmxGetAOMin, Dylib::Import::cMangled },
  { "DAQmxGetAOOutputImpedance", (void**)&DAQmxGetAOOutputImpedance, Dylib::Import::cMangled },
  { "DAQmxGetAOOutputType", (void**)&DAQmxGetAOOutputType, Dylib::Import::cMangled },
  { "DAQmxGetAOPowerAmpChannelEnable", (void**)&DAQmxGetAOPowerAmpChannelEnable, Dylib::Import::cMangled },
  { "DAQmxGetAOPowerAmpGain", (void**)&DAQmxGetAOPowerAmpGain, Dylib::Import::cMangled },
  { "DAQmxGetAOPowerAmpOffset", (void**)&DAQmxGetAOPowerAmpOffset, Dylib::Import::cMangled },
  { "DAQmxGetAOPowerAmpOvercurrent", (void**)&DAQmxGetAOPowerAmpOvercurrent, Dylib::Import::cMangled },
  { "DAQmxGetAOPowerAmpScalingCoeff", (void**)&DAQmxGetAOPowerAmpScalingCoeff, Dylib::Import::cMangled },
  { "DAQmxGetAOReglitchEnable", (void**)&DAQmxGetAOReglitchEnable, Dylib::Import::cMangled },
  { "DAQmxGetAOResolution", (void**)&DAQmxGetAOResolution, Dylib::Import::cMangled },
  { "DAQmxGetAOResolutionUnits", (void**)&DAQmxGetAOResolutionUnits, Dylib::Import::cMangled },
  { "DAQmxGetAOTermCfg", (void**)&DAQmxGetAOTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetAOUsbXferReqCount", (void**)&DAQmxGetAOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxGetAOUsbXferReqSize", (void**)&DAQmxGetAOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxGetAOUseOnlyOnBrdMem", (void**)&DAQmxGetAOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxGetAOVoltageCurrentLimit", (void**)&DAQmxGetAOVoltageCurrentLimit, Dylib::Import::cMangled },
  { "DAQmxGetAOVoltageUnits", (void**)&DAQmxGetAOVoltageUnits, Dylib::Import::cMangled },
  { "DAQmxGetAdvTrigType", (void**)&DAQmxGetAdvTrigType, Dylib::Import::cMangled },
  { "DAQmxGetAnalogPowerUpStates", (void**)&DAQmxGetAnalogPowerUpStates, Dylib::Import::cMangled },
  { "DAQmxGetAnalogPowerUpStatesWithOutputType", (void**)&DAQmxGetAnalogPowerUpStatesWithOutputType, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigCoupling", (void**)&DAQmxGetAnlgEdgeRefTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigDigFltrEnable", (void**)&DAQmxGetAnlgEdgeRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigDigFltrMinPulseWidth", (void**)&DAQmxGetAnlgEdgeRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseRate", (void**)&DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseSrc", (void**)&DAQmxGetAnlgEdgeRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigDigSyncEnable", (void**)&DAQmxGetAnlgEdgeRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigHyst", (void**)&DAQmxGetAnlgEdgeRefTrigHyst, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigLvl", (void**)&DAQmxGetAnlgEdgeRefTrigLvl, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigSlope", (void**)&DAQmxGetAnlgEdgeRefTrigSlope, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeRefTrigSrc", (void**)&DAQmxGetAnlgEdgeRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigCoupling", (void**)&DAQmxGetAnlgEdgeStartTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigDigFltrEnable", (void**)&DAQmxGetAnlgEdgeStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigDigFltrMinPulseWidth", (void**)&DAQmxGetAnlgEdgeStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseRate", (void**)&DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseSrc", (void**)&DAQmxGetAnlgEdgeStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigDigSyncEnable", (void**)&DAQmxGetAnlgEdgeStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigHyst", (void**)&DAQmxGetAnlgEdgeStartTrigHyst, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigLvl", (void**)&DAQmxGetAnlgEdgeStartTrigLvl, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigSlope", (void**)&DAQmxGetAnlgEdgeStartTrigSlope, Dylib::Import::cMangled },
  { "DAQmxGetAnlgEdgeStartTrigSrc", (void**)&DAQmxGetAnlgEdgeStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigCoupling", (void**)&DAQmxGetAnlgLvlPauseTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigDigFltrEnable", (void**)&DAQmxGetAnlgLvlPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxGetAnlgLvlPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseRate", (void**)&DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxGetAnlgLvlPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigDigSyncEnable", (void**)&DAQmxGetAnlgLvlPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigHyst", (void**)&DAQmxGetAnlgLvlPauseTrigHyst, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigLvl", (void**)&DAQmxGetAnlgLvlPauseTrigLvl, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigSrc", (void**)&DAQmxGetAnlgLvlPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgLvlPauseTrigWhen", (void**)&DAQmxGetAnlgLvlPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeRefTrigCouplings", (void**)&DAQmxGetAnlgMultiEdgeRefTrigCouplings, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeRefTrigHysts", (void**)&DAQmxGetAnlgMultiEdgeRefTrigHysts, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeRefTrigLvls", (void**)&DAQmxGetAnlgMultiEdgeRefTrigLvls, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeRefTrigSlopes", (void**)&DAQmxGetAnlgMultiEdgeRefTrigSlopes, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeRefTrigSrcs", (void**)&DAQmxGetAnlgMultiEdgeRefTrigSrcs, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeStartTrigCouplings", (void**)&DAQmxGetAnlgMultiEdgeStartTrigCouplings, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeStartTrigHysts", (void**)&DAQmxGetAnlgMultiEdgeStartTrigHysts, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeStartTrigLvls", (void**)&DAQmxGetAnlgMultiEdgeStartTrigLvls, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeStartTrigSlopes", (void**)&DAQmxGetAnlgMultiEdgeStartTrigSlopes, Dylib::Import::cMangled },
  { "DAQmxGetAnlgMultiEdgeStartTrigSrcs", (void**)&DAQmxGetAnlgMultiEdgeStartTrigSrcs, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigBtm", (void**)&DAQmxGetAnlgWinPauseTrigBtm, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigCoupling", (void**)&DAQmxGetAnlgWinPauseTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigDigFltrEnable", (void**)&DAQmxGetAnlgWinPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxGetAnlgWinPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigDigFltrTimebaseRate", (void**)&DAQmxGetAnlgWinPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxGetAnlgWinPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigDigSyncEnable", (void**)&DAQmxGetAnlgWinPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigSrc", (void**)&DAQmxGetAnlgWinPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigTop", (void**)&DAQmxGetAnlgWinPauseTrigTop, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinPauseTrigWhen", (void**)&DAQmxGetAnlgWinPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigBtm", (void**)&DAQmxGetAnlgWinRefTrigBtm, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigCoupling", (void**)&DAQmxGetAnlgWinRefTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigDigFltrEnable", (void**)&DAQmxGetAnlgWinRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigDigFltrMinPulseWidth", (void**)&DAQmxGetAnlgWinRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigDigFltrTimebaseRate", (void**)&DAQmxGetAnlgWinRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigDigFltrTimebaseSrc", (void**)&DAQmxGetAnlgWinRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigDigSyncEnable", (void**)&DAQmxGetAnlgWinRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigSrc", (void**)&DAQmxGetAnlgWinRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigTop", (void**)&DAQmxGetAnlgWinRefTrigTop, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinRefTrigWhen", (void**)&DAQmxGetAnlgWinRefTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigBtm", (void**)&DAQmxGetAnlgWinStartTrigBtm, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigCoupling", (void**)&DAQmxGetAnlgWinStartTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigDigFltrEnable", (void**)&DAQmxGetAnlgWinStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigDigFltrMinPulseWidth", (void**)&DAQmxGetAnlgWinStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigDigFltrTimebaseRate", (void**)&DAQmxGetAnlgWinStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigDigFltrTimebaseSrc", (void**)&DAQmxGetAnlgWinStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigDigSyncEnable", (void**)&DAQmxGetAnlgWinStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigSrc", (void**)&DAQmxGetAnlgWinStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigTop", (void**)&DAQmxGetAnlgWinStartTrigTop, Dylib::Import::cMangled },
  { "DAQmxGetAnlgWinStartTrigWhen", (void**)&DAQmxGetAnlgWinStartTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTerm", (void**)&DAQmxGetArmStartTerm, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTrigTimescale", (void**)&DAQmxGetArmStartTrigTimescale, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTrigTimestampEnable", (void**)&DAQmxGetArmStartTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTrigTimestampTimescale", (void**)&DAQmxGetArmStartTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTrigTimestampVal", (void**)&DAQmxGetArmStartTrigTimestampVal, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTrigTrigWhen", (void**)&DAQmxGetArmStartTrigTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetArmStartTrigType", (void**)&DAQmxGetArmStartTrigType, Dylib::Import::cMangled },
  { "DAQmxGetAutoConfiguredCDAQSyncConnections", (void**)&DAQmxGetAutoConfiguredCDAQSyncConnections, Dylib::Import::cMangled },
  { "DAQmxGetBufInputBufSize", (void**)&DAQmxGetBufInputBufSize, Dylib::Import::cMangled },
  { "DAQmxGetBufInputOnbrdBufSize", (void**)&DAQmxGetBufInputOnbrdBufSize, Dylib::Import::cMangled },
  { "DAQmxGetBufOutputBufSize", (void**)&DAQmxGetBufOutputBufSize, Dylib::Import::cMangled },
  { "DAQmxGetBufOutputOnbrdBufSize", (void**)&DAQmxGetBufOutputOnbrdBufSize, Dylib::Import::cMangled },
  { "DAQmxGetBufferAttribute", (void**)&DAQmxGetBufferAttribute, Dylib::Import::cMangled },
  { "DAQmxGetCIAngEncoderInitialAngle", (void**)&DAQmxGetCIAngEncoderInitialAngle, Dylib::Import::cMangled },
  { "DAQmxGetCIAngEncoderPulsesPerRev", (void**)&DAQmxGetCIAngEncoderPulsesPerRev, Dylib::Import::cMangled },
  { "DAQmxGetCIAngEncoderUnits", (void**)&DAQmxGetCIAngEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxGetCICount", (void**)&DAQmxGetCICount, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesActiveEdge", (void**)&DAQmxGetCICountEdgesActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirDigFltrEnable", (void**)&DAQmxGetCICountEdgesCountDirDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirDigFltrMinPulseWidth", (void**)&DAQmxGetCICountEdgesCountDirDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirDigFltrTimebaseRate", (void**)&DAQmxGetCICountEdgesCountDirDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirDigFltrTimebaseSrc", (void**)&DAQmxGetCICountEdgesCountDirDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirDigSyncEnable", (void**)&DAQmxGetCICountEdgesCountDirDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirHyst", (void**)&DAQmxGetCICountEdgesCountDirHyst, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirLogicLvlBehavior", (void**)&DAQmxGetCICountEdgesCountDirLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirTermCfg", (void**)&DAQmxGetCICountEdgesCountDirTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountDirThreshVoltage", (void**)&DAQmxGetCICountEdgesCountDirThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetActiveEdge", (void**)&DAQmxGetCICountEdgesCountResetActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetDigFltrEnable", (void**)&DAQmxGetCICountEdgesCountResetDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetDigFltrMinPulseWidth", (void**)&DAQmxGetCICountEdgesCountResetDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetDigFltrTimebaseRate", (void**)&DAQmxGetCICountEdgesCountResetDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetDigFltrTimebaseSrc", (void**)&DAQmxGetCICountEdgesCountResetDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetDigSyncEnable", (void**)&DAQmxGetCICountEdgesCountResetDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetEnable", (void**)&DAQmxGetCICountEdgesCountResetEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetHyst", (void**)&DAQmxGetCICountEdgesCountResetHyst, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetLogicLvlBehavior", (void**)&DAQmxGetCICountEdgesCountResetLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetResetCount", (void**)&DAQmxGetCICountEdgesCountResetResetCount, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetTerm", (void**)&DAQmxGetCICountEdgesCountResetTerm, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetTermCfg", (void**)&DAQmxGetCICountEdgesCountResetTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesCountResetThreshVoltage", (void**)&DAQmxGetCICountEdgesCountResetThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDigFltrEnable", (void**)&DAQmxGetCICountEdgesDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDigFltrMinPulseWidth", (void**)&DAQmxGetCICountEdgesDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDigFltrTimebaseRate", (void**)&DAQmxGetCICountEdgesDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDigFltrTimebaseSrc", (void**)&DAQmxGetCICountEdgesDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDigSyncEnable", (void**)&DAQmxGetCICountEdgesDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDir", (void**)&DAQmxGetCICountEdgesDir, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesDirTerm", (void**)&DAQmxGetCICountEdgesDirTerm, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateDigFltrEnable", (void**)&DAQmxGetCICountEdgesGateDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateDigFltrMinPulseWidth", (void**)&DAQmxGetCICountEdgesGateDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateDigFltrTimebaseRate", (void**)&DAQmxGetCICountEdgesGateDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateDigFltrTimebaseSrc", (void**)&DAQmxGetCICountEdgesGateDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateEnable", (void**)&DAQmxGetCICountEdgesGateEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateHyst", (void**)&DAQmxGetCICountEdgesGateHyst, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateLogicLvlBehavior", (void**)&DAQmxGetCICountEdgesGateLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateTerm", (void**)&DAQmxGetCICountEdgesGateTerm, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateTermCfg", (void**)&DAQmxGetCICountEdgesGateTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateThreshVoltage", (void**)&DAQmxGetCICountEdgesGateThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesGateWhen", (void**)&DAQmxGetCICountEdgesGateWhen, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesHyst", (void**)&DAQmxGetCICountEdgesHyst, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesInitialCnt", (void**)&DAQmxGetCICountEdgesInitialCnt, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesLogicLvlBehavior", (void**)&DAQmxGetCICountEdgesLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesTerm", (void**)&DAQmxGetCICountEdgesTerm, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesTermCfg", (void**)&DAQmxGetCICountEdgesTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCICountEdgesThreshVoltage", (void**)&DAQmxGetCICountEdgesThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseActiveEdge", (void**)&DAQmxGetCICtrTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseDigFltrEnable", (void**)&DAQmxGetCICtrTimebaseDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseDigFltrMinPulseWidth", (void**)&DAQmxGetCICtrTimebaseDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseDigFltrTimebaseRate", (void**)&DAQmxGetCICtrTimebaseDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseDigFltrTimebaseSrc", (void**)&DAQmxGetCICtrTimebaseDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseDigSyncEnable", (void**)&DAQmxGetCICtrTimebaseDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseMasterTimebaseDiv", (void**)&DAQmxGetCICtrTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseRate", (void**)&DAQmxGetCICtrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCICtrTimebaseSrc", (void**)&DAQmxGetCICtrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCICustomScaleName", (void**)&DAQmxGetCICustomScaleName, Dylib::Import::cMangled },
  { "DAQmxGetCIDataXferMech", (void**)&DAQmxGetCIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxGetCIDataXferReqCond", (void**)&DAQmxGetCIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxGetCIDupCountPrevent", (void**)&DAQmxGetCIDupCountPrevent, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleDigFltrEnable", (void**)&DAQmxGetCIDutyCycleDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleDigFltrMinPulseWidth", (void**)&DAQmxGetCIDutyCycleDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleDigFltrTimebaseRate", (void**)&DAQmxGetCIDutyCycleDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleDigFltrTimebaseSrc", (void**)&DAQmxGetCIDutyCycleDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleLogicLvlBehavior", (void**)&DAQmxGetCIDutyCycleLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleStartingEdge", (void**)&DAQmxGetCIDutyCycleStartingEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleTerm", (void**)&DAQmxGetCIDutyCycleTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIDutyCycleTermCfg", (void**)&DAQmxGetCIDutyCycleTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputDigFltrEnable", (void**)&DAQmxGetCIEncoderAInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputDigFltrMinPulseWidth", (void**)&DAQmxGetCIEncoderAInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputDigFltrTimebaseRate", (void**)&DAQmxGetCIEncoderAInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputDigFltrTimebaseSrc", (void**)&DAQmxGetCIEncoderAInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputDigSyncEnable", (void**)&DAQmxGetCIEncoderAInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputLogicLvlBehavior", (void**)&DAQmxGetCIEncoderAInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputTerm", (void**)&DAQmxGetCIEncoderAInputTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderAInputTermCfg", (void**)&DAQmxGetCIEncoderAInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputDigFltrEnable", (void**)&DAQmxGetCIEncoderBInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputDigFltrMinPulseWidth", (void**)&DAQmxGetCIEncoderBInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputDigFltrTimebaseRate", (void**)&DAQmxGetCIEncoderBInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputDigFltrTimebaseSrc", (void**)&DAQmxGetCIEncoderBInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputDigSyncEnable", (void**)&DAQmxGetCIEncoderBInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputLogicLvlBehavior", (void**)&DAQmxGetCIEncoderBInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputTerm", (void**)&DAQmxGetCIEncoderBInputTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderBInputTermCfg", (void**)&DAQmxGetCIEncoderBInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderDecodingType", (void**)&DAQmxGetCIEncoderDecodingType, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZIndexEnable", (void**)&DAQmxGetCIEncoderZIndexEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZIndexPhase", (void**)&DAQmxGetCIEncoderZIndexPhase, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZIndexVal", (void**)&DAQmxGetCIEncoderZIndexVal, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputDigFltrEnable", (void**)&DAQmxGetCIEncoderZInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputDigFltrMinPulseWidth", (void**)&DAQmxGetCIEncoderZInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputDigFltrTimebaseRate", (void**)&DAQmxGetCIEncoderZInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputDigFltrTimebaseSrc", (void**)&DAQmxGetCIEncoderZInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputDigSyncEnable", (void**)&DAQmxGetCIEncoderZInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputLogicLvlBehavior", (void**)&DAQmxGetCIEncoderZInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputTerm", (void**)&DAQmxGetCIEncoderZInputTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIEncoderZInputTermCfg", (void**)&DAQmxGetCIEncoderZInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIFilterDelay", (void**)&DAQmxGetCIFilterDelay, Dylib::Import::cMangled },
  { "DAQmxGetCIFilterDelayUnits", (void**)&DAQmxGetCIFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIFilterEnable", (void**)&DAQmxGetCIFilterEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIFilterFreq", (void**)&DAQmxGetCIFilterFreq, Dylib::Import::cMangled },
  { "DAQmxGetCIFilterOrder", (void**)&DAQmxGetCIFilterOrder, Dylib::Import::cMangled },
  { "DAQmxGetCIFilterResponse", (void**)&DAQmxGetCIFilterResponse, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqDigFltrEnable", (void**)&DAQmxGetCIFreqDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqDigFltrMinPulseWidth", (void**)&DAQmxGetCIFreqDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqDigFltrTimebaseRate", (void**)&DAQmxGetCIFreqDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqDigFltrTimebaseSrc", (void**)&DAQmxGetCIFreqDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqDigSyncEnable", (void**)&DAQmxGetCIFreqDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqDiv", (void**)&DAQmxGetCIFreqDiv, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqEnableAveraging", (void**)&DAQmxGetCIFreqEnableAveraging, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqHyst", (void**)&DAQmxGetCIFreqHyst, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqLogicLvlBehavior", (void**)&DAQmxGetCIFreqLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqMeasMeth", (void**)&DAQmxGetCIFreqMeasMeth, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqMeasTime", (void**)&DAQmxGetCIFreqMeasTime, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqStartingEdge", (void**)&DAQmxGetCIFreqStartingEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqTerm", (void**)&DAQmxGetCIFreqTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqTermCfg", (void**)&DAQmxGetCIFreqTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqThreshVoltage", (void**)&DAQmxGetCIFreqThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCIFreqUnits", (void**)&DAQmxGetCIFreqUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIGPSSyncMethod", (void**)&DAQmxGetCIGPSSyncMethod, Dylib::Import::cMangled },
  { "DAQmxGetCIGPSSyncSrc", (void**)&DAQmxGetCIGPSSyncSrc, Dylib::Import::cMangled },
  { "DAQmxGetCILinEncoderDistPerPulse", (void**)&DAQmxGetCILinEncoderDistPerPulse, Dylib::Import::cMangled },
  { "DAQmxGetCILinEncoderInitialPos", (void**)&DAQmxGetCILinEncoderInitialPos, Dylib::Import::cMangled },
  { "DAQmxGetCILinEncoderUnits", (void**)&DAQmxGetCILinEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIMax", (void**)&DAQmxGetCIMax, Dylib::Import::cMangled },
  { "DAQmxGetCIMaxMeasPeriod", (void**)&DAQmxGetCIMaxMeasPeriod, Dylib::Import::cMangled },
  { "DAQmxGetCIMeasType", (void**)&DAQmxGetCIMeasType, Dylib::Import::cMangled },
  { "DAQmxGetCIMemMapEnable", (void**)&DAQmxGetCIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIMin", (void**)&DAQmxGetCIMin, Dylib::Import::cMangled },
  { "DAQmxGetCINumPossiblyInvalidSamps", (void**)&DAQmxGetCINumPossiblyInvalidSamps, Dylib::Import::cMangled },
  { "DAQmxGetCIOutputState", (void**)&DAQmxGetCIOutputState, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodDigFltrEnable", (void**)&DAQmxGetCIPeriodDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodDigFltrMinPulseWidth", (void**)&DAQmxGetCIPeriodDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodDigFltrTimebaseRate", (void**)&DAQmxGetCIPeriodDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodDigFltrTimebaseSrc", (void**)&DAQmxGetCIPeriodDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodDigSyncEnable", (void**)&DAQmxGetCIPeriodDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodDiv", (void**)&DAQmxGetCIPeriodDiv, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodEnableAveraging", (void**)&DAQmxGetCIPeriodEnableAveraging, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodHyst", (void**)&DAQmxGetCIPeriodHyst, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodLogicLvlBehavior", (void**)&DAQmxGetCIPeriodLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodMeasMeth", (void**)&DAQmxGetCIPeriodMeasMeth, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodMeasTime", (void**)&DAQmxGetCIPeriodMeasTime, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodStartingEdge", (void**)&DAQmxGetCIPeriodStartingEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodTerm", (void**)&DAQmxGetCIPeriodTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodTermCfg", (void**)&DAQmxGetCIPeriodTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodThreshVoltage", (void**)&DAQmxGetCIPeriodThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCIPeriodUnits", (void**)&DAQmxGetCIPeriodUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIPrescaler", (void**)&DAQmxGetCIPrescaler, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqDigFltrEnable", (void**)&DAQmxGetCIPulseFreqDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqDigFltrMinPulseWidth", (void**)&DAQmxGetCIPulseFreqDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqDigFltrTimebaseRate", (void**)&DAQmxGetCIPulseFreqDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqDigFltrTimebaseSrc", (void**)&DAQmxGetCIPulseFreqDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqDigSyncEnable", (void**)&DAQmxGetCIPulseFreqDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqLogicLvlBehavior", (void**)&DAQmxGetCIPulseFreqLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqStartEdge", (void**)&DAQmxGetCIPulseFreqStartEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqTerm", (void**)&DAQmxGetCIPulseFreqTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqTermCfg", (void**)&DAQmxGetCIPulseFreqTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseFreqUnits", (void**)&DAQmxGetCIPulseFreqUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksDigFltrEnable", (void**)&DAQmxGetCIPulseTicksDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksDigFltrMinPulseWidth", (void**)&DAQmxGetCIPulseTicksDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksDigFltrTimebaseRate", (void**)&DAQmxGetCIPulseTicksDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksDigFltrTimebaseSrc", (void**)&DAQmxGetCIPulseTicksDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksDigSyncEnable", (void**)&DAQmxGetCIPulseTicksDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksLogicLvlBehavior", (void**)&DAQmxGetCIPulseTicksLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksStartEdge", (void**)&DAQmxGetCIPulseTicksStartEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksTerm", (void**)&DAQmxGetCIPulseTicksTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTicksTermCfg", (void**)&DAQmxGetCIPulseTicksTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeDigFltrEnable", (void**)&DAQmxGetCIPulseTimeDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeDigFltrMinPulseWidth", (void**)&DAQmxGetCIPulseTimeDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeDigFltrTimebaseRate", (void**)&DAQmxGetCIPulseTimeDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeDigFltrTimebaseSrc", (void**)&DAQmxGetCIPulseTimeDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeDigSyncEnable", (void**)&DAQmxGetCIPulseTimeDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeLogicLvlBehavior", (void**)&DAQmxGetCIPulseTimeLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeStartEdge", (void**)&DAQmxGetCIPulseTimeStartEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeTerm", (void**)&DAQmxGetCIPulseTimeTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeTermCfg", (void**)&DAQmxGetCIPulseTimeTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseTimeUnits", (void**)&DAQmxGetCIPulseTimeUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthDigFltrEnable", (void**)&DAQmxGetCIPulseWidthDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthDigFltrMinPulseWidth", (void**)&DAQmxGetCIPulseWidthDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthDigFltrTimebaseRate", (void**)&DAQmxGetCIPulseWidthDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthDigFltrTimebaseSrc", (void**)&DAQmxGetCIPulseWidthDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthDigSyncEnable", (void**)&DAQmxGetCIPulseWidthDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthLogicLvlBehavior", (void**)&DAQmxGetCIPulseWidthLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthStartingEdge", (void**)&DAQmxGetCIPulseWidthStartingEdge, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthTerm", (void**)&DAQmxGetCIPulseWidthTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthTermCfg", (void**)&DAQmxGetCIPulseWidthTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIPulseWidthUnits", (void**)&DAQmxGetCIPulseWidthUnits, Dylib::Import::cMangled },
  { "DAQmxGetCISampClkOverrunBehavior", (void**)&DAQmxGetCISampClkOverrunBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCISampClkOverrunSentinelVal", (void**)&DAQmxGetCISampClkOverrunSentinelVal, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodDigFltrEnable", (void**)&DAQmxGetCISemiPeriodDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodDigFltrMinPulseWidth", (void**)&DAQmxGetCISemiPeriodDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodDigFltrTimebaseRate", (void**)&DAQmxGetCISemiPeriodDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodDigFltrTimebaseSrc", (void**)&DAQmxGetCISemiPeriodDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodDigSyncEnable", (void**)&DAQmxGetCISemiPeriodDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodLogicLvlBehavior", (void**)&DAQmxGetCISemiPeriodLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodStartingEdge", (void**)&DAQmxGetCISemiPeriodStartingEdge, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodTerm", (void**)&DAQmxGetCISemiPeriodTerm, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodTermCfg", (void**)&DAQmxGetCISemiPeriodTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCISemiPeriodUnits", (void**)&DAQmxGetCISemiPeriodUnits, Dylib::Import::cMangled },
  { "DAQmxGetCITCReached", (void**)&DAQmxGetCITCReached, Dylib::Import::cMangled },
  { "DAQmxGetCIThreshVoltage", (void**)&DAQmxGetCIThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxGetCITimestampInitialSeconds", (void**)&DAQmxGetCITimestampInitialSeconds, Dylib::Import::cMangled },
  { "DAQmxGetCITimestampUnits", (void**)&DAQmxGetCITimestampUnits, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstDigFltrEnable", (void**)&DAQmxGetCITwoEdgeSepFirstDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstDigFltrMinPulseWidth", (void**)&DAQmxGetCITwoEdgeSepFirstDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseRate", (void**)&DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseSrc", (void**)&DAQmxGetCITwoEdgeSepFirstDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstDigSyncEnable", (void**)&DAQmxGetCITwoEdgeSepFirstDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstEdge", (void**)&DAQmxGetCITwoEdgeSepFirstEdge, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstLogicLvlBehavior", (void**)&DAQmxGetCITwoEdgeSepFirstLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstTerm", (void**)&DAQmxGetCITwoEdgeSepFirstTerm, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepFirstTermCfg", (void**)&DAQmxGetCITwoEdgeSepFirstTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondDigFltrEnable", (void**)&DAQmxGetCITwoEdgeSepSecondDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondDigFltrMinPulseWidth", (void**)&DAQmxGetCITwoEdgeSepSecondDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseRate", (void**)&DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseSrc", (void**)&DAQmxGetCITwoEdgeSepSecondDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondDigSyncEnable", (void**)&DAQmxGetCITwoEdgeSepSecondDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondEdge", (void**)&DAQmxGetCITwoEdgeSepSecondEdge, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondLogicLvlBehavior", (void**)&DAQmxGetCITwoEdgeSepSecondLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondTerm", (void**)&DAQmxGetCITwoEdgeSepSecondTerm, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepSecondTermCfg", (void**)&DAQmxGetCITwoEdgeSepSecondTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCITwoEdgeSepUnits", (void**)&DAQmxGetCITwoEdgeSepUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIUsbXferReqCount", (void**)&DAQmxGetCIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxGetCIUsbXferReqSize", (void**)&DAQmxGetCIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityAngEncoderPulsesPerRev", (void**)&DAQmxGetCIVelocityAngEncoderPulsesPerRev, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityAngEncoderUnits", (void**)&DAQmxGetCIVelocityAngEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityDiv", (void**)&DAQmxGetCIVelocityDiv, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputDigFltrEnable", (void**)&DAQmxGetCIVelocityEncoderAInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputDigFltrMinPulseWidth", (void**)&DAQmxGetCIVelocityEncoderAInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseRate", (void**)&DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseSrc", (void**)&DAQmxGetCIVelocityEncoderAInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputLogicLvlBehavior", (void**)&DAQmxGetCIVelocityEncoderAInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputTerm", (void**)&DAQmxGetCIVelocityEncoderAInputTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderAInputTermCfg", (void**)&DAQmxGetCIVelocityEncoderAInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputDigFltrEnable", (void**)&DAQmxGetCIVelocityEncoderBInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputDigFltrMinPulseWidth", (void**)&DAQmxGetCIVelocityEncoderBInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseRate", (void**)&DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseSrc", (void**)&DAQmxGetCIVelocityEncoderBInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputLogicLvlBehavior", (void**)&DAQmxGetCIVelocityEncoderBInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputTerm", (void**)&DAQmxGetCIVelocityEncoderBInputTerm, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderBInputTermCfg", (void**)&DAQmxGetCIVelocityEncoderBInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityEncoderDecodingType", (void**)&DAQmxGetCIVelocityEncoderDecodingType, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityLinEncoderDistPerPulse", (void**)&DAQmxGetCIVelocityLinEncoderDistPerPulse, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityLinEncoderUnits", (void**)&DAQmxGetCIVelocityLinEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxGetCIVelocityMeasTime", (void**)&DAQmxGetCIVelocityMeasTime, Dylib::Import::cMangled },
  { "DAQmxGetCOAutoIncrCnt", (void**)&DAQmxGetCOAutoIncrCnt, Dylib::Import::cMangled },
  { "DAQmxGetCOConstrainedGenMode", (void**)&DAQmxGetCOConstrainedGenMode, Dylib::Import::cMangled },
  { "DAQmxGetCOCount", (void**)&DAQmxGetCOCount, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseActiveEdge", (void**)&DAQmxGetCOCtrTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseDigFltrEnable", (void**)&DAQmxGetCOCtrTimebaseDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseDigFltrMinPulseWidth", (void**)&DAQmxGetCOCtrTimebaseDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseDigFltrTimebaseRate", (void**)&DAQmxGetCOCtrTimebaseDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseDigFltrTimebaseSrc", (void**)&DAQmxGetCOCtrTimebaseDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseDigSyncEnable", (void**)&DAQmxGetCOCtrTimebaseDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseMasterTimebaseDiv", (void**)&DAQmxGetCOCtrTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseRate", (void**)&DAQmxGetCOCtrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetCOCtrTimebaseSrc", (void**)&DAQmxGetCOCtrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetCODataXferMech", (void**)&DAQmxGetCODataXferMech, Dylib::Import::cMangled },
  { "DAQmxGetCODataXferReqCond", (void**)&DAQmxGetCODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxGetCOEnableInitialDelayOnRetrigger", (void**)&DAQmxGetCOEnableInitialDelayOnRetrigger, Dylib::Import::cMangled },
  { "DAQmxGetCOMemMapEnable", (void**)&DAQmxGetCOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxGetCOOutputState", (void**)&DAQmxGetCOOutputState, Dylib::Import::cMangled },
  { "DAQmxGetCOOutputType", (void**)&DAQmxGetCOOutputType, Dylib::Import::cMangled },
  { "DAQmxGetCOPrescaler", (void**)&DAQmxGetCOPrescaler, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseDone", (void**)&DAQmxGetCOPulseDone, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseDutyCyc", (void**)&DAQmxGetCOPulseDutyCyc, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseFreq", (void**)&DAQmxGetCOPulseFreq, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseFreqInitialDelay", (void**)&DAQmxGetCOPulseFreqInitialDelay, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseFreqUnits", (void**)&DAQmxGetCOPulseFreqUnits, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseHighTicks", (void**)&DAQmxGetCOPulseHighTicks, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseHighTime", (void**)&DAQmxGetCOPulseHighTime, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseIdleState", (void**)&DAQmxGetCOPulseIdleState, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseLowTicks", (void**)&DAQmxGetCOPulseLowTicks, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseLowTime", (void**)&DAQmxGetCOPulseLowTime, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseTerm", (void**)&DAQmxGetCOPulseTerm, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseTicksInitialDelay", (void**)&DAQmxGetCOPulseTicksInitialDelay, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseTimeInitialDelay", (void**)&DAQmxGetCOPulseTimeInitialDelay, Dylib::Import::cMangled },
  { "DAQmxGetCOPulseTimeUnits", (void**)&DAQmxGetCOPulseTimeUnits, Dylib::Import::cMangled },
  { "DAQmxGetCORdyForNewVal", (void**)&DAQmxGetCORdyForNewVal, Dylib::Import::cMangled },
  { "DAQmxGetCOUsbXferReqCount", (void**)&DAQmxGetCOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxGetCOUsbXferReqSize", (void**)&DAQmxGetCOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxGetCOUseOnlyOnBrdMem", (void**)&DAQmxGetCOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxGetCalAccConnectionCount", (void**)&DAQmxGetCalAccConnectionCount, Dylib::Import::cMangled },
  { "DAQmxGetCalDevTemp", (void**)&DAQmxGetCalDevTemp, Dylib::Import::cMangled },
  { "DAQmxGetCalInfoAttribute", (void**)&DAQmxGetCalInfoAttribute, Dylib::Import::cMangled },
  { "DAQmxGetCalRecommendedAccConnectionCountLimit", (void**)&DAQmxGetCalRecommendedAccConnectionCountLimit, Dylib::Import::cMangled },
  { "DAQmxGetCalUserDefinedInfo", (void**)&DAQmxGetCalUserDefinedInfo, Dylib::Import::cMangled },
  { "DAQmxGetCalUserDefinedInfoMaxSize", (void**)&DAQmxGetCalUserDefinedInfoMaxSize, Dylib::Import::cMangled },
  { "DAQmxGetCarrierSerialNum", (void**)&DAQmxGetCarrierSerialNum, Dylib::Import::cMangled },
  { "DAQmxGetChanAttribute", (void**)&DAQmxGetChanAttribute, Dylib::Import::cMangled },
  { "DAQmxGetChanDescr", (void**)&DAQmxGetChanDescr, Dylib::Import::cMangled },
  { "DAQmxGetChanIsGlobal", (void**)&DAQmxGetChanIsGlobal, Dylib::Import::cMangled },
  { "DAQmxGetChanSyncUnlockBehavior", (void**)&DAQmxGetChanSyncUnlockBehavior, Dylib::Import::cMangled },
  { "DAQmxGetChanType", (void**)&DAQmxGetChanType, Dylib::Import::cMangled },
  { "DAQmxGetChangeDetectDIFallingEdgePhysicalChans", (void**)&DAQmxGetChangeDetectDIFallingEdgePhysicalChans, Dylib::Import::cMangled },
  { "DAQmxGetChangeDetectDIRisingEdgePhysicalChans", (void**)&DAQmxGetChangeDetectDIRisingEdgePhysicalChans, Dylib::Import::cMangled },
  { "DAQmxGetChangeDetectDITristate", (void**)&DAQmxGetChangeDetectDITristate, Dylib::Import::cMangled },
  { "DAQmxGetDIAcquireOn", (void**)&DAQmxGetDIAcquireOn, Dylib::Import::cMangled },
  { "DAQmxGetDIDataXferMech", (void**)&DAQmxGetDIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxGetDIDataXferReqCond", (void**)&DAQmxGetDIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxGetDIDigFltrEnable", (void**)&DAQmxGetDIDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetDIDigFltrEnableBusMode", (void**)&DAQmxGetDIDigFltrEnableBusMode, Dylib::Import::cMangled },
  { "DAQmxGetDIDigFltrMinPulseWidth", (void**)&DAQmxGetDIDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetDIDigFltrTimebaseRate", (void**)&DAQmxGetDIDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetDIDigFltrTimebaseSrc", (void**)&DAQmxGetDIDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetDIDigSyncEnable", (void**)&DAQmxGetDIDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetDIInvertLines", (void**)&DAQmxGetDIInvertLines, Dylib::Import::cMangled },
  { "DAQmxGetDILogicFamily", (void**)&DAQmxGetDILogicFamily, Dylib::Import::cMangled },
  { "DAQmxGetDIMemMapEnable", (void**)&DAQmxGetDIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxGetDINumLines", (void**)&DAQmxGetDINumLines, Dylib::Import::cMangled },
  { "DAQmxGetDITristate", (void**)&DAQmxGetDITristate, Dylib::Import::cMangled },
  { "DAQmxGetDIUsbXferReqCount", (void**)&DAQmxGetDIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxGetDIUsbXferReqSize", (void**)&DAQmxGetDIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxGetDODataXferMech", (void**)&DAQmxGetDODataXferMech, Dylib::Import::cMangled },
  { "DAQmxGetDODataXferReqCond", (void**)&DAQmxGetDODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxGetDOGenerateOn", (void**)&DAQmxGetDOGenerateOn, Dylib::Import::cMangled },
  { "DAQmxGetDOInvertLines", (void**)&DAQmxGetDOInvertLines, Dylib::Import::cMangled },
  { "DAQmxGetDOLineStatesDoneState", (void**)&DAQmxGetDOLineStatesDoneState, Dylib::Import::cMangled },
  { "DAQmxGetDOLineStatesPausedState", (void**)&DAQmxGetDOLineStatesPausedState, Dylib::Import::cMangled },
  { "DAQmxGetDOLineStatesStartState", (void**)&DAQmxGetDOLineStatesStartState, Dylib::Import::cMangled },
  { "DAQmxGetDOLogicFamily", (void**)&DAQmxGetDOLogicFamily, Dylib::Import::cMangled },
  { "DAQmxGetDOMemMapEnable", (void**)&DAQmxGetDOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxGetDONumLines", (void**)&DAQmxGetDONumLines, Dylib::Import::cMangled },
  { "DAQmxGetDOOutputDriveType", (void**)&DAQmxGetDOOutputDriveType, Dylib::Import::cMangled },
  { "DAQmxGetDOOvercurrentAutoReenable", (void**)&DAQmxGetDOOvercurrentAutoReenable, Dylib::Import::cMangled },
  { "DAQmxGetDOOvercurrentLimit", (void**)&DAQmxGetDOOvercurrentLimit, Dylib::Import::cMangled },
  { "DAQmxGetDOOvercurrentReenablePeriod", (void**)&DAQmxGetDOOvercurrentReenablePeriod, Dylib::Import::cMangled },
  { "DAQmxGetDOTristate", (void**)&DAQmxGetDOTristate, Dylib::Import::cMangled },
  { "DAQmxGetDOUsbXferReqCount", (void**)&DAQmxGetDOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxGetDOUsbXferReqSize", (void**)&DAQmxGetDOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxGetDOUseOnlyOnBrdMem", (void**)&DAQmxGetDOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxGetDelayFromSampClkDelay", (void**)&DAQmxGetDelayFromSampClkDelay, Dylib::Import::cMangled },
  { "DAQmxGetDelayFromSampClkDelayEx", (void**)&DAQmxGetDelayFromSampClkDelayEx, Dylib::Import::cMangled },
  { "DAQmxGetDelayFromSampClkDelayUnits", (void**)&DAQmxGetDelayFromSampClkDelayUnits, Dylib::Import::cMangled },
  { "DAQmxGetDelayFromSampClkDelayUnitsEx", (void**)&DAQmxGetDelayFromSampClkDelayUnitsEx, Dylib::Import::cMangled },
  { "DAQmxGetDevAIBridgeRngs", (void**)&DAQmxGetDevAIBridgeRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAIChargeRngs", (void**)&DAQmxGetDevAIChargeRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAICouplings", (void**)&DAQmxGetDevAICouplings, Dylib::Import::cMangled },
  { "DAQmxGetDevAICurrentIntExcitDiscreteVals", (void**)&DAQmxGetDevAICurrentIntExcitDiscreteVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAICurrentRngs", (void**)&DAQmxGetDevAICurrentRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAIDigFltrLowpassCutoffFreqDiscreteVals", (void**)&DAQmxGetDevAIDigFltrLowpassCutoffFreqDiscreteVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAIDigFltrLowpassCutoffFreqRangeVals", (void**)&DAQmxGetDevAIDigFltrLowpassCutoffFreqRangeVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAIFreqRngs", (void**)&DAQmxGetDevAIFreqRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAIGains", (void**)&DAQmxGetDevAIGains, Dylib::Import::cMangled },
  { "DAQmxGetDevAILowpassCutoffFreqDiscreteVals", (void**)&DAQmxGetDevAILowpassCutoffFreqDiscreteVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAILowpassCutoffFreqRangeVals", (void**)&DAQmxGetDevAILowpassCutoffFreqRangeVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAIMaxMultiChanRate", (void**)&DAQmxGetDevAIMaxMultiChanRate, Dylib::Import::cMangled },
  { "DAQmxGetDevAIMaxSingleChanRate", (void**)&DAQmxGetDevAIMaxSingleChanRate, Dylib::Import::cMangled },
  { "DAQmxGetDevAIMinRate", (void**)&DAQmxGetDevAIMinRate, Dylib::Import::cMangled },
  { "DAQmxGetDevAINumSampTimingEngines", (void**)&DAQmxGetDevAINumSampTimingEngines, Dylib::Import::cMangled },
  { "DAQmxGetDevAINumSyncPulseSrcs", (void**)&DAQmxGetDevAINumSyncPulseSrcs, Dylib::Import::cMangled },
  { "DAQmxGetDevAIPhysicalChans", (void**)&DAQmxGetDevAIPhysicalChans, Dylib::Import::cMangled },
  { "DAQmxGetDevAIResistanceRngs", (void**)&DAQmxGetDevAIResistanceRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAISampModes", (void**)&DAQmxGetDevAISampModes, Dylib::Import::cMangled },
  { "DAQmxGetDevAISimultaneousSamplingSupported", (void**)&DAQmxGetDevAISimultaneousSamplingSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevAISupportedMeasTypes", (void**)&DAQmxGetDevAISupportedMeasTypes, Dylib::Import::cMangled },
  { "DAQmxGetDevAITrigUsage", (void**)&DAQmxGetDevAITrigUsage, Dylib::Import::cMangled },
  { "DAQmxGetDevAIVoltageIntExcitDiscreteVals", (void**)&DAQmxGetDevAIVoltageIntExcitDiscreteVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAIVoltageIntExcitRangeVals", (void**)&DAQmxGetDevAIVoltageIntExcitRangeVals, Dylib::Import::cMangled },
  { "DAQmxGetDevAIVoltageRngs", (void**)&DAQmxGetDevAIVoltageRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAOCurrentRngs", (void**)&DAQmxGetDevAOCurrentRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAOGains", (void**)&DAQmxGetDevAOGains, Dylib::Import::cMangled },
  { "DAQmxGetDevAOMaxRate", (void**)&DAQmxGetDevAOMaxRate, Dylib::Import::cMangled },
  { "DAQmxGetDevAOMinRate", (void**)&DAQmxGetDevAOMinRate, Dylib::Import::cMangled },
  { "DAQmxGetDevAONumSampTimingEngines", (void**)&DAQmxGetDevAONumSampTimingEngines, Dylib::Import::cMangled },
  { "DAQmxGetDevAONumSyncPulseSrcs", (void**)&DAQmxGetDevAONumSyncPulseSrcs, Dylib::Import::cMangled },
  { "DAQmxGetDevAOPhysicalChans", (void**)&DAQmxGetDevAOPhysicalChans, Dylib::Import::cMangled },
  { "DAQmxGetDevAOSampClkSupported", (void**)&DAQmxGetDevAOSampClkSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevAOSampModes", (void**)&DAQmxGetDevAOSampModes, Dylib::Import::cMangled },
  { "DAQmxGetDevAOSupportedOutputTypes", (void**)&DAQmxGetDevAOSupportedOutputTypes, Dylib::Import::cMangled },
  { "DAQmxGetDevAOTrigUsage", (void**)&DAQmxGetDevAOTrigUsage, Dylib::Import::cMangled },
  { "DAQmxGetDevAOVoltageRngs", (void**)&DAQmxGetDevAOVoltageRngs, Dylib::Import::cMangled },
  { "DAQmxGetDevAccessoryProductNums", (void**)&DAQmxGetDevAccessoryProductNums, Dylib::Import::cMangled },
  { "DAQmxGetDevAccessoryProductTypes", (void**)&DAQmxGetDevAccessoryProductTypes, Dylib::Import::cMangled },
  { "DAQmxGetDevAccessorySerialNums", (void**)&DAQmxGetDevAccessorySerialNums, Dylib::Import::cMangled },
  { "DAQmxGetDevAnlgTrigSupported", (void**)&DAQmxGetDevAnlgTrigSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevBusType", (void**)&DAQmxGetDevBusType, Dylib::Import::cMangled },
  { "DAQmxGetDevCIMaxSize", (void**)&DAQmxGetDevCIMaxSize, Dylib::Import::cMangled },
  { "DAQmxGetDevCIMaxTimebase", (void**)&DAQmxGetDevCIMaxTimebase, Dylib::Import::cMangled },
  { "DAQmxGetDevCIPhysicalChans", (void**)&DAQmxGetDevCIPhysicalChans, Dylib::Import::cMangled },
  { "DAQmxGetDevCISampClkSupported", (void**)&DAQmxGetDevCISampClkSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevCISampModes", (void**)&DAQmxGetDevCISampModes, Dylib::Import::cMangled },
  { "DAQmxGetDevCISupportedMeasTypes", (void**)&DAQmxGetDevCISupportedMeasTypes, Dylib::Import::cMangled },
  { "DAQmxGetDevCITrigUsage", (void**)&DAQmxGetDevCITrigUsage, Dylib::Import::cMangled },
  { "DAQmxGetDevCOMaxSize", (void**)&DAQmxGetDevCOMaxSize, Dylib::Import::cMangled },
  { "DAQmxGetDevCOMaxTimebase", (void**)&DAQmxGetDevCOMaxTimebase, Dylib::Import::cMangled },
  { "DAQmxGetDevCOPhysicalChans", (void**)&DAQmxGetDevCOPhysicalChans, Dylib::Import::cMangled },
  { "DAQmxGetDevCOSampClkSupported", (void**)&DAQmxGetDevCOSampClkSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevCOSampModes", (void**)&DAQmxGetDevCOSampModes, Dylib::Import::cMangled },
  { "DAQmxGetDevCOSupportedOutputTypes", (void**)&DAQmxGetDevCOSupportedOutputTypes, Dylib::Import::cMangled },
  { "DAQmxGetDevCOTrigUsage", (void**)&DAQmxGetDevCOTrigUsage, Dylib::Import::cMangled },
  { "DAQmxGetDevChassisModuleDevNames", (void**)&DAQmxGetDevChassisModuleDevNames, Dylib::Import::cMangled },
  { "DAQmxGetDevCompactDAQChassisDevName", (void**)&DAQmxGetDevCompactDAQChassisDevName, Dylib::Import::cMangled },
  { "DAQmxGetDevCompactDAQSlotNum", (void**)&DAQmxGetDevCompactDAQSlotNum, Dylib::Import::cMangled },
  { "DAQmxGetDevCompactRIOChassisDevName", (void**)&DAQmxGetDevCompactRIOChassisDevName, Dylib::Import::cMangled },
  { "DAQmxGetDevCompactRIOSlotNum", (void**)&DAQmxGetDevCompactRIOSlotNum, Dylib::Import::cMangled },
  { "DAQmxGetDevDILines", (void**)&DAQmxGetDevDILines, Dylib::Import::cMangled },
  { "DAQmxGetDevDIMaxRate", (void**)&DAQmxGetDevDIMaxRate, Dylib::Import::cMangled },
  { "DAQmxGetDevDINumSampTimingEngines", (void**)&DAQmxGetDevDINumSampTimingEngines, Dylib::Import::cMangled },
  { "DAQmxGetDevDIPorts", (void**)&DAQmxGetDevDIPorts, Dylib::Import::cMangled },
  { "DAQmxGetDevDITrigUsage", (void**)&DAQmxGetDevDITrigUsage, Dylib::Import::cMangled },
  { "DAQmxGetDevDOLines", (void**)&DAQmxGetDevDOLines, Dylib::Import::cMangled },
  { "DAQmxGetDevDOMaxRate", (void**)&DAQmxGetDevDOMaxRate, Dylib::Import::cMangled },
  { "DAQmxGetDevDONumSampTimingEngines", (void**)&DAQmxGetDevDONumSampTimingEngines, Dylib::Import::cMangled },
  { "DAQmxGetDevDOPorts", (void**)&DAQmxGetDevDOPorts, Dylib::Import::cMangled },
  { "DAQmxGetDevDOTrigUsage", (void**)&DAQmxGetDevDOTrigUsage, Dylib::Import::cMangled },
  { "DAQmxGetDevDigTrigSupported", (void**)&DAQmxGetDevDigTrigSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevIsSimulated", (void**)&DAQmxGetDevIsSimulated, Dylib::Import::cMangled },
  { "DAQmxGetDevNumDMAChans", (void**)&DAQmxGetDevNumDMAChans, Dylib::Import::cMangled },
  { "DAQmxGetDevNumTimeTrigs", (void**)&DAQmxGetDevNumTimeTrigs, Dylib::Import::cMangled },
  { "DAQmxGetDevNumTimestampEngines", (void**)&DAQmxGetDevNumTimestampEngines, Dylib::Import::cMangled },
  { "DAQmxGetDevPCIBusNum", (void**)&DAQmxGetDevPCIBusNum, Dylib::Import::cMangled },
  { "DAQmxGetDevPCIDevNum", (void**)&DAQmxGetDevPCIDevNum, Dylib::Import::cMangled },
  { "DAQmxGetDevPXIChassisNum", (void**)&DAQmxGetDevPXIChassisNum, Dylib::Import::cMangled },
  { "DAQmxGetDevPXISlotNum", (void**)&DAQmxGetDevPXISlotNum, Dylib::Import::cMangled },
  { "DAQmxGetDevProductCategory", (void**)&DAQmxGetDevProductCategory, Dylib::Import::cMangled },
  { "DAQmxGetDevProductNum", (void**)&DAQmxGetDevProductNum, Dylib::Import::cMangled },
  { "DAQmxGetDevProductType", (void**)&DAQmxGetDevProductType, Dylib::Import::cMangled },
  { "DAQmxGetDevSerialNum", (void**)&DAQmxGetDevSerialNum, Dylib::Import::cMangled },
  { "DAQmxGetDevTCPIPEthernetIP", (void**)&DAQmxGetDevTCPIPEthernetIP, Dylib::Import::cMangled },
  { "DAQmxGetDevTCPIPHostname", (void**)&DAQmxGetDevTCPIPHostname, Dylib::Import::cMangled },
  { "DAQmxGetDevTCPIPWirelessIP", (void**)&DAQmxGetDevTCPIPWirelessIP, Dylib::Import::cMangled },
  { "DAQmxGetDevTEDSHWTEDSSupported", (void**)&DAQmxGetDevTEDSHWTEDSSupported, Dylib::Import::cMangled },
  { "DAQmxGetDevTerminals", (void**)&DAQmxGetDevTerminals, Dylib::Import::cMangled },
  { "DAQmxGetDevTimeTrigSupported", (void**)&DAQmxGetDevTimeTrigSupported, Dylib::Import::cMangled },
  { "DAQmxGetDeviceAttribute", (void**)&DAQmxGetDeviceAttribute, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeAdvTrigDigFltrEnable", (void**)&DAQmxGetDigEdgeAdvTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeAdvTrigEdge", (void**)&DAQmxGetDigEdgeAdvTrigEdge, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeAdvTrigSrc", (void**)&DAQmxGetDigEdgeAdvTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigDigFltrEnable", (void**)&DAQmxGetDigEdgeArmStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigDigFltrMinPulseWidth", (void**)&DAQmxGetDigEdgeArmStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseRate", (void**)&DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseSrc", (void**)&DAQmxGetDigEdgeArmStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigDigSyncEnable", (void**)&DAQmxGetDigEdgeArmStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigEdge", (void**)&DAQmxGetDigEdgeArmStartTrigEdge, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeArmStartTrigSrc", (void**)&DAQmxGetDigEdgeArmStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigDigFltrEnable", (void**)&DAQmxGetDigEdgeRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigDigFltrMinPulseWidth", (void**)&DAQmxGetDigEdgeRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigDigFltrTimebaseRate", (void**)&DAQmxGetDigEdgeRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigDigFltrTimebaseSrc", (void**)&DAQmxGetDigEdgeRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigDigSyncEnable", (void**)&DAQmxGetDigEdgeRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigEdge", (void**)&DAQmxGetDigEdgeRefTrigEdge, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeRefTrigSrc", (void**)&DAQmxGetDigEdgeRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigDigFltrEnable", (void**)&DAQmxGetDigEdgeStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigDigFltrMinPulseWidth", (void**)&DAQmxGetDigEdgeStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigDigFltrTimebaseRate", (void**)&DAQmxGetDigEdgeStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigDigFltrTimebaseSrc", (void**)&DAQmxGetDigEdgeStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigDigSyncEnable", (void**)&DAQmxGetDigEdgeStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigEdge", (void**)&DAQmxGetDigEdgeStartTrigEdge, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeStartTrigSrc", (void**)&DAQmxGetDigEdgeStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeWatchdogExpirTrigEdge", (void**)&DAQmxGetDigEdgeWatchdogExpirTrigEdge, Dylib::Import::cMangled },
  { "DAQmxGetDigEdgeWatchdogExpirTrigSrc", (void**)&DAQmxGetDigEdgeWatchdogExpirTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigDigFltrEnable", (void**)&DAQmxGetDigLvlPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxGetDigLvlPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigDigFltrTimebaseRate", (void**)&DAQmxGetDigLvlPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxGetDigLvlPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigDigSyncEnable", (void**)&DAQmxGetDigLvlPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigSrc", (void**)&DAQmxGetDigLvlPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigLvlPauseTrigWhen", (void**)&DAQmxGetDigLvlPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternPauseTrigPattern", (void**)&DAQmxGetDigPatternPauseTrigPattern, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternPauseTrigSrc", (void**)&DAQmxGetDigPatternPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternPauseTrigWhen", (void**)&DAQmxGetDigPatternPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternRefTrigPattern", (void**)&DAQmxGetDigPatternRefTrigPattern, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternRefTrigSrc", (void**)&DAQmxGetDigPatternRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternRefTrigWhen", (void**)&DAQmxGetDigPatternRefTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternStartTrigPattern", (void**)&DAQmxGetDigPatternStartTrigPattern, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternStartTrigSrc", (void**)&DAQmxGetDigPatternStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetDigPatternStartTrigWhen", (void**)&DAQmxGetDigPatternStartTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetDigitalLogicFamilyPowerUpState", (void**)&DAQmxGetDigitalLogicFamilyPowerUpState, Dylib::Import::cMangled },
  { "DAQmxGetDigitalPowerUpStates", (void**)&DAQmxGetDigitalPowerUpStates, Dylib::Import::cMangled },
  { "DAQmxGetDigitalPullUpPullDownStates", (void**)&DAQmxGetDigitalPullUpPullDownStates, Dylib::Import::cMangled },
  { "DAQmxGetDisconnectedCDAQSyncPorts", (void**)&DAQmxGetDisconnectedCDAQSyncPorts, Dylib::Import::cMangled },
  { "DAQmxGetErrorString", (void**)&DAQmxGetErrorString, Dylib::Import::cMangled },
  { "DAQmxGetExported10MHzRefClkOutputTerm", (void**)&DAQmxGetExported10MHzRefClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExported20MHzTimebaseOutputTerm", (void**)&DAQmxGetExported20MHzTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedAIConvClkOutputTerm", (void**)&DAQmxGetExportedAIConvClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedAIConvClkPulsePolarity", (void**)&DAQmxGetExportedAIConvClkPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedAIHoldCmpltEventOutputTerm", (void**)&DAQmxGetExportedAIHoldCmpltEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedAIHoldCmpltEventPulsePolarity", (void**)&DAQmxGetExportedAIHoldCmpltEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvCmpltEventDelay", (void**)&DAQmxGetExportedAdvCmpltEventDelay, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvCmpltEventOutputTerm", (void**)&DAQmxGetExportedAdvCmpltEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvCmpltEventPulsePolarity", (void**)&DAQmxGetExportedAdvCmpltEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvCmpltEventPulseWidth", (void**)&DAQmxGetExportedAdvCmpltEventPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvTrigOutputTerm", (void**)&DAQmxGetExportedAdvTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvTrigPulsePolarity", (void**)&DAQmxGetExportedAdvTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvTrigPulseWidth", (void**)&DAQmxGetExportedAdvTrigPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetExportedAdvTrigPulseWidthUnits", (void**)&DAQmxGetExportedAdvTrigPulseWidthUnits, Dylib::Import::cMangled },
  { "DAQmxGetExportedChangeDetectEventOutputTerm", (void**)&DAQmxGetExportedChangeDetectEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedChangeDetectEventPulsePolarity", (void**)&DAQmxGetExportedChangeDetectEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedCtrOutEventOutputBehavior", (void**)&DAQmxGetExportedCtrOutEventOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxGetExportedCtrOutEventOutputTerm", (void**)&DAQmxGetExportedCtrOutEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedCtrOutEventPulsePolarity", (void**)&DAQmxGetExportedCtrOutEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedCtrOutEventToggleIdleState", (void**)&DAQmxGetExportedCtrOutEventToggleIdleState, Dylib::Import::cMangled },
  { "DAQmxGetExportedDataActiveEventLvlActiveLvl", (void**)&DAQmxGetExportedDataActiveEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxGetExportedDataActiveEventOutputTerm", (void**)&DAQmxGetExportedDataActiveEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedDividedSampClkTimebaseOutputTerm", (void**)&DAQmxGetExportedDividedSampClkTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventDelay", (void**)&DAQmxGetExportedHshkEventDelay, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventInterlockedAssertOnStart", (void**)&DAQmxGetExportedHshkEventInterlockedAssertOnStart, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventInterlockedAssertedLvl", (void**)&DAQmxGetExportedHshkEventInterlockedAssertedLvl, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventInterlockedDeassertDelay", (void**)&DAQmxGetExportedHshkEventInterlockedDeassertDelay, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventOutputBehavior", (void**)&DAQmxGetExportedHshkEventOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventOutputTerm", (void**)&DAQmxGetExportedHshkEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventPulsePolarity", (void**)&DAQmxGetExportedHshkEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedHshkEventPulseWidth", (void**)&DAQmxGetExportedHshkEventPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetExportedPauseTrigLvlActiveLvl", (void**)&DAQmxGetExportedPauseTrigLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxGetExportedPauseTrigOutputTerm", (void**)&DAQmxGetExportedPauseTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedRdyForStartEventLvlActiveLvl", (void**)&DAQmxGetExportedRdyForStartEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxGetExportedRdyForStartEventOutputTerm", (void**)&DAQmxGetExportedRdyForStartEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedRdyForXferEventDeassertCond", (void**)&DAQmxGetExportedRdyForXferEventDeassertCond, Dylib::Import::cMangled },
  { "DAQmxGetExportedRdyForXferEventDeassertCondCustomThreshold", (void**)&DAQmxGetExportedRdyForXferEventDeassertCondCustomThreshold, Dylib::Import::cMangled },
  { "DAQmxGetExportedRdyForXferEventLvlActiveLvl", (void**)&DAQmxGetExportedRdyForXferEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxGetExportedRdyForXferEventOutputTerm", (void**)&DAQmxGetExportedRdyForXferEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedRefTrigOutputTerm", (void**)&DAQmxGetExportedRefTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedRefTrigPulsePolarity", (void**)&DAQmxGetExportedRefTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedSampClkDelayOffset", (void**)&DAQmxGetExportedSampClkDelayOffset, Dylib::Import::cMangled },
  { "DAQmxGetExportedSampClkOutputBehavior", (void**)&DAQmxGetExportedSampClkOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxGetExportedSampClkOutputTerm", (void**)&DAQmxGetExportedSampClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedSampClkPulsePolarity", (void**)&DAQmxGetExportedSampClkPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedSampClkTimebaseOutputTerm", (void**)&DAQmxGetExportedSampClkTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedSignalAttribute", (void**)&DAQmxGetExportedSignalAttribute, Dylib::Import::cMangled },
  { "DAQmxGetExportedStartTrigOutputTerm", (void**)&DAQmxGetExportedStartTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedStartTrigPulsePolarity", (void**)&DAQmxGetExportedStartTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxGetExportedSyncPulseEventOutputTerm", (void**)&DAQmxGetExportedSyncPulseEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExportedWatchdogExpiredEventOutputTerm", (void**)&DAQmxGetExportedWatchdogExpiredEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxGetExtCalLastDateAndTime", (void**)&DAQmxGetExtCalLastDateAndTime, Dylib::Import::cMangled },
  { "DAQmxGetExtCalLastTemp", (void**)&DAQmxGetExtCalLastTemp, Dylib::Import::cMangled },
  { "DAQmxGetExtCalRecommendedInterval", (void**)&DAQmxGetExtCalRecommendedInterval, Dylib::Import::cMangled },
  { "DAQmxGetExtendedErrorInfo", (void**)&DAQmxGetExtendedErrorInfo, Dylib::Import::cMangled },
  { "DAQmxGetFieldDAQBankDevNames", (void**)&DAQmxGetFieldDAQBankDevNames, Dylib::Import::cMangled },
  { "DAQmxGetFieldDAQDevName", (void**)&DAQmxGetFieldDAQDevName, Dylib::Import::cMangled },
  { "DAQmxGetFirstSampClkOffset", (void**)&DAQmxGetFirstSampClkOffset, Dylib::Import::cMangled },
  { "DAQmxGetFirstSampClkTimescale", (void**)&DAQmxGetFirstSampClkTimescale, Dylib::Import::cMangled },
  { "DAQmxGetFirstSampClkWhen", (void**)&DAQmxGetFirstSampClkWhen, Dylib::Import::cMangled },
  { "DAQmxGetFirstSampTimestampEnable", (void**)&DAQmxGetFirstSampTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxGetFirstSampTimestampTimescale", (void**)&DAQmxGetFirstSampTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxGetFirstSampTimestampVal", (void**)&DAQmxGetFirstSampTimestampVal, Dylib::Import::cMangled },
  { "DAQmxGetHshkDelayAfterXfer", (void**)&DAQmxGetHshkDelayAfterXfer, Dylib::Import::cMangled },
  { "DAQmxGetHshkSampleInputDataWhen", (void**)&DAQmxGetHshkSampleInputDataWhen, Dylib::Import::cMangled },
  { "DAQmxGetHshkStartCond", (void**)&DAQmxGetHshkStartCond, Dylib::Import::cMangled },
  { "DAQmxGetHshkTrigType", (void**)&DAQmxGetHshkTrigType, Dylib::Import::cMangled },
  { "DAQmxGetImplicitUnderflowBehavior", (void**)&DAQmxGetImplicitUnderflowBehavior, Dylib::Import::cMangled },
  { "DAQmxGetInterlockedHshkTrigAssertedLvl", (void**)&DAQmxGetInterlockedHshkTrigAssertedLvl, Dylib::Import::cMangled },
  { "DAQmxGetInterlockedHshkTrigSrc", (void**)&DAQmxGetInterlockedHshkTrigSrc, Dylib::Import::cMangled },
  { "DAQmxGetLoggingFilePath", (void**)&DAQmxGetLoggingFilePath, Dylib::Import::cMangled },
  { "DAQmxGetLoggingFilePreallocationSize", (void**)&DAQmxGetLoggingFilePreallocationSize, Dylib::Import::cMangled },
  { "DAQmxGetLoggingFileWriteSize", (void**)&DAQmxGetLoggingFileWriteSize, Dylib::Import::cMangled },
  { "DAQmxGetLoggingMode", (void**)&DAQmxGetLoggingMode, Dylib::Import::cMangled },
  { "DAQmxGetLoggingPause", (void**)&DAQmxGetLoggingPause, Dylib::Import::cMangled },
  { "DAQmxGetLoggingSampsPerFile", (void**)&DAQmxGetLoggingSampsPerFile, Dylib::Import::cMangled },
  { "DAQmxGetLoggingTDMSGroupName", (void**)&DAQmxGetLoggingTDMSGroupName, Dylib::Import::cMangled },
  { "DAQmxGetLoggingTDMSOperation", (void**)&DAQmxGetLoggingTDMSOperation, Dylib::Import::cMangled },
  { "DAQmxGetMasterTimebaseRate", (void**)&DAQmxGetMasterTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetMasterTimebaseSrc", (void**)&DAQmxGetMasterTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetNthTaskChannel", (void**)&DAQmxGetNthTaskChannel, Dylib::Import::cMangled },
  { "DAQmxGetNthTaskDevice", (void**)&DAQmxGetNthTaskDevice, Dylib::Import::cMangled },
  { "DAQmxGetNthTaskReadChannel", (void**)&DAQmxGetNthTaskReadChannel, Dylib::Import::cMangled },
  { "DAQmxGetOnDemandSimultaneousAOEnable", (void**)&DAQmxGetOnDemandSimultaneousAOEnable, Dylib::Import::cMangled },
  { "DAQmxGetPauseTrigTerm", (void**)&DAQmxGetPauseTrigTerm, Dylib::Import::cMangled },
  { "DAQmxGetPauseTrigType", (void**)&DAQmxGetPauseTrigType, Dylib::Import::cMangled },
  { "DAQmxGetPersistedChanAllowInteractiveDeletion", (void**)&DAQmxGetPersistedChanAllowInteractiveDeletion, Dylib::Import::cMangled },
  { "DAQmxGetPersistedChanAllowInteractiveEditing", (void**)&DAQmxGetPersistedChanAllowInteractiveEditing, Dylib::Import::cMangled },
  { "DAQmxGetPersistedChanAttribute", (void**)&DAQmxGetPersistedChanAttribute, Dylib::Import::cMangled },
  { "DAQmxGetPersistedChanAuthor", (void**)&DAQmxGetPersistedChanAuthor, Dylib::Import::cMangled },
  { "DAQmxGetPersistedScaleAllowInteractiveDeletion", (void**)&DAQmxGetPersistedScaleAllowInteractiveDeletion, Dylib::Import::cMangled },
  { "DAQmxGetPersistedScaleAllowInteractiveEditing", (void**)&DAQmxGetPersistedScaleAllowInteractiveEditing, Dylib::Import::cMangled },
  { "DAQmxGetPersistedScaleAttribute", (void**)&DAQmxGetPersistedScaleAttribute, Dylib::Import::cMangled },
  { "DAQmxGetPersistedScaleAuthor", (void**)&DAQmxGetPersistedScaleAuthor, Dylib::Import::cMangled },
  { "DAQmxGetPersistedTaskAllowInteractiveDeletion", (void**)&DAQmxGetPersistedTaskAllowInteractiveDeletion, Dylib::Import::cMangled },
  { "DAQmxGetPersistedTaskAllowInteractiveEditing", (void**)&DAQmxGetPersistedTaskAllowInteractiveEditing, Dylib::Import::cMangled },
  { "DAQmxGetPersistedTaskAttribute", (void**)&DAQmxGetPersistedTaskAttribute, Dylib::Import::cMangled },
  { "DAQmxGetPersistedTaskAuthor", (void**)&DAQmxGetPersistedTaskAuthor, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAIInputSrcs", (void**)&DAQmxGetPhysicalChanAIInputSrcs, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAIPowerControlEnable", (void**)&DAQmxGetPhysicalChanAIPowerControlEnable, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAIPowerControlType", (void**)&DAQmxGetPhysicalChanAIPowerControlType, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAIPowerControlVoltage", (void**)&DAQmxGetPhysicalChanAIPowerControlVoltage, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAISensorPowerOpenChan", (void**)&DAQmxGetPhysicalChanAISensorPowerOpenChan, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAISensorPowerOvercurrent", (void**)&DAQmxGetPhysicalChanAISensorPowerOvercurrent, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAISensorPowerTypes", (void**)&DAQmxGetPhysicalChanAISensorPowerTypes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAISensorPowerVoltageRangeVals", (void**)&DAQmxGetPhysicalChanAISensorPowerVoltageRangeVals, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAISupportedMeasTypes", (void**)&DAQmxGetPhysicalChanAISupportedMeasTypes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAITermCfgs", (void**)&DAQmxGetPhysicalChanAITermCfgs, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOManualControlAmplitude", (void**)&DAQmxGetPhysicalChanAOManualControlAmplitude, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOManualControlEnable", (void**)&DAQmxGetPhysicalChanAOManualControlEnable, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOManualControlFreq", (void**)&DAQmxGetPhysicalChanAOManualControlFreq, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOManualControlShortDetected", (void**)&DAQmxGetPhysicalChanAOManualControlShortDetected, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOSupportedOutputTypes", (void**)&DAQmxGetPhysicalChanAOSupportedOutputTypes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOSupportedPowerUpOutputTypes", (void**)&DAQmxGetPhysicalChanAOSupportedPowerUpOutputTypes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAOTermCfgs", (void**)&DAQmxGetPhysicalChanAOTermCfgs, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanAttribute", (void**)&DAQmxGetPhysicalChanAttribute, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanCISupportedMeasTypes", (void**)&DAQmxGetPhysicalChanCISupportedMeasTypes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanCOSupportedOutputTypes", (void**)&DAQmxGetPhysicalChanCOSupportedOutputTypes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDIChangeDetectSupported", (void**)&DAQmxGetPhysicalChanDIChangeDetectSupported, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDIPortWidth", (void**)&DAQmxGetPhysicalChanDIPortWidth, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDISampClkSupported", (void**)&DAQmxGetPhysicalChanDISampClkSupported, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDISampModes", (void**)&DAQmxGetPhysicalChanDISampModes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDOPortWidth", (void**)&DAQmxGetPhysicalChanDOPortWidth, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDOSampClkSupported", (void**)&DAQmxGetPhysicalChanDOSampClkSupported, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanDOSampModes", (void**)&DAQmxGetPhysicalChanDOSampModes, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanName", (void**)&DAQmxGetPhysicalChanName, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSBitStream", (void**)&DAQmxGetPhysicalChanTEDSBitStream, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSMfgID", (void**)&DAQmxGetPhysicalChanTEDSMfgID, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSModelNum", (void**)&DAQmxGetPhysicalChanTEDSModelNum, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSSerialNum", (void**)&DAQmxGetPhysicalChanTEDSSerialNum, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSTemplateIDs", (void**)&DAQmxGetPhysicalChanTEDSTemplateIDs, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSVersionLetter", (void**)&DAQmxGetPhysicalChanTEDSVersionLetter, Dylib::Import::cMangled },
  { "DAQmxGetPhysicalChanTEDSVersionNum", (void**)&DAQmxGetPhysicalChanTEDSVersionNum, Dylib::Import::cMangled },
  { "DAQmxGetPossibleSCExpressCalAccConnections", (void**)&DAQmxGetPossibleSCExpressCalAccConnections, Dylib::Import::cMangled },
  { "DAQmxGetReadAccessoryInsertionOrRemovalDetected", (void**)&DAQmxGetReadAccessoryInsertionOrRemovalDetected, Dylib::Import::cMangled },
  { "DAQmxGetReadAttribute", (void**)&DAQmxGetReadAttribute, Dylib::Import::cMangled },
  { "DAQmxGetReadAutoStart", (void**)&DAQmxGetReadAutoStart, Dylib::Import::cMangled },
  { "DAQmxGetReadAvailSampPerChan", (void**)&DAQmxGetReadAvailSampPerChan, Dylib::Import::cMangled },
  { "DAQmxGetReadChangeDetectHasOverflowed", (void**)&DAQmxGetReadChangeDetectHasOverflowed, Dylib::Import::cMangled },
  { "DAQmxGetReadChannelsToRead", (void**)&DAQmxGetReadChannelsToRead, Dylib::Import::cMangled },
  { "DAQmxGetReadCommonModeRangeErrorChans", (void**)&DAQmxGetReadCommonModeRangeErrorChans, Dylib::Import::cMangled },
  { "DAQmxGetReadCommonModeRangeErrorChansExist", (void**)&DAQmxGetReadCommonModeRangeErrorChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadCurrReadPos", (void**)&DAQmxGetReadCurrReadPos, Dylib::Import::cMangled },
  { "DAQmxGetReadDevsWithInsertedOrRemovedAccessories", (void**)&DAQmxGetReadDevsWithInsertedOrRemovedAccessories, Dylib::Import::cMangled },
  { "DAQmxGetReadDigitalLinesBytesPerChan", (void**)&DAQmxGetReadDigitalLinesBytesPerChan, Dylib::Import::cMangled },
  { "DAQmxGetReadExcitFaultChans", (void**)&DAQmxGetReadExcitFaultChans, Dylib::Import::cMangled },
  { "DAQmxGetReadExcitFaultChansExist", (void**)&DAQmxGetReadExcitFaultChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadInputLimitsFaultChans", (void**)&DAQmxGetReadInputLimitsFaultChans, Dylib::Import::cMangled },
  { "DAQmxGetReadInputLimitsFaultChansExist", (void**)&DAQmxGetReadInputLimitsFaultChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadNumChans", (void**)&DAQmxGetReadNumChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOffset", (void**)&DAQmxGetReadOffset, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenChans", (void**)&DAQmxGetReadOpenChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenChansDetails", (void**)&DAQmxGetReadOpenChansDetails, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenChansExist", (void**)&DAQmxGetReadOpenChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenCurrentLoopChans", (void**)&DAQmxGetReadOpenCurrentLoopChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenCurrentLoopChansExist", (void**)&DAQmxGetReadOpenCurrentLoopChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenThrmcplChans", (void**)&DAQmxGetReadOpenThrmcplChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOpenThrmcplChansExist", (void**)&DAQmxGetReadOpenThrmcplChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadOverWrite", (void**)&DAQmxGetReadOverWrite, Dylib::Import::cMangled },
  { "DAQmxGetReadOvercurrentChans", (void**)&DAQmxGetReadOvercurrentChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOvercurrentChansExist", (void**)&DAQmxGetReadOvercurrentChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadOverloadedChans", (void**)&DAQmxGetReadOverloadedChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOverloadedChansExist", (void**)&DAQmxGetReadOverloadedChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadOvertemperatureChans", (void**)&DAQmxGetReadOvertemperatureChans, Dylib::Import::cMangled },
  { "DAQmxGetReadOvertemperatureChansExist", (void**)&DAQmxGetReadOvertemperatureChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadPLLUnlockedChans", (void**)&DAQmxGetReadPLLUnlockedChans, Dylib::Import::cMangled },
  { "DAQmxGetReadPLLUnlockedChansExist", (void**)&DAQmxGetReadPLLUnlockedChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadPowerSupplyFaultChans", (void**)&DAQmxGetReadPowerSupplyFaultChans, Dylib::Import::cMangled },
  { "DAQmxGetReadPowerSupplyFaultChansExist", (void**)&DAQmxGetReadPowerSupplyFaultChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadRawDataWidth", (void**)&DAQmxGetReadRawDataWidth, Dylib::Import::cMangled },
  { "DAQmxGetReadReadAllAvailSamp", (void**)&DAQmxGetReadReadAllAvailSamp, Dylib::Import::cMangled },
  { "DAQmxGetReadRelativeTo", (void**)&DAQmxGetReadRelativeTo, Dylib::Import::cMangled },
  { "DAQmxGetReadSleepTime", (void**)&DAQmxGetReadSleepTime, Dylib::Import::cMangled },
  { "DAQmxGetReadSyncUnlockedChans", (void**)&DAQmxGetReadSyncUnlockedChans, Dylib::Import::cMangled },
  { "DAQmxGetReadSyncUnlockedChansExist", (void**)&DAQmxGetReadSyncUnlockedChansExist, Dylib::Import::cMangled },
  { "DAQmxGetReadTotalSampPerChanAcquired", (void**)&DAQmxGetReadTotalSampPerChanAcquired, Dylib::Import::cMangled },
  { "DAQmxGetReadWaitMode", (void**)&DAQmxGetReadWaitMode, Dylib::Import::cMangled },
  { "DAQmxGetRealTimeAttribute", (void**)&DAQmxGetRealTimeAttribute, Dylib::Import::cMangled },
  { "DAQmxGetRealTimeConvLateErrorsToWarnings", (void**)&DAQmxGetRealTimeConvLateErrorsToWarnings, Dylib::Import::cMangled },
  { "DAQmxGetRealTimeNumOfWarmupIters", (void**)&DAQmxGetRealTimeNumOfWarmupIters, Dylib::Import::cMangled },
  { "DAQmxGetRealTimeReportMissedSamp", (void**)&DAQmxGetRealTimeReportMissedSamp, Dylib::Import::cMangled },
  { "DAQmxGetRealTimeWaitForNextSampClkWaitMode", (void**)&DAQmxGetRealTimeWaitForNextSampClkWaitMode, Dylib::Import::cMangled },
  { "DAQmxGetRealTimeWriteRecoveryMode", (void**)&DAQmxGetRealTimeWriteRecoveryMode, Dylib::Import::cMangled },
  { "DAQmxGetRefClkRate", (void**)&DAQmxGetRefClkRate, Dylib::Import::cMangled },
  { "DAQmxGetRefClkSrc", (void**)&DAQmxGetRefClkSrc, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigAutoTrigEnable", (void**)&DAQmxGetRefTrigAutoTrigEnable, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigAutoTriggered", (void**)&DAQmxGetRefTrigAutoTriggered, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigDelay", (void**)&DAQmxGetRefTrigDelay, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigMaxNumTrigsToDetect", (void**)&DAQmxGetRefTrigMaxNumTrigsToDetect, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigPretrigSamples", (void**)&DAQmxGetRefTrigPretrigSamples, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigRetriggerWin", (void**)&DAQmxGetRefTrigRetriggerWin, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigRetriggerable", (void**)&DAQmxGetRefTrigRetriggerable, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigTerm", (void**)&DAQmxGetRefTrigTerm, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigTimestampEnable", (void**)&DAQmxGetRefTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigTimestampTimescale", (void**)&DAQmxGetRefTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigTimestampVal", (void**)&DAQmxGetRefTrigTimestampVal, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigTrigWin", (void**)&DAQmxGetRefTrigTrigWin, Dylib::Import::cMangled },
  { "DAQmxGetRefTrigType", (void**)&DAQmxGetRefTrigType, Dylib::Import::cMangled },
  { "DAQmxGetSampClkActiveEdge", (void**)&DAQmxGetSampClkActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetSampClkDigFltrEnable", (void**)&DAQmxGetSampClkDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxGetSampClkDigFltrMinPulseWidth", (void**)&DAQmxGetSampClkDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxGetSampClkDigFltrTimebaseRate", (void**)&DAQmxGetSampClkDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetSampClkDigFltrTimebaseSrc", (void**)&DAQmxGetSampClkDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetSampClkDigSyncEnable", (void**)&DAQmxGetSampClkDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxGetSampClkMaxRate", (void**)&DAQmxGetSampClkMaxRate, Dylib::Import::cMangled },
  { "DAQmxGetSampClkOverrunBehavior", (void**)&DAQmxGetSampClkOverrunBehavior, Dylib::Import::cMangled },
  { "DAQmxGetSampClkRate", (void**)&DAQmxGetSampClkRate, Dylib::Import::cMangled },
  { "DAQmxGetSampClkSrc", (void**)&DAQmxGetSampClkSrc, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTerm", (void**)&DAQmxGetSampClkTerm, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimebaseActiveEdge", (void**)&DAQmxGetSampClkTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimebaseDiv", (void**)&DAQmxGetSampClkTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimebaseMasterTimebaseDiv", (void**)&DAQmxGetSampClkTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimebaseRate", (void**)&DAQmxGetSampClkTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimebaseSrc", (void**)&DAQmxGetSampClkTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimebaseTerm", (void**)&DAQmxGetSampClkTimebaseTerm, Dylib::Import::cMangled },
  { "DAQmxGetSampClkTimingResponseMode", (void**)&DAQmxGetSampClkTimingResponseMode, Dylib::Import::cMangled },
  { "DAQmxGetSampClkUnderflowBehavior", (void**)&DAQmxGetSampClkUnderflowBehavior, Dylib::Import::cMangled },
  { "DAQmxGetSampClkWriteWfmUseInitialWfmDT", (void**)&DAQmxGetSampClkWriteWfmUseInitialWfmDT, Dylib::Import::cMangled },
  { "DAQmxGetSampQuantSampMode", (void**)&DAQmxGetSampQuantSampMode, Dylib::Import::cMangled },
  { "DAQmxGetSampQuantSampPerChan", (void**)&DAQmxGetSampQuantSampPerChan, Dylib::Import::cMangled },
  { "DAQmxGetSampTimingEngine", (void**)&DAQmxGetSampTimingEngine, Dylib::Import::cMangled },
  { "DAQmxGetSampTimingType", (void**)&DAQmxGetSampTimingType, Dylib::Import::cMangled },
  { "DAQmxGetScaleAttribute", (void**)&DAQmxGetScaleAttribute, Dylib::Import::cMangled },
  { "DAQmxGetScaleDescr", (void**)&DAQmxGetScaleDescr, Dylib::Import::cMangled },
  { "DAQmxGetScaleLinSlope", (void**)&DAQmxGetScaleLinSlope, Dylib::Import::cMangled },
  { "DAQmxGetScaleLinYIntercept", (void**)&DAQmxGetScaleLinYIntercept, Dylib::Import::cMangled },
  { "DAQmxGetScaleMapPreScaledMax", (void**)&DAQmxGetScaleMapPreScaledMax, Dylib::Import::cMangled },
  { "DAQmxGetScaleMapPreScaledMin", (void**)&DAQmxGetScaleMapPreScaledMin, Dylib::Import::cMangled },
  { "DAQmxGetScaleMapScaledMax", (void**)&DAQmxGetScaleMapScaledMax, Dylib::Import::cMangled },
  { "DAQmxGetScaleMapScaledMin", (void**)&DAQmxGetScaleMapScaledMin, Dylib::Import::cMangled },
  { "DAQmxGetScalePolyForwardCoeff", (void**)&DAQmxGetScalePolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxGetScalePolyReverseCoeff", (void**)&DAQmxGetScalePolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxGetScalePreScaledUnits", (void**)&DAQmxGetScalePreScaledUnits, Dylib::Import::cMangled },
  { "DAQmxGetScaleScaledUnits", (void**)&DAQmxGetScaleScaledUnits, Dylib::Import::cMangled },
  { "DAQmxGetScaleTablePreScaledVals", (void**)&DAQmxGetScaleTablePreScaledVals, Dylib::Import::cMangled },
  { "DAQmxGetScaleTableScaledVals", (void**)&DAQmxGetScaleTableScaledVals, Dylib::Import::cMangled },
  { "DAQmxGetScaleType", (void**)&DAQmxGetScaleType, Dylib::Import::cMangled },
  { "DAQmxGetSelfCalLastDateAndTime", (void**)&DAQmxGetSelfCalLastDateAndTime, Dylib::Import::cMangled },
  { "DAQmxGetSelfCalLastTemp", (void**)&DAQmxGetSelfCalLastTemp, Dylib::Import::cMangled },
  { "DAQmxGetSelfCalSupported", (void**)&DAQmxGetSelfCalSupported, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigDelay", (void**)&DAQmxGetStartTrigDelay, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigDelayUnits", (void**)&DAQmxGetStartTrigDelayUnits, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigMaxNumTrigsToDetect", (void**)&DAQmxGetStartTrigMaxNumTrigsToDetect, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigRetriggerWin", (void**)&DAQmxGetStartTrigRetriggerWin, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigRetriggerable", (void**)&DAQmxGetStartTrigRetriggerable, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTerm", (void**)&DAQmxGetStartTrigTerm, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTimescale", (void**)&DAQmxGetStartTrigTimescale, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTimestampEnable", (void**)&DAQmxGetStartTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTimestampTimescale", (void**)&DAQmxGetStartTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTimestampVal", (void**)&DAQmxGetStartTrigTimestampVal, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTrigWhen", (void**)&DAQmxGetStartTrigTrigWhen, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigTrigWin", (void**)&DAQmxGetStartTrigTrigWin, Dylib::Import::cMangled },
  { "DAQmxGetStartTrigType", (void**)&DAQmxGetStartTrigType, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanAnlgBusSharingEnable", (void**)&DAQmxGetSwitchChanAnlgBusSharingEnable, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanAttribute", (void**)&DAQmxGetSwitchChanAttribute, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanBandwidth", (void**)&DAQmxGetSwitchChanBandwidth, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanImpedance", (void**)&DAQmxGetSwitchChanImpedance, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxACCarryCurrent", (void**)&DAQmxGetSwitchChanMaxACCarryCurrent, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxACCarryPwr", (void**)&DAQmxGetSwitchChanMaxACCarryPwr, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxACSwitchCurrent", (void**)&DAQmxGetSwitchChanMaxACSwitchCurrent, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxACSwitchPwr", (void**)&DAQmxGetSwitchChanMaxACSwitchPwr, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxACVoltage", (void**)&DAQmxGetSwitchChanMaxACVoltage, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxDCCarryCurrent", (void**)&DAQmxGetSwitchChanMaxDCCarryCurrent, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxDCCarryPwr", (void**)&DAQmxGetSwitchChanMaxDCCarryPwr, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxDCSwitchCurrent", (void**)&DAQmxGetSwitchChanMaxDCSwitchCurrent, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxDCSwitchPwr", (void**)&DAQmxGetSwitchChanMaxDCSwitchPwr, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanMaxDCVoltage", (void**)&DAQmxGetSwitchChanMaxDCVoltage, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanUsage", (void**)&DAQmxGetSwitchChanUsage, Dylib::Import::cMangled },
  { "DAQmxGetSwitchChanWireMode", (void**)&DAQmxGetSwitchChanWireMode, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevAutoConnAnlgBus", (void**)&DAQmxGetSwitchDevAutoConnAnlgBus, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevNumColumns", (void**)&DAQmxGetSwitchDevNumColumns, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevNumRelays", (void**)&DAQmxGetSwitchDevNumRelays, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevNumRows", (void**)&DAQmxGetSwitchDevNumRows, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevNumSwitchChans", (void**)&DAQmxGetSwitchDevNumSwitchChans, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevPwrDownLatchRelaysAfterSettling", (void**)&DAQmxGetSwitchDevPwrDownLatchRelaysAfterSettling, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevRelayList", (void**)&DAQmxGetSwitchDevRelayList, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevSettled", (void**)&DAQmxGetSwitchDevSettled, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevSettlingTime", (void**)&DAQmxGetSwitchDevSettlingTime, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevSwitchChanList", (void**)&DAQmxGetSwitchDevSwitchChanList, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevTemperature", (void**)&DAQmxGetSwitchDevTemperature, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDevTopology", (void**)&DAQmxGetSwitchDevTopology, Dylib::Import::cMangled },
  { "DAQmxGetSwitchDeviceAttribute", (void**)&DAQmxGetSwitchDeviceAttribute, Dylib::Import::cMangled },
  { "DAQmxGetSwitchScanAttribute", (void**)&DAQmxGetSwitchScanAttribute, Dylib::Import::cMangled },
  { "DAQmxGetSwitchScanBreakMode", (void**)&DAQmxGetSwitchScanBreakMode, Dylib::Import::cMangled },
  { "DAQmxGetSwitchScanRepeatMode", (void**)&DAQmxGetSwitchScanRepeatMode, Dylib::Import::cMangled },
  { "DAQmxGetSwitchScanWaitingForAdv", (void**)&DAQmxGetSwitchScanWaitingForAdv, Dylib::Import::cMangled },
  { "DAQmxGetSyncClkInterval", (void**)&DAQmxGetSyncClkInterval, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseMinDelayToStart", (void**)&DAQmxGetSyncPulseMinDelayToStart, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseResetDelay", (void**)&DAQmxGetSyncPulseResetDelay, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseResetTime", (void**)&DAQmxGetSyncPulseResetTime, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseSrc", (void**)&DAQmxGetSyncPulseSrc, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseSyncTime", (void**)&DAQmxGetSyncPulseSyncTime, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseTerm", (void**)&DAQmxGetSyncPulseTerm, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseTimeTimescale", (void**)&DAQmxGetSyncPulseTimeTimescale, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseTimeWhen", (void**)&DAQmxGetSyncPulseTimeWhen, Dylib::Import::cMangled },
  { "DAQmxGetSyncPulseType", (void**)&DAQmxGetSyncPulseType, Dylib::Import::cMangled },
  { "DAQmxGetSysDevNames", (void**)&DAQmxGetSysDevNames, Dylib::Import::cMangled },
  { "DAQmxGetSysGlobalChans", (void**)&DAQmxGetSysGlobalChans, Dylib::Import::cMangled },
  { "DAQmxGetSysNIDAQMajorVersion", (void**)&DAQmxGetSysNIDAQMajorVersion, Dylib::Import::cMangled },
  { "DAQmxGetSysNIDAQMinorVersion", (void**)&DAQmxGetSysNIDAQMinorVersion, Dylib::Import::cMangled },
  { "DAQmxGetSysNIDAQUpdateVersion", (void**)&DAQmxGetSysNIDAQUpdateVersion, Dylib::Import::cMangled },
  { "DAQmxGetSysScales", (void**)&DAQmxGetSysScales, Dylib::Import::cMangled },
  { "DAQmxGetSysTasks", (void**)&DAQmxGetSysTasks, Dylib::Import::cMangled },
  { "DAQmxGetSystemInfoAttribute", (void**)&DAQmxGetSystemInfoAttribute, Dylib::Import::cMangled },
  { "DAQmxGetTaskAttribute", (void**)&DAQmxGetTaskAttribute, Dylib::Import::cMangled },
  { "DAQmxGetTaskChannels", (void**)&DAQmxGetTaskChannels, Dylib::Import::cMangled },
  { "DAQmxGetTaskComplete", (void**)&DAQmxGetTaskComplete, Dylib::Import::cMangled },
  { "DAQmxGetTaskDevices", (void**)&DAQmxGetTaskDevices, Dylib::Import::cMangled },
  { "DAQmxGetTaskName", (void**)&DAQmxGetTaskName, Dylib::Import::cMangled },
  { "DAQmxGetTaskNumChans", (void**)&DAQmxGetTaskNumChans, Dylib::Import::cMangled },
  { "DAQmxGetTaskNumDevices", (void**)&DAQmxGetTaskNumDevices, Dylib::Import::cMangled },
  { "DAQmxGetTimingAttribute", (void**)&DAQmxGetTimingAttribute, Dylib::Import::cMangled },
  { "DAQmxGetTimingAttributeEx", (void**)&DAQmxGetTimingAttributeEx, Dylib::Import::cMangled },
  { "DAQmxGetTrigAttribute", (void**)&DAQmxGetTrigAttribute, Dylib::Import::cMangled },
  { "DAQmxGetTriggerSyncType", (void**)&DAQmxGetTriggerSyncType, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogAOExpirState", (void**)&DAQmxGetWatchdogAOExpirState, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogAOOutputType", (void**)&DAQmxGetWatchdogAOOutputType, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogAttribute", (void**)&DAQmxGetWatchdogAttribute, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogCOExpirState", (void**)&DAQmxGetWatchdogCOExpirState, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogDOExpirState", (void**)&DAQmxGetWatchdogDOExpirState, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogExpirTrigTrigOnNetworkConnLoss", (void**)&DAQmxGetWatchdogExpirTrigTrigOnNetworkConnLoss, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogExpirTrigType", (void**)&DAQmxGetWatchdogExpirTrigType, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogHasExpired", (void**)&DAQmxGetWatchdogHasExpired, Dylib::Import::cMangled },
  { "DAQmxGetWatchdogTimeout", (void**)&DAQmxGetWatchdogTimeout, Dylib::Import::cMangled },
  { "DAQmxGetWriteAccessoryInsertionOrRemovalDetected", (void**)&DAQmxGetWriteAccessoryInsertionOrRemovalDetected, Dylib::Import::cMangled },
  { "DAQmxGetWriteAttribute", (void**)&DAQmxGetWriteAttribute, Dylib::Import::cMangled },
  { "DAQmxGetWriteCurrWritePos", (void**)&DAQmxGetWriteCurrWritePos, Dylib::Import::cMangled },
  { "DAQmxGetWriteDevsWithInsertedOrRemovedAccessories", (void**)&DAQmxGetWriteDevsWithInsertedOrRemovedAccessories, Dylib::Import::cMangled },
  { "DAQmxGetWriteDigitalLinesBytesPerChan", (void**)&DAQmxGetWriteDigitalLinesBytesPerChan, Dylib::Import::cMangled },
  { "DAQmxGetWriteExternalOvervoltageChans", (void**)&DAQmxGetWriteExternalOvervoltageChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteExternalOvervoltageChansExist", (void**)&DAQmxGetWriteExternalOvervoltageChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWriteNumChans", (void**)&DAQmxGetWriteNumChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteOffset", (void**)&DAQmxGetWriteOffset, Dylib::Import::cMangled },
  { "DAQmxGetWriteOpenCurrentLoopChans", (void**)&DAQmxGetWriteOpenCurrentLoopChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteOpenCurrentLoopChansExist", (void**)&DAQmxGetWriteOpenCurrentLoopChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWriteOvercurrentChans", (void**)&DAQmxGetWriteOvercurrentChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteOvercurrentChansExist", (void**)&DAQmxGetWriteOvercurrentChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWriteOverloadedChans", (void**)&DAQmxGetWriteOverloadedChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteOverloadedChansExist", (void**)&DAQmxGetWriteOverloadedChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWriteOvertemperatureChans", (void**)&DAQmxGetWriteOvertemperatureChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteOvertemperatureChansExist", (void**)&DAQmxGetWriteOvertemperatureChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWritePowerSupplyFaultChans", (void**)&DAQmxGetWritePowerSupplyFaultChans, Dylib::Import::cMangled },
  { "DAQmxGetWritePowerSupplyFaultChansExist", (void**)&DAQmxGetWritePowerSupplyFaultChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWriteRawDataWidth", (void**)&DAQmxGetWriteRawDataWidth, Dylib::Import::cMangled },
  { "DAQmxGetWriteRegenMode", (void**)&DAQmxGetWriteRegenMode, Dylib::Import::cMangled },
  { "DAQmxGetWriteRelativeTo", (void**)&DAQmxGetWriteRelativeTo, Dylib::Import::cMangled },
  { "DAQmxGetWriteSleepTime", (void**)&DAQmxGetWriteSleepTime, Dylib::Import::cMangled },
  { "DAQmxGetWriteSpaceAvail", (void**)&DAQmxGetWriteSpaceAvail, Dylib::Import::cMangled },
  { "DAQmxGetWriteSyncUnlockedChans", (void**)&DAQmxGetWriteSyncUnlockedChans, Dylib::Import::cMangled },
  { "DAQmxGetWriteSyncUnlockedChansExist", (void**)&DAQmxGetWriteSyncUnlockedChansExist, Dylib::Import::cMangled },
  { "DAQmxGetWriteTotalSampPerChanGenerated", (void**)&DAQmxGetWriteTotalSampPerChanGenerated, Dylib::Import::cMangled },
  { "DAQmxGetWriteWaitMode", (void**)&DAQmxGetWriteWaitMode, Dylib::Import::cMangled },
  { "DAQmxInitExtCal", (void**)&DAQmxInitExtCal, Dylib::Import::cMangled },
  { "DAQmxIsReadOrWriteLate", (void**)&DAQmxIsReadOrWriteLate, Dylib::Import::cMangled },
  { "DAQmxIsTaskDone", (void**)&DAQmxIsTaskDone, Dylib::Import::cMangled },
  { "DAQmxLoadTask", (void**)&DAQmxLoadTask, Dylib::Import::cMangled },
  { "DAQmxMSeriesCalAdjust", (void**)&DAQmxMSeriesCalAdjust, Dylib::Import::cMangled },
  { "DAQmxPerformBridgeOffsetNullingCal", (void**)&DAQmxPerformBridgeOffsetNullingCal, Dylib::Import::cMangled },
  { "DAQmxPerformBridgeOffsetNullingCalEx", (void**)&DAQmxPerformBridgeOffsetNullingCalEx, Dylib::Import::cMangled },
  { "DAQmxPerformBridgeShuntCal", (void**)&DAQmxPerformBridgeShuntCal, Dylib::Import::cMangled },
  { "DAQmxPerformBridgeShuntCalEx", (void**)&DAQmxPerformBridgeShuntCalEx, Dylib::Import::cMangled },
  { "DAQmxPerformStrainShuntCal", (void**)&DAQmxPerformStrainShuntCal, Dylib::Import::cMangled },
  { "DAQmxPerformStrainShuntCalEx", (void**)&DAQmxPerformStrainShuntCalEx, Dylib::Import::cMangled },
  { "DAQmxPerformThrmcplLeadOffsetNullingCal", (void**)&DAQmxPerformThrmcplLeadOffsetNullingCal, Dylib::Import::cMangled },
  { "DAQmxReadAnalogF64", (void**)&DAQmxReadAnalogF64, Dylib::Import::cMangled },
  { "DAQmxReadAnalogScalarF64", (void**)&DAQmxReadAnalogScalarF64, Dylib::Import::cMangled },
  { "DAQmxReadBinaryI16", (void**)&DAQmxReadBinaryI16, Dylib::Import::cMangled },
  { "DAQmxReadBinaryI32", (void**)&DAQmxReadBinaryI32, Dylib::Import::cMangled },
  { "DAQmxReadBinaryU16", (void**)&DAQmxReadBinaryU16, Dylib::Import::cMangled },
  { "DAQmxReadBinaryU32", (void**)&DAQmxReadBinaryU32, Dylib::Import::cMangled },
  { "DAQmxReadCounterF64", (void**)&DAQmxReadCounterF64, Dylib::Import::cMangled },
  { "DAQmxReadCounterF64Ex", (void**)&DAQmxReadCounterF64Ex, Dylib::Import::cMangled },
  { "DAQmxReadCounterScalarF64", (void**)&DAQmxReadCounterScalarF64, Dylib::Import::cMangled },
  { "DAQmxReadCounterScalarU32", (void**)&DAQmxReadCounterScalarU32, Dylib::Import::cMangled },
  { "DAQmxReadCounterU32", (void**)&DAQmxReadCounterU32, Dylib::Import::cMangled },
  { "DAQmxReadCounterU32Ex", (void**)&DAQmxReadCounterU32Ex, Dylib::Import::cMangled },
  { "DAQmxReadCtrFreq", (void**)&DAQmxReadCtrFreq, Dylib::Import::cMangled },
  { "DAQmxReadCtrFreqScalar", (void**)&DAQmxReadCtrFreqScalar, Dylib::Import::cMangled },
  { "DAQmxReadCtrTicks", (void**)&DAQmxReadCtrTicks, Dylib::Import::cMangled },
  { "DAQmxReadCtrTicksScalar", (void**)&DAQmxReadCtrTicksScalar, Dylib::Import::cMangled },
  { "DAQmxReadCtrTime", (void**)&DAQmxReadCtrTime, Dylib::Import::cMangled },
  { "DAQmxReadCtrTimeScalar", (void**)&DAQmxReadCtrTimeScalar, Dylib::Import::cMangled },
  { "DAQmxReadDigitalLines", (void**)&DAQmxReadDigitalLines, Dylib::Import::cMangled },
  { "DAQmxReadDigitalScalarU32", (void**)&DAQmxReadDigitalScalarU32, Dylib::Import::cMangled },
  { "DAQmxReadDigitalU16", (void**)&DAQmxReadDigitalU16, Dylib::Import::cMangled },
  { "DAQmxReadDigitalU32", (void**)&DAQmxReadDigitalU32, Dylib::Import::cMangled },
  { "DAQmxReadDigitalU8", (void**)&DAQmxReadDigitalU8, Dylib::Import::cMangled },
  { "DAQmxReadRaw", (void**)&DAQmxReadRaw, Dylib::Import::cMangled },
  { "DAQmxRegisterDoneEvent", (void**)&DAQmxRegisterDoneEvent, Dylib::Import::cMangled },
  { "DAQmxRegisterEveryNSamplesEvent", (void**)&DAQmxRegisterEveryNSamplesEvent, Dylib::Import::cMangled },
  { "DAQmxRegisterSignalEvent", (void**)&DAQmxRegisterSignalEvent, Dylib::Import::cMangled },
  { "DAQmxRemoveCDAQSyncConnection", (void**)&DAQmxRemoveCDAQSyncConnection, Dylib::Import::cMangled },
  { "DAQmxReserveNetworkDevice", (void**)&DAQmxReserveNetworkDevice, Dylib::Import::cMangled },
  { "DAQmxResetAIACExcitFreq", (void**)&DAQmxResetAIACExcitFreq, Dylib::Import::cMangled },
  { "DAQmxResetAIACExcitSyncEnable", (void**)&DAQmxResetAIACExcitSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIACExcitWireMode", (void**)&DAQmxResetAIACExcitWireMode, Dylib::Import::cMangled },
  { "DAQmxResetAIADCCustomTimingMode", (void**)&DAQmxResetAIADCCustomTimingMode, Dylib::Import::cMangled },
  { "DAQmxResetAIADCTimingMode", (void**)&DAQmxResetAIADCTimingMode, Dylib::Import::cMangled },
  { "DAQmxResetAIAccel4WireDCVoltageSensitivity", (void**)&DAQmxResetAIAccel4WireDCVoltageSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIAccel4WireDCVoltageSensitivityUnits", (void**)&DAQmxResetAIAccel4WireDCVoltageSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIAccelChargeSensitivity", (void**)&DAQmxResetAIAccelChargeSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIAccelChargeSensitivityUnits", (void**)&DAQmxResetAIAccelChargeSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIAccelSensitivity", (void**)&DAQmxResetAIAccelSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIAccelSensitivityUnits", (void**)&DAQmxResetAIAccelSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIAccelUnits", (void**)&DAQmxResetAIAccelUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIAcceldBRef", (void**)&DAQmxResetAIAcceldBRef, Dylib::Import::cMangled },
  { "DAQmxResetAIAtten", (void**)&DAQmxResetAIAtten, Dylib::Import::cMangled },
  { "DAQmxResetAIAutoZeroMode", (void**)&DAQmxResetAIAutoZeroMode, Dylib::Import::cMangled },
  { "DAQmxResetAIAveragingWinSize", (void**)&DAQmxResetAIAveragingWinSize, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeBalanceCoarsePot", (void**)&DAQmxResetAIBridgeBalanceCoarsePot, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeBalanceFinePot", (void**)&DAQmxResetAIBridgeBalanceFinePot, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeCfg", (void**)&DAQmxResetAIBridgeCfg, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeElectricalUnits", (void**)&DAQmxResetAIBridgeElectricalUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeInitialRatio", (void**)&DAQmxResetAIBridgeInitialRatio, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeInitialVoltage", (void**)&DAQmxResetAIBridgeInitialVoltage, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeNomResistance", (void**)&DAQmxResetAIBridgeNomResistance, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgePhysicalUnits", (void**)&DAQmxResetAIBridgePhysicalUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgePolyForwardCoeff", (void**)&DAQmxResetAIBridgePolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgePolyReverseCoeff", (void**)&DAQmxResetAIBridgePolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeScaleType", (void**)&DAQmxResetAIBridgeScaleType, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalEnable", (void**)&DAQmxResetAIBridgeShuntCalEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalGainAdjust", (void**)&DAQmxResetAIBridgeShuntCalGainAdjust, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalSelect", (void**)&DAQmxResetAIBridgeShuntCalSelect, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalShuntCalAActualResistance", (void**)&DAQmxResetAIBridgeShuntCalShuntCalAActualResistance, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalShuntCalAResistance", (void**)&DAQmxResetAIBridgeShuntCalShuntCalAResistance, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalShuntCalASrc", (void**)&DAQmxResetAIBridgeShuntCalShuntCalASrc, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalShuntCalBActualResistance", (void**)&DAQmxResetAIBridgeShuntCalShuntCalBActualResistance, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeShuntCalShuntCalBResistance", (void**)&DAQmxResetAIBridgeShuntCalShuntCalBResistance, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeTableElectricalVals", (void**)&DAQmxResetAIBridgeTableElectricalVals, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeTablePhysicalVals", (void**)&DAQmxResetAIBridgeTablePhysicalVals, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeTwoPointLinFirstElectricalVal", (void**)&DAQmxResetAIBridgeTwoPointLinFirstElectricalVal, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeTwoPointLinFirstPhysicalVal", (void**)&DAQmxResetAIBridgeTwoPointLinFirstPhysicalVal, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeTwoPointLinSecondElectricalVal", (void**)&DAQmxResetAIBridgeTwoPointLinSecondElectricalVal, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeTwoPointLinSecondPhysicalVal", (void**)&DAQmxResetAIBridgeTwoPointLinSecondPhysicalVal, Dylib::Import::cMangled },
  { "DAQmxResetAIBridgeUnits", (void**)&DAQmxResetAIBridgeUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalApplyCalIfExp", (void**)&DAQmxResetAIChanCalApplyCalIfExp, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalDesc", (void**)&DAQmxResetAIChanCalDesc, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalEnableCal", (void**)&DAQmxResetAIChanCalEnableCal, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalOperatorName", (void**)&DAQmxResetAIChanCalOperatorName, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalPolyForwardCoeff", (void**)&DAQmxResetAIChanCalPolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalPolyReverseCoeff", (void**)&DAQmxResetAIChanCalPolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalScaleType", (void**)&DAQmxResetAIChanCalScaleType, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalTablePreScaledVals", (void**)&DAQmxResetAIChanCalTablePreScaledVals, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalTableScaledVals", (void**)&DAQmxResetAIChanCalTableScaledVals, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalVerifAcqVals", (void**)&DAQmxResetAIChanCalVerifAcqVals, Dylib::Import::cMangled },
  { "DAQmxResetAIChanCalVerifRefVals", (void**)&DAQmxResetAIChanCalVerifRefVals, Dylib::Import::cMangled },
  { "DAQmxResetAIChargeUnits", (void**)&DAQmxResetAIChargeUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIChopEnable", (void**)&DAQmxResetAIChopEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIConvActiveEdge", (void**)&DAQmxResetAIConvActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetAIConvActiveEdgeEx", (void**)&DAQmxResetAIConvActiveEdgeEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrEnable", (void**)&DAQmxResetAIConvDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrEnableEx", (void**)&DAQmxResetAIConvDigFltrEnableEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrMinPulseWidth", (void**)&DAQmxResetAIConvDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrMinPulseWidthEx", (void**)&DAQmxResetAIConvDigFltrMinPulseWidthEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrTimebaseRate", (void**)&DAQmxResetAIConvDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrTimebaseRateEx", (void**)&DAQmxResetAIConvDigFltrTimebaseRateEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrTimebaseSrc", (void**)&DAQmxResetAIConvDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigFltrTimebaseSrcEx", (void**)&DAQmxResetAIConvDigFltrTimebaseSrcEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigSyncEnable", (void**)&DAQmxResetAIConvDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIConvDigSyncEnableEx", (void**)&DAQmxResetAIConvDigSyncEnableEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvRate", (void**)&DAQmxResetAIConvRate, Dylib::Import::cMangled },
  { "DAQmxResetAIConvRateEx", (void**)&DAQmxResetAIConvRateEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvSrc", (void**)&DAQmxResetAIConvSrc, Dylib::Import::cMangled },
  { "DAQmxResetAIConvSrcEx", (void**)&DAQmxResetAIConvSrcEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvTimebaseDiv", (void**)&DAQmxResetAIConvTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxResetAIConvTimebaseDivEx", (void**)&DAQmxResetAIConvTimebaseDivEx, Dylib::Import::cMangled },
  { "DAQmxResetAIConvTimebaseSrc", (void**)&DAQmxResetAIConvTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAIConvTimebaseSrcEx", (void**)&DAQmxResetAIConvTimebaseSrcEx, Dylib::Import::cMangled },
  { "DAQmxResetAICoupling", (void**)&DAQmxResetAICoupling, Dylib::Import::cMangled },
  { "DAQmxResetAICurrentACRMSUnits", (void**)&DAQmxResetAICurrentACRMSUnits, Dylib::Import::cMangled },
  { "DAQmxResetAICurrentShuntLoc", (void**)&DAQmxResetAICurrentShuntLoc, Dylib::Import::cMangled },
  { "DAQmxResetAICurrentShuntResistance", (void**)&DAQmxResetAICurrentShuntResistance, Dylib::Import::cMangled },
  { "DAQmxResetAICurrentUnits", (void**)&DAQmxResetAICurrentUnits, Dylib::Import::cMangled },
  { "DAQmxResetAICustomScaleName", (void**)&DAQmxResetAICustomScaleName, Dylib::Import::cMangled },
  { "DAQmxResetAIDCOffset", (void**)&DAQmxResetAIDCOffset, Dylib::Import::cMangled },
  { "DAQmxResetAIDataXferCustomThreshold", (void**)&DAQmxResetAIDataXferCustomThreshold, Dylib::Import::cMangled },
  { "DAQmxResetAIDataXferMaxRate", (void**)&DAQmxResetAIDataXferMaxRate, Dylib::Import::cMangled },
  { "DAQmxResetAIDataXferMech", (void**)&DAQmxResetAIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxResetAIDataXferReqCond", (void**)&DAQmxResetAIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrBandpassCenterFreq", (void**)&DAQmxResetAIDigFltrBandpassCenterFreq, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrBandpassWidth", (void**)&DAQmxResetAIDigFltrBandpassWidth, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrCoeff", (void**)&DAQmxResetAIDigFltrCoeff, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrEnable", (void**)&DAQmxResetAIDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrHighpassCutoffFreq", (void**)&DAQmxResetAIDigFltrHighpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrLowpassCutoffFreq", (void**)&DAQmxResetAIDigFltrLowpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrNotchCenterFreq", (void**)&DAQmxResetAIDigFltrNotchCenterFreq, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrNotchWidth", (void**)&DAQmxResetAIDigFltrNotchWidth, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrOrder", (void**)&DAQmxResetAIDigFltrOrder, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrResponse", (void**)&DAQmxResetAIDigFltrResponse, Dylib::Import::cMangled },
  { "DAQmxResetAIDigFltrType", (void**)&DAQmxResetAIDigFltrType, Dylib::Import::cMangled },
  { "DAQmxResetAIDitherEnable", (void**)&DAQmxResetAIDitherEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIEddyCurrentProxProbeSensitivity", (void**)&DAQmxResetAIEddyCurrentProxProbeSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIEddyCurrentProxProbeSensitivityUnits", (void**)&DAQmxResetAIEddyCurrentProxProbeSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIEddyCurrentProxProbeUnits", (void**)&DAQmxResetAIEddyCurrentProxProbeUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIEnhancedAliasRejectionEnable", (void**)&DAQmxResetAIEnhancedAliasRejectionEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitActualVal", (void**)&DAQmxResetAIExcitActualVal, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitDCorAC", (void**)&DAQmxResetAIExcitDCorAC, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitIdleOutputBehavior", (void**)&DAQmxResetAIExcitIdleOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitSense", (void**)&DAQmxResetAIExcitSense, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitSrc", (void**)&DAQmxResetAIExcitSrc, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitUseForScaling", (void**)&DAQmxResetAIExcitUseForScaling, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitUseMultiplexed", (void**)&DAQmxResetAIExcitUseMultiplexed, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitVal", (void**)&DAQmxResetAIExcitVal, Dylib::Import::cMangled },
  { "DAQmxResetAIExcitVoltageOrCurrent", (void**)&DAQmxResetAIExcitVoltageOrCurrent, Dylib::Import::cMangled },
  { "DAQmxResetAIFilterDelayAdjustment", (void**)&DAQmxResetAIFilterDelayAdjustment, Dylib::Import::cMangled },
  { "DAQmxResetAIFilterDelayUnits", (void**)&DAQmxResetAIFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIFilterEnable", (void**)&DAQmxResetAIFilterEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIFilterFreq", (void**)&DAQmxResetAIFilterFreq, Dylib::Import::cMangled },
  { "DAQmxResetAIFilterOrder", (void**)&DAQmxResetAIFilterOrder, Dylib::Import::cMangled },
  { "DAQmxResetAIFilterResponse", (void**)&DAQmxResetAIFilterResponse, Dylib::Import::cMangled },
  { "DAQmxResetAIForceIEPESensorSensitivity", (void**)&DAQmxResetAIForceIEPESensorSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIForceIEPESensorSensitivityUnits", (void**)&DAQmxResetAIForceIEPESensorSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIForceReadFromChan", (void**)&DAQmxResetAIForceReadFromChan, Dylib::Import::cMangled },
  { "DAQmxResetAIForceUnits", (void**)&DAQmxResetAIForceUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIFreqHyst", (void**)&DAQmxResetAIFreqHyst, Dylib::Import::cMangled },
  { "DAQmxResetAIFreqThreshVoltage", (void**)&DAQmxResetAIFreqThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetAIFreqUnits", (void**)&DAQmxResetAIFreqUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIGain", (void**)&DAQmxResetAIGain, Dylib::Import::cMangled },
  { "DAQmxResetAIImpedance", (void**)&DAQmxResetAIImpedance, Dylib::Import::cMangled },
  { "DAQmxResetAIInputLimitsFaultDetectEnable", (void**)&DAQmxResetAIInputLimitsFaultDetectEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIInputLimitsFaultDetectLowerLimit", (void**)&DAQmxResetAIInputLimitsFaultDetectLowerLimit, Dylib::Import::cMangled },
  { "DAQmxResetAIInputLimitsFaultDetectUpperLimit", (void**)&DAQmxResetAIInputLimitsFaultDetectUpperLimit, Dylib::Import::cMangled },
  { "DAQmxResetAIInputSrc", (void**)&DAQmxResetAIInputSrc, Dylib::Import::cMangled },
  { "DAQmxResetAILVDTSensitivity", (void**)&DAQmxResetAILVDTSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAILVDTSensitivityUnits", (void**)&DAQmxResetAILVDTSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAILVDTUnits", (void**)&DAQmxResetAILVDTUnits, Dylib::Import::cMangled },
  { "DAQmxResetAILeadWireResistance", (void**)&DAQmxResetAILeadWireResistance, Dylib::Import::cMangled },
  { "DAQmxResetAILossyLSBRemovalCompressedSampSize", (void**)&DAQmxResetAILossyLSBRemovalCompressedSampSize, Dylib::Import::cMangled },
  { "DAQmxResetAILowpassCutoffFreq", (void**)&DAQmxResetAILowpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxResetAILowpassEnable", (void**)&DAQmxResetAILowpassEnable, Dylib::Import::cMangled },
  { "DAQmxResetAILowpassSwitchCapClkSrc", (void**)&DAQmxResetAILowpassSwitchCapClkSrc, Dylib::Import::cMangled },
  { "DAQmxResetAILowpassSwitchCapExtClkDiv", (void**)&DAQmxResetAILowpassSwitchCapExtClkDiv, Dylib::Import::cMangled },
  { "DAQmxResetAILowpassSwitchCapExtClkFreq", (void**)&DAQmxResetAILowpassSwitchCapExtClkFreq, Dylib::Import::cMangled },
  { "DAQmxResetAILowpassSwitchCapOutClkDiv", (void**)&DAQmxResetAILowpassSwitchCapOutClkDiv, Dylib::Import::cMangled },
  { "DAQmxResetAIMax", (void**)&DAQmxResetAIMax, Dylib::Import::cMangled },
  { "DAQmxResetAIMemMapEnable", (void**)&DAQmxResetAIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIMicrophoneSensitivity", (void**)&DAQmxResetAIMicrophoneSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIMin", (void**)&DAQmxResetAIMin, Dylib::Import::cMangled },
  { "DAQmxResetAIOpenChanDetectEnable", (void**)&DAQmxResetAIOpenChanDetectEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIOpenThrmcplDetectEnable", (void**)&DAQmxResetAIOpenThrmcplDetectEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIOvercurrentDetectEnable", (void**)&DAQmxResetAIOvercurrentDetectEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIPowerSupplyFaultDetectEnable", (void**)&DAQmxResetAIPowerSupplyFaultDetectEnable, Dylib::Import::cMangled },
  { "DAQmxResetAIPressureUnits", (void**)&DAQmxResetAIPressureUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIProbeAtten", (void**)&DAQmxResetAIProbeAtten, Dylib::Import::cMangled },
  { "DAQmxResetAIRTDA", (void**)&DAQmxResetAIRTDA, Dylib::Import::cMangled },
  { "DAQmxResetAIRTDB", (void**)&DAQmxResetAIRTDB, Dylib::Import::cMangled },
  { "DAQmxResetAIRTDC", (void**)&DAQmxResetAIRTDC, Dylib::Import::cMangled },
  { "DAQmxResetAIRTDR0", (void**)&DAQmxResetAIRTDR0, Dylib::Import::cMangled },
  { "DAQmxResetAIRTDType", (void**)&DAQmxResetAIRTDType, Dylib::Import::cMangled },
  { "DAQmxResetAIRVDTSensitivity", (void**)&DAQmxResetAIRVDTSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIRVDTSensitivityUnits", (void**)&DAQmxResetAIRVDTSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIRVDTUnits", (void**)&DAQmxResetAIRVDTUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIRawDataCompressionType", (void**)&DAQmxResetAIRawDataCompressionType, Dylib::Import::cMangled },
  { "DAQmxResetAIRemoveFilterDelay", (void**)&DAQmxResetAIRemoveFilterDelay, Dylib::Import::cMangled },
  { "DAQmxResetAIResistanceCfg", (void**)&DAQmxResetAIResistanceCfg, Dylib::Import::cMangled },
  { "DAQmxResetAIResistanceUnits", (void**)&DAQmxResetAIResistanceUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIRngHigh", (void**)&DAQmxResetAIRngHigh, Dylib::Import::cMangled },
  { "DAQmxResetAIRngLow", (void**)&DAQmxResetAIRngLow, Dylib::Import::cMangled },
  { "DAQmxResetAIRosetteStrainGageOrientation", (void**)&DAQmxResetAIRosetteStrainGageOrientation, Dylib::Import::cMangled },
  { "DAQmxResetAIRosetteStrainGageRosetteMeasType", (void**)&DAQmxResetAIRosetteStrainGageRosetteMeasType, Dylib::Import::cMangled },
  { "DAQmxResetAISampAndHoldEnable", (void**)&DAQmxResetAISampAndHoldEnable, Dylib::Import::cMangled },
  { "DAQmxResetAISensorPowerCfg", (void**)&DAQmxResetAISensorPowerCfg, Dylib::Import::cMangled },
  { "DAQmxResetAISensorPowerType", (void**)&DAQmxResetAISensorPowerType, Dylib::Import::cMangled },
  { "DAQmxResetAISensorPowerVoltage", (void**)&DAQmxResetAISensorPowerVoltage, Dylib::Import::cMangled },
  { "DAQmxResetAISoundPressureMaxSoundPressureLvl", (void**)&DAQmxResetAISoundPressureMaxSoundPressureLvl, Dylib::Import::cMangled },
  { "DAQmxResetAISoundPressureUnits", (void**)&DAQmxResetAISoundPressureUnits, Dylib::Import::cMangled },
  { "DAQmxResetAISoundPressuredBRef", (void**)&DAQmxResetAISoundPressuredBRef, Dylib::Import::cMangled },
  { "DAQmxResetAIStrainGageCfg", (void**)&DAQmxResetAIStrainGageCfg, Dylib::Import::cMangled },
  { "DAQmxResetAIStrainGageForceReadFromChan", (void**)&DAQmxResetAIStrainGageForceReadFromChan, Dylib::Import::cMangled },
  { "DAQmxResetAIStrainGageGageFactor", (void**)&DAQmxResetAIStrainGageGageFactor, Dylib::Import::cMangled },
  { "DAQmxResetAIStrainGagePoissonRatio", (void**)&DAQmxResetAIStrainGagePoissonRatio, Dylib::Import::cMangled },
  { "DAQmxResetAIStrainUnits", (void**)&DAQmxResetAIStrainUnits, Dylib::Import::cMangled },
  { "DAQmxResetAITempUnits", (void**)&DAQmxResetAITempUnits, Dylib::Import::cMangled },
  { "DAQmxResetAITermCfg", (void**)&DAQmxResetAITermCfg, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmcplCJCVal", (void**)&DAQmxResetAIThrmcplCJCVal, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmcplLeadOffsetVoltage", (void**)&DAQmxResetAIThrmcplLeadOffsetVoltage, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmcplScaleType", (void**)&DAQmxResetAIThrmcplScaleType, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmcplType", (void**)&DAQmxResetAIThrmcplType, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmstrA", (void**)&DAQmxResetAIThrmstrA, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmstrB", (void**)&DAQmxResetAIThrmstrB, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmstrC", (void**)&DAQmxResetAIThrmstrC, Dylib::Import::cMangled },
  { "DAQmxResetAIThrmstrR1", (void**)&DAQmxResetAIThrmstrR1, Dylib::Import::cMangled },
  { "DAQmxResetAITorqueUnits", (void**)&DAQmxResetAITorqueUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIUsbXferReqCount", (void**)&DAQmxResetAIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxResetAIUsbXferReqSize", (void**)&DAQmxResetAIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxResetAIVelocityIEPESensorSensitivity", (void**)&DAQmxResetAIVelocityIEPESensorSensitivity, Dylib::Import::cMangled },
  { "DAQmxResetAIVelocityIEPESensorSensitivityUnits", (void**)&DAQmxResetAIVelocityIEPESensorSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIVelocityIEPESensordBRef", (void**)&DAQmxResetAIVelocityIEPESensordBRef, Dylib::Import::cMangled },
  { "DAQmxResetAIVelocityUnits", (void**)&DAQmxResetAIVelocityUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIVoltageACRMSUnits", (void**)&DAQmxResetAIVoltageACRMSUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIVoltageUnits", (void**)&DAQmxResetAIVoltageUnits, Dylib::Import::cMangled },
  { "DAQmxResetAIVoltagedBRef", (void**)&DAQmxResetAIVoltagedBRef, Dylib::Import::cMangled },
  { "DAQmxResetAOCommonModeOffset", (void**)&DAQmxResetAOCommonModeOffset, Dylib::Import::cMangled },
  { "DAQmxResetAOCurrentUnits", (void**)&DAQmxResetAOCurrentUnits, Dylib::Import::cMangled },
  { "DAQmxResetAOCustomScaleName", (void**)&DAQmxResetAOCustomScaleName, Dylib::Import::cMangled },
  { "DAQmxResetAODACOffsetExtSrc", (void**)&DAQmxResetAODACOffsetExtSrc, Dylib::Import::cMangled },
  { "DAQmxResetAODACOffsetSrc", (void**)&DAQmxResetAODACOffsetSrc, Dylib::Import::cMangled },
  { "DAQmxResetAODACOffsetVal", (void**)&DAQmxResetAODACOffsetVal, Dylib::Import::cMangled },
  { "DAQmxResetAODACRefAllowConnToGnd", (void**)&DAQmxResetAODACRefAllowConnToGnd, Dylib::Import::cMangled },
  { "DAQmxResetAODACRefConnToGnd", (void**)&DAQmxResetAODACRefConnToGnd, Dylib::Import::cMangled },
  { "DAQmxResetAODACRefExtSrc", (void**)&DAQmxResetAODACRefExtSrc, Dylib::Import::cMangled },
  { "DAQmxResetAODACRefSrc", (void**)&DAQmxResetAODACRefSrc, Dylib::Import::cMangled },
  { "DAQmxResetAODACRefVal", (void**)&DAQmxResetAODACRefVal, Dylib::Import::cMangled },
  { "DAQmxResetAODACRngHigh", (void**)&DAQmxResetAODACRngHigh, Dylib::Import::cMangled },
  { "DAQmxResetAODACRngLow", (void**)&DAQmxResetAODACRngLow, Dylib::Import::cMangled },
  { "DAQmxResetAODataXferMech", (void**)&DAQmxResetAODataXferMech, Dylib::Import::cMangled },
  { "DAQmxResetAODataXferReqCond", (void**)&DAQmxResetAODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxResetAOEnhancedImageRejectionEnable", (void**)&DAQmxResetAOEnhancedImageRejectionEnable, Dylib::Import::cMangled },
  { "DAQmxResetAOFilterDelay", (void**)&DAQmxResetAOFilterDelay, Dylib::Import::cMangled },
  { "DAQmxResetAOFilterDelayAdjustment", (void**)&DAQmxResetAOFilterDelayAdjustment, Dylib::Import::cMangled },
  { "DAQmxResetAOFilterDelayUnits", (void**)&DAQmxResetAOFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenAmplitude", (void**)&DAQmxResetAOFuncGenAmplitude, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenFMDeviation", (void**)&DAQmxResetAOFuncGenFMDeviation, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenFreq", (void**)&DAQmxResetAOFuncGenFreq, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenModulationType", (void**)&DAQmxResetAOFuncGenModulationType, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenOffset", (void**)&DAQmxResetAOFuncGenOffset, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenSquareDutyCycle", (void**)&DAQmxResetAOFuncGenSquareDutyCycle, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenStartPhase", (void**)&DAQmxResetAOFuncGenStartPhase, Dylib::Import::cMangled },
  { "DAQmxResetAOFuncGenType", (void**)&DAQmxResetAOFuncGenType, Dylib::Import::cMangled },
  { "DAQmxResetAOGain", (void**)&DAQmxResetAOGain, Dylib::Import::cMangled },
  { "DAQmxResetAOIdleOutputBehavior", (void**)&DAQmxResetAOIdleOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxResetAOLoadImpedance", (void**)&DAQmxResetAOLoadImpedance, Dylib::Import::cMangled },
  { "DAQmxResetAOMax", (void**)&DAQmxResetAOMax, Dylib::Import::cMangled },
  { "DAQmxResetAOMemMapEnable", (void**)&DAQmxResetAOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxResetAOMin", (void**)&DAQmxResetAOMin, Dylib::Import::cMangled },
  { "DAQmxResetAOOutputImpedance", (void**)&DAQmxResetAOOutputImpedance, Dylib::Import::cMangled },
  { "DAQmxResetAOPowerAmpChannelEnable", (void**)&DAQmxResetAOPowerAmpChannelEnable, Dylib::Import::cMangled },
  { "DAQmxResetAOReglitchEnable", (void**)&DAQmxResetAOReglitchEnable, Dylib::Import::cMangled },
  { "DAQmxResetAOResolutionUnits", (void**)&DAQmxResetAOResolutionUnits, Dylib::Import::cMangled },
  { "DAQmxResetAOTermCfg", (void**)&DAQmxResetAOTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetAOUsbXferReqCount", (void**)&DAQmxResetAOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxResetAOUsbXferReqSize", (void**)&DAQmxResetAOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxResetAOUseOnlyOnBrdMem", (void**)&DAQmxResetAOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxResetAOVoltageCurrentLimit", (void**)&DAQmxResetAOVoltageCurrentLimit, Dylib::Import::cMangled },
  { "DAQmxResetAOVoltageUnits", (void**)&DAQmxResetAOVoltageUnits, Dylib::Import::cMangled },
  { "DAQmxResetAdvTrigType", (void**)&DAQmxResetAdvTrigType, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigCoupling", (void**)&DAQmxResetAnlgEdgeRefTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigDigFltrEnable", (void**)&DAQmxResetAnlgEdgeRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigDigFltrMinPulseWidth", (void**)&DAQmxResetAnlgEdgeRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseRate", (void**)&DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseSrc", (void**)&DAQmxResetAnlgEdgeRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigDigSyncEnable", (void**)&DAQmxResetAnlgEdgeRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigHyst", (void**)&DAQmxResetAnlgEdgeRefTrigHyst, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigLvl", (void**)&DAQmxResetAnlgEdgeRefTrigLvl, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigSlope", (void**)&DAQmxResetAnlgEdgeRefTrigSlope, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeRefTrigSrc", (void**)&DAQmxResetAnlgEdgeRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigCoupling", (void**)&DAQmxResetAnlgEdgeStartTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigDigFltrEnable", (void**)&DAQmxResetAnlgEdgeStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigDigFltrMinPulseWidth", (void**)&DAQmxResetAnlgEdgeStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseRate", (void**)&DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseSrc", (void**)&DAQmxResetAnlgEdgeStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigDigSyncEnable", (void**)&DAQmxResetAnlgEdgeStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigHyst", (void**)&DAQmxResetAnlgEdgeStartTrigHyst, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigLvl", (void**)&DAQmxResetAnlgEdgeStartTrigLvl, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigSlope", (void**)&DAQmxResetAnlgEdgeStartTrigSlope, Dylib::Import::cMangled },
  { "DAQmxResetAnlgEdgeStartTrigSrc", (void**)&DAQmxResetAnlgEdgeStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigCoupling", (void**)&DAQmxResetAnlgLvlPauseTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigDigFltrEnable", (void**)&DAQmxResetAnlgLvlPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxResetAnlgLvlPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseRate", (void**)&DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxResetAnlgLvlPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigDigSyncEnable", (void**)&DAQmxResetAnlgLvlPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigHyst", (void**)&DAQmxResetAnlgLvlPauseTrigHyst, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigLvl", (void**)&DAQmxResetAnlgLvlPauseTrigLvl, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigSrc", (void**)&DAQmxResetAnlgLvlPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgLvlPauseTrigWhen", (void**)&DAQmxResetAnlgLvlPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeRefTrigCouplings", (void**)&DAQmxResetAnlgMultiEdgeRefTrigCouplings, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeRefTrigHysts", (void**)&DAQmxResetAnlgMultiEdgeRefTrigHysts, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeRefTrigLvls", (void**)&DAQmxResetAnlgMultiEdgeRefTrigLvls, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeRefTrigSlopes", (void**)&DAQmxResetAnlgMultiEdgeRefTrigSlopes, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeRefTrigSrcs", (void**)&DAQmxResetAnlgMultiEdgeRefTrigSrcs, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeStartTrigCouplings", (void**)&DAQmxResetAnlgMultiEdgeStartTrigCouplings, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeStartTrigHysts", (void**)&DAQmxResetAnlgMultiEdgeStartTrigHysts, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeStartTrigLvls", (void**)&DAQmxResetAnlgMultiEdgeStartTrigLvls, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeStartTrigSlopes", (void**)&DAQmxResetAnlgMultiEdgeStartTrigSlopes, Dylib::Import::cMangled },
  { "DAQmxResetAnlgMultiEdgeStartTrigSrcs", (void**)&DAQmxResetAnlgMultiEdgeStartTrigSrcs, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigBtm", (void**)&DAQmxResetAnlgWinPauseTrigBtm, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigCoupling", (void**)&DAQmxResetAnlgWinPauseTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigDigFltrEnable", (void**)&DAQmxResetAnlgWinPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxResetAnlgWinPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigDigFltrTimebaseRate", (void**)&DAQmxResetAnlgWinPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxResetAnlgWinPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigDigSyncEnable", (void**)&DAQmxResetAnlgWinPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigSrc", (void**)&DAQmxResetAnlgWinPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigTop", (void**)&DAQmxResetAnlgWinPauseTrigTop, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinPauseTrigWhen", (void**)&DAQmxResetAnlgWinPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigBtm", (void**)&DAQmxResetAnlgWinRefTrigBtm, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigCoupling", (void**)&DAQmxResetAnlgWinRefTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigDigFltrEnable", (void**)&DAQmxResetAnlgWinRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigDigFltrMinPulseWidth", (void**)&DAQmxResetAnlgWinRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigDigFltrTimebaseRate", (void**)&DAQmxResetAnlgWinRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigDigFltrTimebaseSrc", (void**)&DAQmxResetAnlgWinRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigDigSyncEnable", (void**)&DAQmxResetAnlgWinRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigSrc", (void**)&DAQmxResetAnlgWinRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigTop", (void**)&DAQmxResetAnlgWinRefTrigTop, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinRefTrigWhen", (void**)&DAQmxResetAnlgWinRefTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigBtm", (void**)&DAQmxResetAnlgWinStartTrigBtm, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigCoupling", (void**)&DAQmxResetAnlgWinStartTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigDigFltrEnable", (void**)&DAQmxResetAnlgWinStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigDigFltrMinPulseWidth", (void**)&DAQmxResetAnlgWinStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigDigFltrTimebaseRate", (void**)&DAQmxResetAnlgWinStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigDigFltrTimebaseSrc", (void**)&DAQmxResetAnlgWinStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigDigSyncEnable", (void**)&DAQmxResetAnlgWinStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigSrc", (void**)&DAQmxResetAnlgWinStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigTop", (void**)&DAQmxResetAnlgWinStartTrigTop, Dylib::Import::cMangled },
  { "DAQmxResetAnlgWinStartTrigWhen", (void**)&DAQmxResetAnlgWinStartTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetArmStartTrigTimescale", (void**)&DAQmxResetArmStartTrigTimescale, Dylib::Import::cMangled },
  { "DAQmxResetArmStartTrigTimestampEnable", (void**)&DAQmxResetArmStartTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxResetArmStartTrigTimestampTimescale", (void**)&DAQmxResetArmStartTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxResetArmStartTrigTrigWhen", (void**)&DAQmxResetArmStartTrigTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetArmStartTrigType", (void**)&DAQmxResetArmStartTrigType, Dylib::Import::cMangled },
  { "DAQmxResetBufInputBufSize", (void**)&DAQmxResetBufInputBufSize, Dylib::Import::cMangled },
  { "DAQmxResetBufOutputBufSize", (void**)&DAQmxResetBufOutputBufSize, Dylib::Import::cMangled },
  { "DAQmxResetBufOutputOnbrdBufSize", (void**)&DAQmxResetBufOutputOnbrdBufSize, Dylib::Import::cMangled },
  { "DAQmxResetBufferAttribute", (void**)&DAQmxResetBufferAttribute, Dylib::Import::cMangled },
  { "DAQmxResetCIAngEncoderInitialAngle", (void**)&DAQmxResetCIAngEncoderInitialAngle, Dylib::Import::cMangled },
  { "DAQmxResetCIAngEncoderPulsesPerRev", (void**)&DAQmxResetCIAngEncoderPulsesPerRev, Dylib::Import::cMangled },
  { "DAQmxResetCIAngEncoderUnits", (void**)&DAQmxResetCIAngEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesActiveEdge", (void**)&DAQmxResetCICountEdgesActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirDigFltrEnable", (void**)&DAQmxResetCICountEdgesCountDirDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirDigFltrMinPulseWidth", (void**)&DAQmxResetCICountEdgesCountDirDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirDigFltrTimebaseRate", (void**)&DAQmxResetCICountEdgesCountDirDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirDigFltrTimebaseSrc", (void**)&DAQmxResetCICountEdgesCountDirDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirDigSyncEnable", (void**)&DAQmxResetCICountEdgesCountDirDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirHyst", (void**)&DAQmxResetCICountEdgesCountDirHyst, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirLogicLvlBehavior", (void**)&DAQmxResetCICountEdgesCountDirLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirTermCfg", (void**)&DAQmxResetCICountEdgesCountDirTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountDirThreshVoltage", (void**)&DAQmxResetCICountEdgesCountDirThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetActiveEdge", (void**)&DAQmxResetCICountEdgesCountResetActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetDigFltrEnable", (void**)&DAQmxResetCICountEdgesCountResetDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetDigFltrMinPulseWidth", (void**)&DAQmxResetCICountEdgesCountResetDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetDigFltrTimebaseRate", (void**)&DAQmxResetCICountEdgesCountResetDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetDigFltrTimebaseSrc", (void**)&DAQmxResetCICountEdgesCountResetDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetDigSyncEnable", (void**)&DAQmxResetCICountEdgesCountResetDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetEnable", (void**)&DAQmxResetCICountEdgesCountResetEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetHyst", (void**)&DAQmxResetCICountEdgesCountResetHyst, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetLogicLvlBehavior", (void**)&DAQmxResetCICountEdgesCountResetLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetResetCount", (void**)&DAQmxResetCICountEdgesCountResetResetCount, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetTerm", (void**)&DAQmxResetCICountEdgesCountResetTerm, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetTermCfg", (void**)&DAQmxResetCICountEdgesCountResetTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesCountResetThreshVoltage", (void**)&DAQmxResetCICountEdgesCountResetThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDigFltrEnable", (void**)&DAQmxResetCICountEdgesDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDigFltrMinPulseWidth", (void**)&DAQmxResetCICountEdgesDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDigFltrTimebaseRate", (void**)&DAQmxResetCICountEdgesDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDigFltrTimebaseSrc", (void**)&DAQmxResetCICountEdgesDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDigSyncEnable", (void**)&DAQmxResetCICountEdgesDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDir", (void**)&DAQmxResetCICountEdgesDir, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesDirTerm", (void**)&DAQmxResetCICountEdgesDirTerm, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateDigFltrEnable", (void**)&DAQmxResetCICountEdgesGateDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateDigFltrMinPulseWidth", (void**)&DAQmxResetCICountEdgesGateDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateDigFltrTimebaseRate", (void**)&DAQmxResetCICountEdgesGateDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateDigFltrTimebaseSrc", (void**)&DAQmxResetCICountEdgesGateDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateEnable", (void**)&DAQmxResetCICountEdgesGateEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateHyst", (void**)&DAQmxResetCICountEdgesGateHyst, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateLogicLvlBehavior", (void**)&DAQmxResetCICountEdgesGateLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateTerm", (void**)&DAQmxResetCICountEdgesGateTerm, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateTermCfg", (void**)&DAQmxResetCICountEdgesGateTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateThreshVoltage", (void**)&DAQmxResetCICountEdgesGateThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesGateWhen", (void**)&DAQmxResetCICountEdgesGateWhen, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesHyst", (void**)&DAQmxResetCICountEdgesHyst, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesInitialCnt", (void**)&DAQmxResetCICountEdgesInitialCnt, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesLogicLvlBehavior", (void**)&DAQmxResetCICountEdgesLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesTerm", (void**)&DAQmxResetCICountEdgesTerm, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesTermCfg", (void**)&DAQmxResetCICountEdgesTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCICountEdgesThreshVoltage", (void**)&DAQmxResetCICountEdgesThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseActiveEdge", (void**)&DAQmxResetCICtrTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseDigFltrEnable", (void**)&DAQmxResetCICtrTimebaseDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseDigFltrMinPulseWidth", (void**)&DAQmxResetCICtrTimebaseDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseDigFltrTimebaseRate", (void**)&DAQmxResetCICtrTimebaseDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseDigFltrTimebaseSrc", (void**)&DAQmxResetCICtrTimebaseDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseDigSyncEnable", (void**)&DAQmxResetCICtrTimebaseDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseMasterTimebaseDiv", (void**)&DAQmxResetCICtrTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseRate", (void**)&DAQmxResetCICtrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCICtrTimebaseSrc", (void**)&DAQmxResetCICtrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCICustomScaleName", (void**)&DAQmxResetCICustomScaleName, Dylib::Import::cMangled },
  { "DAQmxResetCIDataXferMech", (void**)&DAQmxResetCIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxResetCIDataXferReqCond", (void**)&DAQmxResetCIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxResetCIDupCountPrevent", (void**)&DAQmxResetCIDupCountPrevent, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleDigFltrEnable", (void**)&DAQmxResetCIDutyCycleDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleDigFltrMinPulseWidth", (void**)&DAQmxResetCIDutyCycleDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleDigFltrTimebaseRate", (void**)&DAQmxResetCIDutyCycleDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleDigFltrTimebaseSrc", (void**)&DAQmxResetCIDutyCycleDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleLogicLvlBehavior", (void**)&DAQmxResetCIDutyCycleLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleStartingEdge", (void**)&DAQmxResetCIDutyCycleStartingEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleTerm", (void**)&DAQmxResetCIDutyCycleTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIDutyCycleTermCfg", (void**)&DAQmxResetCIDutyCycleTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputDigFltrEnable", (void**)&DAQmxResetCIEncoderAInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputDigFltrMinPulseWidth", (void**)&DAQmxResetCIEncoderAInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputDigFltrTimebaseRate", (void**)&DAQmxResetCIEncoderAInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputDigFltrTimebaseSrc", (void**)&DAQmxResetCIEncoderAInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputDigSyncEnable", (void**)&DAQmxResetCIEncoderAInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputLogicLvlBehavior", (void**)&DAQmxResetCIEncoderAInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputTerm", (void**)&DAQmxResetCIEncoderAInputTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderAInputTermCfg", (void**)&DAQmxResetCIEncoderAInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputDigFltrEnable", (void**)&DAQmxResetCIEncoderBInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputDigFltrMinPulseWidth", (void**)&DAQmxResetCIEncoderBInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputDigFltrTimebaseRate", (void**)&DAQmxResetCIEncoderBInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputDigFltrTimebaseSrc", (void**)&DAQmxResetCIEncoderBInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputDigSyncEnable", (void**)&DAQmxResetCIEncoderBInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputLogicLvlBehavior", (void**)&DAQmxResetCIEncoderBInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputTerm", (void**)&DAQmxResetCIEncoderBInputTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderBInputTermCfg", (void**)&DAQmxResetCIEncoderBInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderDecodingType", (void**)&DAQmxResetCIEncoderDecodingType, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZIndexEnable", (void**)&DAQmxResetCIEncoderZIndexEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZIndexPhase", (void**)&DAQmxResetCIEncoderZIndexPhase, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZIndexVal", (void**)&DAQmxResetCIEncoderZIndexVal, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputDigFltrEnable", (void**)&DAQmxResetCIEncoderZInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputDigFltrMinPulseWidth", (void**)&DAQmxResetCIEncoderZInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputDigFltrTimebaseRate", (void**)&DAQmxResetCIEncoderZInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputDigFltrTimebaseSrc", (void**)&DAQmxResetCIEncoderZInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputDigSyncEnable", (void**)&DAQmxResetCIEncoderZInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputLogicLvlBehavior", (void**)&DAQmxResetCIEncoderZInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputTerm", (void**)&DAQmxResetCIEncoderZInputTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIEncoderZInputTermCfg", (void**)&DAQmxResetCIEncoderZInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIFilterDelayUnits", (void**)&DAQmxResetCIFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIFilterEnable", (void**)&DAQmxResetCIFilterEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIFilterFreq", (void**)&DAQmxResetCIFilterFreq, Dylib::Import::cMangled },
  { "DAQmxResetCIFilterOrder", (void**)&DAQmxResetCIFilterOrder, Dylib::Import::cMangled },
  { "DAQmxResetCIFilterResponse", (void**)&DAQmxResetCIFilterResponse, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqDigFltrEnable", (void**)&DAQmxResetCIFreqDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqDigFltrMinPulseWidth", (void**)&DAQmxResetCIFreqDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqDigFltrTimebaseRate", (void**)&DAQmxResetCIFreqDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqDigFltrTimebaseSrc", (void**)&DAQmxResetCIFreqDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqDigSyncEnable", (void**)&DAQmxResetCIFreqDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqDiv", (void**)&DAQmxResetCIFreqDiv, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqEnableAveraging", (void**)&DAQmxResetCIFreqEnableAveraging, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqHyst", (void**)&DAQmxResetCIFreqHyst, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqLogicLvlBehavior", (void**)&DAQmxResetCIFreqLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqMeasMeth", (void**)&DAQmxResetCIFreqMeasMeth, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqMeasTime", (void**)&DAQmxResetCIFreqMeasTime, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqStartingEdge", (void**)&DAQmxResetCIFreqStartingEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqTerm", (void**)&DAQmxResetCIFreqTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqTermCfg", (void**)&DAQmxResetCIFreqTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqThreshVoltage", (void**)&DAQmxResetCIFreqThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCIFreqUnits", (void**)&DAQmxResetCIFreqUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIGPSSyncMethod", (void**)&DAQmxResetCIGPSSyncMethod, Dylib::Import::cMangled },
  { "DAQmxResetCIGPSSyncSrc", (void**)&DAQmxResetCIGPSSyncSrc, Dylib::Import::cMangled },
  { "DAQmxResetCILinEncoderDistPerPulse", (void**)&DAQmxResetCILinEncoderDistPerPulse, Dylib::Import::cMangled },
  { "DAQmxResetCILinEncoderInitialPos", (void**)&DAQmxResetCILinEncoderInitialPos, Dylib::Import::cMangled },
  { "DAQmxResetCILinEncoderUnits", (void**)&DAQmxResetCILinEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIMax", (void**)&DAQmxResetCIMax, Dylib::Import::cMangled },
  { "DAQmxResetCIMaxMeasPeriod", (void**)&DAQmxResetCIMaxMeasPeriod, Dylib::Import::cMangled },
  { "DAQmxResetCIMemMapEnable", (void**)&DAQmxResetCIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIMin", (void**)&DAQmxResetCIMin, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodDigFltrEnable", (void**)&DAQmxResetCIPeriodDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodDigFltrMinPulseWidth", (void**)&DAQmxResetCIPeriodDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodDigFltrTimebaseRate", (void**)&DAQmxResetCIPeriodDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodDigFltrTimebaseSrc", (void**)&DAQmxResetCIPeriodDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodDigSyncEnable", (void**)&DAQmxResetCIPeriodDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodDiv", (void**)&DAQmxResetCIPeriodDiv, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodEnableAveraging", (void**)&DAQmxResetCIPeriodEnableAveraging, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodHyst", (void**)&DAQmxResetCIPeriodHyst, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodLogicLvlBehavior", (void**)&DAQmxResetCIPeriodLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodMeasMeth", (void**)&DAQmxResetCIPeriodMeasMeth, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodMeasTime", (void**)&DAQmxResetCIPeriodMeasTime, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodStartingEdge", (void**)&DAQmxResetCIPeriodStartingEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodTerm", (void**)&DAQmxResetCIPeriodTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodTermCfg", (void**)&DAQmxResetCIPeriodTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodThreshVoltage", (void**)&DAQmxResetCIPeriodThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCIPeriodUnits", (void**)&DAQmxResetCIPeriodUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIPrescaler", (void**)&DAQmxResetCIPrescaler, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqDigFltrEnable", (void**)&DAQmxResetCIPulseFreqDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqDigFltrMinPulseWidth", (void**)&DAQmxResetCIPulseFreqDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqDigFltrTimebaseRate", (void**)&DAQmxResetCIPulseFreqDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqDigFltrTimebaseSrc", (void**)&DAQmxResetCIPulseFreqDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqDigSyncEnable", (void**)&DAQmxResetCIPulseFreqDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqLogicLvlBehavior", (void**)&DAQmxResetCIPulseFreqLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqStartEdge", (void**)&DAQmxResetCIPulseFreqStartEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqTerm", (void**)&DAQmxResetCIPulseFreqTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqTermCfg", (void**)&DAQmxResetCIPulseFreqTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseFreqUnits", (void**)&DAQmxResetCIPulseFreqUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksDigFltrEnable", (void**)&DAQmxResetCIPulseTicksDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksDigFltrMinPulseWidth", (void**)&DAQmxResetCIPulseTicksDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksDigFltrTimebaseRate", (void**)&DAQmxResetCIPulseTicksDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksDigFltrTimebaseSrc", (void**)&DAQmxResetCIPulseTicksDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksDigSyncEnable", (void**)&DAQmxResetCIPulseTicksDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksLogicLvlBehavior", (void**)&DAQmxResetCIPulseTicksLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksStartEdge", (void**)&DAQmxResetCIPulseTicksStartEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksTerm", (void**)&DAQmxResetCIPulseTicksTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTicksTermCfg", (void**)&DAQmxResetCIPulseTicksTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeDigFltrEnable", (void**)&DAQmxResetCIPulseTimeDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeDigFltrMinPulseWidth", (void**)&DAQmxResetCIPulseTimeDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeDigFltrTimebaseRate", (void**)&DAQmxResetCIPulseTimeDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeDigFltrTimebaseSrc", (void**)&DAQmxResetCIPulseTimeDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeDigSyncEnable", (void**)&DAQmxResetCIPulseTimeDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeLogicLvlBehavior", (void**)&DAQmxResetCIPulseTimeLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeStartEdge", (void**)&DAQmxResetCIPulseTimeStartEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeTerm", (void**)&DAQmxResetCIPulseTimeTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeTermCfg", (void**)&DAQmxResetCIPulseTimeTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseTimeUnits", (void**)&DAQmxResetCIPulseTimeUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthDigFltrEnable", (void**)&DAQmxResetCIPulseWidthDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthDigFltrMinPulseWidth", (void**)&DAQmxResetCIPulseWidthDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthDigFltrTimebaseRate", (void**)&DAQmxResetCIPulseWidthDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthDigFltrTimebaseSrc", (void**)&DAQmxResetCIPulseWidthDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthDigSyncEnable", (void**)&DAQmxResetCIPulseWidthDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthLogicLvlBehavior", (void**)&DAQmxResetCIPulseWidthLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthStartingEdge", (void**)&DAQmxResetCIPulseWidthStartingEdge, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthTerm", (void**)&DAQmxResetCIPulseWidthTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthTermCfg", (void**)&DAQmxResetCIPulseWidthTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIPulseWidthUnits", (void**)&DAQmxResetCIPulseWidthUnits, Dylib::Import::cMangled },
  { "DAQmxResetCISampClkOverrunBehavior", (void**)&DAQmxResetCISampClkOverrunBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCISampClkOverrunSentinelVal", (void**)&DAQmxResetCISampClkOverrunSentinelVal, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodDigFltrEnable", (void**)&DAQmxResetCISemiPeriodDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodDigFltrMinPulseWidth", (void**)&DAQmxResetCISemiPeriodDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodDigFltrTimebaseRate", (void**)&DAQmxResetCISemiPeriodDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodDigFltrTimebaseSrc", (void**)&DAQmxResetCISemiPeriodDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodDigSyncEnable", (void**)&DAQmxResetCISemiPeriodDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodLogicLvlBehavior", (void**)&DAQmxResetCISemiPeriodLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodStartingEdge", (void**)&DAQmxResetCISemiPeriodStartingEdge, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodTerm", (void**)&DAQmxResetCISemiPeriodTerm, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodTermCfg", (void**)&DAQmxResetCISemiPeriodTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCISemiPeriodUnits", (void**)&DAQmxResetCISemiPeriodUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIThreshVoltage", (void**)&DAQmxResetCIThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxResetCITimestampInitialSeconds", (void**)&DAQmxResetCITimestampInitialSeconds, Dylib::Import::cMangled },
  { "DAQmxResetCITimestampUnits", (void**)&DAQmxResetCITimestampUnits, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstDigFltrEnable", (void**)&DAQmxResetCITwoEdgeSepFirstDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstDigFltrMinPulseWidth", (void**)&DAQmxResetCITwoEdgeSepFirstDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseRate", (void**)&DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseSrc", (void**)&DAQmxResetCITwoEdgeSepFirstDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstDigSyncEnable", (void**)&DAQmxResetCITwoEdgeSepFirstDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstEdge", (void**)&DAQmxResetCITwoEdgeSepFirstEdge, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstLogicLvlBehavior", (void**)&DAQmxResetCITwoEdgeSepFirstLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstTerm", (void**)&DAQmxResetCITwoEdgeSepFirstTerm, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepFirstTermCfg", (void**)&DAQmxResetCITwoEdgeSepFirstTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondDigFltrEnable", (void**)&DAQmxResetCITwoEdgeSepSecondDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondDigFltrMinPulseWidth", (void**)&DAQmxResetCITwoEdgeSepSecondDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseRate", (void**)&DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseSrc", (void**)&DAQmxResetCITwoEdgeSepSecondDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondDigSyncEnable", (void**)&DAQmxResetCITwoEdgeSepSecondDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondEdge", (void**)&DAQmxResetCITwoEdgeSepSecondEdge, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondLogicLvlBehavior", (void**)&DAQmxResetCITwoEdgeSepSecondLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondTerm", (void**)&DAQmxResetCITwoEdgeSepSecondTerm, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepSecondTermCfg", (void**)&DAQmxResetCITwoEdgeSepSecondTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCITwoEdgeSepUnits", (void**)&DAQmxResetCITwoEdgeSepUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIUsbXferReqCount", (void**)&DAQmxResetCIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxResetCIUsbXferReqSize", (void**)&DAQmxResetCIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityAngEncoderPulsesPerRev", (void**)&DAQmxResetCIVelocityAngEncoderPulsesPerRev, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityAngEncoderUnits", (void**)&DAQmxResetCIVelocityAngEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityDiv", (void**)&DAQmxResetCIVelocityDiv, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputDigFltrEnable", (void**)&DAQmxResetCIVelocityEncoderAInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputDigFltrMinPulseWidth", (void**)&DAQmxResetCIVelocityEncoderAInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseRate", (void**)&DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseSrc", (void**)&DAQmxResetCIVelocityEncoderAInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputLogicLvlBehavior", (void**)&DAQmxResetCIVelocityEncoderAInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputTerm", (void**)&DAQmxResetCIVelocityEncoderAInputTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderAInputTermCfg", (void**)&DAQmxResetCIVelocityEncoderAInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputDigFltrEnable", (void**)&DAQmxResetCIVelocityEncoderBInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputDigFltrMinPulseWidth", (void**)&DAQmxResetCIVelocityEncoderBInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseRate", (void**)&DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseSrc", (void**)&DAQmxResetCIVelocityEncoderBInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputLogicLvlBehavior", (void**)&DAQmxResetCIVelocityEncoderBInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputTerm", (void**)&DAQmxResetCIVelocityEncoderBInputTerm, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderBInputTermCfg", (void**)&DAQmxResetCIVelocityEncoderBInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityEncoderDecodingType", (void**)&DAQmxResetCIVelocityEncoderDecodingType, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityLinEncoderDistPerPulse", (void**)&DAQmxResetCIVelocityLinEncoderDistPerPulse, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityLinEncoderUnits", (void**)&DAQmxResetCIVelocityLinEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxResetCIVelocityMeasTime", (void**)&DAQmxResetCIVelocityMeasTime, Dylib::Import::cMangled },
  { "DAQmxResetCOAutoIncrCnt", (void**)&DAQmxResetCOAutoIncrCnt, Dylib::Import::cMangled },
  { "DAQmxResetCOConstrainedGenMode", (void**)&DAQmxResetCOConstrainedGenMode, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseActiveEdge", (void**)&DAQmxResetCOCtrTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseDigFltrEnable", (void**)&DAQmxResetCOCtrTimebaseDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseDigFltrMinPulseWidth", (void**)&DAQmxResetCOCtrTimebaseDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseDigFltrTimebaseRate", (void**)&DAQmxResetCOCtrTimebaseDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseDigFltrTimebaseSrc", (void**)&DAQmxResetCOCtrTimebaseDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseDigSyncEnable", (void**)&DAQmxResetCOCtrTimebaseDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseMasterTimebaseDiv", (void**)&DAQmxResetCOCtrTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseRate", (void**)&DAQmxResetCOCtrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetCOCtrTimebaseSrc", (void**)&DAQmxResetCOCtrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetCODataXferMech", (void**)&DAQmxResetCODataXferMech, Dylib::Import::cMangled },
  { "DAQmxResetCODataXferReqCond", (void**)&DAQmxResetCODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxResetCOEnableInitialDelayOnRetrigger", (void**)&DAQmxResetCOEnableInitialDelayOnRetrigger, Dylib::Import::cMangled },
  { "DAQmxResetCOMemMapEnable", (void**)&DAQmxResetCOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxResetCOPrescaler", (void**)&DAQmxResetCOPrescaler, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseDutyCyc", (void**)&DAQmxResetCOPulseDutyCyc, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseFreq", (void**)&DAQmxResetCOPulseFreq, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseFreqInitialDelay", (void**)&DAQmxResetCOPulseFreqInitialDelay, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseFreqUnits", (void**)&DAQmxResetCOPulseFreqUnits, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseHighTicks", (void**)&DAQmxResetCOPulseHighTicks, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseHighTime", (void**)&DAQmxResetCOPulseHighTime, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseIdleState", (void**)&DAQmxResetCOPulseIdleState, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseLowTicks", (void**)&DAQmxResetCOPulseLowTicks, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseLowTime", (void**)&DAQmxResetCOPulseLowTime, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseTerm", (void**)&DAQmxResetCOPulseTerm, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseTicksInitialDelay", (void**)&DAQmxResetCOPulseTicksInitialDelay, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseTimeInitialDelay", (void**)&DAQmxResetCOPulseTimeInitialDelay, Dylib::Import::cMangled },
  { "DAQmxResetCOPulseTimeUnits", (void**)&DAQmxResetCOPulseTimeUnits, Dylib::Import::cMangled },
  { "DAQmxResetCOUsbXferReqCount", (void**)&DAQmxResetCOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxResetCOUsbXferReqSize", (void**)&DAQmxResetCOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxResetCOUseOnlyOnBrdMem", (void**)&DAQmxResetCOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxResetChanAttribute", (void**)&DAQmxResetChanAttribute, Dylib::Import::cMangled },
  { "DAQmxResetChanDescr", (void**)&DAQmxResetChanDescr, Dylib::Import::cMangled },
  { "DAQmxResetChanSyncUnlockBehavior", (void**)&DAQmxResetChanSyncUnlockBehavior, Dylib::Import::cMangled },
  { "DAQmxResetChangeDetectDIFallingEdgePhysicalChans", (void**)&DAQmxResetChangeDetectDIFallingEdgePhysicalChans, Dylib::Import::cMangled },
  { "DAQmxResetChangeDetectDIRisingEdgePhysicalChans", (void**)&DAQmxResetChangeDetectDIRisingEdgePhysicalChans, Dylib::Import::cMangled },
  { "DAQmxResetChangeDetectDITristate", (void**)&DAQmxResetChangeDetectDITristate, Dylib::Import::cMangled },
  { "DAQmxResetDIAcquireOn", (void**)&DAQmxResetDIAcquireOn, Dylib::Import::cMangled },
  { "DAQmxResetDIDataXferMech", (void**)&DAQmxResetDIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxResetDIDataXferReqCond", (void**)&DAQmxResetDIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxResetDIDigFltrEnable", (void**)&DAQmxResetDIDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetDIDigFltrEnableBusMode", (void**)&DAQmxResetDIDigFltrEnableBusMode, Dylib::Import::cMangled },
  { "DAQmxResetDIDigFltrMinPulseWidth", (void**)&DAQmxResetDIDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetDIDigFltrTimebaseRate", (void**)&DAQmxResetDIDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetDIDigFltrTimebaseSrc", (void**)&DAQmxResetDIDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetDIDigSyncEnable", (void**)&DAQmxResetDIDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetDIInvertLines", (void**)&DAQmxResetDIInvertLines, Dylib::Import::cMangled },
  { "DAQmxResetDILogicFamily", (void**)&DAQmxResetDILogicFamily, Dylib::Import::cMangled },
  { "DAQmxResetDIMemMapEnable", (void**)&DAQmxResetDIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxResetDITristate", (void**)&DAQmxResetDITristate, Dylib::Import::cMangled },
  { "DAQmxResetDIUsbXferReqCount", (void**)&DAQmxResetDIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxResetDIUsbXferReqSize", (void**)&DAQmxResetDIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxResetDODataXferMech", (void**)&DAQmxResetDODataXferMech, Dylib::Import::cMangled },
  { "DAQmxResetDODataXferReqCond", (void**)&DAQmxResetDODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxResetDOGenerateOn", (void**)&DAQmxResetDOGenerateOn, Dylib::Import::cMangled },
  { "DAQmxResetDOInvertLines", (void**)&DAQmxResetDOInvertLines, Dylib::Import::cMangled },
  { "DAQmxResetDOLineStatesDoneState", (void**)&DAQmxResetDOLineStatesDoneState, Dylib::Import::cMangled },
  { "DAQmxResetDOLineStatesPausedState", (void**)&DAQmxResetDOLineStatesPausedState, Dylib::Import::cMangled },
  { "DAQmxResetDOLineStatesStartState", (void**)&DAQmxResetDOLineStatesStartState, Dylib::Import::cMangled },
  { "DAQmxResetDOLogicFamily", (void**)&DAQmxResetDOLogicFamily, Dylib::Import::cMangled },
  { "DAQmxResetDOMemMapEnable", (void**)&DAQmxResetDOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxResetDOOutputDriveType", (void**)&DAQmxResetDOOutputDriveType, Dylib::Import::cMangled },
  { "DAQmxResetDOOvercurrentAutoReenable", (void**)&DAQmxResetDOOvercurrentAutoReenable, Dylib::Import::cMangled },
  { "DAQmxResetDOOvercurrentLimit", (void**)&DAQmxResetDOOvercurrentLimit, Dylib::Import::cMangled },
  { "DAQmxResetDOOvercurrentReenablePeriod", (void**)&DAQmxResetDOOvercurrentReenablePeriod, Dylib::Import::cMangled },
  { "DAQmxResetDOTristate", (void**)&DAQmxResetDOTristate, Dylib::Import::cMangled },
  { "DAQmxResetDOUsbXferReqCount", (void**)&DAQmxResetDOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxResetDOUsbXferReqSize", (void**)&DAQmxResetDOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxResetDOUseOnlyOnBrdMem", (void**)&DAQmxResetDOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxResetDelayFromSampClkDelay", (void**)&DAQmxResetDelayFromSampClkDelay, Dylib::Import::cMangled },
  { "DAQmxResetDelayFromSampClkDelayEx", (void**)&DAQmxResetDelayFromSampClkDelayEx, Dylib::Import::cMangled },
  { "DAQmxResetDelayFromSampClkDelayUnits", (void**)&DAQmxResetDelayFromSampClkDelayUnits, Dylib::Import::cMangled },
  { "DAQmxResetDelayFromSampClkDelayUnitsEx", (void**)&DAQmxResetDelayFromSampClkDelayUnitsEx, Dylib::Import::cMangled },
  { "DAQmxResetDevice", (void**)&DAQmxResetDevice, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeAdvTrigDigFltrEnable", (void**)&DAQmxResetDigEdgeAdvTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeAdvTrigEdge", (void**)&DAQmxResetDigEdgeAdvTrigEdge, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeAdvTrigSrc", (void**)&DAQmxResetDigEdgeAdvTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigDigFltrEnable", (void**)&DAQmxResetDigEdgeArmStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigDigFltrMinPulseWidth", (void**)&DAQmxResetDigEdgeArmStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseRate", (void**)&DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseSrc", (void**)&DAQmxResetDigEdgeArmStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigDigSyncEnable", (void**)&DAQmxResetDigEdgeArmStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigEdge", (void**)&DAQmxResetDigEdgeArmStartTrigEdge, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeArmStartTrigSrc", (void**)&DAQmxResetDigEdgeArmStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigDigFltrEnable", (void**)&DAQmxResetDigEdgeRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigDigFltrMinPulseWidth", (void**)&DAQmxResetDigEdgeRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigDigFltrTimebaseRate", (void**)&DAQmxResetDigEdgeRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigDigFltrTimebaseSrc", (void**)&DAQmxResetDigEdgeRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigDigSyncEnable", (void**)&DAQmxResetDigEdgeRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigEdge", (void**)&DAQmxResetDigEdgeRefTrigEdge, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeRefTrigSrc", (void**)&DAQmxResetDigEdgeRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigDigFltrEnable", (void**)&DAQmxResetDigEdgeStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigDigFltrMinPulseWidth", (void**)&DAQmxResetDigEdgeStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigDigFltrTimebaseRate", (void**)&DAQmxResetDigEdgeStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigDigFltrTimebaseSrc", (void**)&DAQmxResetDigEdgeStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigDigSyncEnable", (void**)&DAQmxResetDigEdgeStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigEdge", (void**)&DAQmxResetDigEdgeStartTrigEdge, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeStartTrigSrc", (void**)&DAQmxResetDigEdgeStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeWatchdogExpirTrigEdge", (void**)&DAQmxResetDigEdgeWatchdogExpirTrigEdge, Dylib::Import::cMangled },
  { "DAQmxResetDigEdgeWatchdogExpirTrigSrc", (void**)&DAQmxResetDigEdgeWatchdogExpirTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigDigFltrEnable", (void**)&DAQmxResetDigLvlPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxResetDigLvlPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigDigFltrTimebaseRate", (void**)&DAQmxResetDigLvlPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxResetDigLvlPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigDigSyncEnable", (void**)&DAQmxResetDigLvlPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigSrc", (void**)&DAQmxResetDigLvlPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigLvlPauseTrigWhen", (void**)&DAQmxResetDigLvlPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternPauseTrigPattern", (void**)&DAQmxResetDigPatternPauseTrigPattern, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternPauseTrigSrc", (void**)&DAQmxResetDigPatternPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternPauseTrigWhen", (void**)&DAQmxResetDigPatternPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternRefTrigPattern", (void**)&DAQmxResetDigPatternRefTrigPattern, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternRefTrigSrc", (void**)&DAQmxResetDigPatternRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternRefTrigWhen", (void**)&DAQmxResetDigPatternRefTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternStartTrigPattern", (void**)&DAQmxResetDigPatternStartTrigPattern, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternStartTrigSrc", (void**)&DAQmxResetDigPatternStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetDigPatternStartTrigWhen", (void**)&DAQmxResetDigPatternStartTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetExported10MHzRefClkOutputTerm", (void**)&DAQmxResetExported10MHzRefClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExported20MHzTimebaseOutputTerm", (void**)&DAQmxResetExported20MHzTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedAIConvClkOutputTerm", (void**)&DAQmxResetExportedAIConvClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedAIHoldCmpltEventOutputTerm", (void**)&DAQmxResetExportedAIHoldCmpltEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedAIHoldCmpltEventPulsePolarity", (void**)&DAQmxResetExportedAIHoldCmpltEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvCmpltEventDelay", (void**)&DAQmxResetExportedAdvCmpltEventDelay, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvCmpltEventOutputTerm", (void**)&DAQmxResetExportedAdvCmpltEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvCmpltEventPulsePolarity", (void**)&DAQmxResetExportedAdvCmpltEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvCmpltEventPulseWidth", (void**)&DAQmxResetExportedAdvCmpltEventPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvTrigOutputTerm", (void**)&DAQmxResetExportedAdvTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvTrigPulseWidth", (void**)&DAQmxResetExportedAdvTrigPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetExportedAdvTrigPulseWidthUnits", (void**)&DAQmxResetExportedAdvTrigPulseWidthUnits, Dylib::Import::cMangled },
  { "DAQmxResetExportedChangeDetectEventOutputTerm", (void**)&DAQmxResetExportedChangeDetectEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedChangeDetectEventPulsePolarity", (void**)&DAQmxResetExportedChangeDetectEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedCtrOutEventOutputBehavior", (void**)&DAQmxResetExportedCtrOutEventOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxResetExportedCtrOutEventOutputTerm", (void**)&DAQmxResetExportedCtrOutEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedCtrOutEventPulsePolarity", (void**)&DAQmxResetExportedCtrOutEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedCtrOutEventToggleIdleState", (void**)&DAQmxResetExportedCtrOutEventToggleIdleState, Dylib::Import::cMangled },
  { "DAQmxResetExportedDataActiveEventLvlActiveLvl", (void**)&DAQmxResetExportedDataActiveEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxResetExportedDataActiveEventOutputTerm", (void**)&DAQmxResetExportedDataActiveEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedDividedSampClkTimebaseOutputTerm", (void**)&DAQmxResetExportedDividedSampClkTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventDelay", (void**)&DAQmxResetExportedHshkEventDelay, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventInterlockedAssertOnStart", (void**)&DAQmxResetExportedHshkEventInterlockedAssertOnStart, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventInterlockedAssertedLvl", (void**)&DAQmxResetExportedHshkEventInterlockedAssertedLvl, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventInterlockedDeassertDelay", (void**)&DAQmxResetExportedHshkEventInterlockedDeassertDelay, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventOutputBehavior", (void**)&DAQmxResetExportedHshkEventOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventOutputTerm", (void**)&DAQmxResetExportedHshkEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventPulsePolarity", (void**)&DAQmxResetExportedHshkEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedHshkEventPulseWidth", (void**)&DAQmxResetExportedHshkEventPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetExportedPauseTrigLvlActiveLvl", (void**)&DAQmxResetExportedPauseTrigLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxResetExportedPauseTrigOutputTerm", (void**)&DAQmxResetExportedPauseTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedRdyForStartEventLvlActiveLvl", (void**)&DAQmxResetExportedRdyForStartEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxResetExportedRdyForStartEventOutputTerm", (void**)&DAQmxResetExportedRdyForStartEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedRdyForXferEventDeassertCond", (void**)&DAQmxResetExportedRdyForXferEventDeassertCond, Dylib::Import::cMangled },
  { "DAQmxResetExportedRdyForXferEventDeassertCondCustomThreshold", (void**)&DAQmxResetExportedRdyForXferEventDeassertCondCustomThreshold, Dylib::Import::cMangled },
  { "DAQmxResetExportedRdyForXferEventLvlActiveLvl", (void**)&DAQmxResetExportedRdyForXferEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxResetExportedRdyForXferEventOutputTerm", (void**)&DAQmxResetExportedRdyForXferEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedRefTrigOutputTerm", (void**)&DAQmxResetExportedRefTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedRefTrigPulsePolarity", (void**)&DAQmxResetExportedRefTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedSampClkDelayOffset", (void**)&DAQmxResetExportedSampClkDelayOffset, Dylib::Import::cMangled },
  { "DAQmxResetExportedSampClkOutputBehavior", (void**)&DAQmxResetExportedSampClkOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxResetExportedSampClkOutputTerm", (void**)&DAQmxResetExportedSampClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedSampClkPulsePolarity", (void**)&DAQmxResetExportedSampClkPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedSampClkTimebaseOutputTerm", (void**)&DAQmxResetExportedSampClkTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedSignalAttribute", (void**)&DAQmxResetExportedSignalAttribute, Dylib::Import::cMangled },
  { "DAQmxResetExportedStartTrigOutputTerm", (void**)&DAQmxResetExportedStartTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedStartTrigPulsePolarity", (void**)&DAQmxResetExportedStartTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxResetExportedSyncPulseEventOutputTerm", (void**)&DAQmxResetExportedSyncPulseEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetExportedWatchdogExpiredEventOutputTerm", (void**)&DAQmxResetExportedWatchdogExpiredEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxResetFirstSampClkOffset", (void**)&DAQmxResetFirstSampClkOffset, Dylib::Import::cMangled },
  { "DAQmxResetFirstSampClkTimescale", (void**)&DAQmxResetFirstSampClkTimescale, Dylib::Import::cMangled },
  { "DAQmxResetFirstSampClkWhen", (void**)&DAQmxResetFirstSampClkWhen, Dylib::Import::cMangled },
  { "DAQmxResetFirstSampTimestampEnable", (void**)&DAQmxResetFirstSampTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxResetFirstSampTimestampTimescale", (void**)&DAQmxResetFirstSampTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxResetHshkDelayAfterXfer", (void**)&DAQmxResetHshkDelayAfterXfer, Dylib::Import::cMangled },
  { "DAQmxResetHshkSampleInputDataWhen", (void**)&DAQmxResetHshkSampleInputDataWhen, Dylib::Import::cMangled },
  { "DAQmxResetHshkStartCond", (void**)&DAQmxResetHshkStartCond, Dylib::Import::cMangled },
  { "DAQmxResetHshkTrigType", (void**)&DAQmxResetHshkTrigType, Dylib::Import::cMangled },
  { "DAQmxResetImplicitUnderflowBehavior", (void**)&DAQmxResetImplicitUnderflowBehavior, Dylib::Import::cMangled },
  { "DAQmxResetInterlockedHshkTrigAssertedLvl", (void**)&DAQmxResetInterlockedHshkTrigAssertedLvl, Dylib::Import::cMangled },
  { "DAQmxResetInterlockedHshkTrigSrc", (void**)&DAQmxResetInterlockedHshkTrigSrc, Dylib::Import::cMangled },
  { "DAQmxResetLoggingFilePath", (void**)&DAQmxResetLoggingFilePath, Dylib::Import::cMangled },
  { "DAQmxResetLoggingFilePreallocationSize", (void**)&DAQmxResetLoggingFilePreallocationSize, Dylib::Import::cMangled },
  { "DAQmxResetLoggingFileWriteSize", (void**)&DAQmxResetLoggingFileWriteSize, Dylib::Import::cMangled },
  { "DAQmxResetLoggingMode", (void**)&DAQmxResetLoggingMode, Dylib::Import::cMangled },
  { "DAQmxResetLoggingPause", (void**)&DAQmxResetLoggingPause, Dylib::Import::cMangled },
  { "DAQmxResetLoggingSampsPerFile", (void**)&DAQmxResetLoggingSampsPerFile, Dylib::Import::cMangled },
  { "DAQmxResetLoggingTDMSGroupName", (void**)&DAQmxResetLoggingTDMSGroupName, Dylib::Import::cMangled },
  { "DAQmxResetLoggingTDMSOperation", (void**)&DAQmxResetLoggingTDMSOperation, Dylib::Import::cMangled },
  { "DAQmxResetMasterTimebaseRate", (void**)&DAQmxResetMasterTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetMasterTimebaseSrc", (void**)&DAQmxResetMasterTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetOnDemandSimultaneousAOEnable", (void**)&DAQmxResetOnDemandSimultaneousAOEnable, Dylib::Import::cMangled },
  { "DAQmxResetPauseTrigType", (void**)&DAQmxResetPauseTrigType, Dylib::Import::cMangled },
  { "DAQmxResetPhysicalChanAIPowerControlEnable", (void**)&DAQmxResetPhysicalChanAIPowerControlEnable, Dylib::Import::cMangled },
  { "DAQmxResetPhysicalChanAIPowerControlType", (void**)&DAQmxResetPhysicalChanAIPowerControlType, Dylib::Import::cMangled },
  { "DAQmxResetPhysicalChanAIPowerControlVoltage", (void**)&DAQmxResetPhysicalChanAIPowerControlVoltage, Dylib::Import::cMangled },
  { "DAQmxResetPhysicalChanAOManualControlEnable", (void**)&DAQmxResetPhysicalChanAOManualControlEnable, Dylib::Import::cMangled },
  { "DAQmxResetReadAttribute", (void**)&DAQmxResetReadAttribute, Dylib::Import::cMangled },
  { "DAQmxResetReadAutoStart", (void**)&DAQmxResetReadAutoStart, Dylib::Import::cMangled },
  { "DAQmxResetReadChannelsToRead", (void**)&DAQmxResetReadChannelsToRead, Dylib::Import::cMangled },
  { "DAQmxResetReadOffset", (void**)&DAQmxResetReadOffset, Dylib::Import::cMangled },
  { "DAQmxResetReadOverWrite", (void**)&DAQmxResetReadOverWrite, Dylib::Import::cMangled },
  { "DAQmxResetReadReadAllAvailSamp", (void**)&DAQmxResetReadReadAllAvailSamp, Dylib::Import::cMangled },
  { "DAQmxResetReadRelativeTo", (void**)&DAQmxResetReadRelativeTo, Dylib::Import::cMangled },
  { "DAQmxResetReadSleepTime", (void**)&DAQmxResetReadSleepTime, Dylib::Import::cMangled },
  { "DAQmxResetReadWaitMode", (void**)&DAQmxResetReadWaitMode, Dylib::Import::cMangled },
  { "DAQmxResetRealTimeAttribute", (void**)&DAQmxResetRealTimeAttribute, Dylib::Import::cMangled },
  { "DAQmxResetRealTimeConvLateErrorsToWarnings", (void**)&DAQmxResetRealTimeConvLateErrorsToWarnings, Dylib::Import::cMangled },
  { "DAQmxResetRealTimeNumOfWarmupIters", (void**)&DAQmxResetRealTimeNumOfWarmupIters, Dylib::Import::cMangled },
  { "DAQmxResetRealTimeReportMissedSamp", (void**)&DAQmxResetRealTimeReportMissedSamp, Dylib::Import::cMangled },
  { "DAQmxResetRealTimeWaitForNextSampClkWaitMode", (void**)&DAQmxResetRealTimeWaitForNextSampClkWaitMode, Dylib::Import::cMangled },
  { "DAQmxResetRealTimeWriteRecoveryMode", (void**)&DAQmxResetRealTimeWriteRecoveryMode, Dylib::Import::cMangled },
  { "DAQmxResetRefClkRate", (void**)&DAQmxResetRefClkRate, Dylib::Import::cMangled },
  { "DAQmxResetRefClkSrc", (void**)&DAQmxResetRefClkSrc, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigAutoTrigEnable", (void**)&DAQmxResetRefTrigAutoTrigEnable, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigDelay", (void**)&DAQmxResetRefTrigDelay, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigMaxNumTrigsToDetect", (void**)&DAQmxResetRefTrigMaxNumTrigsToDetect, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigPretrigSamples", (void**)&DAQmxResetRefTrigPretrigSamples, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigRetriggerWin", (void**)&DAQmxResetRefTrigRetriggerWin, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigRetriggerable", (void**)&DAQmxResetRefTrigRetriggerable, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigTimestampEnable", (void**)&DAQmxResetRefTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigTimestampTimescale", (void**)&DAQmxResetRefTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigTrigWin", (void**)&DAQmxResetRefTrigTrigWin, Dylib::Import::cMangled },
  { "DAQmxResetRefTrigType", (void**)&DAQmxResetRefTrigType, Dylib::Import::cMangled },
  { "DAQmxResetSampClkActiveEdge", (void**)&DAQmxResetSampClkActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetSampClkDigFltrEnable", (void**)&DAQmxResetSampClkDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxResetSampClkDigFltrMinPulseWidth", (void**)&DAQmxResetSampClkDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxResetSampClkDigFltrTimebaseRate", (void**)&DAQmxResetSampClkDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetSampClkDigFltrTimebaseSrc", (void**)&DAQmxResetSampClkDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetSampClkDigSyncEnable", (void**)&DAQmxResetSampClkDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxResetSampClkOverrunBehavior", (void**)&DAQmxResetSampClkOverrunBehavior, Dylib::Import::cMangled },
  { "DAQmxResetSampClkRate", (void**)&DAQmxResetSampClkRate, Dylib::Import::cMangled },
  { "DAQmxResetSampClkSrc", (void**)&DAQmxResetSampClkSrc, Dylib::Import::cMangled },
  { "DAQmxResetSampClkTimebaseActiveEdge", (void**)&DAQmxResetSampClkTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxResetSampClkTimebaseDiv", (void**)&DAQmxResetSampClkTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxResetSampClkTimebaseMasterTimebaseDiv", (void**)&DAQmxResetSampClkTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxResetSampClkTimebaseRate", (void**)&DAQmxResetSampClkTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxResetSampClkTimebaseSrc", (void**)&DAQmxResetSampClkTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxResetSampClkTimingResponseMode", (void**)&DAQmxResetSampClkTimingResponseMode, Dylib::Import::cMangled },
  { "DAQmxResetSampClkUnderflowBehavior", (void**)&DAQmxResetSampClkUnderflowBehavior, Dylib::Import::cMangled },
  { "DAQmxResetSampClkWriteWfmUseInitialWfmDT", (void**)&DAQmxResetSampClkWriteWfmUseInitialWfmDT, Dylib::Import::cMangled },
  { "DAQmxResetSampQuantSampMode", (void**)&DAQmxResetSampQuantSampMode, Dylib::Import::cMangled },
  { "DAQmxResetSampQuantSampPerChan", (void**)&DAQmxResetSampQuantSampPerChan, Dylib::Import::cMangled },
  { "DAQmxResetSampTimingEngine", (void**)&DAQmxResetSampTimingEngine, Dylib::Import::cMangled },
  { "DAQmxResetSampTimingType", (void**)&DAQmxResetSampTimingType, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigDelay", (void**)&DAQmxResetStartTrigDelay, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigDelayUnits", (void**)&DAQmxResetStartTrigDelayUnits, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigMaxNumTrigsToDetect", (void**)&DAQmxResetStartTrigMaxNumTrigsToDetect, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigRetriggerWin", (void**)&DAQmxResetStartTrigRetriggerWin, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigRetriggerable", (void**)&DAQmxResetStartTrigRetriggerable, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigTimescale", (void**)&DAQmxResetStartTrigTimescale, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigTimestampEnable", (void**)&DAQmxResetStartTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigTimestampTimescale", (void**)&DAQmxResetStartTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigTrigWhen", (void**)&DAQmxResetStartTrigTrigWhen, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigTrigWin", (void**)&DAQmxResetStartTrigTrigWin, Dylib::Import::cMangled },
  { "DAQmxResetStartTrigType", (void**)&DAQmxResetStartTrigType, Dylib::Import::cMangled },
  { "DAQmxResetSwitchScanAttribute", (void**)&DAQmxResetSwitchScanAttribute, Dylib::Import::cMangled },
  { "DAQmxResetSwitchScanBreakMode", (void**)&DAQmxResetSwitchScanBreakMode, Dylib::Import::cMangled },
  { "DAQmxResetSwitchScanRepeatMode", (void**)&DAQmxResetSwitchScanRepeatMode, Dylib::Import::cMangled },
  { "DAQmxResetSyncClkInterval", (void**)&DAQmxResetSyncClkInterval, Dylib::Import::cMangled },
  { "DAQmxResetSyncPulseMinDelayToStart", (void**)&DAQmxResetSyncPulseMinDelayToStart, Dylib::Import::cMangled },
  { "DAQmxResetSyncPulseResetDelay", (void**)&DAQmxResetSyncPulseResetDelay, Dylib::Import::cMangled },
  { "DAQmxResetSyncPulseSrc", (void**)&DAQmxResetSyncPulseSrc, Dylib::Import::cMangled },
  { "DAQmxResetSyncPulseTimeTimescale", (void**)&DAQmxResetSyncPulseTimeTimescale, Dylib::Import::cMangled },
  { "DAQmxResetSyncPulseTimeWhen", (void**)&DAQmxResetSyncPulseTimeWhen, Dylib::Import::cMangled },
  { "DAQmxResetSyncPulseType", (void**)&DAQmxResetSyncPulseType, Dylib::Import::cMangled },
  { "DAQmxResetTimingAttribute", (void**)&DAQmxResetTimingAttribute, Dylib::Import::cMangled },
  { "DAQmxResetTimingAttributeEx", (void**)&DAQmxResetTimingAttributeEx, Dylib::Import::cMangled },
  { "DAQmxResetTrigAttribute", (void**)&DAQmxResetTrigAttribute, Dylib::Import::cMangled },
  { "DAQmxResetTriggerSyncType", (void**)&DAQmxResetTriggerSyncType, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogAOExpirState", (void**)&DAQmxResetWatchdogAOExpirState, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogAOOutputType", (void**)&DAQmxResetWatchdogAOOutputType, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogAttribute", (void**)&DAQmxResetWatchdogAttribute, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogCOExpirState", (void**)&DAQmxResetWatchdogCOExpirState, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogDOExpirState", (void**)&DAQmxResetWatchdogDOExpirState, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogExpirTrigTrigOnNetworkConnLoss", (void**)&DAQmxResetWatchdogExpirTrigTrigOnNetworkConnLoss, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogExpirTrigType", (void**)&DAQmxResetWatchdogExpirTrigType, Dylib::Import::cMangled },
  { "DAQmxResetWatchdogTimeout", (void**)&DAQmxResetWatchdogTimeout, Dylib::Import::cMangled },
  { "DAQmxResetWriteAttribute", (void**)&DAQmxResetWriteAttribute, Dylib::Import::cMangled },
  { "DAQmxResetWriteOffset", (void**)&DAQmxResetWriteOffset, Dylib::Import::cMangled },
  { "DAQmxResetWriteRegenMode", (void**)&DAQmxResetWriteRegenMode, Dylib::Import::cMangled },
  { "DAQmxResetWriteRelativeTo", (void**)&DAQmxResetWriteRelativeTo, Dylib::Import::cMangled },
  { "DAQmxResetWriteSleepTime", (void**)&DAQmxResetWriteSleepTime, Dylib::Import::cMangled },
  { "DAQmxResetWriteWaitMode", (void**)&DAQmxResetWriteWaitMode, Dylib::Import::cMangled },
  { "DAQmxRestoreLastExtCalConst", (void**)&DAQmxRestoreLastExtCalConst, Dylib::Import::cMangled },
  { "DAQmxSCBaseboardCalAdjust", (void**)&DAQmxSCBaseboardCalAdjust, Dylib::Import::cMangled },
  { "DAQmxSSeriesCalAdjust", (void**)&DAQmxSSeriesCalAdjust, Dylib::Import::cMangled },
  { "DAQmxSaveGlobalChan", (void**)&DAQmxSaveGlobalChan, Dylib::Import::cMangled },
  { "DAQmxSaveScale", (void**)&DAQmxSaveScale, Dylib::Import::cMangled },
  { "DAQmxSaveTask", (void**)&DAQmxSaveTask, Dylib::Import::cMangled },
  { "DAQmxSelfCal", (void**)&DAQmxSelfCal, Dylib::Import::cMangled },
  { "DAQmxSelfTestDevice", (void**)&DAQmxSelfTestDevice, Dylib::Import::cMangled },
  { "DAQmxSendSoftwareTrigger", (void**)&DAQmxSendSoftwareTrigger, Dylib::Import::cMangled },
  { "DAQmxSetAIACExcitFreq", (void**)&DAQmxSetAIACExcitFreq, Dylib::Import::cMangled },
  { "DAQmxSetAIACExcitSyncEnable", (void**)&DAQmxSetAIACExcitSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIACExcitWireMode", (void**)&DAQmxSetAIACExcitWireMode, Dylib::Import::cMangled },
  { "DAQmxSetAIADCCustomTimingMode", (void**)&DAQmxSetAIADCCustomTimingMode, Dylib::Import::cMangled },
  { "DAQmxSetAIADCTimingMode", (void**)&DAQmxSetAIADCTimingMode, Dylib::Import::cMangled },
  { "DAQmxSetAIAccel4WireDCVoltageSensitivity", (void**)&DAQmxSetAIAccel4WireDCVoltageSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIAccel4WireDCVoltageSensitivityUnits", (void**)&DAQmxSetAIAccel4WireDCVoltageSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIAccelChargeSensitivity", (void**)&DAQmxSetAIAccelChargeSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIAccelChargeSensitivityUnits", (void**)&DAQmxSetAIAccelChargeSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIAccelSensitivity", (void**)&DAQmxSetAIAccelSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIAccelSensitivityUnits", (void**)&DAQmxSetAIAccelSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIAccelUnits", (void**)&DAQmxSetAIAccelUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIAcceldBRef", (void**)&DAQmxSetAIAcceldBRef, Dylib::Import::cMangled },
  { "DAQmxSetAIAtten", (void**)&DAQmxSetAIAtten, Dylib::Import::cMangled },
  { "DAQmxSetAIAutoZeroMode", (void**)&DAQmxSetAIAutoZeroMode, Dylib::Import::cMangled },
  { "DAQmxSetAIAveragingWinSize", (void**)&DAQmxSetAIAveragingWinSize, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeBalanceCoarsePot", (void**)&DAQmxSetAIBridgeBalanceCoarsePot, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeBalanceFinePot", (void**)&DAQmxSetAIBridgeBalanceFinePot, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeCfg", (void**)&DAQmxSetAIBridgeCfg, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeElectricalUnits", (void**)&DAQmxSetAIBridgeElectricalUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeInitialRatio", (void**)&DAQmxSetAIBridgeInitialRatio, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeInitialVoltage", (void**)&DAQmxSetAIBridgeInitialVoltage, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeNomResistance", (void**)&DAQmxSetAIBridgeNomResistance, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgePhysicalUnits", (void**)&DAQmxSetAIBridgePhysicalUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgePolyForwardCoeff", (void**)&DAQmxSetAIBridgePolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgePolyReverseCoeff", (void**)&DAQmxSetAIBridgePolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeScaleType", (void**)&DAQmxSetAIBridgeScaleType, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalEnable", (void**)&DAQmxSetAIBridgeShuntCalEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalGainAdjust", (void**)&DAQmxSetAIBridgeShuntCalGainAdjust, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalSelect", (void**)&DAQmxSetAIBridgeShuntCalSelect, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalShuntCalAActualResistance", (void**)&DAQmxSetAIBridgeShuntCalShuntCalAActualResistance, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalShuntCalAResistance", (void**)&DAQmxSetAIBridgeShuntCalShuntCalAResistance, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalShuntCalASrc", (void**)&DAQmxSetAIBridgeShuntCalShuntCalASrc, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalShuntCalBActualResistance", (void**)&DAQmxSetAIBridgeShuntCalShuntCalBActualResistance, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeShuntCalShuntCalBResistance", (void**)&DAQmxSetAIBridgeShuntCalShuntCalBResistance, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeTableElectricalVals", (void**)&DAQmxSetAIBridgeTableElectricalVals, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeTablePhysicalVals", (void**)&DAQmxSetAIBridgeTablePhysicalVals, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeTwoPointLinFirstElectricalVal", (void**)&DAQmxSetAIBridgeTwoPointLinFirstElectricalVal, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeTwoPointLinFirstPhysicalVal", (void**)&DAQmxSetAIBridgeTwoPointLinFirstPhysicalVal, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeTwoPointLinSecondElectricalVal", (void**)&DAQmxSetAIBridgeTwoPointLinSecondElectricalVal, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeTwoPointLinSecondPhysicalVal", (void**)&DAQmxSetAIBridgeTwoPointLinSecondPhysicalVal, Dylib::Import::cMangled },
  { "DAQmxSetAIBridgeUnits", (void**)&DAQmxSetAIBridgeUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalApplyCalIfExp", (void**)&DAQmxSetAIChanCalApplyCalIfExp, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalCalDate", (void**)&DAQmxSetAIChanCalCalDate, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalDesc", (void**)&DAQmxSetAIChanCalDesc, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalEnableCal", (void**)&DAQmxSetAIChanCalEnableCal, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalExpDate", (void**)&DAQmxSetAIChanCalExpDate, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalOperatorName", (void**)&DAQmxSetAIChanCalOperatorName, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalPolyForwardCoeff", (void**)&DAQmxSetAIChanCalPolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalPolyReverseCoeff", (void**)&DAQmxSetAIChanCalPolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalScaleType", (void**)&DAQmxSetAIChanCalScaleType, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalTablePreScaledVals", (void**)&DAQmxSetAIChanCalTablePreScaledVals, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalTableScaledVals", (void**)&DAQmxSetAIChanCalTableScaledVals, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalVerifAcqVals", (void**)&DAQmxSetAIChanCalVerifAcqVals, Dylib::Import::cMangled },
  { "DAQmxSetAIChanCalVerifRefVals", (void**)&DAQmxSetAIChanCalVerifRefVals, Dylib::Import::cMangled },
  { "DAQmxSetAIChargeUnits", (void**)&DAQmxSetAIChargeUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIChopEnable", (void**)&DAQmxSetAIChopEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIConvActiveEdge", (void**)&DAQmxSetAIConvActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetAIConvActiveEdgeEx", (void**)&DAQmxSetAIConvActiveEdgeEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrEnable", (void**)&DAQmxSetAIConvDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrEnableEx", (void**)&DAQmxSetAIConvDigFltrEnableEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrMinPulseWidth", (void**)&DAQmxSetAIConvDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrMinPulseWidthEx", (void**)&DAQmxSetAIConvDigFltrMinPulseWidthEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrTimebaseRate", (void**)&DAQmxSetAIConvDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrTimebaseRateEx", (void**)&DAQmxSetAIConvDigFltrTimebaseRateEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrTimebaseSrc", (void**)&DAQmxSetAIConvDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigFltrTimebaseSrcEx", (void**)&DAQmxSetAIConvDigFltrTimebaseSrcEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigSyncEnable", (void**)&DAQmxSetAIConvDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIConvDigSyncEnableEx", (void**)&DAQmxSetAIConvDigSyncEnableEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvRate", (void**)&DAQmxSetAIConvRate, Dylib::Import::cMangled },
  { "DAQmxSetAIConvRateEx", (void**)&DAQmxSetAIConvRateEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvSrc", (void**)&DAQmxSetAIConvSrc, Dylib::Import::cMangled },
  { "DAQmxSetAIConvSrcEx", (void**)&DAQmxSetAIConvSrcEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvTimebaseDiv", (void**)&DAQmxSetAIConvTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxSetAIConvTimebaseDivEx", (void**)&DAQmxSetAIConvTimebaseDivEx, Dylib::Import::cMangled },
  { "DAQmxSetAIConvTimebaseSrc", (void**)&DAQmxSetAIConvTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAIConvTimebaseSrcEx", (void**)&DAQmxSetAIConvTimebaseSrcEx, Dylib::Import::cMangled },
  { "DAQmxSetAICoupling", (void**)&DAQmxSetAICoupling, Dylib::Import::cMangled },
  { "DAQmxSetAICurrentACRMSUnits", (void**)&DAQmxSetAICurrentACRMSUnits, Dylib::Import::cMangled },
  { "DAQmxSetAICurrentShuntLoc", (void**)&DAQmxSetAICurrentShuntLoc, Dylib::Import::cMangled },
  { "DAQmxSetAICurrentShuntResistance", (void**)&DAQmxSetAICurrentShuntResistance, Dylib::Import::cMangled },
  { "DAQmxSetAICurrentUnits", (void**)&DAQmxSetAICurrentUnits, Dylib::Import::cMangled },
  { "DAQmxSetAICustomScaleName", (void**)&DAQmxSetAICustomScaleName, Dylib::Import::cMangled },
  { "DAQmxSetAIDCOffset", (void**)&DAQmxSetAIDCOffset, Dylib::Import::cMangled },
  { "DAQmxSetAIDataXferCustomThreshold", (void**)&DAQmxSetAIDataXferCustomThreshold, Dylib::Import::cMangled },
  { "DAQmxSetAIDataXferMaxRate", (void**)&DAQmxSetAIDataXferMaxRate, Dylib::Import::cMangled },
  { "DAQmxSetAIDataXferMech", (void**)&DAQmxSetAIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxSetAIDataXferReqCond", (void**)&DAQmxSetAIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrBandpassCenterFreq", (void**)&DAQmxSetAIDigFltrBandpassCenterFreq, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrBandpassWidth", (void**)&DAQmxSetAIDigFltrBandpassWidth, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrCoeff", (void**)&DAQmxSetAIDigFltrCoeff, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrEnable", (void**)&DAQmxSetAIDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrHighpassCutoffFreq", (void**)&DAQmxSetAIDigFltrHighpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrLowpassCutoffFreq", (void**)&DAQmxSetAIDigFltrLowpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrNotchCenterFreq", (void**)&DAQmxSetAIDigFltrNotchCenterFreq, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrNotchWidth", (void**)&DAQmxSetAIDigFltrNotchWidth, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrOrder", (void**)&DAQmxSetAIDigFltrOrder, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrResponse", (void**)&DAQmxSetAIDigFltrResponse, Dylib::Import::cMangled },
  { "DAQmxSetAIDigFltrType", (void**)&DAQmxSetAIDigFltrType, Dylib::Import::cMangled },
  { "DAQmxSetAIDitherEnable", (void**)&DAQmxSetAIDitherEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIEddyCurrentProxProbeSensitivity", (void**)&DAQmxSetAIEddyCurrentProxProbeSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIEddyCurrentProxProbeSensitivityUnits", (void**)&DAQmxSetAIEddyCurrentProxProbeSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIEddyCurrentProxProbeUnits", (void**)&DAQmxSetAIEddyCurrentProxProbeUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIEnhancedAliasRejectionEnable", (void**)&DAQmxSetAIEnhancedAliasRejectionEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitActualVal", (void**)&DAQmxSetAIExcitActualVal, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitDCorAC", (void**)&DAQmxSetAIExcitDCorAC, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitIdleOutputBehavior", (void**)&DAQmxSetAIExcitIdleOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitSense", (void**)&DAQmxSetAIExcitSense, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitSrc", (void**)&DAQmxSetAIExcitSrc, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitUseForScaling", (void**)&DAQmxSetAIExcitUseForScaling, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitUseMultiplexed", (void**)&DAQmxSetAIExcitUseMultiplexed, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitVal", (void**)&DAQmxSetAIExcitVal, Dylib::Import::cMangled },
  { "DAQmxSetAIExcitVoltageOrCurrent", (void**)&DAQmxSetAIExcitVoltageOrCurrent, Dylib::Import::cMangled },
  { "DAQmxSetAIFilterDelayAdjustment", (void**)&DAQmxSetAIFilterDelayAdjustment, Dylib::Import::cMangled },
  { "DAQmxSetAIFilterDelayUnits", (void**)&DAQmxSetAIFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIFilterEnable", (void**)&DAQmxSetAIFilterEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIFilterFreq", (void**)&DAQmxSetAIFilterFreq, Dylib::Import::cMangled },
  { "DAQmxSetAIFilterOrder", (void**)&DAQmxSetAIFilterOrder, Dylib::Import::cMangled },
  { "DAQmxSetAIFilterResponse", (void**)&DAQmxSetAIFilterResponse, Dylib::Import::cMangled },
  { "DAQmxSetAIForceIEPESensorSensitivity", (void**)&DAQmxSetAIForceIEPESensorSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIForceIEPESensorSensitivityUnits", (void**)&DAQmxSetAIForceIEPESensorSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIForceReadFromChan", (void**)&DAQmxSetAIForceReadFromChan, Dylib::Import::cMangled },
  { "DAQmxSetAIForceUnits", (void**)&DAQmxSetAIForceUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIFreqHyst", (void**)&DAQmxSetAIFreqHyst, Dylib::Import::cMangled },
  { "DAQmxSetAIFreqThreshVoltage", (void**)&DAQmxSetAIFreqThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetAIFreqUnits", (void**)&DAQmxSetAIFreqUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIGain", (void**)&DAQmxSetAIGain, Dylib::Import::cMangled },
  { "DAQmxSetAIImpedance", (void**)&DAQmxSetAIImpedance, Dylib::Import::cMangled },
  { "DAQmxSetAIInputLimitsFaultDetectEnable", (void**)&DAQmxSetAIInputLimitsFaultDetectEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIInputLimitsFaultDetectLowerLimit", (void**)&DAQmxSetAIInputLimitsFaultDetectLowerLimit, Dylib::Import::cMangled },
  { "DAQmxSetAIInputLimitsFaultDetectUpperLimit", (void**)&DAQmxSetAIInputLimitsFaultDetectUpperLimit, Dylib::Import::cMangled },
  { "DAQmxSetAIInputSrc", (void**)&DAQmxSetAIInputSrc, Dylib::Import::cMangled },
  { "DAQmxSetAILVDTSensitivity", (void**)&DAQmxSetAILVDTSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAILVDTSensitivityUnits", (void**)&DAQmxSetAILVDTSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAILVDTUnits", (void**)&DAQmxSetAILVDTUnits, Dylib::Import::cMangled },
  { "DAQmxSetAILeadWireResistance", (void**)&DAQmxSetAILeadWireResistance, Dylib::Import::cMangled },
  { "DAQmxSetAILossyLSBRemovalCompressedSampSize", (void**)&DAQmxSetAILossyLSBRemovalCompressedSampSize, Dylib::Import::cMangled },
  { "DAQmxSetAILowpassCutoffFreq", (void**)&DAQmxSetAILowpassCutoffFreq, Dylib::Import::cMangled },
  { "DAQmxSetAILowpassEnable", (void**)&DAQmxSetAILowpassEnable, Dylib::Import::cMangled },
  { "DAQmxSetAILowpassSwitchCapClkSrc", (void**)&DAQmxSetAILowpassSwitchCapClkSrc, Dylib::Import::cMangled },
  { "DAQmxSetAILowpassSwitchCapExtClkDiv", (void**)&DAQmxSetAILowpassSwitchCapExtClkDiv, Dylib::Import::cMangled },
  { "DAQmxSetAILowpassSwitchCapExtClkFreq", (void**)&DAQmxSetAILowpassSwitchCapExtClkFreq, Dylib::Import::cMangled },
  { "DAQmxSetAILowpassSwitchCapOutClkDiv", (void**)&DAQmxSetAILowpassSwitchCapOutClkDiv, Dylib::Import::cMangled },
  { "DAQmxSetAIMax", (void**)&DAQmxSetAIMax, Dylib::Import::cMangled },
  { "DAQmxSetAIMemMapEnable", (void**)&DAQmxSetAIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIMicrophoneSensitivity", (void**)&DAQmxSetAIMicrophoneSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIMin", (void**)&DAQmxSetAIMin, Dylib::Import::cMangled },
  { "DAQmxSetAIOpenChanDetectEnable", (void**)&DAQmxSetAIOpenChanDetectEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIOpenThrmcplDetectEnable", (void**)&DAQmxSetAIOpenThrmcplDetectEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIOvercurrentDetectEnable", (void**)&DAQmxSetAIOvercurrentDetectEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIPowerSupplyFaultDetectEnable", (void**)&DAQmxSetAIPowerSupplyFaultDetectEnable, Dylib::Import::cMangled },
  { "DAQmxSetAIPressureUnits", (void**)&DAQmxSetAIPressureUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIProbeAtten", (void**)&DAQmxSetAIProbeAtten, Dylib::Import::cMangled },
  { "DAQmxSetAIRTDA", (void**)&DAQmxSetAIRTDA, Dylib::Import::cMangled },
  { "DAQmxSetAIRTDB", (void**)&DAQmxSetAIRTDB, Dylib::Import::cMangled },
  { "DAQmxSetAIRTDC", (void**)&DAQmxSetAIRTDC, Dylib::Import::cMangled },
  { "DAQmxSetAIRTDR0", (void**)&DAQmxSetAIRTDR0, Dylib::Import::cMangled },
  { "DAQmxSetAIRTDType", (void**)&DAQmxSetAIRTDType, Dylib::Import::cMangled },
  { "DAQmxSetAIRVDTSensitivity", (void**)&DAQmxSetAIRVDTSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIRVDTSensitivityUnits", (void**)&DAQmxSetAIRVDTSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIRVDTUnits", (void**)&DAQmxSetAIRVDTUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIRawDataCompressionType", (void**)&DAQmxSetAIRawDataCompressionType, Dylib::Import::cMangled },
  { "DAQmxSetAIRemoveFilterDelay", (void**)&DAQmxSetAIRemoveFilterDelay, Dylib::Import::cMangled },
  { "DAQmxSetAIResistanceCfg", (void**)&DAQmxSetAIResistanceCfg, Dylib::Import::cMangled },
  { "DAQmxSetAIResistanceUnits", (void**)&DAQmxSetAIResistanceUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIRngHigh", (void**)&DAQmxSetAIRngHigh, Dylib::Import::cMangled },
  { "DAQmxSetAIRngLow", (void**)&DAQmxSetAIRngLow, Dylib::Import::cMangled },
  { "DAQmxSetAIRosetteStrainGageOrientation", (void**)&DAQmxSetAIRosetteStrainGageOrientation, Dylib::Import::cMangled },
  { "DAQmxSetAIRosetteStrainGageRosetteMeasType", (void**)&DAQmxSetAIRosetteStrainGageRosetteMeasType, Dylib::Import::cMangled },
  { "DAQmxSetAISampAndHoldEnable", (void**)&DAQmxSetAISampAndHoldEnable, Dylib::Import::cMangled },
  { "DAQmxSetAISensorPowerCfg", (void**)&DAQmxSetAISensorPowerCfg, Dylib::Import::cMangled },
  { "DAQmxSetAISensorPowerType", (void**)&DAQmxSetAISensorPowerType, Dylib::Import::cMangled },
  { "DAQmxSetAISensorPowerVoltage", (void**)&DAQmxSetAISensorPowerVoltage, Dylib::Import::cMangled },
  { "DAQmxSetAISoundPressureMaxSoundPressureLvl", (void**)&DAQmxSetAISoundPressureMaxSoundPressureLvl, Dylib::Import::cMangled },
  { "DAQmxSetAISoundPressureUnits", (void**)&DAQmxSetAISoundPressureUnits, Dylib::Import::cMangled },
  { "DAQmxSetAISoundPressuredBRef", (void**)&DAQmxSetAISoundPressuredBRef, Dylib::Import::cMangled },
  { "DAQmxSetAIStrainGageCfg", (void**)&DAQmxSetAIStrainGageCfg, Dylib::Import::cMangled },
  { "DAQmxSetAIStrainGageForceReadFromChan", (void**)&DAQmxSetAIStrainGageForceReadFromChan, Dylib::Import::cMangled },
  { "DAQmxSetAIStrainGageGageFactor", (void**)&DAQmxSetAIStrainGageGageFactor, Dylib::Import::cMangled },
  { "DAQmxSetAIStrainGagePoissonRatio", (void**)&DAQmxSetAIStrainGagePoissonRatio, Dylib::Import::cMangled },
  { "DAQmxSetAIStrainUnits", (void**)&DAQmxSetAIStrainUnits, Dylib::Import::cMangled },
  { "DAQmxSetAITempUnits", (void**)&DAQmxSetAITempUnits, Dylib::Import::cMangled },
  { "DAQmxSetAITermCfg", (void**)&DAQmxSetAITermCfg, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmcplCJCVal", (void**)&DAQmxSetAIThrmcplCJCVal, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmcplLeadOffsetVoltage", (void**)&DAQmxSetAIThrmcplLeadOffsetVoltage, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmcplScaleType", (void**)&DAQmxSetAIThrmcplScaleType, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmcplType", (void**)&DAQmxSetAIThrmcplType, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmstrA", (void**)&DAQmxSetAIThrmstrA, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmstrB", (void**)&DAQmxSetAIThrmstrB, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmstrC", (void**)&DAQmxSetAIThrmstrC, Dylib::Import::cMangled },
  { "DAQmxSetAIThrmstrR1", (void**)&DAQmxSetAIThrmstrR1, Dylib::Import::cMangled },
  { "DAQmxSetAITorqueUnits", (void**)&DAQmxSetAITorqueUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIUsbXferReqCount", (void**)&DAQmxSetAIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxSetAIUsbXferReqSize", (void**)&DAQmxSetAIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxSetAIVelocityIEPESensorSensitivity", (void**)&DAQmxSetAIVelocityIEPESensorSensitivity, Dylib::Import::cMangled },
  { "DAQmxSetAIVelocityIEPESensorSensitivityUnits", (void**)&DAQmxSetAIVelocityIEPESensorSensitivityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIVelocityIEPESensordBRef", (void**)&DAQmxSetAIVelocityIEPESensordBRef, Dylib::Import::cMangled },
  { "DAQmxSetAIVelocityUnits", (void**)&DAQmxSetAIVelocityUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIVoltageACRMSUnits", (void**)&DAQmxSetAIVoltageACRMSUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIVoltageUnits", (void**)&DAQmxSetAIVoltageUnits, Dylib::Import::cMangled },
  { "DAQmxSetAIVoltagedBRef", (void**)&DAQmxSetAIVoltagedBRef, Dylib::Import::cMangled },
  { "DAQmxSetAOCommonModeOffset", (void**)&DAQmxSetAOCommonModeOffset, Dylib::Import::cMangled },
  { "DAQmxSetAOCurrentUnits", (void**)&DAQmxSetAOCurrentUnits, Dylib::Import::cMangled },
  { "DAQmxSetAOCustomScaleName", (void**)&DAQmxSetAOCustomScaleName, Dylib::Import::cMangled },
  { "DAQmxSetAODACOffsetExtSrc", (void**)&DAQmxSetAODACOffsetExtSrc, Dylib::Import::cMangled },
  { "DAQmxSetAODACOffsetSrc", (void**)&DAQmxSetAODACOffsetSrc, Dylib::Import::cMangled },
  { "DAQmxSetAODACOffsetVal", (void**)&DAQmxSetAODACOffsetVal, Dylib::Import::cMangled },
  { "DAQmxSetAODACRefAllowConnToGnd", (void**)&DAQmxSetAODACRefAllowConnToGnd, Dylib::Import::cMangled },
  { "DAQmxSetAODACRefConnToGnd", (void**)&DAQmxSetAODACRefConnToGnd, Dylib::Import::cMangled },
  { "DAQmxSetAODACRefExtSrc", (void**)&DAQmxSetAODACRefExtSrc, Dylib::Import::cMangled },
  { "DAQmxSetAODACRefSrc", (void**)&DAQmxSetAODACRefSrc, Dylib::Import::cMangled },
  { "DAQmxSetAODACRefVal", (void**)&DAQmxSetAODACRefVal, Dylib::Import::cMangled },
  { "DAQmxSetAODACRngHigh", (void**)&DAQmxSetAODACRngHigh, Dylib::Import::cMangled },
  { "DAQmxSetAODACRngLow", (void**)&DAQmxSetAODACRngLow, Dylib::Import::cMangled },
  { "DAQmxSetAODataXferMech", (void**)&DAQmxSetAODataXferMech, Dylib::Import::cMangled },
  { "DAQmxSetAODataXferReqCond", (void**)&DAQmxSetAODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxSetAOEnhancedImageRejectionEnable", (void**)&DAQmxSetAOEnhancedImageRejectionEnable, Dylib::Import::cMangled },
  { "DAQmxSetAOFilterDelay", (void**)&DAQmxSetAOFilterDelay, Dylib::Import::cMangled },
  { "DAQmxSetAOFilterDelayAdjustment", (void**)&DAQmxSetAOFilterDelayAdjustment, Dylib::Import::cMangled },
  { "DAQmxSetAOFilterDelayUnits", (void**)&DAQmxSetAOFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenAmplitude", (void**)&DAQmxSetAOFuncGenAmplitude, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenFMDeviation", (void**)&DAQmxSetAOFuncGenFMDeviation, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenFreq", (void**)&DAQmxSetAOFuncGenFreq, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenModulationType", (void**)&DAQmxSetAOFuncGenModulationType, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenOffset", (void**)&DAQmxSetAOFuncGenOffset, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenSquareDutyCycle", (void**)&DAQmxSetAOFuncGenSquareDutyCycle, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenStartPhase", (void**)&DAQmxSetAOFuncGenStartPhase, Dylib::Import::cMangled },
  { "DAQmxSetAOFuncGenType", (void**)&DAQmxSetAOFuncGenType, Dylib::Import::cMangled },
  { "DAQmxSetAOGain", (void**)&DAQmxSetAOGain, Dylib::Import::cMangled },
  { "DAQmxSetAOIdleOutputBehavior", (void**)&DAQmxSetAOIdleOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxSetAOLoadImpedance", (void**)&DAQmxSetAOLoadImpedance, Dylib::Import::cMangled },
  { "DAQmxSetAOMax", (void**)&DAQmxSetAOMax, Dylib::Import::cMangled },
  { "DAQmxSetAOMemMapEnable", (void**)&DAQmxSetAOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxSetAOMin", (void**)&DAQmxSetAOMin, Dylib::Import::cMangled },
  { "DAQmxSetAOOutputImpedance", (void**)&DAQmxSetAOOutputImpedance, Dylib::Import::cMangled },
  { "DAQmxSetAOPowerAmpChannelEnable", (void**)&DAQmxSetAOPowerAmpChannelEnable, Dylib::Import::cMangled },
  { "DAQmxSetAOReglitchEnable", (void**)&DAQmxSetAOReglitchEnable, Dylib::Import::cMangled },
  { "DAQmxSetAOResolutionUnits", (void**)&DAQmxSetAOResolutionUnits, Dylib::Import::cMangled },
  { "DAQmxSetAOTermCfg", (void**)&DAQmxSetAOTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetAOUsbXferReqCount", (void**)&DAQmxSetAOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxSetAOUsbXferReqSize", (void**)&DAQmxSetAOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxSetAOUseOnlyOnBrdMem", (void**)&DAQmxSetAOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxSetAOVoltageCurrentLimit", (void**)&DAQmxSetAOVoltageCurrentLimit, Dylib::Import::cMangled },
  { "DAQmxSetAOVoltageUnits", (void**)&DAQmxSetAOVoltageUnits, Dylib::Import::cMangled },
  { "DAQmxSetAdvTrigType", (void**)&DAQmxSetAdvTrigType, Dylib::Import::cMangled },
  { "DAQmxSetAnalogPowerUpStates", (void**)&DAQmxSetAnalogPowerUpStates, Dylib::Import::cMangled },
  { "DAQmxSetAnalogPowerUpStatesWithOutputType", (void**)&DAQmxSetAnalogPowerUpStatesWithOutputType, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigCoupling", (void**)&DAQmxSetAnlgEdgeRefTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigDigFltrEnable", (void**)&DAQmxSetAnlgEdgeRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigDigFltrMinPulseWidth", (void**)&DAQmxSetAnlgEdgeRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseRate", (void**)&DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseSrc", (void**)&DAQmxSetAnlgEdgeRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigDigSyncEnable", (void**)&DAQmxSetAnlgEdgeRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigHyst", (void**)&DAQmxSetAnlgEdgeRefTrigHyst, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigLvl", (void**)&DAQmxSetAnlgEdgeRefTrigLvl, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigSlope", (void**)&DAQmxSetAnlgEdgeRefTrigSlope, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeRefTrigSrc", (void**)&DAQmxSetAnlgEdgeRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigCoupling", (void**)&DAQmxSetAnlgEdgeStartTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigDigFltrEnable", (void**)&DAQmxSetAnlgEdgeStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigDigFltrMinPulseWidth", (void**)&DAQmxSetAnlgEdgeStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseRate", (void**)&DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseSrc", (void**)&DAQmxSetAnlgEdgeStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigDigSyncEnable", (void**)&DAQmxSetAnlgEdgeStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigHyst", (void**)&DAQmxSetAnlgEdgeStartTrigHyst, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigLvl", (void**)&DAQmxSetAnlgEdgeStartTrigLvl, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigSlope", (void**)&DAQmxSetAnlgEdgeStartTrigSlope, Dylib::Import::cMangled },
  { "DAQmxSetAnlgEdgeStartTrigSrc", (void**)&DAQmxSetAnlgEdgeStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigCoupling", (void**)&DAQmxSetAnlgLvlPauseTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigDigFltrEnable", (void**)&DAQmxSetAnlgLvlPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxSetAnlgLvlPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseRate", (void**)&DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxSetAnlgLvlPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigDigSyncEnable", (void**)&DAQmxSetAnlgLvlPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigHyst", (void**)&DAQmxSetAnlgLvlPauseTrigHyst, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigLvl", (void**)&DAQmxSetAnlgLvlPauseTrigLvl, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigSrc", (void**)&DAQmxSetAnlgLvlPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgLvlPauseTrigWhen", (void**)&DAQmxSetAnlgLvlPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeRefTrigCouplings", (void**)&DAQmxSetAnlgMultiEdgeRefTrigCouplings, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeRefTrigHysts", (void**)&DAQmxSetAnlgMultiEdgeRefTrigHysts, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeRefTrigLvls", (void**)&DAQmxSetAnlgMultiEdgeRefTrigLvls, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeRefTrigSlopes", (void**)&DAQmxSetAnlgMultiEdgeRefTrigSlopes, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeRefTrigSrcs", (void**)&DAQmxSetAnlgMultiEdgeRefTrigSrcs, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeStartTrigCouplings", (void**)&DAQmxSetAnlgMultiEdgeStartTrigCouplings, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeStartTrigHysts", (void**)&DAQmxSetAnlgMultiEdgeStartTrigHysts, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeStartTrigLvls", (void**)&DAQmxSetAnlgMultiEdgeStartTrigLvls, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeStartTrigSlopes", (void**)&DAQmxSetAnlgMultiEdgeStartTrigSlopes, Dylib::Import::cMangled },
  { "DAQmxSetAnlgMultiEdgeStartTrigSrcs", (void**)&DAQmxSetAnlgMultiEdgeStartTrigSrcs, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigBtm", (void**)&DAQmxSetAnlgWinPauseTrigBtm, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigCoupling", (void**)&DAQmxSetAnlgWinPauseTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigDigFltrEnable", (void**)&DAQmxSetAnlgWinPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxSetAnlgWinPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigDigFltrTimebaseRate", (void**)&DAQmxSetAnlgWinPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxSetAnlgWinPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigDigSyncEnable", (void**)&DAQmxSetAnlgWinPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigSrc", (void**)&DAQmxSetAnlgWinPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigTop", (void**)&DAQmxSetAnlgWinPauseTrigTop, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinPauseTrigWhen", (void**)&DAQmxSetAnlgWinPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigBtm", (void**)&DAQmxSetAnlgWinRefTrigBtm, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigCoupling", (void**)&DAQmxSetAnlgWinRefTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigDigFltrEnable", (void**)&DAQmxSetAnlgWinRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigDigFltrMinPulseWidth", (void**)&DAQmxSetAnlgWinRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigDigFltrTimebaseRate", (void**)&DAQmxSetAnlgWinRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigDigFltrTimebaseSrc", (void**)&DAQmxSetAnlgWinRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigDigSyncEnable", (void**)&DAQmxSetAnlgWinRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigSrc", (void**)&DAQmxSetAnlgWinRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigTop", (void**)&DAQmxSetAnlgWinRefTrigTop, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinRefTrigWhen", (void**)&DAQmxSetAnlgWinRefTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigBtm", (void**)&DAQmxSetAnlgWinStartTrigBtm, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigCoupling", (void**)&DAQmxSetAnlgWinStartTrigCoupling, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigDigFltrEnable", (void**)&DAQmxSetAnlgWinStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigDigFltrMinPulseWidth", (void**)&DAQmxSetAnlgWinStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigDigFltrTimebaseRate", (void**)&DAQmxSetAnlgWinStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigDigFltrTimebaseSrc", (void**)&DAQmxSetAnlgWinStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigDigSyncEnable", (void**)&DAQmxSetAnlgWinStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigSrc", (void**)&DAQmxSetAnlgWinStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigTop", (void**)&DAQmxSetAnlgWinStartTrigTop, Dylib::Import::cMangled },
  { "DAQmxSetAnlgWinStartTrigWhen", (void**)&DAQmxSetAnlgWinStartTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetArmStartTrigTimescale", (void**)&DAQmxSetArmStartTrigTimescale, Dylib::Import::cMangled },
  { "DAQmxSetArmStartTrigTimestampEnable", (void**)&DAQmxSetArmStartTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxSetArmStartTrigTimestampTimescale", (void**)&DAQmxSetArmStartTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxSetArmStartTrigTrigWhen", (void**)&DAQmxSetArmStartTrigTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetArmStartTrigType", (void**)&DAQmxSetArmStartTrigType, Dylib::Import::cMangled },
  { "DAQmxSetBufInputBufSize", (void**)&DAQmxSetBufInputBufSize, Dylib::Import::cMangled },
  { "DAQmxSetBufOutputBufSize", (void**)&DAQmxSetBufOutputBufSize, Dylib::Import::cMangled },
  { "DAQmxSetBufOutputOnbrdBufSize", (void**)&DAQmxSetBufOutputOnbrdBufSize, Dylib::Import::cMangled },
  { "DAQmxSetBufferAttribute", (void**)&DAQmxSetBufferAttribute, Dylib::Import::cMangled },
  { "DAQmxSetCIAngEncoderInitialAngle", (void**)&DAQmxSetCIAngEncoderInitialAngle, Dylib::Import::cMangled },
  { "DAQmxSetCIAngEncoderPulsesPerRev", (void**)&DAQmxSetCIAngEncoderPulsesPerRev, Dylib::Import::cMangled },
  { "DAQmxSetCIAngEncoderUnits", (void**)&DAQmxSetCIAngEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesActiveEdge", (void**)&DAQmxSetCICountEdgesActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirDigFltrEnable", (void**)&DAQmxSetCICountEdgesCountDirDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirDigFltrMinPulseWidth", (void**)&DAQmxSetCICountEdgesCountDirDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirDigFltrTimebaseRate", (void**)&DAQmxSetCICountEdgesCountDirDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirDigFltrTimebaseSrc", (void**)&DAQmxSetCICountEdgesCountDirDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirDigSyncEnable", (void**)&DAQmxSetCICountEdgesCountDirDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirHyst", (void**)&DAQmxSetCICountEdgesCountDirHyst, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirLogicLvlBehavior", (void**)&DAQmxSetCICountEdgesCountDirLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirTermCfg", (void**)&DAQmxSetCICountEdgesCountDirTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountDirThreshVoltage", (void**)&DAQmxSetCICountEdgesCountDirThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetActiveEdge", (void**)&DAQmxSetCICountEdgesCountResetActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetDigFltrEnable", (void**)&DAQmxSetCICountEdgesCountResetDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetDigFltrMinPulseWidth", (void**)&DAQmxSetCICountEdgesCountResetDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetDigFltrTimebaseRate", (void**)&DAQmxSetCICountEdgesCountResetDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetDigFltrTimebaseSrc", (void**)&DAQmxSetCICountEdgesCountResetDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetDigSyncEnable", (void**)&DAQmxSetCICountEdgesCountResetDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetEnable", (void**)&DAQmxSetCICountEdgesCountResetEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetHyst", (void**)&DAQmxSetCICountEdgesCountResetHyst, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetLogicLvlBehavior", (void**)&DAQmxSetCICountEdgesCountResetLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetResetCount", (void**)&DAQmxSetCICountEdgesCountResetResetCount, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetTerm", (void**)&DAQmxSetCICountEdgesCountResetTerm, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetTermCfg", (void**)&DAQmxSetCICountEdgesCountResetTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesCountResetThreshVoltage", (void**)&DAQmxSetCICountEdgesCountResetThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDigFltrEnable", (void**)&DAQmxSetCICountEdgesDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDigFltrMinPulseWidth", (void**)&DAQmxSetCICountEdgesDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDigFltrTimebaseRate", (void**)&DAQmxSetCICountEdgesDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDigFltrTimebaseSrc", (void**)&DAQmxSetCICountEdgesDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDigSyncEnable", (void**)&DAQmxSetCICountEdgesDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDir", (void**)&DAQmxSetCICountEdgesDir, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesDirTerm", (void**)&DAQmxSetCICountEdgesDirTerm, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateDigFltrEnable", (void**)&DAQmxSetCICountEdgesGateDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateDigFltrMinPulseWidth", (void**)&DAQmxSetCICountEdgesGateDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateDigFltrTimebaseRate", (void**)&DAQmxSetCICountEdgesGateDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateDigFltrTimebaseSrc", (void**)&DAQmxSetCICountEdgesGateDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateEnable", (void**)&DAQmxSetCICountEdgesGateEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateHyst", (void**)&DAQmxSetCICountEdgesGateHyst, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateLogicLvlBehavior", (void**)&DAQmxSetCICountEdgesGateLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateTerm", (void**)&DAQmxSetCICountEdgesGateTerm, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateTermCfg", (void**)&DAQmxSetCICountEdgesGateTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateThreshVoltage", (void**)&DAQmxSetCICountEdgesGateThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesGateWhen", (void**)&DAQmxSetCICountEdgesGateWhen, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesHyst", (void**)&DAQmxSetCICountEdgesHyst, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesInitialCnt", (void**)&DAQmxSetCICountEdgesInitialCnt, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesLogicLvlBehavior", (void**)&DAQmxSetCICountEdgesLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesTerm", (void**)&DAQmxSetCICountEdgesTerm, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesTermCfg", (void**)&DAQmxSetCICountEdgesTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCICountEdgesThreshVoltage", (void**)&DAQmxSetCICountEdgesThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseActiveEdge", (void**)&DAQmxSetCICtrTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseDigFltrEnable", (void**)&DAQmxSetCICtrTimebaseDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseDigFltrMinPulseWidth", (void**)&DAQmxSetCICtrTimebaseDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseDigFltrTimebaseRate", (void**)&DAQmxSetCICtrTimebaseDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseDigFltrTimebaseSrc", (void**)&DAQmxSetCICtrTimebaseDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseDigSyncEnable", (void**)&DAQmxSetCICtrTimebaseDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseMasterTimebaseDiv", (void**)&DAQmxSetCICtrTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseRate", (void**)&DAQmxSetCICtrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCICtrTimebaseSrc", (void**)&DAQmxSetCICtrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCICustomScaleName", (void**)&DAQmxSetCICustomScaleName, Dylib::Import::cMangled },
  { "DAQmxSetCIDataXferMech", (void**)&DAQmxSetCIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxSetCIDataXferReqCond", (void**)&DAQmxSetCIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxSetCIDupCountPrevent", (void**)&DAQmxSetCIDupCountPrevent, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleDigFltrEnable", (void**)&DAQmxSetCIDutyCycleDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleDigFltrMinPulseWidth", (void**)&DAQmxSetCIDutyCycleDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleDigFltrTimebaseRate", (void**)&DAQmxSetCIDutyCycleDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleDigFltrTimebaseSrc", (void**)&DAQmxSetCIDutyCycleDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleLogicLvlBehavior", (void**)&DAQmxSetCIDutyCycleLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleStartingEdge", (void**)&DAQmxSetCIDutyCycleStartingEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleTerm", (void**)&DAQmxSetCIDutyCycleTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIDutyCycleTermCfg", (void**)&DAQmxSetCIDutyCycleTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputDigFltrEnable", (void**)&DAQmxSetCIEncoderAInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputDigFltrMinPulseWidth", (void**)&DAQmxSetCIEncoderAInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputDigFltrTimebaseRate", (void**)&DAQmxSetCIEncoderAInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputDigFltrTimebaseSrc", (void**)&DAQmxSetCIEncoderAInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputDigSyncEnable", (void**)&DAQmxSetCIEncoderAInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputLogicLvlBehavior", (void**)&DAQmxSetCIEncoderAInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputTerm", (void**)&DAQmxSetCIEncoderAInputTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderAInputTermCfg", (void**)&DAQmxSetCIEncoderAInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputDigFltrEnable", (void**)&DAQmxSetCIEncoderBInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputDigFltrMinPulseWidth", (void**)&DAQmxSetCIEncoderBInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputDigFltrTimebaseRate", (void**)&DAQmxSetCIEncoderBInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputDigFltrTimebaseSrc", (void**)&DAQmxSetCIEncoderBInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputDigSyncEnable", (void**)&DAQmxSetCIEncoderBInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputLogicLvlBehavior", (void**)&DAQmxSetCIEncoderBInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputTerm", (void**)&DAQmxSetCIEncoderBInputTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderBInputTermCfg", (void**)&DAQmxSetCIEncoderBInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderDecodingType", (void**)&DAQmxSetCIEncoderDecodingType, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZIndexEnable", (void**)&DAQmxSetCIEncoderZIndexEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZIndexPhase", (void**)&DAQmxSetCIEncoderZIndexPhase, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZIndexVal", (void**)&DAQmxSetCIEncoderZIndexVal, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputDigFltrEnable", (void**)&DAQmxSetCIEncoderZInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputDigFltrMinPulseWidth", (void**)&DAQmxSetCIEncoderZInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputDigFltrTimebaseRate", (void**)&DAQmxSetCIEncoderZInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputDigFltrTimebaseSrc", (void**)&DAQmxSetCIEncoderZInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputDigSyncEnable", (void**)&DAQmxSetCIEncoderZInputDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputLogicLvlBehavior", (void**)&DAQmxSetCIEncoderZInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputTerm", (void**)&DAQmxSetCIEncoderZInputTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIEncoderZInputTermCfg", (void**)&DAQmxSetCIEncoderZInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIFilterDelayUnits", (void**)&DAQmxSetCIFilterDelayUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIFilterEnable", (void**)&DAQmxSetCIFilterEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIFilterFreq", (void**)&DAQmxSetCIFilterFreq, Dylib::Import::cMangled },
  { "DAQmxSetCIFilterOrder", (void**)&DAQmxSetCIFilterOrder, Dylib::Import::cMangled },
  { "DAQmxSetCIFilterResponse", (void**)&DAQmxSetCIFilterResponse, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqDigFltrEnable", (void**)&DAQmxSetCIFreqDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqDigFltrMinPulseWidth", (void**)&DAQmxSetCIFreqDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqDigFltrTimebaseRate", (void**)&DAQmxSetCIFreqDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqDigFltrTimebaseSrc", (void**)&DAQmxSetCIFreqDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqDigSyncEnable", (void**)&DAQmxSetCIFreqDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqDiv", (void**)&DAQmxSetCIFreqDiv, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqEnableAveraging", (void**)&DAQmxSetCIFreqEnableAveraging, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqHyst", (void**)&DAQmxSetCIFreqHyst, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqLogicLvlBehavior", (void**)&DAQmxSetCIFreqLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqMeasMeth", (void**)&DAQmxSetCIFreqMeasMeth, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqMeasTime", (void**)&DAQmxSetCIFreqMeasTime, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqStartingEdge", (void**)&DAQmxSetCIFreqStartingEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqTerm", (void**)&DAQmxSetCIFreqTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqTermCfg", (void**)&DAQmxSetCIFreqTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqThreshVoltage", (void**)&DAQmxSetCIFreqThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCIFreqUnits", (void**)&DAQmxSetCIFreqUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIGPSSyncMethod", (void**)&DAQmxSetCIGPSSyncMethod, Dylib::Import::cMangled },
  { "DAQmxSetCIGPSSyncSrc", (void**)&DAQmxSetCIGPSSyncSrc, Dylib::Import::cMangled },
  { "DAQmxSetCILinEncoderDistPerPulse", (void**)&DAQmxSetCILinEncoderDistPerPulse, Dylib::Import::cMangled },
  { "DAQmxSetCILinEncoderInitialPos", (void**)&DAQmxSetCILinEncoderInitialPos, Dylib::Import::cMangled },
  { "DAQmxSetCILinEncoderUnits", (void**)&DAQmxSetCILinEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIMax", (void**)&DAQmxSetCIMax, Dylib::Import::cMangled },
  { "DAQmxSetCIMaxMeasPeriod", (void**)&DAQmxSetCIMaxMeasPeriod, Dylib::Import::cMangled },
  { "DAQmxSetCIMemMapEnable", (void**)&DAQmxSetCIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIMin", (void**)&DAQmxSetCIMin, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodDigFltrEnable", (void**)&DAQmxSetCIPeriodDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodDigFltrMinPulseWidth", (void**)&DAQmxSetCIPeriodDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodDigFltrTimebaseRate", (void**)&DAQmxSetCIPeriodDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodDigFltrTimebaseSrc", (void**)&DAQmxSetCIPeriodDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodDigSyncEnable", (void**)&DAQmxSetCIPeriodDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodDiv", (void**)&DAQmxSetCIPeriodDiv, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodEnableAveraging", (void**)&DAQmxSetCIPeriodEnableAveraging, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodHyst", (void**)&DAQmxSetCIPeriodHyst, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodLogicLvlBehavior", (void**)&DAQmxSetCIPeriodLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodMeasMeth", (void**)&DAQmxSetCIPeriodMeasMeth, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodMeasTime", (void**)&DAQmxSetCIPeriodMeasTime, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodStartingEdge", (void**)&DAQmxSetCIPeriodStartingEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodTerm", (void**)&DAQmxSetCIPeriodTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodTermCfg", (void**)&DAQmxSetCIPeriodTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodThreshVoltage", (void**)&DAQmxSetCIPeriodThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCIPeriodUnits", (void**)&DAQmxSetCIPeriodUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIPrescaler", (void**)&DAQmxSetCIPrescaler, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqDigFltrEnable", (void**)&DAQmxSetCIPulseFreqDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqDigFltrMinPulseWidth", (void**)&DAQmxSetCIPulseFreqDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqDigFltrTimebaseRate", (void**)&DAQmxSetCIPulseFreqDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqDigFltrTimebaseSrc", (void**)&DAQmxSetCIPulseFreqDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqDigSyncEnable", (void**)&DAQmxSetCIPulseFreqDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqLogicLvlBehavior", (void**)&DAQmxSetCIPulseFreqLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqStartEdge", (void**)&DAQmxSetCIPulseFreqStartEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqTerm", (void**)&DAQmxSetCIPulseFreqTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqTermCfg", (void**)&DAQmxSetCIPulseFreqTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseFreqUnits", (void**)&DAQmxSetCIPulseFreqUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksDigFltrEnable", (void**)&DAQmxSetCIPulseTicksDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksDigFltrMinPulseWidth", (void**)&DAQmxSetCIPulseTicksDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksDigFltrTimebaseRate", (void**)&DAQmxSetCIPulseTicksDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksDigFltrTimebaseSrc", (void**)&DAQmxSetCIPulseTicksDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksDigSyncEnable", (void**)&DAQmxSetCIPulseTicksDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksLogicLvlBehavior", (void**)&DAQmxSetCIPulseTicksLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksStartEdge", (void**)&DAQmxSetCIPulseTicksStartEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksTerm", (void**)&DAQmxSetCIPulseTicksTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTicksTermCfg", (void**)&DAQmxSetCIPulseTicksTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeDigFltrEnable", (void**)&DAQmxSetCIPulseTimeDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeDigFltrMinPulseWidth", (void**)&DAQmxSetCIPulseTimeDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeDigFltrTimebaseRate", (void**)&DAQmxSetCIPulseTimeDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeDigFltrTimebaseSrc", (void**)&DAQmxSetCIPulseTimeDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeDigSyncEnable", (void**)&DAQmxSetCIPulseTimeDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeLogicLvlBehavior", (void**)&DAQmxSetCIPulseTimeLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeStartEdge", (void**)&DAQmxSetCIPulseTimeStartEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeTerm", (void**)&DAQmxSetCIPulseTimeTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeTermCfg", (void**)&DAQmxSetCIPulseTimeTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseTimeUnits", (void**)&DAQmxSetCIPulseTimeUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthDigFltrEnable", (void**)&DAQmxSetCIPulseWidthDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthDigFltrMinPulseWidth", (void**)&DAQmxSetCIPulseWidthDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthDigFltrTimebaseRate", (void**)&DAQmxSetCIPulseWidthDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthDigFltrTimebaseSrc", (void**)&DAQmxSetCIPulseWidthDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthDigSyncEnable", (void**)&DAQmxSetCIPulseWidthDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthLogicLvlBehavior", (void**)&DAQmxSetCIPulseWidthLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthStartingEdge", (void**)&DAQmxSetCIPulseWidthStartingEdge, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthTerm", (void**)&DAQmxSetCIPulseWidthTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthTermCfg", (void**)&DAQmxSetCIPulseWidthTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIPulseWidthUnits", (void**)&DAQmxSetCIPulseWidthUnits, Dylib::Import::cMangled },
  { "DAQmxSetCISampClkOverrunBehavior", (void**)&DAQmxSetCISampClkOverrunBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCISampClkOverrunSentinelVal", (void**)&DAQmxSetCISampClkOverrunSentinelVal, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodDigFltrEnable", (void**)&DAQmxSetCISemiPeriodDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodDigFltrMinPulseWidth", (void**)&DAQmxSetCISemiPeriodDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodDigFltrTimebaseRate", (void**)&DAQmxSetCISemiPeriodDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodDigFltrTimebaseSrc", (void**)&DAQmxSetCISemiPeriodDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodDigSyncEnable", (void**)&DAQmxSetCISemiPeriodDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodLogicLvlBehavior", (void**)&DAQmxSetCISemiPeriodLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodStartingEdge", (void**)&DAQmxSetCISemiPeriodStartingEdge, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodTerm", (void**)&DAQmxSetCISemiPeriodTerm, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodTermCfg", (void**)&DAQmxSetCISemiPeriodTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCISemiPeriodUnits", (void**)&DAQmxSetCISemiPeriodUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIThreshVoltage", (void**)&DAQmxSetCIThreshVoltage, Dylib::Import::cMangled },
  { "DAQmxSetCITimestampInitialSeconds", (void**)&DAQmxSetCITimestampInitialSeconds, Dylib::Import::cMangled },
  { "DAQmxSetCITimestampUnits", (void**)&DAQmxSetCITimestampUnits, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstDigFltrEnable", (void**)&DAQmxSetCITwoEdgeSepFirstDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstDigFltrMinPulseWidth", (void**)&DAQmxSetCITwoEdgeSepFirstDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseRate", (void**)&DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseSrc", (void**)&DAQmxSetCITwoEdgeSepFirstDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstDigSyncEnable", (void**)&DAQmxSetCITwoEdgeSepFirstDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstEdge", (void**)&DAQmxSetCITwoEdgeSepFirstEdge, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstLogicLvlBehavior", (void**)&DAQmxSetCITwoEdgeSepFirstLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstTerm", (void**)&DAQmxSetCITwoEdgeSepFirstTerm, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepFirstTermCfg", (void**)&DAQmxSetCITwoEdgeSepFirstTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondDigFltrEnable", (void**)&DAQmxSetCITwoEdgeSepSecondDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondDigFltrMinPulseWidth", (void**)&DAQmxSetCITwoEdgeSepSecondDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseRate", (void**)&DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseSrc", (void**)&DAQmxSetCITwoEdgeSepSecondDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondDigSyncEnable", (void**)&DAQmxSetCITwoEdgeSepSecondDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondEdge", (void**)&DAQmxSetCITwoEdgeSepSecondEdge, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondLogicLvlBehavior", (void**)&DAQmxSetCITwoEdgeSepSecondLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondTerm", (void**)&DAQmxSetCITwoEdgeSepSecondTerm, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepSecondTermCfg", (void**)&DAQmxSetCITwoEdgeSepSecondTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCITwoEdgeSepUnits", (void**)&DAQmxSetCITwoEdgeSepUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIUsbXferReqCount", (void**)&DAQmxSetCIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxSetCIUsbXferReqSize", (void**)&DAQmxSetCIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityAngEncoderPulsesPerRev", (void**)&DAQmxSetCIVelocityAngEncoderPulsesPerRev, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityAngEncoderUnits", (void**)&DAQmxSetCIVelocityAngEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityDiv", (void**)&DAQmxSetCIVelocityDiv, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputDigFltrEnable", (void**)&DAQmxSetCIVelocityEncoderAInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputDigFltrMinPulseWidth", (void**)&DAQmxSetCIVelocityEncoderAInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseRate", (void**)&DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseSrc", (void**)&DAQmxSetCIVelocityEncoderAInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputLogicLvlBehavior", (void**)&DAQmxSetCIVelocityEncoderAInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputTerm", (void**)&DAQmxSetCIVelocityEncoderAInputTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderAInputTermCfg", (void**)&DAQmxSetCIVelocityEncoderAInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputDigFltrEnable", (void**)&DAQmxSetCIVelocityEncoderBInputDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputDigFltrMinPulseWidth", (void**)&DAQmxSetCIVelocityEncoderBInputDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseRate", (void**)&DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseSrc", (void**)&DAQmxSetCIVelocityEncoderBInputDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputLogicLvlBehavior", (void**)&DAQmxSetCIVelocityEncoderBInputLogicLvlBehavior, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputTerm", (void**)&DAQmxSetCIVelocityEncoderBInputTerm, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderBInputTermCfg", (void**)&DAQmxSetCIVelocityEncoderBInputTermCfg, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityEncoderDecodingType", (void**)&DAQmxSetCIVelocityEncoderDecodingType, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityLinEncoderDistPerPulse", (void**)&DAQmxSetCIVelocityLinEncoderDistPerPulse, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityLinEncoderUnits", (void**)&DAQmxSetCIVelocityLinEncoderUnits, Dylib::Import::cMangled },
  { "DAQmxSetCIVelocityMeasTime", (void**)&DAQmxSetCIVelocityMeasTime, Dylib::Import::cMangled },
  { "DAQmxSetCOAutoIncrCnt", (void**)&DAQmxSetCOAutoIncrCnt, Dylib::Import::cMangled },
  { "DAQmxSetCOConstrainedGenMode", (void**)&DAQmxSetCOConstrainedGenMode, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseActiveEdge", (void**)&DAQmxSetCOCtrTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseDigFltrEnable", (void**)&DAQmxSetCOCtrTimebaseDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseDigFltrMinPulseWidth", (void**)&DAQmxSetCOCtrTimebaseDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseDigFltrTimebaseRate", (void**)&DAQmxSetCOCtrTimebaseDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseDigFltrTimebaseSrc", (void**)&DAQmxSetCOCtrTimebaseDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseDigSyncEnable", (void**)&DAQmxSetCOCtrTimebaseDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseMasterTimebaseDiv", (void**)&DAQmxSetCOCtrTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseRate", (void**)&DAQmxSetCOCtrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetCOCtrTimebaseSrc", (void**)&DAQmxSetCOCtrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetCODataXferMech", (void**)&DAQmxSetCODataXferMech, Dylib::Import::cMangled },
  { "DAQmxSetCODataXferReqCond", (void**)&DAQmxSetCODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxSetCOEnableInitialDelayOnRetrigger", (void**)&DAQmxSetCOEnableInitialDelayOnRetrigger, Dylib::Import::cMangled },
  { "DAQmxSetCOMemMapEnable", (void**)&DAQmxSetCOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxSetCOPrescaler", (void**)&DAQmxSetCOPrescaler, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseDutyCyc", (void**)&DAQmxSetCOPulseDutyCyc, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseFreq", (void**)&DAQmxSetCOPulseFreq, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseFreqInitialDelay", (void**)&DAQmxSetCOPulseFreqInitialDelay, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseFreqUnits", (void**)&DAQmxSetCOPulseFreqUnits, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseHighTicks", (void**)&DAQmxSetCOPulseHighTicks, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseHighTime", (void**)&DAQmxSetCOPulseHighTime, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseIdleState", (void**)&DAQmxSetCOPulseIdleState, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseLowTicks", (void**)&DAQmxSetCOPulseLowTicks, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseLowTime", (void**)&DAQmxSetCOPulseLowTime, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseTerm", (void**)&DAQmxSetCOPulseTerm, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseTicksInitialDelay", (void**)&DAQmxSetCOPulseTicksInitialDelay, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseTimeInitialDelay", (void**)&DAQmxSetCOPulseTimeInitialDelay, Dylib::Import::cMangled },
  { "DAQmxSetCOPulseTimeUnits", (void**)&DAQmxSetCOPulseTimeUnits, Dylib::Import::cMangled },
  { "DAQmxSetCOUsbXferReqCount", (void**)&DAQmxSetCOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxSetCOUsbXferReqSize", (void**)&DAQmxSetCOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxSetCOUseOnlyOnBrdMem", (void**)&DAQmxSetCOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxSetCalAccConnectionCount", (void**)&DAQmxSetCalAccConnectionCount, Dylib::Import::cMangled },
  { "DAQmxSetCalInfoAttribute", (void**)&DAQmxSetCalInfoAttribute, Dylib::Import::cMangled },
  { "DAQmxSetCalUserDefinedInfo", (void**)&DAQmxSetCalUserDefinedInfo, Dylib::Import::cMangled },
  { "DAQmxSetChanAttribute", (void**)&DAQmxSetChanAttribute, Dylib::Import::cMangled },
  { "DAQmxSetChanDescr", (void**)&DAQmxSetChanDescr, Dylib::Import::cMangled },
  { "DAQmxSetChanSyncUnlockBehavior", (void**)&DAQmxSetChanSyncUnlockBehavior, Dylib::Import::cMangled },
  { "DAQmxSetChangeDetectDIFallingEdgePhysicalChans", (void**)&DAQmxSetChangeDetectDIFallingEdgePhysicalChans, Dylib::Import::cMangled },
  { "DAQmxSetChangeDetectDIRisingEdgePhysicalChans", (void**)&DAQmxSetChangeDetectDIRisingEdgePhysicalChans, Dylib::Import::cMangled },
  { "DAQmxSetChangeDetectDITristate", (void**)&DAQmxSetChangeDetectDITristate, Dylib::Import::cMangled },
  { "DAQmxSetDIAcquireOn", (void**)&DAQmxSetDIAcquireOn, Dylib::Import::cMangled },
  { "DAQmxSetDIDataXferMech", (void**)&DAQmxSetDIDataXferMech, Dylib::Import::cMangled },
  { "DAQmxSetDIDataXferReqCond", (void**)&DAQmxSetDIDataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxSetDIDigFltrEnable", (void**)&DAQmxSetDIDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetDIDigFltrEnableBusMode", (void**)&DAQmxSetDIDigFltrEnableBusMode, Dylib::Import::cMangled },
  { "DAQmxSetDIDigFltrMinPulseWidth", (void**)&DAQmxSetDIDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetDIDigFltrTimebaseRate", (void**)&DAQmxSetDIDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetDIDigFltrTimebaseSrc", (void**)&DAQmxSetDIDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetDIDigSyncEnable", (void**)&DAQmxSetDIDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetDIInvertLines", (void**)&DAQmxSetDIInvertLines, Dylib::Import::cMangled },
  { "DAQmxSetDILogicFamily", (void**)&DAQmxSetDILogicFamily, Dylib::Import::cMangled },
  { "DAQmxSetDIMemMapEnable", (void**)&DAQmxSetDIMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxSetDITristate", (void**)&DAQmxSetDITristate, Dylib::Import::cMangled },
  { "DAQmxSetDIUsbXferReqCount", (void**)&DAQmxSetDIUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxSetDIUsbXferReqSize", (void**)&DAQmxSetDIUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxSetDODataXferMech", (void**)&DAQmxSetDODataXferMech, Dylib::Import::cMangled },
  { "DAQmxSetDODataXferReqCond", (void**)&DAQmxSetDODataXferReqCond, Dylib::Import::cMangled },
  { "DAQmxSetDOGenerateOn", (void**)&DAQmxSetDOGenerateOn, Dylib::Import::cMangled },
  { "DAQmxSetDOInvertLines", (void**)&DAQmxSetDOInvertLines, Dylib::Import::cMangled },
  { "DAQmxSetDOLineStatesDoneState", (void**)&DAQmxSetDOLineStatesDoneState, Dylib::Import::cMangled },
  { "DAQmxSetDOLineStatesPausedState", (void**)&DAQmxSetDOLineStatesPausedState, Dylib::Import::cMangled },
  { "DAQmxSetDOLineStatesStartState", (void**)&DAQmxSetDOLineStatesStartState, Dylib::Import::cMangled },
  { "DAQmxSetDOLogicFamily", (void**)&DAQmxSetDOLogicFamily, Dylib::Import::cMangled },
  { "DAQmxSetDOMemMapEnable", (void**)&DAQmxSetDOMemMapEnable, Dylib::Import::cMangled },
  { "DAQmxSetDOOutputDriveType", (void**)&DAQmxSetDOOutputDriveType, Dylib::Import::cMangled },
  { "DAQmxSetDOOvercurrentAutoReenable", (void**)&DAQmxSetDOOvercurrentAutoReenable, Dylib::Import::cMangled },
  { "DAQmxSetDOOvercurrentLimit", (void**)&DAQmxSetDOOvercurrentLimit, Dylib::Import::cMangled },
  { "DAQmxSetDOOvercurrentReenablePeriod", (void**)&DAQmxSetDOOvercurrentReenablePeriod, Dylib::Import::cMangled },
  { "DAQmxSetDOTristate", (void**)&DAQmxSetDOTristate, Dylib::Import::cMangled },
  { "DAQmxSetDOUsbXferReqCount", (void**)&DAQmxSetDOUsbXferReqCount, Dylib::Import::cMangled },
  { "DAQmxSetDOUsbXferReqSize", (void**)&DAQmxSetDOUsbXferReqSize, Dylib::Import::cMangled },
  { "DAQmxSetDOUseOnlyOnBrdMem", (void**)&DAQmxSetDOUseOnlyOnBrdMem, Dylib::Import::cMangled },
  { "DAQmxSetDelayFromSampClkDelay", (void**)&DAQmxSetDelayFromSampClkDelay, Dylib::Import::cMangled },
  { "DAQmxSetDelayFromSampClkDelayEx", (void**)&DAQmxSetDelayFromSampClkDelayEx, Dylib::Import::cMangled },
  { "DAQmxSetDelayFromSampClkDelayUnits", (void**)&DAQmxSetDelayFromSampClkDelayUnits, Dylib::Import::cMangled },
  { "DAQmxSetDelayFromSampClkDelayUnitsEx", (void**)&DAQmxSetDelayFromSampClkDelayUnitsEx, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeAdvTrigDigFltrEnable", (void**)&DAQmxSetDigEdgeAdvTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeAdvTrigEdge", (void**)&DAQmxSetDigEdgeAdvTrigEdge, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeAdvTrigSrc", (void**)&DAQmxSetDigEdgeAdvTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigDigFltrEnable", (void**)&DAQmxSetDigEdgeArmStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigDigFltrMinPulseWidth", (void**)&DAQmxSetDigEdgeArmStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseRate", (void**)&DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseSrc", (void**)&DAQmxSetDigEdgeArmStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigDigSyncEnable", (void**)&DAQmxSetDigEdgeArmStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigEdge", (void**)&DAQmxSetDigEdgeArmStartTrigEdge, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeArmStartTrigSrc", (void**)&DAQmxSetDigEdgeArmStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigDigFltrEnable", (void**)&DAQmxSetDigEdgeRefTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigDigFltrMinPulseWidth", (void**)&DAQmxSetDigEdgeRefTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigDigFltrTimebaseRate", (void**)&DAQmxSetDigEdgeRefTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigDigFltrTimebaseSrc", (void**)&DAQmxSetDigEdgeRefTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigDigSyncEnable", (void**)&DAQmxSetDigEdgeRefTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigEdge", (void**)&DAQmxSetDigEdgeRefTrigEdge, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeRefTrigSrc", (void**)&DAQmxSetDigEdgeRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigDigFltrEnable", (void**)&DAQmxSetDigEdgeStartTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigDigFltrMinPulseWidth", (void**)&DAQmxSetDigEdgeStartTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigDigFltrTimebaseRate", (void**)&DAQmxSetDigEdgeStartTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigDigFltrTimebaseSrc", (void**)&DAQmxSetDigEdgeStartTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigDigSyncEnable", (void**)&DAQmxSetDigEdgeStartTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigEdge", (void**)&DAQmxSetDigEdgeStartTrigEdge, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeStartTrigSrc", (void**)&DAQmxSetDigEdgeStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeWatchdogExpirTrigEdge", (void**)&DAQmxSetDigEdgeWatchdogExpirTrigEdge, Dylib::Import::cMangled },
  { "DAQmxSetDigEdgeWatchdogExpirTrigSrc", (void**)&DAQmxSetDigEdgeWatchdogExpirTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigDigFltrEnable", (void**)&DAQmxSetDigLvlPauseTrigDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigDigFltrMinPulseWidth", (void**)&DAQmxSetDigLvlPauseTrigDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigDigFltrTimebaseRate", (void**)&DAQmxSetDigLvlPauseTrigDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigDigFltrTimebaseSrc", (void**)&DAQmxSetDigLvlPauseTrigDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigDigSyncEnable", (void**)&DAQmxSetDigLvlPauseTrigDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigSrc", (void**)&DAQmxSetDigLvlPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigLvlPauseTrigWhen", (void**)&DAQmxSetDigLvlPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternPauseTrigPattern", (void**)&DAQmxSetDigPatternPauseTrigPattern, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternPauseTrigSrc", (void**)&DAQmxSetDigPatternPauseTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternPauseTrigWhen", (void**)&DAQmxSetDigPatternPauseTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternRefTrigPattern", (void**)&DAQmxSetDigPatternRefTrigPattern, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternRefTrigSrc", (void**)&DAQmxSetDigPatternRefTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternRefTrigWhen", (void**)&DAQmxSetDigPatternRefTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternStartTrigPattern", (void**)&DAQmxSetDigPatternStartTrigPattern, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternStartTrigSrc", (void**)&DAQmxSetDigPatternStartTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetDigPatternStartTrigWhen", (void**)&DAQmxSetDigPatternStartTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetDigitalLogicFamilyPowerUpState", (void**)&DAQmxSetDigitalLogicFamilyPowerUpState, Dylib::Import::cMangled },
  { "DAQmxSetDigitalPowerUpStates", (void**)&DAQmxSetDigitalPowerUpStates, Dylib::Import::cMangled },
  { "DAQmxSetDigitalPullUpPullDownStates", (void**)&DAQmxSetDigitalPullUpPullDownStates, Dylib::Import::cMangled },
  { "DAQmxSetExported10MHzRefClkOutputTerm", (void**)&DAQmxSetExported10MHzRefClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExported20MHzTimebaseOutputTerm", (void**)&DAQmxSetExported20MHzTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedAIConvClkOutputTerm", (void**)&DAQmxSetExportedAIConvClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedAIHoldCmpltEventOutputTerm", (void**)&DAQmxSetExportedAIHoldCmpltEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedAIHoldCmpltEventPulsePolarity", (void**)&DAQmxSetExportedAIHoldCmpltEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvCmpltEventDelay", (void**)&DAQmxSetExportedAdvCmpltEventDelay, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvCmpltEventOutputTerm", (void**)&DAQmxSetExportedAdvCmpltEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvCmpltEventPulsePolarity", (void**)&DAQmxSetExportedAdvCmpltEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvCmpltEventPulseWidth", (void**)&DAQmxSetExportedAdvCmpltEventPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvTrigOutputTerm", (void**)&DAQmxSetExportedAdvTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvTrigPulseWidth", (void**)&DAQmxSetExportedAdvTrigPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetExportedAdvTrigPulseWidthUnits", (void**)&DAQmxSetExportedAdvTrigPulseWidthUnits, Dylib::Import::cMangled },
  { "DAQmxSetExportedChangeDetectEventOutputTerm", (void**)&DAQmxSetExportedChangeDetectEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedChangeDetectEventPulsePolarity", (void**)&DAQmxSetExportedChangeDetectEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedCtrOutEventOutputBehavior", (void**)&DAQmxSetExportedCtrOutEventOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxSetExportedCtrOutEventOutputTerm", (void**)&DAQmxSetExportedCtrOutEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedCtrOutEventPulsePolarity", (void**)&DAQmxSetExportedCtrOutEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedCtrOutEventToggleIdleState", (void**)&DAQmxSetExportedCtrOutEventToggleIdleState, Dylib::Import::cMangled },
  { "DAQmxSetExportedDataActiveEventLvlActiveLvl", (void**)&DAQmxSetExportedDataActiveEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxSetExportedDataActiveEventOutputTerm", (void**)&DAQmxSetExportedDataActiveEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedDividedSampClkTimebaseOutputTerm", (void**)&DAQmxSetExportedDividedSampClkTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventDelay", (void**)&DAQmxSetExportedHshkEventDelay, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventInterlockedAssertOnStart", (void**)&DAQmxSetExportedHshkEventInterlockedAssertOnStart, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventInterlockedAssertedLvl", (void**)&DAQmxSetExportedHshkEventInterlockedAssertedLvl, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventInterlockedDeassertDelay", (void**)&DAQmxSetExportedHshkEventInterlockedDeassertDelay, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventOutputBehavior", (void**)&DAQmxSetExportedHshkEventOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventOutputTerm", (void**)&DAQmxSetExportedHshkEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventPulsePolarity", (void**)&DAQmxSetExportedHshkEventPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedHshkEventPulseWidth", (void**)&DAQmxSetExportedHshkEventPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetExportedPauseTrigLvlActiveLvl", (void**)&DAQmxSetExportedPauseTrigLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxSetExportedPauseTrigOutputTerm", (void**)&DAQmxSetExportedPauseTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedRdyForStartEventLvlActiveLvl", (void**)&DAQmxSetExportedRdyForStartEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxSetExportedRdyForStartEventOutputTerm", (void**)&DAQmxSetExportedRdyForStartEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedRdyForXferEventDeassertCond", (void**)&DAQmxSetExportedRdyForXferEventDeassertCond, Dylib::Import::cMangled },
  { "DAQmxSetExportedRdyForXferEventDeassertCondCustomThreshold", (void**)&DAQmxSetExportedRdyForXferEventDeassertCondCustomThreshold, Dylib::Import::cMangled },
  { "DAQmxSetExportedRdyForXferEventLvlActiveLvl", (void**)&DAQmxSetExportedRdyForXferEventLvlActiveLvl, Dylib::Import::cMangled },
  { "DAQmxSetExportedRdyForXferEventOutputTerm", (void**)&DAQmxSetExportedRdyForXferEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedRefTrigOutputTerm", (void**)&DAQmxSetExportedRefTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedRefTrigPulsePolarity", (void**)&DAQmxSetExportedRefTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedSampClkDelayOffset", (void**)&DAQmxSetExportedSampClkDelayOffset, Dylib::Import::cMangled },
  { "DAQmxSetExportedSampClkOutputBehavior", (void**)&DAQmxSetExportedSampClkOutputBehavior, Dylib::Import::cMangled },
  { "DAQmxSetExportedSampClkOutputTerm", (void**)&DAQmxSetExportedSampClkOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedSampClkPulsePolarity", (void**)&DAQmxSetExportedSampClkPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedSampClkTimebaseOutputTerm", (void**)&DAQmxSetExportedSampClkTimebaseOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedSignalAttribute", (void**)&DAQmxSetExportedSignalAttribute, Dylib::Import::cMangled },
  { "DAQmxSetExportedStartTrigOutputTerm", (void**)&DAQmxSetExportedStartTrigOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedStartTrigPulsePolarity", (void**)&DAQmxSetExportedStartTrigPulsePolarity, Dylib::Import::cMangled },
  { "DAQmxSetExportedSyncPulseEventOutputTerm", (void**)&DAQmxSetExportedSyncPulseEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetExportedWatchdogExpiredEventOutputTerm", (void**)&DAQmxSetExportedWatchdogExpiredEventOutputTerm, Dylib::Import::cMangled },
  { "DAQmxSetFirstSampClkOffset", (void**)&DAQmxSetFirstSampClkOffset, Dylib::Import::cMangled },
  { "DAQmxSetFirstSampClkTimescale", (void**)&DAQmxSetFirstSampClkTimescale, Dylib::Import::cMangled },
  { "DAQmxSetFirstSampClkWhen", (void**)&DAQmxSetFirstSampClkWhen, Dylib::Import::cMangled },
  { "DAQmxSetFirstSampTimestampEnable", (void**)&DAQmxSetFirstSampTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxSetFirstSampTimestampTimescale", (void**)&DAQmxSetFirstSampTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxSetHshkDelayAfterXfer", (void**)&DAQmxSetHshkDelayAfterXfer, Dylib::Import::cMangled },
  { "DAQmxSetHshkSampleInputDataWhen", (void**)&DAQmxSetHshkSampleInputDataWhen, Dylib::Import::cMangled },
  { "DAQmxSetHshkStartCond", (void**)&DAQmxSetHshkStartCond, Dylib::Import::cMangled },
  { "DAQmxSetHshkTrigType", (void**)&DAQmxSetHshkTrigType, Dylib::Import::cMangled },
  { "DAQmxSetImplicitUnderflowBehavior", (void**)&DAQmxSetImplicitUnderflowBehavior, Dylib::Import::cMangled },
  { "DAQmxSetInterlockedHshkTrigAssertedLvl", (void**)&DAQmxSetInterlockedHshkTrigAssertedLvl, Dylib::Import::cMangled },
  { "DAQmxSetInterlockedHshkTrigSrc", (void**)&DAQmxSetInterlockedHshkTrigSrc, Dylib::Import::cMangled },
  { "DAQmxSetLoggingFilePath", (void**)&DAQmxSetLoggingFilePath, Dylib::Import::cMangled },
  { "DAQmxSetLoggingFilePreallocationSize", (void**)&DAQmxSetLoggingFilePreallocationSize, Dylib::Import::cMangled },
  { "DAQmxSetLoggingFileWriteSize", (void**)&DAQmxSetLoggingFileWriteSize, Dylib::Import::cMangled },
  { "DAQmxSetLoggingMode", (void**)&DAQmxSetLoggingMode, Dylib::Import::cMangled },
  { "DAQmxSetLoggingPause", (void**)&DAQmxSetLoggingPause, Dylib::Import::cMangled },
  { "DAQmxSetLoggingSampsPerFile", (void**)&DAQmxSetLoggingSampsPerFile, Dylib::Import::cMangled },
  { "DAQmxSetLoggingTDMSGroupName", (void**)&DAQmxSetLoggingTDMSGroupName, Dylib::Import::cMangled },
  { "DAQmxSetLoggingTDMSOperation", (void**)&DAQmxSetLoggingTDMSOperation, Dylib::Import::cMangled },
  { "DAQmxSetMasterTimebaseRate", (void**)&DAQmxSetMasterTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetMasterTimebaseSrc", (void**)&DAQmxSetMasterTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetOnDemandSimultaneousAOEnable", (void**)&DAQmxSetOnDemandSimultaneousAOEnable, Dylib::Import::cMangled },
  { "DAQmxSetPauseTrigType", (void**)&DAQmxSetPauseTrigType, Dylib::Import::cMangled },
  { "DAQmxSetPhysicalChanAIPowerControlEnable", (void**)&DAQmxSetPhysicalChanAIPowerControlEnable, Dylib::Import::cMangled },
  { "DAQmxSetPhysicalChanAIPowerControlType", (void**)&DAQmxSetPhysicalChanAIPowerControlType, Dylib::Import::cMangled },
  { "DAQmxSetPhysicalChanAIPowerControlVoltage", (void**)&DAQmxSetPhysicalChanAIPowerControlVoltage, Dylib::Import::cMangled },
  { "DAQmxSetPhysicalChanAOManualControlEnable", (void**)&DAQmxSetPhysicalChanAOManualControlEnable, Dylib::Import::cMangled },
  { "DAQmxSetPhysicalChanName", (void**)&DAQmxSetPhysicalChanName, Dylib::Import::cMangled },
  { "DAQmxSetReadAttribute", (void**)&DAQmxSetReadAttribute, Dylib::Import::cMangled },
  { "DAQmxSetReadAutoStart", (void**)&DAQmxSetReadAutoStart, Dylib::Import::cMangled },
  { "DAQmxSetReadChannelsToRead", (void**)&DAQmxSetReadChannelsToRead, Dylib::Import::cMangled },
  { "DAQmxSetReadOffset", (void**)&DAQmxSetReadOffset, Dylib::Import::cMangled },
  { "DAQmxSetReadOverWrite", (void**)&DAQmxSetReadOverWrite, Dylib::Import::cMangled },
  { "DAQmxSetReadReadAllAvailSamp", (void**)&DAQmxSetReadReadAllAvailSamp, Dylib::Import::cMangled },
  { "DAQmxSetReadRelativeTo", (void**)&DAQmxSetReadRelativeTo, Dylib::Import::cMangled },
  { "DAQmxSetReadSleepTime", (void**)&DAQmxSetReadSleepTime, Dylib::Import::cMangled },
  { "DAQmxSetReadWaitMode", (void**)&DAQmxSetReadWaitMode, Dylib::Import::cMangled },
  { "DAQmxSetRealTimeAttribute", (void**)&DAQmxSetRealTimeAttribute, Dylib::Import::cMangled },
  { "DAQmxSetRealTimeConvLateErrorsToWarnings", (void**)&DAQmxSetRealTimeConvLateErrorsToWarnings, Dylib::Import::cMangled },
  { "DAQmxSetRealTimeNumOfWarmupIters", (void**)&DAQmxSetRealTimeNumOfWarmupIters, Dylib::Import::cMangled },
  { "DAQmxSetRealTimeReportMissedSamp", (void**)&DAQmxSetRealTimeReportMissedSamp, Dylib::Import::cMangled },
  { "DAQmxSetRealTimeWaitForNextSampClkWaitMode", (void**)&DAQmxSetRealTimeWaitForNextSampClkWaitMode, Dylib::Import::cMangled },
  { "DAQmxSetRealTimeWriteRecoveryMode", (void**)&DAQmxSetRealTimeWriteRecoveryMode, Dylib::Import::cMangled },
  { "DAQmxSetRefClkRate", (void**)&DAQmxSetRefClkRate, Dylib::Import::cMangled },
  { "DAQmxSetRefClkSrc", (void**)&DAQmxSetRefClkSrc, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigAutoTrigEnable", (void**)&DAQmxSetRefTrigAutoTrigEnable, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigDelay", (void**)&DAQmxSetRefTrigDelay, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigMaxNumTrigsToDetect", (void**)&DAQmxSetRefTrigMaxNumTrigsToDetect, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigPretrigSamples", (void**)&DAQmxSetRefTrigPretrigSamples, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigRetriggerWin", (void**)&DAQmxSetRefTrigRetriggerWin, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigRetriggerable", (void**)&DAQmxSetRefTrigRetriggerable, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigTimestampEnable", (void**)&DAQmxSetRefTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigTimestampTimescale", (void**)&DAQmxSetRefTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigTrigWin", (void**)&DAQmxSetRefTrigTrigWin, Dylib::Import::cMangled },
  { "DAQmxSetRefTrigType", (void**)&DAQmxSetRefTrigType, Dylib::Import::cMangled },
  { "DAQmxSetSCExpressCalAccBridgeOutput", (void**)&DAQmxSetSCExpressCalAccBridgeOutput, Dylib::Import::cMangled },
  { "DAQmxSetSampClkActiveEdge", (void**)&DAQmxSetSampClkActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetSampClkDigFltrEnable", (void**)&DAQmxSetSampClkDigFltrEnable, Dylib::Import::cMangled },
  { "DAQmxSetSampClkDigFltrMinPulseWidth", (void**)&DAQmxSetSampClkDigFltrMinPulseWidth, Dylib::Import::cMangled },
  { "DAQmxSetSampClkDigFltrTimebaseRate", (void**)&DAQmxSetSampClkDigFltrTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetSampClkDigFltrTimebaseSrc", (void**)&DAQmxSetSampClkDigFltrTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetSampClkDigSyncEnable", (void**)&DAQmxSetSampClkDigSyncEnable, Dylib::Import::cMangled },
  { "DAQmxSetSampClkOverrunBehavior", (void**)&DAQmxSetSampClkOverrunBehavior, Dylib::Import::cMangled },
  { "DAQmxSetSampClkRate", (void**)&DAQmxSetSampClkRate, Dylib::Import::cMangled },
  { "DAQmxSetSampClkSrc", (void**)&DAQmxSetSampClkSrc, Dylib::Import::cMangled },
  { "DAQmxSetSampClkTimebaseActiveEdge", (void**)&DAQmxSetSampClkTimebaseActiveEdge, Dylib::Import::cMangled },
  { "DAQmxSetSampClkTimebaseDiv", (void**)&DAQmxSetSampClkTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxSetSampClkTimebaseMasterTimebaseDiv", (void**)&DAQmxSetSampClkTimebaseMasterTimebaseDiv, Dylib::Import::cMangled },
  { "DAQmxSetSampClkTimebaseRate", (void**)&DAQmxSetSampClkTimebaseRate, Dylib::Import::cMangled },
  { "DAQmxSetSampClkTimebaseSrc", (void**)&DAQmxSetSampClkTimebaseSrc, Dylib::Import::cMangled },
  { "DAQmxSetSampClkTimingResponseMode", (void**)&DAQmxSetSampClkTimingResponseMode, Dylib::Import::cMangled },
  { "DAQmxSetSampClkUnderflowBehavior", (void**)&DAQmxSetSampClkUnderflowBehavior, Dylib::Import::cMangled },
  { "DAQmxSetSampClkWriteWfmUseInitialWfmDT", (void**)&DAQmxSetSampClkWriteWfmUseInitialWfmDT, Dylib::Import::cMangled },
  { "DAQmxSetSampQuantSampMode", (void**)&DAQmxSetSampQuantSampMode, Dylib::Import::cMangled },
  { "DAQmxSetSampQuantSampPerChan", (void**)&DAQmxSetSampQuantSampPerChan, Dylib::Import::cMangled },
  { "DAQmxSetSampTimingEngine", (void**)&DAQmxSetSampTimingEngine, Dylib::Import::cMangled },
  { "DAQmxSetSampTimingType", (void**)&DAQmxSetSampTimingType, Dylib::Import::cMangled },
  { "DAQmxSetScaleAttribute", (void**)&DAQmxSetScaleAttribute, Dylib::Import::cMangled },
  { "DAQmxSetScaleDescr", (void**)&DAQmxSetScaleDescr, Dylib::Import::cMangled },
  { "DAQmxSetScaleLinSlope", (void**)&DAQmxSetScaleLinSlope, Dylib::Import::cMangled },
  { "DAQmxSetScaleLinYIntercept", (void**)&DAQmxSetScaleLinYIntercept, Dylib::Import::cMangled },
  { "DAQmxSetScaleMapPreScaledMax", (void**)&DAQmxSetScaleMapPreScaledMax, Dylib::Import::cMangled },
  { "DAQmxSetScaleMapPreScaledMin", (void**)&DAQmxSetScaleMapPreScaledMin, Dylib::Import::cMangled },
  { "DAQmxSetScaleMapScaledMax", (void**)&DAQmxSetScaleMapScaledMax, Dylib::Import::cMangled },
  { "DAQmxSetScaleMapScaledMin", (void**)&DAQmxSetScaleMapScaledMin, Dylib::Import::cMangled },
  { "DAQmxSetScalePolyForwardCoeff", (void**)&DAQmxSetScalePolyForwardCoeff, Dylib::Import::cMangled },
  { "DAQmxSetScalePolyReverseCoeff", (void**)&DAQmxSetScalePolyReverseCoeff, Dylib::Import::cMangled },
  { "DAQmxSetScalePreScaledUnits", (void**)&DAQmxSetScalePreScaledUnits, Dylib::Import::cMangled },
  { "DAQmxSetScaleScaledUnits", (void**)&DAQmxSetScaleScaledUnits, Dylib::Import::cMangled },
  { "DAQmxSetScaleTablePreScaledVals", (void**)&DAQmxSetScaleTablePreScaledVals, Dylib::Import::cMangled },
  { "DAQmxSetScaleTableScaledVals", (void**)&DAQmxSetScaleTableScaledVals, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigDelay", (void**)&DAQmxSetStartTrigDelay, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigDelayUnits", (void**)&DAQmxSetStartTrigDelayUnits, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigMaxNumTrigsToDetect", (void**)&DAQmxSetStartTrigMaxNumTrigsToDetect, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigRetriggerWin", (void**)&DAQmxSetStartTrigRetriggerWin, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigRetriggerable", (void**)&DAQmxSetStartTrigRetriggerable, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigTimescale", (void**)&DAQmxSetStartTrigTimescale, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigTimestampEnable", (void**)&DAQmxSetStartTrigTimestampEnable, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigTimestampTimescale", (void**)&DAQmxSetStartTrigTimestampTimescale, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigTrigWhen", (void**)&DAQmxSetStartTrigTrigWhen, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigTrigWin", (void**)&DAQmxSetStartTrigTrigWin, Dylib::Import::cMangled },
  { "DAQmxSetStartTrigType", (void**)&DAQmxSetStartTrigType, Dylib::Import::cMangled },
  { "DAQmxSetSwitchChanAnlgBusSharingEnable", (void**)&DAQmxSetSwitchChanAnlgBusSharingEnable, Dylib::Import::cMangled },
  { "DAQmxSetSwitchChanAttribute", (void**)&DAQmxSetSwitchChanAttribute, Dylib::Import::cMangled },
  { "DAQmxSetSwitchChanUsage", (void**)&DAQmxSetSwitchChanUsage, Dylib::Import::cMangled },
  { "DAQmxSetSwitchDevAutoConnAnlgBus", (void**)&DAQmxSetSwitchDevAutoConnAnlgBus, Dylib::Import::cMangled },
  { "DAQmxSetSwitchDevPwrDownLatchRelaysAfterSettling", (void**)&DAQmxSetSwitchDevPwrDownLatchRelaysAfterSettling, Dylib::Import::cMangled },
  { "DAQmxSetSwitchDevSettlingTime", (void**)&DAQmxSetSwitchDevSettlingTime, Dylib::Import::cMangled },
  { "DAQmxSetSwitchDeviceAttribute", (void**)&DAQmxSetSwitchDeviceAttribute, Dylib::Import::cMangled },
  { "DAQmxSetSwitchScanAttribute", (void**)&DAQmxSetSwitchScanAttribute, Dylib::Import::cMangled },
  { "DAQmxSetSwitchScanBreakMode", (void**)&DAQmxSetSwitchScanBreakMode, Dylib::Import::cMangled },
  { "DAQmxSetSwitchScanRepeatMode", (void**)&DAQmxSetSwitchScanRepeatMode, Dylib::Import::cMangled },
  { "DAQmxSetSyncClkInterval", (void**)&DAQmxSetSyncClkInterval, Dylib::Import::cMangled },
  { "DAQmxSetSyncPulseMinDelayToStart", (void**)&DAQmxSetSyncPulseMinDelayToStart, Dylib::Import::cMangled },
  { "DAQmxSetSyncPulseResetDelay", (void**)&DAQmxSetSyncPulseResetDelay, Dylib::Import::cMangled },
  { "DAQmxSetSyncPulseSrc", (void**)&DAQmxSetSyncPulseSrc, Dylib::Import::cMangled },
  { "DAQmxSetSyncPulseTimeTimescale", (void**)&DAQmxSetSyncPulseTimeTimescale, Dylib::Import::cMangled },
  { "DAQmxSetSyncPulseTimeWhen", (void**)&DAQmxSetSyncPulseTimeWhen, Dylib::Import::cMangled },
  { "DAQmxSetSyncPulseType", (void**)&DAQmxSetSyncPulseType, Dylib::Import::cMangled },
  { "DAQmxSetTimingAttribute", (void**)&DAQmxSetTimingAttribute, Dylib::Import::cMangled },
  { "DAQmxSetTimingAttributeEx", (void**)&DAQmxSetTimingAttributeEx, Dylib::Import::cMangled },
  { "DAQmxSetTrigAttribute", (void**)&DAQmxSetTrigAttribute, Dylib::Import::cMangled },
  { "DAQmxSetTriggerSyncType", (void**)&DAQmxSetTriggerSyncType, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogAOExpirState", (void**)&DAQmxSetWatchdogAOExpirState, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogAOOutputType", (void**)&DAQmxSetWatchdogAOOutputType, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogAttribute", (void**)&DAQmxSetWatchdogAttribute, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogCOExpirState", (void**)&DAQmxSetWatchdogCOExpirState, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogDOExpirState", (void**)&DAQmxSetWatchdogDOExpirState, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogExpirTrigTrigOnNetworkConnLoss", (void**)&DAQmxSetWatchdogExpirTrigTrigOnNetworkConnLoss, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogExpirTrigType", (void**)&DAQmxSetWatchdogExpirTrigType, Dylib::Import::cMangled },
  { "DAQmxSetWatchdogTimeout", (void**)&DAQmxSetWatchdogTimeout, Dylib::Import::cMangled },
  { "DAQmxSetWriteAttribute", (void**)&DAQmxSetWriteAttribute, Dylib::Import::cMangled },
  { "DAQmxSetWriteOffset", (void**)&DAQmxSetWriteOffset, Dylib::Import::cMangled },
  { "DAQmxSetWriteRegenMode", (void**)&DAQmxSetWriteRegenMode, Dylib::Import::cMangled },
  { "DAQmxSetWriteRelativeTo", (void**)&DAQmxSetWriteRelativeTo, Dylib::Import::cMangled },
  { "DAQmxSetWriteSleepTime", (void**)&DAQmxSetWriteSleepTime, Dylib::Import::cMangled },
  { "DAQmxSetWriteWaitMode", (void**)&DAQmxSetWriteWaitMode, Dylib::Import::cMangled },
  { "DAQmxSetup1102Cal", (void**)&DAQmxSetup1102Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1104Cal", (void**)&DAQmxSetup1104Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1112Cal", (void**)&DAQmxSetup1112Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1122Cal", (void**)&DAQmxSetup1122Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1124Cal", (void**)&DAQmxSetup1124Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1125Cal", (void**)&DAQmxSetup1125Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1126Cal", (void**)&DAQmxSetup1126Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1141Cal", (void**)&DAQmxSetup1141Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1142Cal", (void**)&DAQmxSetup1142Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1143Cal", (void**)&DAQmxSetup1143Cal, Dylib::Import::cMangled },
  { "DAQmxSetup11605Cal", (void**)&DAQmxSetup11605Cal, Dylib::Import::cMangled },
  { "DAQmxSetup11634Cal", (void**)&DAQmxSetup11634Cal, Dylib::Import::cMangled },
  { "DAQmxSetup11637Cal", (void**)&DAQmxSetup11637Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1502Cal", (void**)&DAQmxSetup1502Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1503Cal", (void**)&DAQmxSetup1503Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1520Cal", (void**)&DAQmxSetup1520Cal, Dylib::Import::cMangled },
  { "DAQmxSetup1521Cal", (void**)&DAQmxSetup1521Cal, Dylib::Import::cMangled },
  { "DAQmxSetup153xCal", (void**)&DAQmxSetup153xCal, Dylib::Import::cMangled },
  { "DAQmxSetup1540Cal", (void**)&DAQmxSetup1540Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4302Cal", (void**)&DAQmxSetup4302Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4303Cal", (void**)&DAQmxSetup4303Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4304Cal", (void**)&DAQmxSetup4304Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4305Cal", (void**)&DAQmxSetup4305Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4322Cal", (void**)&DAQmxSetup4322Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4339Cal", (void**)&DAQmxSetup4339Cal, Dylib::Import::cMangled },
  { "DAQmxSetup433xCal", (void**)&DAQmxSetup433xCal, Dylib::Import::cMangled },
  { "DAQmxSetup4463Cal", (void**)&DAQmxSetup4463Cal, Dylib::Import::cMangled },
  { "DAQmxSetup4480Cal", (void**)&DAQmxSetup4480Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9218Cal", (void**)&DAQmxSetup9218Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9219Cal", (void**)&DAQmxSetup9219Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9242Cal", (void**)&DAQmxSetup9242Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9244Cal", (void**)&DAQmxSetup9244Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9260Cal", (void**)&DAQmxSetup9260Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9262Cal", (void**)&DAQmxSetup9262Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9263Cal", (void**)&DAQmxSetup9263Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9264Cal", (void**)&DAQmxSetup9264Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9265Cal", (void**)&DAQmxSetup9265Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9266Cal", (void**)&DAQmxSetup9266Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9269Cal", (void**)&DAQmxSetup9269Cal, Dylib::Import::cMangled },
  { "DAQmxSetup9628AOCal", (void**)&DAQmxSetup9628AOCal, Dylib::Import::cMangled },
  { "DAQmxSetup9629AOCal", (void**)&DAQmxSetup9629AOCal, Dylib::Import::cMangled },
  { "DAQmxSetup9638AOCal", (void**)&DAQmxSetup9638AOCal, Dylib::Import::cMangled },
  { "DAQmxSetupDSAAOTimebaseCal", (void**)&DAQmxSetupDSAAOTimebaseCal, Dylib::Import::cMangled },
  { "DAQmxStartNewFile", (void**)&DAQmxStartNewFile, Dylib::Import::cMangled },
  { "DAQmxStartTask", (void**)&DAQmxStartTask, Dylib::Import::cMangled },
  { "DAQmxStopTask", (void**)&DAQmxStopTask, Dylib::Import::cMangled },
  { "DAQmxSwitchCloseRelays", (void**)&DAQmxSwitchCloseRelays, Dylib::Import::cMangled },
  { "DAQmxSwitchConnect", (void**)&DAQmxSwitchConnect, Dylib::Import::cMangled },
  { "DAQmxSwitchConnectMulti", (void**)&DAQmxSwitchConnectMulti, Dylib::Import::cMangled },
  { "DAQmxSwitchCreateScanList", (void**)&DAQmxSwitchCreateScanList, Dylib::Import::cMangled },
  { "DAQmxSwitchDisconnect", (void**)&DAQmxSwitchDisconnect, Dylib::Import::cMangled },
  { "DAQmxSwitchDisconnectAll", (void**)&DAQmxSwitchDisconnectAll, Dylib::Import::cMangled },
  { "DAQmxSwitchDisconnectMulti", (void**)&DAQmxSwitchDisconnectMulti, Dylib::Import::cMangled },
  { "DAQmxSwitchFindPath", (void**)&DAQmxSwitchFindPath, Dylib::Import::cMangled },
  { "DAQmxSwitchGetMultiRelayCount", (void**)&DAQmxSwitchGetMultiRelayCount, Dylib::Import::cMangled },
  { "DAQmxSwitchGetMultiRelayPos", (void**)&DAQmxSwitchGetMultiRelayPos, Dylib::Import::cMangled },
  { "DAQmxSwitchGetSingleRelayCount", (void**)&DAQmxSwitchGetSingleRelayCount, Dylib::Import::cMangled },
  { "DAQmxSwitchGetSingleRelayPos", (void**)&DAQmxSwitchGetSingleRelayPos, Dylib::Import::cMangled },
  { "DAQmxSwitchOpenRelays", (void**)&DAQmxSwitchOpenRelays, Dylib::Import::cMangled },
  { "DAQmxSwitchSetTopologyAndReset", (void**)&DAQmxSwitchSetTopologyAndReset, Dylib::Import::cMangled },
  { "DAQmxSwitchWaitForSettling", (void**)&DAQmxSwitchWaitForSettling, Dylib::Import::cMangled },
  { "DAQmxTaskControl", (void**)&DAQmxTaskControl, Dylib::Import::cMangled },
  { "DAQmxTristateOutputTerm", (void**)&DAQmxTristateOutputTerm, Dylib::Import::cMangled },
  { "DAQmxUnreserveNetworkDevice", (void**)&DAQmxUnreserveNetworkDevice, Dylib::Import::cMangled },
  { "DAQmxWaitForNextSampleClock", (void**)&DAQmxWaitForNextSampleClock, Dylib::Import::cMangled },
  { "DAQmxWaitForValidTimestamp", (void**)&DAQmxWaitForValidTimestamp, Dylib::Import::cMangled },
  { "DAQmxWaitUntilTaskDone", (void**)&DAQmxWaitUntilTaskDone, Dylib::Import::cMangled },
  { "DAQmxWriteAnalogF64", (void**)&DAQmxWriteAnalogF64, Dylib::Import::cMangled },
  { "DAQmxWriteAnalogScalarF64", (void**)&DAQmxWriteAnalogScalarF64, Dylib::Import::cMangled },
  { "DAQmxWriteBinaryI16", (void**)&DAQmxWriteBinaryI16, Dylib::Import::cMangled },
  { "DAQmxWriteBinaryI32", (void**)&DAQmxWriteBinaryI32, Dylib::Import::cMangled },
  { "DAQmxWriteBinaryU16", (void**)&DAQmxWriteBinaryU16, Dylib::Import::cMangled },
  { "DAQmxWriteBinaryU32", (void**)&DAQmxWriteBinaryU32, Dylib::Import::cMangled },
  { "DAQmxWriteCtrFreq", (void**)&DAQmxWriteCtrFreq, Dylib::Import::cMangled },
  { "DAQmxWriteCtrFreqScalar", (void**)&DAQmxWriteCtrFreqScalar, Dylib::Import::cMangled },
  { "DAQmxWriteCtrTicks", (void**)&DAQmxWriteCtrTicks, Dylib::Import::cMangled },
  { "DAQmxWriteCtrTicksScalar", (void**)&DAQmxWriteCtrTicksScalar, Dylib::Import::cMangled },
  { "DAQmxWriteCtrTime", (void**)&DAQmxWriteCtrTime, Dylib::Import::cMangled },
  { "DAQmxWriteCtrTimeScalar", (void**)&DAQmxWriteCtrTimeScalar, Dylib::Import::cMangled },
  { "DAQmxWriteDigitalLines", (void**)&DAQmxWriteDigitalLines, Dylib::Import::cMangled },
  { "DAQmxWriteDigitalScalarU32", (void**)&DAQmxWriteDigitalScalarU32, Dylib::Import::cMangled },
  { "DAQmxWriteDigitalU16", (void**)&DAQmxWriteDigitalU16, Dylib::Import::cMangled },
  { "DAQmxWriteDigitalU32", (void**)&DAQmxWriteDigitalU32, Dylib::Import::cMangled },
  { "DAQmxWriteDigitalU8", (void**)&DAQmxWriteDigitalU8, Dylib::Import::cMangled },
  { "DAQmxWriteRaw", (void**)&DAQmxWriteRaw, Dylib::Import::cMangled },
  { "DAQmxWriteToTEDSFromArray", (void**)&DAQmxWriteToTEDSFromArray, Dylib::Import::cMangled },
  { "DAQmxWriteToTEDSFromFile", (void**)&DAQmxWriteToTEDSFromFile, Dylib::Import::cMangled },
  { "DAQmxXSeriesCalAdjust", (void**)&DAQmxXSeriesCalAdjust, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBNICAIU

