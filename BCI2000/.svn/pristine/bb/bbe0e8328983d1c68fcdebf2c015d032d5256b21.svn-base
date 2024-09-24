// Import declarations for postproc-58 library
// generated Tue May 21 14:47:03 2024 by DylibTool
#if STATIC_LIBPOSTPROC_58

namespace Dylib { bool postproc_58_Loaded() { return true; } }
extern "C" {
void* p_postproc_ffversion = &postproc_ffversion;
void* p_pp_help = &pp_help;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( postproc_58, "postproc-58", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_postproc_ffversion = Dylib::postproc_58.ResolveDataExport( "postproc_ffversion" );
void* p_pp_help = Dylib::postproc_58.ResolveDataExport( "pp_help" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void postproc_configuration() { Dylib::postproc_58.UninitializedFunctionExport( "postproc_configuration" ); }
void postproc_license() { Dylib::postproc_58.UninitializedFunctionExport( "postproc_license" ); }
void postproc_version() { Dylib::postproc_58.UninitializedFunctionExport( "postproc_version" ); }
void pp_free_context() { Dylib::postproc_58.UninitializedFunctionExport( "pp_free_context" ); }
void pp_free_mode() { Dylib::postproc_58.UninitializedFunctionExport( "pp_free_mode" ); }
void pp_get_context() { Dylib::postproc_58.UninitializedFunctionExport( "pp_get_context" ); }
void pp_get_mode_by_name_and_quality() { Dylib::postproc_58.UninitializedFunctionExport( "pp_get_mode_by_name_and_quality" ); }
void pp_postprocess() { Dylib::postproc_58.UninitializedFunctionExport( "pp_postprocess" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "postproc_configuration", (void**)&postproc_configuration, Dylib::Import::functionStub },
  { "postproc_license", (void**)&postproc_license, Dylib::Import::functionStub },
  { "postproc_version", (void**)&postproc_version, Dylib::Import::functionStub },
  { "pp_free_context", (void**)&pp_free_context, Dylib::Import::functionStub },
  { "pp_free_mode", (void**)&pp_free_mode, Dylib::Import::functionStub },
  { "pp_get_context", (void**)&pp_get_context, Dylib::Import::functionStub },
  { "pp_get_mode_by_name_and_quality", (void**)&pp_get_mode_by_name_and_quality, Dylib::Import::functionStub },
  { "pp_postprocess", (void**)&pp_postprocess, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBPOSTPROC_58

