#include "Shapes.h"
#include "TextField.h"
#include "ApplicationWindow.h"
#include "ApplicationBase.h"

class HyperscanningPongTest : public ApplicationBase {
	public:
		HyperscanningPongTest();

		void Publish() override;

		void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void Process( const GenericSignal& Input, GenericSignal& Output ) override;

		bool CheckKeyPress( std::string, int );

	private:
		ApplicationWindow& mrDisplay;
		RectangularShape* player1Paddle;
		RectangularShape* player2Paddle;
		RectangularShape* ball;

		TextField* player1Score;
		TextField* player2Score;
		int c;
};
