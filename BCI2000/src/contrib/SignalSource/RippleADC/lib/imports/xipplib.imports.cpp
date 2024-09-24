// Import definitions for xipplib library
// generated Sat Sep  3 09:36:20 2022 by DylibTool
#ifdef STATIC_LIBXIPPLIB

namespace Dylib { bool xipplib_Loaded() { return true; } }
void* p_XL_DEFAULT_CONNECTION_OPTIONS = &XL_DEFAULT_CONNECTION_OPTIONS;
void* p_XL_DEFAULT_STATUS_CONNECTION_OPTIONS = &XL_DEFAULT_STATUS_CONNECTION_OPTIONS;


#else // STATIC_LIBXIPPLIB

#include "xipplib.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( xipplib, "xipplib", functionImports, notFoundMsg, notFoundURL );

void* p_XL_DEFAULT_CONNECTION_OPTIONS = Dylib::xipplib.ResolveDataExport( "XL_DEFAULT_CONNECTION_OPTIONS" );
void* p_XL_DEFAULT_STATUS_CONNECTION_OPTIONS = Dylib::xipplib.ResolveDataExport( "XL_DEFAULT_STATUS_CONNECTION_OPTIONS" );
decltype(&__43232a05_xl_audio_tone_set) xl_audio_tone_set = 0;
decltype(&__62765b07_xl_button_get_count) xl_button_get_count = 0;
decltype(&__0b714168_xl_button_set_monitor) xl_button_set_monitor = 0;
decltype(&__14032c06_xl_close) xl_close = 0;
decltype(&__726f4c3c_xl_cont_emg) xl_cont_emg = 0;
decltype(&__1a1a4d4e_xl_cont_hifreq) xl_cont_hifreq = 0;
decltype(&__0c6b5959_xl_cont_hires) xl_cont_hires = 0;
decltype(&__7b645b3c_xl_cont_lfp) xl_cont_lfp = 0;
decltype(&__65635c3c_xl_cont_raw) xl_cont_raw = 0;
decltype(&__11054a48_xl_cont_status) xl_cont_status = 0;
decltype(&__110b360a_xl_digin) xl_digin = 0;
decltype(&__650b3011_xl_digout) xl_digout = 0;
decltype(&__36165872_xl_error_check) xl_error_check = 0;
decltype(&__3a515365_xl_error_copy_message) xl_error_copy_message = 0;
decltype(&__26544e70_xl_error_free_message) xl_error_free_message = 0;
decltype(&__556e5f67_xl_error_pop) xl_error_pop = 0;
decltype(&__41472015_xl_error_register_callback) xl_error_register_callback = 0;
decltype(&__21353071_xl_error_requires_error) xl_error_requires_error = 0;
decltype(&__3c032365_xl_error_string) xl_error_string = 0;
decltype(&__45694a4e_xl_error_unregister_callback) xl_error_unregister_callback = 0;
decltype(&__696a5b3c_xl_external_battery) xl_external_battery = 0;
decltype(&__6e7b4a62_xl_external_battery_detected) xl_external_battery_detected = 0;
decltype(&__061f5f4d_xl_fast_settle) xl_fast_settle = 0;
decltype(&__6e77362a_xl_fast_settle_get_choices) xl_fast_settle_get_choices = 0;
decltype(&__7f76503a_xl_fast_settle_get_duration) xl_fast_settle_get_duration = 0;
decltype(&__0140406c_xl_fe_version) xl_fe_version = 0;
decltype(&__71467d77_xl_filter_list_names) xl_filter_list_names = 0;
decltype(&__012e1214_xl_filter_list_selection) xl_filter_list_selection = 0;
decltype(&__175f5866_xl_filter_set) xl_filter_set = 0;
decltype(&__6474547e_xl_filter_set_custom) xl_filter_set_custom = 0;
decltype(&__78180001_xl_get_fe) xl_get_fe = 0;
decltype(&__79221218_xl_get_fe_streams) xl_get_fe_streams = 0;
decltype(&__6046567e_xl_hw_ground) xl_hw_ground = 0;
decltype(&__07353a68_xl_hw_reference) xl_hw_reference = 0;
decltype(&__151c5f69_xl_imp) xl_imp = 0;
decltype(&__7f6a5b30_xl_internal_battery) xl_internal_battery = 0;
decltype(&__1d085f6c_xl_led) xl_led = 0;
decltype(&__787c001f_xl_led_set) xl_led_set = 0;
decltype(&__78603606_xl_led_set_monitor) xl_led_set_monitor = 0;
decltype(&__1b127373_xl_lib_version) xl_lib_version = 0;
decltype(&__74734e50_xl_list_elec) xl_list_elec = 0;
decltype(&__186e697c_xl_nip_serial) xl_nip_serial = 0;
decltype(&__7e631609_xl_nipexec_version) xl_nipexec_version = 0;
decltype(&__0809316f_xl_open) xl_open = 0;
decltype(&__7c6a4130_xl_open_tcp) xl_open_tcp = 0;
decltype(&__7d6d4130_xl_open_udp) xl_open_udp = 0;
decltype(&__1d023b51_xl_operator_add) xl_operator_add = 0;
decltype(&__6579303a_xl_operator_lookup) xl_operator_lookup = 0;
decltype(&__1c674a74_xl_processor_restart_software) xl_processor_restart_software = 0;
decltype(&__52765406_xl_processor_status) xl_processor_status = 0;
decltype(&__7d0b3112_xl_signal) xl_signal = 0;
decltype(&__0d545d74_xl_signal_lfp) xl_signal_lfp = 0;
decltype(&__0a544373_xl_signal_raw) xl_signal_raw = 0;
decltype(&__0b314273_xl_signal_save) xl_signal_save = 0;
decltype(&__6e451d00_xl_signal_save_set) xl_signal_save_set = 0;
decltype(&__09544277_xl_signal_set) xl_signal_set = 0;
decltype(&__790b2e11_xl_signal_set_lfp) xl_signal_set_lfp = 0;
decltype(&__7e0b3016_xl_signal_set_raw) xl_signal_set_raw = 0;
decltype(&__620b3107_xl_signal_set_spk) xl_signal_set_spk = 0;
decltype(&__60663103_xl_signal_set_stim) xl_signal_set_stim = 0;
decltype(&__16544262_xl_signal_spk) xl_signal_spk = 0;
decltype(&__14394266_xl_signal_stim) xl_signal_stim = 0;
decltype(&__69736117_xl_spk_data) xl_spk_data = 0;
decltype(&__08756574_xl_spk_thresh) xl_spk_thresh = 0;
decltype(&__7c2a1611_xl_spk_thresh_set) xl_spk_thresh_set = 0;
decltype(&__0f1f2744_xl_stim_biphase) xl_stim_biphase = 0;
decltype(&__6864464d_xl_stim_data) xl_stim_data = 0;
decltype(&__050e534e_xl_stim_enable) xl_stim_enable = 0;
decltype(&__607a0c3d_xl_stim_enable_set) xl_stim_enable_set = 0;
decltype(&__7b6b5a12_xl_stim_get_exhaust) xl_stim_get_exhaust = 0;
decltype(&__19053573_xl_stim_get_res) xl_stim_get_res = 0;
decltype(&__6e60683f_xl_stim_header_word) xl_stim_header_word = 0;
decltype(&__7f60432c_xl_stim_seq) xl_stim_seq = 0;
decltype(&__6f6b5a12_xl_stim_set_exhaust) xl_stim_set_exhaust = 0;
decltype(&__0d053573_xl_stim_set_res) xl_stim_set_res = 0;
decltype(&__7b6a4048_xl_stim_word) xl_stim_word = 0;
decltype(&__11013a74_xl_time) xl_time = 0;
decltype(&__0d136a73_xl_transceiver_command) xl_transceiver_command = 0;
decltype(&__00000c13_xl_transceiver_connected) xl_transceiver_connected = 0;
decltype(&__077b6578_xl_transceiver_enable) xl_transceiver_enable = 0;
decltype(&__0b7b7377_xl_transceiver_status) xl_transceiver_status = 0;
decltype(&__0a053e18_xl_trial) xl_trial = 0;
decltype(&__38053e18_xl_trial2) xl_trial2 = 0;
decltype(&__0b66736a_xl_vdd_sensor) xl_vdd_sensor = 0;
decltype(&__05175d5b_xl_wall_sensor) xl_wall_sensor = 0;
decltype(&__0e163354_xl_xipp_version) xl_xipp_version = 0;


namespace {
const Dylib::Import functionImports_[] =
{
  { "xl_audio_tone_set", (void**)&xl_audio_tone_set, Dylib::Import::cMangled },
  { "xl_button_get_count", (void**)&xl_button_get_count, Dylib::Import::cMangled },
  { "xl_button_set_monitor", (void**)&xl_button_set_monitor, Dylib::Import::cMangled },
  { "xl_close", (void**)&xl_close, Dylib::Import::cMangled },
  { "xl_cont_emg", (void**)&xl_cont_emg, Dylib::Import::cMangled },
  { "xl_cont_hifreq", (void**)&xl_cont_hifreq, Dylib::Import::cMangled },
  { "xl_cont_hires", (void**)&xl_cont_hires, Dylib::Import::cMangled },
  { "xl_cont_lfp", (void**)&xl_cont_lfp, Dylib::Import::cMangled },
  { "xl_cont_raw", (void**)&xl_cont_raw, Dylib::Import::cMangled },
  { "xl_cont_status", (void**)&xl_cont_status, Dylib::Import::cMangled },
  { "xl_digin", (void**)&xl_digin, Dylib::Import::cMangled },
  { "xl_digout", (void**)&xl_digout, Dylib::Import::cMangled },
  { "xl_error_check", (void**)&xl_error_check, Dylib::Import::cMangled },
  { "xl_error_copy_message", (void**)&xl_error_copy_message, Dylib::Import::cMangled },
  { "xl_error_free_message", (void**)&xl_error_free_message, Dylib::Import::cMangled },
  { "xl_error_pop", (void**)&xl_error_pop, Dylib::Import::cMangled },
  { "xl_error_register_callback", (void**)&xl_error_register_callback, Dylib::Import::cMangled },
  { "xl_error_requires_error", (void**)&xl_error_requires_error, Dylib::Import::cMangled },
  { "xl_error_string", (void**)&xl_error_string, Dylib::Import::cMangled },
  { "xl_error_unregister_callback", (void**)&xl_error_unregister_callback, Dylib::Import::cMangled },
  { "xl_external_battery", (void**)&xl_external_battery, Dylib::Import::cMangled },
  { "xl_external_battery_detected", (void**)&xl_external_battery_detected, Dylib::Import::cMangled },
  { "xl_fast_settle", (void**)&xl_fast_settle, Dylib::Import::cMangled },
  { "xl_fast_settle_get_choices", (void**)&xl_fast_settle_get_choices, Dylib::Import::cMangled },
  { "xl_fast_settle_get_duration", (void**)&xl_fast_settle_get_duration, Dylib::Import::cMangled },
  { "xl_fe_version", (void**)&xl_fe_version, Dylib::Import::cMangled },
  { "xl_filter_list_names", (void**)&xl_filter_list_names, Dylib::Import::cMangled },
  { "xl_filter_list_selection", (void**)&xl_filter_list_selection, Dylib::Import::cMangled },
  { "xl_filter_set", (void**)&xl_filter_set, Dylib::Import::cMangled },
  { "xl_filter_set_custom", (void**)&xl_filter_set_custom, Dylib::Import::cMangled },
  { "xl_get_fe", (void**)&xl_get_fe, Dylib::Import::cMangled },
  { "xl_get_fe_streams", (void**)&xl_get_fe_streams, Dylib::Import::cMangled },
  { "xl_hw_ground", (void**)&xl_hw_ground, Dylib::Import::cMangled },
  { "xl_hw_reference", (void**)&xl_hw_reference, Dylib::Import::cMangled },
  { "xl_imp", (void**)&xl_imp, Dylib::Import::cMangled },
  { "xl_internal_battery", (void**)&xl_internal_battery, Dylib::Import::cMangled },
  { "xl_led", (void**)&xl_led, Dylib::Import::cMangled },
  { "xl_led_set", (void**)&xl_led_set, Dylib::Import::cMangled },
  { "xl_led_set_monitor", (void**)&xl_led_set_monitor, Dylib::Import::cMangled },
  { "xl_lib_version", (void**)&xl_lib_version, Dylib::Import::cMangled },
  { "xl_list_elec", (void**)&xl_list_elec, Dylib::Import::cMangled },
  { "xl_nip_serial", (void**)&xl_nip_serial, Dylib::Import::cMangled },
  { "xl_nipexec_version", (void**)&xl_nipexec_version, Dylib::Import::cMangled },
  { "xl_open", (void**)&xl_open, Dylib::Import::cMangled },
  { "xl_open_tcp", (void**)&xl_open_tcp, Dylib::Import::cMangled },
  { "xl_open_udp", (void**)&xl_open_udp, Dylib::Import::cMangled },
  { "xl_operator_add", (void**)&xl_operator_add, Dylib::Import::cMangled },
  { "xl_operator_lookup", (void**)&xl_operator_lookup, Dylib::Import::cMangled },
  { "xl_processor_restart_software", (void**)&xl_processor_restart_software, Dylib::Import::cMangled },
  { "xl_processor_status", (void**)&xl_processor_status, Dylib::Import::cMangled },
  { "xl_signal", (void**)&xl_signal, Dylib::Import::cMangled },
  { "xl_signal_lfp", (void**)&xl_signal_lfp, Dylib::Import::cMangled },
  { "xl_signal_raw", (void**)&xl_signal_raw, Dylib::Import::cMangled },
  { "xl_signal_save", (void**)&xl_signal_save, Dylib::Import::cMangled },
  { "xl_signal_save_set", (void**)&xl_signal_save_set, Dylib::Import::cMangled },
  { "xl_signal_set", (void**)&xl_signal_set, Dylib::Import::cMangled },
  { "xl_signal_set_lfp", (void**)&xl_signal_set_lfp, Dylib::Import::cMangled },
  { "xl_signal_set_raw", (void**)&xl_signal_set_raw, Dylib::Import::cMangled },
  { "xl_signal_set_spk", (void**)&xl_signal_set_spk, Dylib::Import::cMangled },
  { "xl_signal_set_stim", (void**)&xl_signal_set_stim, Dylib::Import::cMangled },
  { "xl_signal_spk", (void**)&xl_signal_spk, Dylib::Import::cMangled },
  { "xl_signal_stim", (void**)&xl_signal_stim, Dylib::Import::cMangled },
  { "xl_spk_data", (void**)&xl_spk_data, Dylib::Import::cMangled },
  { "xl_spk_thresh", (void**)&xl_spk_thresh, Dylib::Import::cMangled },
  { "xl_spk_thresh_set", (void**)&xl_spk_thresh_set, Dylib::Import::cMangled },
  { "xl_stim_biphase", (void**)&xl_stim_biphase, Dylib::Import::cMangled },
  { "xl_stim_data", (void**)&xl_stim_data, Dylib::Import::cMangled },
  { "xl_stim_enable", (void**)&xl_stim_enable, Dylib::Import::cMangled },
  { "xl_stim_enable_set", (void**)&xl_stim_enable_set, Dylib::Import::cMangled },
  { "xl_stim_get_exhaust", (void**)&xl_stim_get_exhaust, Dylib::Import::cMangled },
  { "xl_stim_get_res", (void**)&xl_stim_get_res, Dylib::Import::cMangled },
  { "xl_stim_header_word", (void**)&xl_stim_header_word, Dylib::Import::cMangled },
  { "xl_stim_seq", (void**)&xl_stim_seq, Dylib::Import::cMangled },
  { "xl_stim_set_exhaust", (void**)&xl_stim_set_exhaust, Dylib::Import::cMangled },
  { "xl_stim_set_res", (void**)&xl_stim_set_res, Dylib::Import::cMangled },
  { "xl_stim_word", (void**)&xl_stim_word, Dylib::Import::cMangled },
  { "xl_time", (void**)&xl_time, Dylib::Import::cMangled },
  { "xl_transceiver_command", (void**)&xl_transceiver_command, Dylib::Import::cMangled },
  { "xl_transceiver_connected", (void**)&xl_transceiver_connected, Dylib::Import::cMangled },
  { "xl_transceiver_enable", (void**)&xl_transceiver_enable, Dylib::Import::cMangled },
  { "xl_transceiver_status", (void**)&xl_transceiver_status, Dylib::Import::cMangled },
  { "xl_trial", (void**)&xl_trial, Dylib::Import::cMangled },
  { "xl_trial2", (void**)&xl_trial2, Dylib::Import::cMangled },
  { "xl_vdd_sensor", (void**)&xl_vdd_sensor, Dylib::Import::cMangled },
  { "xl_wall_sensor", (void**)&xl_wall_sensor, Dylib::Import::cMangled },
  { "xl_xipp_version", (void**)&xl_xipp_version, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBXIPPLIB

