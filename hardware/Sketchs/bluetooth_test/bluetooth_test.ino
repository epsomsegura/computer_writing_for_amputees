void setup(){ 
  delay(500);              // Espera antes de encender el modulo
  Serial1.begin(57600);
}

void loop(){
  bt_connection();
}

//  Conexión Bluetooth
void bt_connection(){
  if(Serial1.available()){
    String socket = Serial1.readStringUntil('\n');
    if(socket == "get_data"){
      Serial1.write("TESTING");
    }
  }
}
