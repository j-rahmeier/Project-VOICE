// $Id: ExampleSourceSketchMPU6050.ino 7792 2024-01-17 18:30:04Z jhill $

/*
 * BCI2000-SerialWidget-compatible sketch for reading accelerometer and gyroscope readings from an MPU6050.
 * This involves subclassing the virtual base class SignalAcquisition that is provided by the SignalAcquisition
 * library. Since we read signal values in a way that is very different from the base SignalAcquisition class,
 * the subclass overrides the virtual methods preparePin() and readSample().
 *
 * `samplesPerSecond` should be 625 as this seems to be the maximum native speed of the device.
 *
 * `sourcePins` are not interpreted literally as pin addresses but rather as "0 1 2" -> accelerometer X Y and Z,
 * and "3 4 5" -> gyroscope X Y and Z.  Pin 6 will get you the digital TTL input - this is acquired as a channel
 * rather than a BCI2000 event/state variable, which is a divergence from the way the other BCI2000 example
 * sketches do things: we found that *any* additional serial communication beyond the 625 Hz data clock seems to
 * throw off the timing, so we cannot use the asynchronous BCI2000 "Event" mechanism.
 * Any other pin number will get you a flat zero signal.
 *
 * Teensy:  connect the accelerometer's SCL to pin 19 (8th row right),  and SDA to pin 18 (9th row right)
 * Pico:    connect the accelerometer's SCL to pin  5 (7th row left),   and SDA to pin  4 (6th row left)
 * Arduino UNO: don't even try it, you'll hit memory limitations causing some functionality to fail, and its serial communication is not fast enough anyway.
 *
 */

#include "SignalAcquisitionMPU6050.h" // defines the subclass; requires the SignalAcquisition library to be installed, via the library manager of your Arduino IDE
#include "Keyhole.h"                  // (The Keyhole library will also be installed, as a dependency.)


#ifndef   LED_BUILTIN
# define  LED_BUILTIN 2  // works for ItsyBitsy, whose headers for some reason don't define this
#endif // LED_BUILTIN


int gInputPin  = 16; // default value (on the Pico, GP16 is the bottom right corner)
int gOutputPin = 15; // default value (on the Pico, GP15 is the bottom left corner)

int gMute = 1;
int gOutput = 0;
/* TTL input is now stored in a member variable of the SignalAcquisitionMPU6050*/

SignalAcquisitionMPU6050 gSignal;

void setup()
{
  if( gInputPin >= 0 )
    pinMode( gInputPin, INPUT );
  
  if( gOutputPin >= 0 )
  {
    pinMode( gOutputPin, OUTPUT );
    digitalWrite( gOutputPin, LOW );
  }

  Serial.begin( 115200 ); // note that Teensy, Pico and others ignore this rate and use faster USB rates anyway;  but for actual Arduinos: 115200 is the max BAUD rate supported by the Arduino IDE's Serial Monitor 

  gSignal.begin();
  // Flicker the LED for a second to say hello:
  Keyhole::flicker( LED_BUILTIN, 100, 50, 1000 );
  gSignal.configure();
}

void loop()
{  
  unsigned int now = micros();
  if( gInputPin >= 0 ) gSignal.mDigitalInput = digitalRead( gInputPin ); /* note, we have removed the TTLInput event reporting, because this is now acquired as a signal channel (see comments above)*/
  if( !gMute ) gSignal.acquire( now );
  
  KEYHOLE keyhole( Serial);
  
  if( keyhole.begin( now ) )
  {
    gSignal.exposeVariables( keyhole ); // This exposes three variables for
    // SerialWidgetADC compatibility, corresponding to the BCI2000 parameters
    // SamplingRate, SampleBlockSize, and SourceChPins. Their respective keys are
    // "samplesPerSecond" (a float), "samplesPerBlock" (an int) and "sourcePins"
    // (a String containing the sequence of pin indices as space-delimited text).
    // The number of channels in the sample-block will be equal to the number of
    // pin indices in the String. Acquisition can be disabled by setting any of
    // these variables to illegal values.

    if( keyhole.variable( "mute", gMute ) )  // "mute=0\n" and "mute=1\n" can be used as start and stop commands
      gSignal.reset( now ); // this call is also for SerialWidgetADC purposes
      
    // The rest is the same as TTLExampleSketch

    keyhole.variable( "input", gSignal.mDigitalInput, VARIABLE_READ_ONLY );
    
    if( keyhole.variable( "output", gOutput ) )
    {
      if( gOutputPin >= 0 ) digitalWrite( gOutputPin, gOutput ? HIGH : LOW );
      digitalWrite( LED_BUILTIN, gOutput ? HIGH : LOW );
    }
    
    if( keyhole.variable( "TTLInputPin",  gInputPin  ) && gInputPin  >= 0 )
    { // Since we have declared this as a parameter, BCI2000 will set its value during Initialize()
      pinMode( gInputPin, INPUT );
    }
      
    if( keyhole.variable( "TTLOutputPin", gOutputPin ) && gOutputPin >= 0 )
    { // Since we have declared this as a parameter, BCI2000 will set its value during Initialize()
      pinMode( gOutputPin, OUTPUT );
      digitalWrite( gOutputPin, gOutput ? HIGH : LOW );
    }

    if( keyhole.command( "publish" ) ) // The string "publish\n" will prompt the sketch to reply with the following definitions.
    {                                  // The BCI2000's SerialInterface will send it if you start it with --PublishCommand=publish\n

      // BCI2000 Parameter definitions (any line containing an `=` sign will be interpreted as a parameter definition):
      keyhole << "Source:Microcontroller%20Sketch int    TTLInputPin=  " << gInputPin  << " % % % // microcontroller pin number for TTL voltage input";
      keyhole << "Source:Microcontroller%20Sketch int    TTLOutputPin= " << gOutputPin << " % % % // microcontroller pin number for TTL voltage output";
      keyhole << "Source:Microcontroller%20Sketch string SketchDescription= MPU6050%20accelerometer/gyroscope%20acquisition%20module % % % // description provided by the microcontroller (readonly)";

      // A blank line lets BCI2000 know to stop listening for definitions:
      keyhole << "";

      // NB: using `keyhole << whatever` requires Keyhole version 1.5.0+. It is slower than stringing together the equivalent Serial.print(); and
      //     Serial.println(); calls but it is much more convenient to write (and this section, the response to "publish\n", is not time-critical)
    }

    keyhole.end();
  }
}
