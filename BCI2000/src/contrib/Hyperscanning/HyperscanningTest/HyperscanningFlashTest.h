#include "Shapes.h"
#include "TextField.h"
#include "ApplicationWindow.h"
#include "ImageStimulus.h"
#include "ApplicationBase.h"
#include "HyperscanningApplicationBase.h"

class HyperscanningFlashTest : public HyperscanningApplicationBase {
	public:
		HyperscanningFlashTest();

		void SharedPublish() override;

		void SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void SharedProcess( const GenericSignal& Input, GenericSignal& Output ) override;

		bool CheckKeyPress( std::string, int );

		void Reset();

	private:
		ApplicationWindow& mrDisplay;

		RectangularShape* square;

		int counter;

};
