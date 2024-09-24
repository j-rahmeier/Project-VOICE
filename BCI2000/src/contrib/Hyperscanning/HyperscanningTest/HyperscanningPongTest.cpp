#include "HyperscanningPongTest.h"
#include "Shapes.h"
#include "TextField.h"
#include <limits>
#include "BCIEvent.h"
#include <string>


//Shared States
//BallX 32
//BallY 32
//BallVX 8
//BallVY 8
//Player1Y 32
//Player2Y 32
//Player1Ready 8
//Player2Ready 8
//GamePhase 8
//Player1Score 8
//Player2Score 8
//
//Game Phases
//0 : Instruction
//1 : Game
//2 : Between Rounds

RegisterFilter( HyperscanningPongTest, 3 );

HyperscanningPongTest::HyperscanningPongTest() : player1Paddle( NULL ), player2Paddle( NULL ), ball( NULL ), player1Score( NULL ), player2Score( NULL ), mrDisplay( Window() ) {
}

void HyperscanningPongTest::Publish() {
}

void HyperscanningPongTest::Preflight( const SignalProperties& Input, SignalProperties& Output ) const {
	State( "BallX" );
	State( "BallY" );
	State( "BallVX" );
	State( "BallVY" );
	State( "Player1Y" );
	State( "Player2Y" );
	State( "Player1Ready" );
	State( "Player2Ready" );
	State( "GamePhase" );
	State("ClientNumber");
	State( "KeyDown" );
	Parameter( "SampleBlockSize" );
	State("Player1Score");
	State("Player2Score");
}

void HyperscanningPongTest::Initialize( const SignalProperties& Input, const SignalProperties& Output ) {
	ApplicationBase::Initialize( Input, Output );

	player1Paddle = new RectangularShape( mrDisplay );
	player1Paddle->SetFillColor( RGBColor::Teal );
	player1Paddle->SetVisible( true );

	player2Paddle = new RectangularShape( mrDisplay );
	player2Paddle->SetFillColor( RGBColor::Red );
	player2Paddle->SetVisible( true );

	ball = new RectangularShape( mrDisplay );
	ball->SetFillColor( RGBColor::Black );
	ball->SetVisible( true );

	player1Score = new TextField( mrDisplay );
	player1Score->SetVisible( true );
	player1Score->SetDisplayRect( { 0.2f, 0.2f, 0.3f, 0.3f } );

	player2Score = new TextField( mrDisplay );
	player2Score->SetVisible( true );
	player2Score->SetDisplayRect( { 0.7f, 0.2f, 0.8f, 0.3f });
	State( "Player1Y" ) = 200;
	State( "Player2Y" ) = 800;
	State( "BallVX" ) = 493;
	State( "BallVY" ) = 497;
	State( "GamePhase" ) = 2;
}

void HyperscanningPongTest::Process( const GenericSignal& Input, GenericSignal& Output ) {
	if ( State( "GamePhase" ) == 1 ) {
		if ( State( "ClientNumber" ) == 0 ) {
			State( "BallX" ) = State( "BallX" ) + State( "BallVX" ) - 500;
			State( "BallY" ) = State( "BallY" ) + State( "BallVY" ) - 500;

			if ( State( "BallY" ) > 950 || State( "BallY" ) < 50 ) {
				State( "BallVY" ) = 1000 - State( "BallVY" );
			}

			if ( State( "BallX" ) + 50 > 875 && State( "BallX" ) - 50 < 925 && State( "BallY" ) + 50 > State( "Player2Y" ) - 100 && State( "BallY" ) - 50 < State( "Player2Y" ) + 100 ) {
				State( "BallVX" ) = 1000 - State( "BallVX" );
				State( "BallX" ) = 875 - 50;
			}

			if ( State( "BallX" ) + 50 > 75  && State( "BallX" ) - 50 < 125 && State( "BallY" ) + 50 > State( "Player1Y" ) - 100 && State( "BallY" ) - 50 < State( "Player1Y" ) + 100 ) {
				State( "BallVX" ) = 1000 - State( "BallVX" );
				State( "BallX" ) = 125 + 50;
			}

			if ( State( "BallX" ) + 50 > 1000 ) {
				State( "BallX" ) = 500;
				State( "BallVY" ) = State( "BallVY" ) + 1;
				State( "GamePhase" ) = 2;
				State( "Player1Score" ) = State( "Player1Score" ) + 1;
				player1Score->SetText( std::to_string( ( int )State( "Player1Score" ) ) );
				player2Score->SetText( std::to_string( ( int )State( "Player2Score" ) ) );
			}

			if ( State( "BallX" ) - 50 < 0 ) {
				State( "BallX" ) = 500;
				State( "BallVY" ) = State( "BallVY" ) + 1;
				State( "GamePhase" ) = 2;
				State( "Player2Score" ) = State( "Player2Score" ) + 1;
				player1Score->SetText(std::to_string((int)State("Player1Score")));
				player2Score->SetText(std::to_string((int)State("Player2Score")));
			}
		}

		if ( CheckKeyPress( "KeyDown", 38 ) ) { //Up
			if ( State( "ClientNumber" ) == 0 )
				State( "Player1Y" ) = State( "Player1Y" ) - 30;
			else 
				State( "Player2Y" ) = State( "Player2Y" ) - 30;
		}

		if ( CheckKeyPress( "KeyDown", 40 ) ) { //Down
			if ( State( "ClientNumber" ) == 0 )
				State( "Player1Y" ) = State( "Player1Y" ) + 30;
			else 
				State( "Player2Y" ) = State( "Player2Y" ) + 30;
		}
	}

	if ( State( "GamePhase" ) == 2 ) {
		if ( CheckKeyPress( "KeyDown", 32 ) ) { //Space
			if ( State( "ClientNumber" ) == 0 )
				State( "Player1Ready" ) = 1;
			else
				State( "Player2Ready" ) = 1;
		}

		if ( State( "Player1Ready" ) == 1 && State( "Player2Ready" ) == 1 ) {
			State( "GamePhase" ) = 1;
			State( "Player1Ready" ) = 0;
			State( "Player2Ready" ) = 0;
			player1Score->SetText(std::to_string((int)State("Player1Score")));
			player2Score->SetText(std::to_string((int)State("Player2Score")));
		}
	}



	bciout << "Ball: (" << State("BallX") << ", " << State("BallY") << ")";
	bciout << "Player1: " << State("Player1Y");

	GUI::Rect rect = { ( State( "BallX" ) / 1000.f ) - 0.05, ( State( "BallY" ) / 1000.f ) - 0.05, ( State( "BallX" ) / 1000.f ) + 0.05, ( State( "BallY" ) / 1000.f ) + 0.05 };
	ball->SetObjectRect( rect );

	rect = { 0.075f, ( State( "Player1Y" ) / 1000.f ) - 0.1f, 0.125f, ( State( "Player1Y" ) / 1000.f ) + 0.1f };
	player1Paddle->SetObjectRect( rect );

	rect = { 0.875f, ( State( "Player2Y" ) / 1000.f ) - 0.1f, 0.925f, ( State( "Player2Y" ) / 1000.f ) + 0.1f };
	player2Paddle->SetObjectRect( rect );

	//bciwarn << "Color: " << State( "Color" );
	//mpRect->SetFillColor( State( "Color" ) ? RGBColor::Teal : RGBColor::Red );
	//while( SDL_PollEvent( &e ) != 0 ) {
	//	if ( e.type == SDL_MOUSEBUTTONDOWN ) {
	//		State( "Color" ) = !State( "Color" );
	//	}
	//}
}

bool HyperscanningPongTest::CheckKeyPress( std::string event_name, int value ) {
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( event_name )( i ) == value ) {
			return true;
		}
	}
	return false;
}
