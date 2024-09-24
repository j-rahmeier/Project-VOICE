// Import declarations for avformat-61 library
// generated Tue May 21 14:46:26 2024 by DylibTool
#if STATIC_LIBAVFORMAT_61

namespace Dylib { bool avformat_61_Loaded() { return true; } }
extern "C" {
void* p_av_format_ffversion = &av_format_ffversion;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( avformat_61, "avformat-61", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_av_format_ffversion = Dylib::avformat_61.ResolveDataExport( "av_format_ffversion" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void av_add_index_entry() { Dylib::avformat_61.UninitializedFunctionExport( "av_add_index_entry" ); }
void av_append_packet() { Dylib::avformat_61.UninitializedFunctionExport( "av_append_packet" ); }
void av_codec_get_id() { Dylib::avformat_61.UninitializedFunctionExport( "av_codec_get_id" ); }
void av_codec_get_tag() { Dylib::avformat_61.UninitializedFunctionExport( "av_codec_get_tag" ); }
void av_codec_get_tag2() { Dylib::avformat_61.UninitializedFunctionExport( "av_codec_get_tag2" ); }
void av_demuxer_iterate() { Dylib::avformat_61.UninitializedFunctionExport( "av_demuxer_iterate" ); }
void av_disposition_from_string() { Dylib::avformat_61.UninitializedFunctionExport( "av_disposition_from_string" ); }
void av_disposition_to_string() { Dylib::avformat_61.UninitializedFunctionExport( "av_disposition_to_string" ); }
void av_dump_format() { Dylib::avformat_61.UninitializedFunctionExport( "av_dump_format" ); }
void av_filename_number_test() { Dylib::avformat_61.UninitializedFunctionExport( "av_filename_number_test" ); }
void av_find_best_stream() { Dylib::avformat_61.UninitializedFunctionExport( "av_find_best_stream" ); }
void av_find_default_stream_index() { Dylib::avformat_61.UninitializedFunctionExport( "av_find_default_stream_index" ); }
void av_find_input_format() { Dylib::avformat_61.UninitializedFunctionExport( "av_find_input_format" ); }
void av_find_program_from_stream() { Dylib::avformat_61.UninitializedFunctionExport( "av_find_program_from_stream" ); }
void av_fmt_ctx_get_duration_estimation_method() { Dylib::avformat_61.UninitializedFunctionExport( "av_fmt_ctx_get_duration_estimation_method" ); }
void av_format_inject_global_side_data() { Dylib::avformat_61.UninitializedFunctionExport( "av_format_inject_global_side_data" ); }
void av_get_frame_filename() { Dylib::avformat_61.UninitializedFunctionExport( "av_get_frame_filename" ); }
void av_get_frame_filename2() { Dylib::avformat_61.UninitializedFunctionExport( "av_get_frame_filename2" ); }
void av_get_output_timestamp() { Dylib::avformat_61.UninitializedFunctionExport( "av_get_output_timestamp" ); }
void av_get_packet() { Dylib::avformat_61.UninitializedFunctionExport( "av_get_packet" ); }
void av_guess_codec() { Dylib::avformat_61.UninitializedFunctionExport( "av_guess_codec" ); }
void av_guess_format() { Dylib::avformat_61.UninitializedFunctionExport( "av_guess_format" ); }
void av_guess_frame_rate() { Dylib::avformat_61.UninitializedFunctionExport( "av_guess_frame_rate" ); }
void av_guess_sample_aspect_ratio() { Dylib::avformat_61.UninitializedFunctionExport( "av_guess_sample_aspect_ratio" ); }
void av_hex_dump() { Dylib::avformat_61.UninitializedFunctionExport( "av_hex_dump" ); }
void av_hex_dump_log() { Dylib::avformat_61.UninitializedFunctionExport( "av_hex_dump_log" ); }
void av_index_search_timestamp() { Dylib::avformat_61.UninitializedFunctionExport( "av_index_search_timestamp" ); }
void av_interleaved_write_frame() { Dylib::avformat_61.UninitializedFunctionExport( "av_interleaved_write_frame" ); }
void av_interleaved_write_uncoded_frame() { Dylib::avformat_61.UninitializedFunctionExport( "av_interleaved_write_uncoded_frame" ); }
void av_match_ext() { Dylib::avformat_61.UninitializedFunctionExport( "av_match_ext" ); }
void av_muxer_iterate() { Dylib::avformat_61.UninitializedFunctionExport( "av_muxer_iterate" ); }
void av_new_program() { Dylib::avformat_61.UninitializedFunctionExport( "av_new_program" ); }
void av_pkt_dump2() { Dylib::avformat_61.UninitializedFunctionExport( "av_pkt_dump2" ); }
void av_pkt_dump_log2() { Dylib::avformat_61.UninitializedFunctionExport( "av_pkt_dump_log2" ); }
void av_probe_input_buffer() { Dylib::avformat_61.UninitializedFunctionExport( "av_probe_input_buffer" ); }
void av_probe_input_buffer2() { Dylib::avformat_61.UninitializedFunctionExport( "av_probe_input_buffer2" ); }
void av_probe_input_format() { Dylib::avformat_61.UninitializedFunctionExport( "av_probe_input_format" ); }
void av_probe_input_format2() { Dylib::avformat_61.UninitializedFunctionExport( "av_probe_input_format2" ); }
void av_probe_input_format3() { Dylib::avformat_61.UninitializedFunctionExport( "av_probe_input_format3" ); }
void av_program_add_stream_index() { Dylib::avformat_61.UninitializedFunctionExport( "av_program_add_stream_index" ); }
void av_read_frame() { Dylib::avformat_61.UninitializedFunctionExport( "av_read_frame" ); }
void av_read_pause() { Dylib::avformat_61.UninitializedFunctionExport( "av_read_pause" ); }
void av_read_play() { Dylib::avformat_61.UninitializedFunctionExport( "av_read_play" ); }
void av_sdp_create() { Dylib::avformat_61.UninitializedFunctionExport( "av_sdp_create" ); }
void av_seek_frame() { Dylib::avformat_61.UninitializedFunctionExport( "av_seek_frame" ); }
void av_stream_add_side_data() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_add_side_data" ); }
void av_stream_get_class() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_get_class" ); }
void av_stream_get_codec_timebase() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_get_codec_timebase" ); }
void av_stream_get_parser() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_get_parser" ); }
void av_stream_get_side_data() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_get_side_data" ); }
void av_stream_group_get_class() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_group_get_class" ); }
void av_stream_new_side_data() { Dylib::avformat_61.UninitializedFunctionExport( "av_stream_new_side_data" ); }
void av_url_split() { Dylib::avformat_61.UninitializedFunctionExport( "av_url_split" ); }
void av_write_frame() { Dylib::avformat_61.UninitializedFunctionExport( "av_write_frame" ); }
void av_write_trailer() { Dylib::avformat_61.UninitializedFunctionExport( "av_write_trailer" ); }
void av_write_uncoded_frame() { Dylib::avformat_61.UninitializedFunctionExport( "av_write_uncoded_frame" ); }
void av_write_uncoded_frame_query() { Dylib::avformat_61.UninitializedFunctionExport( "av_write_uncoded_frame_query" ); }
void avformat_alloc_context() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_alloc_context" ); }
void avformat_alloc_output_context2() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_alloc_output_context2" ); }
void avformat_close_input() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_close_input" ); }
void avformat_configuration() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_configuration" ); }
void avformat_find_stream_info() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_find_stream_info" ); }
void avformat_flush() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_flush" ); }
void avformat_free_context() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_free_context" ); }
void avformat_get_class() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_get_class" ); }
void avformat_get_mov_audio_tags() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_get_mov_audio_tags" ); }
void avformat_get_mov_video_tags() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_get_mov_video_tags" ); }
void avformat_get_riff_audio_tags() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_get_riff_audio_tags" ); }
void avformat_get_riff_video_tags() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_get_riff_video_tags" ); }
void avformat_index_get_entries_count() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_index_get_entries_count" ); }
void avformat_index_get_entry() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_index_get_entry" ); }
void avformat_index_get_entry_from_timestamp() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_index_get_entry_from_timestamp" ); }
void avformat_init_output() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_init_output" ); }
void avformat_license() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_license" ); }
void avformat_match_stream_specifier() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_match_stream_specifier" ); }
void avformat_network_deinit() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_network_deinit" ); }
void avformat_network_init() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_network_init" ); }
void avformat_new_stream() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_new_stream" ); }
void avformat_open_input() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_open_input" ); }
void avformat_query_codec() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_query_codec" ); }
void avformat_queue_attached_pictures() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_queue_attached_pictures" ); }
void avformat_seek_file() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_seek_file" ); }
void avformat_stream_group_add_stream() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_stream_group_add_stream" ); }
void avformat_stream_group_create() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_stream_group_create" ); }
void avformat_stream_group_name() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_stream_group_name" ); }
void avformat_transfer_internal_stream_timing_info() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_transfer_internal_stream_timing_info" ); }
void avformat_version() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_version" ); }
void avformat_write_header() { Dylib::avformat_61.UninitializedFunctionExport( "avformat_write_header" ); }
void avio_accept() { Dylib::avformat_61.UninitializedFunctionExport( "avio_accept" ); }
void avio_alloc_context() { Dylib::avformat_61.UninitializedFunctionExport( "avio_alloc_context" ); }
void avio_check() { Dylib::avformat_61.UninitializedFunctionExport( "avio_check" ); }
void avio_close() { Dylib::avformat_61.UninitializedFunctionExport( "avio_close" ); }
void avio_close_dir() { Dylib::avformat_61.UninitializedFunctionExport( "avio_close_dir" ); }
void avio_close_dyn_buf() { Dylib::avformat_61.UninitializedFunctionExport( "avio_close_dyn_buf" ); }
void avio_closep() { Dylib::avformat_61.UninitializedFunctionExport( "avio_closep" ); }
void avio_context_free() { Dylib::avformat_61.UninitializedFunctionExport( "avio_context_free" ); }
void avio_enum_protocols() { Dylib::avformat_61.UninitializedFunctionExport( "avio_enum_protocols" ); }
void avio_feof() { Dylib::avformat_61.UninitializedFunctionExport( "avio_feof" ); }
void avio_find_protocol_name() { Dylib::avformat_61.UninitializedFunctionExport( "avio_find_protocol_name" ); }
void avio_flush() { Dylib::avformat_61.UninitializedFunctionExport( "avio_flush" ); }
void avio_free_directory_entry() { Dylib::avformat_61.UninitializedFunctionExport( "avio_free_directory_entry" ); }
void avio_get_dyn_buf() { Dylib::avformat_61.UninitializedFunctionExport( "avio_get_dyn_buf" ); }
void avio_get_str() { Dylib::avformat_61.UninitializedFunctionExport( "avio_get_str" ); }
void avio_get_str16be() { Dylib::avformat_61.UninitializedFunctionExport( "avio_get_str16be" ); }
void avio_get_str16le() { Dylib::avformat_61.UninitializedFunctionExport( "avio_get_str16le" ); }
void avio_handshake() { Dylib::avformat_61.UninitializedFunctionExport( "avio_handshake" ); }
void avio_open() { Dylib::avformat_61.UninitializedFunctionExport( "avio_open" ); }
void avio_open2() { Dylib::avformat_61.UninitializedFunctionExport( "avio_open2" ); }
void avio_open_dir() { Dylib::avformat_61.UninitializedFunctionExport( "avio_open_dir" ); }
void avio_open_dyn_buf() { Dylib::avformat_61.UninitializedFunctionExport( "avio_open_dyn_buf" ); }
void avio_pause() { Dylib::avformat_61.UninitializedFunctionExport( "avio_pause" ); }
void avio_print_string_array() { Dylib::avformat_61.UninitializedFunctionExport( "avio_print_string_array" ); }
void avio_printf() { Dylib::avformat_61.UninitializedFunctionExport( "avio_printf" ); }
void avio_protocol_get_class() { Dylib::avformat_61.UninitializedFunctionExport( "avio_protocol_get_class" ); }
void avio_put_str() { Dylib::avformat_61.UninitializedFunctionExport( "avio_put_str" ); }
void avio_put_str16be() { Dylib::avformat_61.UninitializedFunctionExport( "avio_put_str16be" ); }
void avio_put_str16le() { Dylib::avformat_61.UninitializedFunctionExport( "avio_put_str16le" ); }
void avio_r8() { Dylib::avformat_61.UninitializedFunctionExport( "avio_r8" ); }
void avio_rb16() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rb16" ); }
void avio_rb24() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rb24" ); }
void avio_rb32() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rb32" ); }
void avio_rb64() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rb64" ); }
void avio_read() { Dylib::avformat_61.UninitializedFunctionExport( "avio_read" ); }
void avio_read_dir() { Dylib::avformat_61.UninitializedFunctionExport( "avio_read_dir" ); }
void avio_read_partial() { Dylib::avformat_61.UninitializedFunctionExport( "avio_read_partial" ); }
void avio_read_to_bprint() { Dylib::avformat_61.UninitializedFunctionExport( "avio_read_to_bprint" ); }
void avio_rl16() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rl16" ); }
void avio_rl24() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rl24" ); }
void avio_rl32() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rl32" ); }
void avio_rl64() { Dylib::avformat_61.UninitializedFunctionExport( "avio_rl64" ); }
void avio_seek() { Dylib::avformat_61.UninitializedFunctionExport( "avio_seek" ); }
void avio_seek_time() { Dylib::avformat_61.UninitializedFunctionExport( "avio_seek_time" ); }
void avio_size() { Dylib::avformat_61.UninitializedFunctionExport( "avio_size" ); }
void avio_skip() { Dylib::avformat_61.UninitializedFunctionExport( "avio_skip" ); }
void avio_vprintf() { Dylib::avformat_61.UninitializedFunctionExport( "avio_vprintf" ); }
void avio_w8() { Dylib::avformat_61.UninitializedFunctionExport( "avio_w8" ); }
void avio_wb16() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wb16" ); }
void avio_wb24() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wb24" ); }
void avio_wb32() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wb32" ); }
void avio_wb64() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wb64" ); }
void avio_wl16() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wl16" ); }
void avio_wl24() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wl24" ); }
void avio_wl32() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wl32" ); }
void avio_wl64() { Dylib::avformat_61.UninitializedFunctionExport( "avio_wl64" ); }
void avio_write() { Dylib::avformat_61.UninitializedFunctionExport( "avio_write" ); }
void avio_write_marker() { Dylib::avformat_61.UninitializedFunctionExport( "avio_write_marker" ); }
void avpriv_dv_get_packet() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_dv_get_packet" ); }
void avpriv_dv_init_demux() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_dv_init_demux" ); }
void avpriv_dv_produce_packet() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_dv_produce_packet" ); }
void avpriv_mpegts_parse_close() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_mpegts_parse_close" ); }
void avpriv_mpegts_parse_open() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_mpegts_parse_open" ); }
void avpriv_mpegts_parse_packet() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_mpegts_parse_packet" ); }
void avpriv_new_chapter() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_new_chapter" ); }
void avpriv_register_devices() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_register_devices" ); }
void avpriv_set_pts_info() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_set_pts_info" ); }
void avpriv_stream_set_need_parsing() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_stream_set_need_parsing" ); }
void avpriv_update_cur_dts() { Dylib::avformat_61.UninitializedFunctionExport( "avpriv_update_cur_dts" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "av_add_index_entry", (void**)&av_add_index_entry, Dylib::Import::functionStub },
  { "av_append_packet", (void**)&av_append_packet, Dylib::Import::functionStub },
  { "av_codec_get_id", (void**)&av_codec_get_id, Dylib::Import::functionStub },
  { "av_codec_get_tag", (void**)&av_codec_get_tag, Dylib::Import::functionStub },
  { "av_codec_get_tag2", (void**)&av_codec_get_tag2, Dylib::Import::functionStub },
  { "av_demuxer_iterate", (void**)&av_demuxer_iterate, Dylib::Import::functionStub },
  { "av_disposition_from_string", (void**)&av_disposition_from_string, Dylib::Import::functionStub },
  { "av_disposition_to_string", (void**)&av_disposition_to_string, Dylib::Import::functionStub },
  { "av_dump_format", (void**)&av_dump_format, Dylib::Import::functionStub },
  { "av_filename_number_test", (void**)&av_filename_number_test, Dylib::Import::functionStub },
  { "av_find_best_stream", (void**)&av_find_best_stream, Dylib::Import::functionStub },
  { "av_find_default_stream_index", (void**)&av_find_default_stream_index, Dylib::Import::functionStub },
  { "av_find_input_format", (void**)&av_find_input_format, Dylib::Import::functionStub },
  { "av_find_program_from_stream", (void**)&av_find_program_from_stream, Dylib::Import::functionStub },
  { "av_fmt_ctx_get_duration_estimation_method", (void**)&av_fmt_ctx_get_duration_estimation_method, Dylib::Import::functionStub },
  { "av_format_inject_global_side_data", (void**)&av_format_inject_global_side_data, Dylib::Import::functionStub },
  { "av_get_frame_filename", (void**)&av_get_frame_filename, Dylib::Import::functionStub },
  { "av_get_frame_filename2", (void**)&av_get_frame_filename2, Dylib::Import::functionStub },
  { "av_get_output_timestamp", (void**)&av_get_output_timestamp, Dylib::Import::functionStub },
  { "av_get_packet", (void**)&av_get_packet, Dylib::Import::functionStub },
  { "av_guess_codec", (void**)&av_guess_codec, Dylib::Import::functionStub },
  { "av_guess_format", (void**)&av_guess_format, Dylib::Import::functionStub },
  { "av_guess_frame_rate", (void**)&av_guess_frame_rate, Dylib::Import::functionStub },
  { "av_guess_sample_aspect_ratio", (void**)&av_guess_sample_aspect_ratio, Dylib::Import::functionStub },
  { "av_hex_dump", (void**)&av_hex_dump, Dylib::Import::functionStub },
  { "av_hex_dump_log", (void**)&av_hex_dump_log, Dylib::Import::functionStub },
  { "av_index_search_timestamp", (void**)&av_index_search_timestamp, Dylib::Import::functionStub },
  { "av_interleaved_write_frame", (void**)&av_interleaved_write_frame, Dylib::Import::functionStub },
  { "av_interleaved_write_uncoded_frame", (void**)&av_interleaved_write_uncoded_frame, Dylib::Import::functionStub },
  { "av_match_ext", (void**)&av_match_ext, Dylib::Import::functionStub },
  { "av_muxer_iterate", (void**)&av_muxer_iterate, Dylib::Import::functionStub },
  { "av_new_program", (void**)&av_new_program, Dylib::Import::functionStub },
  { "av_pkt_dump2", (void**)&av_pkt_dump2, Dylib::Import::functionStub },
  { "av_pkt_dump_log2", (void**)&av_pkt_dump_log2, Dylib::Import::functionStub },
  { "av_probe_input_buffer", (void**)&av_probe_input_buffer, Dylib::Import::functionStub },
  { "av_probe_input_buffer2", (void**)&av_probe_input_buffer2, Dylib::Import::functionStub },
  { "av_probe_input_format", (void**)&av_probe_input_format, Dylib::Import::functionStub },
  { "av_probe_input_format2", (void**)&av_probe_input_format2, Dylib::Import::functionStub },
  { "av_probe_input_format3", (void**)&av_probe_input_format3, Dylib::Import::functionStub },
  { "av_program_add_stream_index", (void**)&av_program_add_stream_index, Dylib::Import::functionStub },
  { "av_read_frame", (void**)&av_read_frame, Dylib::Import::functionStub },
  { "av_read_pause", (void**)&av_read_pause, Dylib::Import::functionStub },
  { "av_read_play", (void**)&av_read_play, Dylib::Import::functionStub },
  { "av_sdp_create", (void**)&av_sdp_create, Dylib::Import::functionStub },
  { "av_seek_frame", (void**)&av_seek_frame, Dylib::Import::functionStub },
  { "av_stream_add_side_data", (void**)&av_stream_add_side_data, Dylib::Import::functionStub },
  { "av_stream_get_class", (void**)&av_stream_get_class, Dylib::Import::functionStub },
  { "av_stream_get_codec_timebase", (void**)&av_stream_get_codec_timebase, Dylib::Import::functionStub },
  { "av_stream_get_parser", (void**)&av_stream_get_parser, Dylib::Import::functionStub },
  { "av_stream_get_side_data", (void**)&av_stream_get_side_data, Dylib::Import::functionStub },
  { "av_stream_group_get_class", (void**)&av_stream_group_get_class, Dylib::Import::functionStub },
  { "av_stream_new_side_data", (void**)&av_stream_new_side_data, Dylib::Import::functionStub },
  { "av_url_split", (void**)&av_url_split, Dylib::Import::functionStub },
  { "av_write_frame", (void**)&av_write_frame, Dylib::Import::functionStub },
  { "av_write_trailer", (void**)&av_write_trailer, Dylib::Import::functionStub },
  { "av_write_uncoded_frame", (void**)&av_write_uncoded_frame, Dylib::Import::functionStub },
  { "av_write_uncoded_frame_query", (void**)&av_write_uncoded_frame_query, Dylib::Import::functionStub },
  { "avformat_alloc_context", (void**)&avformat_alloc_context, Dylib::Import::functionStub },
  { "avformat_alloc_output_context2", (void**)&avformat_alloc_output_context2, Dylib::Import::functionStub },
  { "avformat_close_input", (void**)&avformat_close_input, Dylib::Import::functionStub },
  { "avformat_configuration", (void**)&avformat_configuration, Dylib::Import::functionStub },
  { "avformat_find_stream_info", (void**)&avformat_find_stream_info, Dylib::Import::functionStub },
  { "avformat_flush", (void**)&avformat_flush, Dylib::Import::functionStub },
  { "avformat_free_context", (void**)&avformat_free_context, Dylib::Import::functionStub },
  { "avformat_get_class", (void**)&avformat_get_class, Dylib::Import::functionStub },
  { "avformat_get_mov_audio_tags", (void**)&avformat_get_mov_audio_tags, Dylib::Import::functionStub },
  { "avformat_get_mov_video_tags", (void**)&avformat_get_mov_video_tags, Dylib::Import::functionStub },
  { "avformat_get_riff_audio_tags", (void**)&avformat_get_riff_audio_tags, Dylib::Import::functionStub },
  { "avformat_get_riff_video_tags", (void**)&avformat_get_riff_video_tags, Dylib::Import::functionStub },
  { "avformat_index_get_entries_count", (void**)&avformat_index_get_entries_count, Dylib::Import::functionStub },
  { "avformat_index_get_entry", (void**)&avformat_index_get_entry, Dylib::Import::functionStub },
  { "avformat_index_get_entry_from_timestamp", (void**)&avformat_index_get_entry_from_timestamp, Dylib::Import::functionStub },
  { "avformat_init_output", (void**)&avformat_init_output, Dylib::Import::functionStub },
  { "avformat_license", (void**)&avformat_license, Dylib::Import::functionStub },
  { "avformat_match_stream_specifier", (void**)&avformat_match_stream_specifier, Dylib::Import::functionStub },
  { "avformat_network_deinit", (void**)&avformat_network_deinit, Dylib::Import::functionStub },
  { "avformat_network_init", (void**)&avformat_network_init, Dylib::Import::functionStub },
  { "avformat_new_stream", (void**)&avformat_new_stream, Dylib::Import::functionStub },
  { "avformat_open_input", (void**)&avformat_open_input, Dylib::Import::functionStub },
  { "avformat_query_codec", (void**)&avformat_query_codec, Dylib::Import::functionStub },
  { "avformat_queue_attached_pictures", (void**)&avformat_queue_attached_pictures, Dylib::Import::functionStub },
  { "avformat_seek_file", (void**)&avformat_seek_file, Dylib::Import::functionStub },
  { "avformat_stream_group_add_stream", (void**)&avformat_stream_group_add_stream, Dylib::Import::functionStub },
  { "avformat_stream_group_create", (void**)&avformat_stream_group_create, Dylib::Import::functionStub },
  { "avformat_stream_group_name", (void**)&avformat_stream_group_name, Dylib::Import::functionStub },
  { "avformat_transfer_internal_stream_timing_info", (void**)&avformat_transfer_internal_stream_timing_info, Dylib::Import::functionStub },
  { "avformat_version", (void**)&avformat_version, Dylib::Import::functionStub },
  { "avformat_write_header", (void**)&avformat_write_header, Dylib::Import::functionStub },
  { "avio_accept", (void**)&avio_accept, Dylib::Import::functionStub },
  { "avio_alloc_context", (void**)&avio_alloc_context, Dylib::Import::functionStub },
  { "avio_check", (void**)&avio_check, Dylib::Import::functionStub },
  { "avio_close", (void**)&avio_close, Dylib::Import::functionStub },
  { "avio_close_dir", (void**)&avio_close_dir, Dylib::Import::functionStub },
  { "avio_close_dyn_buf", (void**)&avio_close_dyn_buf, Dylib::Import::functionStub },
  { "avio_closep", (void**)&avio_closep, Dylib::Import::functionStub },
  { "avio_context_free", (void**)&avio_context_free, Dylib::Import::functionStub },
  { "avio_enum_protocols", (void**)&avio_enum_protocols, Dylib::Import::functionStub },
  { "avio_feof", (void**)&avio_feof, Dylib::Import::functionStub },
  { "avio_find_protocol_name", (void**)&avio_find_protocol_name, Dylib::Import::functionStub },
  { "avio_flush", (void**)&avio_flush, Dylib::Import::functionStub },
  { "avio_free_directory_entry", (void**)&avio_free_directory_entry, Dylib::Import::functionStub },
  { "avio_get_dyn_buf", (void**)&avio_get_dyn_buf, Dylib::Import::functionStub },
  { "avio_get_str", (void**)&avio_get_str, Dylib::Import::functionStub },
  { "avio_get_str16be", (void**)&avio_get_str16be, Dylib::Import::functionStub },
  { "avio_get_str16le", (void**)&avio_get_str16le, Dylib::Import::functionStub },
  { "avio_handshake", (void**)&avio_handshake, Dylib::Import::functionStub },
  { "avio_open", (void**)&avio_open, Dylib::Import::functionStub },
  { "avio_open2", (void**)&avio_open2, Dylib::Import::functionStub },
  { "avio_open_dir", (void**)&avio_open_dir, Dylib::Import::functionStub },
  { "avio_open_dyn_buf", (void**)&avio_open_dyn_buf, Dylib::Import::functionStub },
  { "avio_pause", (void**)&avio_pause, Dylib::Import::functionStub },
  { "avio_print_string_array", (void**)&avio_print_string_array, Dylib::Import::functionStub },
  { "avio_printf", (void**)&avio_printf, Dylib::Import::functionStub },
  { "avio_protocol_get_class", (void**)&avio_protocol_get_class, Dylib::Import::functionStub },
  { "avio_put_str", (void**)&avio_put_str, Dylib::Import::functionStub },
  { "avio_put_str16be", (void**)&avio_put_str16be, Dylib::Import::functionStub },
  { "avio_put_str16le", (void**)&avio_put_str16le, Dylib::Import::functionStub },
  { "avio_r8", (void**)&avio_r8, Dylib::Import::functionStub },
  { "avio_rb16", (void**)&avio_rb16, Dylib::Import::functionStub },
  { "avio_rb24", (void**)&avio_rb24, Dylib::Import::functionStub },
  { "avio_rb32", (void**)&avio_rb32, Dylib::Import::functionStub },
  { "avio_rb64", (void**)&avio_rb64, Dylib::Import::functionStub },
  { "avio_read", (void**)&avio_read, Dylib::Import::functionStub },
  { "avio_read_dir", (void**)&avio_read_dir, Dylib::Import::functionStub },
  { "avio_read_partial", (void**)&avio_read_partial, Dylib::Import::functionStub },
  { "avio_read_to_bprint", (void**)&avio_read_to_bprint, Dylib::Import::functionStub },
  { "avio_rl16", (void**)&avio_rl16, Dylib::Import::functionStub },
  { "avio_rl24", (void**)&avio_rl24, Dylib::Import::functionStub },
  { "avio_rl32", (void**)&avio_rl32, Dylib::Import::functionStub },
  { "avio_rl64", (void**)&avio_rl64, Dylib::Import::functionStub },
  { "avio_seek", (void**)&avio_seek, Dylib::Import::functionStub },
  { "avio_seek_time", (void**)&avio_seek_time, Dylib::Import::functionStub },
  { "avio_size", (void**)&avio_size, Dylib::Import::functionStub },
  { "avio_skip", (void**)&avio_skip, Dylib::Import::functionStub },
  { "avio_vprintf", (void**)&avio_vprintf, Dylib::Import::functionStub },
  { "avio_w8", (void**)&avio_w8, Dylib::Import::functionStub },
  { "avio_wb16", (void**)&avio_wb16, Dylib::Import::functionStub },
  { "avio_wb24", (void**)&avio_wb24, Dylib::Import::functionStub },
  { "avio_wb32", (void**)&avio_wb32, Dylib::Import::functionStub },
  { "avio_wb64", (void**)&avio_wb64, Dylib::Import::functionStub },
  { "avio_wl16", (void**)&avio_wl16, Dylib::Import::functionStub },
  { "avio_wl24", (void**)&avio_wl24, Dylib::Import::functionStub },
  { "avio_wl32", (void**)&avio_wl32, Dylib::Import::functionStub },
  { "avio_wl64", (void**)&avio_wl64, Dylib::Import::functionStub },
  { "avio_write", (void**)&avio_write, Dylib::Import::functionStub },
  { "avio_write_marker", (void**)&avio_write_marker, Dylib::Import::functionStub },
  { "avpriv_dv_get_packet", (void**)&avpriv_dv_get_packet, Dylib::Import::functionStub },
  { "avpriv_dv_init_demux", (void**)&avpriv_dv_init_demux, Dylib::Import::functionStub },
  { "avpriv_dv_produce_packet", (void**)&avpriv_dv_produce_packet, Dylib::Import::functionStub },
  { "avpriv_mpegts_parse_close", (void**)&avpriv_mpegts_parse_close, Dylib::Import::functionStub },
  { "avpriv_mpegts_parse_open", (void**)&avpriv_mpegts_parse_open, Dylib::Import::functionStub },
  { "avpriv_mpegts_parse_packet", (void**)&avpriv_mpegts_parse_packet, Dylib::Import::functionStub },
  { "avpriv_new_chapter", (void**)&avpriv_new_chapter, Dylib::Import::functionStub },
  { "avpriv_register_devices", (void**)&avpriv_register_devices, Dylib::Import::functionStub },
  { "avpriv_set_pts_info", (void**)&avpriv_set_pts_info, Dylib::Import::functionStub },
  { "avpriv_stream_set_need_parsing", (void**)&avpriv_stream_set_need_parsing, Dylib::Import::functionStub },
  { "avpriv_update_cur_dts", (void**)&avpriv_update_cur_dts, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBAVFORMAT_61

