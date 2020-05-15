/*********************************************************
* Exemplo para demonstrar o uso de task Notification entre
* tarefas
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/*definição dos pinos*/
#define LED 2       /*LED ligado ao GPIO2*/
#define BT 12


/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTask1,xTask2;

/* Protótipo das Tasks*/
void vTask1( void *pvParameters );
void vTask2( void *pvParameters );

/* Funções auxiliares */
void vInitHW(void);


void setup() {

  vInitHW();
  
  /* Cria task 1 */
  xTaskCreatePinnedToCore(vTask1,  "Task1",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTask1,APP_CPU_NUM);    

  /* Cria task 2 */
  xTaskCreatePinnedToCore(vTask2,  "Task2",  configMINIMAL_STACK_SIZE + 1024,  NULL,  2,  &xTask2,APP_CPU_NUM);    

}

void loop() {

}

/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
    pinMode(BT, INPUT_PULLUP); /*configura pino do Botão */ 
}

/* Task 1*/
void vTask1( void *pvParameters )
{
  (void) pvParameters;
  while(1)
  {
    /* Aguarda a notificação*/
    ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
    Serial.println("Inverte LED");
    /*inverte estado do LED*/
    digitalWrite(LED, !digitalRead(LED));
  }

}


/* Task 2*/
void vTask2( void * pvParameters )
{
  (void) pvParameters;

   uint8_t debouncingTime = 0;

  /* loop forever */
  while(1)
  {
    if(digitalRead(BT)==LOW){
      debouncingTime++;
      if(debouncingTime >= 10 )
      {
    
          debouncingTime = 0; 
          Serial.println("Notifica Task 1");
          xTaskNotifyGive(xTask1);

      }
    }
    else{
      debouncingTime = 0;
    }
    
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

