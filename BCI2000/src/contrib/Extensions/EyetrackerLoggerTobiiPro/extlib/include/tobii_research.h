/**
@copyright COPYRIGHT 2019 - PROPERTY OF TOBII PRO AB
@copyright 2019 TOBII PRO AB - KARLSROVAGEN 2D, DANDERYD 182 53, SWEDEN - All Rights Reserved.

@copyright NOTICE:  All information contained herein is, and remains, the property of Tobii Pro AB and its suppliers, if any.  The intellectual and technical concepts contained herein are proprietary to Tobii Pro AB and its suppliers and may be covered by U.S.and Foreign Patents, patent applications, and are protected by trade secret or copyright law. Dissemination of this information or reproduction of this material is strictly forbidden unless prior written permission is obtained from Tobii Pro AB.
*/

/**
 * @file tobii_research.h
 * @brief <b>Generic SDK functions.</b>
 *
 */

#ifndef TOBII_RESEARCH_H_
#define TOBII_RESEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#if _WIN32 || _WIN64
#ifdef TOBII_STATIC_LIB
#define TOBII_RESEARCH_CALL
#define TOBII_RESEARCH_API
#else
#define TOBII_RESEARCH_CALL __cdecl
#ifdef TOBII_EXPORTING
#define TOBII_RESEARCH_API __declspec(dllexport)
#else
#define TOBII_RESEARCH_API __declspec(dllimport)
#endif /* TOBII_EXPORTING */
#endif /* TOBII_STATIC_LIB */
#else
#define TOBII_RESEARCH_API
#define TOBII_RESEARCH_CALL
#endif /* _WIN32 */


/**
Status codes returned by the SDK.
 */
typedef enum {
    /**
    No error.
    */
    TOBII_RESEARCH_STATUS_OK,

    /**
    Fatal error. This should normally not happen.
    */
    TOBII_RESEARCH_STATUS_FATAL_ERROR,

    /**
    Failed to initialize the API. This is a fatal error.
    */
    TOBII_RESEARCH_STATUS_INITIALIZE_FAILED,

    /**
    Failed to terminate the API.
    */
    TOBII_RESEARCH_STATUS_TERMINATE_FAILED,

    /**
    Failed to create browser for finding local devices.
    */
    TOBII_RESEARCH_STATUS_LOCALBROWSER_CREATE_FAILED,

    /**
    Failed to poll local devices.
    */
    TOBII_RESEARCH_STATUS_LOCALBROWSER_POLL_FAILED,

    /**
    Failed to create zero configuration browser.
    */
    TOBII_RESEARCH_STATUS_ZEROCONFBROWSER_CREATE_FAILED,

    /**
    Failed to poll devices from zero configuration browser.
    */
    TOBII_RESEARCH_STATUS_ZEROCONFBROWSER_POLL_FAILED,

    /**
    Failed to create browser that looks for devices in file.
    */
    TOBII_RESEARCH_STATUS_FILEBROWSER_CREATE_FAILED,

    /**
    Failed to poll devices from file browser.
    */
    TOBII_RESEARCH_STATUS_FILEBROWSER_POLL_FAILED,

    /**
    An invalid parameter was given to the method.
    */
    TOBII_RESEARCH_STATUS_INVALID_PARAMETER,

    /**
    The operation was invalid.
    */
    TOBII_RESEARCH_STATUS_INVALID_OPERATION,

    /**
    Internal core error code. Should never be returned by the SDK.
    */
    TOBII_RESEARCH_STATUS_UNINITIALIZED,

    /**
    A parameter is out of bounds.
    */
    TOBII_RESEARCH_STATUS_OUT_OF_BOUNDS,

    /**
    The display area is not valid. Please configure the eye tracker.
    */
    TOBII_RESEARCH_STATUS_DISPLAY_AREA_NOT_VALID,

    /**
    The buffer is too small.
    */
    TOBII_RESEARCH_STATUS_BUFFER_TOO_SMALL,

    /**
    tobii_research_initialize has not been called.
    */
    TOBII_RESEARCH_STATUS_NOT_INITIALIZED,

    /**
    tobii_research_initialize has already been called.
    */
    TOBII_RESEARCH_STATUS_ALREADY_INITIALIZED,

    /**
    The license saved on the device failed to apply when connecting. It has probably expired.
    */
    TOBII_RESEARCH_STATUS_SAVED_LICENSE_FAILED_TO_APPLY,

    /**
    Internal stream engine error.
    */
    TOBII_RESEARCH_STATUS_SE_INTERNAL = 200,

    /**
    The operation requires a higher license type.
    */
    TOBII_RESEARCH_STATUS_SE_INSUFFICIENT_LICENSE,

    /**
    The operations isn't supported in the current context.
    */
    TOBII_RESEARCH_STATUS_SE_NOT_SUPPORTED,

    /**
    The device is unavailable.
    */
    TOBII_RESEARCH_STATUS_SE_NOT_AVAILABLE,

    /**
    Connection to the device failed.
    */
    TOBII_RESEARCH_STATUS_SE_CONNECTION_FAILED,

    /**
    The operation timed out.
    */
    TOBII_RESEARCH_STATUS_SE_TIMED_OUT,

    /**
    Failed to allocate memory.
    */
    TOBII_RESEARCH_STATUS_SE_ALLOCATION_FAILED,

    /**
    The API is already initialized.
    */
    TOBII_RESEARCH_STATUS_SE_ALREADY_INITIALIZED,

    /**
    The API isn't initialized.
    */
    TOBII_RESEARCH_STATUS_SE_NOT_INITIALIZED,

    /**
    An invalid parameter was given to the method.
    */
    TOBII_RESEARCH_STATUS_SE_INVALID_PARAMETER,

    /**
    Calibration has already started.
    */
    TOBII_RESEARCH_STATUS_SE_CALIBRATION_ALREADY_STARTED,

    /**
    Calibration isn't started.
    */
    TOBII_RESEARCH_STATUS_SE_CALIBRATION_NOT_STARTED,

    /**
    Already subscribed.
    */
    TOBII_RESEARCH_STATUS_SE_ALREADY_SUBSCRIBED,

    /**
    Not subscribed.
    */
    TOBII_RESEARCH_STATUS_SE_NOT_SUBSCRIBED,

    /**
    Operation failed.
    */
    TOBII_RESEARCH_STATUS_SE_OPERATION_FAILED,

    /**
    Conflicting api instances.
    */
    TOBII_RESEARCH_STATUS_SE_CONFLICTING_API_INSTANCES,

    /**
    Calibration busy.
    */
    TOBII_RESEARCH_STATUS_SE_CALIBRATION_BUSY,

    /**
    Callback in progress.
    */
    TOBII_RESEARCH_STATUS_SE_CALLBACK_IN_PROGRESS,

    /**
    Too many users subscribed to a stream.
    */
    TOBII_RESEARCH_STATUS_SE_TOO_MANY_SUBSCRIBERS,

    /**
    The buffer is too small.
    */
    TOBII_RESEARCH_STATUS_SE_BUFFER_TOO_SMALL,

    /**
    No response from firmware.
    */
    TOBII_RESEARCH_STATUS_SE_FIRMWARE_NO_RESPONSE,

    /**
    Internal error.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_INTERNAL = 400,

    /**
    Firmware upgrade is not supported.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_NOT_SUPPORTED,

    /**
    Unknown firmware version.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_UNKNOWN_FIRMWARE_VERSION,

    /**
    Connection failed.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_CONNECTION_FAILED,

    /**
    Invalid parameter.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_INVALID_PARAMETER,

    /**
    Device mismatch. The firmware package is not meant for the device.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_PACKAGE_DEVICE_MISMATCH,

    /**
    Parse response.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_PARSE_RESPONSE,

    /**
    The firmware upgrade operation failed.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_OPERATION_FAILED,

    /**
    Memory allocation failed during firmware upgrade.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_ALLOCATION_FAILED,

    /**
    The firmware failed to respond during firmware upgrade.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_FIRMWARE_NO_RESPONSE,

    /**
    The firmware downgrade operation is not supported.
    */
    TOBII_RESEARCH_STATUS_FWUPGRADE_DOWNGRADE_NOT_SUPPORTED,

    /**
    Unknown error. This is a fatal error.
    */
    TOBII_RESEARCH_STATUS_UNKNOWN = 1000
} TobiiResearchStatus;

/**
Opaque representation of an eye tracker struct.
*/
typedef struct TobiiResearchEyeTracker TobiiResearchEyeTracker;

/**
Contains all eye trackers connected to the computer or the network.
*/
typedef struct {
    /**
    An array of pointers to eye trackers.
    */
    TobiiResearchEyeTracker** eyetrackers;
    /**
    Number of eye tracker pointers in the array.
    */
    size_t count;
} TobiiResearchEyeTrackers;

/**
Source of log message.
*/
typedef enum {
    /**
    The log message is from stream engine.
    */
    TOBII_RESEARCH_LOG_SOURCE_STREAM_ENGINE,

    /**
    The log message is from the SDK.
    */
    TOBII_RESEARCH_LOG_SOURCE_SDK,

    /**
    The log message is from the firmware upgrade module.
    */
    TOBII_RESEARCH_LOG_SOURCE_FIRMWARE_UPGRADE
} TobiiResearchLogSource;

/**
Log level.
*/
typedef enum {
    /**
    Error message.
    */
    TOBII_RESEARCH_LOG_LEVEL_ERROR,

    /**
    Warning message.
    */
    TOBII_RESEARCH_LOG_LEVEL_WARNING,

    /**
    Information message.
    */
    TOBII_RESEARCH_LOG_LEVEL_INFORMATION,

    /**
    Debug message.
    */
    TOBII_RESEARCH_LOG_LEVEL_DEBUG,

    /**
    Trace message.
    */
    TOBII_RESEARCH_LOG_LEVEL_TRACE
} TobiiResearchLogLevel;

/**
Represents a normalized x- and y-coordinate point in a two-dimensional plane.
 */
typedef struct {
    /**
    Position of the point in the X axis.
    */
    float x;
    /**
    Position of the point in the Y axis.
    */
    float y;
} TobiiResearchNormalizedPoint2D;


/**
Represents an x-, y- and z-coordinate point in a three-dimensional space.
 */
typedef struct {
    /**
    Position of the point in the X axis.
    */
    float x;
    /**
    Position of the point in the Y axis.
    */
    float y;
    /**
    Position of the point in the Z axis.
    */
    float z;
} TobiiResearchPoint3D;

/**
Represents a normalized x-, y- and z-coordinate point in a three-dimensional space.
 */
typedef TobiiResearchPoint3D TobiiResearchNormalizedPoint3D;

/**
@brief Log callback.

Implement this and send as a parameter to @ref tobii_research_logging_subscribe.
@param system_time_stamp: The time stamp according to the computer's internal clock.
@param source: Source of log message.
@param level: Log message level.
@param message: The log message.
*/
typedef void(*tobii_research_log_callback)(int64_t system_time_stamp,
                TobiiResearchLogSource source,
                TobiiResearchLogLevel level,
                const char* message);

/**
@brief Subscribes to logging.

@param callback: Callback that will receive log messages.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_logging_subscribe(
                        tobii_research_log_callback callback);

/**
@brief Unsubscribes from logging.

@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_logging_unsubscribe();

/**
@brief Finds eye trackers connected to the computer or the network.

The eye trackers can be used in any tobii_research function that requires an eye tracker.

\snippet find_all_eyetrackers.c FindAllEyetrackers

@param eyetrackers: Pointers to found eye trackers will be stored in this struct.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_find_all_eyetrackers(
                        TobiiResearchEyeTrackers** eyetrackers);

/**
@brief Free memory allocation for the result received via @ref tobii_research_find_all_eyetrackers.

@param eyetrackers: Eye trackers to free.
*/
TOBII_RESEARCH_API void TOBII_RESEARCH_CALL tobii_research_free_eyetrackers(
                        TobiiResearchEyeTrackers* eyetrackers);

/**
@brief Gets data for an eye tracker given an address.

\snippet create_eyetracker.c Example

@param address: Address of eye tracker to get data for.
@param eyetracker: Eye tracker object returned.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_eyetracker(
                        const char* address,
                        TobiiResearchEyeTracker** eyetracker);

/**
@brief Retrieves the time stamp from the system clock in microseconds.

\snippet get_system_time_stamp.c Example

@param time_stamp_us: The time stamp of the system in microseconds.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_system_time_stamp(int64_t* time_stamp_us);

/**
@brief Free memory allocation for a string allocated by the SDK.

@param str: String to free.
*/
TOBII_RESEARCH_API void TOBII_RESEARCH_CALL tobii_research_free_string(char* str);


/**
SDK Version.
*/
typedef struct {
    /**
    Major.
    */
    int major;
    /**
    Minor.
    */
    int minor;
    /**
    Revision.
    */
    int revision;
    /**
    Build.
    */
    int build;
} TobiiResearchSDKVersion;

/**
Gets the SDK version.

@param sdk_version: Version of the SDK.
@returns A TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_sdk_version(
                        TobiiResearchSDKVersion* sdk_version);


/**
"Defines the overall status of a calibration process.
*/
typedef enum {
    /**
    Indicates that the calibration process failed.
    */
    TOBII_RESEARCH_CALIBRATION_FAILURE = 0,
    /**
    Indicates that the calibration process succeeded for both eyes.
    */
    TOBII_RESEARCH_CALIBRATION_SUCCESS = 1,
    /**
    Indicates that the left eye calibration process succeeded.
    */
    TOBII_RESEARCH_CALIBRATION_SUCCESS_LEFT_EYE = 2,
    /**
    Indicates that the right eye calibration process succeeded.
    */
    TOBII_RESEARCH_CALIBRATION_SUCCESS_RIGHT_EYE = 3
} TobiiResearchCalibrationStatus;

/**
Defines the selected eye.
*/
typedef enum TobiiResearchSelectedEye {
    /**
    Left Eye
    */
    TOBII_RESEARCH_SELECTED_EYE_LEFT,

    /**
    Right Eye
    */
    TOBII_RESEARCH_SELECTED_EYE_RIGHT,

    /**
    Both Eyes
    */
    TOBII_RESEARCH_SELECTED_EYE_BOTH
} TobiiResearchSelectedEye;

/**
Defines the validity of calibration eye data.
*/
typedef enum {
    /**
    The eye tracking failed or the calibration eye data is invalid.
    */
    TOBII_RESEARCH_CALIBRATION_EYE_VALIDITY_INVALID_AND_NOT_USED = -1,

    /**
    Eye Tracking was successful, but the calibration eye data was not used in calibration e.g. gaze was too far away.
    */
    TOBII_RESEARCH_CALIBRATION_EYE_VALIDITY_VALID_BUT_NOT_USED = 0,

    /**
    The calibration eye data was valid and used in calibration.
    */
    TOBII_RESEARCH_CALIBRATION_EYE_VALIDITY_VALID_AND_USED = 1,

    /**
    The calibration eye data has an unexpected validity.
    */
    TOBII_RESEARCH_CALIBRATION_EYE_VALIDITY_UNKNOWN
} TobiiResearchCalibrationEyeValidity;

/**
Represents the calibration sample data collected for one eye.
 */
typedef struct {
    /**
    The eye sample position on the active display Area for the left eye.
    */
    TobiiResearchNormalizedPoint2D position_on_display_area;
    /**
    Information about if the sample was used or not in the calculation for the left eye.
    */
    TobiiResearchCalibrationEyeValidity validity;
} TobiiResearchCalibrationEyeData;

/**
Represents the data collected for a calibration sample.
 */
typedef struct {
    /**
    The calibration sample data for the left eye.
    */
    TobiiResearchCalibrationEyeData left_eye;
    /**
    The calibration sample data for the right eye.
    */
    TobiiResearchCalibrationEyeData right_eye;
} TobiiResearchCalibrationSample;

/**
Represents the Calibration Point and its collected calibration samples.
*/
typedef struct {
    /**
    The position of the calibration point in the Active Display Area.
    */
    TobiiResearchNormalizedPoint2D position_on_display_area;
    /**
    An array of calibration samples.
    */
    TobiiResearchCalibrationSample* calibration_samples;
    /**
    Number of calibration calibration points in the array.
    */
    size_t calibration_sample_count;
} TobiiResearchCalibrationPoint;

/**
Represents the result of the calculated calibration.
*/
typedef struct {
    /**
    Array of calibration points.
    */
    TobiiResearchCalibrationPoint* calibration_points;
    /**
    Number of calibration calibration points in the array.
    */
    size_t calibration_point_count;
    /**
    Gets the status of the calculation.
    */
    TobiiResearchCalibrationStatus status;
} TobiiResearchCalibrationResult;


/**
Represents the result of the calculated HMD based calibration.
*/
typedef struct {
    /**
    Gets the status of the calculation.
    */
    TobiiResearchCalibrationStatus status;
} TobiiResearchHMDCalibrationResult;

/**
@brief Enters the screen based calibration mode and the eye tracker is made ready for collecting data and calculating new calibrations.

\snippet calibration.c CalibrationEnterExample

@param eyetracker: Eye tracker object.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
                       tobii_research_screen_based_calibration_enter_calibration_mode(
                               TobiiResearchEyeTracker* eyetracker);

/**
@brief Leaves the screen based calibration mode.

\snippet calibration.c CalibrationLeftExample

@param eyetracker: Eye tracker object.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
                        tobii_research_screen_based_calibration_leave_calibration_mode(
                                TobiiResearchEyeTracker* eyetracker);

/**
@brief Starts collecting data for a calibration point.

The argument used is the point the calibration user is assumed to
be looking at and is given in the active display area coordinate system.

You must call tobii_research_calibration_enter_calibration_mode before calling this function.
This function is blocking while collecting data and may take up to 10 seconds.

\snippet calibration.c CalibrationExample

@param eyetracker: Eye tracker object.
@param x: Normalized x coordinate on active display area where the user is looking.
@param y: Normalized y coordinate on active display area where the user is looking.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_screen_based_calibration_collect_data(
        TobiiResearchEyeTracker* eyetracker,
        float x,
        float y);

/**
@brief Removes the collected data associated with a specific calibration point.

\snippet calibration.c ReCalibrationExample

@param eyetracker: Eye tracker object.
@param x: Normalized x coordinate of point to discard.
@param y: Normalized y coordinate of point to discard.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_screen_based_calibration_discard_data(
        TobiiResearchEyeTracker* eyetracker,
        float x,
        float y);

/**
@brief Uses the collected data and tries to compute calibration parameters.

If the calculation is successful, the result is applied to the eye tracker.
If there is insufficient data to compute a new calibration or if the collected
data is not good enough then calibration is failed and will not be applied.

\snippet calibration.c CalibrationExample

@param eyetracker: Eye tracker object.
@param result: Represents the result of the calculated calibration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_screen_based_calibration_compute_and_apply(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchCalibrationResult** result);

/**
@brief Free memory allocation for the calibration result received via @ref tobii_research_screen_based_calibration_compute_and_apply.

@param result: Calibration result to free.
*/
TOBII_RESEARCH_API void TOBII_RESEARCH_CALL tobii_research_free_screen_based_calibration_result(
        TobiiResearchCalibrationResult* result);

/**
@brief Starts collecting data for a calibration point.

The argument used is the point the calibration user is assumed to
be looking at and is given in the active display area coordinate system.

You must call tobii_research_calibration_enter_calibration_mode before calling this function.
This function is blocking while collecting data and may take up to 10 seconds.

@param eyetracker: Eye tracker object.
@param x: Normalized x coordinate on active display area where the user is looking.
@param y: Normalized y coordinate on active display area where the user is looking.
@param eye_to_calibrate: TobiiResearchSelectedEye instance that selects for which eye to collect data for the monocular calibration.
@param collected_eyes: TobiiResearchSelectedEye instance that indicates for which eyes data was collected for the monocular calibration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
    tobii_research_screen_based_monocular_calibration_collect_data(TobiiResearchEyeTracker* eyetracker,
                float x, float y, TobiiResearchSelectedEye eye_to_calibrate, TobiiResearchSelectedEye* collected_eyes);

/**
@brief Removes the collected data associated with a specific calibration point.

@param eyetracker: Eye tracker object.
@param x: Normalized x coordinate of point to discard.
@param y: Normalized y coordinate of point to discard.
@param eye_to_calibrate:  TobiiResearchSelectedEye instance that selects for which eye to discard data for the monocular calibration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
    tobii_research_screen_based_monocular_calibration_discard_data(TobiiResearchEyeTracker* eyetracker,
                float x, float y, TobiiResearchSelectedEye eye_to_calibrate);
/**
@brief Uses the collected data and tries to compute calibration parameters.

If the calculation is successful, the result is
applied to the eye tracker. If there is insufficient data to compute a new calibration or if the collected data is
not good enough then calibration is failed and will not be applied.

@param eyetracker: Eye tracker object.
@param calibrated_eyes: TobiiResearchSelectedEye instance with the calibrated eyes in the monocular calibration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
    tobii_research_screen_based_monocular_calibration_compute_and_apply(TobiiResearchEyeTracker* eyetracker,
        TobiiResearchCalibrationResult** result);

/**
@brief Enters the hmd based calibration mode and the eye tracker is made ready for collecting data and calculating new calibrations.

\snippet hmd_calibration.c HMDCalibrationEnterExample

@param eyetracker: Eye tracker object.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
                       tobii_research_hmd_based_calibration_enter_calibration_mode(
                               TobiiResearchEyeTracker* eyetracker);

/**
@brief Leaves the hmd based calibration mode.

\snippet hmd_calibration.c HMDCalibrationLeftExample

@param eyetracker: Eye tracker object.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL
                        tobii_research_hmd_based_calibration_leave_calibration_mode(
                                TobiiResearchEyeTracker* eyetracker);

/**
@brief Starts collecting data for a calibration point.

The argument used is the point the calibration user is assumed to be looking at and is given in the HMD coordinate system.

You must call tobii_research_screen_based_calibration_enter_calibration_mode before calling this function.
This function is blocking while collecting data and may take up to 10 seconds.

@param eyetracker: Eye tracker object.
@param x: x coordinate in the HMD coordinate system where the user is looking.
@param y: y coordinate in the HMD coordinate system where the user is looking.
@param z: z coordinate in the HMD coordinate system where the user is looking.

@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_hmd_based_calibration_collect_data(
    TobiiResearchEyeTracker* eyetracker, float x, float y, float z);


/**
@brief Uses the collected data and tries to compute calibration parameters.

If the calculation is successful, the result is applied to the eye tracker.
If there is insufficient data to compute a new calibration or if the collected
data is not good enough then calibration is failed and will not be applied.

\snippet hmd_calibration.c HMDCalibrationExample

@param eyetracker: Eye tracker object.
@param result: Represents the result of the calculated HMD calibration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_hmd_based_calibration_compute_and_apply(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchHMDCalibrationResult* result);


/**
Defines the capabilities.
*/
typedef enum {
    /**
    * No capabilities set.
    */
    TOBII_RESEARCH_CAPABILITIES_NONE,

    /**
    * Indicates that the device can have display areas set.
    */
    TOBII_RESEARCH_CAPABILITIES_CAN_SET_DISPLAY_AREA = 1 << 0,

    /**
    * Indicates that the device can deliver an external signal stream.
    */
    TOBII_RESEARCH_CAPABILITIES_HAS_EXTERNAL_SIGNAL = 1 << 1,

    /**
    * Indicates that the device can deliver an eye image stream.
    */
    TOBII_RESEARCH_CAPABILITIES_HAS_EYE_IMAGES = 1 << 2,

    /**
    * Indicates that the device can deliver a gaze data stream. Standard for all screen based eye trackers.
    */
    TOBII_RESEARCH_CAPABILITIES_HAS_GAZE_DATA  = 1 << 3,

    /**
    * Indicates that the device can deliver a HMD gaze data stream.
    */
    TOBII_RESEARCH_CAPABILITIES_HAS_HMD_GAZE_DATA = 1 << 4,

    /**
    * Indicates that screen based calibration can be performed on the device.
    */
    TOBII_RESEARCH_CAPABILITIES_CAN_DO_SCREEN_BASED_CALIBRATION = 1 << 5,

    /**
    * Indicates that HMD based calibration can be performed on the device.
    */
    TOBII_RESEARCH_CAPABILITIES_CAN_DO_HMD_BASED_CALIBRATION = 1 << 6,

    /**
    * Indicates that it's possible to get and set the HMD lens configuration on the device.
    */
    TOBII_RESEARCH_CAPABILITIES_HAS_HMD_LENS_CONFIG = 1 << 7,

    /**
    * Indicates that monocular calibration can be performed on the device.
    */
    TOBII_RESEARCH_CAPABILITIES_CAN_DO_MONOCULAR_CALIBRATION = 1 << 8,
} TobiiResearchCapabilities;

/**
Represents the eight corners in user coordinate system that together forms the track box.
*/
typedef struct {
    /**
    The back lower left corner of the track box.
    */
    TobiiResearchPoint3D back_lower_left;

    /**
    The back lower right corner of the track box.
    */
    TobiiResearchPoint3D back_lower_right;

    /**
    The back upper left corner of the track box.
    */
    TobiiResearchPoint3D back_upper_left;

    /**
    The back upper right corner of the track box.
    */
    TobiiResearchPoint3D back_upper_right;

    /**
    The front lower left corner of the track box.
    */
    TobiiResearchPoint3D front_lower_left;

    /**
    The front lower right corner of the track box.
    */
    TobiiResearchPoint3D front_lower_right;

    /**
    The front upper left corner of the track box.
    */
    TobiiResearchPoint3D front_upper_left;

    /**
    The front upper right corner of the track box.
    */
    TobiiResearchPoint3D front_upper_right;
} TobiiResearchTrackBox;

/**
Specifies license validation result.
*/
typedef enum TobiiResearchLicenseValidationResult {
    /**
    The license is ok.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_OK,

    /**
    The license is tampered.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_TAMPERED,

    /**
    The application signature is invalid.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_INVALID_APPLICATION_SIGNATURE,

    /**
    The application has not been signed.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_NONSIGNED_APPLICATION,

    /**
    The license has expired.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_EXPIRED,

    /**
    The license is not yet valid.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_PREMATURE,

    /**
    The process name does not match the license.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_INVALID_PROCESS_NAME,

    /**
    The serial number does not match the license.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_INVALID_SERIAL_NUMBER,

    /**
    The model does not match the license.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_INVALID_MODEL,

    /**
    The license validation returned an unexpected result.
    */
    TOBII_RESEARCH_LICENSE_VALIDATION_RESULT_UNKNOWN
} TobiiResearchLicenseValidationResult;

/**
Represents the corners of the active display area in the user coordinate system, and its size.
BottomRight, Height, and Width are calculated values.
*/
typedef struct {
    /**
    The bottom left corner of the active display area.
    */
    TobiiResearchPoint3D bottom_left;

    /**
    The bottom right corner of the active display area.
    */
    TobiiResearchPoint3D bottom_right;

    /**
    The height in millimeters of the active display area.
    */
    float height;

    /**
    The top left corner of the active display area.
    */
    TobiiResearchPoint3D top_left;

    /**
    The top right corner of the active display area.
    */
    TobiiResearchPoint3D top_right;

    /**
    The width in millimeters of the active display area.
    */
    float width;
} TobiiResearchDisplayArea;

/**
Represents the calibration data used by the eye tracker.
 */
typedef struct {
    /**
     The calibration data used by the eye tracker.
     */
    void* data;
    /**
     The size of the calibration data used by the eye tracker.
     */
    size_t size;
} TobiiResearchCalibrationData;

/**
Represents the gaze output frequencies supported by the eye tracker.
 */
typedef struct {
    /**
    An array of gaze sampling frequencies supported by the eye tracker.
    */
    float* frequencies;
    /**
    The number of gaze output frequencies.
    */
    size_t frequency_count;
} TobiiResearchGazeOutputFrequencies;

/**
Represents the eye tracking modes supported by the eye tracker.
 */
typedef struct {
    /**
    An array of strings containing eye tracking modes supported by the eye tracker.
    */
    char** modes;
    /**
    The number of eye tracking modes supported by the eye tracker.
    */
    size_t mode_count;
} TobiiResearchEyeTrackingModes;

/**
Represents the lens configuration of the HMD device.
 */
typedef struct {
    /**
    The point in HMD coordinate system that defines the position of the left lens (in millimeters).
     */
    TobiiResearchPoint3D left;
    /**
    The point in HMD coordinate system that defines the position of the right lens (in millimeters).
     */
    TobiiResearchPoint3D right;
} TobiiResearchHMDLensConfiguration;

/**
@brief Gets the address (URI) of the eye tracker device.

\snippet find_all_eyetrackers.c GetEyetrackerProps

@param eyetracker: Eye tracker object.
@param address: Address as string, should be freed when not in use by @ref tobii_research_free_string.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_address(
        TobiiResearchEyeTracker* eyetracker, char** address);
/**
@brief Gets the serial number of the eye tracker. All physical eye trackers have a unique serial number.

\snippet find_all_eyetrackers.c GetEyetrackerProps

@param eyetracker: Eye tracker object.
@param serial_number: Serial number as string, should be freed when not in use by @ref tobii_research_free_string.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_serial_number(
        TobiiResearchEyeTracker* eyetracker, char** serial_number);

/**
@brief Gets the name of the eye tracker.

\snippet find_all_eyetrackers.c GetEyetrackerProps

@param eyetracker: Eye tracker object.
@param device_name: Device name as string, should be freed when not in use by @ref tobii_research_free_string.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_device_name(
        TobiiResearchEyeTracker* eyetracker, char** device_name);

/**
@brief Gets the model of the eye tracker.

@param eyetracker: Eye tracker object.
@param model: Model as string, should be freed when not in use by @ref tobii_research_free_string.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_model(
        TobiiResearchEyeTracker* eyetracker, char** model);

/**
@brief Gets the firmware version of the eye tracker.

@param eyetracker: Eye tracker object.
@param fw_version: Firmware version as string, should be freed when not in use by @ref tobii_research_free_string.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_firmware_version(
        TobiiResearchEyeTracker* eyetracker, char** fw_version);

/**
@brief Gets the runtime build version of the eye tracker.

@param eyetracker: Eye tracker object.
@param fw_version: Runtime build version as string, should be freed when not in use by @ref tobii_research_free_string.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_runtime_version(
        TobiiResearchEyeTracker* eyetracker, char** runtime_version);


/**
@brief Gets the capabilities of the device.

@param eyetracker: Eye tracker object.
@param capabilities: Bit array where each bit set indicates a supported capability, see @ref TobiiResearchCapabilities.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_capabilities(
        TobiiResearchEyeTracker* eyetracker, TobiiResearchCapabilities* capabilities);

/**
@brief Gets the calibration data used currently by the eye tracker.

This data can be saved to a file for later use. See @ref tobii_research_apply_calibration_data

\snippet calibration_data.c GetCalibrationData

@param eyetracker: Eye tracker object.
@param data: Calibration data.
@returns A @ref TobiiResearchStatus code.
 */
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_retrieve_calibration_data(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchCalibrationData** data);

/**
@brief Free memory allocation for the calibration data received via tobii_research_retrieve_calibration_data.

@param data: Calibration data to free.
*/
TOBII_RESEARCH_API void TOBII_RESEARCH_CALL tobii_research_free_calibration_data(
        TobiiResearchCalibrationData* data);

/**
@brief Sets the provided calibration data to the eye tracker, which means it will be active calibration.

This function should not be called during calibration. Also see @ref tobii_research_retrieve_calibration_data.

\snippet calibration_data.c ApplyCalibrationData

@param eyetracker: Eye tracker object.
@param data: Calibration data.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_apply_calibration_data(
        TobiiResearchEyeTracker* eyetracker,
        const TobiiResearchCalibrationData* data);

/**
@brief Gets an array of gaze output frequencies supported by the eye tracker.

\snippet gaze_output_frequencies.c GetOutputFrequencies

@param eyetracker: Eye tracker object.
@param frequencies: Gaze output frequencies.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_all_gaze_output_frequencies(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchGazeOutputFrequencies** frequencies);

/**
@brief Free memory allocation for the gaze output frequencies received via tobii_research_free_gaze_output_frequencies.

@param frequencies: Gaze output frequencies to free.
*/
TOBII_RESEARCH_API void TOBII_RESEARCH_CALL tobii_research_free_gaze_output_frequencies(
        TobiiResearchGazeOutputFrequencies* frequencies);

/**
@brief Gets the gaze output frequency of the eye tracker.

\snippet gaze_output_frequencies.c GetOutputFrequencie

@param eyetracker: Eye tracker object.
@param gaze_output_frequency: The current gaze output frequency.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_gaze_output_frequency(
        TobiiResearchEyeTracker* eyetracker,
        float* gaze_output_frequency);

/**
@brief Sets the gaze output frequency of the eye tracker.

\snippet gaze_output_frequencies.c SetOutputFrequencie

@param eyetracker: Eye tracker object.
@param gaze_output_frequency: The gaze output frequency.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_set_gaze_output_frequency(
        TobiiResearchEyeTracker* eyetracker,
        float gaze_output_frequency);

/**
@brief Gets a all eye tracking modes supported by the eye tracker.

\snippet eye_tracking_modes.c GetEyeTrackingModes

@param eyetracker: Eye tracker object.
@param modes: Eye tracking modes.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_all_eye_tracking_modes(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchEyeTrackingModes** modes);


/**
@brief Free memory allocation for the eye tracking modes received via tobii_research_get_all_eye_tracking_modes.

@param modes: Eye tracker modes to free.
*/
TOBII_RESEARCH_API void TOBII_RESEARCH_CALL tobii_research_free_eye_tracking_modes(
        TobiiResearchEyeTrackingModes* modes);

/**
@brief Gets the eye tracking mode of the eye tracker.

\snippet eye_tracking_modes.c GetEyeTrackingMode

@param eyetracker: Eye tracker object.
@param eye_tracking_mode: The current eye tracking mode.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_eye_tracking_mode(
        TobiiResearchEyeTracker* eyetracker,
        char** eye_tracking_mode);

/**
@brief Sets the eye tracking mode of the eye tracker.

\snippet eye_tracking_modes.c SetEyeTrackingMode

@param eyetracker: Eye tracker object.
@param eye_tracking_mode: The eye tracking mode.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_set_eye_tracking_mode(
        TobiiResearchEyeTracker* eyetracker,
        const char* eye_tracking_mode);

/**
@brief Gets the track box of the eye tracker.

\snippet get_track_box.c Example

@param eyetracker: Eye tracker object.
@param track_box: Track box coordinates.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_track_box(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchTrackBox* track_box);

/**
@brief Apply one or more licenses to unlock features of the eye tracker.

The validation_results array indicates whether all licenses were applied or not.

\snippet apply_licenses.c Example

@param eyetracker: Eye tracker object.
@param license_key: Licenses to apply.
@param license_keys_size: A list of license key sizes.
@param validation_results: Optional. Validation result for each license.
@param number_of_licenses: Number of licenses in license_key_ring.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_apply_licenses(
        TobiiResearchEyeTracker* eyetracker,
        const void** license_key,
        size_t* license_keys_size,
        TobiiResearchLicenseValidationResult* validation_results,
        size_t number_of_licenses);


/**
@brief Clears any previously applied licenses.

@param eyetracker: Eye tracker object.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_clear_applied_licenses(
        TobiiResearchEyeTracker* eyetracker);

/**
@brief Gets the size and corners of the display area.

\snippet get_display_area.c Example

@param eyetracker: Eye tracker object.
@param display_area: The eye tracker's display area.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_display_area(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchDisplayArea* display_area);

/**
@brief Sets the display area of the eye tracker. It is strongly recommended to use
Eye Tracker Manager to calculate the display area coordinates as the origin of the
User Coordinate System differs between eye tracker models.

\snippet get_display_area.c Example

@param eyetracker: Eye tracker object.
@param display_area: The eye tracker's desired display area.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_set_display_area(
        TobiiResearchEyeTracker* eyetracker,
        const TobiiResearchDisplayArea* display_area);

/**
@brief Changes the device name. This is not supported by all eye trackers.

\snippet set_device_name.c Example

@param eyetracker: Eye tracker object.
@param device_name: The eye tracker's desired name.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_set_device_name(
        TobiiResearchEyeTracker* eyetracker,
        const char* device_name);

/**
@brief Gets the current lens configuration of the HMD based eye tracker.
The lens configuration describes how the lenses of the HMD device are positioned.

\snippet get_hmd_lens_configuration.c Example

@param eyetracker: Eye tracker object.
@param lens_configuration: The eye tracker's lens_configuration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_get_hmd_lens_configuration(
        TobiiResearchEyeTracker* eyetracker,
        TobiiResearchHMDLensConfiguration* lens_configuration);

/**
@brief Sets the lens configuration of the HMD based eye tracker.
The lens configuration describes how the lenses of the HMD device are positioned.

\snippet set_hmd_lens_configuration.c Example

@param eyetracker: Eye tracker object.
@param lens_configuration: The eye tracker's desired lens_configuration.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_set_hmd_lens_configuration(
        TobiiResearchEyeTracker* eyetracker,
        const TobiiResearchHMDLensConfiguration* lens_configuration);

/**
Specifies the validity.
*/
typedef enum {
    /**
    Indicates invalid.
    */
    TOBII_RESEARCH_VALIDITY_INVALID,

    /**
    Indicates valid.
    */
    TOBII_RESEARCH_VALIDITY_VALID
} TobiiResearchValidity;

/**
Provides properties for the gaze origin.
*/
typedef struct {
    /**
    The gaze origin position in 3D in the user coordinate system.
    */
    TobiiResearchPoint3D position_in_user_coordinates;

    /**
    The normalized gaze origin position in 3D in the track box coordinate system.
    */
    TobiiResearchNormalizedPoint3D position_in_track_box_coordinates;

    /**
     The validity of the gaze origin data.
    */
    TobiiResearchValidity validity;
} TobiiResearchGazeOrigin;

/**
Provides properties for the pupil data.
*/
typedef struct {
    /**
    The diameter of the pupil in millimeters.
    */
    float diameter;

    /**
    The validity of the pupil data.
    */
    TobiiResearchValidity validity;
} TobiiResearchPupilData;

/**
Provides properties for the gaze point.
*/
typedef struct {
    /**
    The gaze point position in 2D on the active display area.
    */
    TobiiResearchNormalizedPoint2D position_on_display_area;

    /**
    The gaze point position in 3D in the user coordinate system.
    */
    TobiiResearchPoint3D position_in_user_coordinates;

    /**
    The validity of the gaze point data.
    */
    TobiiResearchValidity validity;
} TobiiResearchGazePoint;

/**
Provides properties for the eye data.
*/
typedef struct {
    /**
    The gaze point data.
    */
    TobiiResearchGazePoint gaze_point;

    /**
    The pupil data.
    */
    TobiiResearchPupilData pupil_data;

    /**
    The gaze origin data.
    */
    TobiiResearchGazeOrigin gaze_origin;
} TobiiResearchEyeData;

/**
Provides data for the @ref tobii_research_gaze_data_callback callback.
*/
typedef struct {
    /**
    The gaze data for the left eye.
    */
    TobiiResearchEyeData left_eye;

    /**
    The gaze data for the right eye.
    */
    TobiiResearchEyeData right_eye;

    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t device_time_stamp;

    /**
    The time stamp according to the computer's internal clock.
    */
    int64_t system_time_stamp;
} TobiiResearchGazeData;


/**
Provides properties eye user position guide.
*/
typedef struct {
    /**
    The (normalized) 3D coordinates that describes the user's position, (0.5, 0.5, 0.5) is the ideal position".
    */
    TobiiResearchNormalizedPoint3D user_position;

    /**
    The validity of the user postion guide.
    */
    TobiiResearchValidity validity;
} TobiiResearchEyeUserPositionGuide;


/**
Provides data for the @ref tobii_research_user_position_guide_callback callback.
*/
typedef struct {
    /**
    The user position guide for the left eye.
    */
    TobiiResearchEyeUserPositionGuide left_eye;

    /**
    The user position guide for the right eye.
    */
    TobiiResearchEyeUserPositionGuide right_eye;
} TobiiResearchUserPositionGuide;


/**
Provides properties for the HMD pupil position.
*/
typedef struct {
    /**
    The (normalized) 2D coordinates that describes the pupil's position in the HMD's tracking area.
    */
    TobiiResearchNormalizedPoint2D position_in_tracking_area;

    /**
    The validity of the pupil position data.
    */
    TobiiResearchValidity validity;
} TobiiResearchHMDPupilPosition;

/**
Provides properties for the HMD gaze origin.
*/
typedef struct {
    /**
    The 3D coordinates that describes the gaze origin in (in mm).
    */
    TobiiResearchPoint3D position_in_hmd_coordinates;

    /**
    The validity of the gaze origin data.
    */
    TobiiResearchValidity validity;
} TobiiResearchHMDGazeOrigin;

/**
Provides properties for the HMD gaze direction.
*/
typedef struct {
    /**
    The 3D unit vector that describes the gaze direction.
    */
    TobiiResearchNormalizedPoint3D unit_vector;

    /**
    The validity of the gaze direction data.
    */
    TobiiResearchValidity validity;
} TobiiResearchHMDGazeDirection;

/**
Provides properties for the eye data when gotten from an HMD based device.
*/
typedef struct {
    /**
    The gaze direction data.
    */
    TobiiResearchHMDGazeDirection gaze_direction;

    /**
    The pupil data.
    */
    TobiiResearchPupilData pupil_data;

    /**
    The gaze origin data.
    */
    TobiiResearchHMDGazeOrigin gaze_origin;

    /**
    The pupil position in HMD track box.
    */
    TobiiResearchHMDPupilPosition pupil_position;
} TobiiResearchHMDEyeData;

/**
Provides data for the @ref tobii_research_hmd_gaze_data_callback callback.
*/
typedef struct {
    /**
    The gaze data for the left eye.
    */
    TobiiResearchHMDEyeData left_eye;

    /**
    The gaze data for the right eye.
    */
    TobiiResearchHMDEyeData right_eye;

    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t device_time_stamp;

    /**
    The time stamp according to the computer's internal clock.
    */
    int64_t system_time_stamp;
} TobiiResearchHMDGazeData;

/**
Provides data for the @ref tobii_research_time_synchronization_data_callback callback.
*/
typedef struct {
    /**
    The time stamp when the computer sent the request to the eye tracker.
    */
    int64_t system_request_time_stamp;

    /**
    The time stamp when the eye tracker received the request, according to the eye tracker's clock.
    */
    int64_t device_time_stamp;

    /**
    The time stamp when the computer received the response from the eye tracker.
    */
    int64_t system_response_time_stamp;
} TobiiResearchTimeSynchronizationData;

/**
Defines external signal change type.
*/
typedef enum {
    /**
    Indicates that the value sent to the eye tracker has changed.
    */
    TOBII_RESEARCH_EXTERNAL_SIGNAL_VALUE_CHANGED,

    /**
    Indicates that the value is the initial value, and is received when starting a subscription.
    */
    TOBII_RESEARCH_EXTERNAL_SIGNAL_INITIAL_VALUE,

    /**
    Indicates that there has been a connection lost and now it is restored and the value is the current value.
    */
    TOBII_RESEARCH_EXTERNAL_SIGNAL_CONNECTION_RESTORED
} TobiiResearchExternalSignalChangeType;

/**
Provides data for the @ref tobii_research_external_signal_data_callback callback.
*/
typedef struct {
    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t device_time_stamp;

    /**
    The time stamp according to the computer's internal clock.
    */
    int64_t system_time_stamp;

    /**
    The value of the external signal port on the eye tracker.
    */
    uint32_t value;

    TobiiResearchExternalSignalChangeType change_type;
} TobiiResearchExternalSignalData;

/**
Defines error types
*/
typedef enum {
    /**
    Indicates that the connection to the eye tracker was lost.
    */
    TOBII_RESEARCH_STREAM_ERROR_CONNECTION_LOST,

    /**
    Indicates that the license is insufficient for subscribing to the stream.
    */
    TOBII_RESEARCH_STREAM_ERROR_INSUFFICIENT_LICENSE,

    /**
    Indicates that the stream isn't supported by the eye tracker.
    */
    TOBII_RESEARCH_STREAM_ERROR_NOT_SUPPORTED,

    /**
    Indicates that number of subscriptions to the stream has reached its limit.
    */
    TOBII_RESEARCH_STREAM_ERROR_TOO_MANY_SUBSCRIBERS,

    /**
    Indicates that an internal error occurred.
    */
    TOBII_RESEARCH_STREAM_ERROR_INTERNAL_ERROR,

    /**
    Indicates that the user threw an exception in the callback.
    */
    TOBII_RESEARCH_STREAM_ERROR_USER_ERROR
} TobiiResearchStreamError;

/**
Defines error sources
*/
typedef enum {
    /**
    User callback failed.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_USER,

    /**
    Error when pumping event.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_STREAM_PUMP,

    /**
    Error when subscribing to event for gaze data.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_GAZE_DATA,

    /**
    Error when subscribing to event for external signal.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_EXTERNAL_SIGNAL,

    /**
    Error when subscribing to event for time synchronization data.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_TIME_SYNCHRONIZATION_DATA,

    /**
    Error when subscribing to event for eye images.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_EYE_IMAGE,

    /**
    Error when subscribing to notification event.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_NOTIFICATION,

    /**
    Error when subscribing to event for hmd gaze data.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_HMD_GAZE_DATA,

    /**
    Error when subscribing to event for user position guide.
    */
    TOBII_RESEARCH_STREAM_ERROR_SOURCE_SUBSCRIPTION_USER_POSITION_GUIDE
} TobiiResearchStreamErrorSource;

/**
Defines notification types
*/
typedef enum {
    /**
    Indicates that the connection to the eye tracker is lost.
    */
    TOBII_RESEARCH_NOTIFICATION_CONNECTION_LOST,

    /**
    Indicates that the connection to the eye tracker is restored.
    */
    TOBII_RESEARCH_NOTIFICATION_CONNECTION_RESTORED,

    /**
    Indicates that the calibration mode is entered.
    */
    TOBII_RESEARCH_NOTIFICATION_CALIBRATION_MODE_ENTERED,

    /**
    Indicates that the calibration mode is left.
    */
    TOBII_RESEARCH_NOTIFICATION_CALIBRATION_MODE_LEFT,

    /**
    Indicates that the calibration is changed.
    */
    TOBII_RESEARCH_NOTIFICATION_CALIBRATION_CHANGED,

    /**
    Indicates that the track box is changed.
    */
    TOBII_RESEARCH_NOTIFICATION_TRACK_BOX_CHANGED,

    /**
    Indicates that the display area is changed.
    */
    TOBII_RESEARCH_NOTIFICATION_DISPLAY_AREA_CHANGED,

    /**
    Indicates that the gaze output frequency is changed.
    */
    TOBII_RESEARCH_NOTIFICATION_GAZE_OUTPUT_FREQUENCY_CHANGED,

    /**
    Indicates that the eye tracking mode is changed.
    */
    TOBII_RESEARCH_NOTIFICATION_EYE_TRACKING_MODE_CHANGED,

    /**
    Indicates that the device has reported new faults.
    */
    TOBII_RESEARCH_NOTIFICATION_DEVICE_FAULTS,

    /**
    Indicates that the device has reported new warnings.
    */
    TOBII_RESEARCH_NOTIFICATION_DEVICE_WARNINGS,

    /**
    */
    TOBII_RESEARCH_NOTIFICATION_UNKNOWN
} TobiiResearchNotificationType;


typedef char TobiiResearchNotificationString[512];

/**
Provides data for the @ref tobii_research_notification_callback callback.
*/
typedef struct {
    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t system_time_stamp;
    /**
    The notification type.
    */
    TobiiResearchNotificationType notification_type;
    union {
        /**
        The new output frequency if notification of type @ref TOBII_RESEARCH_NOTIFICATION_GAZE_OUTPUT_FREQUENCY_CHANGED
        */
        float output_frequency;

        /**
        The new display area if notification of type @ref TOBII_RESEARCH_NOTIFICATION_DISPLAY_AREA_CHANGED
        */
        TobiiResearchDisplayArea display_area;

        /** The new faults if notification of type @ref TOBII_RESEARCH_NOTIFICATION_DEVICE_FAULTS.
        The new warnings if notification of type @ref TOBII_RESEARCH_NOTIFICATION_DEVICE_WARNINGS.
        Contains a comma separated list of warnings or faults.
        */
        TobiiResearchNotificationString text;
    } value;
} TobiiResearchNotification;

/**
@brief Gaze data callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_gaze_data.
@param gaze_data: Gaze data received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_gaze_data.
*/
typedef void(*tobii_research_gaze_data_callback)(TobiiResearchGazeData* gaze_data, void* user_data);

/**
@brief Subscribes to gaze data for the eye tracker.

You will get a callback when time synchronized gaze is received.Time synchronized gaze is not supported on all eye trackers,
other eye trackers need additional license to activate this support.

\snippet gaze_data.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the gaze data.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_gaze_data(
                        TobiiResearchEyeTracker* eyetracker,
                        tobii_research_gaze_data_callback callback, void* user_data);

/**
@brief Unsubscribes from gaze data for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_gaze_data.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_gaze_data(
                        TobiiResearchEyeTracker* eyetracker,
                        tobii_research_gaze_data_callback callback);


/**
@brief User position guide data callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_user_position_guide.
@param gaze_data: Gaze data received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_user_position_guide.
*/
typedef void(*tobii_research_user_position_guide_callback)(TobiiResearchUserPositionGuide* user_position_guide,
    void* user_data);


/**
@brief Subscribes to user position guide for the eye tracker.

\snippet user_position_guide.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the user position guide.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_user_position_guide(
                        TobiiResearchEyeTracker* eyetracker,
                        tobii_research_user_position_guide_callback callback, void* user_data);

/**
@brief Unsubscribes from user position guide for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_user_position_guide.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_user_position_guide(
                        TobiiResearchEyeTracker* eyetracker,
                        tobii_research_user_position_guide_callback callback);

/**
@brief HMD gaze data callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_hmd_gaze_data.
@param hmd_gaze_data: HMD gaze data received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_hmd_gaze_data.
*/
typedef void(*tobii_research_hmd_gaze_data_callback)(TobiiResearchHMDGazeData* hmd_gaze_data, void* user_data);

/**
@brief Subscribes to gaze data for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the gaze data.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_hmd_gaze_data(
        TobiiResearchEyeTracker* eyetracker, tobii_research_hmd_gaze_data_callback callback, void* user_data);

/**
@brief Unsubscribes from HMD gaze data for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_hmd_gaze_data.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_hmd_gaze_data(
        TobiiResearchEyeTracker* eyetracker, tobii_research_hmd_gaze_data_callback callback);

/**
@brief External signal callback.

Implement this and send as a parameter to #tobii_research_subscribe_to_external_signal_data.
@param external_signal_data: External signal data received from the eye tracker.
@param user_data: Caller specific data sent in with #tobii_research_subscribe_to_external_signal_data.
*/
typedef void(*tobii_research_external_signal_data_callback)(TobiiResearchExternalSignalData* external_signal_data,
        void* user_data);

/**
@brief Subscribes to external signal data for the eye tracker.

You will get a callback when the value of the external signal port (TTL input) on the eye tracker device changes. Not
all eye trackers have an output trigger port. The output feature could be used to synchronize the eye tracker data
with data from other devices. The output data contains a time reference that matches the time reference on the time
synchronized gaze data.

\snippet external_signal.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the external signal data.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_external_signal_data(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_external_signal_data_callback callback, void* user_data);

/**
@brief Unsubscribes from external signal data for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_external_signal_data
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_external_signal_data(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_external_signal_data_callback callback);

/**
@brief Time synchronization callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_time_synchronization_data.
@param gaze_data: Time synchronization data received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_time_synchronization_data.
*/
typedef void(*tobii_research_time_synchronization_data_callback)(
        TobiiResearchTimeSynchronizationData* time_synchronization_data,
        void* user_data);

/**
@brief Subscribes to time synchronization data for the eye tracker.

You will get a callback when the computer and the eye trackers clocks gets synchronized. To handle normal drifts
between clocks the clocks are checked on regular basis, and this results in that the time stamps are adjusted for the
drifts in the data streams. This drift handling is done in real time. The data received from this event could be used
for an even more accurate drift adjustment in the post processing.

\snippet time_synchronization_data.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the time synchronization data.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_time_synchronization_data(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_time_synchronization_data_callback callback,
        void* user_data);

/**
@brief Unsubscribes from time synchronization data for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_time_synchronization_data.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_time_synchronization_data(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_time_synchronization_data_callback callback);


/**
Provides properties for the stream error data.
*/
typedef struct {
    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t system_time_stamp;
    /**
    Type of error.
    */
    TobiiResearchStreamError error;
    /**
    Source of the error.
    */
    TobiiResearchStreamErrorSource source;
    /**
    The error message.
    */
    const char* message;
} TobiiResearchStreamErrorData;

/**
@brief Stream error callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_stream_errors.
@param stream_error: @ref TobiiResearchStreamErrorData object.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_stream_errors.
*/
typedef void(*tobii_research_stream_error_callback)(
    TobiiResearchStreamErrorData* stream_error,
    void* user_data);

/**
@brief Subscribes to stream errors for the eye tracker.

You will get a callback when an error occurs on other streams. You can get errors when subscribing, when something
happened to the connection in the stream pump or when an error was raised in a callback.

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the stream errors.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_stream_errors(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_stream_error_callback callback,
        void* user_data);

/**
@brief Unsubscribes from stream errors for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_stream_errors.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_stream_errors(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_stream_error_callback callback);

/**
@brief Notification callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_notifications.
@param notification: Notification received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_notifications.
*/
typedef void(*tobii_research_notification_callback)(
        TobiiResearchNotification* notification,
        void* user_data);

/**
@brief Subscribes to notifications for the eye tracker.

You will get a callback when notification is received.

\snippet notifications.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the notifications.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_notifications(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_notification_callback callback,
        void* user_data);

/**
@brief Unsubscribes from notifications for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_notifications.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_notifications(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_notification_callback callback);

/**
Defines eye image type.
*/
typedef enum {
    /**
    Indicates that the eye tracker could not identify the eyes, and the image is the full image.
    */
    TOBII_RESEARCH_EYE_IMAGE_TYPE_FULL,
    /**
    Indicates that the image is cropped and shows the eyes.
    */
    TOBII_RESEARCH_EYE_IMAGE_TYPE_CROPPED,
    /**
    The eye image is part of a group of regions of interest.
    */
    TOBII_RESEARCH_EYE_IMAGE_TYPE_MULTI_ROI,
    /**
    The eye image has an unexpected type.
    */
    TOBII_RESEARCH_EYE_IMAGE_TYPE_UNKNOWN
} TobiiResearchEyeImageType;

/**
Provides data for the @ref tobii_research_eye_image_callback callback.
*/
typedef struct {
    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t device_time_stamp;
    /**
    The time stamp according to the computer's internal clock.
    */
    int64_t system_time_stamp;
    /**
    The bits per pixel for the eye image.
    */
    int bits_per_pixel;
    /**
    The padding bits per pixel for the eye image.
    */
    int padding_per_pixel;
    /**
    The width in pixel for the eye image.
    */
    int width;
    /**
    The height in pixels for the eye image.
    */
    int height;
    /**
    The type of eye image.
    */
    TobiiResearchEyeImageType type;
    /**
    The source/which camera that generated the image.
    */
    int camera_id;
    /**
    Size in bytes of the data blob.
    */
    size_t data_size;
    /**
    The data blob sent by the eye tracker.
    */
    void* data;
    /**
    The region id for the eye image.
    */
    int region_id;
    /**
    The top position in pixels for the eye image.
    */
    int top;
    /**
    The left position in pixels for the eye image.
    */
    int left;
} TobiiResearchEyeImage;

/**
Provides data for the @ref tobii_research_eye_image_as_gif_callback callback.
*/
typedef struct {
    /**
    The time stamp according to the eye tracker's internal clock.
    */
    int64_t device_time_stamp;
    /**
    The time stamp according to the computer's internal clock.
    */
    int64_t system_time_stamp;
    /**
    The type of eye image.
    */
    TobiiResearchEyeImageType type;
    /**
    The source/which camera that generated the image.
    */
    int camera_id;
    /**
    Size in bytes of the image data.
    */
    size_t image_size;
    /**
    The GIF image data.
    */
    void* image_data;
    /**
    The region id for the eye image.
    */
    int region_id;
    /**
    The top position in pixels for the eye image.
    */
    int top;
    /**
    The left position in pixels for the eye image.
    */
    int left;
} TobiiResearchEyeImageGif;

/**
@brief Eye image callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_eye_image.
@param frame: Eye image frame received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_eye_image.
*/
typedef void(*tobii_research_eye_image_callback)(TobiiResearchEyeImage* frame, void* user_data);

/**
@brief Eye image gif callback.

Implement this and send as a parameter to @ref tobii_research_subscribe_to_eye_image_as_gif.
@param frame: Eye image gif frame received from the eye tracker.
@param user_data: Caller specific data sent in with @ref tobii_research_subscribe_to_eye_image_as_gif.
*/
typedef void(*tobii_research_eye_image_as_gif_callback)(TobiiResearchEyeImageGif* frame, void* user_data);

/**
@brief  Subscribes to eye image for the eye tracker.

You will get a callback when a new eye image is received, and the occurrence depends on the eye tracker model. Not all
eye tracker models support this feature. If no one is listening to gaze data, the eye tracker will only deliver full
images, otherwise either cropped or full images will be delivered depending on whether or not the eye tracker has
detected eyes.

\snippet eye_images.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the frames.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_eye_image(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_eye_image_callback callback,
        void* user_data);

/**
@brief  Subscribes to eye image for the eye tracker with the image delivered in gif format.

You will get a callback when a new eye image is received, and the occurrence depends on the eye tracker model. Not all
eye tracker models support this feature. If no one is listening to gaze data, the eye tracker will only deliver full
images, otherwise either cropped or full images will be delivered depending on whether or not the eye tracker has
detected eyes.

\snippet eye_images_as_gif.c Example

@param eyetracker: Eye tracker object.
@param callback: Callback that will receive the frames.
@param user_data: Caller specific data that will be sent to the callback.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_subscribe_to_eye_image_as_gif(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_eye_image_as_gif_callback callback,
        void* user_data);

/**
@brief Unsubscribes from eye image for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_eye_image.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_eye_image(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_eye_image_callback callback);

/**
@brief Unsubscribes from eye image for the eye tracker.

@param eyetracker: Eye tracker object.
@param callback: Callback sent to @ref tobii_research_subscribe_to_eye_image_as_gif.
@returns A @ref TobiiResearchStatus code.
*/
TOBII_RESEARCH_API TobiiResearchStatus TOBII_RESEARCH_CALL tobii_research_unsubscribe_from_eye_image_as_gif(
        TobiiResearchEyeTracker* eyetracker,
        tobii_research_eye_image_as_gif_callback callback);

						
#ifdef __cplusplus
}
#endif 
#endif /* TOBII_RESEARCH_H_ */
