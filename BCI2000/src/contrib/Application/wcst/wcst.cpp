#include "wcst.h"
#include "ImageStimulus.h"
#include "TextStimulus.h"
#include "DisplayWindow.h"

#include <fstream>
#include <cstdlib>


RegisterFilter( wcst, 3 );


wcst::wcst(): mrDisplay( Window() ) {
	location = "../src/contrib/Application/wcst/";

	BEGIN_PARAMETER_DEFINITIONS
		"Application:Sequencing float CorrectnessDuration= 1.5s 1.5s 0 % // Duration to display correctness",
		"Application:Stimuli int ImageSize= 200 200 % % // Size of the images to display in pixels",
		"Application:Stimuli int ImageSpacingX= 100 100 % % // Distance from the center of the window to the edges of the images on x axis",
		"Application:Stimuli int ImageSpacingY= 100 100 % % // Distance from the center of the window to the edges of the images on y axis",
		"Application:Stimuli int MaxTrials= 200 200 % % // Number of trials",
	END_PARAMETER_DEFINITIONS

	BEGIN_STATE_DEFINITIONS
		"RuleDim             8 0 0 0",  // Dimension of the rule
		"RuleN               8 0 0 0",  // Number within the dimension of the rule
		"CorrectDirection    8 0 0 0", // Correct direciton for the subject to indicate
		"IndicatedDirection  8 0 0 0", // Direction that the subject indicated
		"Trial              16 0 0 0", // Trial number

		//     N:  0   1   2   3
		// Dim 0: 'C' 'Q' 'S' 'T' 
		// Dim 1: 'B' 'G' 'M' 'Y'
		// Dim 2: 'L' 'P' 'R' 'S'

	END_STATE_DEFINITIONS
}

wcst::~wcst() {
}

void wcst::OnPreflight( const SignalProperties& inp ) const {
	State( "KeyDown" );
	Parameter( "SampleBlockSize" );

	Parameter( "CorrectnessDuration" );
	Parameter( "ImageSize" );
	Parameter( "ImageSpacingX" );
	Parameter( "ImageSpacingY" );
	Parameter( "MaxTrials" );

	State( "RuleDim" );
	State( "RuleN" );
	State( "CorrectDirection" );
	State( "IndicatedDirection" );
	State( "Trial" );

}

void wcst::OnInitialize( const SignalProperties& inp ) {

	text = new TextStimulus( mrDisplay );
	text->SetTextHeight( 0.05f );
	text->SetTextColor( RGBColor::White );
    text->SetObjectRect( { 0.f, 0.f, 1.f, 1.f } );
	Associations()[ 4 ].Add( text );
	
	topImage = new ImageStimulus( mrDisplay );
	topImage->SetFile( location + "CBL.bmp" );

	int width = Parameter( "ImageSize" );
	float x = ( float )width / ( float ) mrDisplay.Display().Width();
	x /= 2;

	int height = Parameter( "ImageSize" );
	float y = ( float ) height / ( float ) mrDisplay.Display().Height();
	y /= 2;

	float spacex = ( float ) Parameter( "ImageSpacingX" ) / ( float ) mrDisplay.Display().Width();
	float spacey = ( float ) Parameter( "ImageSpacingY" ) / ( float ) mrDisplay.Display().Height();


    topImage->SetObjectRect( { 0.5f - x, 0.5f - ( 2 * y ) - spacey, 0.5f + x, 0.5f - spacey } );
	Associations()[ 1 ].Add( topImage );

	rightImage = new ImageStimulus( mrDisplay );
    rightImage->SetObjectRect( { 0.5f + spacex, 0.5f - y, 0.5f + ( 2 * x ) + spacex, 0.5f + y } );
	Associations()[ 1 ].Add( rightImage );

	bottomImage = new ImageStimulus( mrDisplay );
    bottomImage->SetObjectRect( { 0.5f - x, 0.5f + spacey, 0.5f + x, 0.5f + ( 2 * y ) + spacey } );
	Associations()[ 1 ].Add( bottomImage );

	leftImage = new ImageStimulus( mrDisplay );
    leftImage->SetObjectRect( { 0.5f - ( 2 * x ) - spacex, 0.5f - y, 0.5f - spacex, 0.5f + y } );
	Associations()[ 1 ].Add( leftImage );
	Associations()[ 1 ].SetISIMaxDuration( 0 );
	Associations()[ 1 ].SetISIMinDuration( 0 );

	fixationCross = new ImageStimulus( mrDisplay );
	fixationCross->SetFile( location + "cross.png" );
    fixationCross->SetObjectRect( { 0.5f - x, 0.5f - y, 0.5f + x, 0.5f + y } );
	Associations()[ 3 ].Add( fixationCross );


	correctText = new TextStimulus( mrDisplay );
	correctText->SetTextHeight( 0.3f );
	correctText->SetTextColor( RGBColor::White );
    correctText->SetObjectRect( { 0.f, 0.f, 1.f, 0.5f } );
	correctText->SetText( "Correct" );

	incorrectText = new TextStimulus( mrDisplay );
	incorrectText->SetTextHeight( 0.3f );
	incorrectText->SetTextColor( RGBColor::White );
    incorrectText->SetObjectRect( { 0.f, 0.f, 1.f, 0.5f } );
	incorrectText->SetText( "Incorrect" );

	Associations()[ 2 ].Add( correctText );
	Associations()[ 2 ].Add( incorrectText );
	Associations()[ 2 ].SetStimulusDuration( static_cast<int>( Parameter( "CorrectnessDuration" ).InSampleBlocks() ) );
	Associations()[ 2 ].SetISIMinDuration( static_cast<int>( Parameter( "ISIMinDuration" ).InSampleBlocks() ) );
	Associations()[ 2 ].SetISIMaxDuration( static_cast<int>( Parameter( "ISIMaxDuration" ).InSampleBlocks() ) );

	trials = 1;
	stim = 0;
	maxTrials = Parameter ("MaxTrials");
	correctInARow = 0;


}

void wcst::DoPreRun( const GenericSignal & input, bool& doProgress ) {
	// Welcome Text
	text->SetText( "Welcome to the task.\n Press any key to continue" );
	Associations()[ 4 ].Present();

	doProgress = false;
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( "KeyDown" )( i ) ) {
			doProgress = true;
			Associations()[ 4 ].Conceal();

			State( "ruleN" ) = rand() % 4;
			State( "ruleDim" ) = rand() % 3;
			AppLog << "Initial Rule: " << ruleChars[ State( "ruleDim" ) ][ State( "ruleN" ) ] << " (" << State( "ruleDim" ) << ", " << State( "ruleN" ) << ")" << std::endl;
		}
	}

}

int wcst::OnNextStimulusCode() {
	// if completed trials, end
	if ( trials > maxTrials ) {
		return 0;
	}

	// if reset, present images
	if ( stim == 0 ) {
		stim = 1;
		State( "IndicatedDirection" ) = 0;
		return stim;
	}

	// if presented images, present correctness
	if ( stim == 1 ) {
		stim = 2;
		return stim;
	}

	// if presented correctness, present reset
	if ( stim == 2 ) {
		stim = 0;
		trials++;
		return stim;
	}


	bciwarn << "Unknown Stimulus Situation";
	return 0;
}

void wcst::DoPreSequence( const GenericSignal& input, bool& doProgress ) {
	// Fixation Cross
	Associations()[ 3 ].Present();

	if ( doProgress ) {
		Associations()[ 3 ].Conceal();
	
		int dims[] = { rand() % 4, rand() % 4, rand() % 4 };

		State( "CorrectDirection" ) = 1 + ( ( 4 + State( "ruleN" ) - dims[ State( "ruleDim" ) ] ) % 4 );
		bciout << "Correct direction: " << State( "CorrectDirection" );
		
		topImage->SetFile( location + ruleChars[ 0 ][ dims[ 0 ] ] + ruleChars[ 1 ][ dims[ 1 ] ] + ruleChars[ 2 ][ dims[ 2 ] ]  + ".bmp" );	
		for ( int i = 0; i < 3; i++ ) dims[ i ] = ( dims[ i ] + 1 ) % 4;

		rightImage->SetFile( location + ruleChars[ 0 ][ dims[ 0 ] ] + ruleChars[ 1 ][ dims[ 1 ] ] + ruleChars[ 2 ][ dims[ 2 ] ]  + ".bmp" );	
		for ( int i = 0; i < 3; i++ ) dims[ i ] = ( dims[ i ] + 1 ) % 4;

		bottomImage->SetFile( location + ruleChars[ 0 ][ dims[ 0 ] ] + ruleChars[ 1 ][ dims[ 1 ] ] + ruleChars[ 2 ][ dims[ 2 ] ]  + ".bmp" );	
		for ( int i = 0; i < 3; i++ ) dims[ i ] = ( dims[ i ] + 1 ) % 4;

		leftImage->SetFile( location + ruleChars[ 0 ][ dims[ 0 ] ] + ruleChars[ 1 ][ dims[ 1 ] ] + ruleChars[ 2 ][ dims[ 2 ] ]  + ".bmp" );	
	}


}
void wcst::OnStimulusBegin( int stimcode ) {

	State("Trial") = trials;

	if ( stimcode == 2 ) {
		if ( State( "IndicatedDirection" ) == State( "CorrectDirection" ) ) {
			correctText->Present();
			incorrectText->Conceal();
			answers.push_back( 1 );
			correctInARow++;
		} else {
			incorrectText->Present();
			correctText->Conceal();
			answers.push_back( 0 );
			correctInARow = 0;
		}

		AppLog << "=========================" << std::endl;
		AppLog << "--------------------------------------------------------" << std::endl;
		AppLog << "Trial #" << trials << " / " << maxTrials << std::endl;
		AppLog << "--------------------------------------------------------" << std::endl;
		AppLog << ( ( State( "IndicatedDirection" ) == State( "CorrectDirection" ) ) ? "Correct" : "Incorrect" ) << std::endl;
		AppLog << correctInARow << " correct in a row" << std::endl;

		// Regenerate Rule
		if ( correctInARow >= 5 ) {
			correctInARow = 0;

			bool withinDimension = rand() % 2;
			AppLog << "-------------" << std::endl;
			AppLog << "Rule Change" << std::endl;
			AppLog << "-------------" << std::endl;

			bciout << "Within Dimension: " << withinDimension;

			if ( !withinDimension ) {
				State( "ruleDim" ) = ( 1 + State( "ruleDim" ) + ( rand() % 2 ) ) % 3;
				State( "ruleN" ) = rand() % 4;
				AppLog << "Interdimensional Chnage. New Dimension: " << State( "ruleDim" ) << std::endl;
			} else {
				int n = 1 + ( rand() % 3 );
				State( "ruleN" ) = ( State( "ruleN" ) + n ) % 4;
				AppLog << "Intradimensional Change" << std::endl;
			}
			

			AppLog << "New Rule: " << ruleChars[ State( "ruleDim" ) ][ State( "ruleN" ) ] << std::endl;
			AppLog << "-------------" << std::endl;

			bciout << "Rule: " << State( "ruleDim" ) << ", " << State( "ruleN" );
		}
		AppLog << "--------------------------------------------------------" << std::endl;
	} else {
		Associations()[ stimcode ].Present();
	}
}

void wcst::DoStimulus( const GenericSignal & input, bool& doProgress ) {
	if ( stim == 1 ) {
		if ( CheckKeyPress( "KeyDown", 38 ) ) { // up
			State( "IndicatedDirection" ) = 1;
		}
		if ( CheckKeyPress( "KeyDown", 39 ) ) { // right
			State( "IndicatedDirection" ) = 2;
		}
		if ( CheckKeyPress( "KeyDown", 40 ) ) { // down
			State( "IndicatedDirection" ) = 3;
		}
		if ( CheckKeyPress( "KeyDown", 37 ) ) { // left
			State( "IndicatedDirection" ) = 4;
		}
		if ( State( "IndicatedDirection" ) )
			doProgress = true;

	}
}


void wcst::DoPostRun( const GenericSignal& input, bool& doProgress ) {
	text->SetText( "Thanks for your participation" );
	Associations()[ 4 ].Present();
	doProgress = false;
}

bool wcst::CheckKeyPress( std::string event_name, int value ) {
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( event_name )( i ) == value ) {
			return true;
		}
	}
	return false;
}

void wcst::OnHalt() {
	Associations()[ 1 ].Clear();
	Associations()[ 2 ].Clear();
	Associations()[ 3 ].Clear();
	Associations()[ 4 ].Clear();

	if ( text )
		delete text;

	if ( fixationCross )
		delete fixationCross;

	if ( topImage ) {
		delete topImage;
		delete rightImage;
		delete bottomImage;
		delete leftImage;
	}

	if ( correctText ) {
		delete correctText;
		delete incorrectText;
	}
}
