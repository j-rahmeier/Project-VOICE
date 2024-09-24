#include "HyperscanningPong.h"
#include "Shapes.h"
#include <limits>
#include <cmath>


//Shared States
//BallX 32
//BallY 32
//BallA 16
//Player1Y 32
//Player2Y 32
//Player1Ready 1
//Player2Ready 1
//GamePhase 8
//Player1Score 8
//Player2Score 8
//
//Game Phases
//0 : Instruction
//1 : Game
//2 : Between Rounds

RegisterFilter( Pong, 3 );

Pong::Pong() : player1Paddle( NULL ), player2Paddle( NULL ), ball( NULL ), mrDisplay( Window() ) {
}

void Pong::SharedPublish() {
	BEGIN_PARAMETER_DEFINITIONS
		"Application:Pong int PaddleHeight= auto auto auto auto // Height of the paddle",
		"Application:Pong int BallSize= auto auto auto auto // Size of the ball",
		"Application:Pong int BallSpeed= auto auto auto auto // Speed of the ball",
		"Application:Pong int MaxAngle= auto auto auto auto // Maximum angle of the ball when bouncing off of paddles",

	END_PARAMETER_DEFINITIONS
}

void Pong::SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const {
	State( "BallX" );
	State( "BallY" );
	State( "BallA" );
	State( "Player1Y" );
	State( "Player2Y" );
	State( "Player1Ready" );
	State( "Player2Ready" );
	State("Player1Score");
	State("Player2Score");
	State( "GamePhase" );
	State("ClientNumber");
	State( "KeyDown" );
	State( "KeyUp" );
	State( "MousePosY" );
	Parameter( "SampleBlockSize" );

	Parameter( "PaddleHeight" );
	Parameter( "BallSize" );
	Parameter( "BallSpeed" );
	Parameter("MaxAngle");
	Parameter( "WindowTop" );
	Parameter( "WindowHeight" );
}

void Pong::SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) {
	

	player1Paddle = new RectangularShape( mrDisplay );
	player1Paddle->SetFillColor( RGBColor::Teal );
	player1Paddle->SetVisible( true );

	player2Paddle = new RectangularShape( mrDisplay );
	player2Paddle->SetFillColor( RGBColor::Red );
	player2Paddle->SetVisible( true );

	ball = new RectangularShape( mrDisplay );
	ball->SetFillColor( RGBColor::Black );
	ball->SetVisible( true );

	p1s = new TextField( mrDisplay );
	p1s->SetObjectRect( { 0.125f, 0.125f, 0.5f, 0.5f } );
	p1s->SetTextHeight( 0.15f );
	p1s->SetTextColor( RGBColor::Gray );
	p1s->Show();

	p1r = new TextField( mrDisplay );
	p1r->SetObjectRect( { 0.125f, 1 - 0.125f, 0.5f, 1 - 0.5f } );
	p1r->SetTextHeight( 0.15f );
	p1r->SetTextColor( RGBColor::Gray );
	p1r->Show();


	p2s = new TextField( mrDisplay );
	p2s->SetObjectRect( { 1 - 0.5f, 1 - 0.5f, 1 - 0.125f, 1 - 0.125f } );
	p2s->SetTextHeight( 0.15f );
	p2s->SetTextColor( RGBColor::Gray );
	p2s->Show();

	p2r = new TextField( mrDisplay );
	p2r->SetObjectRect( { 1 - 0.5f, 0.5f, 1 - 0.125f, 0.125f } );
	p2r->SetTextHeight( 0.15f );
	p2r->SetTextColor( RGBColor::Gray );
	p2r->Show();
}

void Pong::SharedStartRun() {
	State( "BallX" ) = 500;
	State( "BallY" ) = 500;
	State( "Player1Y" ) = 200;
	State( "Player2Y" ) = 800;
	State( "BallA" ) = 0;
	State( "GamePhase" ) = 2;

	pHeight = Parameter("PaddleHeight");
	bSize = Parameter("BallSize");
	bSpeed = Parameter("BallSpeed");
	mAngle = Parameter("MaxAngle");

	down = false;
	up = false;
}

void Pong::SharedAutoConfig( const SignalProperties& Input ) {
	Parameters->Load((std::string)Parameter("ParameterPath"));
}

void Pong::SharedProcess( const GenericSignal& Input, GenericSignal& Output ) {
	if ( State( "GamePhase" ) == 1 ) {
		if (State("ClientNumber") == 0) {
			State("BallX") = ( int ) ( State("BallX") + bSpeed * cos(((State("BallA")) / 180.f) * 3.14159) );
			State("BallY") = ( int ) ( State("BallY") + bSpeed * sin(((State("BallA")) / 180.f) * 3.14159) );
			x = State("BallX");
			y = State("BallY");

            int xSpeed = std::abs( bSpeed * cos(((State("BallA")) / 180.f) * 3.14159) );
            int ySpeed = std::abs( bSpeed * sin(((State("BallA")) / 180.f) * 3.14159) );

			if ( State("BallY") + ySpeed >= 1000 - bSize ) {
				State("BallA") = ( int )( 360 - State( "BallA" ) );
				State("BallY") = ( int )( 1000 - bSize - ySpeed - 1 );
			} else if ( State( "BallY" ) <= bSize + ySpeed ) {
				State( "BallA" ) = ( int )( 360 - State( "BallA" ) );
				State("BallY") = ( int ) ( bSize + ySpeed + 1 );

			}

			if ( State( "BallX" ) + bSize + xSpeed > 1000 - pX - pW && State( "BallX" ) < 1000 - pX + pW + bSize + xSpeed && State( "BallY" ) + bSize + pHeight > State( "Player2Y" ) && State( "BallY" ) < State( "Player2Y" ) + pHeight + bSize ) {
				
				float yInt = ( int ) State("Player2Y") - ( int ) State("BallY");

				yInt /= ( float ) pHeight;

				yInt *= ( float ) mAngle;

				yInt = 180 + yInt;
				State("BallA") = (int)yInt;
				State( "BallX" ) =  1000 - pX - pW - bSize - xSpeed;
			}

			if ( State( "BallX" ) + bSize + xSpeed > pX - pW && State( "BallX" ) < pX + pW + bSize + xSpeed && State( "BallY" ) + bSize > State( "Player1Y" ) - pHeight  && State( "BallY" ) < State( "Player1Y" ) + pHeight + bSize ) {
				float yInt = ( int ) State("Player1Y") - ( int ) State("BallY");
				yInt /= ( float ) pHeight;
				yInt *= ( float ) mAngle;
				State("BallA") = (int)( 360 - yInt) % 360;
				State( "BallX" ) = pX + pW + bSize + xSpeed;
			}

			if ( State( "BallX" ) + bSize + xSpeed > 1000 ) {
				bciout << "Player 1 Scored";
				State( "BallX" ) = 500;
				State( "BallA" ) = 0;
				State( "GamePhase" ) = 2;
				State( "Player1Score" ) = State( "Player1Score" ) + 1;
			} else if ( State( "BallX" ) < bSize + xSpeed ) {
				bciout << "Player 2 Scored";
				State( "BallX" ) = 500;
				State( "BallA" ) = 180;
				State( "GamePhase" ) = 2;
				State( "Player2Score" ) = State( "Player2Score" ) + 1;
			}
		} else {
			if ( lastX != State( "BallX" ) ) {
				lastX = State( "BallX" );
				x = State( "BallX" );
			}
			if ( lastY != State( "BallY" ) ) {
				lastY = State( "BallY" );
				y = State( "BallY" );
			}

			x += bSpeed * cos(((State("BallA")) / 180.f) * 3.14159);
			y += bSpeed * sin(((State("BallA")) / 180.f) * 3.14159);
		}

		// if (CheckKeyPress("KeyDown", 38)) { //Up
		// 	up = true;
		// }
		// else if (CheckKeyPress("KeyUp", 38)) {
		// 	up = false;
		// }
		// if ( up ) {
		// 	if ( State( "ClientNumber" ) == 0 ) {
		// 		if ( State( "Player1Y" ) > pHeight + pV )
		// 			State( "Player1Y" ) = State( "Player1Y" ) - pV;
		// 	}
		// 	else {
		// 		if ( State( "Player2Y" ) > pHeight + pV )
		// 		State( "Player2Y" ) = State( "Player2Y" ) - pV;
		// 	}
		// }

		// if (CheckKeyPress("KeyDown", 40)) {
		// 	down = true;
		// }
		// else if (CheckKeyPress("KeyUp", 40)) { //Down
		// 	down = false;
		// }
		// if (down) {
		// 	if ( State( "ClientNumber" ) == 0 ) {
		// 		if ( State( "Player1Y" ) < 1000 - pV - pHeight )
		// 			State( "Player1Y" ) = State( "Player1Y" ) + pV;
		// 	}
		// 	else {
		// 		if ( State( "Player2Y" ) < 1000 - pV - pHeight )
		// 			State( "Player2Y" ) = State( "Player2Y" ) + pV;
		// 	}
		// }

		int y = 1000 * ( State( "MousePosY" ) - 32768 - Parameter( "WindowTop" ) ) / Parameter( "WindowHeight" );
		if ( y < pHeight ) 
			y = pHeight;
		if ( y > 1000 - pHeight )
			y = 1000 - pHeight;

		if ( State( "ClientNumber" )  == 0 ) {
			State( "Player1Y" ) = y;
		} else {
			State( "Player2Y" ) = y;
		}

	}

	if ( State( "GamePhase" ) == 2 ) {
		if ( State( "Player1Ready" ) )
			p1r->SetText( "P1 Ready" );
		else
			p1r->SetText( "P1 Press Space Bar");
		if ( State( "Player2Ready" ) )
			p2r->SetText( "P2 Ready" );
		else
			p2r->SetText( "P2 Press Space Bar");
		if ( CheckKeyPress( "KeyDown", 32 ) == 1 ) { //Space
			if ( State( "ClientNumber" ) == 0 )
				State( "Player1Ready" ) = 1;
			else
				State( "Player2Ready" ) = 1;
		}

		if ( State( "Player1Ready" ) == 1 && State( "Player2Ready" ) == 1 ) {
			State( "GamePhase" ) = 1;
			State( "Player1Ready" ) = 0;
			State( "Player2Ready" ) = 0;
			down = false;
			up = false;
		}
	} else {

		p1r->SetText( "" );
		p2r->SetText( "" );
	}


	GUI::Rect rect = { ( x / 1000.f ) - ( bSize / 1000.f ) , ( y / 1000.f ) - ( bSize / 1000.f ), ( x / 1000.f ) + ( bSize / 1000.f ), ( y / 1000.f ) + ( bSize / 1000.f ) };
	ball->SetObjectRect( rect );

	rect = { ((float)pX - pW) / 1000.f, (State("Player1Y") / 1000.f) - (pHeight / 1000.f), ((float)pX + pW) / 1000.f, (State("Player1Y") / 1000.f) + (pHeight / 1000.f) };
	player1Paddle->SetObjectRect( rect );

	rect = { 1.f - (((float)pX - pW) / 1000.f), (State("Player2Y") / 1000.f) - (pHeight / 1000.f), 1.f - (((float)pX + pW) / 1000.f), (State("Player2Y") / 1000.f) + (pHeight / 1000.f) };
	player2Paddle->SetObjectRect( rect );

	p1s->SetText( "P1: " + std::to_string( State( "Player1Score" ) ) );
	p2s->SetText( "P2: " + std::to_string( State( "Player2Score" ) ) );

}

bool Pong::CheckKeyPress( std::string event_name, int value ) {
	for ( unsigned int i = 0; i < Parameter( "SampleBlockSize" ); i++ ) {
		if ( State( event_name )( i ) == value ) {
			return true;
		}
	}
	return false;
}
