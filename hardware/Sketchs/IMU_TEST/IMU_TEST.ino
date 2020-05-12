// Librerias I2C para controlar el mpu6050
// la libreria MPU6050.h necesita I2Cdev.h, I2Cdev.h necesita Wire.h
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

// La dirección del MPU6050 puede ser 0x68 o 0x69, dependiendo 
// del estado de AD0. Si no se especifica, 0x68 estará implicito
MPU6050 sensor;

// Bandera pintar datos
int flag = 0;
String motion="";

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;

//Variables usadas por el filtro pasa bajos
long f_ax,f_ay, f_az;
int p_ax, p_ay, p_az;
long f_gx,f_gy, f_gz;
int p_gx, p_gy, p_gz;
int counter=0;

//Valor de los offsets
int ax_o,ay_o,az_o;
int gx_o,gy_o,gz_o;

void setup() {
  Serial.begin(57600);   //Iniciando puerto serial
  while(!Serial){;}
  Wire.begin();           //Iniciando I2C  
  sensor.initialize();    //Iniciando el sensor
  Serial.println("INIT TEST");

  if (sensor.testConnection()) {Serial.println("Sensor iniciado correctamente");}
  else{ Serial.println("ERROR SENSOR");}

  // Leer los offset los offsets anteriores
  ax_o=sensor.getXAccelOffset();
  ay_o=sensor.getYAccelOffset();
  az_o=sensor.getZAccelOffset();
  gx_o=sensor.getXGyroOffset();
  gy_o=sensor.getYGyroOffset();
  gz_o=sensor.getZGyroOffset();
  
  Serial.println("Offsets:");
  Serial.print(ax_o); Serial.print("\t"); 
  Serial.print(ay_o); Serial.print("\t"); 
  Serial.print(az_o); Serial.print("\t"); 
  Serial.print(gx_o); Serial.print("\t"); 
  Serial.print(gy_o); Serial.print("\t");
  Serial.print(gz_o); Serial.print("\t");
  Serial.println("\n\nEnvie cualquier caracter para empezar la calibracion\n\n");  
  // Espera un caracter para empezar a calibrar
  while (true){if (Serial.available()) break;}  
  Serial.println("Calibrando, no mover IMU");  
  
}


//
//
//#include <MPU9250_asukiaaa.h>
//MPU9250_asukiaaa mySensor;
//float aX, aY, aZ, aSqrt;
//
//void setup() {
//  Wire.begin();
//  mySensor.setWire(&Wire);
//  mySensor.beginAccel();
//}
//
//void loop() {
//  uint8_t sensorId;
//  if (mySensor.readId(&sensorId) == 0) {
//    Serial.println("sensorId: " + String(sensorId));
//  } else {
//    Serial.println("Cannot read sensorId");
//  }
//
//  if (mySensor.accelUpdate() == 0) {
//    mySensor.accelUpdate();
//    aX = mySensor.accelX();
//    aY = mySensor.accelY();
//    aZ = mySensor.accelZ();
//    aSqrt = mySensor.accelSqrt();
//  }
//  else {
//    Serial.println("Cannod read accel values");
//  }
//  // Do what you want
//
//  Serial.println("Lecturas del sensor MPU9250\n");
//  Serial.print("Acelaración SQRT: "); Serial.println(aSqrt);
//  Serial.print("aX:");Serial.print(aX);Serial.print("|\t");
//  Serial.print("aY:");Serial.print(aY);Serial.print("|\t");
//  Serial.print("aZ:");Serial.print(aZ);Serial.print("|\t");
//  Serial.println("");
//  delay(100);
//}



//
//
//
//#include <MPU9250_asukiaaa.h>
//MPU9250_asukiaaa mySensor;
//int
//  MPU_LED=4;
//float 
//  gX, gY, gZ;
//
//void setup() {
//  pinMode(MPU_LED,OUTPUT);
//  Wire.begin();
//  mySensor.setWire(&Wire);
//  mySensor.beginGyro();
//}
//
//void loop() {
//  mySensor.gyroUpdate();
//  gX = mySensor.gyroX();
//  gY = mySensor.gyroY();
//  gZ = mySensor.gyroZ();
//  // Do what you want
//
//  Serial.println("Lecturas del sensor MPU9250\n");
//  Serial.print("gX:");Serial.print((int)gX);Serial.print("|\t");
//  Serial.print("gY:");Serial.print((int)gY);Serial.print("|\t");
//  Serial.print("gZ:");Serial.print((int)gZ);Serial.print("|\t");
//  Serial.println("");
//
//  if((gX<10 && gX>-10) && (gY<10 && gY>-10) && (gZ<10 && gZ>-10))digitalWrite(MPU_LED,LOW);
//  else digitalWrite(MPU_LED,HIGH);
//  
//  delay(100);
//}



//#include <MPU9250_asukiaaa.h>
//MPU9250_asukiaaa mySensor;
//float mDirection;
//uint16_t mX, mY, mZ;
//
//void setup() {
//  Wire.begin();
//  mySensor.setWire(&Wire);
//  mySensor.beginMag();
//}
//
//void loop() {
//  Serial.begin(115200);
//  mySensor.magUpdate();
//  mX = mySensor.magX();
//  mY = mySensor.magY();
//  mZ = mySensor.magZ();
//  mDirection = mySensor.magHorizDirection();
// // Do what you want
//
//    Serial.println("Lecturas del sensor MPU9250\n");
//    Serial.print("Dirección: "); Serial.println(mDirection);
//  Serial.print("mX:");Serial.print((int)mX);Serial.print("|\t");
//  Serial.print("mY:");Serial.print((int)mY);Serial.print("|\t");
//  Serial.print("mZ:");Serial.print((int)mZ);Serial.print("|\t");
//  Serial.println("");
////
////  if((gX<10 && gX>-10) && (gY<10 && gY>-10) && (gZ<10 && gZ>-10))digitalWrite(MPU_LED,HIGH);
////  else digitalWrite(MPU_LED,LOW);
//////  
//  delay(100);
//}
