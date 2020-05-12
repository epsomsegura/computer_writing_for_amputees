//// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
//// is used in I2Cdev.h
//#include "Wire.h"
// 
//// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
//// for both classes must be in the include path of your project
//#include "I2Cdev.h"
//#include "MPU6050.h"
// 
//// class default I2C address is 0x68
//// specific I2C addresses may be passed as a parameter here
//// AD0 low = 0x68 (default for InvenSense evaluation board)
//// AD0 high = 0x69
//MPU6050 accelgyro;
// 
//int16_t ax, ay, az;
//int16_t gx, gy, gz;
// 
//#define LED_PIN 13
//bool blinkState = false;
// 
//void setup() {
//  // join I2C bus (I2Cdev library doesn't do this automatically)
//  Wire.begin();
//   
//  // initialize serial communication
//  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
//  // it's really up to you depending on your project)
//  Serial.begin(57600);
//  while(!Serial){;}
//  pinMode(4,OUTPUT);
//  digitalWrite(4,HIGH);
//   
//  // initialize device
//  Serial.println("Initializing I2C devices...");
//  accelgyro.initialize();
//  digitalWrite(4,LOW);
//   
//  // verify connection
//  Serial.println("Testing device connections...");
//  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
//   
//  // configure Arduino LED for
//pinMode(LED_PIN, OUTPUT);
//}
// 
//void loop() {
//  // read raw accel/gyro measurements from device
//  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//   
//  // these methods (and a few others) are also available
//  //accelgyro.getAcceleration(&ax, &ay, &az);
//  //accelgyro.getRotation(&gx, &gy, &gz);
//   
//  // display tab-separated accel/gyro x/y/z values
//  Serial.print("a/g:\t");
//  Serial.print(ax); Serial.print("\t");
//  Serial.print(ay); Serial.print("\t");
//  Serial.print(az); Serial.print("\t");
//  Serial.print(gx); Serial.print("\t");
//  Serial.print(gy); Serial.print("\t");
//  Serial.println(gz);
//   
//  // blink LED to indicate activity
//  blinkState = !blinkState;
//  digitalWrite(LED_PIN, blinkState);
//}



// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//
bool blinkState = false;
#include <Wire.h>


void setup()
{
  Wire.begin();

  Serial.begin(57600);
  while(!Serial){;}
  Serial.println("\nI2C Scanner");
  pinMode(3, OUTPUT);
}


void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    Serial.println("Prueba "+String(address));
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    Serial.println("Paso_1");
    error = Wire.endTransmission();
    Serial.println("Paso_2");

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
        blinkState = !blinkState;
        digitalWrite(3, blinkState);
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }   
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
