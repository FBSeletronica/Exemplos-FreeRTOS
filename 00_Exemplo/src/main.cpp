/*********************************************************
* Exemplo Blink LED - ESP32
* Pisca o LED da placa em intervalos de 1 segundo
* Exemplo para entender o fluxo de desenvolvimento no PIO
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*Definição do Pino do LED*/
#define LED 2

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  /*Configura pino do LED como saída*/
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
}

/*Função loop - fica em loop*/
void loop() {
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  delay(1000);
  Serial.println("Hello");
}