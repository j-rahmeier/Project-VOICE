////////////////////////////////////////////////////////////////////////////////
// $Id: GazeMonitorFilter.cpp 8268 2024-07-23 16:38:37Z mellinger $
// Author: griffin.milsap@gmail.com
// Description: A helper filter which acts on Gaze data from an eyetracker
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
////////////////////////////////////////////////////////////////////////////////
#include "GazeMonitorFilter.h"
#include "FileUtils.h"
#include "MeasurementUnits.h"
#include "ApplicationWindow.h"

// Define visualization properties
#define EYE_RADIUS 0.05f
#define CORRECTION_TIME "4.0s"
#define CLOSE_PLANE 550
#define FAR_PLANE 600
#define DEFAULT_VIS_WIDTH 160
#define DEFAULT_VIS_HEIGHT 120
#define DEFAULT_TEMPORAL_DECIMATION 4

RegisterFilter( GazeMonitorFilter, 3.A );

GazeMonitorFilter::GazeMonitorFilter() :
  mEnforceFixation( false ),
  mFixationRadius( 0.0f ),
  mShowFixationZone( false ),
  mpFixationImage( NULL ),
  mpFixationViolationImage( NULL ),
  mFixationSize( 0 ),
  mpPrompt( NULL ),
  mpZone( NULL ),
  mVisualizeGaze( false ),
  mLogGazeInformation( false ),
  mFixated( true ),
  mScale( 0.0f ),
  mOffset( 0.0f ),
  mLoggingEyetracker( false ),
  mLoggingGaze( false ),
  mLoggingEyePos( false ),
  mLoggingEyeDist( false ),
  mCorrection( 0 ),
  mLostRightEye( false ),
  mLostLeftEye( false ),
  mAspectRatio( 1.0 ),
  mBlinkTime( 0 ),
  mBlinkBlocks( 0 ),
  mSaccadeTime( 0 ),
  mSaccadeBlocks( 0 ),
  mTemporalDecimation( 1 ),
  mBlockCount( 0 ),
  mpAppDisplay( NULL ),
  mpRightEye( NULL ),
  mpLeftEye( NULL ),
  mpGaze( NULL ),
  mFixationDuration ( 0 ),
  mFixationStartTime ( 0 ),
  mEnableGazeMonitor (false),
  mChangeFixationEnabled (false),
  mHeight(0),
  mWidth(0),
  mLastGazeX(0),
  mLastGazeY(0)
{
  /*if (!OptionalParameter("EnableGazeMonitorFilter", false))
    return;*/
  // Define the GazeMonitor Parameters
  BEGIN_PARAMETER_DEFINITIONS
    "Application:GazeMonitor int /EnableGazeMonitorFilter= 0 0 0 1 "
    " // Enable gaze monitor filter (boolean)",
    "Application:GazeMonitor int VisualizeGazeMonitorFilter= 0 0 0 1 "
      " // Visualize the gaze data on the app visualization (boolean)",
    "Application:GazeMonitor int EnforceFixation= 0 0 0 1 "
      " // Enforce a fixation point on the screen (boolean)",
    "Application:GazeMonitor string FixationX= 0.5 0.5 % % "
      " // X coordinate of fixation center - 0.0 to 1.0 (expression)",
    "Application:GazeMonitor string FixationY= 0.5 0.5 % % "
      " // Y coordinate of fixation center - 0.0 to 1.0 (expression)",
    "Application:GazeMonitor int ShowFixationZone= 0 0 0 1 "
      " // Show the legal fixation zone with a ring (boolean)",
    "Application:GazeMonitor float BlinkTime= 100ms 100ms 0 %"
      " // Maximum allowed time for invalid eyes",
    "Application:GazeMonitor float SaccadeTime= 100ms 100ms 0 %"
      " // Maximum allowed time for violating the fixation",
    "Application:GazeMonitor float FixationRadius= 0.1 0.1 0.0 1.0 "
      " // Allowable distance from fixation center",
    "Application:GazeMonitor string FixationImage= % % % % "
      " // Image to render at fixation (inputfile)",
    "Application:GazeMonitor string FixationViolationImage= % % % % "
      " // Image to render at fixation when violated (inputfile)",
    "Application:GazeMonitor string FixationViolationSound= % % % % "
      " // Sound to play when fixation violated (inputfile)",
    "Application:GazeMonitor float FixationImageSize= 0.04 0.04 0.01 1.0 "
      " // Size in percentage of screen height of the fixation image",
    "Application:GazeMonitor int LogGazeInformation= 0 0 0 1 "
      " // Add Eyetracker information to applog (boolean)",
    "Application:GazeMonitor matrix ChangeFixation= "
      "{ 1 } "
      "{ Expression FixationX FixationY } "
      //"StimulusCode==1  0.5 0.5 "
      "0 0 0"
      " // Change fixation centers during a run",
    "Application:GazeMonitor int GazeColor= 0x000000 0x000000 0 0xffffff "
    " // Color of Gaze element in visualization (color)",
    "Application:GazeMonitor int EyeColor= 0x808080 0x808080 0 0xffffff "
    " // Color of Eye elements in visualization (color)",
    END_PARAMETER_DEFINITIONS

  // Define the GazeMonitor States
  BEGIN_STATE_DEFINITIONS
    "FixationViolated 1 0 0 0",
    "GazeCorrectionMode 1 0 0 0", // Set this state true to enable user correction
    "FixationCrossVisible 1 1 0 0", // Set this state to turn off fixation cross rendering
    "FixationDuration 16 0 0 0"
  END_STATE_DEFINITIONS
}

GazeMonitorFilter::~GazeMonitorFilter()
{
  delete mpFixationImage;
  delete mpFixationViolationImage;
  delete mpZone;
  delete mpPrompt;
  delete mpRightEye;
  delete mpLeftEye;
  delete mpGaze;
  mViolationSound.Stop();
}

void
GazeMonitorFilter::Preflight( const SignalProperties &Input, SignalProperties &Output ) const
{
  Output = Input;

  if (!OptionalParameter("EnableGazeMonitorFilter", false)
    && GazeMonitorFilterEnabled())
  {
    //if no flag, but using gaze monitor filter 
    bciwarn << "GazeMonitorFilter: To use GazeMonitorFilter, append \"--EnableGazeMonitorFilter=1\" to the Application module "
      << "in your batch file. You can still currently use it without the flag (for backwards compatability), but soon this will be deprecated." << std::endl;
  }
  if( Windows->Exists( "Application" ) )
    Window( "Application" );

  bool loggingEyetracker = false, loggingGaze = false, loggingEyePos = false, loggingEyeDist = false;
  if( EyetrackerEnabled() )
  {
    loggingEyetracker = true;
    if( ( int )OptionalParameter( "LogGazeData", 0 ) )
    {
      loggingGaze = true;
      OptionalState( "EyetrackerLeftEyeGazeX" , 0);
      OptionalState( "EyetrackerLeftEyeGazeY", 0);
      OptionalState( "EyetrackerRightEyeGazeX", 0);
      OptionalState( "EyetrackerRightEyeGazeY", 0);
      OptionalParameter( "GazeScale", 1);
      OptionalParameter( "GazeOffset", 0);
      OptionalState("SourceTime");
      State("FixationDuration");
    }
    if( ( int )OptionalParameter( "LogEyePos", 0 ) )
    {
      loggingEyePos = true;
      State( "EyetrackerLeftEyePosX" );
      State( "EyetrackerLeftEyePosY" );
      State( "EyetrackerRightEyePosX" );
      State( "EyetrackerRightEyePosY" );
    }
    if( ( int )OptionalParameter( "LogEyeDist", 0 ) )
    {
      loggingEyeDist = true;
      State( "EyetrackerLeftEyeDist" );
      State( "EyetrackerRightEyeDist" );
    }
    OptionalState( "EyetrackerLeftEyeValidity" , 0);
    OptionalState( "EyetrackerRightEyeValidity" , 0);
  }

  bool enforceFixation = ( int )Parameter( "EnforceFixation" );
  bool visGaze = ( int )Parameter( "VisualizeGazeMonitorFilter" );
  bool showFixationZone = ( int )Parameter( "ShowFixationZone" );

  if( enforceFixation || visGaze )
  {
    OptionalParameter( "WindowWidth", DEFAULT_VIS_WIDTH );
    OptionalParameter( "WindowHeight", DEFAULT_VIS_HEIGHT );
  }

  if( visGaze )
  {
    OptionalParameter( "AppWindowSpatialDecimation" );
    OptionalParameter( "AppWindowTemporalDecimation", DEFAULT_TEMPORAL_DECIMATION );
  }

  OptionalParameter("EyetrackerLegacyCorrectionFactor");
  OptionalParameter("EyetrackerLegacyCorrectionOffset");

  // Do some preflight error checking
  if( enforceFixation )
  {
    GenericSignal preflightInput( Input );
    if( std::string( Parameter( "FixationX" ) ) == "" )
      bcierr << "Requested fixation enforcement but specified no FixationX coordinate.";
    else Expression( Parameter( "FixationX" ) ).Evaluate( &preflightInput );

    if( std::string( Parameter( "FixationY" ) ) == "" )
      bcierr << "Requested fixation enforcement but specified no FixationY coordinate.";
    else Expression( Parameter( "FixationY" ) ).Evaluate( &preflightInput );

    // We can't render fixation crosses just anywhere onto CursorTask 3D
    if( Parameters->Exists( "RenderingQuality" ) )
      if( Parameter( "RenderingQuality" ) == 1 )
          if( std::string( Parameter( "FixationX" ) ) != "0.5" || std::string( Parameter( "FixationY" ) ) != "0.5" )
              bcierr << "FixationX and FixationY must be set to 0.5 for CursorTask in 3D.";
 
    GUI::GraphDisplay preflightDisplay;
    ImageStimulus pimg( preflightDisplay );
    if( std::string( Parameter( "FixationImage" ) ) != "" )
      pimg.SetFile( std::string( Parameter( "FixationImage" ) ) );
    Parameter( "FixationImageSize" );

    if( loggingGaze )
    {
      State( "FixationViolated" );

      if( Parameter( "FixationRadius" ) < 0.0001 )
        bcierr << "Requested fixation enforcement but specified no FixationRadius.";

      // Check if requested images exist and try to load them
      if( std::string( Parameter( "FixationViolationImage" ) ) != "" )
        pimg.SetFile( std::string( Parameter( "FixationViolationImage" ) ) );

      // Check if the requested sound exists and try to load it
      if( std::string( Parameter( "FixationViolationSound" ) ) != "" )
      {
        std::string filename = std::string( Parameter( "FixationViolationSound" ) );
        WavePlayer wp;
        InitSound( filename, wp );
      }
      Parameter( "BlinkTime" );
      Parameter( "SaccadeTime" );
    } else
      bciout << "Cannot actually enforce fixation due to lack of gaze data.";
  }
  Parameter( "LogGazeInformation" );
  State( "GazeCorrectionMode" );
  State( "FixationCrossVisible" );

  if (Parameter("ChangeFixation")->NumColumns() > 2) //dictates if it is being used
  {
    int numFixations = Parameter("ChangeFixation")->NumRows();
    for (int i = 0; i < numFixations; i++)
    {
      Expression exp = Parameter("ChangeFixation")(i, 0).ToString();
      exp.Compile();
      GenericSignal preflightSignals(Input);
      exp.Evaluate(&preflightSignals);

      //check fixation bounds
      double x = Parameter("ChangeFixation")(i, 1);
      double y = Parameter("ChangeFixation")(i, 2);
      if (x < 0 || x > 1)
      {
        bcierr << "GazeMonitorFilter: FixationX (row " << i << ") in the ChangeFixation matrix must be between 0 and 1";
      }
      if (y < 0 || y > 1)
      {
        bcierr << "GazeMonitorFilter: FixationY (row " << i << ") in the ChangeFixation matrix must be between 0 and 1";
      }
    }
  }
}

void
GazeMonitorFilter::Initialize( const SignalProperties &Input, const SignalProperties &Output )
{
  mEnableGazeMonitor = GazeMonitorFilterEnabled();
  if (!mEnableGazeMonitor)
    return;
  mpAppDisplay = NULL;
  if( Windows->Exists( "Application" ) )
  {
    mpAppDisplay = &Window( "Application" );
    mVis.SetVisID( VisID( mpAppDisplay->VisID() ).SetLayerID( "1" ) );
  }
  else
  {
    mVis.SetVisID( "GAZE" );
  }

  delete mpFixationImage; mpFixationImage = NULL;
  delete mpFixationViolationImage; mpFixationViolationImage = NULL;
  delete mpZone; mpZone = NULL;
  delete mpPrompt; mpPrompt = NULL;

  mShowFixationZone = ( int )Parameter( "ShowFixationZone" );

  if( mpAppDisplay )
  {
    mpFixationImage = new ImageStimulus( *mpAppDisplay );
    mpFixationViolationImage = new ImageStimulus( *mpAppDisplay );
    mpFixationViolationImage->SetZOrder(0);
    mpPrompt = new TextField( *mpAppDisplay );
    if( mShowFixationZone ) mpZone = new EllipticShape( *mpAppDisplay );
  }

  delete mpRightEye;
  mpRightEye = new EllipticShape( mVisDisplay, 5 );
  delete mpLeftEye;
  mpLeftEye = new EllipticShape( mVisDisplay, 5 );
  delete mpGaze;
  mpGaze = new EllipticShape( mVisDisplay, 0 );

  mLoggingGaze = mLoggingEyePos = mLoggingEyeDist = false;
  mLoggingEyetracker = EyetrackerEnabled();
  if( mLoggingEyetracker ) {
    mLoggingGaze = ( int )OptionalParameter( "LogGazeData", 0 );
    mLoggingEyePos = ( int )OptionalParameter( "LogEyePos", 0 );
    mLoggingEyeDist = ( int )OptionalParameter( "LogEyeDist", 0 );
  }
  mEnforceFixation = ( int )Parameter( "EnforceFixation" );
  mVisualizeGaze = ( int )Parameter( "VisualizeGazeMonitorFilter" );
  mLogGazeInformation = ( int )Parameter( "LogGazeInformation" );
  mLostLeftEye = false; mLostRightEye = false;

  // If we're logging gaze at all, we need extra information to process it.
  if( mLoggingGaze )
  {
    mScale = ( float )OptionalParameter( "GazeScale", 1 );
    mOffset = ( float )OptionalParameter( "GazeOffset", 0 );
  }

  // We need an aspect ratio if we're going to do any rendering/fixation monitoring
  if( mVisualizeGaze || mEnforceFixation )
    mAspectRatio = OptionalParameter( "WindowWidth", DEFAULT_VIS_WIDTH ) 
        / ( float )OptionalParameter( "WindowHeight", DEFAULT_VIS_HEIGHT );

  if( mVisualizeGaze )
  {
    int visWidth = OptionalParameter( "WindowWidth", DEFAULT_VIS_WIDTH );
    visWidth /= OptionalParameter( "AppWindowSpatialDecimation", 1 );
    int visHeight = OptionalParameter( "WindowHeight", DEFAULT_VIS_HEIGHT );
    visHeight /= OptionalParameter( "AppWindowSpatialDecimation", 1 );
    mTemporalDecimation = OptionalParameter( "AppWindowTemporalDecimation", DEFAULT_TEMPORAL_DECIMATION );
    GUI::DrawContext dc = mVisDisplay.Context();
    GUI::Rect r = { 0, 0, visWidth, visHeight };
    dc.rect = r;
    mVisDisplay.SetContext( dc );
    mVisDisplay.SetColor( RGBColor::NullColor );

    if( mLoggingGaze )
    {
      mpGaze->SetZOrder( 0 );
      mpGaze->SetColor(RGBColor(Parameter("GazeColor")));
      mpGaze->SetScalingMode( GUI::ScalingMode::AdjustWidth );
    }

    if( mLoggingEyePos )
    {
      mpRightEye->SetColor( RGBColor::Black ); mpLeftEye->SetColor( RGBColor::Black );
      mpRightEye->SetFillColor(RGBColor(Parameter("EyeColor")));
      mpLeftEye->SetFillColor(RGBColor(Parameter("EyeColor")));
      mpRightEye->SetScalingMode( GUI::ScalingMode::AdjustWidth );
      mpLeftEye->SetScalingMode( GUI::ScalingMode::AdjustWidth );
    }

  }

  if( mEnforceFixation && mpAppDisplay )
  {
    GenericSignal initInput( Input );
    mFixationX = Expression( std::string( Parameter( "FixationX" ) ) );
    mFixationY = Expression( std::string( Parameter( "FixationY" ) ) );

    // Calculate current requested center of fixation
    float cx = mFixationX.Evaluate( &initInput );
    float cy = mFixationY.Evaluate( &initInput );

    //store fixation centers
    mChangeFixationEnabled = false;
    if (Parameter("ChangeFixation")->NumColumns() > 2)
      mChangeFixationEnabled = true;
    if (mChangeFixationEnabled)
    {
      if (!mChangeFixation.empty())
        mChangeFixation.clear();
      for (int i = 0; i < Parameter("ChangeFixation")->NumRows(); i++)
      {
        mChangeFixation.push_back(ChangeFixation(
          (Expression)Parameter("ChangeFixation")(i, 0),
          Parameter("ChangeFixation")(i, 1),
          Parameter("ChangeFixation")(i, 2)
        ));
      }
    }
    // Set up the fixation image
    mFixationSize = Parameter( "FixationImageSize" ) / 2.0f;
    if( std::string( Parameter( "FixationImage" ) ) != "" )
    {
      mpFixationImage->SetFile( std::string( Parameter( "FixationImage" ) ) );
      mpFixationImage->SetZOrder( GUI::GraphObject::MessageZOrder + 1 );
      mpFixationImage->SetRenderingMode( GUI::RenderingMode::Transparent );
      mpFixationImage->SetScalingMode( GUI::ScalingMode::AdjustWidth );
      SetObjectRect( mpFixationImage, cx, cy, mFixationSize );
      mpFixationImage->SetPresentationMode( VisualStimulus::ShowHide );
      mpFixationImage->Present();
    }

    if( mLoggingGaze )
    {
      mFixationRadius = ( float ) Parameter( "FixationRadius" );

      // Set up the fixation violation image
      if( std::string( Parameter( "FixationViolationImage" ) ) != "" )
      {
        mpFixationViolationImage->SetFile( std::string( Parameter( "FixationViolationImage" ) ) );
        mpFixationViolationImage->SetRenderingMode( GUI::RenderingMode::Transparent );
        mpFixationViolationImage->SetScalingMode( GUI::ScalingMode::AdjustWidth );
        SetObjectRect( mpFixationViolationImage, cx, cy, mFixationSize );
        mpFixationViolationImage->SetPresentationMode( VisualStimulus::ShowHide );
        mpFixationViolationImage->Conceal();
      }

      // Set up the violation sound
      std::string filename = std::string( Parameter( "FixationViolationSound" ) );
      if( filename != "" )
        InitSound( filename, mViolationSound );

      // Set up fixation zone visualization
      if( mpZone )
      {
        mpZone->SetColor( RGBColor::Gray );
        mpZone->SetFillColor( RGBColor::NullColor );
        mpZone->SetScalingMode( GUI::ScalingMode::AdjustWidth );
        mpZone->SetLineWidth( 4.0f );
        SetObjectRect( mpZone, cx, cy, mFixationRadius );
        mpZone->Show();
      }
      // Create a prompt for user correction
      mpPrompt->SetTextColor( RGBColor::White );
      GUI::Rect textRect = { 0.45f, 0.50f, 0.55f, 0.60f };
      mpPrompt->SetObjectRect( textRect );
      mpPrompt->SetTextHeight( 0.30f );
      mpPrompt->Hide();

      mBlinkTime = Parameter( "BlinkTime" ).InSampleBlocks();
      mBlinkBlocks = 0;
      mSaccadeTime = Parameter( "SaccadeTime" ).InSampleBlocks();
      mSaccadeBlocks = 0;
      mFixated = true;

      if( mLogGazeInformation ) AppLog << "GazeMonitorFilter Enforcing Fixation." << std::endl;
    }
  }
  if( mLogGazeInformation ) AppLog << "GazeMonitorFilter Initialized." << std::endl;

  auto LegacyCorrectionFactor = OptionalParameter("EyetrackerLegacyCorrectionFactor");
  if (LegacyCorrectionFactor->NumValues() >= 3)
  {
    mLegacyCorrectionFactorX = LegacyCorrectionFactor("x");
    mLegacyCorrectionFactorY = LegacyCorrectionFactor("y");
    mLegacyCorrectionFactorZ = LegacyCorrectionFactor("z");
  }
  else
  {
    mLegacyCorrectionFactorX = 1;
    mLegacyCorrectionFactorY = 1;
    mLegacyCorrectionFactorZ = 1;
  }

  auto LegacyCorrectionOffset = OptionalParameter("EyetrackerLegacyCorrectionOffset");
  if (LegacyCorrectionOffset->NumValues() >= 3)
  {
      mLegacyCorrectionOffsetX = LegacyCorrectionOffset("x");
      mLegacyCorrectionOffsetY = LegacyCorrectionOffset("y");
      mLegacyCorrectionOffsetZ = LegacyCorrectionOffset("z");
  }
  else
  {
      mLegacyCorrectionOffsetX = 0;
      mLegacyCorrectionOffsetY = 0;
      mLegacyCorrectionOffsetZ = 0;
  }
}

void
GazeMonitorFilter::StartRun()
{
  if (!mEnableGazeMonitor)
    return;
  if( mVisualizeGaze )
  {
    mpGaze->Show();
    mpLeftEye->Show();
    mpRightEye->Show();
    mVisDisplay.Paint();
    mVis.SendReferenceFrame( mVisDisplay.BitmapData() );
    mBlockCount = mTemporalDecimation - 1;
  }
  mLastGazeX = 0.0f; mLastGazeY = 0.0f;

  //EyeLink and TobiiX have different coordinate systems
  if (OptionalParameter("LogEyetrackerEyeLink", 0))
  {
    mWidth = 2.0f * 65535.0f / (float)OptionalParameter("WindowWidth", DEFAULT_VIS_WIDTH);
    mHeight = 2.0f * 65535.0f / (float)OptionalParameter("WindowHeight", DEFAULT_VIS_HEIGHT);
  }
  else if (OptionalParameter("LogEyetrackerTobiiX", 0))
  {
    mWidth = 65535.0f / (float)OptionalParameter("WindowWidth", DEFAULT_VIS_WIDTH);
    mHeight = 65535.0f / (float)OptionalParameter("WindowHeight", DEFAULT_VIS_HEIGHT);
  }
  else
  {
    mWidth = 1;
    mHeight = 1;
  }
}

void
GazeMonitorFilter::StopRun()
{
  if (!mEnableGazeMonitor)
    return;
  // Ensure We're not in "Correction" state
  State( "GazeCorrectionMode" ) = 0;
  mCorrection = 0;
  if( mpAppDisplay )
  {
    mpPrompt->Hide();
    if( mpZone ) mpZone->SetColor( RGBColor::Gray );
  }

  // Hide visualizations
  mpGaze->Hide();
  mpLeftEye->Hide();
  mpRightEye->Hide();
}

void
GazeMonitorFilter::Halt()
{
}

void
GazeMonitorFilter::Process( const GenericSignal &Input, GenericSignal &Output )
{
  Output = Input;
  if (!mEnableGazeMonitor)
    return;
  // Determine gaze location
  float gx = 0.0f, gy = 0.0f;
  if( mLoggingGaze )
  {
    if (OptionalParameter("LogEyetrackerEyeLink", 0) &&
      !((OptionalState("EyetrackerLeftEyeGazeX", 0) == 0) || (OptionalState("EyetrackerRightEyeGazeX", 0) == 0)))
    {
      //EyeLink binocular mode
      mWidth /= 2;
      mHeight /= 2;
    }
    gx = ( float )OptionalState( "EyetrackerLeftEyeGazeX" ,0) + ( float )OptionalState( "EyetrackerRightEyeGazeX" ,0);
    gy = ( float )OptionalState( "EyetrackerLeftEyeGazeY" ,0) + ( float )OptionalState( "EyetrackerRightEyeGazeY" ,0);
    gx /= 2.0f;
    gy /= 2.0f;
    gx -= mLegacyCorrectionOffsetX;
    gy -= mLegacyCorrectionOffsetY;
    gx *= mLegacyCorrectionFactorX;
    gy *= mLegacyCorrectionFactorY;
    gx *= mWidth;   gx /= 65535.0f;  gx -= mOffset; gx /= mScale;
    gy *= mHeight;  gy /= 65535.0f;  gy -= mOffset; gy /= mScale;
  }

  // Determine eye position
  float plx = 0.0f, ply = 0.0f;
  float prx = 0.0f, pry = 0.0f;
  if( mLoggingEyePos )
  {
    plx = State("EyetrackerLeftEyePosX");
    plx -= mLegacyCorrectionOffsetX;
    plx *= mLegacyCorrectionFactorX;
    plx = 1.0f - plx / 65535.0f;
    ply = State("EyetrackerLeftEyePosY");
    ply -= mLegacyCorrectionOffsetY;
    ply *= mLegacyCorrectionFactorY;
    ply = ply / 65535.0f;
    prx = State("EyetrackerRightEyePosX");
    prx -= mLegacyCorrectionOffsetX;
    prx *= mLegacyCorrectionFactorX;
    prx = 1.0f - prx / 65535.0f;
    pry = State( "EyetrackerRightEyePosY" );
    pry -= mLegacyCorrectionOffsetY;
    pry *= mLegacyCorrectionFactorY;
    pry = pry / 65535.0f;
  }

  // Determine Eye Distance
  float eyedist = 550.0f;
  if (mLoggingEyeDist)
  {
      eyedist = ((float)State("EyetrackerLeftEyeDist")
          + (float)State("EyetrackerRightEyeDist")) / 2.0f;
      eyedist -= mLegacyCorrectionOffsetZ;
      eyedist *= mLegacyCorrectionFactorZ;
  }


  // Determine (and log) Validity
  bool leftEyeValid = true, rightEyeValid = true;
  if( mLoggingEyetracker )
  {
    leftEyeValid = ( int )OptionalState( "EyetrackerLeftEyeValidity" , 15) < 4; //number might have to be changed
    rightEyeValid = ( int )OptionalState( "EyetrackerRightEyeValidity" , 15) < 4;
  }

  if (mLostLeftEye && mLostRightEye)
  {
    if (leftEyeValid && rightEyeValid)
    {
      if (mLogGazeInformation) AppLog << "Both Eyes Tracking Correctly." << std::endl;
      mLostLeftEye = false;
      mLostRightEye = false;
      mBlinkBlocks = 0;
    }
    else
      mBlinkBlocks++;

  }

	if (leftEyeValid && mLostLeftEye)
	{
		if (mLogGazeInformation) AppLog << "Left Eye Tracking Correctly." << std::endl;
		mLostLeftEye = false;
		mBlinkBlocks = 0;
	}

	if (rightEyeValid && mLostRightEye)
	{
		if (mLogGazeInformation) AppLog << "Right Eye Tracking Correctly." << std::endl;
		mLostRightEye = false;
		mBlinkBlocks = 0;
	}


  if( !leftEyeValid && !mLostLeftEye )
  {
    if( mLogGazeInformation ) AppLog << "Lost Left Eye!" << std::endl;
    mLostLeftEye = true;
  }

  if( !rightEyeValid && !mLostRightEye )
  {
    if( mLogGazeInformation ) AppLog << "Lost Right Eye!" << std::endl;
    mLostRightEye = true;
  }
  bool eyesInvalid = ( mBlinkBlocks > mBlinkTime );

  // Update position of visualizations
  if( mLoggingGaze )
  {
    float x = mLastGazeX + ( gx - mLastGazeX ) / 3.0f;
    float y = mLastGazeY + ( gy - mLastGazeY ) / 3.0f;
    mLastGazeX = x; mLastGazeY = y;  
    SetObjectRect( mpGaze, gx, gy, ( ( eyedist - 400 ) / 400 ) * 0.06 );
    if( mpPrompt ) SetObjectRect( mpPrompt, x, y, 0.07f );
    mpGaze->SetFillColor( RGBColor( ( int )eyedist % CLOSE_PLANE, 
      CLOSE_PLANE - ( ( int )eyedist % CLOSE_PLANE ), 50 ) );
    mpGaze->Show();
  }

  if( mLoggingEyePos )
  {
    SetObjectRect( mpLeftEye, plx, ply, EYE_RADIUS );
    if( leftEyeValid ) { mpLeftEye->Show(); }
    else { mpLeftEye->Hide(); }
    SetObjectRect( mpRightEye, prx, pry, EYE_RADIUS );
    if( rightEyeValid ) { mpRightEye->Show(); }
    else { mpRightEye->Hide(); }
  }

  if( mEnforceFixation && mpAppDisplay )
  {
    // Set fixation zone based on current stimulus
    float fx = -1;
    float fy;
    if (mChangeFixationEnabled)
    {
      for (auto it = mChangeFixation.begin(); it != mChangeFixation.end(); it++)
      {
        if (it->exp.Evaluate())
        {
          fx = it->cx;
          fy = it->cy;
          break;
        }
      }
    }
    if (fx == -1)
    {
      fx = (float)mFixationX.Evaluate(&Input);
      fy = (float)mFixationY.Evaluate(&Input);
    }

    // Move visual stimuli to fixation
    if( mpFixationImage )
      SetObjectRect( mpFixationImage, fx, fy, mFixationSize );

    if( mLoggingGaze )
    {
      if( mpFixationViolationImage )
        SetObjectRect( mpFixationViolationImage, fx, fy, mFixationSize );
      if( mpZone )
        SetObjectRect( mpZone, fx, fy, mFixationRadius );

      // Calculate distance of gaze from fixation center
      float dist = pow( ( gx - fx ) * mAspectRatio, 2.0f ) + pow( gy - fy, 2.0f );
      dist = sqrt( dist );
      bool lookingAway = false;
      if( dist > mFixationRadius )
      {
        // Subject is looking outside the fixation radius.  Is it a momentary saccade?
        if( mSaccadeBlocks <= mSaccadeTime )
          mSaccadeBlocks++;
        else // This is more than a blink
          lookingAway = true;
      }
      else
        mSaccadeBlocks = 0;

      // Fixation Violation Logic
      if( mFixated && ( eyesInvalid || lookingAway ) )
        ViolatedFixation();
      else if( !mFixated && ( !eyesInvalid && !lookingAway ) )
        AcquiredFixation();
      //update fixation duration
      if ( mFixated && mFixationStartTime > 0 )
      {
        mFixationDuration = State("SourceTime") - mFixationStartTime;
      }
      State("FixationDuration") = mFixationDuration;

      // If we want the subject to correct their eyetracking behavior, show them eyetracker output
      if( ( int )State( "GazeCorrectionMode" ) )
      {
        // Give the subject a prompt
        mpPrompt->Show();
        std::string prompt = "";
        int correctionTime = MeasurementUnits::TimeInSampleBlocks( CORRECTION_TIME );
        if( mCorrection >= ( ( correctionTime * 3 ) / 4 ) ) prompt = "1...";
        else if( mCorrection >= ( ( correctionTime * 2 ) / 4 ) ) prompt = "2...";
        else if( mCorrection >= ( correctionTime / 4 ) ) prompt = "3...";
        else if( mCorrection != 0 ) prompt = "OK!";
        else prompt = "Fixate";

        // Determine if user has corrected fixation
        if( mFixated && eyedist >= CLOSE_PLANE && eyedist < FAR_PLANE )
        {
          mCorrection++;
          if( mpZone ) mpZone->SetColor( RGBColor::Green );
          if( mCorrection >= correctionTime ) 
          {
            //Disable drawing to the subject's screen
            State( "GazeCorrectionMode" ) = 0;
            mCorrection = 0;
            mpPrompt->Hide();
            if( mpZone ) mpZone->SetColor( RGBColor::Gray );
          }
        } else if( mFixated && eyedist < CLOSE_PLANE ) {
          prompt = "Further";
          mCorrection = 0;
          if( mpZone ) mpZone->SetColor( RGBColor::Gray );
        } else if( mFixated && eyedist >= FAR_PLANE ) {
          prompt = "Closer";
          mCorrection = 0;
          if( mpZone ) mpZone->SetColor( RGBColor::Gray );
        } else {
          mCorrection = 0;
          if( mpZone ) mpZone->SetColor( RGBColor::Gray );
        }
        mpPrompt->SetText( prompt );
      }
    }
  }

  // Using correction without fixation enforcement is a no-no.
  if( ( int )State( "GazeCorrectionMode" ) && !mEnforceFixation && mLoggingGaze )
  {
    bciout << "Attempting to use eyetracker correction without full fixation enforcement.";
    State( "GazeCorrectionMode" ) = 0;
  }

  // Handle Fixation Cross Hiding
  if( ( int )State( "FixationCrossVisible" ) == 0 )
  {
    if( mpFixationImage ) mpFixationImage->Hide();
    if( mpFixationViolationImage ) mpFixationViolationImage->Hide();
  } 
  else 
  {
    if( mpFixationImage ) mpFixationImage->Show();
    if( mpFixationViolationImage ) mpFixationViolationImage->Show();
  }

  // Draw preview frame
  if( mVisualizeGaze && ( ++mBlockCount %= mTemporalDecimation ) == 0 )
  {
    mVisDisplay.Paint();
    BitmapImage b = mVisDisplay.BitmapData();
    mVis.SendDifferenceFrame( b );
  }
}

void
GazeMonitorFilter::InitSound( const std::string& inFilename, WavePlayer& ioPlayer ) const
{
  if( ioPlayer.ErrorState() != WavePlayer::noError )
    bcierr << "There was an issue creating a waveplayer object.";
  ioPlayer.SetFile( FileUtils::AbsolutePath( inFilename ) );
  if( ioPlayer.ErrorState() != WavePlayer::noError )
    bcierr << "Could not open file: " << FileUtils::AbsolutePath( inFilename );
  ioPlayer.SetVolume( 1.0f );
}

void
GazeMonitorFilter::SetObjectRect( GUI::GraphObject *obj, float cx, float cy, float rad )
{
  GUI::Rect rect = { cx - ( rad / mAspectRatio ), cy - rad,
                     cx + ( rad / mAspectRatio ), cy + rad };
  obj->SetObjectRect( rect );
}

void
GazeMonitorFilter::ViolatedFixation()
{
  if( mLogGazeInformation ) AppLog << "Gaze Left Fixation." << std::endl;
  mViolationSound.Play();
  State( "FixationViolated" ) = true;
  if( mpFixationImage ) mpFixationImage->Conceal();
  if( mpFixationViolationImage ) mpFixationViolationImage->Present();
  mFixated = false;
  mFixationDuration = 0;
}

void
GazeMonitorFilter::AcquiredFixation()
{
  if( mLogGazeInformation ) AppLog << "Gaze Fixation Acquired." << std::endl;
  State( "FixationViolated" ) = false;
  if( mpFixationImage ) mpFixationImage->Present();
  if( mpFixationViolationImage ) mpFixationViolationImage->Conceal();
  mFixated = true;
  mFixationStartTime = State("SourceTime");
  mFixationDuration = 0;
}

bool
GazeMonitorFilter::EyetrackerEnabled() const
{
  return (int)OptionalParameter("LogEyetracker", 0)
    || (int)OptionalParameter("LogEyetrackerTobiiPro", 0)
    || (int)OptionalParameter("LogEyetrackerEyeLink", 0)
    || (int)OptionalParameter("LogEyetrackerTobiiX", 0)
    || (int)OptionalParameter("LogEyetrackerTobii3", 0)
    || (int)OptionalParameter("LogEyetrackerSimulator", 0);
}

bool
GazeMonitorFilter::GazeMonitorFilterEnabled() const
{
  return OptionalParameter("EnableGazeMonitorFilter", false)

      //parameters that show we want to use gaze monitor filter
      || Parameter("VisualizeGazeMonitorFilter")
      || Parameter("EnforceFixation")
      || Parameter("ShowFixationZone");
}
