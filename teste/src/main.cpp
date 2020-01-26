/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*Definição do Pino do LED*/
#define LED 2

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  /*Configura pino do LED como saída*/
  pinMode(LED,OUTPUT);
}

/*Função loop - fica em loop*/
void loop() {
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
  delay(1000);
}