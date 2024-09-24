/////////////////////////////////////////////////////////////////////////////
// $Id: EyetrackerLoggerTobiiPro.cpp 7993 2024-04-04 20:44:05Z mellinger $
// Authors: markus.adamek@gmail.com, mellinger@neurotechcenter.org
// Description: The Eyetracker Logger logs incoming data from a Tobii with TobiiPro SDK
// Eyetracker and changes states
//
// Parameterization:
//   Eyetracker logging is enabled from the command line adding
//     --EyetrackerLoggerTobiiPro=1
//   As a command line option.
//   Information may be selectively logged by setting these parameters to true
//     LogGazeData - record gaze data to states
//     LogEyePosition - record eye position to states
//     LogPupilSize - record pupil size to states
//     LogEyeDist - record eye distances to states
//
// State Variables:
//   EyetrackerLeftEyeGazeX   - Left Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyeGazeY   - Left Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyeGazeX  - Right Eye X Gaze Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyeGazeY  - Right Eye Y Gaze Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftEyePosX    - Left Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerLeftEyePosY    - Left Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerRightEyePosX   - Right Eye X Position (0 = leftmost, 65535 = rightmost)
//   EyetrackerRightEyePosY   - Right Eye Y Position (0 = topmost, 65535 = bottommost)
//   EyetrackerLeftPupilSize  - Left Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerRightPupilSize - Right Eye Pupil Size (mm), Depends on eye pos and dist
//   EyetrackerLeftEyeDist    - Left Eye Distance from Screen (mm), Approx
//   EyetrackerRightEyeDist   - Right Eye Distance from Screen (mm), Approx
//   EyetrackerTime     - The timestamp for the gaze data relative to BCI timer
//
// $BEGIN_BCI2000_LICENSE$
// 
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
// 
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// $END_BCI2000_LICENSE$
/////////////////////////////////////////////////////////////////////////////
#include "EyetrackerLoggerTobiiPro.h"
#include "BCIEvent.h"
#include "VideoRecorder.h"
#include "FrameComposer.h"
#include "FileUtils.h"
#include "RunManager.h"

#define EVENTS \
  _(Time, 16, uint, T) \
  //

#define PER_EYE_EVENTS \
  _(EyeValidity, 3, uint,) \
  _(PupilSize, 32, uint, Pupil) \
  _(EyeGazeX, 32, int, X) _(EyeGazeY, 32, int, Y) \
  _(EyePosX, 32, int, X) _(EyePosY, 32, int, Y) _(EyeDist, 32, int, Z) \
  //

Extension( EyetrackerLoggerTobiiPro );

namespace {
  static const double eps = std::numeric_limits<double>::epsilon();

  // currently, no vector algebra available in BCI2000 framework
  template<class T> double sq(const T& t) { return t*t; }
  struct vector3d { double dx, dy, dz; };

  vector3d operator-(const vector3d& p1, const vector3d& p2)
  { vector3d v = { p1.dx - p2.dx, p1.dy - p2.dy, p1.dz - p2.dz }; return v; }
  vector3d operator-(const TobiiResearchPoint3D& p1, const vector3d& p2)
  { vector3d v = { p1.x - p2.dx, p1.y - p2.dy, p1.z - p2.dz }; return v; }

  vector3d operator-(const TobiiResearchPoint3D& p1, const TobiiResearchPoint3D& p2)
  { vector3d v = { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z }; return v; }
  double operator*(const vector3d& v1, const vector3d& v2)
  { return v1.dx*v2.dx + v1.dy*v2.dy + v1.dz*v2.dz; }
  vector3d operator*(const vector3d& v1, double d)
  { vector3d v2 = { v1.dx*d, v1.dy*d, v1.dz*d }; return v2; }
  vector3d crossprod(const vector3d& v1, const vector3d& v2)
  {
    vector3d v = {
      v1.dy*v2.dz - v1.dz*v2.dy,
      v1.dz*v2.dx - v1.dx*v2.dz,
      v1.dx*v2.dy - v1.dy*v2.dx
    };
    return v;
  }
}

#define _(x, y, z, u) static const uint64_t bitmask_##x = (1ULL<<(y))-1;
EVENTS PER_EYE_EVENTS
#undef _

static const double sGazeOffset = 1ull << 15;

static const int sCameraIndex = 4; // WebcamLogger supports up to 4 cameras

enum
{
  leftCamera = 0, rightCamera = 1,
  leftEye = 0, rightEye = 1,
};
static const int sEyetrackerCameraFps = 5;
static const int sFullImageSize = 280;
static const int sEyeImageSize = 170;

struct EyetrackerLoggerTobiiPro::TobiiProScreenCoords
{
  vector3d mScreenXAxis, mScreenYAxis, mScreenZAxis, mScreenOrigin;
  PhysicalUnit mUnitX, mUnitY, mUnitZ, mUnitPupil;
  vector3d tobiiUCSToscreenRelativeCoordinates(const TobiiResearchPoint3D&);
};

vector3d EyetrackerLoggerTobiiPro::TobiiProScreenCoords::tobiiUCSToscreenRelativeCoordinates(const TobiiResearchPoint3D& p)
{
  vector3d s = p - mScreenOrigin;
  vector3d r = { s*mScreenXAxis, s*mScreenYAxis, s*mScreenZAxis };
  return r;
}

EyetrackerLoggerTobiiPro::EyetrackerLoggerTobiiPro()
  : m_eyetrackerEnable( false ),
  m_connectedDevice(nullptr),
  m_deviceList( nullptr),
  mpCoordSystem(new TobiiProScreenCoords),
  m_subscribed(false),
  m_isLogging(false),
  m_firstData(true),
  m_prevGazeData({0, 0}),
  m_sync_calculated(false),
  m_timeOffset(0)
{
    PublishEnabler("LogEyetrackerTobiiPro");
}

EyetrackerLoggerTobiiPro::~EyetrackerLoggerTobiiPro()
{
  Halt();
  delete mpVideoRecorderFull;
  delete mpFrameComposerFull;
  delete mpVideoRecorderRoi;
  delete mpFrameComposerRoi;
  if(m_deviceList != nullptr)
    tobii_research_free_eyetrackers(m_deviceList);
  delete mpCoordSystem;
}

void EyetrackerLoggerTobiiPro::Publish()
{
  bool eyetrackerEnable = false;
  eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiPro" ) != 0 );
  if( !eyetrackerEnable )
    return;

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Eyetracker int /LogEyetrackerTobiiPro= 1 0 0 1"
    " // enable eye tracker logging (boolean)",
    "Source:Eyetracker float GazeScale= 1 1 0 1"
    " // value to scale gaze data by (obsolete)",
    "Source:Eyetracker float GazeOffset= 0 0 & &"
    " // value to offset gaze data after scaled (obsolete)",
    "Source:Eyetracker string GazeResolution= 0.1mm % % % "
    "// eye tracker gaze recording resolution",
    "Source:Eyetracker string PupilResolution= 0.01mm % % % "
    "// eye tracker pupil diameter recording resolution",
    "Source:Eyetracker matrix EyetrackerInfo= 1 1 auto "
    "// eye tracker properties and settings (readonly)(noedit)",
    "Source:Eyetracker matrix EyetrackerData= 1 1 auto "
    "// eye tracker data channel properties (readonly)(noedit)",
    "Source:Eyetracker floatlist EyetrackerScreenSize= 1 auto "
    "// height and width of eyetracker's screen",
    "Source:Eyetracker floatlist EyetrackerLegacyCorrectionOffset= 1 auto "
    "// subtract offset to get gaze/pupil in legacy units (readonly)(noedit)",
    "Source:Eyetracker floatlist EyetrackerLegacyCorrectionFactor= 1 auto "
    "// multiply by respective factor to get gaze/pupil in legacy units (readonly)(noedit)",
    "Source:Eyetracker int LogGazeData= 1 1 auto "
    "// compatibility",
    "Source:Eyetracker int RecordEyeGazeVideo= 0 0 0 1"
    "// record eye gaze video (boolean)",
    "Source:Eyetracker int DisplayEyeGazeVideo= 0 0 0 1"
    "// display eye gaze video in a window (boolean)",
  END_PARAMETER_DEFINITIONS

  BEGIN_EVENT_DEFINITIONS
#define _(x, y, z, u) "Eyetracker" #x " " #y " 0 0 0",
    EVENTS
#undef _
#define _(x, y, z, u) "EyetrackerLeft" #x " " #y " 0 0 0",
    PER_EYE_EVENTS
#undef _
#define _(x, y, z, u) "EyetrackerRight" #x " " #y " 0 0 0",
    PER_EYE_EVENTS
#undef _
  END_EVENT_DEFINITIONS

  for (int i = sCameraIndex; i < sCameraIndex + 2; ++i)
  {
      std::ostringstream oss;
      oss << "WebcamFrame" << i << " 32 0";
      BEGIN_EVENT_DEFINITIONS
        oss.str().c_str(),
      END_EVENT_DEFINITIONS
  }
}

void EyetrackerLoggerTobiiPro::AutoConfig()
{
  m_eyetrackerEnable = ( ( int )OptionalParameter( "LogEyetrackerTobiiPro" ) != 0 );
  if(m_eyetrackerEnable)
  {
    TobiiResearchSDKVersion version;
    if (TOBII_RESEARCH_STATUS_OK == ::tobii_research_get_sdk_version(&version))
    {
      bciout << "Tobii SDK version is "
             << version.major << "." << version.minor << "."
             << version.revision << "." << version.build;

    }

    std::lock_guard<std::mutex> lock(m_Mutex);
    // Initialize parameters
    std::string gazeResolution = Parameter("GazeResolution");
    std::string pupilResolution = Parameter("PupilResolution");

    // Get device and connect
    m_connectedDevice = FindDevice();
    if(m_connectedDevice)
      bciout << "Successfully connected to Tobii EyeLogger";

    storeEyetrackerInformation(m_connectedDevice);
    TobiiResearchDisplayArea display_area;
    if(tobii_research_get_display_area(m_connectedDevice,&display_area) != TOBII_RESEARCH_STATUS_OK)
      throw bcierr<< "EyetrackerLoggerTobiiPro Error: Failed to get screen coordinates";

    auto xAxis = display_area.top_right - display_area.top_left,
      yAxis = display_area.bottom_left - display_area.top_left,
      zAxis = crossprod(yAxis, xAxis);

    auto xLen = ::sqrt(sq(xAxis)), yLen = ::sqrt(sq(yAxis)), zLen = ::sqrt(sq(zAxis));
    if(xLen < eps || yLen < eps || zLen < eps)
      throw bcierr << "EyetrackerLoggerTobiiPro Error: Invalid screen coordinates";

    mpCoordSystem->mScreenOrigin.dx = display_area.top_left.x;
    mpCoordSystem->mScreenOrigin.dy = display_area.top_left.y;
    mpCoordSystem->mScreenOrigin.dz = display_area.top_left.z;

    mpCoordSystem->mScreenXAxis = xAxis * (1e-3/xLen); // Tobii values are in mm
    mpCoordSystem->mScreenYAxis = yAxis * (1e-3/yLen);
    mpCoordSystem->mScreenZAxis = zAxis * (1e-3/zLen);

    const double xyzRange = 1ull << 32;

    mpCoordSystem->mUnitX.SetGainWithSymbol(gazeResolution).SetOffset(sGazeOffset);
    mpCoordSystem->mUnitX.SetRawMin(0).SetPhysicalMax("4m");
    if (mpCoordSystem->mUnitX.RawMax() >= xyzRange)
        bcierr << "X coordinate range exceeds state value range";

    mpCoordSystem->mUnitY.SetGainWithSymbol(gazeResolution).SetOffset(sGazeOffset);
    mpCoordSystem->mUnitY.SetRawMin(0).SetPhysicalMax("4m");
    if (mpCoordSystem->mUnitY.RawMax() >= xyzRange)
        bcierr << "Y coordinate range exceeds state value range";

    mpCoordSystem->mUnitZ.SetGainWithSymbol(gazeResolution).SetOffset(sGazeOffset);
    mpCoordSystem->mUnitZ.SetRawMin(0).SetPhysicalMax("3m");
    if (mpCoordSystem->mUnitZ.RawMax() >= xyzRange)
        bcierr << "Z coordinate range exceeds state value range";

    double pupilRange = 1ull << 32;
    mpCoordSystem->mUnitPupil.SetGainWithSymbol(pupilResolution);
    mpCoordSystem->mUnitPupil.SetRawMin(0).SetPhysicalMax("0.1m");
    if (mpCoordSystem->mUnitPupil.RawMax() >= pupilRange)
        bcierr << "Pupil size range exceeds state value range";

    PhysicalUnit timeMs;
    timeMs.SetGainWithSymbol("1ms");
    PhysicalUnit none;

    const PhysicalUnit
      *puX = &mpCoordSystem->mUnitX, *puY = &mpCoordSystem->mUnitY, *puZ = &mpCoordSystem->mUnitZ,
      *puPupil = &mpCoordSystem->mUnitPupil, *puT = &timeMs, *pu = &none;
    struct { const char* name, *format; const PhysicalUnit* unit; }
    events[] =
    {
#define _(x, y, z, u) { "Eyetracker" #x, #z #y, pu##u },
      EVENTS
#undef _
#define _(x, y, z, u) { "EyetrackerLeft" #x, #z #y, pu##u },
      PER_EYE_EVENTS
#undef _
#define _(x, y, z, u) { "EyetrackerRight" #x, #z #y, pu##u },
      PER_EYE_EVENTS
#undef _
    };
    auto EyetrackerData = Parameter("EyetrackerData");
    EyetrackerData->SetDimensions(sizeof(events)/sizeof(*events), 2);
    EyetrackerData->ColumnLabels()[0] = "type";
    EyetrackerData->ColumnLabels()[1] = "unit";
    int row = 0;
    for(const auto& s : events)
    {
      EyetrackerData->RowLabels()[row] = s.name;
      EyetrackerData(row, 0) = s.format;
      EyetrackerData(row, 1) << *s.unit;
      ++row;
    }

    auto EyetrackerScreenSize = Parameter("EyetrackerScreenSize");
    EyetrackerScreenSize->SetNumValues(2);
    EyetrackerScreenSize->Labels()[0] = "width";
    EyetrackerScreenSize->Labels()[1] = "height";
    PhysicalUnit screenUnit;
    screenUnit.SetGainWithSymbol("1mm");
    EyetrackerScreenSize("width") = screenUnit.RawToPhysical(xLen);
    EyetrackerScreenSize("height") = screenUnit.RawToPhysical(yLen);

    const double legacy_xyRange = 1ull << 16;

    auto LegacyCorrectionFactor = Parameter("EyetrackerLegacyCorrectionFactor"),
         LegacyCorrectionOffset = Parameter("EyetrackerLegacyCorrectionOffset");
    LegacyCorrectionFactor->SetNumValues(4);
    LegacyCorrectionOffset->SetNumValues(4);
    int idx = 0;
    for (const char* label : { "x", "y", "z", "pupil" }) {
        LegacyCorrectionFactor->Labels()[idx] = label;
        LegacyCorrectionOffset->Labels()[idx] = label;
        idx++;
    }

    LegacyCorrectionFactor("x") = mpCoordSystem->mUnitX.Gain() / (xLen * 1e-3 / legacy_xyRange);
    LegacyCorrectionOffset("x") = mpCoordSystem->mUnitX.Offset();
    LegacyCorrectionFactor("y") = mpCoordSystem->mUnitY.Gain() / (yLen * 1e-3 / legacy_xyRange);
    LegacyCorrectionOffset("y") = mpCoordSystem->mUnitY.Offset();
    LegacyCorrectionFactor("z") = mpCoordSystem->mUnitZ.Gain() / 1e-3;
    LegacyCorrectionOffset("z") = mpCoordSystem->mUnitZ.Offset();
    LegacyCorrectionFactor("pupil") = mpCoordSystem->mUnitPupil.Gain() / 1e-4;
    LegacyCorrectionOffset("pupil") = mpCoordSystem->mUnitPupil.Offset();
  }
}


void EyetrackerLoggerTobiiPro::Preflight() const
{
    bool eyetrackerEnable = false;
    float gazeScale = 0.0f;
    float gazeOffset = 0.0f;

    eyetrackerEnable = ((int)OptionalParameter("LogEyetrackerTobiiPro") != 0);

    if (eyetrackerEnable)
    {
        gazeScale = Parameter("GazeScale");
        gazeOffset = Parameter("GazeOffset");
        if (gazeScale != 1 || gazeOffset != 0)
            bcierr << "Parameters GazeScale and GazeOffset are obsolete, must be 1 and 0";
        State("SourceTime");
        Parameter("EyetrackerData");
        Parameter("EyetrackerInfo");
    }

    State("FilePart");
}

void EyetrackerLoggerTobiiPro::Initialize()
{
    if (!m_eyetrackerEnable)
        return;

    ::tobii_research_unsubscribe_from_eye_image(m_connectedDevice, &eye_image_callback);
    delete mpVideoRecorderFull;
    mpVideoRecorderFull = nullptr;
    delete mpFrameComposerFull;
    mpFrameComposerFull = nullptr;
    delete mpVideoRecorderRoi;
    mpVideoRecorderRoi = nullptr;
    delete mpFrameComposerRoi;
    mpFrameComposerRoi = nullptr;
    mDisplayEyeGazeVideo = Parameter("DisplayEyeGazeVideo");
    mRecordEyeGazeVideo = Parameter("RecordEyeGazeVideo");
    if (mDisplayEyeGazeVideo || mRecordEyeGazeVideo)
    {
      mpVideoRecorderFull = new VideoRecorder;
      mpVideoRecorderFull->setFramesPerSecond(sEyetrackerCameraFps)
                          .setId(0)
                          .setName("Eyetracker Cameras Full Image")
                          .setHeight(sFullImageSize)
                          .setWidth(2*sFullImageSize);
      mpFrameComposerFull = new FrameComposer;
      mpFrameComposerFull->setHeight(mpVideoRecorderFull->height())
                          .setWidth(mpVideoRecorderFull->width())
                          .setRows(1).setCols(2);
      mpVideoRecorderRoi = new VideoRecorder;
      mpVideoRecorderRoi->setFramesPerSecond(sEyetrackerCameraFps)
                          .setName("Eyetracker Cameras ROIs")
                          .setId(1)
                          .setHeight(2*sEyeImageSize)
                          .setWidth(2*sEyeImageSize);
      mpFrameComposerRoi = new FrameComposer;
      mpFrameComposerRoi->setHeight(mpVideoRecorderRoi->height())
                          .setWidth(mpVideoRecorderRoi->width())
                          .setRows(2).setCols(2);
      TobiiResearchStatus s = ::tobii_research_subscribe_to_eye_image(m_connectedDevice, &eye_image_callback, this);
      if (s != TOBII_RESEARCH_STATUS_OK)
      {
        bcierr << "Problem during start of eye image recording";
      }
      else if (mDisplayEyeGazeVideo)
      {
        mpVideoRecorderFull->showWindow();
        mpVideoRecorderRoi->showWindow();
      }
    }
    m_isLogging = false;
    RegisterGazeCallbacks();
}

void EyetrackerLoggerTobiiPro::StartRun()
{
  if(m_eyetrackerEnable) {
    std::lock_guard<std::mutex> lock(m_Mutex);

    m_sync_calculated=false;
    m_firstData=true;
    m_isLogging=true;

    if (mRecordEyeGazeVideo)
    {
      mFileBase = FileUtils::StripExtension(RunManager()->CurrentRun());
      mFilePart = 1;

      mpVideoRecorderFull->startRecording(ConstructVideoFileName(sCameraIndex));
      if (!mpVideoRecorderFull->error().empty())
        bcierr << mpVideoRecorderFull->error();

      mpVideoRecorderRoi->startRecording(ConstructVideoFileName(sCameraIndex + 1));
      if (!mpVideoRecorderRoi->error().empty())
        bcierr << mpVideoRecorderRoi->error();
    }
  }
}

void EyetrackerLoggerTobiiPro::Process()
{
  if (m_eyetrackerEnable && mRecordEyeGazeVideo)
  {
    int filePart = State("FilePart")(Statevector->Samples() - 1);
    if (filePart > mFilePart)
    {
      mFilePart = filePart;
      if (filePart > 1)
      {
        // make sure no images are acquired while recording is suspended
        std::lock_guard<std::mutex> lock(mEyeImageMutex);

        mpVideoRecorderFull->stopRecording();
        mpVideoRecorderFull->startRecording(ConstructVideoFileName(sCameraIndex));
        if (!mpVideoRecorderFull->error().empty())
          bcierr << mpVideoRecorderFull->error();

        mpVideoRecorderRoi->stopRecording();
        mpVideoRecorderRoi->startRecording(ConstructVideoFileName(sCameraIndex + 1));
        if (!mpVideoRecorderRoi->error().empty())
          bcierr << mpVideoRecorderRoi->error();
      }
    }
  }
}

void EyetrackerLoggerTobiiPro::StopRun()
{
  if (m_eyetrackerEnable)
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_isLogging = false;
    if (mRecordEyeGazeVideo)
    {
      Assert(mpVideoRecorderFull);
      mpVideoRecorderFull->stopRecording();
      Assert(mpVideoRecorderRoi);
      mpVideoRecorderRoi->stopRecording();
    }
    mFilePart = 0;
  }
}

void EyetrackerLoggerTobiiPro::Halt()
{
  {
    std::lock_guard<std::mutex> lock(m_Mutex);
    ::tobii_research_unsubscribe_from_eye_image(m_connectedDevice, &eye_image_callback);
    UnregisterGazeCallbacks();
  }
  StopRun();
}

//static method used for subscription -> reroutes to object
//user data must be EyetrackerLogger instance
void EyetrackerLoggerTobiiPro::gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data)
{
  EyetrackerLoggerTobiiPro* pObject = static_cast<EyetrackerLoggerTobiiPro*>(user_data);
  pObject->SetGazeData(*gaze_data);
}

// dito
void EyetrackerLoggerTobiiPro::eye_image_callback(TobiiResearchEyeImage* frame, void* user_data)
{
  EyetrackerLoggerTobiiPro* pObject = static_cast<EyetrackerLoggerTobiiPro*>(user_data);
  pObject->ViewEyeImageData(frame);
}

void EyetrackerLoggerTobiiPro::SetGazeData(TobiiResearchGazeData const  &gaze)
{
  std::lock_guard<std::mutex> lock(m_Mutex);
  if (!m_isLogging)
    return;

  uint64_t timestamp = 0;
  // Write states
  if(!m_sync_calculated && Statevector)
  {
    m_timeOffset=gaze.system_time_stamp/1000 - (uint64_t)State("SourceTime");
    m_sync_calculated=true;
  }
  if (m_sync_calculated)
  {
    timestamp = gaze.system_time_stamp / 1000 - m_timeOffset;
    bcievent << "EyetrackerTime " << (timestamp & 65535) << std::flush;
  }

  logEyeData(gaze.left_eye,m_prevGazeData.left_eye,"Left");
  logEyeData(gaze.right_eye,m_prevGazeData.right_eye,"Right");
  m_firstData = false;

  // push back data
  m_prevGazeData = gaze;
}

bool EyetrackerLoggerTobiiPro::logEyeData(const TobiiResearchEyeData  &data,const TobiiResearchEyeData  &prev_data,const char* eye)
{
  std::string prefix="Eyetracker";
  prefix+=eye;
  uint8_t valid=0;
  if(data.gaze_point.validity != TOBII_RESEARCH_VALIDITY_VALID)
  {
    valid=4u;
  }
  bcievent << prefix <<"EyeValidity " << (valid & bitmask_EyeValidity) << std::flush;

  bool reportGaze = m_firstData || sq(data.gaze_point.position_in_user_coordinates - prev_data.gaze_point.position_in_user_coordinates) > eps;
  reportGaze = reportGaze && (data.gaze_point.validity == TOBII_RESEARCH_VALIDITY_VALID);

  bool reportEyePos = m_firstData || sq(data.gaze_origin.position_in_user_coordinates - prev_data.gaze_origin.position_in_user_coordinates) > eps;
  reportEyePos = reportEyePos && (data.gaze_origin.validity == TOBII_RESEARCH_VALIDITY_VALID);

  bool reportPupilSize = m_firstData || sq(data.pupil_data.diameter - prev_data.pupil_data.diameter) > eps;
  reportPupilSize = reportPupilSize && (data.pupil_data.validity == TOBII_RESEARCH_VALIDITY_VALID);

  if(reportGaze ) 
  {
    auto gazePoint=mpCoordSystem->tobiiUCSToscreenRelativeCoordinates(data.gaze_point.position_in_user_coordinates);
    int32_t eyeGazeX = mpCoordSystem->mUnitX.PhysicalToRawValue(gazePoint.dx),
            eyeGazeY = mpCoordSystem->mUnitY.PhysicalToRawValue(gazePoint.dy);
    bcievent << prefix << "EyeGazeX " << (eyeGazeX & bitmask_EyeGazeX) << std::flush;
    bcievent << prefix << "EyeGazeY " << (eyeGazeY & bitmask_EyeGazeY) << std::flush;

    Assert(gazePoint.dz*gazePoint.dz < 1e-6); // always zero in screen relative system
  }

  if (reportEyePos)
  {
    auto eyePos = mpCoordSystem->tobiiUCSToscreenRelativeCoordinates(data.gaze_origin.position_in_user_coordinates);
    int32_t eyePosX = mpCoordSystem->mUnitX.PhysicalToRawValue(eyePos.dx),
            eyePosY = mpCoordSystem->mUnitY.PhysicalToRawValue(eyePos.dy),
            eyePosZ = mpCoordSystem->mUnitZ.PhysicalToRawValue(eyePos.dz);
    bcievent << prefix << "EyePosX " << (eyePosX & bitmask_EyePosX) << std::flush;
    bcievent << prefix << "EyePosY " << (eyePosY & bitmask_EyePosY) << std::flush;
    bcievent << prefix << "EyeDist " << (eyePosZ & bitmask_EyeDist) << std::flush;
  }

  if(reportPupilSize)
  {
    int32_t pupilSize = mpCoordSystem->mUnitPupil.PhysicalToRawValue(data.pupil_data.diameter*1e-3);
    bcievent << prefix << "PupilSize " << (pupilSize & bitmask_PupilSize) << std::flush;
  }

  return true;
}

TobiiResearchEyeTracker* EyetrackerLoggerTobiiPro::FindDevice()
{
  if(m_deviceList != NULL) //free old list
  {
    tobii_research_free_eyetrackers(m_deviceList);
    m_deviceList=NULL;
  }
  TobiiResearchEyeTrackers* _trackers = NULL;
  TobiiResearchStatus status;
  if((status = tobii_research_find_all_eyetrackers(&_trackers)) != TOBII_RESEARCH_STATUS_OK)
  {
    throw bcierr << "EyetrackerLoggerTobiiPro Error: Problem during device search: " << status;
  }

  if(_trackers->count < 1)
  {
    throw bcierr << "EyetrackerLoggerTobiiPro Error: No device(s) found";
  }


  m_deviceList=_trackers;
  return _trackers->eyetrackers[0]; //always use first tracker
}


void EyetrackerLoggerTobiiPro::storeEyetrackerInformation(TobiiResearchEyeTracker* tracker)
{
  std::vector<std::pair<std::string, std::string>> info;
  char *serialNumber, *model, *firmwareVersion, *unitName, *illuminationMode;

  if(tobii_research_get_serial_number(tracker,&serialNumber)== TOBII_RESEARCH_STATUS_OK)
  {
    std::string s_serialNumber(serialNumber);
    info.push_back(std::make_pair("SerialNumber", s_serialNumber));
    tobii_research_free_string(serialNumber);
  }

  if(tobii_research_get_model(tracker,&model)== TOBII_RESEARCH_STATUS_OK)
  {
    std::string s_model(model);
    info.push_back(std::make_pair("Model", s_model));
    tobii_research_free_string(model);
  }

  if(tobii_research_get_firmware_version(tracker,&firmwareVersion)== TOBII_RESEARCH_STATUS_OK)
  {
    std::string s_firmwareVersion(firmwareVersion);
    info.push_back(std::make_pair("FirmwareVersion", s_firmwareVersion));
    tobii_research_free_string(firmwareVersion);
  }

  if(tobii_research_get_device_name(tracker,&unitName)== TOBII_RESEARCH_STATUS_OK)
  {
    std::string s_unitName(firmwareVersion);
    info.push_back(std::make_pair("UnitName", s_unitName));
    tobii_research_free_string(unitName);
  }

  float f;

  if(tobii_research_get_gaze_output_frequency(tracker,&f)== TOBII_RESEARCH_STATUS_OK)
  {
    info.push_back(std::make_pair("FrameRate", StringUtils::String() << f));
  }

  if(tobii_research_get_eye_tracking_mode(tracker,&illuminationMode)== TOBII_RESEARCH_STATUS_OK)
  {
    std::string s_illuminationMode(illuminationMode);
    info.push_back(std::make_pair("EyeTrackingMode", s_illuminationMode));
  }
  std::string s;
  for(const auto& entry : info)
    if(!entry.first.empty())
      s += "\t" + entry.first + "\t" + entry.second + "\n";
  bciout << s;

  auto EyetrackerInfo = Parameter("EyetrackerInfo");
  EyetrackerInfo->SetNumColumns(1);
  EyetrackerInfo->SetNumRows(info.size());
  for(int row = 0; row < info.size(); ++row)
  {
    EyetrackerInfo->RowLabels()[row] = info[row].first;
    EyetrackerInfo(row) = info[row].second;
  }
}

void EyetrackerLoggerTobiiPro::ViewEyeImageData(const TobiiResearchEyeImage* pFrame)
{
  uint8_t* pData = static_cast<uint8_t*>(pFrame->data);
  uint8_t* pMemory = nullptr;
  if (pFrame->bits_per_pixel == 8 && pFrame->padding_per_pixel == 0)
  {
    // Copy grayscale data into a full BGRA image.
    pMemory = new uint8_t[pFrame->width * pFrame->height * sizeof(RGBQUAD)];
    pData = pMemory;
    uint8_t* p = pData, *pGrayData = static_cast<uint8_t*>(pFrame->data);
    for (int i = 0; i < pFrame->data_size; ++i)
    {
      *p++ = pGrayData[i];
      *p++ = pGrayData[i];
      *p++ = pGrayData[i];
      *p++ = 0;
    }
  }
  else if (pFrame->bits_per_pixel + pFrame->padding_per_pixel != 32)
  {
    bcierr << "Cannot handle video format";
    return;
  }

  if (pFrame->type == TOBII_RESEARCH_EYE_IMAGE_TYPE_FULL)
  {
    if (pFrame->camera_id == 0 || pFrame->camera_id == 1)
    {
      mpFrameComposerFull->putSubImage(0, pFrame->camera_id, pFrame->width, pFrame->height, pData);
      if (!mpFrameComposerFull->error().empty())
        bcierr << mpFrameComposerFull->error();
      if (mpFrameComposerFull->imageDone())
      {
        uint64_t count = 0;
        VideoRecorder::Presentation presentation = { 0 };
        {
          std::lock_guard<std::mutex> lock(mEyeImageMutex);
          presentation.timeMicroseconds = pFrame->device_time_stamp;
          if (mFullFrameData.prevts < 0)
            presentation.durationMicroseconds = 1e6 / sEyetrackerCameraFps;
          else
            presentation.durationMicroseconds = pFrame->device_time_stamp - mFullFrameData.prevts;
          mFullFrameData.prevts = pFrame->device_time_stamp;
          count = mFullFrameData.count++;
        }
        mpVideoRecorderFull->setBlank(&presentation, false);
        mpVideoRecorderRoi->setBlank(&presentation, true);
        if (mpVideoRecorderFull->isRecording())
          bcievent << "WebcamFrame" << sCameraIndex << " " << count;
        mpVideoRecorderFull->writeFrame(&presentation, mpFrameComposerFull->imageData());
        if (!mpVideoRecorderFull->error().empty())
          bcierr << mpVideoRecorderFull->error();
        mpFrameComposerFull->clear();
      }
    }
  }
  else if (pFrame->type == TOBII_RESEARCH_EYE_IMAGE_TYPE_CROPPED)
  {
    if (pFrame->camera_id == 0 || pFrame->camera_id == 1)
    {
      mpFrameComposerRoi->putSubImage(pFrame->camera_id, 0, 1, 2, pFrame->width, pFrame->height, pData);
      if (!mpFrameComposerRoi->error().empty())
        bcierr << mpFrameComposerRoi->error();
      if (mpFrameComposerRoi->imageDone())
      {
        int64_t count = 0;
        VideoRecorder::Presentation presentation = { 0 };
        {
          std::lock_guard<std::mutex> lock(mEyeImageMutex);
          presentation.timeMicroseconds = pFrame->device_time_stamp;
          if (mRoiFrameData.prevts < 0)
            presentation.durationMicroseconds = 1e6 / sEyetrackerCameraFps;
          else
            presentation.durationMicroseconds = pFrame->device_time_stamp - mRoiFrameData.prevts;
          mRoiFrameData.prevts = pFrame->device_time_stamp;
          count = mRoiFrameData.count++;
        }
        mpVideoRecorderRoi->setBlank(&presentation, false);
        mpVideoRecorderFull->setBlank(&presentation, true);
        if (mpVideoRecorderRoi->isRecording())
          bcievent << "WebcamFrame" << sCameraIndex + 1 << " " << count;
        mpVideoRecorderRoi->writeFrame(&presentation, mpFrameComposerRoi->imageData());
        if (!mpVideoRecorderRoi->error().empty())
          bcierr << mpVideoRecorderRoi->error();
        mpFrameComposerRoi->clear();
      }
    }
  }
  else if (pFrame->type == TOBII_RESEARCH_EYE_IMAGE_TYPE_MULTI_ROI)
  {
      if (pFrame->camera_id == 0 || pFrame->camera_id == 1)
      {
          mpFrameComposerRoi->putSubImage(pFrame->camera_id, pFrame->region_id, pFrame->width, pFrame->height, pData);
          if (!mpFrameComposerRoi->error().empty())
              bcierr << mpFrameComposerRoi->error();
          if (mpFrameComposerRoi->imageDone())
          {
              int64_t count = 0;
              VideoRecorder::Presentation presentation = { 0 };
              {
                  std::lock_guard<std::mutex> lock(mEyeImageMutex);
                  presentation.timeMicroseconds = pFrame->device_time_stamp;
                  if (mRoiFrameData.prevts < 0)
                      presentation.durationMicroseconds = 1e6 / sEyetrackerCameraFps;
                  else
                      presentation.durationMicroseconds = pFrame->device_time_stamp - mRoiFrameData.prevts;
                  mRoiFrameData.prevts = pFrame->device_time_stamp;
                  count = mRoiFrameData.count++;
              }
              mpVideoRecorderRoi->setBlank(&presentation, false);
              mpVideoRecorderFull->setBlank(&presentation, true);
              if (mpVideoRecorderRoi->isRecording())
                  bcievent << "WebcamFrame" << sCameraIndex + 1 << " " << count;
              mpVideoRecorderRoi->writeFrame(&presentation, mpFrameComposerRoi->imageData());
              if (!mpVideoRecorderRoi->error().empty())
                  bcierr << mpVideoRecorderRoi->error();
              mpFrameComposerRoi->clear();
          }
      }
  }
#if BCIDEBUG
  else
  {
    bciwarn << "Unexpected frame type: " << pFrame->type;
  }
#endif
  delete[] pMemory;
}

std::string EyetrackerLoggerTobiiPro::ConstructVideoFileName(int cameraIndex) const
{
  std::string name = mFileBase + "_" + std::to_string(cameraIndex) + "_vid";
  if (mFilePart != 1)
    name += ".part" + std::to_string(mFilePart);
  return name + ".mp4";
}

void EyetrackerLoggerTobiiPro::RegisterGazeCallbacks()
{
  if (m_subscribed)
    return;

  if (tobii_research_subscribe_to_gaze_data(m_connectedDevice, gaze_data_callback, this) != TOBII_RESEARCH_STATUS_OK)
  {
    bcierr << "EyetrackerLoggerTobiiPro Error: Problem during start of gaze data collection";
    return;
  }
  m_subscribed = true;
}

void EyetrackerLoggerTobiiPro::UnregisterGazeCallbacks()
{
  if (!m_subscribed)
    return;

  if (tobii_research_unsubscribe_from_gaze_data(m_connectedDevice, gaze_data_callback) != TOBII_RESEARCH_STATUS_OK)
  {
    bcierr << "EyetrackerLoggerTobiiPro Error: Problem during unsubscribe from gaze data callback";
    return;
  }
  m_subscribed = false;
}

