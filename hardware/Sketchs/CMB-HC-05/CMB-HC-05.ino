/*
* 
*   Proyecto: CMB-HC-05
*   Versión: 1.0
*   Fecha de creación: 08/02/2020
*   Desarrollador:  Epsom Enrique Segura Jaramillo
*                   segurajaramilloepsom@gmail.com
*                   https://github.com/epsomsegura
*                   https://www.youtube.com/channel/UCWHILEt3oj_6Yphau7g46oQ
*   
*   Descripción:
*   El CMB-HC-05 es un sketch creado para configurar el módulo bluetooth HC-05
*   mediante comandos AT, es compatible para configurar el módulo bluetooth HC-06.
*   Hace uso de la libreria "SoftwareSerial" para no interferir en la comunicación
*   serial natural mediante el puerto USB del microcontrolador.
*   Este sketch fue probado con "Arduino Leonardo" y el módulo bluetooth HC-05, el
*   cual tiene la característica específica de contar con un botón en la parte
*   inferior derecha si es visto en la posición frontal (KEY). Antes de iniciar el sketch 
*   o antes de alimentar el módulo, es necesario mantener presionado ete botón para que
*   el módulo cambie su estado a modo "Comandos AT" y posteriormente ser configurado
*   desde el "Monitor Serie". A continuación, se presentan los comandos AT básicos para
*   el módulo HC-05/HC-06:
*   
*   COMANDO             CATEGORÍA                       RESPUESTA                     PARÁMETRO
*   AT                  Test                            OK                            -
*   AT+RESET            Reset                           OK                            -
*   AT+VERSION?         Versión del firmware            +VERSION: <Param> OK          Param: Versión del firmware
*   AT+ORGL             Restaurar                       OK                            -
*   AT+ADDR?            Dirección MAC                   +ADDR: <Param> OK             Param: Dirección del módulo bluethoot
*   AT+NAME=<Param>     Cambiar nombre                  OK                            Param: Nuevo nombre del bluetooth
*   AT+NAME?            Obtener nombre                  +NAME: <Param> OK (/Fail)     Param: Nombre del bluetooth
*   AT+ROLE=<Param>     Cambiar modo                    OK                            Param: 0-Esclavo 1-Maestro 2-Bucle-Esclavo
*   AT+ROLE?            Obtener modo                    +ROLE: <Param> OK (/Fail)     Param: 0-Esclavo 1-Maestro 2-Bucle-Esclavo
*   AT+UART=<Param1>,   Cambiar configuración serial    OK                            Param1: Baudios
*           <Param2>,                                                                 Param2: Stopbit
*           <Param3>                                                                  Param3: Paridad
*   AT+UART?            Obtener configuración serial    +UART: <Param1>,              Param1: Baudios
*                                                              <Param2>,              Param2: Stopbit
*                                                              <Param3>               Param3: Paridad
*                                                              OK
*   AT+PSWD=<Param>     Cambiar password                OK                            Param: Código PIN (Cuatro dígitos)
*   AT+PSWD?            Cambiar password                +PSWD: <Param> OK             Param: Código PIN (Cuatro dígitos)
*   
*/


//  Librerias
#include <SoftwareSerial.h>

//  Variables
int
  Rx=10,    //  Pin asignado para Rx
  Tx=11;    //  Pin asignado para TX

//  Serial para bluetooth
SoftwareSerial BT_Serial(Rx, Tx);

//  Configuración
void setup(){
  delay (500);
  Serial.begin(57600);
  while(!Serial){;}
  Serial.println("CMB-HC-05\nPor Epsom Enrique Segura Jaramillo");
  Serial.println("Si está usando el módulo HC-05 no olvide presionar el botón KEY y encender el módulo\n");
  Serial.println("Levantando el modulo HC-05");
  Serial.println("Esperando comandos AT:");
  BT_Serial.begin(57600); 
}

//  Operación
void loop(){  
  if (BT_Serial.available())
    Serial.write(BT_Serial.read());
  if (Serial.available())
    BT_Serial.write(Serial.read());
}
