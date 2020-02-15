/*********************************************************
* Código para ler o MAC da placa
* Por: Fábio Souza
*********************************************************/

#include <Arduino.h>
#include "WiFi.h"
 
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
