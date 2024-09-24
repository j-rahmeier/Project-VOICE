// $Id: ExampleSourceSketch.ino 7896 2024-03-05 15:15:27Z jhill $
#include "SignalAcquisition.h" // Install the SignalAcquisition library using the library manager of your Arduino IDE.
#include "Keyhole.h"           // (The Keyhole library will also be installed, as a dependency.)

/*

 * To receive, over the serial port, a report of all exposed variables, send the string "?"
 * via the serial port monitor  (or "?\n" if using your own program - all commands will need
 * to be newline-terminated).
 * 
 * To turn on the LED and set the voltage high on the output pin, send::
 * 
 *     output = 1
 * 
 * To turn off the LED and set the voltage low on the output pin, send::
 * 
 *     output = 0
 * 
 * You can also change the values of `TTLInputPin` and `TTLOutputPin` on-the-fly in the
 * same way (but bear in mind that settings reconfigured this way will not be remembered
 * once power is lost).
 * 
 * When the voltage goes high on the *input* pin, the microcontroller will send the
 * string `TTLInput 1\n` over the serial port. When it goes low, it will send
 * `TTLInput 0\n`.
 * 
 * If you send the command `publish\n` then the sketch will reply with BCI2000 parameter
 * and/or event definitions.
 * 
 * Thanks to the `SignalAcquisition` library, this variant of the sketch additionally
 * has the ability to deliver binary signal data and can be used with BCI2000's
 * `SerialWidget` source module. To turn on sampling, set the variable `samplesPerSecond`
 * greater than zero (NB: it must correspond to an integer number of microseconds per
 * sample),  `samplesPerBlock` also greater than zero, and `sourcePins` non-empty (it
 * should be a space-limited string containing ascii representations of numeric pin
 * addresses). For example, to test without BCI2000 you would send the following lines
 * over the serial port::
 * 
 *     samplesPerSecond = 1000
 *     samplesPerBlock = 40
 *     sourcePins = "26 27"
 * 
 *     mute = 0
 * 
 * The BCI2000 SerialWidget module sends these lines for you, based on the `SamplingRate`,
 * `SampleBlockSize` and `SourceChPins` parameters, respectively. When these are
 * correctly configured, the sketch will deliver sample-blocks in the way SerialWidget
 * is hard-coded to expect: first a binary representation of `(uint16_t)1` is sent to
 * herald each block and to signal the sender's native endianity; then a line-ending;
 * then the packed `float32` sample values are sent (all channels for the first sample
 * of the block, then all channels for the second sample, and so on).
*/

#ifndef   LED_BUILTIN
# define  LED_BUILTIN 2  // works for ItsyBitsy, whose headers for some reason don't define this
#endif // LED_BUILTIN


int gInputPin  = 16; // default value (on the Pico, GP16 is the bottom right corner)
int gOutputPin = 15; // default value (on the Pico, GP15 is the bottom left corner)

int gMute = 1;
int gOutput = 0;
int gInput  = 0;

SignalAcquisition<float> gSignal;

void setup()
{
  if( gInputPin >= 0 )
    pinMode( gInputPin, INPUT );
  
  if( gOutputPin >= 0 )
  {
    pinMode( gOutputPin, OUTPUT );
    digitalWrite( gOutputPin, LOW );
  }

  Serial.begin(115200); // note that Teensy, Pico and others ignore this rate and use faster USB rates anyway;  but for actual Arduinos: 115200 is the max BAUD rate supported by the Arduino IDE's Serial Monitor 

  // Flicker the LED for a second to say hello:
  Keyhole::flicker( LED_BUILTIN, 50, 50, 1000 );

  gSignal.configure();
}

void loop()
{
  int newInput = 0;
  if( gInputPin >= 0 ) newInput = digitalRead( gInputPin );
  if( newInput != gInput )
  {
    static unsigned long sPulseStartTimeInMillis = 0;
    unsigned long pulseDurationInMillis;
    if( newInput ) sPulseStartTimeInMillis = millis();
    else pulseDurationInMillis = millis() - sPulseStartTimeInMillis;
    if( !gMute )                    // without the "mute" mechanism, you get a LOT of these messages
    {                               //  if the pin voltage happens to be floating, and that might disrupt
      Serial.print( "TTLInput " );  //  other communications (only a problem with some microcontrollers)
      Serial.println( newInput );
      if( !newInput )
      {
        Serial.print( "PulseDurationMsec " );
        Serial.print( pulseDurationInMillis );
        Serial.println( " 0" ); // optional third field of Event Descriptor (must be 0, if supplied) - makes the Event a transient with a duration of 1 sample
      }
      Serial.flush();
    }
    gInput = newInput;
  }
  
  unsigned int now = micros();
  if( !gMute ) gSignal.acquire( now ); // This method wraps up the following functionality:
  // 1 - clocking the sample (i.e. deciding whether it is time to read) at the requested
  //     samplesPerSecond rate
  // 2 - reading from each specified pin with analogRead() (unless you have subclassed
  //     the SignalAcquisition class and overridden its virtual readSample() method to
  //     behave differently).
  // 3 - buffering the samples until the requested samplesPerBlock have been gathered
  // 4 - writing the block header and raw block data to the serial port.
  
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

    keyhole.variable( "input", gInput, VARIABLE_READ_ONLY );
    
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
      keyhole << "Source:Microcontroller%20Sketch string SketchDescription= SerialWidgetADC%20example%20with%20TTL%20input/output%20capabilities % % % // description provided by the microcontroller (readonly)";

      // BCI2000 Event definitions (any line without an `=` sign will be interpreted as an event definition):
      keyhole << "TTLInput           1 0 0 0";
      keyhole << "PulseDurationMsec 15 0 0 0";

      // A blank line lets BCI2000 know to stop listening for definitions:
      keyhole << "";

      // NB: using `keyhole << whatever` requires Keyhole version 1.5.0+. It is slower than stringing together the equivalent Serial.print(); and
      //     Serial.println(); calls but it is much more convenient to write (and this section, the response to "publish\n", is not time-critical)
    }

    keyhole.end();
  }
}
