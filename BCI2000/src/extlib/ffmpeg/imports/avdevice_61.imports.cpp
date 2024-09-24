// Import declarations for avdevice-61 library
// generated Tue May 21 14:46:07 2024 by DylibTool
#if STATIC_LIBAVDEVICE_61

namespace Dylib { bool avdevice_61_Loaded() { return true; } }
extern "C" {
void* p_av_device_ffversion = &av_device_ffversion;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( avdevice_61, "avdevice-61", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_av_device_ffversion = Dylib::avdevice_61.ResolveDataExport( "av_device_ffversion" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void av_input_audio_device_next() { Dylib::avdevice_61.UninitializedFunctionExport( "av_input_audio_device_next" ); }
void av_input_video_device_next() { Dylib::avdevice_61.UninitializedFunctionExport( "av_input_video_device_next" ); }
void av_output_audio_device_next() { Dylib::avdevice_61.UninitializedFunctionExport( "av_output_audio_device_next" ); }
void av_output_video_device_next() { Dylib::avdevice_61.UninitializedFunctionExport( "av_output_video_device_next" ); }
void avdevice_app_to_dev_control_message() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_app_to_dev_control_message" ); }
void avdevice_configuration() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_configuration" ); }
void avdevice_dev_to_app_control_message() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_dev_to_app_control_message" ); }
void avdevice_free_list_devices() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_free_list_devices" ); }
void avdevice_license() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_license" ); }
void avdevice_list_devices() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_list_devices" ); }
void avdevice_list_input_sources() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_list_input_sources" ); }
void avdevice_list_output_sinks() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_list_output_sinks" ); }
void avdevice_register_all() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_register_all" ); }
void avdevice_version() { Dylib::avdevice_61.UninitializedFunctionExport( "avdevice_version" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "av_input_audio_device_next", (void**)&av_input_audio_device_next, Dylib::Import::functionStub },
  { "av_input_video_device_next", (void**)&av_input_video_device_next, Dylib::Import::functionStub },
  { "av_output_audio_device_next", (void**)&av_output_audio_device_next, Dylib::Import::functionStub },
  { "av_output_video_device_next", (void**)&av_output_video_device_next, Dylib::Import::functionStub },
  { "avdevice_app_to_dev_control_message", (void**)&avdevice_app_to_dev_control_message, Dylib::Import::functionStub },
  { "avdevice_configuration", (void**)&avdevice_configuration, Dylib::Import::functionStub },
  { "avdevice_dev_to_app_control_message", (void**)&avdevice_dev_to_app_control_message, Dylib::Import::functionStub },
  { "avdevice_free_list_devices", (void**)&avdevice_free_list_devices, Dylib::Import::functionStub },
  { "avdevice_license", (void**)&avdevice_license, Dylib::Import::functionStub },
  { "avdevice_list_devices", (void**)&avdevice_list_devices, Dylib::Import::functionStub },
  { "avdevice_list_input_sources", (void**)&avdevice_list_input_sources, Dylib::Import::functionStub },
  { "avdevice_list_output_sinks", (void**)&avdevice_list_output_sinks, Dylib::Import::functionStub },
  { "avdevice_register_all", (void**)&avdevice_register_all, Dylib::Import::functionStub },
  { "avdevice_version", (void**)&avdevice_version, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBAVDEVICE_61

