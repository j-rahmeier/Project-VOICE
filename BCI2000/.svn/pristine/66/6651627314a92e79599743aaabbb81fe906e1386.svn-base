#include "Shapes.h"
#include "ApplicationWindow.h"
#include "ApplicationBase.h"
#include "HyperscanningApplicationBase.h"

class HyperscanningColorChangeTest : public HyperscanningApplicationBase {
	public:
		HyperscanningColorChangeTest();

		void SharedPublish() override;

		void SharedPreflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void SharedInitialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void SharedProcess( const GenericSignal& Input, GenericSignal& Output ) override;

	private:
		ApplicationWindow& mrDisplay;
		RectangularShape* mpRect;
		int c;
};
