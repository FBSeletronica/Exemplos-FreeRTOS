/*********************************************************
* Exemplo Criação de Task
* Esse exemplo exibe como criar duas tasks com diferentes tamanhos de Stacks
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
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

/* Protótipo das Tasks*/
void vTask1(void *pvParameters ); 
void vTask2(void *pvParameters ); 

/* Funções auxiliares */
void vInitHW(void);

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  
  vInitHW();      /* Configuração do Hardware */

  xTaskCreate(
     vTask1                     /* função da task*/   
    ,  "Task1"                  /* Nome da Task */
    ,  configMINIMAL_STACK_SIZE /* Stack Size */
    ,  NULL                     /* parametro passado para a task*/
    ,  1                        /* Prioridade da task*/
    ,  &task1Handle             /* handle da task*/
    );    

    xTaskCreate(
     vTask2                             /* função da task*/
    ,  "Task2"                          /* Nome da Task */
    ,  configMINIMAL_STACK_SIZE + 1024  /* Stack Size,*/
    ,  NULL                             /* parametro passado para a task*/
    ,  2                                /* Prioridade da task*/
    ,  &task2Handle                     /* handle da task*/
    );       
}

/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(9600);   /*inicializa a comunicação serial*/
    pinMode(LED,OUTPUT);  /*Configura pino do LED como saída*/
}

/* Função loop */
void loop() { 
    vTaskDelay(pdMS_TO_TICKS(3000));    /* Delay de 3 segundos */
}

/* Task 1 - Blink LED */
void vTask1(void *pvParameters )
{
  (void) pvParameters;

  while(1)
  {
    digitalWrite(LED,!digitalRead(LED)); /*inverte o estado do LED*/
    vTaskDelay(pdMS_TO_TICKS(100));     /* Delay de 1 segundos */
  }
}

/* Task 2 - Imprime valor na serial*/
void vTask2(void *pvParameters )
{

  (void) pvParameters;  
  int count = 0;        /*variável auxiliar */

  while(1)
  {
    Serial.println("Task 2: " + String(count++));
    vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
  }
}
