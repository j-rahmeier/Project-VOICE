#include "Shapes.h"
#include "TextField.h"
#include "ApplicationWindow.h"
#include "ImageStimulus.h"
#include "ApplicationBase.h"

class Hyperscanning1PFlash : public ApplicationBase {
	public:
		Hyperscanning1PFlash();

		void Publish() override;

		void Preflight( const SignalProperties& Input, SignalProperties& Output ) const override;
		void Initialize( const SignalProperties& Input, const SignalProperties& Output ) override;
		void Process( const GenericSignal& Input, GenericSignal& Output ) override;

		bool CheckKeyPress( std::string, int );

		void Reset();

	private:
		ApplicationWindow& mrDisplay;

		RectangularShape* square;
		RectangularShape* square2;

};
