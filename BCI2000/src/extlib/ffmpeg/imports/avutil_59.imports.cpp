// Import declarations for avutil-59 library
// generated Tue May 21 14:46:37 2024 by DylibTool
#if STATIC_LIBAVUTIL_59

namespace Dylib { bool avutil_59_Loaded() { return true; } }
extern "C" {
void* p_av_aes_size = &av_aes_size;
void* p_av_camellia_size = &av_camellia_size;
void* p_av_cast5_size = &av_cast5_size;
void* p_av_md5_size = &av_md5_size;
void* p_av_ripemd_size = &av_ripemd_size;
void* p_av_sha512_size = &av_sha512_size;
void* p_av_sha_size = &av_sha_size;
void* p_av_tea_size = &av_tea_size;
void* p_av_tree_node_size = &av_tree_node_size;
void* p_av_twofish_size = &av_twofish_size;
void* p_av_util_ffversion = &av_util_ffversion;
void* p_avpriv_cga_font = &avpriv_cga_font;
void* p_avpriv_vga16_font = &avpriv_vga16_font;
}

#else

#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( avutil_59, "avutil-59", functionImports, notFoundMsg, notFoundURL );

extern "C" {
void* p_av_aes_size = Dylib::avutil_59.ResolveDataExport( "av_aes_size" );
void* p_av_camellia_size = Dylib::avutil_59.ResolveDataExport( "av_camellia_size" );
void* p_av_cast5_size = Dylib::avutil_59.ResolveDataExport( "av_cast5_size" );
void* p_av_md5_size = Dylib::avutil_59.ResolveDataExport( "av_md5_size" );
void* p_av_ripemd_size = Dylib::avutil_59.ResolveDataExport( "av_ripemd_size" );
void* p_av_sha512_size = Dylib::avutil_59.ResolveDataExport( "av_sha512_size" );
void* p_av_sha_size = Dylib::avutil_59.ResolveDataExport( "av_sha_size" );
void* p_av_tea_size = Dylib::avutil_59.ResolveDataExport( "av_tea_size" );
void* p_av_tree_node_size = Dylib::avutil_59.ResolveDataExport( "av_tree_node_size" );
void* p_av_twofish_size = Dylib::avutil_59.ResolveDataExport( "av_twofish_size" );
void* p_av_util_ffversion = Dylib::avutil_59.ResolveDataExport( "av_util_ffversion" );
void* p_avpriv_cga_font = Dylib::avutil_59.ResolveDataExport( "avpriv_cga_font" );
void* p_avpriv_vga16_font = Dylib::avutil_59.ResolveDataExport( "avpriv_vga16_font" );

// NOTE: When loading the library, function bodies will be overwritten with absolute jumps
// to imported functions, i.e. function bodies must be roughly 2 * sizeof(void*) bytes long.
// A call to a member function with a pointer argument should be large enough.
// 
// In addition, having distinct function bodies for each function stub
// is important -- that makes it impossible for size optimizers to
// collapse them into a single definition.
//
void av_add_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_add_i" ); }
void av_add_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_add_q" ); }
void av_add_stable() { Dylib::avutil_59.UninitializedFunctionExport( "av_add_stable" ); }
void av_adler32_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_adler32_update" ); }
void av_aes_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_alloc" ); }
void av_aes_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_crypt" ); }
void av_aes_ctr_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_alloc" ); }
void av_aes_ctr_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_crypt" ); }
void av_aes_ctr_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_free" ); }
void av_aes_ctr_get_iv() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_get_iv" ); }
void av_aes_ctr_increment_iv() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_increment_iv" ); }
void av_aes_ctr_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_init" ); }
void av_aes_ctr_set_full_iv() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_set_full_iv" ); }
void av_aes_ctr_set_iv() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_set_iv" ); }
void av_aes_ctr_set_random_iv() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_ctr_set_random_iv" ); }
void av_aes_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_aes_init" ); }
void av_ambient_viewing_environment_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_ambient_viewing_environment_alloc" ); }
void av_ambient_viewing_environment_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_ambient_viewing_environment_create_side_data" ); }
void av_append_path_component() { Dylib::avutil_59.UninitializedFunctionExport( "av_append_path_component" ); }
void av_asprintf() { Dylib::avutil_59.UninitializedFunctionExport( "av_asprintf" ); }
void av_assert0_fpu() { Dylib::avutil_59.UninitializedFunctionExport( "av_assert0_fpu" ); }
void av_audio_fifo_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_alloc" ); }
void av_audio_fifo_drain() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_drain" ); }
void av_audio_fifo_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_free" ); }
void av_audio_fifo_peek() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_peek" ); }
void av_audio_fifo_peek_at() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_peek_at" ); }
void av_audio_fifo_read() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_read" ); }
void av_audio_fifo_realloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_realloc" ); }
void av_audio_fifo_reset() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_reset" ); }
void av_audio_fifo_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_size" ); }
void av_audio_fifo_space() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_space" ); }
void av_audio_fifo_write() { Dylib::avutil_59.UninitializedFunctionExport( "av_audio_fifo_write" ); }
void av_base64_decode() { Dylib::avutil_59.UninitializedFunctionExport( "av_base64_decode" ); }
void av_base64_encode() { Dylib::avutil_59.UninitializedFunctionExport( "av_base64_encode" ); }
void av_basename() { Dylib::avutil_59.UninitializedFunctionExport( "av_basename" ); }
void av_bessel_i0() { Dylib::avutil_59.UninitializedFunctionExport( "av_bessel_i0" ); }
void av_blowfish_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_blowfish_alloc" ); }
void av_blowfish_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_blowfish_crypt" ); }
void av_blowfish_crypt_ecb() { Dylib::avutil_59.UninitializedFunctionExport( "av_blowfish_crypt_ecb" ); }
void av_blowfish_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_blowfish_init" ); }
void av_bmg_get() { Dylib::avutil_59.UninitializedFunctionExport( "av_bmg_get" ); }
void av_bprint_append_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_append_data" ); }
void av_bprint_chars() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_chars" ); }
void av_bprint_clear() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_clear" ); }
void av_bprint_escape() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_escape" ); }
void av_bprint_finalize() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_finalize" ); }
void av_bprint_get_buffer() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_get_buffer" ); }
void av_bprint_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_init" ); }
void av_bprint_init_for_buffer() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_init_for_buffer" ); }
void av_bprint_strftime() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprint_strftime" ); }
void av_bprintf() { Dylib::avutil_59.UninitializedFunctionExport( "av_bprintf" ); }
void av_buffer_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_alloc" ); }
void av_buffer_allocz() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_allocz" ); }
void av_buffer_create() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_create" ); }
void av_buffer_default_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_default_free" ); }
void av_buffer_get_opaque() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_get_opaque" ); }
void av_buffer_get_ref_count() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_get_ref_count" ); }
void av_buffer_is_writable() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_is_writable" ); }
void av_buffer_make_writable() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_make_writable" ); }
void av_buffer_pool_buffer_get_opaque() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_pool_buffer_get_opaque" ); }
void av_buffer_pool_get() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_pool_get" ); }
void av_buffer_pool_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_pool_init" ); }
void av_buffer_pool_init2() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_pool_init2" ); }
void av_buffer_pool_uninit() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_pool_uninit" ); }
void av_buffer_realloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_realloc" ); }
void av_buffer_ref() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_ref" ); }
void av_buffer_replace() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_replace" ); }
void av_buffer_unref() { Dylib::avutil_59.UninitializedFunctionExport( "av_buffer_unref" ); }
void av_calloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_calloc" ); }
void av_camellia_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_camellia_alloc" ); }
void av_camellia_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_camellia_crypt" ); }
void av_camellia_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_camellia_init" ); }
void av_cast5_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_cast5_alloc" ); }
void av_cast5_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_cast5_crypt" ); }
void av_cast5_crypt2() { Dylib::avutil_59.UninitializedFunctionExport( "av_cast5_crypt2" ); }
void av_cast5_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_cast5_init" ); }
void av_channel_description() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_description" ); }
void av_channel_description_bprint() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_description_bprint" ); }
void av_channel_from_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_from_string" ); }
void av_channel_layout_channel_from_index() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_channel_from_index" ); }
void av_channel_layout_channel_from_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_channel_from_string" ); }
void av_channel_layout_check() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_check" ); }
void av_channel_layout_compare() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_compare" ); }
void av_channel_layout_copy() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_copy" ); }
void av_channel_layout_custom_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_custom_init" ); }
void av_channel_layout_default() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_default" ); }
void av_channel_layout_describe() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_describe" ); }
void av_channel_layout_describe_bprint() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_describe_bprint" ); }
void av_channel_layout_from_mask() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_from_mask" ); }
void av_channel_layout_from_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_from_string" ); }
void av_channel_layout_index_from_channel() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_index_from_channel" ); }
void av_channel_layout_index_from_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_index_from_string" ); }
void av_channel_layout_retype() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_retype" ); }
void av_channel_layout_standard() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_standard" ); }
void av_channel_layout_subset() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_subset" ); }
void av_channel_layout_uninit() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_layout_uninit" ); }
void av_channel_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_name" ); }
void av_channel_name_bprint() { Dylib::avutil_59.UninitializedFunctionExport( "av_channel_name_bprint" ); }
void av_chroma_location_enum_to_pos() { Dylib::avutil_59.UninitializedFunctionExport( "av_chroma_location_enum_to_pos" ); }
void av_chroma_location_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_chroma_location_from_name" ); }
void av_chroma_location_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_chroma_location_name" ); }
void av_chroma_location_pos_to_enum() { Dylib::avutil_59.UninitializedFunctionExport( "av_chroma_location_pos_to_enum" ); }
void av_cmp_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_cmp_i" ); }
void av_color_primaries_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_primaries_from_name" ); }
void av_color_primaries_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_primaries_name" ); }
void av_color_range_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_range_from_name" ); }
void av_color_range_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_range_name" ); }
void av_color_space_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_space_from_name" ); }
void av_color_space_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_space_name" ); }
void av_color_transfer_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_transfer_from_name" ); }
void av_color_transfer_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_color_transfer_name" ); }
void av_compare_mod() { Dylib::avutil_59.UninitializedFunctionExport( "av_compare_mod" ); }
void av_compare_ts() { Dylib::avutil_59.UninitializedFunctionExport( "av_compare_ts" ); }
void av_content_light_metadata_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_content_light_metadata_alloc" ); }
void av_content_light_metadata_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_content_light_metadata_create_side_data" ); }
void av_cpu_count() { Dylib::avutil_59.UninitializedFunctionExport( "av_cpu_count" ); }
void av_cpu_force_count() { Dylib::avutil_59.UninitializedFunctionExport( "av_cpu_force_count" ); }
void av_cpu_max_align() { Dylib::avutil_59.UninitializedFunctionExport( "av_cpu_max_align" ); }
void av_crc() { Dylib::avutil_59.UninitializedFunctionExport( "av_crc" ); }
void av_crc_get_table() { Dylib::avutil_59.UninitializedFunctionExport( "av_crc_get_table" ); }
void av_crc_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_crc_init" ); }
void av_csp_approximate_trc_gamma() { Dylib::avutil_59.UninitializedFunctionExport( "av_csp_approximate_trc_gamma" ); }
void av_csp_luma_coeffs_from_avcsp() { Dylib::avutil_59.UninitializedFunctionExport( "av_csp_luma_coeffs_from_avcsp" ); }
void av_csp_primaries_desc_from_id() { Dylib::avutil_59.UninitializedFunctionExport( "av_csp_primaries_desc_from_id" ); }
void av_csp_primaries_id_from_desc() { Dylib::avutil_59.UninitializedFunctionExport( "av_csp_primaries_id_from_desc" ); }
void av_csp_trc_func_from_id() { Dylib::avutil_59.UninitializedFunctionExport( "av_csp_trc_func_from_id" ); }
void av_d2q() { Dylib::avutil_59.UninitializedFunctionExport( "av_d2q" ); }
void av_default_get_category() { Dylib::avutil_59.UninitializedFunctionExport( "av_default_get_category" ); }
void av_default_item_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_default_item_name" ); }
void av_des_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_des_alloc" ); }
void av_des_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_des_crypt" ); }
void av_des_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_des_init" ); }
void av_des_mac() { Dylib::avutil_59.UninitializedFunctionExport( "av_des_mac" ); }
void av_detection_bbox_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_detection_bbox_alloc" ); }
void av_detection_bbox_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_detection_bbox_create_side_data" ); }
void av_dict_copy() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_copy" ); }
void av_dict_count() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_count" ); }
void av_dict_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_free" ); }
void av_dict_get() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_get" ); }
void av_dict_get_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_get_string" ); }
void av_dict_iterate() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_iterate" ); }
void av_dict_parse_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_parse_string" ); }
void av_dict_set() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_set" ); }
void av_dict_set_int() { Dylib::avutil_59.UninitializedFunctionExport( "av_dict_set_int" ); }
void av_dirname() { Dylib::avutil_59.UninitializedFunctionExport( "av_dirname" ); }
void av_display_matrix_flip() { Dylib::avutil_59.UninitializedFunctionExport( "av_display_matrix_flip" ); }
void av_display_rotation_get() { Dylib::avutil_59.UninitializedFunctionExport( "av_display_rotation_get" ); }
void av_display_rotation_set() { Dylib::avutil_59.UninitializedFunctionExport( "av_display_rotation_set" ); }
void av_div_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_div_i" ); }
void av_div_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_div_q" ); }
void av_dovi_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_dovi_alloc" ); }
void av_dovi_metadata_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_dovi_metadata_alloc" ); }
void av_downmix_info_update_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_downmix_info_update_side_data" ); }
void av_dynamic_hdr_plus_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynamic_hdr_plus_alloc" ); }
void av_dynamic_hdr_plus_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynamic_hdr_plus_create_side_data" ); }
void av_dynamic_hdr_plus_from_t35() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynamic_hdr_plus_from_t35" ); }
void av_dynamic_hdr_plus_to_t35() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynamic_hdr_plus_to_t35" ); }
void av_dynamic_hdr_vivid_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynamic_hdr_vivid_alloc" ); }
void av_dynamic_hdr_vivid_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynamic_hdr_vivid_create_side_data" ); }
void av_dynarray2_add() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynarray2_add" ); }
void av_dynarray_add() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynarray_add" ); }
void av_dynarray_add_nofree() { Dylib::avutil_59.UninitializedFunctionExport( "av_dynarray_add_nofree" ); }
void av_encryption_info_add_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_info_add_side_data" ); }
void av_encryption_info_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_info_alloc" ); }
void av_encryption_info_clone() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_info_clone" ); }
void av_encryption_info_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_info_free" ); }
void av_encryption_info_get_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_info_get_side_data" ); }
void av_encryption_init_info_add_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_init_info_add_side_data" ); }
void av_encryption_init_info_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_init_info_alloc" ); }
void av_encryption_init_info_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_init_info_free" ); }
void av_encryption_init_info_get_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_encryption_init_info_get_side_data" ); }
void av_escape() { Dylib::avutil_59.UninitializedFunctionExport( "av_escape" ); }
void av_executor_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_executor_alloc" ); }
void av_executor_execute() { Dylib::avutil_59.UninitializedFunctionExport( "av_executor_execute" ); }
void av_executor_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_executor_free" ); }
void av_expr_count_func() { Dylib::avutil_59.UninitializedFunctionExport( "av_expr_count_func" ); }
void av_expr_count_vars() { Dylib::avutil_59.UninitializedFunctionExport( "av_expr_count_vars" ); }
void av_expr_eval() { Dylib::avutil_59.UninitializedFunctionExport( "av_expr_eval" ); }
void av_expr_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_expr_free" ); }
void av_expr_parse() { Dylib::avutil_59.UninitializedFunctionExport( "av_expr_parse" ); }
void av_expr_parse_and_eval() { Dylib::avutil_59.UninitializedFunctionExport( "av_expr_parse_and_eval" ); }
void av_fast_malloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_fast_malloc" ); }
void av_fast_mallocz() { Dylib::avutil_59.UninitializedFunctionExport( "av_fast_mallocz" ); }
void av_fast_realloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_fast_realloc" ); }
void av_fifo_alloc2() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_alloc2" ); }
void av_fifo_auto_grow_limit() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_auto_grow_limit" ); }
void av_fifo_can_read() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_can_read" ); }
void av_fifo_can_write() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_can_write" ); }
void av_fifo_drain2() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_drain2" ); }
void av_fifo_elem_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_elem_size" ); }
void av_fifo_freep2() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_freep2" ); }
void av_fifo_grow2() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_grow2" ); }
void av_fifo_peek() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_peek" ); }
void av_fifo_peek_to_cb() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_peek_to_cb" ); }
void av_fifo_read() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_read" ); }
void av_fifo_read_to_cb() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_read_to_cb" ); }
void av_fifo_reset2() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_reset2" ); }
void av_fifo_write() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_write" ); }
void av_fifo_write_from_cb() { Dylib::avutil_59.UninitializedFunctionExport( "av_fifo_write_from_cb" ); }
void av_file_map() { Dylib::avutil_59.UninitializedFunctionExport( "av_file_map" ); }
void av_file_unmap() { Dylib::avutil_59.UninitializedFunctionExport( "av_file_unmap" ); }
void av_film_grain_params_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_film_grain_params_alloc" ); }
void av_film_grain_params_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_film_grain_params_create_side_data" ); }
void av_film_grain_params_select() { Dylib::avutil_59.UninitializedFunctionExport( "av_film_grain_params_select" ); }
void av_find_best_pix_fmt_of_2() { Dylib::avutil_59.UninitializedFunctionExport( "av_find_best_pix_fmt_of_2" ); }
void av_find_info_tag() { Dylib::avutil_59.UninitializedFunctionExport( "av_find_info_tag" ); }
void av_find_nearest_q_idx() { Dylib::avutil_59.UninitializedFunctionExport( "av_find_nearest_q_idx" ); }
void av_force_cpu_flags() { Dylib::avutil_59.UninitializedFunctionExport( "av_force_cpu_flags" ); }
void av_fourcc_make_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_fourcc_make_string" ); }
void av_frame_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_alloc" ); }
void av_frame_apply_cropping() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_apply_cropping" ); }
void av_frame_clone() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_clone" ); }
void av_frame_copy() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_copy" ); }
void av_frame_copy_props() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_copy_props" ); }
void av_frame_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_free" ); }
void av_frame_get_buffer() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_get_buffer" ); }
void av_frame_get_plane_buffer() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_get_plane_buffer" ); }
void av_frame_get_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_get_side_data" ); }
void av_frame_is_writable() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_is_writable" ); }
void av_frame_make_writable() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_make_writable" ); }
void av_frame_move_ref() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_move_ref" ); }
void av_frame_new_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_new_side_data" ); }
void av_frame_new_side_data_from_buf() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_new_side_data_from_buf" ); }
void av_frame_ref() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_ref" ); }
void av_frame_remove_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_remove_side_data" ); }
void av_frame_replace() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_replace" ); }
void av_frame_side_data_clone() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_side_data_clone" ); }
void av_frame_side_data_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_side_data_free" ); }
void av_frame_side_data_get_c() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_side_data_get_c" ); }
void av_frame_side_data_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_side_data_name" ); }
void av_frame_side_data_new() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_side_data_new" ); }
void av_frame_unref() { Dylib::avutil_59.UninitializedFunctionExport( "av_frame_unref" ); }
void av_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_free" ); }
void av_freep() { Dylib::avutil_59.UninitializedFunctionExport( "av_freep" ); }
void av_gcd() { Dylib::avutil_59.UninitializedFunctionExport( "av_gcd" ); }
void av_gcd_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_gcd_q" ); }
void av_get_alt_sample_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_alt_sample_fmt" ); }
void av_get_bits_per_pixel() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_bits_per_pixel" ); }
void av_get_bytes_per_sample() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_bytes_per_sample" ); }
void av_get_cpu_flags() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_cpu_flags" ); }
void av_get_known_color_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_known_color_name" ); }
void av_get_media_type_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_media_type_string" ); }
void av_get_packed_sample_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_packed_sample_fmt" ); }
void av_get_padded_bits_per_pixel() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_padded_bits_per_pixel" ); }
void av_get_picture_type_char() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_picture_type_char" ); }
void av_get_pix_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_pix_fmt" ); }
void av_get_pix_fmt_loss() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_pix_fmt_loss" ); }
void av_get_pix_fmt_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_pix_fmt_name" ); }
void av_get_pix_fmt_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_pix_fmt_string" ); }
void av_get_planar_sample_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_planar_sample_fmt" ); }
void av_get_random_seed() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_random_seed" ); }
void av_get_sample_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_sample_fmt" ); }
void av_get_sample_fmt_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_sample_fmt_name" ); }
void av_get_sample_fmt_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_sample_fmt_string" ); }
void av_get_time_base_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_time_base_q" ); }
void av_get_token() { Dylib::avutil_59.UninitializedFunctionExport( "av_get_token" ); }
void av_gettime() { Dylib::avutil_59.UninitializedFunctionExport( "av_gettime" ); }
void av_gettime_relative() { Dylib::avutil_59.UninitializedFunctionExport( "av_gettime_relative" ); }
void av_gettime_relative_is_monotonic() { Dylib::avutil_59.UninitializedFunctionExport( "av_gettime_relative_is_monotonic" ); }
void av_hash_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_alloc" ); }
void av_hash_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_final" ); }
void av_hash_final_b64() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_final_b64" ); }
void av_hash_final_bin() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_final_bin" ); }
void av_hash_final_hex() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_final_hex" ); }
void av_hash_freep() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_freep" ); }
void av_hash_get_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_get_name" ); }
void av_hash_get_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_get_size" ); }
void av_hash_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_init" ); }
void av_hash_names() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_names" ); }
void av_hash_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_hash_update" ); }
void av_hmac_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_hmac_alloc" ); }
void av_hmac_calc() { Dylib::avutil_59.UninitializedFunctionExport( "av_hmac_calc" ); }
void av_hmac_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_hmac_final" ); }
void av_hmac_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_hmac_free" ); }
void av_hmac_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_hmac_init" ); }
void av_hmac_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_hmac_update" ); }
void av_hwdevice_ctx_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_ctx_alloc" ); }
void av_hwdevice_ctx_create() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_ctx_create" ); }
void av_hwdevice_ctx_create_derived() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_ctx_create_derived" ); }
void av_hwdevice_ctx_create_derived_opts() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_ctx_create_derived_opts" ); }
void av_hwdevice_ctx_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_ctx_init" ); }
void av_hwdevice_find_type_by_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_find_type_by_name" ); }
void av_hwdevice_get_hwframe_constraints() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_get_hwframe_constraints" ); }
void av_hwdevice_get_type_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_get_type_name" ); }
void av_hwdevice_hwconfig_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_hwconfig_alloc" ); }
void av_hwdevice_iterate_types() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwdevice_iterate_types" ); }
void av_hwframe_constraints_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_constraints_free" ); }
void av_hwframe_ctx_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_ctx_alloc" ); }
void av_hwframe_ctx_create_derived() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_ctx_create_derived" ); }
void av_hwframe_ctx_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_ctx_init" ); }
void av_hwframe_get_buffer() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_get_buffer" ); }
void av_hwframe_map() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_map" ); }
void av_hwframe_transfer_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_transfer_data" ); }
void av_hwframe_transfer_get_formats() { Dylib::avutil_59.UninitializedFunctionExport( "av_hwframe_transfer_get_formats" ); }
void av_i2int() { Dylib::avutil_59.UninitializedFunctionExport( "av_i2int" ); }
void av_iamf_audio_element_add_layer() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_audio_element_add_layer" ); }
void av_iamf_audio_element_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_audio_element_alloc" ); }
void av_iamf_audio_element_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_audio_element_free" ); }
void av_iamf_audio_element_get_class() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_audio_element_get_class" ); }
void av_iamf_mix_presentation_add_submix() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_mix_presentation_add_submix" ); }
void av_iamf_mix_presentation_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_mix_presentation_alloc" ); }
void av_iamf_mix_presentation_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_mix_presentation_free" ); }
void av_iamf_mix_presentation_get_class() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_mix_presentation_get_class" ); }
void av_iamf_param_definition_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_param_definition_alloc" ); }
void av_iamf_param_definition_get_class() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_param_definition_get_class" ); }
void av_iamf_submix_add_element() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_submix_add_element" ); }
void av_iamf_submix_add_layout() { Dylib::avutil_59.UninitializedFunctionExport( "av_iamf_submix_add_layout" ); }
void av_image_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_alloc" ); }
void av_image_check_sar() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_check_sar" ); }
void av_image_check_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_check_size" ); }
void av_image_check_size2() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_check_size2" ); }
void av_image_copy() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_copy" ); }
void av_image_copy_plane() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_copy_plane" ); }
void av_image_copy_plane_uc_from() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_copy_plane_uc_from" ); }
void av_image_copy_to_buffer() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_copy_to_buffer" ); }
void av_image_copy_uc_from() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_copy_uc_from" ); }
void av_image_fill_arrays() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_arrays" ); }
void av_image_fill_black() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_black" ); }
void av_image_fill_color() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_color" ); }
void av_image_fill_linesizes() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_linesizes" ); }
void av_image_fill_max_pixsteps() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_max_pixsteps" ); }
void av_image_fill_plane_sizes() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_plane_sizes" ); }
void av_image_fill_pointers() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_fill_pointers" ); }
void av_image_get_buffer_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_get_buffer_size" ); }
void av_image_get_linesize() { Dylib::avutil_59.UninitializedFunctionExport( "av_image_get_linesize" ); }
void av_int2i() { Dylib::avutil_59.UninitializedFunctionExport( "av_int2i" ); }
void av_int_list_length_for_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_int_list_length_for_size" ); }
void av_lfg_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_lfg_init" ); }
void av_lfg_init_from_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_lfg_init_from_data" ); }
void av_log() { Dylib::avutil_59.UninitializedFunctionExport( "av_log" ); }
void av_log2() { Dylib::avutil_59.UninitializedFunctionExport( "av_log2" ); }
void av_log2_16bit() { Dylib::avutil_59.UninitializedFunctionExport( "av_log2_16bit" ); }
void av_log2_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_log2_i" ); }
void av_log_default_callback() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_default_callback" ); }
void av_log_format_line() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_format_line" ); }
void av_log_format_line2() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_format_line2" ); }
void av_log_get_flags() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_get_flags" ); }
void av_log_get_level() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_get_level" ); }
void av_log_once() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_once" ); }
void av_log_set_callback() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_set_callback" ); }
void av_log_set_flags() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_set_flags" ); }
void av_log_set_level() { Dylib::avutil_59.UninitializedFunctionExport( "av_log_set_level" ); }
void av_lzo1x_decode() { Dylib::avutil_59.UninitializedFunctionExport( "av_lzo1x_decode" ); }
void av_malloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_malloc" ); }
void av_malloc_array() { Dylib::avutil_59.UninitializedFunctionExport( "av_malloc_array" ); }
void av_mallocz() { Dylib::avutil_59.UninitializedFunctionExport( "av_mallocz" ); }
void av_mastering_display_metadata_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_mastering_display_metadata_alloc" ); }
void av_mastering_display_metadata_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_mastering_display_metadata_create_side_data" ); }
void av_match_list() { Dylib::avutil_59.UninitializedFunctionExport( "av_match_list" ); }
void av_match_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_match_name" ); }
void av_max_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_max_alloc" ); }
void av_md5_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_md5_alloc" ); }
void av_md5_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_md5_final" ); }
void av_md5_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_md5_init" ); }
void av_md5_sum() { Dylib::avutil_59.UninitializedFunctionExport( "av_md5_sum" ); }
void av_md5_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_md5_update" ); }
void av_memcpy_backptr() { Dylib::avutil_59.UninitializedFunctionExport( "av_memcpy_backptr" ); }
void av_memdup() { Dylib::avutil_59.UninitializedFunctionExport( "av_memdup" ); }
void av_mod_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_mod_i" ); }
void av_mul_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_mul_i" ); }
void av_mul_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_mul_q" ); }
void av_murmur3_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_murmur3_alloc" ); }
void av_murmur3_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_murmur3_final" ); }
void av_murmur3_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_murmur3_init" ); }
void av_murmur3_init_seeded() { Dylib::avutil_59.UninitializedFunctionExport( "av_murmur3_init_seeded" ); }
void av_murmur3_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_murmur3_update" ); }
void av_nearer_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_nearer_q" ); }
void av_opt_child_class_iterate() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_child_class_iterate" ); }
void av_opt_child_next() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_child_next" ); }
void av_opt_copy() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_copy" ); }
void av_opt_eval_double() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_eval_double" ); }
void av_opt_eval_flags() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_eval_flags" ); }
void av_opt_eval_float() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_eval_float" ); }
void av_opt_eval_int() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_eval_int" ); }
void av_opt_eval_int64() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_eval_int64" ); }
void av_opt_eval_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_eval_q" ); }
void av_opt_find() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_find" ); }
void av_opt_find2() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_find2" ); }
void av_opt_flag_is_set() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_flag_is_set" ); }
void av_opt_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_free" ); }
void av_opt_freep_ranges() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_freep_ranges" ); }
void av_opt_get() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get" ); }
void av_opt_get_chlayout() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_chlayout" ); }
void av_opt_get_dict_val() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_dict_val" ); }
void av_opt_get_double() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_double" ); }
void av_opt_get_image_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_image_size" ); }
void av_opt_get_int() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_int" ); }
void av_opt_get_key_value() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_key_value" ); }
void av_opt_get_pixel_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_pixel_fmt" ); }
void av_opt_get_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_q" ); }
void av_opt_get_sample_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_sample_fmt" ); }
void av_opt_get_video_rate() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_get_video_rate" ); }
void av_opt_is_set_to_default() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_is_set_to_default" ); }
void av_opt_is_set_to_default_by_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_is_set_to_default_by_name" ); }
void av_opt_next() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_next" ); }
void av_opt_ptr() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_ptr" ); }
void av_opt_query_ranges() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_query_ranges" ); }
void av_opt_query_ranges_default() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_query_ranges_default" ); }
void av_opt_serialize() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_serialize" ); }
void av_opt_set() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set" ); }
void av_opt_set_bin() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_bin" ); }
void av_opt_set_chlayout() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_chlayout" ); }
void av_opt_set_defaults() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_defaults" ); }
void av_opt_set_defaults2() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_defaults2" ); }
void av_opt_set_dict() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_dict" ); }
void av_opt_set_dict2() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_dict2" ); }
void av_opt_set_dict_val() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_dict_val" ); }
void av_opt_set_double() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_double" ); }
void av_opt_set_from_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_from_string" ); }
void av_opt_set_image_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_image_size" ); }
void av_opt_set_int() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_int" ); }
void av_opt_set_pixel_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_pixel_fmt" ); }
void av_opt_set_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_q" ); }
void av_opt_set_sample_fmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_sample_fmt" ); }
void av_opt_set_video_rate() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_set_video_rate" ); }
void av_opt_show2() { Dylib::avutil_59.UninitializedFunctionExport( "av_opt_show2" ); }
void av_parse_color() { Dylib::avutil_59.UninitializedFunctionExport( "av_parse_color" ); }
void av_parse_cpu_caps() { Dylib::avutil_59.UninitializedFunctionExport( "av_parse_cpu_caps" ); }
void av_parse_ratio() { Dylib::avutil_59.UninitializedFunctionExport( "av_parse_ratio" ); }
void av_parse_time() { Dylib::avutil_59.UninitializedFunctionExport( "av_parse_time" ); }
void av_parse_video_rate() { Dylib::avutil_59.UninitializedFunctionExport( "av_parse_video_rate" ); }
void av_parse_video_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_parse_video_size" ); }
void av_pix_fmt_count_planes() { Dylib::avutil_59.UninitializedFunctionExport( "av_pix_fmt_count_planes" ); }
void av_pix_fmt_desc_get() { Dylib::avutil_59.UninitializedFunctionExport( "av_pix_fmt_desc_get" ); }
void av_pix_fmt_desc_get_id() { Dylib::avutil_59.UninitializedFunctionExport( "av_pix_fmt_desc_get_id" ); }
void av_pix_fmt_desc_next() { Dylib::avutil_59.UninitializedFunctionExport( "av_pix_fmt_desc_next" ); }
void av_pix_fmt_get_chroma_sub_sample() { Dylib::avutil_59.UninitializedFunctionExport( "av_pix_fmt_get_chroma_sub_sample" ); }
void av_pix_fmt_swap_endianness() { Dylib::avutil_59.UninitializedFunctionExport( "av_pix_fmt_swap_endianness" ); }
void av_pixelutils_get_sad_fn() { Dylib::avutil_59.UninitializedFunctionExport( "av_pixelutils_get_sad_fn" ); }
void av_q2intfloat() { Dylib::avutil_59.UninitializedFunctionExport( "av_q2intfloat" ); }
void av_random_bytes() { Dylib::avutil_59.UninitializedFunctionExport( "av_random_bytes" ); }
void av_rc4_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_rc4_alloc" ); }
void av_rc4_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_rc4_crypt" ); }
void av_rc4_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_rc4_init" ); }
void av_read_image_line() { Dylib::avutil_59.UninitializedFunctionExport( "av_read_image_line" ); }
void av_read_image_line2() { Dylib::avutil_59.UninitializedFunctionExport( "av_read_image_line2" ); }
void av_realloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_realloc" ); }
void av_realloc_array() { Dylib::avutil_59.UninitializedFunctionExport( "av_realloc_array" ); }
void av_realloc_f() { Dylib::avutil_59.UninitializedFunctionExport( "av_realloc_f" ); }
void av_reallocp() { Dylib::avutil_59.UninitializedFunctionExport( "av_reallocp" ); }
void av_reallocp_array() { Dylib::avutil_59.UninitializedFunctionExport( "av_reallocp_array" ); }
void av_reduce() { Dylib::avutil_59.UninitializedFunctionExport( "av_reduce" ); }
void av_rescale() { Dylib::avutil_59.UninitializedFunctionExport( "av_rescale" ); }
void av_rescale_delta() { Dylib::avutil_59.UninitializedFunctionExport( "av_rescale_delta" ); }
void av_rescale_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_rescale_q" ); }
void av_rescale_q_rnd() { Dylib::avutil_59.UninitializedFunctionExport( "av_rescale_q_rnd" ); }
void av_rescale_rnd() { Dylib::avutil_59.UninitializedFunctionExport( "av_rescale_rnd" ); }
void av_ripemd_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_ripemd_alloc" ); }
void av_ripemd_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_ripemd_final" ); }
void av_ripemd_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_ripemd_init" ); }
void av_ripemd_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_ripemd_update" ); }
void av_sample_fmt_is_planar() { Dylib::avutil_59.UninitializedFunctionExport( "av_sample_fmt_is_planar" ); }
void av_samples_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_samples_alloc" ); }
void av_samples_alloc_array_and_samples() { Dylib::avutil_59.UninitializedFunctionExport( "av_samples_alloc_array_and_samples" ); }
void av_samples_copy() { Dylib::avutil_59.UninitializedFunctionExport( "av_samples_copy" ); }
void av_samples_fill_arrays() { Dylib::avutil_59.UninitializedFunctionExport( "av_samples_fill_arrays" ); }
void av_samples_get_buffer_size() { Dylib::avutil_59.UninitializedFunctionExport( "av_samples_get_buffer_size" ); }
void av_samples_set_silence() { Dylib::avutil_59.UninitializedFunctionExport( "av_samples_set_silence" ); }
void av_set_options_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_set_options_string" ); }
void av_sha512_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha512_alloc" ); }
void av_sha512_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha512_final" ); }
void av_sha512_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha512_init" ); }
void av_sha512_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha512_update" ); }
void av_sha_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha_alloc" ); }
void av_sha_final() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha_final" ); }
void av_sha_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha_init" ); }
void av_sha_update() { Dylib::avutil_59.UninitializedFunctionExport( "av_sha_update" ); }
void av_shr_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_shr_i" ); }
void av_size_mult() { Dylib::avutil_59.UninitializedFunctionExport( "av_size_mult" ); }
void av_small_strptime() { Dylib::avutil_59.UninitializedFunctionExport( "av_small_strptime" ); }
void av_spherical_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_spherical_alloc" ); }
void av_spherical_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_spherical_from_name" ); }
void av_spherical_projection_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_spherical_projection_name" ); }
void av_spherical_tile_bounds() { Dylib::avutil_59.UninitializedFunctionExport( "av_spherical_tile_bounds" ); }
void av_sscanf() { Dylib::avutil_59.UninitializedFunctionExport( "av_sscanf" ); }
void av_stereo3d_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_stereo3d_alloc" ); }
void av_stereo3d_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_stereo3d_create_side_data" ); }
void av_stereo3d_from_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_stereo3d_from_name" ); }
void av_stereo3d_type_name() { Dylib::avutil_59.UninitializedFunctionExport( "av_stereo3d_type_name" ); }
void av_strcasecmp() { Dylib::avutil_59.UninitializedFunctionExport( "av_strcasecmp" ); }
void av_strdup() { Dylib::avutil_59.UninitializedFunctionExport( "av_strdup" ); }
void av_strerror() { Dylib::avutil_59.UninitializedFunctionExport( "av_strerror" ); }
void av_strireplace() { Dylib::avutil_59.UninitializedFunctionExport( "av_strireplace" ); }
void av_stristart() { Dylib::avutil_59.UninitializedFunctionExport( "av_stristart" ); }
void av_stristr() { Dylib::avutil_59.UninitializedFunctionExport( "av_stristr" ); }
void av_strlcat() { Dylib::avutil_59.UninitializedFunctionExport( "av_strlcat" ); }
void av_strlcatf() { Dylib::avutil_59.UninitializedFunctionExport( "av_strlcatf" ); }
void av_strlcpy() { Dylib::avutil_59.UninitializedFunctionExport( "av_strlcpy" ); }
void av_strncasecmp() { Dylib::avutil_59.UninitializedFunctionExport( "av_strncasecmp" ); }
void av_strndup() { Dylib::avutil_59.UninitializedFunctionExport( "av_strndup" ); }
void av_strnstr() { Dylib::avutil_59.UninitializedFunctionExport( "av_strnstr" ); }
void av_strstart() { Dylib::avutil_59.UninitializedFunctionExport( "av_strstart" ); }
void av_strtod() { Dylib::avutil_59.UninitializedFunctionExport( "av_strtod" ); }
void av_strtok() { Dylib::avutil_59.UninitializedFunctionExport( "av_strtok" ); }
void av_sub_i() { Dylib::avutil_59.UninitializedFunctionExport( "av_sub_i" ); }
void av_sub_q() { Dylib::avutil_59.UninitializedFunctionExport( "av_sub_q" ); }
void av_tea_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_tea_alloc" ); }
void av_tea_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_tea_crypt" ); }
void av_tea_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_tea_init" ); }
void av_thread_message_flush() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_flush" ); }
void av_thread_message_queue_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_alloc" ); }
void av_thread_message_queue_free() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_free" ); }
void av_thread_message_queue_nb_elems() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_nb_elems" ); }
void av_thread_message_queue_recv() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_recv" ); }
void av_thread_message_queue_send() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_send" ); }
void av_thread_message_queue_set_err_recv() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_set_err_recv" ); }
void av_thread_message_queue_set_err_send() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_set_err_send" ); }
void av_thread_message_queue_set_free_func() { Dylib::avutil_59.UninitializedFunctionExport( "av_thread_message_queue_set_free_func" ); }
void av_timecode_adjust_ntsc_framenum2() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_adjust_ntsc_framenum2" ); }
void av_timecode_check_frame_rate() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_check_frame_rate" ); }
void av_timecode_get_smpte() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_get_smpte" ); }
void av_timecode_get_smpte_from_framenum() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_get_smpte_from_framenum" ); }
void av_timecode_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_init" ); }
void av_timecode_init_from_components() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_init_from_components" ); }
void av_timecode_init_from_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_init_from_string" ); }
void av_timecode_make_mpeg_tc_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_make_mpeg_tc_string" ); }
void av_timecode_make_smpte_tc_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_make_smpte_tc_string" ); }
void av_timecode_make_smpte_tc_string2() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_make_smpte_tc_string2" ); }
void av_timecode_make_string() { Dylib::avutil_59.UninitializedFunctionExport( "av_timecode_make_string" ); }
void av_timegm() { Dylib::avutil_59.UninitializedFunctionExport( "av_timegm" ); }
void av_tree_destroy() { Dylib::avutil_59.UninitializedFunctionExport( "av_tree_destroy" ); }
void av_tree_enumerate() { Dylib::avutil_59.UninitializedFunctionExport( "av_tree_enumerate" ); }
void av_tree_find() { Dylib::avutil_59.UninitializedFunctionExport( "av_tree_find" ); }
void av_tree_insert() { Dylib::avutil_59.UninitializedFunctionExport( "av_tree_insert" ); }
void av_tree_node_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_tree_node_alloc" ); }
void av_ts_make_time_string2() { Dylib::avutil_59.UninitializedFunctionExport( "av_ts_make_time_string2" ); }
void av_twofish_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_twofish_alloc" ); }
void av_twofish_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_twofish_crypt" ); }
void av_twofish_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_twofish_init" ); }
void av_tx_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_tx_init" ); }
void av_tx_uninit() { Dylib::avutil_59.UninitializedFunctionExport( "av_tx_uninit" ); }
void av_usleep() { Dylib::avutil_59.UninitializedFunctionExport( "av_usleep" ); }
void av_utf8_decode() { Dylib::avutil_59.UninitializedFunctionExport( "av_utf8_decode" ); }
void av_uuid_parse() { Dylib::avutil_59.UninitializedFunctionExport( "av_uuid_parse" ); }
void av_uuid_parse_range() { Dylib::avutil_59.UninitializedFunctionExport( "av_uuid_parse_range" ); }
void av_uuid_unparse() { Dylib::avutil_59.UninitializedFunctionExport( "av_uuid_unparse" ); }
void av_uuid_urn_parse() { Dylib::avutil_59.UninitializedFunctionExport( "av_uuid_urn_parse" ); }
void av_vbprintf() { Dylib::avutil_59.UninitializedFunctionExport( "av_vbprintf" ); }
void av_version_info() { Dylib::avutil_59.UninitializedFunctionExport( "av_version_info" ); }
void av_video_enc_params_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_video_enc_params_alloc" ); }
void av_video_enc_params_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_video_enc_params_create_side_data" ); }
void av_video_hint_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_video_hint_alloc" ); }
void av_video_hint_create_side_data() { Dylib::avutil_59.UninitializedFunctionExport( "av_video_hint_create_side_data" ); }
void av_vk_frame_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_vk_frame_alloc" ); }
void av_vkfmt_from_pixfmt() { Dylib::avutil_59.UninitializedFunctionExport( "av_vkfmt_from_pixfmt" ); }
void av_vlog() { Dylib::avutil_59.UninitializedFunctionExport( "av_vlog" ); }
void av_write_image_line() { Dylib::avutil_59.UninitializedFunctionExport( "av_write_image_line" ); }
void av_write_image_line2() { Dylib::avutil_59.UninitializedFunctionExport( "av_write_image_line2" ); }
void av_xtea_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "av_xtea_alloc" ); }
void av_xtea_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_xtea_crypt" ); }
void av_xtea_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_xtea_init" ); }
void av_xtea_le_crypt() { Dylib::avutil_59.UninitializedFunctionExport( "av_xtea_le_crypt" ); }
void av_xtea_le_init() { Dylib::avutil_59.UninitializedFunctionExport( "av_xtea_le_init" ); }
void avpriv_alloc_fixed_dsp() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_alloc_fixed_dsp" ); }
void avpriv_dict_set_timestamp() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_dict_set_timestamp" ); }
void avpriv_emms_asm() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_emms_asm" ); }
void avpriv_float_dsp_alloc() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_float_dsp_alloc" ); }
void avpriv_init_lls() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_init_lls" ); }
void avpriv_report_missing_feature() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_report_missing_feature" ); }
void avpriv_request_sample() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_request_sample" ); }
void avpriv_scalarproduct_float_c() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_scalarproduct_float_c" ); }
void avpriv_set_systematic_pal2() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_set_systematic_pal2" ); }
void avpriv_slicethread_create() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_slicethread_create" ); }
void avpriv_slicethread_execute() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_slicethread_execute" ); }
void avpriv_slicethread_free() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_slicethread_free" ); }
void avpriv_solve_lls() { Dylib::avutil_59.UninitializedFunctionExport( "avpriv_solve_lls" ); }
void avutil_configuration() { Dylib::avutil_59.UninitializedFunctionExport( "avutil_configuration" ); }
void avutil_license() { Dylib::avutil_59.UninitializedFunctionExport( "avutil_license" ); }
void avutil_version() { Dylib::avutil_59.UninitializedFunctionExport( "avutil_version" ); }
}

namespace {
const Dylib::Import functionImports_[] =
{
  { "av_add_i", (void**)&av_add_i, Dylib::Import::functionStub },
  { "av_add_q", (void**)&av_add_q, Dylib::Import::functionStub },
  { "av_add_stable", (void**)&av_add_stable, Dylib::Import::functionStub },
  { "av_adler32_update", (void**)&av_adler32_update, Dylib::Import::functionStub },
  { "av_aes_alloc", (void**)&av_aes_alloc, Dylib::Import::functionStub },
  { "av_aes_crypt", (void**)&av_aes_crypt, Dylib::Import::functionStub },
  { "av_aes_ctr_alloc", (void**)&av_aes_ctr_alloc, Dylib::Import::functionStub },
  { "av_aes_ctr_crypt", (void**)&av_aes_ctr_crypt, Dylib::Import::functionStub },
  { "av_aes_ctr_free", (void**)&av_aes_ctr_free, Dylib::Import::functionStub },
  { "av_aes_ctr_get_iv", (void**)&av_aes_ctr_get_iv, Dylib::Import::functionStub },
  { "av_aes_ctr_increment_iv", (void**)&av_aes_ctr_increment_iv, Dylib::Import::functionStub },
  { "av_aes_ctr_init", (void**)&av_aes_ctr_init, Dylib::Import::functionStub },
  { "av_aes_ctr_set_full_iv", (void**)&av_aes_ctr_set_full_iv, Dylib::Import::functionStub },
  { "av_aes_ctr_set_iv", (void**)&av_aes_ctr_set_iv, Dylib::Import::functionStub },
  { "av_aes_ctr_set_random_iv", (void**)&av_aes_ctr_set_random_iv, Dylib::Import::functionStub },
  { "av_aes_init", (void**)&av_aes_init, Dylib::Import::functionStub },
  { "av_ambient_viewing_environment_alloc", (void**)&av_ambient_viewing_environment_alloc, Dylib::Import::functionStub },
  { "av_ambient_viewing_environment_create_side_data", (void**)&av_ambient_viewing_environment_create_side_data, Dylib::Import::functionStub },
  { "av_append_path_component", (void**)&av_append_path_component, Dylib::Import::functionStub },
  { "av_asprintf", (void**)&av_asprintf, Dylib::Import::functionStub },
  { "av_assert0_fpu", (void**)&av_assert0_fpu, Dylib::Import::functionStub },
  { "av_audio_fifo_alloc", (void**)&av_audio_fifo_alloc, Dylib::Import::functionStub },
  { "av_audio_fifo_drain", (void**)&av_audio_fifo_drain, Dylib::Import::functionStub },
  { "av_audio_fifo_free", (void**)&av_audio_fifo_free, Dylib::Import::functionStub },
  { "av_audio_fifo_peek", (void**)&av_audio_fifo_peek, Dylib::Import::functionStub },
  { "av_audio_fifo_peek_at", (void**)&av_audio_fifo_peek_at, Dylib::Import::functionStub },
  { "av_audio_fifo_read", (void**)&av_audio_fifo_read, Dylib::Import::functionStub },
  { "av_audio_fifo_realloc", (void**)&av_audio_fifo_realloc, Dylib::Import::functionStub },
  { "av_audio_fifo_reset", (void**)&av_audio_fifo_reset, Dylib::Import::functionStub },
  { "av_audio_fifo_size", (void**)&av_audio_fifo_size, Dylib::Import::functionStub },
  { "av_audio_fifo_space", (void**)&av_audio_fifo_space, Dylib::Import::functionStub },
  { "av_audio_fifo_write", (void**)&av_audio_fifo_write, Dylib::Import::functionStub },
  { "av_base64_decode", (void**)&av_base64_decode, Dylib::Import::functionStub },
  { "av_base64_encode", (void**)&av_base64_encode, Dylib::Import::functionStub },
  { "av_basename", (void**)&av_basename, Dylib::Import::functionStub },
  { "av_bessel_i0", (void**)&av_bessel_i0, Dylib::Import::functionStub },
  { "av_blowfish_alloc", (void**)&av_blowfish_alloc, Dylib::Import::functionStub },
  { "av_blowfish_crypt", (void**)&av_blowfish_crypt, Dylib::Import::functionStub },
  { "av_blowfish_crypt_ecb", (void**)&av_blowfish_crypt_ecb, Dylib::Import::functionStub },
  { "av_blowfish_init", (void**)&av_blowfish_init, Dylib::Import::functionStub },
  { "av_bmg_get", (void**)&av_bmg_get, Dylib::Import::functionStub },
  { "av_bprint_append_data", (void**)&av_bprint_append_data, Dylib::Import::functionStub },
  { "av_bprint_chars", (void**)&av_bprint_chars, Dylib::Import::functionStub },
  { "av_bprint_clear", (void**)&av_bprint_clear, Dylib::Import::functionStub },
  { "av_bprint_escape", (void**)&av_bprint_escape, Dylib::Import::functionStub },
  { "av_bprint_finalize", (void**)&av_bprint_finalize, Dylib::Import::functionStub },
  { "av_bprint_get_buffer", (void**)&av_bprint_get_buffer, Dylib::Import::functionStub },
  { "av_bprint_init", (void**)&av_bprint_init, Dylib::Import::functionStub },
  { "av_bprint_init_for_buffer", (void**)&av_bprint_init_for_buffer, Dylib::Import::functionStub },
  { "av_bprint_strftime", (void**)&av_bprint_strftime, Dylib::Import::functionStub },
  { "av_bprintf", (void**)&av_bprintf, Dylib::Import::functionStub },
  { "av_buffer_alloc", (void**)&av_buffer_alloc, Dylib::Import::functionStub },
  { "av_buffer_allocz", (void**)&av_buffer_allocz, Dylib::Import::functionStub },
  { "av_buffer_create", (void**)&av_buffer_create, Dylib::Import::functionStub },
  { "av_buffer_default_free", (void**)&av_buffer_default_free, Dylib::Import::functionStub },
  { "av_buffer_get_opaque", (void**)&av_buffer_get_opaque, Dylib::Import::functionStub },
  { "av_buffer_get_ref_count", (void**)&av_buffer_get_ref_count, Dylib::Import::functionStub },
  { "av_buffer_is_writable", (void**)&av_buffer_is_writable, Dylib::Import::functionStub },
  { "av_buffer_make_writable", (void**)&av_buffer_make_writable, Dylib::Import::functionStub },
  { "av_buffer_pool_buffer_get_opaque", (void**)&av_buffer_pool_buffer_get_opaque, Dylib::Import::functionStub },
  { "av_buffer_pool_get", (void**)&av_buffer_pool_get, Dylib::Import::functionStub },
  { "av_buffer_pool_init", (void**)&av_buffer_pool_init, Dylib::Import::functionStub },
  { "av_buffer_pool_init2", (void**)&av_buffer_pool_init2, Dylib::Import::functionStub },
  { "av_buffer_pool_uninit", (void**)&av_buffer_pool_uninit, Dylib::Import::functionStub },
  { "av_buffer_realloc", (void**)&av_buffer_realloc, Dylib::Import::functionStub },
  { "av_buffer_ref", (void**)&av_buffer_ref, Dylib::Import::functionStub },
  { "av_buffer_replace", (void**)&av_buffer_replace, Dylib::Import::functionStub },
  { "av_buffer_unref", (void**)&av_buffer_unref, Dylib::Import::functionStub },
  { "av_calloc", (void**)&av_calloc, Dylib::Import::functionStub },
  { "av_camellia_alloc", (void**)&av_camellia_alloc, Dylib::Import::functionStub },
  { "av_camellia_crypt", (void**)&av_camellia_crypt, Dylib::Import::functionStub },
  { "av_camellia_init", (void**)&av_camellia_init, Dylib::Import::functionStub },
  { "av_cast5_alloc", (void**)&av_cast5_alloc, Dylib::Import::functionStub },
  { "av_cast5_crypt", (void**)&av_cast5_crypt, Dylib::Import::functionStub },
  { "av_cast5_crypt2", (void**)&av_cast5_crypt2, Dylib::Import::functionStub },
  { "av_cast5_init", (void**)&av_cast5_init, Dylib::Import::functionStub },
  { "av_channel_description", (void**)&av_channel_description, Dylib::Import::functionStub },
  { "av_channel_description_bprint", (void**)&av_channel_description_bprint, Dylib::Import::functionStub },
  { "av_channel_from_string", (void**)&av_channel_from_string, Dylib::Import::functionStub },
  { "av_channel_layout_channel_from_index", (void**)&av_channel_layout_channel_from_index, Dylib::Import::functionStub },
  { "av_channel_layout_channel_from_string", (void**)&av_channel_layout_channel_from_string, Dylib::Import::functionStub },
  { "av_channel_layout_check", (void**)&av_channel_layout_check, Dylib::Import::functionStub },
  { "av_channel_layout_compare", (void**)&av_channel_layout_compare, Dylib::Import::functionStub },
  { "av_channel_layout_copy", (void**)&av_channel_layout_copy, Dylib::Import::functionStub },
  { "av_channel_layout_custom_init", (void**)&av_channel_layout_custom_init, Dylib::Import::functionStub },
  { "av_channel_layout_default", (void**)&av_channel_layout_default, Dylib::Import::functionStub },
  { "av_channel_layout_describe", (void**)&av_channel_layout_describe, Dylib::Import::functionStub },
  { "av_channel_layout_describe_bprint", (void**)&av_channel_layout_describe_bprint, Dylib::Import::functionStub },
  { "av_channel_layout_from_mask", (void**)&av_channel_layout_from_mask, Dylib::Import::functionStub },
  { "av_channel_layout_from_string", (void**)&av_channel_layout_from_string, Dylib::Import::functionStub },
  { "av_channel_layout_index_from_channel", (void**)&av_channel_layout_index_from_channel, Dylib::Import::functionStub },
  { "av_channel_layout_index_from_string", (void**)&av_channel_layout_index_from_string, Dylib::Import::functionStub },
  { "av_channel_layout_retype", (void**)&av_channel_layout_retype, Dylib::Import::functionStub },
  { "av_channel_layout_standard", (void**)&av_channel_layout_standard, Dylib::Import::functionStub },
  { "av_channel_layout_subset", (void**)&av_channel_layout_subset, Dylib::Import::functionStub },
  { "av_channel_layout_uninit", (void**)&av_channel_layout_uninit, Dylib::Import::functionStub },
  { "av_channel_name", (void**)&av_channel_name, Dylib::Import::functionStub },
  { "av_channel_name_bprint", (void**)&av_channel_name_bprint, Dylib::Import::functionStub },
  { "av_chroma_location_enum_to_pos", (void**)&av_chroma_location_enum_to_pos, Dylib::Import::functionStub },
  { "av_chroma_location_from_name", (void**)&av_chroma_location_from_name, Dylib::Import::functionStub },
  { "av_chroma_location_name", (void**)&av_chroma_location_name, Dylib::Import::functionStub },
  { "av_chroma_location_pos_to_enum", (void**)&av_chroma_location_pos_to_enum, Dylib::Import::functionStub },
  { "av_cmp_i", (void**)&av_cmp_i, Dylib::Import::functionStub },
  { "av_color_primaries_from_name", (void**)&av_color_primaries_from_name, Dylib::Import::functionStub },
  { "av_color_primaries_name", (void**)&av_color_primaries_name, Dylib::Import::functionStub },
  { "av_color_range_from_name", (void**)&av_color_range_from_name, Dylib::Import::functionStub },
  { "av_color_range_name", (void**)&av_color_range_name, Dylib::Import::functionStub },
  { "av_color_space_from_name", (void**)&av_color_space_from_name, Dylib::Import::functionStub },
  { "av_color_space_name", (void**)&av_color_space_name, Dylib::Import::functionStub },
  { "av_color_transfer_from_name", (void**)&av_color_transfer_from_name, Dylib::Import::functionStub },
  { "av_color_transfer_name", (void**)&av_color_transfer_name, Dylib::Import::functionStub },
  { "av_compare_mod", (void**)&av_compare_mod, Dylib::Import::functionStub },
  { "av_compare_ts", (void**)&av_compare_ts, Dylib::Import::functionStub },
  { "av_content_light_metadata_alloc", (void**)&av_content_light_metadata_alloc, Dylib::Import::functionStub },
  { "av_content_light_metadata_create_side_data", (void**)&av_content_light_metadata_create_side_data, Dylib::Import::functionStub },
  { "av_cpu_count", (void**)&av_cpu_count, Dylib::Import::functionStub },
  { "av_cpu_force_count", (void**)&av_cpu_force_count, Dylib::Import::functionStub },
  { "av_cpu_max_align", (void**)&av_cpu_max_align, Dylib::Import::functionStub },
  { "av_crc", (void**)&av_crc, Dylib::Import::functionStub },
  { "av_crc_get_table", (void**)&av_crc_get_table, Dylib::Import::functionStub },
  { "av_crc_init", (void**)&av_crc_init, Dylib::Import::functionStub },
  { "av_csp_approximate_trc_gamma", (void**)&av_csp_approximate_trc_gamma, Dylib::Import::functionStub },
  { "av_csp_luma_coeffs_from_avcsp", (void**)&av_csp_luma_coeffs_from_avcsp, Dylib::Import::functionStub },
  { "av_csp_primaries_desc_from_id", (void**)&av_csp_primaries_desc_from_id, Dylib::Import::functionStub },
  { "av_csp_primaries_id_from_desc", (void**)&av_csp_primaries_id_from_desc, Dylib::Import::functionStub },
  { "av_csp_trc_func_from_id", (void**)&av_csp_trc_func_from_id, Dylib::Import::functionStub },
  { "av_d2q", (void**)&av_d2q, Dylib::Import::functionStub },
  { "av_default_get_category", (void**)&av_default_get_category, Dylib::Import::functionStub },
  { "av_default_item_name", (void**)&av_default_item_name, Dylib::Import::functionStub },
  { "av_des_alloc", (void**)&av_des_alloc, Dylib::Import::functionStub },
  { "av_des_crypt", (void**)&av_des_crypt, Dylib::Import::functionStub },
  { "av_des_init", (void**)&av_des_init, Dylib::Import::functionStub },
  { "av_des_mac", (void**)&av_des_mac, Dylib::Import::functionStub },
  { "av_detection_bbox_alloc", (void**)&av_detection_bbox_alloc, Dylib::Import::functionStub },
  { "av_detection_bbox_create_side_data", (void**)&av_detection_bbox_create_side_data, Dylib::Import::functionStub },
  { "av_dict_copy", (void**)&av_dict_copy, Dylib::Import::functionStub },
  { "av_dict_count", (void**)&av_dict_count, Dylib::Import::functionStub },
  { "av_dict_free", (void**)&av_dict_free, Dylib::Import::functionStub },
  { "av_dict_get", (void**)&av_dict_get, Dylib::Import::functionStub },
  { "av_dict_get_string", (void**)&av_dict_get_string, Dylib::Import::functionStub },
  { "av_dict_iterate", (void**)&av_dict_iterate, Dylib::Import::functionStub },
  { "av_dict_parse_string", (void**)&av_dict_parse_string, Dylib::Import::functionStub },
  { "av_dict_set", (void**)&av_dict_set, Dylib::Import::functionStub },
  { "av_dict_set_int", (void**)&av_dict_set_int, Dylib::Import::functionStub },
  { "av_dirname", (void**)&av_dirname, Dylib::Import::functionStub },
  { "av_display_matrix_flip", (void**)&av_display_matrix_flip, Dylib::Import::functionStub },
  { "av_display_rotation_get", (void**)&av_display_rotation_get, Dylib::Import::functionStub },
  { "av_display_rotation_set", (void**)&av_display_rotation_set, Dylib::Import::functionStub },
  { "av_div_i", (void**)&av_div_i, Dylib::Import::functionStub },
  { "av_div_q", (void**)&av_div_q, Dylib::Import::functionStub },
  { "av_dovi_alloc", (void**)&av_dovi_alloc, Dylib::Import::functionStub },
  { "av_dovi_metadata_alloc", (void**)&av_dovi_metadata_alloc, Dylib::Import::functionStub },
  { "av_downmix_info_update_side_data", (void**)&av_downmix_info_update_side_data, Dylib::Import::functionStub },
  { "av_dynamic_hdr_plus_alloc", (void**)&av_dynamic_hdr_plus_alloc, Dylib::Import::functionStub },
  { "av_dynamic_hdr_plus_create_side_data", (void**)&av_dynamic_hdr_plus_create_side_data, Dylib::Import::functionStub },
  { "av_dynamic_hdr_plus_from_t35", (void**)&av_dynamic_hdr_plus_from_t35, Dylib::Import::functionStub },
  { "av_dynamic_hdr_plus_to_t35", (void**)&av_dynamic_hdr_plus_to_t35, Dylib::Import::functionStub },
  { "av_dynamic_hdr_vivid_alloc", (void**)&av_dynamic_hdr_vivid_alloc, Dylib::Import::functionStub },
  { "av_dynamic_hdr_vivid_create_side_data", (void**)&av_dynamic_hdr_vivid_create_side_data, Dylib::Import::functionStub },
  { "av_dynarray2_add", (void**)&av_dynarray2_add, Dylib::Import::functionStub },
  { "av_dynarray_add", (void**)&av_dynarray_add, Dylib::Import::functionStub },
  { "av_dynarray_add_nofree", (void**)&av_dynarray_add_nofree, Dylib::Import::functionStub },
  { "av_encryption_info_add_side_data", (void**)&av_encryption_info_add_side_data, Dylib::Import::functionStub },
  { "av_encryption_info_alloc", (void**)&av_encryption_info_alloc, Dylib::Import::functionStub },
  { "av_encryption_info_clone", (void**)&av_encryption_info_clone, Dylib::Import::functionStub },
  { "av_encryption_info_free", (void**)&av_encryption_info_free, Dylib::Import::functionStub },
  { "av_encryption_info_get_side_data", (void**)&av_encryption_info_get_side_data, Dylib::Import::functionStub },
  { "av_encryption_init_info_add_side_data", (void**)&av_encryption_init_info_add_side_data, Dylib::Import::functionStub },
  { "av_encryption_init_info_alloc", (void**)&av_encryption_init_info_alloc, Dylib::Import::functionStub },
  { "av_encryption_init_info_free", (void**)&av_encryption_init_info_free, Dylib::Import::functionStub },
  { "av_encryption_init_info_get_side_data", (void**)&av_encryption_init_info_get_side_data, Dylib::Import::functionStub },
  { "av_escape", (void**)&av_escape, Dylib::Import::functionStub },
  { "av_executor_alloc", (void**)&av_executor_alloc, Dylib::Import::functionStub },
  { "av_executor_execute", (void**)&av_executor_execute, Dylib::Import::functionStub },
  { "av_executor_free", (void**)&av_executor_free, Dylib::Import::functionStub },
  { "av_expr_count_func", (void**)&av_expr_count_func, Dylib::Import::functionStub },
  { "av_expr_count_vars", (void**)&av_expr_count_vars, Dylib::Import::functionStub },
  { "av_expr_eval", (void**)&av_expr_eval, Dylib::Import::functionStub },
  { "av_expr_free", (void**)&av_expr_free, Dylib::Import::functionStub },
  { "av_expr_parse", (void**)&av_expr_parse, Dylib::Import::functionStub },
  { "av_expr_parse_and_eval", (void**)&av_expr_parse_and_eval, Dylib::Import::functionStub },
  { "av_fast_malloc", (void**)&av_fast_malloc, Dylib::Import::functionStub },
  { "av_fast_mallocz", (void**)&av_fast_mallocz, Dylib::Import::functionStub },
  { "av_fast_realloc", (void**)&av_fast_realloc, Dylib::Import::functionStub },
  { "av_fifo_alloc2", (void**)&av_fifo_alloc2, Dylib::Import::functionStub },
  { "av_fifo_auto_grow_limit", (void**)&av_fifo_auto_grow_limit, Dylib::Import::functionStub },
  { "av_fifo_can_read", (void**)&av_fifo_can_read, Dylib::Import::functionStub },
  { "av_fifo_can_write", (void**)&av_fifo_can_write, Dylib::Import::functionStub },
  { "av_fifo_drain2", (void**)&av_fifo_drain2, Dylib::Import::functionStub },
  { "av_fifo_elem_size", (void**)&av_fifo_elem_size, Dylib::Import::functionStub },
  { "av_fifo_freep2", (void**)&av_fifo_freep2, Dylib::Import::functionStub },
  { "av_fifo_grow2", (void**)&av_fifo_grow2, Dylib::Import::functionStub },
  { "av_fifo_peek", (void**)&av_fifo_peek, Dylib::Import::functionStub },
  { "av_fifo_peek_to_cb", (void**)&av_fifo_peek_to_cb, Dylib::Import::functionStub },
  { "av_fifo_read", (void**)&av_fifo_read, Dylib::Import::functionStub },
  { "av_fifo_read_to_cb", (void**)&av_fifo_read_to_cb, Dylib::Import::functionStub },
  { "av_fifo_reset2", (void**)&av_fifo_reset2, Dylib::Import::functionStub },
  { "av_fifo_write", (void**)&av_fifo_write, Dylib::Import::functionStub },
  { "av_fifo_write_from_cb", (void**)&av_fifo_write_from_cb, Dylib::Import::functionStub },
  { "av_file_map", (void**)&av_file_map, Dylib::Import::functionStub },
  { "av_file_unmap", (void**)&av_file_unmap, Dylib::Import::functionStub },
  { "av_film_grain_params_alloc", (void**)&av_film_grain_params_alloc, Dylib::Import::functionStub },
  { "av_film_grain_params_create_side_data", (void**)&av_film_grain_params_create_side_data, Dylib::Import::functionStub },
  { "av_film_grain_params_select", (void**)&av_film_grain_params_select, Dylib::Import::functionStub },
  { "av_find_best_pix_fmt_of_2", (void**)&av_find_best_pix_fmt_of_2, Dylib::Import::functionStub },
  { "av_find_info_tag", (void**)&av_find_info_tag, Dylib::Import::functionStub },
  { "av_find_nearest_q_idx", (void**)&av_find_nearest_q_idx, Dylib::Import::functionStub },
  { "av_force_cpu_flags", (void**)&av_force_cpu_flags, Dylib::Import::functionStub },
  { "av_fourcc_make_string", (void**)&av_fourcc_make_string, Dylib::Import::functionStub },
  { "av_frame_alloc", (void**)&av_frame_alloc, Dylib::Import::functionStub },
  { "av_frame_apply_cropping", (void**)&av_frame_apply_cropping, Dylib::Import::functionStub },
  { "av_frame_clone", (void**)&av_frame_clone, Dylib::Import::functionStub },
  { "av_frame_copy", (void**)&av_frame_copy, Dylib::Import::functionStub },
  { "av_frame_copy_props", (void**)&av_frame_copy_props, Dylib::Import::functionStub },
  { "av_frame_free", (void**)&av_frame_free, Dylib::Import::functionStub },
  { "av_frame_get_buffer", (void**)&av_frame_get_buffer, Dylib::Import::functionStub },
  { "av_frame_get_plane_buffer", (void**)&av_frame_get_plane_buffer, Dylib::Import::functionStub },
  { "av_frame_get_side_data", (void**)&av_frame_get_side_data, Dylib::Import::functionStub },
  { "av_frame_is_writable", (void**)&av_frame_is_writable, Dylib::Import::functionStub },
  { "av_frame_make_writable", (void**)&av_frame_make_writable, Dylib::Import::functionStub },
  { "av_frame_move_ref", (void**)&av_frame_move_ref, Dylib::Import::functionStub },
  { "av_frame_new_side_data", (void**)&av_frame_new_side_data, Dylib::Import::functionStub },
  { "av_frame_new_side_data_from_buf", (void**)&av_frame_new_side_data_from_buf, Dylib::Import::functionStub },
  { "av_frame_ref", (void**)&av_frame_ref, Dylib::Import::functionStub },
  { "av_frame_remove_side_data", (void**)&av_frame_remove_side_data, Dylib::Import::functionStub },
  { "av_frame_replace", (void**)&av_frame_replace, Dylib::Import::functionStub },
  { "av_frame_side_data_clone", (void**)&av_frame_side_data_clone, Dylib::Import::functionStub },
  { "av_frame_side_data_free", (void**)&av_frame_side_data_free, Dylib::Import::functionStub },
  { "av_frame_side_data_get_c", (void**)&av_frame_side_data_get_c, Dylib::Import::functionStub },
  { "av_frame_side_data_name", (void**)&av_frame_side_data_name, Dylib::Import::functionStub },
  { "av_frame_side_data_new", (void**)&av_frame_side_data_new, Dylib::Import::functionStub },
  { "av_frame_unref", (void**)&av_frame_unref, Dylib::Import::functionStub },
  { "av_free", (void**)&av_free, Dylib::Import::functionStub },
  { "av_freep", (void**)&av_freep, Dylib::Import::functionStub },
  { "av_gcd", (void**)&av_gcd, Dylib::Import::functionStub },
  { "av_gcd_q", (void**)&av_gcd_q, Dylib::Import::functionStub },
  { "av_get_alt_sample_fmt", (void**)&av_get_alt_sample_fmt, Dylib::Import::functionStub },
  { "av_get_bits_per_pixel", (void**)&av_get_bits_per_pixel, Dylib::Import::functionStub },
  { "av_get_bytes_per_sample", (void**)&av_get_bytes_per_sample, Dylib::Import::functionStub },
  { "av_get_cpu_flags", (void**)&av_get_cpu_flags, Dylib::Import::functionStub },
  { "av_get_known_color_name", (void**)&av_get_known_color_name, Dylib::Import::functionStub },
  { "av_get_media_type_string", (void**)&av_get_media_type_string, Dylib::Import::functionStub },
  { "av_get_packed_sample_fmt", (void**)&av_get_packed_sample_fmt, Dylib::Import::functionStub },
  { "av_get_padded_bits_per_pixel", (void**)&av_get_padded_bits_per_pixel, Dylib::Import::functionStub },
  { "av_get_picture_type_char", (void**)&av_get_picture_type_char, Dylib::Import::functionStub },
  { "av_get_pix_fmt", (void**)&av_get_pix_fmt, Dylib::Import::functionStub },
  { "av_get_pix_fmt_loss", (void**)&av_get_pix_fmt_loss, Dylib::Import::functionStub },
  { "av_get_pix_fmt_name", (void**)&av_get_pix_fmt_name, Dylib::Import::functionStub },
  { "av_get_pix_fmt_string", (void**)&av_get_pix_fmt_string, Dylib::Import::functionStub },
  { "av_get_planar_sample_fmt", (void**)&av_get_planar_sample_fmt, Dylib::Import::functionStub },
  { "av_get_random_seed", (void**)&av_get_random_seed, Dylib::Import::functionStub },
  { "av_get_sample_fmt", (void**)&av_get_sample_fmt, Dylib::Import::functionStub },
  { "av_get_sample_fmt_name", (void**)&av_get_sample_fmt_name, Dylib::Import::functionStub },
  { "av_get_sample_fmt_string", (void**)&av_get_sample_fmt_string, Dylib::Import::functionStub },
  { "av_get_time_base_q", (void**)&av_get_time_base_q, Dylib::Import::functionStub },
  { "av_get_token", (void**)&av_get_token, Dylib::Import::functionStub },
  { "av_gettime", (void**)&av_gettime, Dylib::Import::functionStub },
  { "av_gettime_relative", (void**)&av_gettime_relative, Dylib::Import::functionStub },
  { "av_gettime_relative_is_monotonic", (void**)&av_gettime_relative_is_monotonic, Dylib::Import::functionStub },
  { "av_hash_alloc", (void**)&av_hash_alloc, Dylib::Import::functionStub },
  { "av_hash_final", (void**)&av_hash_final, Dylib::Import::functionStub },
  { "av_hash_final_b64", (void**)&av_hash_final_b64, Dylib::Import::functionStub },
  { "av_hash_final_bin", (void**)&av_hash_final_bin, Dylib::Import::functionStub },
  { "av_hash_final_hex", (void**)&av_hash_final_hex, Dylib::Import::functionStub },
  { "av_hash_freep", (void**)&av_hash_freep, Dylib::Import::functionStub },
  { "av_hash_get_name", (void**)&av_hash_get_name, Dylib::Import::functionStub },
  { "av_hash_get_size", (void**)&av_hash_get_size, Dylib::Import::functionStub },
  { "av_hash_init", (void**)&av_hash_init, Dylib::Import::functionStub },
  { "av_hash_names", (void**)&av_hash_names, Dylib::Import::functionStub },
  { "av_hash_update", (void**)&av_hash_update, Dylib::Import::functionStub },
  { "av_hmac_alloc", (void**)&av_hmac_alloc, Dylib::Import::functionStub },
  { "av_hmac_calc", (void**)&av_hmac_calc, Dylib::Import::functionStub },
  { "av_hmac_final", (void**)&av_hmac_final, Dylib::Import::functionStub },
  { "av_hmac_free", (void**)&av_hmac_free, Dylib::Import::functionStub },
  { "av_hmac_init", (void**)&av_hmac_init, Dylib::Import::functionStub },
  { "av_hmac_update", (void**)&av_hmac_update, Dylib::Import::functionStub },
  { "av_hwdevice_ctx_alloc", (void**)&av_hwdevice_ctx_alloc, Dylib::Import::functionStub },
  { "av_hwdevice_ctx_create", (void**)&av_hwdevice_ctx_create, Dylib::Import::functionStub },
  { "av_hwdevice_ctx_create_derived", (void**)&av_hwdevice_ctx_create_derived, Dylib::Import::functionStub },
  { "av_hwdevice_ctx_create_derived_opts", (void**)&av_hwdevice_ctx_create_derived_opts, Dylib::Import::functionStub },
  { "av_hwdevice_ctx_init", (void**)&av_hwdevice_ctx_init, Dylib::Import::functionStub },
  { "av_hwdevice_find_type_by_name", (void**)&av_hwdevice_find_type_by_name, Dylib::Import::functionStub },
  { "av_hwdevice_get_hwframe_constraints", (void**)&av_hwdevice_get_hwframe_constraints, Dylib::Import::functionStub },
  { "av_hwdevice_get_type_name", (void**)&av_hwdevice_get_type_name, Dylib::Import::functionStub },
  { "av_hwdevice_hwconfig_alloc", (void**)&av_hwdevice_hwconfig_alloc, Dylib::Import::functionStub },
  { "av_hwdevice_iterate_types", (void**)&av_hwdevice_iterate_types, Dylib::Import::functionStub },
  { "av_hwframe_constraints_free", (void**)&av_hwframe_constraints_free, Dylib::Import::functionStub },
  { "av_hwframe_ctx_alloc", (void**)&av_hwframe_ctx_alloc, Dylib::Import::functionStub },
  { "av_hwframe_ctx_create_derived", (void**)&av_hwframe_ctx_create_derived, Dylib::Import::functionStub },
  { "av_hwframe_ctx_init", (void**)&av_hwframe_ctx_init, Dylib::Import::functionStub },
  { "av_hwframe_get_buffer", (void**)&av_hwframe_get_buffer, Dylib::Import::functionStub },
  { "av_hwframe_map", (void**)&av_hwframe_map, Dylib::Import::functionStub },
  { "av_hwframe_transfer_data", (void**)&av_hwframe_transfer_data, Dylib::Import::functionStub },
  { "av_hwframe_transfer_get_formats", (void**)&av_hwframe_transfer_get_formats, Dylib::Import::functionStub },
  { "av_i2int", (void**)&av_i2int, Dylib::Import::functionStub },
  { "av_iamf_audio_element_add_layer", (void**)&av_iamf_audio_element_add_layer, Dylib::Import::functionStub },
  { "av_iamf_audio_element_alloc", (void**)&av_iamf_audio_element_alloc, Dylib::Import::functionStub },
  { "av_iamf_audio_element_free", (void**)&av_iamf_audio_element_free, Dylib::Import::functionStub },
  { "av_iamf_audio_element_get_class", (void**)&av_iamf_audio_element_get_class, Dylib::Import::functionStub },
  { "av_iamf_mix_presentation_add_submix", (void**)&av_iamf_mix_presentation_add_submix, Dylib::Import::functionStub },
  { "av_iamf_mix_presentation_alloc", (void**)&av_iamf_mix_presentation_alloc, Dylib::Import::functionStub },
  { "av_iamf_mix_presentation_free", (void**)&av_iamf_mix_presentation_free, Dylib::Import::functionStub },
  { "av_iamf_mix_presentation_get_class", (void**)&av_iamf_mix_presentation_get_class, Dylib::Import::functionStub },
  { "av_iamf_param_definition_alloc", (void**)&av_iamf_param_definition_alloc, Dylib::Import::functionStub },
  { "av_iamf_param_definition_get_class", (void**)&av_iamf_param_definition_get_class, Dylib::Import::functionStub },
  { "av_iamf_submix_add_element", (void**)&av_iamf_submix_add_element, Dylib::Import::functionStub },
  { "av_iamf_submix_add_layout", (void**)&av_iamf_submix_add_layout, Dylib::Import::functionStub },
  { "av_image_alloc", (void**)&av_image_alloc, Dylib::Import::functionStub },
  { "av_image_check_sar", (void**)&av_image_check_sar, Dylib::Import::functionStub },
  { "av_image_check_size", (void**)&av_image_check_size, Dylib::Import::functionStub },
  { "av_image_check_size2", (void**)&av_image_check_size2, Dylib::Import::functionStub },
  { "av_image_copy", (void**)&av_image_copy, Dylib::Import::functionStub },
  { "av_image_copy_plane", (void**)&av_image_copy_plane, Dylib::Import::functionStub },
  { "av_image_copy_plane_uc_from", (void**)&av_image_copy_plane_uc_from, Dylib::Import::functionStub },
  { "av_image_copy_to_buffer", (void**)&av_image_copy_to_buffer, Dylib::Import::functionStub },
  { "av_image_copy_uc_from", (void**)&av_image_copy_uc_from, Dylib::Import::functionStub },
  { "av_image_fill_arrays", (void**)&av_image_fill_arrays, Dylib::Import::functionStub },
  { "av_image_fill_black", (void**)&av_image_fill_black, Dylib::Import::functionStub },
  { "av_image_fill_color", (void**)&av_image_fill_color, Dylib::Import::functionStub },
  { "av_image_fill_linesizes", (void**)&av_image_fill_linesizes, Dylib::Import::functionStub },
  { "av_image_fill_max_pixsteps", (void**)&av_image_fill_max_pixsteps, Dylib::Import::functionStub },
  { "av_image_fill_plane_sizes", (void**)&av_image_fill_plane_sizes, Dylib::Import::functionStub },
  { "av_image_fill_pointers", (void**)&av_image_fill_pointers, Dylib::Import::functionStub },
  { "av_image_get_buffer_size", (void**)&av_image_get_buffer_size, Dylib::Import::functionStub },
  { "av_image_get_linesize", (void**)&av_image_get_linesize, Dylib::Import::functionStub },
  { "av_int2i", (void**)&av_int2i, Dylib::Import::functionStub },
  { "av_int_list_length_for_size", (void**)&av_int_list_length_for_size, Dylib::Import::functionStub },
  { "av_lfg_init", (void**)&av_lfg_init, Dylib::Import::functionStub },
  { "av_lfg_init_from_data", (void**)&av_lfg_init_from_data, Dylib::Import::functionStub },
  { "av_log", (void**)&av_log, Dylib::Import::functionStub },
  { "av_log2", (void**)&av_log2, Dylib::Import::functionStub },
  { "av_log2_16bit", (void**)&av_log2_16bit, Dylib::Import::functionStub },
  { "av_log2_i", (void**)&av_log2_i, Dylib::Import::functionStub },
  { "av_log_default_callback", (void**)&av_log_default_callback, Dylib::Import::functionStub },
  { "av_log_format_line", (void**)&av_log_format_line, Dylib::Import::functionStub },
  { "av_log_format_line2", (void**)&av_log_format_line2, Dylib::Import::functionStub },
  { "av_log_get_flags", (void**)&av_log_get_flags, Dylib::Import::functionStub },
  { "av_log_get_level", (void**)&av_log_get_level, Dylib::Import::functionStub },
  { "av_log_once", (void**)&av_log_once, Dylib::Import::functionStub },
  { "av_log_set_callback", (void**)&av_log_set_callback, Dylib::Import::functionStub },
  { "av_log_set_flags", (void**)&av_log_set_flags, Dylib::Import::functionStub },
  { "av_log_set_level", (void**)&av_log_set_level, Dylib::Import::functionStub },
  { "av_lzo1x_decode", (void**)&av_lzo1x_decode, Dylib::Import::functionStub },
  { "av_malloc", (void**)&av_malloc, Dylib::Import::functionStub },
  { "av_malloc_array", (void**)&av_malloc_array, Dylib::Import::functionStub },
  { "av_mallocz", (void**)&av_mallocz, Dylib::Import::functionStub },
  { "av_mastering_display_metadata_alloc", (void**)&av_mastering_display_metadata_alloc, Dylib::Import::functionStub },
  { "av_mastering_display_metadata_create_side_data", (void**)&av_mastering_display_metadata_create_side_data, Dylib::Import::functionStub },
  { "av_match_list", (void**)&av_match_list, Dylib::Import::functionStub },
  { "av_match_name", (void**)&av_match_name, Dylib::Import::functionStub },
  { "av_max_alloc", (void**)&av_max_alloc, Dylib::Import::functionStub },
  { "av_md5_alloc", (void**)&av_md5_alloc, Dylib::Import::functionStub },
  { "av_md5_final", (void**)&av_md5_final, Dylib::Import::functionStub },
  { "av_md5_init", (void**)&av_md5_init, Dylib::Import::functionStub },
  { "av_md5_sum", (void**)&av_md5_sum, Dylib::Import::functionStub },
  { "av_md5_update", (void**)&av_md5_update, Dylib::Import::functionStub },
  { "av_memcpy_backptr", (void**)&av_memcpy_backptr, Dylib::Import::functionStub },
  { "av_memdup", (void**)&av_memdup, Dylib::Import::functionStub },
  { "av_mod_i", (void**)&av_mod_i, Dylib::Import::functionStub },
  { "av_mul_i", (void**)&av_mul_i, Dylib::Import::functionStub },
  { "av_mul_q", (void**)&av_mul_q, Dylib::Import::functionStub },
  { "av_murmur3_alloc", (void**)&av_murmur3_alloc, Dylib::Import::functionStub },
  { "av_murmur3_final", (void**)&av_murmur3_final, Dylib::Import::functionStub },
  { "av_murmur3_init", (void**)&av_murmur3_init, Dylib::Import::functionStub },
  { "av_murmur3_init_seeded", (void**)&av_murmur3_init_seeded, Dylib::Import::functionStub },
  { "av_murmur3_update", (void**)&av_murmur3_update, Dylib::Import::functionStub },
  { "av_nearer_q", (void**)&av_nearer_q, Dylib::Import::functionStub },
  { "av_opt_child_class_iterate", (void**)&av_opt_child_class_iterate, Dylib::Import::functionStub },
  { "av_opt_child_next", (void**)&av_opt_child_next, Dylib::Import::functionStub },
  { "av_opt_copy", (void**)&av_opt_copy, Dylib::Import::functionStub },
  { "av_opt_eval_double", (void**)&av_opt_eval_double, Dylib::Import::functionStub },
  { "av_opt_eval_flags", (void**)&av_opt_eval_flags, Dylib::Import::functionStub },
  { "av_opt_eval_float", (void**)&av_opt_eval_float, Dylib::Import::functionStub },
  { "av_opt_eval_int", (void**)&av_opt_eval_int, Dylib::Import::functionStub },
  { "av_opt_eval_int64", (void**)&av_opt_eval_int64, Dylib::Import::functionStub },
  { "av_opt_eval_q", (void**)&av_opt_eval_q, Dylib::Import::functionStub },
  { "av_opt_find", (void**)&av_opt_find, Dylib::Import::functionStub },
  { "av_opt_find2", (void**)&av_opt_find2, Dylib::Import::functionStub },
  { "av_opt_flag_is_set", (void**)&av_opt_flag_is_set, Dylib::Import::functionStub },
  { "av_opt_free", (void**)&av_opt_free, Dylib::Import::functionStub },
  { "av_opt_freep_ranges", (void**)&av_opt_freep_ranges, Dylib::Import::functionStub },
  { "av_opt_get", (void**)&av_opt_get, Dylib::Import::functionStub },
  { "av_opt_get_chlayout", (void**)&av_opt_get_chlayout, Dylib::Import::functionStub },
  { "av_opt_get_dict_val", (void**)&av_opt_get_dict_val, Dylib::Import::functionStub },
  { "av_opt_get_double", (void**)&av_opt_get_double, Dylib::Import::functionStub },
  { "av_opt_get_image_size", (void**)&av_opt_get_image_size, Dylib::Import::functionStub },
  { "av_opt_get_int", (void**)&av_opt_get_int, Dylib::Import::functionStub },
  { "av_opt_get_key_value", (void**)&av_opt_get_key_value, Dylib::Import::functionStub },
  { "av_opt_get_pixel_fmt", (void**)&av_opt_get_pixel_fmt, Dylib::Import::functionStub },
  { "av_opt_get_q", (void**)&av_opt_get_q, Dylib::Import::functionStub },
  { "av_opt_get_sample_fmt", (void**)&av_opt_get_sample_fmt, Dylib::Import::functionStub },
  { "av_opt_get_video_rate", (void**)&av_opt_get_video_rate, Dylib::Import::functionStub },
  { "av_opt_is_set_to_default", (void**)&av_opt_is_set_to_default, Dylib::Import::functionStub },
  { "av_opt_is_set_to_default_by_name", (void**)&av_opt_is_set_to_default_by_name, Dylib::Import::functionStub },
  { "av_opt_next", (void**)&av_opt_next, Dylib::Import::functionStub },
  { "av_opt_ptr", (void**)&av_opt_ptr, Dylib::Import::functionStub },
  { "av_opt_query_ranges", (void**)&av_opt_query_ranges, Dylib::Import::functionStub },
  { "av_opt_query_ranges_default", (void**)&av_opt_query_ranges_default, Dylib::Import::functionStub },
  { "av_opt_serialize", (void**)&av_opt_serialize, Dylib::Import::functionStub },
  { "av_opt_set", (void**)&av_opt_set, Dylib::Import::functionStub },
  { "av_opt_set_bin", (void**)&av_opt_set_bin, Dylib::Import::functionStub },
  { "av_opt_set_chlayout", (void**)&av_opt_set_chlayout, Dylib::Import::functionStub },
  { "av_opt_set_defaults", (void**)&av_opt_set_defaults, Dylib::Import::functionStub },
  { "av_opt_set_defaults2", (void**)&av_opt_set_defaults2, Dylib::Import::functionStub },
  { "av_opt_set_dict", (void**)&av_opt_set_dict, Dylib::Import::functionStub },
  { "av_opt_set_dict2", (void**)&av_opt_set_dict2, Dylib::Import::functionStub },
  { "av_opt_set_dict_val", (void**)&av_opt_set_dict_val, Dylib::Import::functionStub },
  { "av_opt_set_double", (void**)&av_opt_set_double, Dylib::Import::functionStub },
  { "av_opt_set_from_string", (void**)&av_opt_set_from_string, Dylib::Import::functionStub },
  { "av_opt_set_image_size", (void**)&av_opt_set_image_size, Dylib::Import::functionStub },
  { "av_opt_set_int", (void**)&av_opt_set_int, Dylib::Import::functionStub },
  { "av_opt_set_pixel_fmt", (void**)&av_opt_set_pixel_fmt, Dylib::Import::functionStub },
  { "av_opt_set_q", (void**)&av_opt_set_q, Dylib::Import::functionStub },
  { "av_opt_set_sample_fmt", (void**)&av_opt_set_sample_fmt, Dylib::Import::functionStub },
  { "av_opt_set_video_rate", (void**)&av_opt_set_video_rate, Dylib::Import::functionStub },
  { "av_opt_show2", (void**)&av_opt_show2, Dylib::Import::functionStub },
  { "av_parse_color", (void**)&av_parse_color, Dylib::Import::functionStub },
  { "av_parse_cpu_caps", (void**)&av_parse_cpu_caps, Dylib::Import::functionStub },
  { "av_parse_ratio", (void**)&av_parse_ratio, Dylib::Import::functionStub },
  { "av_parse_time", (void**)&av_parse_time, Dylib::Import::functionStub },
  { "av_parse_video_rate", (void**)&av_parse_video_rate, Dylib::Import::functionStub },
  { "av_parse_video_size", (void**)&av_parse_video_size, Dylib::Import::functionStub },
  { "av_pix_fmt_count_planes", (void**)&av_pix_fmt_count_planes, Dylib::Import::functionStub },
  { "av_pix_fmt_desc_get", (void**)&av_pix_fmt_desc_get, Dylib::Import::functionStub },
  { "av_pix_fmt_desc_get_id", (void**)&av_pix_fmt_desc_get_id, Dylib::Import::functionStub },
  { "av_pix_fmt_desc_next", (void**)&av_pix_fmt_desc_next, Dylib::Import::functionStub },
  { "av_pix_fmt_get_chroma_sub_sample", (void**)&av_pix_fmt_get_chroma_sub_sample, Dylib::Import::functionStub },
  { "av_pix_fmt_swap_endianness", (void**)&av_pix_fmt_swap_endianness, Dylib::Import::functionStub },
  { "av_pixelutils_get_sad_fn", (void**)&av_pixelutils_get_sad_fn, Dylib::Import::functionStub },
  { "av_q2intfloat", (void**)&av_q2intfloat, Dylib::Import::functionStub },
  { "av_random_bytes", (void**)&av_random_bytes, Dylib::Import::functionStub },
  { "av_rc4_alloc", (void**)&av_rc4_alloc, Dylib::Import::functionStub },
  { "av_rc4_crypt", (void**)&av_rc4_crypt, Dylib::Import::functionStub },
  { "av_rc4_init", (void**)&av_rc4_init, Dylib::Import::functionStub },
  { "av_read_image_line", (void**)&av_read_image_line, Dylib::Import::functionStub },
  { "av_read_image_line2", (void**)&av_read_image_line2, Dylib::Import::functionStub },
  { "av_realloc", (void**)&av_realloc, Dylib::Import::functionStub },
  { "av_realloc_array", (void**)&av_realloc_array, Dylib::Import::functionStub },
  { "av_realloc_f", (void**)&av_realloc_f, Dylib::Import::functionStub },
  { "av_reallocp", (void**)&av_reallocp, Dylib::Import::functionStub },
  { "av_reallocp_array", (void**)&av_reallocp_array, Dylib::Import::functionStub },
  { "av_reduce", (void**)&av_reduce, Dylib::Import::functionStub },
  { "av_rescale", (void**)&av_rescale, Dylib::Import::functionStub },
  { "av_rescale_delta", (void**)&av_rescale_delta, Dylib::Import::functionStub },
  { "av_rescale_q", (void**)&av_rescale_q, Dylib::Import::functionStub },
  { "av_rescale_q_rnd", (void**)&av_rescale_q_rnd, Dylib::Import::functionStub },
  { "av_rescale_rnd", (void**)&av_rescale_rnd, Dylib::Import::functionStub },
  { "av_ripemd_alloc", (void**)&av_ripemd_alloc, Dylib::Import::functionStub },
  { "av_ripemd_final", (void**)&av_ripemd_final, Dylib::Import::functionStub },
  { "av_ripemd_init", (void**)&av_ripemd_init, Dylib::Import::functionStub },
  { "av_ripemd_update", (void**)&av_ripemd_update, Dylib::Import::functionStub },
  { "av_sample_fmt_is_planar", (void**)&av_sample_fmt_is_planar, Dylib::Import::functionStub },
  { "av_samples_alloc", (void**)&av_samples_alloc, Dylib::Import::functionStub },
  { "av_samples_alloc_array_and_samples", (void**)&av_samples_alloc_array_and_samples, Dylib::Import::functionStub },
  { "av_samples_copy", (void**)&av_samples_copy, Dylib::Import::functionStub },
  { "av_samples_fill_arrays", (void**)&av_samples_fill_arrays, Dylib::Import::functionStub },
  { "av_samples_get_buffer_size", (void**)&av_samples_get_buffer_size, Dylib::Import::functionStub },
  { "av_samples_set_silence", (void**)&av_samples_set_silence, Dylib::Import::functionStub },
  { "av_set_options_string", (void**)&av_set_options_string, Dylib::Import::functionStub },
  { "av_sha512_alloc", (void**)&av_sha512_alloc, Dylib::Import::functionStub },
  { "av_sha512_final", (void**)&av_sha512_final, Dylib::Import::functionStub },
  { "av_sha512_init", (void**)&av_sha512_init, Dylib::Import::functionStub },
  { "av_sha512_update", (void**)&av_sha512_update, Dylib::Import::functionStub },
  { "av_sha_alloc", (void**)&av_sha_alloc, Dylib::Import::functionStub },
  { "av_sha_final", (void**)&av_sha_final, Dylib::Import::functionStub },
  { "av_sha_init", (void**)&av_sha_init, Dylib::Import::functionStub },
  { "av_sha_update", (void**)&av_sha_update, Dylib::Import::functionStub },
  { "av_shr_i", (void**)&av_shr_i, Dylib::Import::functionStub },
  { "av_size_mult", (void**)&av_size_mult, Dylib::Import::functionStub },
  { "av_small_strptime", (void**)&av_small_strptime, Dylib::Import::functionStub },
  { "av_spherical_alloc", (void**)&av_spherical_alloc, Dylib::Import::functionStub },
  { "av_spherical_from_name", (void**)&av_spherical_from_name, Dylib::Import::functionStub },
  { "av_spherical_projection_name", (void**)&av_spherical_projection_name, Dylib::Import::functionStub },
  { "av_spherical_tile_bounds", (void**)&av_spherical_tile_bounds, Dylib::Import::functionStub },
  { "av_sscanf", (void**)&av_sscanf, Dylib::Import::functionStub },
  { "av_stereo3d_alloc", (void**)&av_stereo3d_alloc, Dylib::Import::functionStub },
  { "av_stereo3d_create_side_data", (void**)&av_stereo3d_create_side_data, Dylib::Import::functionStub },
  { "av_stereo3d_from_name", (void**)&av_stereo3d_from_name, Dylib::Import::functionStub },
  { "av_stereo3d_type_name", (void**)&av_stereo3d_type_name, Dylib::Import::functionStub },
  { "av_strcasecmp", (void**)&av_strcasecmp, Dylib::Import::functionStub },
  { "av_strdup", (void**)&av_strdup, Dylib::Import::functionStub },
  { "av_strerror", (void**)&av_strerror, Dylib::Import::functionStub },
  { "av_strireplace", (void**)&av_strireplace, Dylib::Import::functionStub },
  { "av_stristart", (void**)&av_stristart, Dylib::Import::functionStub },
  { "av_stristr", (void**)&av_stristr, Dylib::Import::functionStub },
  { "av_strlcat", (void**)&av_strlcat, Dylib::Import::functionStub },
  { "av_strlcatf", (void**)&av_strlcatf, Dylib::Import::functionStub },
  { "av_strlcpy", (void**)&av_strlcpy, Dylib::Import::functionStub },
  { "av_strncasecmp", (void**)&av_strncasecmp, Dylib::Import::functionStub },
  { "av_strndup", (void**)&av_strndup, Dylib::Import::functionStub },
  { "av_strnstr", (void**)&av_strnstr, Dylib::Import::functionStub },
  { "av_strstart", (void**)&av_strstart, Dylib::Import::functionStub },
  { "av_strtod", (void**)&av_strtod, Dylib::Import::functionStub },
  { "av_strtok", (void**)&av_strtok, Dylib::Import::functionStub },
  { "av_sub_i", (void**)&av_sub_i, Dylib::Import::functionStub },
  { "av_sub_q", (void**)&av_sub_q, Dylib::Import::functionStub },
  { "av_tea_alloc", (void**)&av_tea_alloc, Dylib::Import::functionStub },
  { "av_tea_crypt", (void**)&av_tea_crypt, Dylib::Import::functionStub },
  { "av_tea_init", (void**)&av_tea_init, Dylib::Import::functionStub },
  { "av_thread_message_flush", (void**)&av_thread_message_flush, Dylib::Import::functionStub },
  { "av_thread_message_queue_alloc", (void**)&av_thread_message_queue_alloc, Dylib::Import::functionStub },
  { "av_thread_message_queue_free", (void**)&av_thread_message_queue_free, Dylib::Import::functionStub },
  { "av_thread_message_queue_nb_elems", (void**)&av_thread_message_queue_nb_elems, Dylib::Import::functionStub },
  { "av_thread_message_queue_recv", (void**)&av_thread_message_queue_recv, Dylib::Import::functionStub },
  { "av_thread_message_queue_send", (void**)&av_thread_message_queue_send, Dylib::Import::functionStub },
  { "av_thread_message_queue_set_err_recv", (void**)&av_thread_message_queue_set_err_recv, Dylib::Import::functionStub },
  { "av_thread_message_queue_set_err_send", (void**)&av_thread_message_queue_set_err_send, Dylib::Import::functionStub },
  { "av_thread_message_queue_set_free_func", (void**)&av_thread_message_queue_set_free_func, Dylib::Import::functionStub },
  { "av_timecode_adjust_ntsc_framenum2", (void**)&av_timecode_adjust_ntsc_framenum2, Dylib::Import::functionStub },
  { "av_timecode_check_frame_rate", (void**)&av_timecode_check_frame_rate, Dylib::Import::functionStub },
  { "av_timecode_get_smpte", (void**)&av_timecode_get_smpte, Dylib::Import::functionStub },
  { "av_timecode_get_smpte_from_framenum", (void**)&av_timecode_get_smpte_from_framenum, Dylib::Import::functionStub },
  { "av_timecode_init", (void**)&av_timecode_init, Dylib::Import::functionStub },
  { "av_timecode_init_from_components", (void**)&av_timecode_init_from_components, Dylib::Import::functionStub },
  { "av_timecode_init_from_string", (void**)&av_timecode_init_from_string, Dylib::Import::functionStub },
  { "av_timecode_make_mpeg_tc_string", (void**)&av_timecode_make_mpeg_tc_string, Dylib::Import::functionStub },
  { "av_timecode_make_smpte_tc_string", (void**)&av_timecode_make_smpte_tc_string, Dylib::Import::functionStub },
  { "av_timecode_make_smpte_tc_string2", (void**)&av_timecode_make_smpte_tc_string2, Dylib::Import::functionStub },
  { "av_timecode_make_string", (void**)&av_timecode_make_string, Dylib::Import::functionStub },
  { "av_timegm", (void**)&av_timegm, Dylib::Import::functionStub },
  { "av_tree_destroy", (void**)&av_tree_destroy, Dylib::Import::functionStub },
  { "av_tree_enumerate", (void**)&av_tree_enumerate, Dylib::Import::functionStub },
  { "av_tree_find", (void**)&av_tree_find, Dylib::Import::functionStub },
  { "av_tree_insert", (void**)&av_tree_insert, Dylib::Import::functionStub },
  { "av_tree_node_alloc", (void**)&av_tree_node_alloc, Dylib::Import::functionStub },
  { "av_ts_make_time_string2", (void**)&av_ts_make_time_string2, Dylib::Import::functionStub },
  { "av_twofish_alloc", (void**)&av_twofish_alloc, Dylib::Import::functionStub },
  { "av_twofish_crypt", (void**)&av_twofish_crypt, Dylib::Import::functionStub },
  { "av_twofish_init", (void**)&av_twofish_init, Dylib::Import::functionStub },
  { "av_tx_init", (void**)&av_tx_init, Dylib::Import::functionStub },
  { "av_tx_uninit", (void**)&av_tx_uninit, Dylib::Import::functionStub },
  { "av_usleep", (void**)&av_usleep, Dylib::Import::functionStub },
  { "av_utf8_decode", (void**)&av_utf8_decode, Dylib::Import::functionStub },
  { "av_uuid_parse", (void**)&av_uuid_parse, Dylib::Import::functionStub },
  { "av_uuid_parse_range", (void**)&av_uuid_parse_range, Dylib::Import::functionStub },
  { "av_uuid_unparse", (void**)&av_uuid_unparse, Dylib::Import::functionStub },
  { "av_uuid_urn_parse", (void**)&av_uuid_urn_parse, Dylib::Import::functionStub },
  { "av_vbprintf", (void**)&av_vbprintf, Dylib::Import::functionStub },
  { "av_version_info", (void**)&av_version_info, Dylib::Import::functionStub },
  { "av_video_enc_params_alloc", (void**)&av_video_enc_params_alloc, Dylib::Import::functionStub },
  { "av_video_enc_params_create_side_data", (void**)&av_video_enc_params_create_side_data, Dylib::Import::functionStub },
  { "av_video_hint_alloc", (void**)&av_video_hint_alloc, Dylib::Import::functionStub },
  { "av_video_hint_create_side_data", (void**)&av_video_hint_create_side_data, Dylib::Import::functionStub },
  { "av_vk_frame_alloc", (void**)&av_vk_frame_alloc, Dylib::Import::functionStub },
  { "av_vkfmt_from_pixfmt", (void**)&av_vkfmt_from_pixfmt, Dylib::Import::functionStub },
  { "av_vlog", (void**)&av_vlog, Dylib::Import::functionStub },
  { "av_write_image_line", (void**)&av_write_image_line, Dylib::Import::functionStub },
  { "av_write_image_line2", (void**)&av_write_image_line2, Dylib::Import::functionStub },
  { "av_xtea_alloc", (void**)&av_xtea_alloc, Dylib::Import::functionStub },
  { "av_xtea_crypt", (void**)&av_xtea_crypt, Dylib::Import::functionStub },
  { "av_xtea_init", (void**)&av_xtea_init, Dylib::Import::functionStub },
  { "av_xtea_le_crypt", (void**)&av_xtea_le_crypt, Dylib::Import::functionStub },
  { "av_xtea_le_init", (void**)&av_xtea_le_init, Dylib::Import::functionStub },
  { "avpriv_alloc_fixed_dsp", (void**)&avpriv_alloc_fixed_dsp, Dylib::Import::functionStub },
  { "avpriv_dict_set_timestamp", (void**)&avpriv_dict_set_timestamp, Dylib::Import::functionStub },
  { "avpriv_emms_asm", (void**)&avpriv_emms_asm, Dylib::Import::functionStub },
  { "avpriv_float_dsp_alloc", (void**)&avpriv_float_dsp_alloc, Dylib::Import::functionStub },
  { "avpriv_init_lls", (void**)&avpriv_init_lls, Dylib::Import::functionStub },
  { "avpriv_report_missing_feature", (void**)&avpriv_report_missing_feature, Dylib::Import::functionStub },
  { "avpriv_request_sample", (void**)&avpriv_request_sample, Dylib::Import::functionStub },
  { "avpriv_scalarproduct_float_c", (void**)&avpriv_scalarproduct_float_c, Dylib::Import::functionStub },
  { "avpriv_set_systematic_pal2", (void**)&avpriv_set_systematic_pal2, Dylib::Import::functionStub },
  { "avpriv_slicethread_create", (void**)&avpriv_slicethread_create, Dylib::Import::functionStub },
  { "avpriv_slicethread_execute", (void**)&avpriv_slicethread_execute, Dylib::Import::functionStub },
  { "avpriv_slicethread_free", (void**)&avpriv_slicethread_free, Dylib::Import::functionStub },
  { "avpriv_solve_lls", (void**)&avpriv_solve_lls, Dylib::Import::functionStub },
  { "avutil_configuration", (void**)&avutil_configuration, Dylib::Import::functionStub },
  { "avutil_license", (void**)&avutil_license, Dylib::Import::functionStub },
  { "avutil_version", (void**)&avutil_version, Dylib::Import::functionStub },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBAVUTIL_59

