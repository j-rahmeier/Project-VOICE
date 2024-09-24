#include "Wire.h" // part of the standard library for most boards

#define USE_MPU6050_LIBRARY 0
#if     USE_MPU6050_LIBRARY
# include "MPU6050.h" // by Jeff Rowberg, originally part of https://github.com/jrowberg/i2cdevlib and later released for the IDE as https://github.com/ElectronicCats/mpu6050
#endif

#include "SignalAcquisition.h" // Install the SignalAcquisition library using the library manager of your Arduino IDE.

class SignalAcquisitionMPU6050 : public SignalAcquisition<float>
{
  public:

#   if USE_MPU6050_LIBRARY
      MPU6050 mDevice;
#   endif

    SignalAcquisitionMPU6050( Stream & stream=Serial, unsigned int maxPins=7 )
    : SignalAcquisition<float>( stream, maxPins ) { }

    ~SignalAcquisitionMPU6050() {}

    void begin( void )
    {
      Wire.setClock( 400000 ); // max. data rate from the MPU6050 datasheet
      Wire.begin();

#     if USE_MPU6050_LIBRARY
        mDevice.initialize();
#     else // if we're not using the MPU6050 library, do it by hand using instructions adapted from https://www.youtube.com/watch?v=yhz3bRQLvBY
        delay(250);  // give the MPU6050 time to start up communication
        Wire.beginTransmission( 0x68 ); // 0x68 is the MPU6050's default I2C address
        Wire.write( 0x6b ); // target the MPU6050's register at address 0x6B
        Wire.write( 0x00 ); // send 0 to that register to turn power on
        Wire.endTransmission();
#     endif

      this->samplesPerSecond = 625; // empirically this seems to be the MPU6050's native rate (it won't deliver readings any faster than this even if you set it higher)
      this->samplesPerBlock  = 0; // change this to something >0 to enable acquisition
      this->sourcePins = "0 1 2 3 4 5 6";
    }

    virtual void preparePin( int pinNumber ) { } // do nothing---overrides the base class's default pinMode(pinNumber, INPUT) behavior
  
    int16_t mAccelX, mAccelY, mAccelZ;
    int16_t  mGyroX,  mGyroY,  mGyroZ;
    int16_t  mDigitalInput;

    virtual void readSample()
    {
#     if USE_MPU6050_LIBRARY
        mDevice.getMotion6( &mAccelX, &mAccelY, &mAccelZ, &mGyroX, &mGyroY, &mGyroZ );
#     else // if we're not using the MPU6050 library, do it by hand using instructions adapted from https://www.youtube.com/watch?v=yhz3bRQLvBY
        Wire.beginTransmission( 0x68 );
        Wire.write( 0x3b ); // send the address of the register where accel + gyro data begin
        Wire.endTransmission();
        Wire.requestFrom( 0x68, 14 ); // request 14 bytes starting at the aforementioned register
        mAccelX = ( Wire.read() << 8 ) | Wire.read();
        mAccelY = ( Wire.read() << 8 ) | Wire.read();
        mAccelZ = ( Wire.read() << 8 ) | Wire.read();
        Wire.read(); Wire.read(); // discard the middle two bytes
        mGyroX  = ( Wire.read() << 8 ) | Wire.read();
        mGyroY  = ( Wire.read() << 8 ) | Wire.read();
        mGyroZ  = ( Wire.read() << 8 ) | Wire.read();
#     endif

      for( unsigned int iPin = 0; iPin < mMaxPins && mSourcePinAddresses[ iPin ] >= 0; iPin++ )
      {
        switch( mSourcePinAddresses[ iPin ] )
        {
          case  0: *mBufferCursor++ = mAccelX; break;
          case  1: *mBufferCursor++ = mAccelY; break;
          case  2: *mBufferCursor++ = mAccelZ; break;
          case  3: *mBufferCursor++ =  mGyroX; break;
          case  4: *mBufferCursor++ =  mGyroY; break;
          case  5: *mBufferCursor++ =  mGyroZ; break;
          case  6: *mBufferCursor++ =  mDigitalInput; break;
          default: *mBufferCursor++ = 0.; break;
        }
      }
    }

};
