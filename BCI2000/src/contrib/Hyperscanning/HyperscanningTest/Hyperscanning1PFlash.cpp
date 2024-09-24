#include "Hyperscanning1PFlash.h"
#include "Shapes.h"
#include "TextField.h"
#include <limits>
#include "BCIEvent.h"
#include <string>


//Shared States
//on, 8

RegisterFilter( Hyperscanning1PFlash, 3 );

Hyperscanning1PFlash::Hyperscanning1PFlash() : square2( NULL ), square( NULL ), mrDisplay( Window() ) {
}

void Hyperscanning1PFlash::Publish() {
}

void Hyperscanning1PFlash::Preflight( const SignalProperties& Input, SignalProperties& Output ) const {
	State( "on" );

	Parameter( "SampleBlockSize" );
}

void Hyperscanning1PFlash::Initialize( const SignalProperties& Input, const SignalProperties& Output ) {
	ApplicationBase::Initialize( Input, Output );

	square = new RectangularShape(mrDisplay);
	square->SetFillColor( RGBColor::White );
	square->SetDisplayRect( { 0.f, 0.f, 0.5f, 1.f } );
	square->SetVisible( false );

	square2 = new RectangularShape(mrDisplay);
	square2->SetFillColor( RGBColor::White );
	square2->SetDisplayRect( { 0.5f, 0.f, 1.f, 1.f } );
	square2->SetVisible( false );

}

void Hyperscanning1PFlash::Process( const GenericSignal& Input, GenericSignal& Output ) {
	if ( State( "on" ) == 1 ) {
		square->SetVisible( true );
		square2->SetVisible( false );
	} else {
		square->SetVisible( false );
		square2->SetVisible( true );
		State( "on" ) = 1;
	}
}

bool Hyperscanning1PFlash::CheckKeyPress( std::string event_name, int value ) {
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( event_name )( i ) == value ) {
			return true;
		}
	}
	return false;
}
