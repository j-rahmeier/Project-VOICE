#include "HyperscanningFlashTest.h"
#include "Shapes.h"
#include "TextField.h"
#include <limits>
#include "BCIEvent.h"
#include <string>


//Shared States
//on, 8

RegisterFilter( HyperscanningFlashTest, 3 );

HyperscanningFlashTest::HyperscanningFlashTest() : square( NULL ), mrDisplay( Window() ) {
}

void HyperscanningFlashTest::SharedPublish() {
}

void HyperscanningFlashTest::SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const {
	State( "on" );
	State( "ClientNumber" );

	Parameter( "SampleBlockSize" );
}

void HyperscanningFlashTest::SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) {
	ApplicationBase::Initialize( Input, Output );

	square = new RectangularShape(mrDisplay);
	square->SetFillColor( RGBColor::White );
    square->SetObjectRect( { 0.f, 0.f, 1.f, 1.f } );
	square->SetVisible( false );

	counter = 0;

}

void HyperscanningFlashTest::SharedProcess( const GenericSignal& Input, GenericSignal& Output ) {
	if ( State( "on" ) == State( "ClientNumber" ) ) {

		if ( counter == 0 )
			square->SetVisible( true );
		else if ( counter == 2 )
			square->SetVisible( false );

		if ( counter == 20 ) {
			counter = 0;
			State( "on" ) = !State( "on" );
			square->SetVisible( true );
		} else {
			counter++;
		}

	} else {
		square->SetVisible( false );
	}
}

bool HyperscanningFlashTest::CheckKeyPress( std::string event_name, int value ) {
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( event_name )( i ) == value ) {
			return true;
		}
	}
	return false;
}
