#include <Wire.h>

//  CONSTANTES
//Direccion I2C de la IMU
#define MPU 0x68
//Ratios de conversion
#define A_R 16384.0 // 32768/2
#define G_R 131.0 // 32768/250
//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779

//  VARIABLES
// Gestos
String 
  motion = "",
  tmp_val = "";
 
// MPU-6050 da los valores en enteros de 16 bits - Valores RAW
int16_t 
  AcX, 
  AcY, 
  AcZ, 
  GyX, 
  GyY, 
  GyZ;
 
// Angulos
float 
  dt,
  Acc[2],
  Gy[3],
  Angle[3];

// Delay
long tiempo_prev;
//float dt;

void setup()
{
//  Inicialización del sensor
Wire.begin(); 
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
//  Inicialización del piuerto Serial
Serial.begin(57600);
// Configuración de pines de entrada
pinMode(13,OUTPUT);
pinMode(8,INPUT);
pinMode(9,INPUT);
}


//  Loop infinito
void loop()
{
   cwfa_device();
}

//  Leer gesto y enviar lectura
void cwfa_device(){
  digitalWrite(13,LOW);
  //Leer los valores del Acelerometro de la IMU
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);   //A partir del 0x3B, se piden 6 registros
  AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  
  //A partir de los valores del acelerometro, se calculan los angulos Y, X
  //respectivamente, con la formula de la tangente.
  Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
  
  //Leer los valores del Giroscopio
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);   //A partir del 0x43, se piden 6 registros
  GyX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();
  
  //Calculo del angulo del Giroscopio
  Gy[0] = GyX/G_R;
  Gy[1] = GyY/G_R;
  Gy[2] = GyZ/G_R;
  
  dt = (millis() - tiempo_prev) / 1000.0;
  tiempo_prev = millis();
  
  //Aplicar el Filtro Complementario
  Angle[0] = 0.98 *(Angle[0]+Gy[0]*dt) + 0.02*Acc[0];
  Angle[1] = 0.98 *(Angle[1]+Gy[1]*dt) + 0.02*Acc[1];
  
  //Integración respecto del tiempo paras calcular el YAW
  Angle[2] = Angle[2]+Gy[2]*dt;

  bool 
    dead_x = (Angle[1]<15 && Angle[1]>-10),
    dead_y = (Angle[0]<20 && Angle[0]>-15);


  if(digitalRead(8)){
    motion = "select";
  }
  else if(digitalRead(9)){
    motion = "backspace";
    }
  else if(dead_x && dead_y){
    motion = "zero";
    digitalWrite(13,HIGH);
  }
  else if(dead_x && (Angle[0]>20 && Angle[0]<60)){
    motion = "up";
    digitalWrite(13,LOW);
  }
  else if(dead_x && (Angle[0]<-15 && Angle[0]>-45)){
    motion = "down";
    digitalWrite(13,LOW);
  }
  else if(dead_y && Angle[1]>15){
    motion = "right";
    digitalWrite(13,LOW);
  }
  else if(dead_y && Angle[1]<-10){
    motion = "left";
    digitalWrite(13,LOW);
  }
  else{
    motion = "zero";
    digitalWrite(13,LOW);
  }
  

  if(tmp_val == ""){
    tmp_val = motion;
    Serial.println(motion);
  }
  
  if(tmp_val!=motion){
    tmp_val = "";
  }
  
  delay(50);
}
