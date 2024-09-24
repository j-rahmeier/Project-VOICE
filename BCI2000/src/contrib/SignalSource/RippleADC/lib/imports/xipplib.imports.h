// Import definitions for xipplib library
// generated Sat Sep  3 09:36:20 2022 by DylibTool
#ifndef XIPPLIB_IMPORTS_H
#define XIPPLIB_IMPORTS_H

#include "Win32Defs.h"

#ifndef STATIC_LIBXIPPLIB
// Disable function declarations in the original header
// file by #defining function names to point to symbols that
// are not referenced by any code.
#define xl_audio_tone_set __43232a05_xl_audio_tone_set
#define xl_button_get_count __62765b07_xl_button_get_count
#define xl_button_set_monitor __0b714168_xl_button_set_monitor
#define xl_close __14032c06_xl_close
#define xl_cont_emg __726f4c3c_xl_cont_emg
#define xl_cont_hifreq __1a1a4d4e_xl_cont_hifreq
#define xl_cont_hires __0c6b5959_xl_cont_hires
#define xl_cont_lfp __7b645b3c_xl_cont_lfp
#define xl_cont_raw __65635c3c_xl_cont_raw
#define xl_cont_status __11054a48_xl_cont_status
#define xl_digin __110b360a_xl_digin
#define xl_digout __650b3011_xl_digout
#define xl_error_check __36165872_xl_error_check
#define xl_error_copy_message __3a515365_xl_error_copy_message
#define xl_error_free_message __26544e70_xl_error_free_message
#define xl_error_pop __556e5f67_xl_error_pop
#define xl_error_register_callback __41472015_xl_error_register_callback
#define xl_error_requires_error __21353071_xl_error_requires_error
#define xl_error_string __3c032365_xl_error_string
#define xl_error_unregister_callback __45694a4e_xl_error_unregister_callback
#define xl_external_battery __696a5b3c_xl_external_battery
#define xl_external_battery_detected __6e7b4a62_xl_external_battery_detected
#define xl_fast_settle __061f5f4d_xl_fast_settle
#define xl_fast_settle_get_choices __6e77362a_xl_fast_settle_get_choices
#define xl_fast_settle_get_duration __7f76503a_xl_fast_settle_get_duration
#define xl_fe_version __0140406c_xl_fe_version
#define xl_filter_list_names __71467d77_xl_filter_list_names
#define xl_filter_list_selection __012e1214_xl_filter_list_selection
#define xl_filter_set __175f5866_xl_filter_set
#define xl_filter_set_custom __6474547e_xl_filter_set_custom
#define xl_get_fe __78180001_xl_get_fe
#define xl_get_fe_streams __79221218_xl_get_fe_streams
#define xl_hw_ground __6046567e_xl_hw_ground
#define xl_hw_reference __07353a68_xl_hw_reference
#define xl_imp __151c5f69_xl_imp
#define xl_internal_battery __7f6a5b30_xl_internal_battery
#define xl_led __1d085f6c_xl_led
#define xl_led_set __787c001f_xl_led_set
#define xl_led_set_monitor __78603606_xl_led_set_monitor
#define xl_lib_version __1b127373_xl_lib_version
#define xl_list_elec __74734e50_xl_list_elec
#define xl_nip_serial __186e697c_xl_nip_serial
#define xl_nipexec_version __7e631609_xl_nipexec_version
#define xl_open __0809316f_xl_open
#define xl_open_tcp __7c6a4130_xl_open_tcp
#define xl_open_udp __7d6d4130_xl_open_udp
#define xl_operator_add __1d023b51_xl_operator_add
#define xl_operator_lookup __6579303a_xl_operator_lookup
#define xl_processor_restart_software __1c674a74_xl_processor_restart_software
#define xl_processor_status __52765406_xl_processor_status
#define xl_signal __7d0b3112_xl_signal
#define xl_signal_lfp __0d545d74_xl_signal_lfp
#define xl_signal_raw __0a544373_xl_signal_raw
#define xl_signal_save __0b314273_xl_signal_save
#define xl_signal_save_set __6e451d00_xl_signal_save_set
#define xl_signal_set __09544277_xl_signal_set
#define xl_signal_set_lfp __790b2e11_xl_signal_set_lfp
#define xl_signal_set_raw __7e0b3016_xl_signal_set_raw
#define xl_signal_set_spk __620b3107_xl_signal_set_spk
#define xl_signal_set_stim __60663103_xl_signal_set_stim
#define xl_signal_spk __16544262_xl_signal_spk
#define xl_signal_stim __14394266_xl_signal_stim
#define xl_spk_data __69736117_xl_spk_data
#define xl_spk_thresh __08756574_xl_spk_thresh
#define xl_spk_thresh_set __7c2a1611_xl_spk_thresh_set
#define xl_stim_biphase __0f1f2744_xl_stim_biphase
#define xl_stim_data __6864464d_xl_stim_data
#define xl_stim_enable __050e534e_xl_stim_enable
#define xl_stim_enable_set __607a0c3d_xl_stim_enable_set
#define xl_stim_get_exhaust __7b6b5a12_xl_stim_get_exhaust
#define xl_stim_get_res __19053573_xl_stim_get_res
#define xl_stim_header_word __6e60683f_xl_stim_header_word
#define xl_stim_seq __7f60432c_xl_stim_seq
#define xl_stim_set_exhaust __6f6b5a12_xl_stim_set_exhaust
#define xl_stim_set_res __0d053573_xl_stim_set_res
#define xl_stim_word __7b6a4048_xl_stim_word
#define xl_time __11013a74_xl_time
#define xl_transceiver_command __0d136a73_xl_transceiver_command
#define xl_transceiver_connected __00000c13_xl_transceiver_connected
#define xl_transceiver_enable __077b6578_xl_transceiver_enable
#define xl_transceiver_status __0b7b7377_xl_transceiver_status
#define xl_trial __0a053e18_xl_trial
#define xl_trial2 __38053e18_xl_trial2
#define xl_vdd_sensor __0b66736a_xl_vdd_sensor
#define xl_wall_sensor __05175d5b_xl_wall_sensor
#define xl_xipp_version __0e163354_xl_xipp_version

#define __declspec(x)
#endif // STATIC_LIBXIPPLIB

#include "xipplib.h"

#ifndef STATIC_LIBXIPPLIB
#undef __declspec
// Use #undef to restore function names before declaring
// function pointers with the names originally used to
// declare imports.
#undef xl_audio_tone_set
extern decltype(&__43232a05_xl_audio_tone_set) xl_audio_tone_set;
#undef xl_button_get_count
extern decltype(&__62765b07_xl_button_get_count) xl_button_get_count;
#undef xl_button_set_monitor
extern decltype(&__0b714168_xl_button_set_monitor) xl_button_set_monitor;
#undef xl_close
extern decltype(&__14032c06_xl_close) xl_close;
#undef xl_cont_emg
extern decltype(&__726f4c3c_xl_cont_emg) xl_cont_emg;
#undef xl_cont_hifreq
extern decltype(&__1a1a4d4e_xl_cont_hifreq) xl_cont_hifreq;
#undef xl_cont_hires
extern decltype(&__0c6b5959_xl_cont_hires) xl_cont_hires;
#undef xl_cont_lfp
extern decltype(&__7b645b3c_xl_cont_lfp) xl_cont_lfp;
#undef xl_cont_raw
extern decltype(&__65635c3c_xl_cont_raw) xl_cont_raw;
#undef xl_cont_status
extern decltype(&__11054a48_xl_cont_status) xl_cont_status;
#undef xl_digin
extern decltype(&__110b360a_xl_digin) xl_digin;
#undef xl_digout
extern decltype(&__650b3011_xl_digout) xl_digout;
#undef xl_error_check
extern decltype(&__36165872_xl_error_check) xl_error_check;
#undef xl_error_copy_message
extern decltype(&__3a515365_xl_error_copy_message) xl_error_copy_message;
#undef xl_error_free_message
extern decltype(&__26544e70_xl_error_free_message) xl_error_free_message;
#undef xl_error_pop
extern decltype(&__556e5f67_xl_error_pop) xl_error_pop;
#undef xl_error_register_callback
extern decltype(&__41472015_xl_error_register_callback) xl_error_register_callback;
#undef xl_error_requires_error
extern decltype(&__21353071_xl_error_requires_error) xl_error_requires_error;
#undef xl_error_string
extern decltype(&__3c032365_xl_error_string) xl_error_string;
#undef xl_error_unregister_callback
extern decltype(&__45694a4e_xl_error_unregister_callback) xl_error_unregister_callback;
#undef xl_external_battery
extern decltype(&__696a5b3c_xl_external_battery) xl_external_battery;
#undef xl_external_battery_detected
extern decltype(&__6e7b4a62_xl_external_battery_detected) xl_external_battery_detected;
#undef xl_fast_settle
extern decltype(&__061f5f4d_xl_fast_settle) xl_fast_settle;
#undef xl_fast_settle_get_choices
extern decltype(&__6e77362a_xl_fast_settle_get_choices) xl_fast_settle_get_choices;
#undef xl_fast_settle_get_duration
extern decltype(&__7f76503a_xl_fast_settle_get_duration) xl_fast_settle_get_duration;
#undef xl_fe_version
extern decltype(&__0140406c_xl_fe_version) xl_fe_version;
#undef xl_filter_list_names
extern decltype(&__71467d77_xl_filter_list_names) xl_filter_list_names;
#undef xl_filter_list_selection
extern decltype(&__012e1214_xl_filter_list_selection) xl_filter_list_selection;
#undef xl_filter_set
extern decltype(&__175f5866_xl_filter_set) xl_filter_set;
#undef xl_filter_set_custom
extern decltype(&__6474547e_xl_filter_set_custom) xl_filter_set_custom;
#undef xl_get_fe
extern decltype(&__78180001_xl_get_fe) xl_get_fe;
#undef xl_get_fe_streams
extern decltype(&__79221218_xl_get_fe_streams) xl_get_fe_streams;
#undef xl_hw_ground
extern decltype(&__6046567e_xl_hw_ground) xl_hw_ground;
#undef xl_hw_reference
extern decltype(&__07353a68_xl_hw_reference) xl_hw_reference;
#undef xl_imp
extern decltype(&__151c5f69_xl_imp) xl_imp;
#undef xl_internal_battery
extern decltype(&__7f6a5b30_xl_internal_battery) xl_internal_battery;
#undef xl_led
extern decltype(&__1d085f6c_xl_led) xl_led;
#undef xl_led_set
extern decltype(&__787c001f_xl_led_set) xl_led_set;
#undef xl_led_set_monitor
extern decltype(&__78603606_xl_led_set_monitor) xl_led_set_monitor;
#undef xl_lib_version
extern decltype(&__1b127373_xl_lib_version) xl_lib_version;
#undef xl_list_elec
extern decltype(&__74734e50_xl_list_elec) xl_list_elec;
#undef xl_nip_serial
extern decltype(&__186e697c_xl_nip_serial) xl_nip_serial;
#undef xl_nipexec_version
extern decltype(&__7e631609_xl_nipexec_version) xl_nipexec_version;
#undef xl_open
extern decltype(&__0809316f_xl_open) xl_open;
#undef xl_open_tcp
extern decltype(&__7c6a4130_xl_open_tcp) xl_open_tcp;
#undef xl_open_udp
extern decltype(&__7d6d4130_xl_open_udp) xl_open_udp;
#undef xl_operator_add
extern decltype(&__1d023b51_xl_operator_add) xl_operator_add;
#undef xl_operator_lookup
extern decltype(&__6579303a_xl_operator_lookup) xl_operator_lookup;
#undef xl_processor_restart_software
extern decltype(&__1c674a74_xl_processor_restart_software) xl_processor_restart_software;
#undef xl_processor_status
extern decltype(&__52765406_xl_processor_status) xl_processor_status;
#undef xl_signal
extern decltype(&__7d0b3112_xl_signal) xl_signal;
#undef xl_signal_lfp
extern decltype(&__0d545d74_xl_signal_lfp) xl_signal_lfp;
#undef xl_signal_raw
extern decltype(&__0a544373_xl_signal_raw) xl_signal_raw;
#undef xl_signal_save
extern decltype(&__0b314273_xl_signal_save) xl_signal_save;
#undef xl_signal_save_set
extern decltype(&__6e451d00_xl_signal_save_set) xl_signal_save_set;
#undef xl_signal_set
extern decltype(&__09544277_xl_signal_set) xl_signal_set;
#undef xl_signal_set_lfp
extern decltype(&__790b2e11_xl_signal_set_lfp) xl_signal_set_lfp;
#undef xl_signal_set_raw
extern decltype(&__7e0b3016_xl_signal_set_raw) xl_signal_set_raw;
#undef xl_signal_set_spk
extern decltype(&__620b3107_xl_signal_set_spk) xl_signal_set_spk;
#undef xl_signal_set_stim
extern decltype(&__60663103_xl_signal_set_stim) xl_signal_set_stim;
#undef xl_signal_spk
extern decltype(&__16544262_xl_signal_spk) xl_signal_spk;
#undef xl_signal_stim
extern decltype(&__14394266_xl_signal_stim) xl_signal_stim;
#undef xl_spk_data
extern decltype(&__69736117_xl_spk_data) xl_spk_data;
#undef xl_spk_thresh
extern decltype(&__08756574_xl_spk_thresh) xl_spk_thresh;
#undef xl_spk_thresh_set
extern decltype(&__7c2a1611_xl_spk_thresh_set) xl_spk_thresh_set;
#undef xl_stim_biphase
extern decltype(&__0f1f2744_xl_stim_biphase) xl_stim_biphase;
#undef xl_stim_data
extern decltype(&__6864464d_xl_stim_data) xl_stim_data;
#undef xl_stim_enable
extern decltype(&__050e534e_xl_stim_enable) xl_stim_enable;
#undef xl_stim_enable_set
extern decltype(&__607a0c3d_xl_stim_enable_set) xl_stim_enable_set;
#undef xl_stim_get_exhaust
extern decltype(&__7b6b5a12_xl_stim_get_exhaust) xl_stim_get_exhaust;
#undef xl_stim_get_res
extern decltype(&__19053573_xl_stim_get_res) xl_stim_get_res;
#undef xl_stim_header_word
extern decltype(&__6e60683f_xl_stim_header_word) xl_stim_header_word;
#undef xl_stim_seq
extern decltype(&__7f60432c_xl_stim_seq) xl_stim_seq;
#undef xl_stim_set_exhaust
extern decltype(&__6f6b5a12_xl_stim_set_exhaust) xl_stim_set_exhaust;
#undef xl_stim_set_res
extern decltype(&__0d053573_xl_stim_set_res) xl_stim_set_res;
#undef xl_stim_word
extern decltype(&__7b6a4048_xl_stim_word) xl_stim_word;
#undef xl_time
extern decltype(&__11013a74_xl_time) xl_time;
#undef xl_transceiver_command
extern decltype(&__0d136a73_xl_transceiver_command) xl_transceiver_command;
#undef xl_transceiver_connected
extern decltype(&__00000c13_xl_transceiver_connected) xl_transceiver_connected;
#undef xl_transceiver_enable
extern decltype(&__077b6578_xl_transceiver_enable) xl_transceiver_enable;
#undef xl_transceiver_status
extern decltype(&__0b7b7377_xl_transceiver_status) xl_transceiver_status;
#undef xl_trial
extern decltype(&__0a053e18_xl_trial) xl_trial;
#undef xl_trial2
extern decltype(&__38053e18_xl_trial2) xl_trial2;
#undef xl_vdd_sensor
extern decltype(&__0b66736a_xl_vdd_sensor) xl_vdd_sensor;
#undef xl_wall_sensor
extern decltype(&__05175d5b_xl_wall_sensor) xl_wall_sensor;
#undef xl_xipp_version
extern decltype(&__0e163354_xl_xipp_version) xl_xipp_version;

#endif // STATIC_LIBXIPPLIB

namespace Dylib { bool xipplib_Loaded(); }
extern void* p_XL_DEFAULT_CONNECTION_OPTIONS = &XL_DEFAULT_CONNECTION_OPTIONS;
extern void* p_XL_DEFAULT_STATUS_CONNECTION_OPTIONS = &XL_DEFAULT_STATUS_CONNECTION_OPTIONS;

#endif // XIPPLIB_IMPORTS_H
