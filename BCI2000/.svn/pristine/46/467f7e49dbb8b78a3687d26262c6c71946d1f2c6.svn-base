#include "Shapes.h"
#include "TextField.h"
#include "ApplicationWindow.h"
#include "ImageStimulus.h"
#include "HyperscanningApplicationBase.h"

class TicTacToe : public HyperscanningApplicationBase {
	public:
		TicTacToe();

		void SharedPublish() override;

		void SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void SharedProcess( const GenericSignal& Input, GenericSignal& Output ) override;
		void SharedStartRun() override;

		bool CheckKeyPress( std::string, int );

		void Reset();

	private:
		ApplicationWindow& mrDisplay;

		ImageStimulus* board;
		ImageStimulus** tiles;
		ImageStimulus* highlight;

		TextField* winText;

		long horizontal_win = 21;
		long vertical_win = 4161;
		long diagonal_win_1 = 65793;
		long diagonal_win_2 = 4368;

		//TextField* player1Score;
		//TextField* player2Score;
		//int c;
};
