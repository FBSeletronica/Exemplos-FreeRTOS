/*********************************************************
* Exemplo para demonstrar o uso de MUTEX para acesso a recursos
* e a inversão de prioridade
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

/*handle do Semaforo*/
SemaphoreHandle_t xMutex;

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTaskTrataISRBTHandle;

/* Protótipo das Tasks*/
void vTask1( void *pvParameter );
void vTask2( void *pvParameters );

/* Funções auxiliares */
void vInitHW(void);

void setup() {

  vInitHW();

  /* Cria o Mutex */
  xMutex = xSemaphoreCreateMutex();
  
  xTaskCreate(vTask1, "vTask1",1000,NULL,1, NULL);
  xTaskCreate(vTask2, "vTask2",1000,NULL,4,NULL);
  
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
}

/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
}

void vTask1( void * pvParameters )
{
  (void) pvParameters;
  while(1)
  { 
    
    xSemaphoreTake( xMutex, portMAX_DELAY );
    Serial.println("vTask1 pega o semáforo");
    Serial.println("Vtask1 em execução por 2 segundos");
    delay(2000);
    Serial.println("vTask1 devolve o semáforo \r\n");
    xSemaphoreGive( xMutex );
  }
}

void vTask2( void * pvParameters )
{
  (void) pvParameters;

  while(1)
  {
    /* highPriorityTask wait until lowPriorityTask release key */
    xSemaphoreTake( xMutex, portMAX_DELAY );
    Serial.println("vTask2 pega o semáforo");
    Serial.println("Vtask2 em execução");
    Serial.println("vTask2 devolve o semáforo \r\n");
    xSemaphoreGive( xMutex );
    delay(1000); /*Delay de 1 segundo, nesse momento a task1 pode pegar o semáforo */
  }
}
