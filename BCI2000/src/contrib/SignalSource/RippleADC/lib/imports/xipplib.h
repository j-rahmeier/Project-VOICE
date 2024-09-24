#ifndef XIPPLIB_H
#define XIPPLIB_H

#if defined(xipplib_EXPORTS) && defined(_MSC_VER)
#define XIPPLIB_EXPORT __declspec(dllexport)
#else
#define XIPPLIB_EXPORT
#endif

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4200 )
#endif

#ifdef __cplusplus
#include <cstdint>
#include <cstdlib>
extern "C" {
#else
#include <stdint.h>
#include <stdlib.h>
#endif

#include "XippStimCmd.h"

/**
 * \brief maximum length for char string based labels
 */
#define STRLEN_LABEL 128

// TODO expand these codes
enum XippErrorCodes
{
  XL_ERR_TRIAL_STATUS_INVALID  = -9,
  XL_ERR_OPERATOR_NOT_FOUND    = -8,
  XL_ERR_NETWORK_INVALID       = -7,
  XL_ERR_PARAM_INVALID         = -6,
  XL_ERR_ELEC_INVALID          = -5,
  XL_ERR_NO_RESPONSE           = -4,
  XL_ERR_OPERATION_UNSUPORTED  = -3,
  XL_ERR_LIBRARY_UNINITIALIZED = -2,
  XL_ERR_UNSPECIFIED           = -1,
  XL_ERR_SUCCESS               = 0,
};




enum XippConnectionPolicy
{
  XL_CONNECT_POLICY_DEFAULT = 0,
  XL_CONNECT_POLICY_PREFER_WIRED = XL_CONNECT_POLICY_DEFAULT,
  XL_CONNECT_POLICY_PREFER_WIRELESS,
  XL_CONNECT_POLICY_WIRED_ONLY,
  XL_CONNECT_POLICY_WIRELESS_ONLY,
};

enum XippConnectionType
{
  XL_CONNECT_TYPE_DEFAULT = 0,
  XL_CONNECT_TYPE_TCP = XL_CONNECT_TYPE_DEFAULT,
  XL_CONNECT_TYPE_UDP,
};

/* \brief Options to connect to processor
 *
 * policy: WIRED_ONLY only searches wired network (192.168.42.*) for
 *             processor
 *         WIRELESS_ONLY only searches wireless network (192.168.43.*)
 *             for processor
 *         PREFER_WIRED first searches wired then wireless networks
 *         PREFER_WIRELESS first searches wireless then wired
 *         DEFAULT is PREFER_WIRED
 *
 * type:   TCP try to connect to processor using TCP/IP
 *         UDP try to connect to processor using UDP/IP
 *         DEFAULT use TCP
 *
 *         \note: if the connection is specified to UDP  the only valid
 *         policy is WIRED_ONLY.  Any other policy will result in an
 *         XL_ERR_OPERATION_UNSUPPORTED error.
 *
 * timeout: connection timeout in milliseconds.  Only used  if the
 *          connection type is TCP
 *
 *  XL_DEFAULT_CONNECTION_OPTIONS is:
 *     policy - PREFER_WIRED
 *     type  - TCP
 *     timeout - 10s (10000ms)
 *
 *   XL_DEFAULT_STATUS_CONNECTION_OPTIONS is:
 *     policy - PREFER_WIRED
 *     type - TCP
 *     timeout - 500ms
 */
struct XippConnectionOptions
{
  XippConnectionPolicy policy;
  XippConnectionType   type;
  int                  timeout;
};

XIPPLIB_EXPORT
extern const XippConnectionOptions * const XL_DEFAULT_CONNECTION_OPTIONS;

XIPPLIB_EXPORT
extern const XippConnectionOptions * const XL_DEFAULT_STATUS_CONNECTION_OPTIONS;

/**
 * \brief struct to send list of stimulation commands to the processor.
 *
 * Basic definition of a stimulation sequence.
 */
typedef struct StimSeq_t {
  //! electrode to stimulate on
  unsigned elec;
  //! length of array including command word
  unsigned length;
  //! list of command words
  uint32_t array[GVSTIM_CMD_MAXLEN];
} StimSeq_t;


//! Length of spike waveform
#define SEGMENT_WF_LEN 52

/**
 * \brief Data packet for a thresholded spike event or stimulation marker.
 *
 * \see xl_spk_data
 * \see xl_stim_data
 */
typedef struct XippSegmentDataPacketFixed_t {
  // header describing the source of the packet on the network
  uint8_t totalSizeQuads;
  uint8_t processorID;
  uint8_t moduleID;
  uint8_t outputStreamID;
  uint32_t timeDooDas;

  uint16_t   streamType;   // set to XIPP_STREAM_SEGMENT

  uint16_t flags;
  // running counter (used to detect packet drops)
  // uint16_t   count;

  //! Unit class corresponding to online sorting
  //! (0 for unsorted, 1-16 for unit 1-16, 255 for noise)
  uint16_t   class_id;
  //! Number of samples in data area of packet, always 52 in Ripple data.
  uint16_t   sample_count;
  //! Array containing thresholded spike data
  //! One ADC count in this field is 0.2 \f$ \mu V \f$.
  int16_t    i16[SEGMENT_WF_LEN];
} XippSegmentDataPacketFixed_t;

/**
 * \brief Data packet for a digital input event.
 *
 * Digital data will be triggered by a single channel, either through
 * a bit changing one of the four SMA channels or by a digital bit
 * changing on the parallel port.  If an event is triggered all digital
 * channels are read out.
 *
 * \see xl_digin
 */
typedef struct XippDigitalEventPacket_t {
  // Total size of data in packet
  uint8_t totalSizeQuads;
  // Id of Grapevine processor
  uint8_t processorID;
  // Module ID corresponding to event
  uint8_t moduleID;
  // Output stream corresponding to digital inputs
  uint8_t outputStreamID;
  // Processor time corresponding to this event
  uint32_t timeDooDas;
  // Type of data stream
  uint16_t streamType;
  // Unused
  uint16_t flags;
  //! Bit mask corresponding to channel that triggered digital event
  uint8_t reason;
  // Unused
  uint8_t reserved;
  //! 16-bit value corresponding to parelle port data
  uint16_t parallel;
  //! Value on SMA1 (high or low)
  uint16_t sma1;
  //! Value on SMA2 (high or low)
  uint16_t sma2;
  //! Value on SMA3 (high or low)
  uint16_t sma3;
  //! Value on SMA4 (high or low)
  uint16_t sma4;
  // Unused
  uint32_t PADDING;
} XippDigitalEventPacket_t;


typedef struct XippTrialDescriptor_t {
  char     *status;
  int       status_size;
  char     *file_name_base;
  int       file_name_base_size;
  char     *error_msg;
  int       error_msg_size;
  unsigned *auto_stop_time;
  unsigned *auto_incr;
  unsigned *incr_num;
} XippTrialDescriptor_t;

#define XIPP_TRIAL_DESCRIPTOR_INITIALIZER \
  {0, NULL, 0, NULL, 0, NULL, 0, NULL, NULL, NULL}


typedef struct XippTransceiverCommand_t {
  uint16_t command;
  uint16_t length;
  uint16_t data[32];
} XippTransceiverCommand_t;



/**
 * \brief convert xipp error code into error message
 *
 * \param buf    caller allocated buffer for the message
 * \param buf_sz size of buffer
 * \param code   error code
 *
 */
XIPPLIB_EXPORT
int xl_error_string(char *buf, size_t buf_sz, int code);

/**
 * \brief Send stim sequence to the processor.
 *
 * \param[in] seq Array of StimSeq_t defining stimulation waveforms.
 * \param[in] Number of elements in StimSeq_t.
 * \return Return code 0 on success.
 *
 * \note xippmex equivalent: xippmex('stimseq')
 * \see StimSeq_t
 */
XIPPLIB_EXPORT
int xl_stim_seq(const StimSeq_t *seq, int sz);

/**
 * \brief Return stim data as nev packets.
 *
 * \param segments Array of XippDigitalEventPacket_t to copy
 *   buffered data.  \p segments must be allocated by caller.
 * \param spike_ids Array of int's to copy buffered event indices.
 *   spike_ids must be allocated by caller.  If NULL, no data will be copied.
 * \param max_spikes Size of allocated data in \p segments and
 *   \p spike_ids.  \p max_spikes will be the maximum number of spikes to copy.
 * \param elec Zero indexed electrode.
 *
 * Copy buffered spike data to \p segments for electrode \p elec up to
 * \p max_spikes number of spikes.  Each time this function is called the
 * buffered spikes are cleared.
 *
 * \note The maximum number of buffered spikes is 1024 so that is the
 * upper limit to the amount of data that can be retrieved.
 */
XIPPLIB_EXPORT
int xl_stim_data(XippSegmentDataPacketFixed_t *xippSegments, int *eventIds,
                 unsigned maxSpike, unsigned elec);

/**
 * \brief Initialize  xipplib with a TCP connection.
 *
 * \return 0 on success, -1 on failure
 */
XIPPLIB_EXPORT
int xl_open_tcp();

/**
 * \brief Open connection to processor
 *
 * \return 0 on success, -1 on failure.
 */
XIPPLIB_EXPORT
int xl_open_udp();


/**
 * \brief Open connection to processor
 *
 * \param options[in] a valid XippConnectionOptions structure
 *                    if this is null, XL_DEFAULT_CONNECTION_OPTIONS
 *                    will be used
 *
 * \return 0 on success, -1 on failure.
 */

XIPPLIB_EXPORT
int xl_open(const XippConnectionOptions *);

/**
 * \brief Close connection to processor.
 *
 * \return 0 on success, -1 on failure.
 */
XIPPLIB_EXPORT
int xl_close();

/*
 * \brief   check if processor is online
 *
 * \param [out] status must be non-null is 1 if processor is online 0
 *              otherwise
 * \param [in]  a valid XippConnectionOptions structure,
 *              if this is null XL_DEFAULT_STATUS_CONNECTION_OPTIONS
 *              will be used
 *
 *              \note processor status can only use a TCP connection
 *              type
 * \return 0 on success -1 on failure
 *
 * \note this function may be called before xl_open
 *
 * */
XIPPLIB_EXPORT
int xl_processor_status(int *status, const XippConnectionOptions *opts);


/**
 * \brief Retrieve raw data (30 kHz, unfiltered).
 *
 * \param[out] timestamp Returned processor timestamp corresponding
 *  to the first data point.  Must be allocated by caller.
 * \param[out] data Returned data, must be allocated by caller to length
 *   length \p points * \p elecs_size.  Returned data is in units
 *   of \f$ \mu V \f$ for electrodes and \f$ V \f$ for Analog IO data.
 * \param[in] points Number of requested points.
 * \param[in] elecs Array of zero-indexed electodes.
 * \param[in] elecs_size Maximum size of electrodes.
 * \param[in] start_timespace If zero, return the most recent data.
 *   If non-zero return the data corresponding the requested processor
 *   time.
 * \return Number of returned data points.  -1 on failure.
 *
 * \note xippmex equivalent: xippmex('cont', 'raw')
 */
XIPPLIB_EXPORT
int xl_cont_raw(unsigned *timestamp, float *data, unsigned points,
                const unsigned *elecs, unsigned elecs_size,
                unsigned start_timestamp);

/**
 * \brief Retrieve hires data (2 kHz)
 *
 * \param[out] timestamp Returned processor timestamp corresponding
 *  to the first data point.  Must be allocated by caller.
 * \param[out] data Returned data, must be allocated by caller to length
 *   length \p points * \p elecs_size.  [\f$ \mu V \f$].
 * \param[in] points Number of requested points.
 * \param[in] elecs Array of zero-indexed electodes.
 * \param[in] elecs_size Maximum size of electrodes.
 * \param[in] start_timespace If zero, return the most recent data.
 *   If non-zero return the data corresponding the requested processor time.
 * \return Number of returned data points.  -1 on failure.
 *
 * \note xippmex equivalent: xippmex('cont', 'hires')
 */
XIPPLIB_EXPORT
int xl_cont_hires(unsigned *timestamp, float *data, unsigned points,
                  const unsigned *elecs, unsigned elecs_size,
                  unsigned start_timestamp);

/**
 * \brief Retrieve Mira status data (2 kHz)
 *
 * Every clock-cycle the Mira front end publishes status information concerning
 * the implant conntectivity and sensor information.
 *
 * | channel | name             | units |
 * | ------- | ---------------- | ----- |
 * | 0       | counter          |       |
 * | 1       | i_status_imp     |   V   |
 * | 2       | i_status_pwr     |   mA  |
 * | 3       | adc_vin_v        |   V   |
 * | 4       | adc_vin_a        |  mA   |
 * | 5       | adc_temp_onboard |  C    |
 * | 6       | adc_temp_offboard|  C    |
 * | 7       | pwr_servo_state  |       |
 * | 8       | impl_serial      |       |
 * | 9       | impl_deviceid    |       |
 * | 10      | impl_temp        |  C    |
 * | 11      | impl_humidity    |  %    |
 * | 12      | impl_voltage     |  V    |
 * | 13      | impl_ver_hw      |       |
 * | 14      | impl_ver_fw      |       |
 *
 * \param[out] timestamp Returned processor timestamp corresponding
 *  to the first data point.  Must be allocated by caller.
 * \param[out] data Returned data, must be allocated by caller to length
 *   length \p points * \p elecs_size.  Every every channel for this signal
 *   may have different dimensionality.
 * \param[in] points Number of requested points.
 * \param[in] elecs Array of zero-indexed electodes.
 * \param[in] elecs_size Maximum size of electrodes.
 * \param[in] start_timespace If zero, return the most recent data.
 *   If non-zero return the data corresponding the requested processor time.
 * \return Number of returned data points.  -1 on failure.
 *
 * \note xippmex equivalent: xippmex('cont', 'status')
 */
XIPPLIB_EXPORT
int xl_cont_status(unsigned *timestamp, float *data, unsigned points,
                   const unsigned *elecs, unsigned elecs_size,
                   unsigned start_timestamp);

/**
 * \brief Retrieve EMG data (2 kHz)
 *
 * This stream is specific to the Mira front-end.
 *
 * \param[out] timestamp Returned processor timestamp corresponding
 *  to the first data point.  Must be allocated by caller.
 * \param[out] data Returned data, must be allocated by caller to length
 *   length \p points * \p elecs_size.  [\f$ \mu V \f$].
 * \param[in] points Number of requested points.
 * \param[in] elecs Array of zero-indexed electodes.
 * \param[in] elecs_size Maximum size of electrodes.
 * \param[in] start_timespace If zero, return the most recent data.
 *   If non-zero return the data corresponding the requested processor time.
 * \return Number of returned data points.  -1 on failure.
 *
 * \note xippmex equivalent: xippmex('cont', 'emg')
 */
XIPPLIB_EXPORT
int xl_cont_emg(unsigned *timestamp, float *data, unsigned points,
                const unsigned *elecs, unsigned elecs_size,
                unsigned start_timestamp);
/**
 * \brief Retrieve hifreq data (7.5 kHz).
 *
 * \param[out] timestamp Returned processor timestamp corresponding
 *  to the first data point.  Must be allocated by caller.
 * \param[out] data Returned data, must be allocated by caller to length
 *   length \p points * \p elecs_size.  [\f$ \mu V \f$]
 * \param[in] points Number of requested points.
 * \param[in] elecs Array of zero-indexed electodes.
 * \param[in] elecs_size Maximum size of electrodes.
 * \param[in] start_timespace If zero, return the most recent data.
 *   If non-zero return the data corresponding the requested processor
 *   time.
 * \return Number of returned data points.  -1 on failure.
 *
 * \note xippmex equivalent: xippmex('cont', 'emg')
 */
XIPPLIB_EXPORT
int xl_cont_hifreq(unsigned *timestamp, float *data, unsigned points,
                   const unsigned *elecs, unsigned elecs_size,
                   unsigned start_timestamp);

/**
 * \brief Retrieve LFP or analog 1 kHz data.
 *
 * \param[out] timestamp Returned processor timestamp corresponding
 *  to the first data point.  Must be allocated by caller.
 * \param[out] data Returned data, must be allocated by caller to length
 *   length \p points * \p elecs_size.  Returned data is in units
 *   of \f$ \mu V \f$ for electrodes and \f$ V \f$ for Analog IO data.
 * \param[in] points Number of requested points.
 * \param[in] elecs Array of zero-indexed electodes.
 * \param[in] elecs_size Maximum size of electrodes.
 * \param[in] start_timespace If zero, return the most recent data.
 *   If non-zero return the data corresponding the requested processor
 *   time.
 * \return Number of returned data points.  -1 on failure.
 *
 * \note xippmex equivalent: xippmex('cont', 'lfp')
 */
XIPPLIB_EXPORT
int xl_cont_lfp(unsigned *timestamp, float *data, unsigned points,
                const unsigned *elecs, unsigned elecs_size,
                unsigned start_timestamp);

/**
 * \brief Return thresholded spike events.
 *
 * \param segments Array of XippDigitalEventPacket_t to copy
 *   buffered data.  \p segments must be allocated by caller.
 * \param spike_ids Array of int's to copy buffered event indices.
 *   spike_ids must be allocated by caller.  If NULL, no data will be copied.
 * \param max_spikes Size of allocated data in \p segments and
 *   \p spike_ids.  \p max_spikes will be the maximum number of spikes to copy.
 * \param elec Zero indexed electrode.
 *
 * Copy buffered spike data to \p segments for electrode \p elec up to
 * \p max_spikes number of spikes.  Each time this function is called the
 * buffered spikes are cleared.
 *
 * \note The maximum number of buffered spikes is 1024 so that is the
 * upper limit to the amount of data that can be retrieved.
 *
 * \note xippmex equivalent: xippmex('spike')
 *
 * \see XippSegmentDataPacketFixed_t
 */
XIPPLIB_EXPORT
int xl_spk_data(XippSegmentDataPacketFixed_t *segments, int *spike_ids,
                unsigned max_spikes, unsigned elec);

/**
 * \brief Set spike thresholds.
 *
 * \param[out] lower Lower threshold to set.
 * \param[out] upper Upper threshold to set.
 * \param[in] elec Zero-indexed electrode.
 * \returned 0 on success, -1 on failure.
 *
 * \note: xippmex equivalent: xippmex('spike', 'thresh')
 */
XIPPLIB_EXPORT
int xl_spk_thresh_set(unsigned elec, float lower, float upper);

/**
 * \brief Return current spike thresholds.
 *
 * \param[out] lower Return lower threshold.  Allocated by caller.
 * \param[out] upper Return upper threshold.  Allocated by caller.
 * \param[in] elec Zero indexed electrode.
 * \returned 0 on success, -1 on failure.
 *
 * \note: xippmex equivalent: xippmex('spike', 'thresh')
 */
XIPPLIB_EXPORT
int xl_spk_thresh(float *lower, float *upper, unsigned elec);

/**
 * \brief Return digital inputs.
 *
 * \param[out] events Array of XippDigitalEventPacket_t to copy
 *   buffered data.  events must be allocated by caller.
 * \param[out] event_ids Array of int's to copy buffered event indices.
 *   event_ids must be allocated by caller.  If NULL, no data will be copied.
 * \param[in] max_events Size of allocated data in events and
 *   event_ids.  max_events will be the maximum number of events to copy.
 *
 * Copy buffered digital events to events up to max_event number of
 * events.  Each time this function is called the buffered events are cleared.
 *
 * \note The maximum number of buffered events is 1024 so that is the
 * upper limit to the amount of data that can be retrieved.
 *
 * \note xippmex equivalent xippmex('digin')
 *
 * \see XippDigitalEventPacket_t
 */
XIPPLIB_EXPORT
int xl_digin(XippDigitalEventPacket_t *events, int *event_ids,
             unsigned max_events);

/**
 * \brief Command processor to send digital outputs
 *
 * \param[in] Outputs array of outputs to command.  SMA channels 1
 *   through 4 correspond to outputs 0 to 3.  The paralle output is
 *   output 4.
 * \param[in] Values array of values to set set for each output.
 *   In the case of the SMA channels (0-3), the values are boolean,
 *   i.e. 0 sets the output low, otherwise the value will be set high.
 *   For the paralle, the output will be set to the lower 16 bits of
 *   the provided value.
 * \param[in] len length of outputs and values.
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('digout')
 */
XIPPLIB_EXPORT
int xl_digout(const unsigned *outputs, const unsigned *values, int len);

/**
 * \brief Return an array with electrodes of particular types.
 *
 * \param[out] elecs Returned electrode indices.
 * \param[out] max_elecs size allocated to \p elecs.
 * \param festr string to correspoding to electrode type.  May be "all",
 *   "stim", "micro", etc.
 * \return Number of electrodes returned, or -1 on error.
 *
 * \note xippmex equivalent: xippmex('elec')
 */
XIPPLIB_EXPORT
int xl_list_elec(unsigned *elecs, unsigned max_elecs, const char *festr);

/**
 * \brief Return most recent processor time.
 *
 * \note xippmex equivalent: xippmex('time')
 */
XIPPLIB_EXPORT
unsigned int xl_time();

/**
 * \internal
 *
 * \brief Retrieve front end index from electrode
 * \param[in] elec Zero indexed electrode.
 * \return -1 on error
 */
XIPPLIB_EXPORT
int xl_get_fe(int elec);

/******************************************************************************
 * The following functions handle signal selection
 *****************************************************************************/
/**
 * \brief Return list of stream types
 *
 * \param[out] streams array of c strings for returned strings.  This
 *   data is allocated by the caller.
 * \param[in] Number of c strings (with length STRLEN_LABEL) allocated in
 *   \p streams.
 * \param[in] elec Zero indexed electrode.
 * \return the number of stream return in \p streams, -1 on error.
 *
 * \note xippmex equivalent: xippmex('signal')
 */
XIPPLIB_EXPORT
int xl_get_fe_streams(char (*streams)[STRLEN_LABEL], int max_streams, int elec);

/**
 * \brief Return status of signal selection based on a string input.
 *
 * \param[in] elec Zero indexed electrode.
 * \param[in] streamstr c string corresponding to string.  Examples are
 *   'lfp', 'raw', 'spk'.
 * \return 1 is stream is selected, otherwise 0.  -1 is returned on failure.
 *
 * xl_get_fe_streams may be used to find strings corresponding the desired
 * data streams for \p streamstr.
 *
 * \note xippmex equivalent xippmex('signal')
 *
 * \see xl_signal_raw, xl_signal_lfp, xl_signal_stim, xl_signal_spk
 * \see xl_get_fe_streams
 */
XIPPLIB_EXPORT
int xl_signal(int elec, const char *streamstr);

/**
 * \brief Set signal selection for desired electrode and stream string.
 *
 * \param[in] elec Zero indexed electrode.
 * \param[in] streamstr c string corresponding to string.  Examples are
 *   'lfp', 'raw', 'spk'.
 * \param[in] val value to set, 1 for on, 0 for off
 * \return 0 on success
 *
 * xl_get_fe_streams may be used to find strings corresponding the desired
 * data streams for \p streamstr.
 *
 * \see xl_signal_set_raw, xl_signal_set_lfp, xl_signal_set_stim,
 * \see xl_signal_set_spk, xl_get_fe_streams
 *
 * \note xippmex equivalent xippmex('signal')
 */
XIPPLIB_EXPORT
int xl_signal_set(int elec, const char *streamstr, int val);

/*****************************************************************************
 * The following functions handle setting signals to be saved on the processor.
 * Note that this is different from the signal selection functions above.  The
 * processor can save to its internal storage a completely different set of
 * signals from that which is sent over the Grapevine network.
 *****************************************************************************/

/**
 * \brief Retrieve the local save state for the stream
 *
 * \param elec Zero-indexed electrode
 * \param streamstr name of the stream to query
 *
 * \return 1 if the stream is set to be saved, 0 otherwise
 */
XIPPLIB_EXPORT
int xl_signal_save(int elec, const char *streamstr);

/**
 * \brief Set the local save state
 * \param elec Zero-indexed electrode
 * \param streamstr name of the stream to save for this electrode
 * \param val 1 or 0, depending on whether to save the signal or not
 */
XIPPLIB_EXPORT
int xl_signal_save_set(int elec, const char *streamstr, int val);

/******************************************************************************
 * The following functions are simply wrappers for xl_signal and xl_signal_set
 * for ease of use and so that one doesn't have to remember all the stream
 * names.
 *****************************************************************************/
/**
 * \brief Set signal selection for raw streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \param[in] val value to set, 1 for on, 0 for off.
 * \return 0 on success, -1 on failure.
 *
 * This function is simply a wrapper around xl_signal_set for raw data streams.
 *
 * \see xl_signal_set
 *
 * \note xippmex equivalent xippmex('signal', 'raw')
 */
XIPPLIB_EXPORT
int xl_signal_set_raw(int elec, int val);

/**
 * \brief Set signal selection for spike streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \param[in] val Value to set, 1 for on, 0 for off.
 * \return 0 on success, -1 on failure.
 *
 * This function is simply a wrapper around xl_signal_set for spike data
 * streams.
 *
 * \see xl_signal_set
 *
 * \note xippmex equivalent xippmex('signal', 'spike')
 */
XIPPLIB_EXPORT
int xl_signal_set_spk(int elec, int val);

/**
 * \brief Set signal selection for lfp streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \param[in] val Value to set, 1 for on, 0 for off.
 * \return 0 on success, -1 on failure.
 *
 * This function is simply a wrapper around xl_signal_set for lfp data streams.
 *
 * \see xl_signal_set
 *
 * \note xippmex equivalent xippmex('signal', 'lfp')
 */
XIPPLIB_EXPORT
int xl_signal_set_lfp(int elec, int val);

/**
 * \brief Set signal selection for stim streams.
 *
 * This function is simply a wrapper around xl_signal_set for stim
 * marker data streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \param[in] val Value to set, 1 for on, 0 for off.
 * \return 0 on success, -1 on failure.
 *
 * \see xl_signal_set
 *
 * \note xippmex equivalent xippmex('signal', 'stim')
 */
XIPPLIB_EXPORT
int xl_signal_set_stim(int elec, int val);

/**
 * \brief Retrieve signal selection status for raw streams.
 *
 * This function is simply a wrapper around xl_signal for raw streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \return 1 if stream is selected, 0 otherwise, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('signal', 'raw')
 */
XIPPLIB_EXPORT
int xl_signal_raw(int elec);

/**
 * \brief Retrieve signal selection status for lfp streams.
 *
 * This function is simply a wrapper around xl_signal for lfp streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \return 1 is stream is selected, 0 otherwise, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('signal', 'lfp')
 */
XIPPLIB_EXPORT
int xl_signal_lfp(int elec);

/**
 * \brief Retrieve signal selection status for spike data streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \return 1 if stream is selected, 0 otherwise, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('signal', 'spk')
 */
XIPPLIB_EXPORT
int xl_signal_spk(int elec);

/**
 * \brief Retrieve signal selection status for stim marker streams.
 *
 * \param[in] elec Zero indexed electrode.
 * \return 1 is stream is selected, 0 otherwise, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('signal', 'stim')
 */
XIPPLIB_EXPORT
int xl_signal_stim(int elec);

/**
 * \brief Set global stim enable.
 *
 * \param val 1 to enable stimulation, 0 to disable.
 * \return 0 on success, -1 on failure
 *
 * \note xippmex equivalent: xippmex('stim', 'enable')
 */
XIPPLIB_EXPORT
int xl_stim_enable_set(int val);

/**
 * \brief Retrieve value of global stimulation enable state.
 *
 * \return 1 if enabled, 0 if disabled, -1 on error.
 *
 * \note xippmex equivalent: xippmex('stim', 'enable')
 */
XIPPLIB_EXPORT
int xl_stim_enable();


/******************************************************************************
 * The following functions handle the listing and setting of the filter
 * parameters for the online NIP filtering.
 *
 * Although these functions are organized by electrode, setting a filter for
 * a given electrode actually changes the electrode for an entire module (32
 * or 16) electrode block. So setting a filter for electrode 0 will modify the
 * filter for electrodes 0-31.
 *
 * Note that each electrode has multiple filters and streams.  The possible
 * streams that allow filtering are "spike", "lfp", and "hires".  The "lfp"
 * and "hires" streams also allow for a notch type filter.  The function
 * xl_filter_list_names will list the types of filters for a given electrode.
 *
 * Of each filter type, there are multple built in filters that may be
 * selected.  The the number of filters and details of them may be found with
 * the functions xl_filter_list_selection() and xl_filter_get_desc().
 *****************************************************************************/

/**
 * \brief Sos parameters for each filter stage
 */
typedef struct SosStage_t {
  float b0;
  float b1;
  float a1;
  float a2;
} SosStage_t;

/**
 * \brief SOS filter definition for xipplib.
 *
 * The xipplib function xl_filter_get_desc uses this struct to return the
 * parameters of filters the build in filters on the Grapevine processor.
 *
 * The xipplib function xl_filter_set_custom accepts this struct to input
 * a custom filter, generally designed by the user.
 *
 * \see xl_filter_get_desc, xl_filter_set_custom
 */
typedef struct SosFilterDesc_t {

  char label[STRLEN_LABEL]; //!< Label defining filter

  float   center;       //!< Center frequency, Hz.  Often zero
  float   lowCutoff;    //!< Low cutoff frequency, Hz
  float   highCutoff;   //!< High cutoff frequency, Hz
  uint8_t centerOrder;
  uint8_t centerFlags;
  uint8_t lowOrder;     //!< order of low cutoff filter component
  uint8_t lowFlags;
  uint8_t highOrder;    //!< order of high cutoff filter component
  uint8_t highFlags;

  uint8_t    maxStages; //!< maximum allowed stages by the processor
  uint8_t    numStages; //!< number of stages.  Corresponds to the length of stages member
  SosStage_t stages[];  //!< actual SOS values
} SosFilterDesc_t ;

/**
 * \brief List possible filter types (as strings) for the given electrode
 *
 * \param[out] filter_strs Returned names of filters.  Pointer to array of c-strings.
 *   Allocated by caller.
 * \param[in] max_stream Number of rows allocated by \p filter_strs.
 * \param[in] elec Zero indexed electrode.
 * \return Number of filter types, -1 on error.
 *
 * \note xippmex equivalent: xippmex('filter', 'list')
 */
XIPPLIB_EXPORT
int xl_filter_list_names(
    char filter_strs[][STRLEN_LABEL],
    int max_stream,
    int elec);

/**
 * \brief xl_filter_list_selection
 * \param[out] selection Returned index of currently selected filter.  Allocated by
 *  caller.
 * \param[in] elec Zero indexed electrode.
 * \param[in] filter_type c-string corresponding to filter type.
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('filter', 'list')
 */
XIPPLIB_EXPORT
int xl_filter_list_selection(int *selection, int elec, const char *filter_type);

/**
 * \brief Set filter by index
 *
 * This function sets a filter to one of the currently selected filters
 * on the processor.  All filters are set by front end.  So, if \p elec is
 * equal to 0, this will modify the filter state for channels 0 through 31.
 *
 * \param elec zero indexed electrode
 * \param filter_type string corresponding to filter type
 * \param selection new filter selection
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('filter', 'set')
 */
XIPPLIB_EXPORT
int xl_filter_set(int elec, const char *filter_type, int filter_index);

/**
 * \brief Retrieve filter descriptor.
 *
 * xl_filter_get_desc() allocates memory with malloc, it is important for
 * the user of this function to use free() to avoid memory leaks.
 *
 * \param elec Zero indexed electrode
 * \param filter_type c-string corresponding to the given filter type.
 * \param filter_index Index to specific filter.
 * \return Pointer to allocated memory.
 *
 * \note xippmex equivalent: xippmex('filter', 'list')
 * \see SosFilterDesc_t
 */
SosFilterDesc_t *xl_filter_get_desc(int elec, const char *filter_type,
                                    int filter_index);

/**
 * \brief Set a custom filter for given Front End.
 *
 * Custom filter's are SOS as defined by SosFilterDesc_t.
 *
 * \param elec Zero indexed electrode.
 * \param filter_type c-string describing the filter type.
 * \param filter_desc SosFilterDesc_t describing updated filter.
 * \return zero on success, -1 on failure.
 *
 * \see SosFilterDesc_t
 * \note xippmex equivalent: xippmex('filter', 'set', 'custom')
 */
XIPPLIB_EXPORT
int xl_filter_set_custom(int elec, const char *filter_type,
                         const SosFilterDesc_t *filter_desc);

/**
 * \brief Retrieve the current stimulation resolution for supported front-ends.
 *
 * This functionality only exists for nano+stim and micro2+stim.
 *
 * \param[in] electrode Zero indexed electrode.
 * \param[out] level_out Returned resolution.  Allocated by caller.
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('stim', 'res')
 */
XIPPLIB_EXPORT
int xl_stim_get_res(int electrode, int* level_out);

/**
 * \brief Retrieve the current stimulation resolution for supported front-ends.
 *
 * This functionality only exists for nano+stim and micro2+stim.
 *
 * \param[in] electrode Zero indexed electrode.
 * \param[in] res Resolution to set, must be 0 through 4.
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('stim', 'res')
 */
XIPPLIB_EXPORT
int xl_stim_set_res(int electrode, int level);

/**
 * \brief Retrieve the current stim exhaust resistor for supported front-ends.
 *
 * This functionality only exists for macro+stim and eeg+tes.
 *
 * \param[in] electrode Zero indexed electrode.
 * \param[out] res_opt_out Returned resolution.  Allocated by caller.
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('stim', 'exhaust')
 */
XIPPLIB_EXPORT
int xl_stim_get_exhaust(int electrode, int* res_opt_out);

/**
 * \brief Set the stim exhausting resistor for supported front-ends.
 *
 * This functionality only exists for macro+stim and eeg+tes.
 *
 * \param[in] electrode Zero indexed electrode.
 * \param[in] res_opt Exhaust resistor option to set, must be 0 through 3.
 * \return 0 on success, -1 on failure.
 *
 * \note xippmex equivalent: xippmex('stim', 'exhaust', res_opt)
 */
XIPPLIB_EXPORT
int xl_stim_set_exhaust(int electrode, int res_opt);

/**
 * \brief Create array of command words to create symetric biphasic pattern.
 *
 * This array should be packed in a XippWordArray and send to the processor
 * with the appropriate Xipp property index.
 *
 * \param[out] words Returned array to pack stimulation words.  Allocated
 *   by caller.
 * \param[out] max_words Size allocated in \p words.
 * \param[in] freq Frequency of repeated pattern. [Hz]
 * \param[in] tl Train length how long to repeat pattern. [ms]
 * \param[in] ampl Amplitude of stimulation phases in 0-127 step sizes.
 * \param[in] dur Duration of anodic or cathodic phases. [ms]
 * \param[in] delay Delay start of wavefrom for interleaving. [ms]
 * \param[in] action Command word - see XippStimCmd.h, one of GVSTIM_ACT_*.
 * \return -1 with failure.  Otherwise, number of words returned in \p words.
 */
XIPPLIB_EXPORT
int xl_stim_biphase(uint32_t words[], int max_words, double freq,
                    double tl, int ampl, double dur, double delay, int action);

/**
 * \brief construct the leading control word that should be add the
 *    head of a stim sequence array.
 */
XIPPLIB_EXPORT
unsigned int xl_stim_header_word(int action, int period, int repeats);

/**
 * \brief build uint32_t control word to define waveform shape
 *   from basic parameters.
 */
XIPPLIB_EXPORT
unsigned int xl_stim_word(int repeats, int ampl, int pol, int enable,
                          int amp_select, int delay, int fs);



/*
 * \brief connect to a Xipp Operator over TCP
 *
 * \param operator_id - if this is given the encoded operator id is
 *        returned. if this is not given xl_operator_lookup() must be
 *        called to get the operator_id for xl_trial
 *
 * \param operator_address ip - address in dotted decimal form of the xipp
 *        operator.  can be short hand "139", "140", etc in which case
 *        the wired and wireless networks will be tried in the order of
 *        the policy specified.  shorthand of "42.139", "43.140", etc
 *        will be completed to the appropriate wired or wireless network
 *        and the connection will be tried directly ignoring the policy
 *        parameter.
 *
 * \param policy - connection policy to attempt to locate the Xipp
 *        operator. If no policy is provided, the default
 *        policy will be used.
 *
 * \param connection_timeout - timeout in milliseconds to wait for the
 *        connection before giving up.  if no connection_timeout is
 *        given the default will be used.
 */
XIPPLIB_EXPORT
int xl_operator_add(
    uint32_t *operator_id,
    const char *operator_address,
    const XippConnectionPolicy *policy,
    const int *connection_timeout);


/*
 * \brief lookup ID of previously connected Xipp Operator
 *
 * \param operator_id -  required space to hold the id of the xipp
 *         operator
 *
 * \param operator_address ip - address in dotted decimal form of the xipp
 *        operator.  can be short hand "139", "140", etc in which case
 *        the wired and wireless networks will be tried in the order of
 *        the policy specified.  shorthand of "42.139", "43.140", etc
 *        will be completed to the appropriate wired or wireless network
 *        and the connection will be tried directly ignoring the policy
 *        parameter.
 */

XIPPLIB_EXPORT
int xl_operator_lookup(
    uint32_t *operator_id,
    const char *operator_address);


/**
 * \brief Exposes the ability to control Trellis file recording.
 *
 * All parameters except for oper are optional if an `in` parameter is
 * passed a NULL it will not be altered. If an `out` parameter is passed
 * a NULL then it isn't returned. The string length parameters are used for
 * both in and out parameters. They can be set to zero if both in and out
 * string parameters are missing.
 *
 * \param oper_id
 * \param status_in
 * \param file_name_base_in
 * \param auto_stop_time_in
 * \param auto_incr_in
 * \param incr_num_in
 * \param status_out
 * \param file_name_base_out
 * \param error_msg_out
 * \param auto_stop_time_out
 * \param auto_incr_out
 * \param incr_num_out
 * \returns 0 on success, an error code otherwise
 */
XIPPLIB_EXPORT
int xl_trial(uint32_t        oper_id,
             const char     *status_in,
             const char     *file_name_base_in,
             const unsigned *auto_stop_time_in,
             const unsigned *auto_incr_in,
             const unsigned *incr_num_in,
             char           *status_out,
             int             status_max_len,
             char           *file_name_base_out,
             int             file_name_base_max_len,
             char           *error_msg_out,
             int             error_msg_max_len,
             unsigned       *auto_stop_time_out,
             unsigned       *auto_incr_out,
             unsigned       *incr_num_out);


/**
 * \brief Exposes the ability to control Trellis file recording.
 *
 * All parameters except for oper are optional. If an `in` parameter is
 * passed a NULL it will not be altered. If an `out` parameter is passed
 * a NULL then it isn't returned. The string length parameters are used for
 * both in and out parameters. They can be set to zero if both in and out
 * string parameters are missing.
 *
 * \returns 0 on success, an error code otherwise
 */

XIPPLIB_EXPORT
int xl_trial2(
    uint32_t oper_id,
    XippTrialDescriptor_t *descriptor_out,
    const XippTrialDescriptor_t *descriptor_in);

/**
 * \brief Retrieve xipplib library version.
 *
 * \param[out] version C string to put library version
 * \param[in] max length of version
 */
XIPPLIB_EXPORT
int xl_lib_version(char *version, int max);

/**
 * \brief Retrieve the XIPP protocol version.
 *
 * \param[out] xipp C string to put protocol version
 * \param[in] max length of xipp
 */
XIPPLIB_EXPORT
int xl_xipp_version(char *xipp, int max);

/**
 * \brief Retrieve nipexec version running on the processor
 *
 * \param[out] nipexec c string to put nipexec version
 * \param[in] max length of nipexec
 */
XIPPLIB_EXPORT
int xl_nipexec_version(char *nipexec,int max);

/**
 * \brief Retrieve R number of the processor
 * \param[out] serial c string to put R number
 * \param[in] max length of serial
 * \returns 0 on success, an error code otherwise
 */

XIPPLIB_EXPORT
int xl_nip_serial(char *serial, int max);

/**
 * \brief Retrieve R number of front end for a given electrode
 * \param[out] version c string to put R number
 * \param[in] max length of serial
 * \param[in] elec index of any electrode on front end (0-511)
 */
XIPPLIB_EXPORT
int xl_fe_version(char *version, int max, int elec);

/**
 * \brief Return impedance as Ohms to \p imp
 *
 * \param[out] imp - caller allocated array to copy impedance values [Ohms].
 * \param[in] chans - channels to measure impedance (0-511).
 * \param[in] sz - length of array of channels.
 * \returns 0 on success, 1 if \p imp or \p chans are NULL.
 *
 * \note xippmex equivalent: xippmex('impedance')
 */
XIPPLIB_EXPORT
int xl_imp(float *imp, const unsigned *chan, int sz);


/**
 * \brief Checkif transceiver is connect to the implant
 *
 * \param[in] elec - electrode index of the Mira device (0-511)
 * \param[out] connected - 1 if implant is connected, otherwise 0
 *
 * \note xippmex equivalent: xippmex('transceiver', 'connected', elec)
 */

XIPPLIB_EXPORT
int xl_transceiver_connected(int elec, int *connected);

/**
 * \brief Enable or disable transceiver
 *
 * \param[in] elec - electrode index of the Mira device (0-511)
 * \param[in] enable - 1 to enable or 0 to disable the transciever
 *
 * \note xippmex equivalent: xippmex('transceiver', 'enable')
 */

XIPPLIB_EXPORT
int xl_transceiver_enable(int elec, int enable);

/**
 * \brief Read the most recent status array from the transceiver
 *
 * \param[out] data - space allocated by the caller
 *                    sufficient to hold an array of 32 uint16_t
 * \param[in] elec - electrode index of the Mira device (0-511)
 *
 *
 * \note xippmex equivalent: xippmex('transceiver', 'status')
 *
 */

XIPPLIB_EXPORT
int xl_transceiver_status(uint16_t data[], int elec);

/**
 * \brief Write the command to the transceiver
 *
 * \param[in] elec - electrode index of the Mira device (0-511)
 * \param[in] command - space allocated by the caller
 *                      sufficient to hold an array of 32 uint16_t
 *
 *
 * \note xippmex equivalent: xippmex('transceiver', 'command')
 *
 */

XIPPLIB_EXPORT
int xl_transceiver_command(int elec,
                           const XippTransceiverCommand_t *command);


XIPPLIB_EXPORT
int xl_fast_settle_get_choices(int fe_index, const char *cmd,
                               char labels[][STRLEN_LABEL], int labels_size,
                               int *current_item);

XIPPLIB_EXPORT
int xl_fast_settle_get_duration(int fe_index,  const char *cmd, float *value);

XIPPLIB_EXPORT
int xl_fast_settle(int fe_index, const char *cmd, int *item, float *duration);

XIPPLIB_EXPORT
int xl_hw_reference(unsigned elec, bool enable);

XIPPLIB_EXPORT
int xl_hw_ground(unsigned elec, bool enable);

/* sensors */
XIPPLIB_EXPORT
int xl_internal_battery(int *result);

XIPPLIB_EXPORT
int xl_external_battery(int *result);

XIPPLIB_EXPORT
int xl_external_battery_detected(int *result);

typedef struct  XippSensor_t
{
  float current;
  float voltage;
  float power;
} XippSensor_t;

XIPPLIB_EXPORT
int xl_wall_sensor(XippSensor_t *);

XIPPLIB_EXPORT
int xl_vdd_sensor(XippSensor_t *);

/* audio */
XIPPLIB_EXPORT
int xl_audio_tone_set(unsigned *freq, unsigned *dur);

/* buttons and lights */
enum XL_BUTTON_ID
{
  XL_BUTTON_STOP_STIM = 1,
  XL_BUTTON_EVENT,
  XL_BUTTON_F1,
  XL_BUTTON_F2,
};

enum XL_LED_ID
{
  XL_LED_EVENT = XL_BUTTON_EVENT,
  XL_LED_F1 = XL_BUTTON_F1,
  XL_LED_F2 = XL_BUTTON_F2,
  XL_LED_PORTA,
  XL_LED_PORTB,
  XL_LED_PORTC,
  XL_LED_PORTD
};

XIPPLIB_EXPORT
int xl_button_get_count(int *, XL_BUTTON_ID id);

XIPPLIB_EXPORT
int xl_button_set_monitor(int, XL_BUTTON_ID id);

XIPPLIB_EXPORT
int xl_led_set(int, XL_LED_ID);

XIPPLIB_EXPORT
int xl_led_set_monitor(int, XL_LED_ID);

XIPPLIB_EXPORT
int xl_led(int *, XL_LED_ID);

/* restart processor software */
XIPPLIB_EXPORT
int xl_processor_restart_software(void);


/* Error handling */

typedef struct XippErrorMessage_t
{
  uint32_t error_code;
  uint32_t error_time;

  uint8_t reserved;
  uint8_t module_id;
  uint8_t port_id;
  uint8_t slot_id;

  char *message;
} XippErrorMessage_t;

XIPPLIB_EXPORT
void xl_error_free_message(XippErrorMessage_t *);

XIPPLIB_EXPORT
XippErrorMessage_t *xl_error_copy_message(const XippErrorMessage_t *);
XIPPLIB_EXPORT
int xl_error_requires_error(const XippErrorMessage_t *);

XIPPLIB_EXPORT
int xl_error_check();
XIPPLIB_EXPORT
int xl_error_pop(XippErrorMessage_t **);

typedef void (*XippErrorCallback_t)(void *context, const XippErrorMessage_t *);
XIPPLIB_EXPORT
int xl_error_register_callback(XippErrorCallback_t, void *context);
XIPPLIB_EXPORT
int xl_error_unregister_callback(XippErrorCallback_t, void *context);

#  ifdef __cplusplus
}
#  endif

#ifdef _MSC_VER
#pragma warning( pop )
#endif


#endif // XIPPLIB_H
