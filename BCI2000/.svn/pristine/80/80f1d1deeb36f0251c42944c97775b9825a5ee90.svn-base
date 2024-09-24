#include "HyperscanningColorChangeTest.h"
#include "Shapes.h"
#include <limits>
#include "BCIEvent.h"

RegisterFilter( HyperscanningColorChangeTest, 3 );

HyperscanningColorChangeTest::HyperscanningColorChangeTest() : mpRect( NULL ), mrDisplay( Window() ) {
}

void HyperscanningColorChangeTest::SharedPublish() {
}

void HyperscanningColorChangeTest::SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const {
	if ( State( "Color" )->Length() != 8 ) {
		bciwarn << State( "Color" );
		bcierr << "The color state must be defined";
	}
	State( "MouseKeys" );
	Output = Input;
}

void HyperscanningColorChangeTest::SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) {
	ApplicationBase::Initialize( Input, Output );

	GUI::Rect rect = { 0.1f, 0.4f, 0.9f, 0.6f };

	mpRect = new RectangularShape( mrDisplay );
	mpRect->SetFillColor( RGBColor::Teal );
	mpRect->SetObjectRect( rect );
	mpRect->SetVisible( true );
}

void HyperscanningColorChangeTest::SharedProcess( const GenericSignal& Input, GenericSignal& Output ) {
	bciwarn << "Color: " << State( "Color" );
	mpRect->SetFillColor( State( "Color" ) ? RGBColor::Teal : RGBColor::Red );
	if ( State( "MouseKeys" ) )
	    State( "Color" ) = !State( "Color" );
}
