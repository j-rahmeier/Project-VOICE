#include "Shapes.h"
#include "ApplicationWindow.h"
#include "HyperscanningApplicationBase.h"
#include "TextField.h"

class Pong : public HyperscanningApplicationBase {
	public:
		Pong();

		void SharedPublish() override;

		void SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void SharedProcess( const GenericSignal& Input, GenericSignal& Output ) override;
		void SharedStartRun() override;
		void SharedAutoConfig( const SignalProperties& Input ) override;

		bool CheckKeyPress( std::string event_name, int value );

	private:
		ApplicationWindow& mrDisplay;
		RectangularShape* player1Paddle;
		RectangularShape* player2Paddle;
		RectangularShape* ball;
		TextField* p1s;
		TextField* p2s;
		TextField* p1r;
		TextField* p2r;

		int pHeight;
		int bSize;
		int bSpeed;

		int x;
		int y;
		int lastX;
		int lastY;

		int pX = 50;
		int pW = 10;
		int mAngle;

		int pV = 30;

		bool up = false;
		bool down = false;
};
