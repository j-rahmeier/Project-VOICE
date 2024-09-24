// Import declarations for avfilter-10 library
// generated Tue May 21 14:46:16 2024 by DylibTool
#if STATIC_LIBAVFILTER_10

namespace Dylib { bool avfilter_10_Loaded() { return true; } }
extern "C" {
void* p_av_filter_ffversion = &av_filter_ffversion;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( avfilter_10, "avfilter-10", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_av_filter_ffversion = Dylib::avfilter_10.ResolveDataExport( "av_filter_ffversion" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void av_buffersink_get_ch_layout() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_ch_layout" ); }
void av_buffersink_get_channels() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_channels" ); }
void av_buffersink_get_color_range() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_color_range" ); }
void av_buffersink_get_colorspace() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_colorspace" ); }
void av_buffersink_get_format() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_format" ); }
void av_buffersink_get_frame() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_frame" ); }
void av_buffersink_get_frame_flags() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_frame_flags" ); }
void av_buffersink_get_frame_rate() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_frame_rate" ); }
void av_buffersink_get_h() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_h" ); }
void av_buffersink_get_hw_frames_ctx() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_hw_frames_ctx" ); }
void av_buffersink_get_sample_aspect_ratio() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_sample_aspect_ratio" ); }
void av_buffersink_get_sample_rate() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_sample_rate" ); }
void av_buffersink_get_samples() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_samples" ); }
void av_buffersink_get_time_base() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_time_base" ); }
void av_buffersink_get_type() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_type" ); }
void av_buffersink_get_w() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_get_w" ); }
void av_buffersink_set_frame_size() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersink_set_frame_size" ); }
void av_buffersrc_add_frame() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_add_frame" ); }
void av_buffersrc_add_frame_flags() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_add_frame_flags" ); }
void av_buffersrc_close() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_close" ); }
void av_buffersrc_get_nb_failed_requests() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_get_nb_failed_requests" ); }
void av_buffersrc_parameters_alloc() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_parameters_alloc" ); }
void av_buffersrc_parameters_set() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_parameters_set" ); }
void av_buffersrc_write_frame() { Dylib::avfilter_10.UninitializedFunctionExport( "av_buffersrc_write_frame" ); }
void av_filter_iterate() { Dylib::avfilter_10.UninitializedFunctionExport( "av_filter_iterate" ); }
void avfilter_config_links() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_config_links" ); }
void avfilter_configuration() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_configuration" ); }
void avfilter_filter_pad_count() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_filter_pad_count" ); }
void avfilter_free() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_free" ); }
void avfilter_get_by_name() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_get_by_name" ); }
void avfilter_get_class() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_get_class" ); }
void avfilter_graph_alloc() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_alloc" ); }
void avfilter_graph_alloc_filter() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_alloc_filter" ); }
void avfilter_graph_config() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_config" ); }
void avfilter_graph_create_filter() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_create_filter" ); }
void avfilter_graph_dump() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_dump" ); }
void avfilter_graph_free() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_free" ); }
void avfilter_graph_get_filter() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_get_filter" ); }
void avfilter_graph_parse() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_parse" ); }
void avfilter_graph_parse2() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_parse2" ); }
void avfilter_graph_parse_ptr() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_parse_ptr" ); }
void avfilter_graph_queue_command() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_queue_command" ); }
void avfilter_graph_request_oldest() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_request_oldest" ); }
void avfilter_graph_segment_apply() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_apply" ); }
void avfilter_graph_segment_apply_opts() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_apply_opts" ); }
void avfilter_graph_segment_create_filters() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_create_filters" ); }
void avfilter_graph_segment_free() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_free" ); }
void avfilter_graph_segment_init() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_init" ); }
void avfilter_graph_segment_link() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_link" ); }
void avfilter_graph_segment_parse() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_segment_parse" ); }
void avfilter_graph_send_command() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_send_command" ); }
void avfilter_graph_set_auto_convert() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_graph_set_auto_convert" ); }
void avfilter_init_dict() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_init_dict" ); }
void avfilter_init_str() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_init_str" ); }
void avfilter_inout_alloc() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_inout_alloc" ); }
void avfilter_inout_free() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_inout_free" ); }
void avfilter_insert_filter() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_insert_filter" ); }
void avfilter_license() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_license" ); }
void avfilter_link() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_link" ); }
void avfilter_link_free() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_link_free" ); }
void avfilter_pad_get_name() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_pad_get_name" ); }
void avfilter_pad_get_type() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_pad_get_type" ); }
void avfilter_process_command() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_process_command" ); }
void avfilter_version() { Dylib::avfilter_10.UninitializedFunctionExport( "avfilter_version" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "av_buffersink_get_ch_layout", (void**)&av_buffersink_get_ch_layout, Dylib::Import::functionStub },
  { "av_buffersink_get_channels", (void**)&av_buffersink_get_channels, Dylib::Import::functionStub },
  { "av_buffersink_get_color_range", (void**)&av_buffersink_get_color_range, Dylib::Import::functionStub },
  { "av_buffersink_get_colorspace", (void**)&av_buffersink_get_colorspace, Dylib::Import::functionStub },
  { "av_buffersink_get_format", (void**)&av_buffersink_get_format, Dylib::Import::functionStub },
  { "av_buffersink_get_frame", (void**)&av_buffersink_get_frame, Dylib::Import::functionStub },
  { "av_buffersink_get_frame_flags", (void**)&av_buffersink_get_frame_flags, Dylib::Import::functionStub },
  { "av_buffersink_get_frame_rate", (void**)&av_buffersink_get_frame_rate, Dylib::Import::functionStub },
  { "av_buffersink_get_h", (void**)&av_buffersink_get_h, Dylib::Import::functionStub },
  { "av_buffersink_get_hw_frames_ctx", (void**)&av_buffersink_get_hw_frames_ctx, Dylib::Import::functionStub },
  { "av_buffersink_get_sample_aspect_ratio", (void**)&av_buffersink_get_sample_aspect_ratio, Dylib::Import::functionStub },
  { "av_buffersink_get_sample_rate", (void**)&av_buffersink_get_sample_rate, Dylib::Import::functionStub },
  { "av_buffersink_get_samples", (void**)&av_buffersink_get_samples, Dylib::Import::functionStub },
  { "av_buffersink_get_time_base", (void**)&av_buffersink_get_time_base, Dylib::Import::functionStub },
  { "av_buffersink_get_type", (void**)&av_buffersink_get_type, Dylib::Import::functionStub },
  { "av_buffersink_get_w", (void**)&av_buffersink_get_w, Dylib::Import::functionStub },
  { "av_buffersink_set_frame_size", (void**)&av_buffersink_set_frame_size, Dylib::Import::functionStub },
  { "av_buffersrc_add_frame", (void**)&av_buffersrc_add_frame, Dylib::Import::functionStub },
  { "av_buffersrc_add_frame_flags", (void**)&av_buffersrc_add_frame_flags, Dylib::Import::functionStub },
  { "av_buffersrc_close", (void**)&av_buffersrc_close, Dylib::Import::functionStub },
  { "av_buffersrc_get_nb_failed_requests", (void**)&av_buffersrc_get_nb_failed_requests, Dylib::Import::functionStub },
  { "av_buffersrc_parameters_alloc", (void**)&av_buffersrc_parameters_alloc, Dylib::Import::functionStub },
  { "av_buffersrc_parameters_set", (void**)&av_buffersrc_parameters_set, Dylib::Import::functionStub },
  { "av_buffersrc_write_frame", (void**)&av_buffersrc_write_frame, Dylib::Import::functionStub },
  { "av_filter_iterate", (void**)&av_filter_iterate, Dylib::Import::functionStub },
  { "avfilter_config_links", (void**)&avfilter_config_links, Dylib::Import::functionStub },
  { "avfilter_configuration", (void**)&avfilter_configuration, Dylib::Import::functionStub },
  { "avfilter_filter_pad_count", (void**)&avfilter_filter_pad_count, Dylib::Import::functionStub },
  { "avfilter_free", (void**)&avfilter_free, Dylib::Import::functionStub },
  { "avfilter_get_by_name", (void**)&avfilter_get_by_name, Dylib::Import::functionStub },
  { "avfilter_get_class", (void**)&avfilter_get_class, Dylib::Import::functionStub },
  { "avfilter_graph_alloc", (void**)&avfilter_graph_alloc, Dylib::Import::functionStub },
  { "avfilter_graph_alloc_filter", (void**)&avfilter_graph_alloc_filter, Dylib::Import::functionStub },
  { "avfilter_graph_config", (void**)&avfilter_graph_config, Dylib::Import::functionStub },
  { "avfilter_graph_create_filter", (void**)&avfilter_graph_create_filter, Dylib::Import::functionStub },
  { "avfilter_graph_dump", (void**)&avfilter_graph_dump, Dylib::Import::functionStub },
  { "avfilter_graph_free", (void**)&avfilter_graph_free, Dylib::Import::functionStub },
  { "avfilter_graph_get_filter", (void**)&avfilter_graph_get_filter, Dylib::Import::functionStub },
  { "avfilter_graph_parse", (void**)&avfilter_graph_parse, Dylib::Import::functionStub },
  { "avfilter_graph_parse2", (void**)&avfilter_graph_parse2, Dylib::Import::functionStub },
  { "avfilter_graph_parse_ptr", (void**)&avfilter_graph_parse_ptr, Dylib::Import::functionStub },
  { "avfilter_graph_queue_command", (void**)&avfilter_graph_queue_command, Dylib::Import::functionStub },
  { "avfilter_graph_request_oldest", (void**)&avfilter_graph_request_oldest, Dylib::Import::functionStub },
  { "avfilter_graph_segment_apply", (void**)&avfilter_graph_segment_apply, Dylib::Import::functionStub },
  { "avfilter_graph_segment_apply_opts", (void**)&avfilter_graph_segment_apply_opts, Dylib::Import::functionStub },
  { "avfilter_graph_segment_create_filters", (void**)&avfilter_graph_segment_create_filters, Dylib::Import::functionStub },
  { "avfilter_graph_segment_free", (void**)&avfilter_graph_segment_free, Dylib::Import::functionStub },
  { "avfilter_graph_segment_init", (void**)&avfilter_graph_segment_init, Dylib::Import::functionStub },
  { "avfilter_graph_segment_link", (void**)&avfilter_graph_segment_link, Dylib::Import::functionStub },
  { "avfilter_graph_segment_parse", (void**)&avfilter_graph_segment_parse, Dylib::Import::functionStub },
  { "avfilter_graph_send_command", (void**)&avfilter_graph_send_command, Dylib::Import::functionStub },
  { "avfilter_graph_set_auto_convert", (void**)&avfilter_graph_set_auto_convert, Dylib::Import::functionStub },
  { "avfilter_init_dict", (void**)&avfilter_init_dict, Dylib::Import::functionStub },
  { "avfilter_init_str", (void**)&avfilter_init_str, Dylib::Import::functionStub },
  { "avfilter_inout_alloc", (void**)&avfilter_inout_alloc, Dylib::Import::functionStub },
  { "avfilter_inout_free", (void**)&avfilter_inout_free, Dylib::Import::functionStub },
  { "avfilter_insert_filter", (void**)&avfilter_insert_filter, Dylib::Import::functionStub },
  { "avfilter_license", (void**)&avfilter_license, Dylib::Import::functionStub },
  { "avfilter_link", (void**)&avfilter_link, Dylib::Import::functionStub },
  { "avfilter_link_free", (void**)&avfilter_link_free, Dylib::Import::functionStub },
  { "avfilter_pad_get_name", (void**)&avfilter_pad_get_name, Dylib::Import::functionStub },
  { "avfilter_pad_get_type", (void**)&avfilter_pad_get_type, Dylib::Import::functionStub },
  { "avfilter_process_command", (void**)&avfilter_process_command, Dylib::Import::functionStub },
  { "avfilter_version", (void**)&avfilter_version, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBAVFILTER_10

