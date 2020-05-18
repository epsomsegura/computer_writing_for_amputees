#include "MPU9250.h"

MPU9250 mpu;


int
  pinOK = 13,
  mode = 0,
  time_delay = 0,
  steps = 0;



  
void setup()
{
  
  pinMode(pinOK,OUTPUT);
  Serial.begin(57600);
  
  Wire.begin();
  
  Serial.println("INICIANDO\n");
  
  while(!Serial){;}
  Serial.println("\nCONFIGURANDO\n");
  mpu.setup();
}

void loop()
{
  String a = Serial.readStringUntil('\n');
  
  if(a.startsWith("$")){
    String b = splitString(a, '$', 1);
    b.replace("$","");
    
    if(b.startsWith("getSettings")){
      Serial.println("@settings|"+(String)mode+"|"+(String)time_delay+"|"+(String)steps+"\n");
    }
    else if(b.startsWith("setSettings")){
      if(mode == 0 && time_delay==0 && steps == 0){
        Serial.println("SET settings");
        mode=splitString(b,'|',1).toInt();
        time_delay=splitString(b,'|',2).toInt();
        steps=splitString(b,'|',3).toInt(); 
      }
    }
    else if(b.startsWith("updSettings")){
      Serial.println("UPDATE settings");
      mode=splitString(b,'|',1).toInt();
      time_delay=splitString(b,'|',2).toInt();
      steps=splitString(b,'|',3).toInt(); 
    }  
  }

  if(mode!=0)
    Serial.println(createVector());
}






//  GET DATA FROM MPU9250
String createVector(){
  String vector = "@";
  int counter=steps;
  while(counter>0){
    static uint32_t prev_ms = millis();
    if ((millis() - prev_ms) > 16){
        mpu.update();

        vector+=(String)mpu.getPitch()+','+(String)mpu.getYaw()+','+(String)mpu.getRoll()+'|';
        prev_ms = millis();
    }
    
    delay((time_delay/steps));
    counter--;
  }
  
  return vector;
}

//  SPLIT A STRING AND RETURN THE VALUE
String splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}





//void testMPUData(){
//    static uint32_t prev_ms = millis();
//    if ((millis() - prev_ms) > 16)
//    {
//        mpu.update();
////      ROLL - ARRIBA/ABAJO (Giro en vertical)
//        Serial.print("roll  (x-forward (north)) : ");
//        Serial.println(mpu.getRoll());         
////      PITCH - DESPLAZAMIENTO DERECHA/IZQUIERDA (Giro en horizontal)
//        Serial.print("pitch (y-right (east))    : ");
//        Serial.println(mpu.getPitch());
////      YAW - ROTACIÓN DERECHA/IZQUIERDA (Giro en el mismo eje horizontal)
//        Serial.print("yaw   (z-down (down))     : ");
//        Serial.println(mpu.getYaw());
//        Serial.print(mpu.getPitch());
//        Serial.print(',');
//        Serial.print(mpu.getYaw());
//        Serial.print(',');
//        Serial.print(mpu.getRoll());
//        Serial.println("|");
//        prev_ms = millis();
//        delay(100);
//    }
//}
