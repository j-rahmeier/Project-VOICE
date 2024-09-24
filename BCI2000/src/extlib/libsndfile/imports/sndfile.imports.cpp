// Import declarations for sndfile library
// generated Thu May  5 18:29:51 2022 by DylibTool
#if STATIC_LIBSNDFILE

namespace Dylib { bool sndfile_Loaded() { return true; } }
extern "C" {
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( sndfile, "sndfile", functionImports, notFoundMsg, notFoundURL );

extern "C" {

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void sf_close() { Dylib::sndfile.UninitializedFunctionExport( "sf_close" ); }
void sf_command() { Dylib::sndfile.UninitializedFunctionExport( "sf_command" ); }
void sf_current_byterate() { Dylib::sndfile.UninitializedFunctionExport( "sf_current_byterate" ); }
void sf_error() { Dylib::sndfile.UninitializedFunctionExport( "sf_error" ); }
void sf_error_number() { Dylib::sndfile.UninitializedFunctionExport( "sf_error_number" ); }
void sf_error_str() { Dylib::sndfile.UninitializedFunctionExport( "sf_error_str" ); }
void sf_format_check() { Dylib::sndfile.UninitializedFunctionExport( "sf_format_check" ); }
void sf_get_chunk_data() { Dylib::sndfile.UninitializedFunctionExport( "sf_get_chunk_data" ); }
void sf_get_chunk_iterator() { Dylib::sndfile.UninitializedFunctionExport( "sf_get_chunk_iterator" ); }
void sf_get_chunk_size() { Dylib::sndfile.UninitializedFunctionExport( "sf_get_chunk_size" ); }
void sf_get_string() { Dylib::sndfile.UninitializedFunctionExport( "sf_get_string" ); }
void sf_next_chunk_iterator() { Dylib::sndfile.UninitializedFunctionExport( "sf_next_chunk_iterator" ); }
void sf_open() { Dylib::sndfile.UninitializedFunctionExport( "sf_open" ); }
void sf_open_fd() { Dylib::sndfile.UninitializedFunctionExport( "sf_open_fd" ); }
void sf_open_virtual() { Dylib::sndfile.UninitializedFunctionExport( "sf_open_virtual" ); }
void sf_perror() { Dylib::sndfile.UninitializedFunctionExport( "sf_perror" ); }
void sf_read_double() { Dylib::sndfile.UninitializedFunctionExport( "sf_read_double" ); }
void sf_read_float() { Dylib::sndfile.UninitializedFunctionExport( "sf_read_float" ); }
void sf_read_int() { Dylib::sndfile.UninitializedFunctionExport( "sf_read_int" ); }
void sf_read_raw() { Dylib::sndfile.UninitializedFunctionExport( "sf_read_raw" ); }
void sf_read_short() { Dylib::sndfile.UninitializedFunctionExport( "sf_read_short" ); }
void sf_readf_double() { Dylib::sndfile.UninitializedFunctionExport( "sf_readf_double" ); }
void sf_readf_float() { Dylib::sndfile.UninitializedFunctionExport( "sf_readf_float" ); }
void sf_readf_int() { Dylib::sndfile.UninitializedFunctionExport( "sf_readf_int" ); }
void sf_readf_short() { Dylib::sndfile.UninitializedFunctionExport( "sf_readf_short" ); }
void sf_seek() { Dylib::sndfile.UninitializedFunctionExport( "sf_seek" ); }
void sf_set_chunk() { Dylib::sndfile.UninitializedFunctionExport( "sf_set_chunk" ); }
void sf_set_string() { Dylib::sndfile.UninitializedFunctionExport( "sf_set_string" ); }
void sf_strerror() { Dylib::sndfile.UninitializedFunctionExport( "sf_strerror" ); }
void sf_version_string() { Dylib::sndfile.UninitializedFunctionExport( "sf_version_string" ); }
void sf_wchar_open() { Dylib::sndfile.UninitializedFunctionExport( "sf_wchar_open" ); }
void sf_write_double() { Dylib::sndfile.UninitializedFunctionExport( "sf_write_double" ); }
void sf_write_float() { Dylib::sndfile.UninitializedFunctionExport( "sf_write_float" ); }
void sf_write_int() { Dylib::sndfile.UninitializedFunctionExport( "sf_write_int" ); }
void sf_write_raw() { Dylib::sndfile.UninitializedFunctionExport( "sf_write_raw" ); }
void sf_write_short() { Dylib::sndfile.UninitializedFunctionExport( "sf_write_short" ); }
void sf_write_sync() { Dylib::sndfile.UninitializedFunctionExport( "sf_write_sync" ); }
void sf_writef_double() { Dylib::sndfile.UninitializedFunctionExport( "sf_writef_double" ); }
void sf_writef_float() { Dylib::sndfile.UninitializedFunctionExport( "sf_writef_float" ); }
void sf_writef_int() { Dylib::sndfile.UninitializedFunctionExport( "sf_writef_int" ); }
void sf_writef_short() { Dylib::sndfile.UninitializedFunctionExport( "sf_writef_short" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "sf_close", (void**)&sf_close, Dylib::Import::functionStub },
  { "sf_command", (void**)&sf_command, Dylib::Import::functionStub },
  { "sf_current_byterate", (void**)&sf_current_byterate, Dylib::Import::functionStub },
  { "sf_error", (void**)&sf_error, Dylib::Import::functionStub },
  { "sf_error_number", (void**)&sf_error_number, Dylib::Import::functionStub },
  { "sf_error_str", (void**)&sf_error_str, Dylib::Import::functionStub },
  { "sf_format_check", (void**)&sf_format_check, Dylib::Import::functionStub },
  { "sf_get_chunk_data", (void**)&sf_get_chunk_data, Dylib::Import::functionStub },
  { "sf_get_chunk_iterator", (void**)&sf_get_chunk_iterator, Dylib::Import::functionStub },
  { "sf_get_chunk_size", (void**)&sf_get_chunk_size, Dylib::Import::functionStub },
  { "sf_get_string", (void**)&sf_get_string, Dylib::Import::functionStub },
  { "sf_next_chunk_iterator", (void**)&sf_next_chunk_iterator, Dylib::Import::functionStub },
  { "sf_open", (void**)&sf_open, Dylib::Import::functionStub },
  { "sf_open_fd", (void**)&sf_open_fd, Dylib::Import::functionStub },
  { "sf_open_virtual", (void**)&sf_open_virtual, Dylib::Import::functionStub },
  { "sf_perror", (void**)&sf_perror, Dylib::Import::functionStub },
  { "sf_read_double", (void**)&sf_read_double, Dylib::Import::functionStub },
  { "sf_read_float", (void**)&sf_read_float, Dylib::Import::functionStub },
  { "sf_read_int", (void**)&sf_read_int, Dylib::Import::functionStub },
  { "sf_read_raw", (void**)&sf_read_raw, Dylib::Import::functionStub },
  { "sf_read_short", (void**)&sf_read_short, Dylib::Import::functionStub },
  { "sf_readf_double", (void**)&sf_readf_double, Dylib::Import::functionStub },
  { "sf_readf_float", (void**)&sf_readf_float, Dylib::Import::functionStub },
  { "sf_readf_int", (void**)&sf_readf_int, Dylib::Import::functionStub },
  { "sf_readf_short", (void**)&sf_readf_short, Dylib::Import::functionStub },
  { "sf_seek", (void**)&sf_seek, Dylib::Import::functionStub },
  { "sf_set_chunk", (void**)&sf_set_chunk, Dylib::Import::functionStub },
  { "sf_set_string", (void**)&sf_set_string, Dylib::Import::functionStub },
  { "sf_strerror", (void**)&sf_strerror, Dylib::Import::functionStub },
  { "sf_version_string", (void**)&sf_version_string, Dylib::Import::functionStub },
  { "sf_wchar_open", (void**)&sf_wchar_open, Dylib::Import::functionStub },
  { "sf_write_double", (void**)&sf_write_double, Dylib::Import::functionStub },
  { "sf_write_float", (void**)&sf_write_float, Dylib::Import::functionStub },
  { "sf_write_int", (void**)&sf_write_int, Dylib::Import::functionStub },
  { "sf_write_raw", (void**)&sf_write_raw, Dylib::Import::functionStub },
  { "sf_write_short", (void**)&sf_write_short, Dylib::Import::functionStub },
  { "sf_write_sync", (void**)&sf_write_sync, Dylib::Import::functionStub },
  { "sf_writef_double", (void**)&sf_writef_double, Dylib::Import::functionStub },
  { "sf_writef_float", (void**)&sf_writef_float, Dylib::Import::functionStub },
  { "sf_writef_int", (void**)&sf_writef_int, Dylib::Import::functionStub },
  { "sf_writef_short", (void**)&sf_writef_short, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBSNDFILE

