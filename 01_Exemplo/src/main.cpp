/*********************************************************
* Exemplo Hello RTOS
* Pisca o LED da placa em intervalos de 200 milisegundo e
* Envia a mensagem "Oi" a cada 10 segundos
* Exemplo para verificar o funcionamento de um RTOS
*
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*Definição do Pino do LED*/
#define LED 2

/* Protótipo da Task*/
void vTaskBlink(void *pvParameters ); 

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {

  Serial.begin(9600);
  
  xTaskCreate(
     vTaskBlink
    ,  "TaskBlink"   // Nome da Task
    ,  1024          // Stack Size, não se preocupe com esse valor agora. Vamos estudar mais pra frente
    ,  NULL          // parametro passado para a task
    ,  2             // Prioridade da task
    ,  NULL);        // handle da task
}

//Função loop
void loop() { 
  vTaskDelay(pdMS_TO_TICKS(100000));
  Serial.println("Oi");
}

//Task Blink LED
void vTaskBlink(void *pvParameters ){

  (void) pvParameters;

  pinMode(LED,OUTPUT);

  while(1){
    digitalWrite(2,!digitalRead(2));
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

