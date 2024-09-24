// Import declarations for avcodec-61 library
// generated Tue May 21 14:44:24 2024 by DylibTool
#if STATIC_LIBAVCODEC_61

namespace Dylib { bool avcodec_61_Loaded() { return true; } }
extern "C" {
void* p_av_codec_ffversion = &av_codec_ffversion;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( avcodec_61, "avcodec-61", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_av_codec_ffversion = Dylib::avcodec_61.ResolveDataExport( "av_codec_ffversion" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void av_ac3_parse_header() { Dylib::avcodec_61.UninitializedFunctionExport( "av_ac3_parse_header" ); }
void av_adts_header_parse() { Dylib::avcodec_61.UninitializedFunctionExport( "av_adts_header_parse" ); }
void av_bsf_alloc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_alloc" ); }
void av_bsf_flush() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_flush" ); }
void av_bsf_free() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_free" ); }
void av_bsf_get_by_name() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_get_by_name" ); }
void av_bsf_get_class() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_get_class" ); }
void av_bsf_get_null_filter() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_get_null_filter" ); }
void av_bsf_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_init" ); }
void av_bsf_iterate() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_iterate" ); }
void av_bsf_list_alloc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_list_alloc" ); }
void av_bsf_list_append() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_list_append" ); }
void av_bsf_list_append2() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_list_append2" ); }
void av_bsf_list_finalize() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_list_finalize" ); }
void av_bsf_list_free() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_list_free" ); }
void av_bsf_list_parse_str() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_list_parse_str" ); }
void av_bsf_receive_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_receive_packet" ); }
void av_bsf_send_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "av_bsf_send_packet" ); }
void av_codec_is_decoder() { Dylib::avcodec_61.UninitializedFunctionExport( "av_codec_is_decoder" ); }
void av_codec_is_encoder() { Dylib::avcodec_61.UninitializedFunctionExport( "av_codec_is_encoder" ); }
void av_codec_iterate() { Dylib::avcodec_61.UninitializedFunctionExport( "av_codec_iterate" ); }
void av_cpb_properties_alloc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_cpb_properties_alloc" ); }
void av_d3d11va_alloc_context() { Dylib::avcodec_61.UninitializedFunctionExport( "av_d3d11va_alloc_context" ); }
void av_dct_calc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dct_calc" ); }
void av_dct_end() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dct_end" ); }
void av_dct_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dct_init" ); }
void av_dirac_parse_sequence_header() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dirac_parse_sequence_header" ); }
void av_dv_codec_profile() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dv_codec_profile" ); }
void av_dv_codec_profile2() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dv_codec_profile2" ); }
void av_dv_frame_profile() { Dylib::avcodec_61.UninitializedFunctionExport( "av_dv_frame_profile" ); }
void av_fast_padded_malloc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_fast_padded_malloc" ); }
void av_fast_padded_mallocz() { Dylib::avcodec_61.UninitializedFunctionExport( "av_fast_padded_mallocz" ); }
void av_fft_calc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_fft_calc" ); }
void av_fft_end() { Dylib::avcodec_61.UninitializedFunctionExport( "av_fft_end" ); }
void av_fft_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_fft_init" ); }
void av_fft_permute() { Dylib::avcodec_61.UninitializedFunctionExport( "av_fft_permute" ); }
void av_get_audio_frame_duration() { Dylib::avcodec_61.UninitializedFunctionExport( "av_get_audio_frame_duration" ); }
void av_get_audio_frame_duration2() { Dylib::avcodec_61.UninitializedFunctionExport( "av_get_audio_frame_duration2" ); }
void av_get_bits_per_sample() { Dylib::avcodec_61.UninitializedFunctionExport( "av_get_bits_per_sample" ); }
void av_get_exact_bits_per_sample() { Dylib::avcodec_61.UninitializedFunctionExport( "av_get_exact_bits_per_sample" ); }
void av_get_pcm_codec() { Dylib::avcodec_61.UninitializedFunctionExport( "av_get_pcm_codec" ); }
void av_get_profile_name() { Dylib::avcodec_61.UninitializedFunctionExport( "av_get_profile_name" ); }
void av_grow_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "av_grow_packet" ); }
void av_imdct_calc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_imdct_calc" ); }
void av_imdct_half() { Dylib::avcodec_61.UninitializedFunctionExport( "av_imdct_half" ); }
void av_init_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "av_init_packet" ); }
void av_jni_get_java_vm() { Dylib::avcodec_61.UninitializedFunctionExport( "av_jni_get_java_vm" ); }
void av_jni_set_java_vm() { Dylib::avcodec_61.UninitializedFunctionExport( "av_jni_set_java_vm" ); }
void av_mdct_calc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mdct_calc" ); }
void av_mdct_end() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mdct_end" ); }
void av_mdct_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mdct_init" ); }
void av_mediacodec_alloc_context() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mediacodec_alloc_context" ); }
void av_mediacodec_default_free() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mediacodec_default_free" ); }
void av_mediacodec_default_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mediacodec_default_init" ); }
void av_mediacodec_release_buffer() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mediacodec_release_buffer" ); }
void av_mediacodec_render_buffer_at_time() { Dylib::avcodec_61.UninitializedFunctionExport( "av_mediacodec_render_buffer_at_time" ); }
void av_new_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "av_new_packet" ); }
void av_packet_add_side_data() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_add_side_data" ); }
void av_packet_alloc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_alloc" ); }
void av_packet_clone() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_clone" ); }
void av_packet_copy_props() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_copy_props" ); }
void av_packet_free() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_free" ); }
void av_packet_free_side_data() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_free_side_data" ); }
void av_packet_from_data() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_from_data" ); }
void av_packet_get_side_data() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_get_side_data" ); }
void av_packet_make_refcounted() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_make_refcounted" ); }
void av_packet_make_writable() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_make_writable" ); }
void av_packet_move_ref() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_move_ref" ); }
void av_packet_new_side_data() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_new_side_data" ); }
void av_packet_pack_dictionary() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_pack_dictionary" ); }
void av_packet_ref() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_ref" ); }
void av_packet_rescale_ts() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_rescale_ts" ); }
void av_packet_shrink_side_data() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_shrink_side_data" ); }
void av_packet_side_data_add() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_side_data_add" ); }
void av_packet_side_data_free() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_side_data_free" ); }
void av_packet_side_data_get() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_side_data_get" ); }
void av_packet_side_data_name() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_side_data_name" ); }
void av_packet_side_data_new() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_side_data_new" ); }
void av_packet_side_data_remove() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_side_data_remove" ); }
void av_packet_unpack_dictionary() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_unpack_dictionary" ); }
void av_packet_unref() { Dylib::avcodec_61.UninitializedFunctionExport( "av_packet_unref" ); }
void av_parser_close() { Dylib::avcodec_61.UninitializedFunctionExport( "av_parser_close" ); }
void av_parser_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_parser_init" ); }
void av_parser_iterate() { Dylib::avcodec_61.UninitializedFunctionExport( "av_parser_iterate" ); }
void av_parser_parse2() { Dylib::avcodec_61.UninitializedFunctionExport( "av_parser_parse2" ); }
void av_qsv_alloc_context() { Dylib::avcodec_61.UninitializedFunctionExport( "av_qsv_alloc_context" ); }
void av_rdft_calc() { Dylib::avcodec_61.UninitializedFunctionExport( "av_rdft_calc" ); }
void av_rdft_end() { Dylib::avcodec_61.UninitializedFunctionExport( "av_rdft_end" ); }
void av_rdft_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_rdft_init" ); }
void av_shrink_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "av_shrink_packet" ); }
void av_vorbis_parse_frame() { Dylib::avcodec_61.UninitializedFunctionExport( "av_vorbis_parse_frame" ); }
void av_vorbis_parse_frame_flags() { Dylib::avcodec_61.UninitializedFunctionExport( "av_vorbis_parse_frame_flags" ); }
void av_vorbis_parse_free() { Dylib::avcodec_61.UninitializedFunctionExport( "av_vorbis_parse_free" ); }
void av_vorbis_parse_init() { Dylib::avcodec_61.UninitializedFunctionExport( "av_vorbis_parse_init" ); }
void av_vorbis_parse_reset() { Dylib::avcodec_61.UninitializedFunctionExport( "av_vorbis_parse_reset" ); }
void av_xiphlacing() { Dylib::avcodec_61.UninitializedFunctionExport( "av_xiphlacing" ); }
void avcodec_align_dimensions() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_align_dimensions" ); }
void avcodec_align_dimensions2() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_align_dimensions2" ); }
void avcodec_alloc_context3() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_alloc_context3" ); }
void avcodec_close() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_close" ); }
void avcodec_configuration() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_configuration" ); }
void avcodec_dct_alloc() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_dct_alloc" ); }
void avcodec_dct_get_class() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_dct_get_class" ); }
void avcodec_dct_init() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_dct_init" ); }
void avcodec_decode_subtitle2() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_decode_subtitle2" ); }
void avcodec_default_execute() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_default_execute" ); }
void avcodec_default_execute2() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_default_execute2" ); }
void avcodec_default_get_buffer2() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_default_get_buffer2" ); }
void avcodec_default_get_encode_buffer() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_default_get_encode_buffer" ); }
void avcodec_default_get_format() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_default_get_format" ); }
void avcodec_descriptor_get() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_descriptor_get" ); }
void avcodec_descriptor_get_by_name() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_descriptor_get_by_name" ); }
void avcodec_descriptor_next() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_descriptor_next" ); }
void avcodec_encode_subtitle() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_encode_subtitle" ); }
void avcodec_fill_audio_frame() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_fill_audio_frame" ); }
void avcodec_find_best_pix_fmt_of_list() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_find_best_pix_fmt_of_list" ); }
void avcodec_find_decoder() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_find_decoder" ); }
void avcodec_find_decoder_by_name() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_find_decoder_by_name" ); }
void avcodec_find_encoder() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_find_encoder" ); }
void avcodec_find_encoder_by_name() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_find_encoder_by_name" ); }
void avcodec_flush_buffers() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_flush_buffers" ); }
void avcodec_free_context() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_free_context" ); }
void avcodec_get_class() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_get_class" ); }
void avcodec_get_hw_config() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_get_hw_config" ); }
void avcodec_get_hw_frames_parameters() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_get_hw_frames_parameters" ); }
void avcodec_get_name() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_get_name" ); }
void avcodec_get_subtitle_rect_class() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_get_subtitle_rect_class" ); }
void avcodec_get_type() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_get_type" ); }
void avcodec_is_open() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_is_open" ); }
void avcodec_license() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_license" ); }
void avcodec_open2() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_open2" ); }
void avcodec_parameters_alloc() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_parameters_alloc" ); }
void avcodec_parameters_copy() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_parameters_copy" ); }
void avcodec_parameters_free() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_parameters_free" ); }
void avcodec_parameters_from_context() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_parameters_from_context" ); }
void avcodec_parameters_to_context() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_parameters_to_context" ); }
void avcodec_pix_fmt_to_codec_tag() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_pix_fmt_to_codec_tag" ); }
void avcodec_profile_name() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_profile_name" ); }
void avcodec_receive_frame() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_receive_frame" ); }
void avcodec_receive_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_receive_packet" ); }
void avcodec_send_frame() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_send_frame" ); }
void avcodec_send_packet() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_send_packet" ); }
void avcodec_string() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_string" ); }
void avcodec_version() { Dylib::avcodec_61.UninitializedFunctionExport( "avcodec_version" ); }
void avpriv_ac3_parse_header() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_ac3_parse_header" ); }
void avpriv_adts_header_parse() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_adts_header_parse" ); }
void avpriv_codec_get_cap_skip_frame_fill_param() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_codec_get_cap_skip_frame_fill_param" ); }
void avpriv_dca_convert_bitstream() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_dca_convert_bitstream" ); }
void avpriv_dca_parse_core_frame_header() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_dca_parse_core_frame_header" ); }
void avpriv_elbg_do() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_elbg_do" ); }
void avpriv_elbg_free() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_elbg_free" ); }
void avpriv_exif_decode_ifd() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_exif_decode_ifd" ); }
void avpriv_find_start_code() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_find_start_code" ); }
void avpriv_fits_header_init() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_fits_header_init" ); }
void avpriv_fits_header_parse_line() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_fits_header_parse_line" ); }
void avpriv_get_raw_pix_fmt_tags() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_get_raw_pix_fmt_tags" ); }
void avpriv_h264_has_num_reorder_frames() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_h264_has_num_reorder_frames" ); }
void avpriv_mpeg4audio_get_config2() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_mpeg4audio_get_config2" ); }
void avpriv_mpegaudio_decode_header() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_mpegaudio_decode_header" ); }
void avpriv_packet_list_free() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_packet_list_free" ); }
void avpriv_packet_list_get() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_packet_list_get" ); }
void avpriv_packet_list_put() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_packet_list_put" ); }
void avpriv_pix_fmt_find() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_pix_fmt_find" ); }
void avpriv_split_xiph_headers() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_split_xiph_headers" ); }
void avpriv_tak_parse_streaminfo() { Dylib::avcodec_61.UninitializedFunctionExport( "avpriv_tak_parse_streaminfo" ); }
void avsubtitle_free() { Dylib::avcodec_61.UninitializedFunctionExport( "avsubtitle_free" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "av_ac3_parse_header", (void**)&av_ac3_parse_header, Dylib::Import::functionStub },
  { "av_adts_header_parse", (void**)&av_adts_header_parse, Dylib::Import::functionStub },
  { "av_bsf_alloc", (void**)&av_bsf_alloc, Dylib::Import::functionStub },
  { "av_bsf_flush", (void**)&av_bsf_flush, Dylib::Import::functionStub },
  { "av_bsf_free", (void**)&av_bsf_free, Dylib::Import::functionStub },
  { "av_bsf_get_by_name", (void**)&av_bsf_get_by_name, Dylib::Import::functionStub },
  { "av_bsf_get_class", (void**)&av_bsf_get_class, Dylib::Import::functionStub },
  { "av_bsf_get_null_filter", (void**)&av_bsf_get_null_filter, Dylib::Import::functionStub },
  { "av_bsf_init", (void**)&av_bsf_init, Dylib::Import::functionStub },
  { "av_bsf_iterate", (void**)&av_bsf_iterate, Dylib::Import::functionStub },
  { "av_bsf_list_alloc", (void**)&av_bsf_list_alloc, Dylib::Import::functionStub },
  { "av_bsf_list_append", (void**)&av_bsf_list_append, Dylib::Import::functionStub },
  { "av_bsf_list_append2", (void**)&av_bsf_list_append2, Dylib::Import::functionStub },
  { "av_bsf_list_finalize", (void**)&av_bsf_list_finalize, Dylib::Import::functionStub },
  { "av_bsf_list_free", (void**)&av_bsf_list_free, Dylib::Import::functionStub },
  { "av_bsf_list_parse_str", (void**)&av_bsf_list_parse_str, Dylib::Import::functionStub },
  { "av_bsf_receive_packet", (void**)&av_bsf_receive_packet, Dylib::Import::functionStub },
  { "av_bsf_send_packet", (void**)&av_bsf_send_packet, Dylib::Import::functionStub },
  { "av_codec_is_decoder", (void**)&av_codec_is_decoder, Dylib::Import::functionStub },
  { "av_codec_is_encoder", (void**)&av_codec_is_encoder, Dylib::Import::functionStub },
  { "av_codec_iterate", (void**)&av_codec_iterate, Dylib::Import::functionStub },
  { "av_cpb_properties_alloc", (void**)&av_cpb_properties_alloc, Dylib::Import::functionStub },
  { "av_d3d11va_alloc_context", (void**)&av_d3d11va_alloc_context, Dylib::Import::functionStub },
  { "av_dct_calc", (void**)&av_dct_calc, Dylib::Import::functionStub },
  { "av_dct_end", (void**)&av_dct_end, Dylib::Import::functionStub },
  { "av_dct_init", (void**)&av_dct_init, Dylib::Import::functionStub },
  { "av_dirac_parse_sequence_header", (void**)&av_dirac_parse_sequence_header, Dylib::Import::functionStub },
  { "av_dv_codec_profile", (void**)&av_dv_codec_profile, Dylib::Import::functionStub },
  { "av_dv_codec_profile2", (void**)&av_dv_codec_profile2, Dylib::Import::functionStub },
  { "av_dv_frame_profile", (void**)&av_dv_frame_profile, Dylib::Import::functionStub },
  { "av_fast_padded_malloc", (void**)&av_fast_padded_malloc, Dylib::Import::functionStub },
  { "av_fast_padded_mallocz", (void**)&av_fast_padded_mallocz, Dylib::Import::functionStub },
  { "av_fft_calc", (void**)&av_fft_calc, Dylib::Import::functionStub },
  { "av_fft_end", (void**)&av_fft_end, Dylib::Import::functionStub },
  { "av_fft_init", (void**)&av_fft_init, Dylib::Import::functionStub },
  { "av_fft_permute", (void**)&av_fft_permute, Dylib::Import::functionStub },
  { "av_get_audio_frame_duration", (void**)&av_get_audio_frame_duration, Dylib::Import::functionStub },
  { "av_get_audio_frame_duration2", (void**)&av_get_audio_frame_duration2, Dylib::Import::functionStub },
  { "av_get_bits_per_sample", (void**)&av_get_bits_per_sample, Dylib::Import::functionStub },
  { "av_get_exact_bits_per_sample", (void**)&av_get_exact_bits_per_sample, Dylib::Import::functionStub },
  { "av_get_pcm_codec", (void**)&av_get_pcm_codec, Dylib::Import::functionStub },
  { "av_get_profile_name", (void**)&av_get_profile_name, Dylib::Import::functionStub },
  { "av_grow_packet", (void**)&av_grow_packet, Dylib::Import::functionStub },
  { "av_imdct_calc", (void**)&av_imdct_calc, Dylib::Import::functionStub },
  { "av_imdct_half", (void**)&av_imdct_half, Dylib::Import::functionStub },
  { "av_init_packet", (void**)&av_init_packet, Dylib::Import::functionStub },
  { "av_jni_get_java_vm", (void**)&av_jni_get_java_vm, Dylib::Import::functionStub },
  { "av_jni_set_java_vm", (void**)&av_jni_set_java_vm, Dylib::Import::functionStub },
  { "av_mdct_calc", (void**)&av_mdct_calc, Dylib::Import::functionStub },
  { "av_mdct_end", (void**)&av_mdct_end, Dylib::Import::functionStub },
  { "av_mdct_init", (void**)&av_mdct_init, Dylib::Import::functionStub },
  { "av_mediacodec_alloc_context", (void**)&av_mediacodec_alloc_context, Dylib::Import::functionStub },
  { "av_mediacodec_default_free", (void**)&av_mediacodec_default_free, Dylib::Import::functionStub },
  { "av_mediacodec_default_init", (void**)&av_mediacodec_default_init, Dylib::Import::functionStub },
  { "av_mediacodec_release_buffer", (void**)&av_mediacodec_release_buffer, Dylib::Import::functionStub },
  { "av_mediacodec_render_buffer_at_time", (void**)&av_mediacodec_render_buffer_at_time, Dylib::Import::functionStub },
  { "av_new_packet", (void**)&av_new_packet, Dylib::Import::functionStub },
  { "av_packet_add_side_data", (void**)&av_packet_add_side_data, Dylib::Import::functionStub },
  { "av_packet_alloc", (void**)&av_packet_alloc, Dylib::Import::functionStub },
  { "av_packet_clone", (void**)&av_packet_clone, Dylib::Import::functionStub },
  { "av_packet_copy_props", (void**)&av_packet_copy_props, Dylib::Import::functionStub },
  { "av_packet_free", (void**)&av_packet_free, Dylib::Import::functionStub },
  { "av_packet_free_side_data", (void**)&av_packet_free_side_data, Dylib::Import::functionStub },
  { "av_packet_from_data", (void**)&av_packet_from_data, Dylib::Import::functionStub },
  { "av_packet_get_side_data", (void**)&av_packet_get_side_data, Dylib::Import::functionStub },
  { "av_packet_make_refcounted", (void**)&av_packet_make_refcounted, Dylib::Import::functionStub },
  { "av_packet_make_writable", (void**)&av_packet_make_writable, Dylib::Import::functionStub },
  { "av_packet_move_ref", (void**)&av_packet_move_ref, Dylib::Import::functionStub },
  { "av_packet_new_side_data", (void**)&av_packet_new_side_data, Dylib::Import::functionStub },
  { "av_packet_pack_dictionary", (void**)&av_packet_pack_dictionary, Dylib::Import::functionStub },
  { "av_packet_ref", (void**)&av_packet_ref, Dylib::Import::functionStub },
  { "av_packet_rescale_ts", (void**)&av_packet_rescale_ts, Dylib::Import::functionStub },
  { "av_packet_shrink_side_data", (void**)&av_packet_shrink_side_data, Dylib::Import::functionStub },
  { "av_packet_side_data_add", (void**)&av_packet_side_data_add, Dylib::Import::functionStub },
  { "av_packet_side_data_free", (void**)&av_packet_side_data_free, Dylib::Import::functionStub },
  { "av_packet_side_data_get", (void**)&av_packet_side_data_get, Dylib::Import::functionStub },
  { "av_packet_side_data_name", (void**)&av_packet_side_data_name, Dylib::Import::functionStub },
  { "av_packet_side_data_new", (void**)&av_packet_side_data_new, Dylib::Import::functionStub },
  { "av_packet_side_data_remove", (void**)&av_packet_side_data_remove, Dylib::Import::functionStub },
  { "av_packet_unpack_dictionary", (void**)&av_packet_unpack_dictionary, Dylib::Import::functionStub },
  { "av_packet_unref", (void**)&av_packet_unref, Dylib::Import::functionStub },
  { "av_parser_close", (void**)&av_parser_close, Dylib::Import::functionStub },
  { "av_parser_init", (void**)&av_parser_init, Dylib::Import::functionStub },
  { "av_parser_iterate", (void**)&av_parser_iterate, Dylib::Import::functionStub },
  { "av_parser_parse2", (void**)&av_parser_parse2, Dylib::Import::functionStub },
  { "av_qsv_alloc_context", (void**)&av_qsv_alloc_context, Dylib::Import::functionStub },
  { "av_rdft_calc", (void**)&av_rdft_calc, Dylib::Import::functionStub },
  { "av_rdft_end", (void**)&av_rdft_end, Dylib::Import::functionStub },
  { "av_rdft_init", (void**)&av_rdft_init, Dylib::Import::functionStub },
  { "av_shrink_packet", (void**)&av_shrink_packet, Dylib::Import::functionStub },
  { "av_vorbis_parse_frame", (void**)&av_vorbis_parse_frame, Dylib::Import::functionStub },
  { "av_vorbis_parse_frame_flags", (void**)&av_vorbis_parse_frame_flags, Dylib::Import::functionStub },
  { "av_vorbis_parse_free", (void**)&av_vorbis_parse_free, Dylib::Import::functionStub },
  { "av_vorbis_parse_init", (void**)&av_vorbis_parse_init, Dylib::Import::functionStub },
  { "av_vorbis_parse_reset", (void**)&av_vorbis_parse_reset, Dylib::Import::functionStub },
  { "av_xiphlacing", (void**)&av_xiphlacing, Dylib::Import::functionStub },
  { "avcodec_align_dimensions", (void**)&avcodec_align_dimensions, Dylib::Import::functionStub },
  { "avcodec_align_dimensions2", (void**)&avcodec_align_dimensions2, Dylib::Import::functionStub },
  { "avcodec_alloc_context3", (void**)&avcodec_alloc_context3, Dylib::Import::functionStub },
  { "avcodec_close", (void**)&avcodec_close, Dylib::Import::functionStub },
  { "avcodec_configuration", (void**)&avcodec_configuration, Dylib::Import::functionStub },
  { "avcodec_dct_alloc", (void**)&avcodec_dct_alloc, Dylib::Import::functionStub },
  { "avcodec_dct_get_class", (void**)&avcodec_dct_get_class, Dylib::Import::functionStub },
  { "avcodec_dct_init", (void**)&avcodec_dct_init, Dylib::Import::functionStub },
  { "avcodec_decode_subtitle2", (void**)&avcodec_decode_subtitle2, Dylib::Import::functionStub },
  { "avcodec_default_execute", (void**)&avcodec_default_execute, Dylib::Import::functionStub },
  { "avcodec_default_execute2", (void**)&avcodec_default_execute2, Dylib::Import::functionStub },
  { "avcodec_default_get_buffer2", (void**)&avcodec_default_get_buffer2, Dylib::Import::functionStub },
  { "avcodec_default_get_encode_buffer", (void**)&avcodec_default_get_encode_buffer, Dylib::Import::functionStub },
  { "avcodec_default_get_format", (void**)&avcodec_default_get_format, Dylib::Import::functionStub },
  { "avcodec_descriptor_get", (void**)&avcodec_descriptor_get, Dylib::Import::functionStub },
  { "avcodec_descriptor_get_by_name", (void**)&avcodec_descriptor_get_by_name, Dylib::Import::functionStub },
  { "avcodec_descriptor_next", (void**)&avcodec_descriptor_next, Dylib::Import::functionStub },
  { "avcodec_encode_subtitle", (void**)&avcodec_encode_subtitle, Dylib::Import::functionStub },
  { "avcodec_fill_audio_frame", (void**)&avcodec_fill_audio_frame, Dylib::Import::functionStub },
  { "avcodec_find_best_pix_fmt_of_list", (void**)&avcodec_find_best_pix_fmt_of_list, Dylib::Import::functionStub },
  { "avcodec_find_decoder", (void**)&avcodec_find_decoder, Dylib::Import::functionStub },
  { "avcodec_find_decoder_by_name", (void**)&avcodec_find_decoder_by_name, Dylib::Import::functionStub },
  { "avcodec_find_encoder", (void**)&avcodec_find_encoder, Dylib::Import::functionStub },
  { "avcodec_find_encoder_by_name", (void**)&avcodec_find_encoder_by_name, Dylib::Import::functionStub },
  { "avcodec_flush_buffers", (void**)&avcodec_flush_buffers, Dylib::Import::functionStub },
  { "avcodec_free_context", (void**)&avcodec_free_context, Dylib::Import::functionStub },
  { "avcodec_get_class", (void**)&avcodec_get_class, Dylib::Import::functionStub },
  { "avcodec_get_hw_config", (void**)&avcodec_get_hw_config, Dylib::Import::functionStub },
  { "avcodec_get_hw_frames_parameters", (void**)&avcodec_get_hw_frames_parameters, Dylib::Import::functionStub },
  { "avcodec_get_name", (void**)&avcodec_get_name, Dylib::Import::functionStub },
  { "avcodec_get_subtitle_rect_class", (void**)&avcodec_get_subtitle_rect_class, Dylib::Import::functionStub },
  { "avcodec_get_type", (void**)&avcodec_get_type, Dylib::Import::functionStub },
  { "avcodec_is_open", (void**)&avcodec_is_open, Dylib::Import::functionStub },
  { "avcodec_license", (void**)&avcodec_license, Dylib::Import::functionStub },
  { "avcodec_open2", (void**)&avcodec_open2, Dylib::Import::functionStub },
  { "avcodec_parameters_alloc", (void**)&avcodec_parameters_alloc, Dylib::Import::functionStub },
  { "avcodec_parameters_copy", (void**)&avcodec_parameters_copy, Dylib::Import::functionStub },
  { "avcodec_parameters_free", (void**)&avcodec_parameters_free, Dylib::Import::functionStub },
  { "avcodec_parameters_from_context", (void**)&avcodec_parameters_from_context, Dylib::Import::functionStub },
  { "avcodec_parameters_to_context", (void**)&avcodec_parameters_to_context, Dylib::Import::functionStub },
  { "avcodec_pix_fmt_to_codec_tag", (void**)&avcodec_pix_fmt_to_codec_tag, Dylib::Import::functionStub },
  { "avcodec_profile_name", (void**)&avcodec_profile_name, Dylib::Import::functionStub },
  { "avcodec_receive_frame", (void**)&avcodec_receive_frame, Dylib::Import::functionStub },
  { "avcodec_receive_packet", (void**)&avcodec_receive_packet, Dylib::Import::functionStub },
  { "avcodec_send_frame", (void**)&avcodec_send_frame, Dylib::Import::functionStub },
  { "avcodec_send_packet", (void**)&avcodec_send_packet, Dylib::Import::functionStub },
  { "avcodec_string", (void**)&avcodec_string, Dylib::Import::functionStub },
  { "avcodec_version", (void**)&avcodec_version, Dylib::Import::functionStub },
  { "avpriv_ac3_parse_header", (void**)&avpriv_ac3_parse_header, Dylib::Import::functionStub },
  { "avpriv_adts_header_parse", (void**)&avpriv_adts_header_parse, Dylib::Import::functionStub },
  { "avpriv_codec_get_cap_skip_frame_fill_param", (void**)&avpriv_codec_get_cap_skip_frame_fill_param, Dylib::Import::functionStub },
  { "avpriv_dca_convert_bitstream", (void**)&avpriv_dca_convert_bitstream, Dylib::Import::functionStub },
  { "avpriv_dca_parse_core_frame_header", (void**)&avpriv_dca_parse_core_frame_header, Dylib::Import::functionStub },
  { "avpriv_elbg_do", (void**)&avpriv_elbg_do, Dylib::Import::functionStub },
  { "avpriv_elbg_free", (void**)&avpriv_elbg_free, Dylib::Import::functionStub },
  { "avpriv_exif_decode_ifd", (void**)&avpriv_exif_decode_ifd, Dylib::Import::functionStub },
  { "avpriv_find_start_code", (void**)&avpriv_find_start_code, Dylib::Import::functionStub },
  { "avpriv_fits_header_init", (void**)&avpriv_fits_header_init, Dylib::Import::functionStub },
  { "avpriv_fits_header_parse_line", (void**)&avpriv_fits_header_parse_line, Dylib::Import::functionStub },
  { "avpriv_get_raw_pix_fmt_tags", (void**)&avpriv_get_raw_pix_fmt_tags, Dylib::Import::functionStub },
  { "avpriv_h264_has_num_reorder_frames", (void**)&avpriv_h264_has_num_reorder_frames, Dylib::Import::functionStub },
  { "avpriv_mpeg4audio_get_config2", (void**)&avpriv_mpeg4audio_get_config2, Dylib::Import::functionStub },
  { "avpriv_mpegaudio_decode_header", (void**)&avpriv_mpegaudio_decode_header, Dylib::Import::functionStub },
  { "avpriv_packet_list_free", (void**)&avpriv_packet_list_free, Dylib::Import::functionStub },
  { "avpriv_packet_list_get", (void**)&avpriv_packet_list_get, Dylib::Import::functionStub },
  { "avpriv_packet_list_put", (void**)&avpriv_packet_list_put, Dylib::Import::functionStub },
  { "avpriv_pix_fmt_find", (void**)&avpriv_pix_fmt_find, Dylib::Import::functionStub },
  { "avpriv_split_xiph_headers", (void**)&avpriv_split_xiph_headers, Dylib::Import::functionStub },
  { "avpriv_tak_parse_streaminfo", (void**)&avpriv_tak_parse_streaminfo, Dylib::Import::functionStub },
  { "avsubtitle_free", (void**)&avsubtitle_free, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBAVCODEC_61

