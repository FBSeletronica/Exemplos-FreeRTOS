/*********************************************************
* Exemplo de Utilitários de tarefas
* 
* 
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
/* Variáveis para Armazenar o handle da Task */
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;

/* Protótipo das Tasks*/
void vTask1(void *pvParameters ); 
void vTask2(void *pvParameters ); 


/* Funções auxiliares */
void vInitHW(void);

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  
  vInitHW();      /* Configuração do Hardware */

  xTaskCreate(
     vTask1
    ,  "Task1"                    /* Nome da Task */
    ,  2048                      /* Stack Size, não se preocupe com esse valor agora. Vamos estudar mais pra frente*/
    ,  NULL                       /* parametro passado para a task*/
    ,  1                        /* Prioridade da task*/
    ,  &xTask1Handle             /* handle da task*/
    );    

    xTaskCreate(
     vTask2
    ,  "Task2"                  /* Nome da Task */
    ,  2048                     /* Stack Size, não se preocupe com esse valor agora. Vamos estudar mais pra frente*/
    ,  NULL                     /* parametro passado para a task*/
    ,  2                        /* Prioridade da task*/
    ,  &xTask2Handle             /* handle da task*/
    );       
}

/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(9600);
    pinMode(LED,OUTPUT);

}

/* Função loop */
void loop() { 
    vTaskDelay(pdMS_TO_TICKS(3000));    /* Delay de 3 segundos */
}

/* Task Blink LED */
void vTask1(void *pvParameters )
{
  (void) pvParameters;
  TickType_t xLastWakeTime;

  /* The xLastWakeTime variable needs to be initialized with the current tick count.
 This is the only time the variable is written to explicitly. After this, xLastWakeTime is
 automatically updated within vTaskDelayUntil(). */
 xLastWakeTime = xTaskGetTickCount();

  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 100) );
    digitalWrite(LED,LOW);
    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 900 ) );    /* Delay de 900 milisegundos */
  }
}

/* Task Blink LED */
void vTask2(void *pvParameters )
{
  int count = 0;
  (void) pvParameters;

  while(1)
  {
    Serial.println("Task 2: " + String(count++));
    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 1 segundos */
  }
}
