/*********************************************************
* Exemplo para Deletar uma Task
* 
* Esse exemplo exibe como criar duas tasks com diferentes tamanhos de Stacks
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

  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));
    digitalWrite(LED,LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
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

    if(count>=10)
    {
        if(xTask2Handle != NULL)
        {
          Serial.println("Task 2 Deletada!");
          vTaskDelete(xTask2Handle);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 1 segundos */
  }
}