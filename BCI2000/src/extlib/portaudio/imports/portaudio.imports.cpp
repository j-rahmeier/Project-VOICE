// Import declarations for libportaudio-i686-w64-mingw32.static library
// generated Thu May  5 19:39:12 2022 by DylibTool
#if STATIC_LIBPORTAUDIO

namespace Dylib { bool portaudio_Loaded() { return true; } }
extern "C" {
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( portaudio, "portaudio-asio|libportaudio", functionImports, notFoundMsg, notFoundURL );

extern "C" {
// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void PaUtil_AdvanceRingBufferReadIndex() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_AdvanceRingBufferReadIndex" ); }
void PaUtil_AdvanceRingBufferWriteIndex() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_AdvanceRingBufferWriteIndex" ); }
void PaUtil_AllocateMemory() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_AllocateMemory" ); }
void PaUtil_BeginBufferProcessing() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_BeginBufferProcessing" ); }
void PaUtil_BeginCpuLoadMeasurement() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_BeginCpuLoadMeasurement" ); }
void PaUtil_CopyInput() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_CopyInput" ); }
void PaUtil_CopyOutput() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_CopyOutput" ); }
void PaUtil_CountCurrentlyAllocatedBlocks() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_CountCurrentlyAllocatedBlocks" ); }
void PaUtil_CreateAllocationGroup() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_CreateAllocationGroup" ); }
void PaUtil_DebugPrint() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DebugPrint" ); }
void PaUtil_DestroyAllocationGroup() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DestroyAllocationGroup" ); }
void PaUtil_DeviceIndexToHostApiDeviceIndex() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DeviceIndexToHostApiDeviceIndex" ); }
void PaUtil_DummyGetCpuLoad() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DummyGetCpuLoad" ); }
void PaUtil_DummyGetReadAvailable() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DummyGetReadAvailable" ); }
void PaUtil_DummyGetWriteAvailable() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DummyGetWriteAvailable" ); }
void PaUtil_DummyRead() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DummyRead" ); }
void PaUtil_DummyWrite() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_DummyWrite" ); }
void PaUtil_EndBufferProcessing() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_EndBufferProcessing" ); }
void PaUtil_EndCpuLoadMeasurement() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_EndCpuLoadMeasurement" ); }
void PaUtil_FlushRingBuffer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_FlushRingBuffer" ); }
void PaUtil_FreeAllAllocations() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_FreeAllAllocations" ); }
void PaUtil_FreeMemory() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_FreeMemory" ); }
void PaUtil_Generate16BitTriangularDither() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Generate16BitTriangularDither" ); }
void PaUtil_GenerateFloatTriangularDither() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GenerateFloatTriangularDither" ); }
void PaUtil_GetBufferProcessorInputLatencyFrames() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetBufferProcessorInputLatencyFrames" ); }
void PaUtil_GetBufferProcessorOutputLatencyFrames() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetBufferProcessorOutputLatencyFrames" ); }
void PaUtil_GetCpuLoad() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetCpuLoad" ); }
void PaUtil_GetHostApiRepresentation() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetHostApiRepresentation" ); }
void PaUtil_GetOutputFrameCount() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetOutputFrameCount" ); }
void PaUtil_GetRingBufferReadAvailable() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetRingBufferReadAvailable" ); }
void PaUtil_GetRingBufferReadRegions() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetRingBufferReadRegions" ); }
void PaUtil_GetRingBufferWriteAvailable() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetRingBufferWriteAvailable" ); }
void PaUtil_GetRingBufferWriteRegions() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetRingBufferWriteRegions" ); }
void PaUtil_GetTime() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GetTime" ); }
void PaUtil_GroupAllocateMemory() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GroupAllocateMemory" ); }
void PaUtil_GroupFreeMemory() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_GroupFreeMemory" ); }
void PaUtil_InitializeBufferProcessor() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeBufferProcessor" ); }
void PaUtil_InitializeClock() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeClock" ); }
void PaUtil_InitializeCpuLoadMeasurer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeCpuLoadMeasurer" ); }
void PaUtil_InitializeRingBuffer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeRingBuffer" ); }
void PaUtil_InitializeStreamInterface() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeStreamInterface" ); }
void PaUtil_InitializeStreamRepresentation() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeStreamRepresentation" ); }
void PaUtil_InitializeTriangularDitherState() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_InitializeTriangularDitherState" ); }
void PaUtil_IsBufferProcessorOutputEmpty() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_IsBufferProcessorOutputEmpty" ); }
void PaUtil_ReadRingBuffer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_ReadRingBuffer" ); }
void PaUtil_ResetBufferProcessor() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_ResetBufferProcessor" ); }
void PaUtil_ResetCpuLoadMeasurer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_ResetCpuLoadMeasurer" ); }
void PaUtil_SelectClosestAvailableFormat() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SelectClosestAvailableFormat" ); }
void PaUtil_SelectConverter() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SelectConverter" ); }
void PaUtil_SelectZeroer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SelectZeroer" ); }
void PaUtil_Set2ndInputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndInputChannel" ); }
void PaUtil_Set2ndInputFrameCount() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndInputFrameCount" ); }
void PaUtil_Set2ndInterleavedInputChannels() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndInterleavedInputChannels" ); }
void PaUtil_Set2ndInterleavedOutputChannels() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndInterleavedOutputChannels" ); }
void PaUtil_Set2ndNonInterleavedInputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndNonInterleavedInputChannel" ); }
void PaUtil_Set2ndNonInterleavedOutputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndNonInterleavedOutputChannel" ); }
void PaUtil_Set2ndOutputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndOutputChannel" ); }
void PaUtil_Set2ndOutputFrameCount() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_Set2ndOutputFrameCount" ); }
void PaUtil_SetDebugPrintFunction() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetDebugPrintFunction" ); }
void PaUtil_SetInputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetInputChannel" ); }
void PaUtil_SetInputFrameCount() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetInputFrameCount" ); }
void PaUtil_SetInterleavedInputChannels() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetInterleavedInputChannels" ); }
void PaUtil_SetInterleavedOutputChannels() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetInterleavedOutputChannels" ); }
void PaUtil_SetLastHostErrorInfo() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetLastHostErrorInfo" ); }
void PaUtil_SetNoInput() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetNoInput" ); }
void PaUtil_SetNoOutput() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetNoOutput" ); }
void PaUtil_SetNonInterleavedInputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetNonInterleavedInputChannel" ); }
void PaUtil_SetNonInterleavedOutputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetNonInterleavedOutputChannel" ); }
void PaUtil_SetOutputChannel() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetOutputChannel" ); }
void PaUtil_SetOutputFrameCount() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_SetOutputFrameCount" ); }
void PaUtil_TerminateBufferProcessor() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_TerminateBufferProcessor" ); }
void PaUtil_TerminateStreamRepresentation() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_TerminateStreamRepresentation" ); }
void PaUtil_TraceStubToSatisfyLinker() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_TraceStubToSatisfyLinker" ); }
void PaUtil_ValidateStreamPointer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_ValidateStreamPointer" ); }
void PaUtil_WriteRingBuffer() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_WriteRingBuffer" ); }
void PaUtil_ZeroOutput() { Dylib::portaudio.UninitializedFunctionExport( "PaUtil_ZeroOutput" ); }
void Pa_AbortStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_AbortStream" ); }
void Pa_CloseStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_CloseStream" ); }
void Pa_GetDefaultHostApi() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetDefaultHostApi" ); }
void Pa_GetDefaultInputDevice() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetDefaultInputDevice" ); }
void Pa_GetDefaultOutputDevice() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetDefaultOutputDevice" ); }
void Pa_GetDeviceCount() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetDeviceCount" ); }
void Pa_GetDeviceInfo() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetDeviceInfo" ); }
void Pa_GetErrorText() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetErrorText" ); }
void Pa_GetHostApiCount() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetHostApiCount" ); }
void Pa_GetHostApiInfo() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetHostApiInfo" ); }
void Pa_GetLastHostErrorInfo() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetLastHostErrorInfo" ); }
void Pa_GetSampleSize() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetSampleSize" ); }
void Pa_GetStreamCpuLoad() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetStreamCpuLoad" ); }
void Pa_GetStreamInfo() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetStreamInfo" ); }
void Pa_GetStreamReadAvailable() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetStreamReadAvailable" ); }
void Pa_GetStreamTime() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetStreamTime" ); }
void Pa_GetStreamWriteAvailable() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetStreamWriteAvailable" ); }
void Pa_GetVersion() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetVersion" ); }
void Pa_GetVersionInfo() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetVersionInfo" ); }
void Pa_GetVersionText() { Dylib::portaudio.UninitializedFunctionExport( "Pa_GetVersionText" ); }
void Pa_HostApiDeviceIndexToDeviceIndex() { Dylib::portaudio.UninitializedFunctionExport( "Pa_HostApiDeviceIndexToDeviceIndex" ); }
void Pa_HostApiTypeIdToHostApiIndex() { Dylib::portaudio.UninitializedFunctionExport( "Pa_HostApiTypeIdToHostApiIndex" ); }
void Pa_Initialize() { Dylib::portaudio.UninitializedFunctionExport( "Pa_Initialize" ); }
void Pa_IsFormatSupported() { Dylib::portaudio.UninitializedFunctionExport( "Pa_IsFormatSupported" ); }
void Pa_IsStreamActive() { Dylib::portaudio.UninitializedFunctionExport( "Pa_IsStreamActive" ); }
void Pa_IsStreamStopped() { Dylib::portaudio.UninitializedFunctionExport( "Pa_IsStreamStopped" ); }
void Pa_OpenDefaultStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_OpenDefaultStream" ); }
void Pa_OpenStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_OpenStream" ); }
void Pa_ReadStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_ReadStream" ); }
void Pa_SetStreamFinishedCallback() { Dylib::portaudio.UninitializedFunctionExport( "Pa_SetStreamFinishedCallback" ); }
void Pa_Sleep() { Dylib::portaudio.UninitializedFunctionExport( "Pa_Sleep" ); }
void Pa_StartStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_StartStream" ); }
void Pa_StopStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_StopStream" ); }
void Pa_Terminate() { Dylib::portaudio.UninitializedFunctionExport( "Pa_Terminate" ); }
void Pa_WriteStream() { Dylib::portaudio.UninitializedFunctionExport( "Pa_WriteStream" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "PaUtil_AdvanceRingBufferReadIndex", (void**)&PaUtil_AdvanceRingBufferReadIndex, Dylib::Import::functionStub },
  { "PaUtil_AdvanceRingBufferWriteIndex", (void**)&PaUtil_AdvanceRingBufferWriteIndex, Dylib::Import::functionStub },
  { "PaUtil_AllocateMemory", (void**)&PaUtil_AllocateMemory, Dylib::Import::functionStub },
  { "PaUtil_BeginBufferProcessing", (void**)&PaUtil_BeginBufferProcessing, Dylib::Import::functionStub },
  { "PaUtil_BeginCpuLoadMeasurement", (void**)&PaUtil_BeginCpuLoadMeasurement, Dylib::Import::functionStub },
  { "PaUtil_CopyInput", (void**)&PaUtil_CopyInput, Dylib::Import::functionStub },
  { "PaUtil_CopyOutput", (void**)&PaUtil_CopyOutput, Dylib::Import::functionStub },
  { "PaUtil_CountCurrentlyAllocatedBlocks", (void**)&PaUtil_CountCurrentlyAllocatedBlocks, Dylib::Import::functionStub },
  { "PaUtil_CreateAllocationGroup", (void**)&PaUtil_CreateAllocationGroup, Dylib::Import::functionStub },
  { "PaUtil_DebugPrint", (void**)&PaUtil_DebugPrint, Dylib::Import::functionStub },
  { "PaUtil_DestroyAllocationGroup", (void**)&PaUtil_DestroyAllocationGroup, Dylib::Import::functionStub },
  { "PaUtil_DeviceIndexToHostApiDeviceIndex", (void**)&PaUtil_DeviceIndexToHostApiDeviceIndex, Dylib::Import::functionStub },
  { "PaUtil_DummyGetCpuLoad", (void**)&PaUtil_DummyGetCpuLoad, Dylib::Import::functionStub },
  { "PaUtil_DummyGetReadAvailable", (void**)&PaUtil_DummyGetReadAvailable, Dylib::Import::functionStub },
  { "PaUtil_DummyGetWriteAvailable", (void**)&PaUtil_DummyGetWriteAvailable, Dylib::Import::functionStub },
  { "PaUtil_DummyRead", (void**)&PaUtil_DummyRead, Dylib::Import::functionStub },
  { "PaUtil_DummyWrite", (void**)&PaUtil_DummyWrite, Dylib::Import::functionStub },
  { "PaUtil_EndBufferProcessing", (void**)&PaUtil_EndBufferProcessing, Dylib::Import::functionStub },
  { "PaUtil_EndCpuLoadMeasurement", (void**)&PaUtil_EndCpuLoadMeasurement, Dylib::Import::functionStub },
  { "PaUtil_FlushRingBuffer", (void**)&PaUtil_FlushRingBuffer, Dylib::Import::functionStub },
  { "PaUtil_FreeAllAllocations", (void**)&PaUtil_FreeAllAllocations, Dylib::Import::functionStub },
  { "PaUtil_FreeMemory", (void**)&PaUtil_FreeMemory, Dylib::Import::functionStub },
  { "PaUtil_Generate16BitTriangularDither", (void**)&PaUtil_Generate16BitTriangularDither, Dylib::Import::functionStub },
  { "PaUtil_GenerateFloatTriangularDither", (void**)&PaUtil_GenerateFloatTriangularDither, Dylib::Import::functionStub },
  { "PaUtil_GetBufferProcessorInputLatencyFrames", (void**)&PaUtil_GetBufferProcessorInputLatencyFrames, Dylib::Import::functionStub },
  { "PaUtil_GetBufferProcessorOutputLatencyFrames", (void**)&PaUtil_GetBufferProcessorOutputLatencyFrames, Dylib::Import::functionStub },
  { "PaUtil_GetCpuLoad", (void**)&PaUtil_GetCpuLoad, Dylib::Import::functionStub },
  { "PaUtil_GetHostApiRepresentation", (void**)&PaUtil_GetHostApiRepresentation, Dylib::Import::functionStub },
  { "PaUtil_GetOutputFrameCount", (void**)&PaUtil_GetOutputFrameCount, Dylib::Import::functionStub },
  { "PaUtil_GetRingBufferReadAvailable", (void**)&PaUtil_GetRingBufferReadAvailable, Dylib::Import::functionStub },
  { "PaUtil_GetRingBufferReadRegions", (void**)&PaUtil_GetRingBufferReadRegions, Dylib::Import::functionStub },
  { "PaUtil_GetRingBufferWriteAvailable", (void**)&PaUtil_GetRingBufferWriteAvailable, Dylib::Import::functionStub },
  { "PaUtil_GetRingBufferWriteRegions", (void**)&PaUtil_GetRingBufferWriteRegions, Dylib::Import::functionStub },
  { "PaUtil_GetTime", (void**)&PaUtil_GetTime, Dylib::Import::functionStub },
  { "PaUtil_GroupAllocateMemory", (void**)&PaUtil_GroupAllocateMemory, Dylib::Import::functionStub },
  { "PaUtil_GroupFreeMemory", (void**)&PaUtil_GroupFreeMemory, Dylib::Import::functionStub },
  { "PaUtil_InitializeBufferProcessor", (void**)&PaUtil_InitializeBufferProcessor, Dylib::Import::functionStub },
  { "PaUtil_InitializeClock", (void**)&PaUtil_InitializeClock, Dylib::Import::functionStub },
  { "PaUtil_InitializeCpuLoadMeasurer", (void**)&PaUtil_InitializeCpuLoadMeasurer, Dylib::Import::functionStub },
  { "PaUtil_InitializeRingBuffer", (void**)&PaUtil_InitializeRingBuffer, Dylib::Import::functionStub },
  { "PaUtil_InitializeStreamInterface", (void**)&PaUtil_InitializeStreamInterface, Dylib::Import::functionStub },
  { "PaUtil_InitializeStreamRepresentation", (void**)&PaUtil_InitializeStreamRepresentation, Dylib::Import::functionStub },
  { "PaUtil_InitializeTriangularDitherState", (void**)&PaUtil_InitializeTriangularDitherState, Dylib::Import::functionStub },
  { "PaUtil_IsBufferProcessorOutputEmpty", (void**)&PaUtil_IsBufferProcessorOutputEmpty, Dylib::Import::functionStub },
  { "PaUtil_ReadRingBuffer", (void**)&PaUtil_ReadRingBuffer, Dylib::Import::functionStub },
  { "PaUtil_ResetBufferProcessor", (void**)&PaUtil_ResetBufferProcessor, Dylib::Import::functionStub },
  { "PaUtil_ResetCpuLoadMeasurer", (void**)&PaUtil_ResetCpuLoadMeasurer, Dylib::Import::functionStub },
  { "PaUtil_SelectClosestAvailableFormat", (void**)&PaUtil_SelectClosestAvailableFormat, Dylib::Import::functionStub },
  { "PaUtil_SelectConverter", (void**)&PaUtil_SelectConverter, Dylib::Import::functionStub },
  { "PaUtil_SelectZeroer", (void**)&PaUtil_SelectZeroer, Dylib::Import::functionStub },
  { "PaUtil_Set2ndInputChannel", (void**)&PaUtil_Set2ndInputChannel, Dylib::Import::functionStub },
  { "PaUtil_Set2ndInputFrameCount", (void**)&PaUtil_Set2ndInputFrameCount, Dylib::Import::functionStub },
  { "PaUtil_Set2ndInterleavedInputChannels", (void**)&PaUtil_Set2ndInterleavedInputChannels, Dylib::Import::functionStub },
  { "PaUtil_Set2ndInterleavedOutputChannels", (void**)&PaUtil_Set2ndInterleavedOutputChannels, Dylib::Import::functionStub },
  { "PaUtil_Set2ndNonInterleavedInputChannel", (void**)&PaUtil_Set2ndNonInterleavedInputChannel, Dylib::Import::functionStub },
  { "PaUtil_Set2ndNonInterleavedOutputChannel", (void**)&PaUtil_Set2ndNonInterleavedOutputChannel, Dylib::Import::functionStub },
  { "PaUtil_Set2ndOutputChannel", (void**)&PaUtil_Set2ndOutputChannel, Dylib::Import::functionStub },
  { "PaUtil_Set2ndOutputFrameCount", (void**)&PaUtil_Set2ndOutputFrameCount, Dylib::Import::functionStub },
  { "PaUtil_SetDebugPrintFunction", (void**)&PaUtil_SetDebugPrintFunction, Dylib::Import::functionStub },
  { "PaUtil_SetInputChannel", (void**)&PaUtil_SetInputChannel, Dylib::Import::functionStub },
  { "PaUtil_SetInputFrameCount", (void**)&PaUtil_SetInputFrameCount, Dylib::Import::functionStub },
  { "PaUtil_SetInterleavedInputChannels", (void**)&PaUtil_SetInterleavedInputChannels, Dylib::Import::functionStub },
  { "PaUtil_SetInterleavedOutputChannels", (void**)&PaUtil_SetInterleavedOutputChannels, Dylib::Import::functionStub },
  { "PaUtil_SetLastHostErrorInfo", (void**)&PaUtil_SetLastHostErrorInfo, Dylib::Import::functionStub },
  { "PaUtil_SetNoInput", (void**)&PaUtil_SetNoInput, Dylib::Import::functionStub },
  { "PaUtil_SetNoOutput", (void**)&PaUtil_SetNoOutput, Dylib::Import::functionStub },
  { "PaUtil_SetNonInterleavedInputChannel", (void**)&PaUtil_SetNonInterleavedInputChannel, Dylib::Import::functionStub },
  { "PaUtil_SetNonInterleavedOutputChannel", (void**)&PaUtil_SetNonInterleavedOutputChannel, Dylib::Import::functionStub },
  { "PaUtil_SetOutputChannel", (void**)&PaUtil_SetOutputChannel, Dylib::Import::functionStub },
  { "PaUtil_SetOutputFrameCount", (void**)&PaUtil_SetOutputFrameCount, Dylib::Import::functionStub },
  { "PaUtil_TerminateBufferProcessor", (void**)&PaUtil_TerminateBufferProcessor, Dylib::Import::functionStub },
  { "PaUtil_TerminateStreamRepresentation", (void**)&PaUtil_TerminateStreamRepresentation, Dylib::Import::functionStub },
  { "PaUtil_TraceStubToSatisfyLinker", (void**)&PaUtil_TraceStubToSatisfyLinker, Dylib::Import::functionStub },
  { "PaUtil_ValidateStreamPointer", (void**)&PaUtil_ValidateStreamPointer, Dylib::Import::functionStub },
  { "PaUtil_WriteRingBuffer", (void**)&PaUtil_WriteRingBuffer, Dylib::Import::functionStub },
  { "PaUtil_ZeroOutput", (void**)&PaUtil_ZeroOutput, Dylib::Import::functionStub },
  { "Pa_AbortStream", (void**)&Pa_AbortStream, Dylib::Import::functionStub },
  { "Pa_CloseStream", (void**)&Pa_CloseStream, Dylib::Import::functionStub },
  { "Pa_GetDefaultHostApi", (void**)&Pa_GetDefaultHostApi, Dylib::Import::functionStub },
  { "Pa_GetDefaultInputDevice", (void**)&Pa_GetDefaultInputDevice, Dylib::Import::functionStub },
  { "Pa_GetDefaultOutputDevice", (void**)&Pa_GetDefaultOutputDevice, Dylib::Import::functionStub },
  { "Pa_GetDeviceCount", (void**)&Pa_GetDeviceCount, Dylib::Import::functionStub },
  { "Pa_GetDeviceInfo", (void**)&Pa_GetDeviceInfo, Dylib::Import::functionStub },
  { "Pa_GetErrorText", (void**)&Pa_GetErrorText, Dylib::Import::functionStub },
  { "Pa_GetHostApiCount", (void**)&Pa_GetHostApiCount, Dylib::Import::functionStub },
  { "Pa_GetHostApiInfo", (void**)&Pa_GetHostApiInfo, Dylib::Import::functionStub },
  { "Pa_GetLastHostErrorInfo", (void**)&Pa_GetLastHostErrorInfo, Dylib::Import::functionStub },
  { "Pa_GetSampleSize", (void**)&Pa_GetSampleSize, Dylib::Import::functionStub },
  { "Pa_GetStreamCpuLoad", (void**)&Pa_GetStreamCpuLoad, Dylib::Import::functionStub },
  { "Pa_GetStreamInfo", (void**)&Pa_GetStreamInfo, Dylib::Import::functionStub },
  { "Pa_GetStreamReadAvailable", (void**)&Pa_GetStreamReadAvailable, Dylib::Import::functionStub },
  { "Pa_GetStreamTime", (void**)&Pa_GetStreamTime, Dylib::Import::functionStub },
  { "Pa_GetStreamWriteAvailable", (void**)&Pa_GetStreamWriteAvailable, Dylib::Import::functionStub },
  { "Pa_GetVersion", (void**)&Pa_GetVersion, Dylib::Import::functionStub },
  { "Pa_GetVersionInfo", (void**)&Pa_GetVersionInfo, Dylib::Import::functionStub },
  { "Pa_GetVersionText", (void**)&Pa_GetVersionText, Dylib::Import::functionStub },
  { "Pa_HostApiDeviceIndexToDeviceIndex", (void**)&Pa_HostApiDeviceIndexToDeviceIndex, Dylib::Import::functionStub },
  { "Pa_HostApiTypeIdToHostApiIndex", (void**)&Pa_HostApiTypeIdToHostApiIndex, Dylib::Import::functionStub },
  { "Pa_Initialize", (void**)&Pa_Initialize, Dylib::Import::functionStub },
  { "Pa_IsFormatSupported", (void**)&Pa_IsFormatSupported, Dylib::Import::functionStub },
  { "Pa_IsStreamActive", (void**)&Pa_IsStreamActive, Dylib::Import::functionStub },
  { "Pa_IsStreamStopped", (void**)&Pa_IsStreamStopped, Dylib::Import::functionStub },
  { "Pa_OpenDefaultStream", (void**)&Pa_OpenDefaultStream, Dylib::Import::functionStub },
  { "Pa_OpenStream", (void**)&Pa_OpenStream, Dylib::Import::functionStub },
  { "Pa_ReadStream", (void**)&Pa_ReadStream, Dylib::Import::functionStub },
  { "Pa_SetStreamFinishedCallback", (void**)&Pa_SetStreamFinishedCallback, Dylib::Import::functionStub },
  { "Pa_Sleep", (void**)&Pa_Sleep, Dylib::Import::functionStub },
  { "Pa_StartStream", (void**)&Pa_StartStream, Dylib::Import::functionStub },
  { "Pa_StopStream", (void**)&Pa_StopStream, Dylib::Import::functionStub },
  { "Pa_Terminate", (void**)&Pa_Terminate, Dylib::Import::functionStub },
  { "Pa_WriteStream", (void**)&Pa_WriteStream, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBPORTAUDIO

