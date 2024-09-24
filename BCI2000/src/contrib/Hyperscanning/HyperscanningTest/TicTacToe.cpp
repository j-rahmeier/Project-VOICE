#include "TicTacToe.h"
#include "Shapes.h"
#include "TextField.h"
#include <limits>
#include "BCIEvent.h"
#include <string>
#include <math.h>

//Shared States
//grid, 32
//turn, 8
//x, 8
//y, 8

RegisterFilter( TicTacToe, 3 );

TicTacToe::TicTacToe() : winText( NULL ), highlight( NULL ), tiles( NULL ), board( NULL ), mrDisplay( Window() ) {
}

void TicTacToe::SharedPublish() {
	BEGIN_PARAMETER_DEFINITIONS
		"Application:TicTacToe int grid= auto 0 % % // inital grid",
		"Application:TicTacToe int turn= auto 0 % % // inital turn",
	END_PARAMETER_DEFINITIONS

	Parameter( "grid" ) = 0;
	Parameter( "turn" ) = 0;


}

void TicTacToe::SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const {
	State( "grid" );
	State( "turn" );
	State( "ClientNumber" );
	State( "KeyDown" );
	State( "MouseKeys" );
	State( "MousePosX" );
	State( "MousePosY" );
	State( "x" );
	State( "y" );

	Parameter( "SampleBlockSize" );
	Parameter( "WindowWidth" );
	Parameter( "WindowHeight" );
	Parameter( "WindowTop" );
	Parameter( "WindowLeft" );
	Parameter( "grid" );
	Parameter( "turn" );
}

void TicTacToe::SharedStartRun() {
	if ( State( "ClientNumber" ) == 0 ) {
		State( "grid" ) = Parameter( "grid" );
		State( "turn" ) = Parameter( "turn" );
	}
}

void TicTacToe::SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) {

	board = new ImageStimulus( mrDisplay );
	board->SetObjectRect( { 0.1f, 0.1f, 0.9f, 0.9f } );
	board->SetScalingMode( GUI::ScalingMode::AdjustHeight );
	board->SetRenderingMode( GUI::RenderingMode::Transparent );
	board->SetFile( "../src/contrib/Hyperscanning/HyperscanningTest/tictactoe.png" );
	board->SetZOrder( 12 );

	winText = new TextField( mrDisplay );
	winText->SetVisible( true );
	winText->SetObjectRect( { 0.2f, 0.4f, 0.8f, 0.6f } );
	winText->SetTextHeight( 0.2f );
	board->SetZOrder( 1 );

	tiles = ( ImageStimulus** ) malloc( sizeof( ImageStimulus ) * 9 );
	for ( int i = 0; i < 9; i++ ) {
		tiles[ i ] = new ImageStimulus( mrDisplay );
		int x = i % 3;
		int y = i / 3;
		tiles[ i ]->SetObjectRect( { x / 3.f, y / 3.f, x / 3.f + 0.33f, y / 3.f + 0.33f } );
		tiles[ i ]->SetScalingMode( GUI::ScalingMode::AdjustBoth );
		tiles[ i ]->SetRenderingMode( GUI::RenderingMode::Opaque );
		tiles[ i ]->SetZOrder( i + 2 );
	}

	highlight = new ImageStimulus( mrDisplay );
	highlight->SetObjectRect( { 0.f, 0.f, 0.33f, 0.33f } );
	highlight->SetScalingMode( GUI::ScalingMode::AdjustBoth );
	highlight->SetRenderingMode( GUI::RenderingMode::Transparent );
	highlight->SetFile( "../src/contrib/Hyperscanning/HyperscanningTest/highlight.png" );
	highlight->SetZOrder( 11 );

}

void TicTacToe::SharedProcess( const GenericSignal& Input, GenericSignal& Output ) {
	board->Present();
	highlight->Present();
	for ( int i = 0; i < 9; i++ ) {
		tiles[ i ]->Present();
	}


	if ( State( "ClientNumber" ) == State( "turn" ) ) {

		int mx = State( "MousePosX" ) - 32768 - Parameter( "WindowLeft" );
		int my = State( "MousePosY" ) - 32768 - Parameter( "WindowTop" );
		bciout << "Mouse: " << mx << ", " << my;
		bciout << "Mouse: " << ((mx / Parameter("WindowWidth")) * 3) << ", " << ((my / Parameter("WindowHeight")) * 3);
		bciout << "Window: " << Parameter( "WindowWidth" ) << ", " << Parameter( "WindowHeight" );
		if ( mx < Parameter( "WindowWidth" ) && mx > 0 ){
			State( "x" ) = ( int ) floor( ( mx / Parameter( "WindowWidth" ) ) * 3 );
		}
		if ( my < Parameter( "WindowHeight" ) && my > 0 ) {
			State( "y" ) = ( int ) floor( ( my / Parameter( "WindowHeight" ) ) * 3 );
		}


		// if ( CheckKeyPress( "KeyDown", 32 ) ) { //Space
		if ( State( "MouseKeys" ) ) {
			long xflag = 1UL << ( 2 * ( State( "y" ) * 3 + State( "x" ) ) );
			long oflag = 1UL << ( 2 * ( State( "y" ) * 3 + State( "x" ) ) + 1 );

			if ( !( State( "grid" ) & xflag ) && !( State( "grid" ) & oflag ) ) {
				if ( State( "ClientNumber" ) == 0 ) {
					State( "grid" ) = State( "grid" ) | xflag;
					State( "turn" ) = 1;
				}

				else {
					State( "grid" ) = State( "grid" ) | oflag;
					State( "turn" ) = 0;
				}
			}
		}
	}

	long flag = 1UL;
	for ( int i = 0; i < 9; i++ ) {
		if ( State( "grid" ) & flag ) {
			tiles[ i ]->SetFile( "../src/contrib/Hyperscanning/HyperscanningTest/x.png" );
		}
		flag <<= 1;
		if ( State( "grid" ) & flag ) {
			tiles[ i ]->SetFile( "../src/contrib/Hyperscanning/HyperscanningTest/o.png" );
		}
		flag <<= 1;
	}

	long hwin = horizontal_win;
	long vwin = vertical_win;
	long dwin1 = diagonal_win_1;
	long dwin2 = diagonal_win_2;

	if ( ( State( "grid" ) & dwin1 ) == dwin1 || ( State( "grid" ) & dwin2 ) == dwin2 ) {
		State( "turn" ) = 3;
		winText->SetText( "X Wins" );
	}

	dwin1 <<= 1;
	dwin2 <<= 1;

	if ( ( State( "grid" ) & dwin1 ) == dwin1 || ( State( "grid" ) & dwin2 ) == dwin2 ) {
		State( "turn" ) = 3;
		winText->SetText( "O Wins" );
	}

	for ( int i = 0; i < 3; i++ ) {
		if ( ( State( "grid" ) & hwin ) == hwin || ( State( "grid" ) & vwin ) == vwin ) {
			State( "turn" ) = 3;
			winText->SetText( "X Wins" );
		}

		hwin <<= 1;
		vwin <<= 1;

		if ( ( State( "grid" ) & hwin ) == hwin || ( State( "grid" ) & vwin ) == vwin ) {
			State( "turn" ) = 3;
			winText->SetText( "O Wins" );
		}

		hwin <<= 5;
		vwin <<= 1;
	}

	int grid = State( "grid" );
	flag = 3UL;
	bool full = true;
	for ( int i = 0; i < 9; i += 1 ) {
		if ( !( grid & flag ) ) {
			full = false;
		}
		flag <<= 2;
	}
	if ( full ) {
		State( "turn" ) = 3;
		winText->SetText( "Cat's Game" );
	}

	highlight->SetObjectRect( { State("x") / 3.f, State("y") / 3.f, (State("x") + 1) / 3.f, (State("y") + 1) / 3.f } );


	if (State("turn") == 3) {
		if (State("MouseKeys")) { //Space
			Reset();
		}
	}
}

bool TicTacToe::CheckKeyPress( std::string event_name, int value ) {
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( event_name )( i ) == value ) {
			return true;
		}
	}
	return false;
}

void TicTacToe::Reset() {
	State( "turn" ) = 0;
	State( "grid" ) = 0;

	winText->SetText( "" );

	for ( int i = 0; i < 9; i++ ) {
		delete tiles[ i ];
		tiles[ i ] = new ImageStimulus( mrDisplay );
		int x = i % 3;
		int y = i / 3;
		tiles[ i ]->SetObjectRect( { x / 3.f, y / 3.f, x / 3.f + 0.33f, y / 3.f + 0.33f } );
		tiles[ i ]->SetScalingMode( GUI::ScalingMode::AdjustBoth );
		tiles[ i ]->SetRenderingMode( GUI::RenderingMode::Opaque );
		tiles[ i ]->SetZOrder( i + 2 );
	}
}
