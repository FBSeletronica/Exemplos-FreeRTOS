/*********************************************************
* Exemplo de passagem de parametro para Tasks na criação* 
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*Definição do Pino do LED*/
#define LED 2
#define LED2 4

/* Variáveis para Armazenar o handle da Task */
TaskHandle_t xTask1Handle, xTaskLED1Handle, xTaskLED2Handle;


/* Protótipo das Tasks*/
void vTask1(void *pvParameters ); 
void vtaskBlinkLed(void *pvParameters);

int valor = 100;
char x;

/* Funções auxiliares */
void vInitHW(void);

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  
  vInitHW();      /* Configuração do Hardware */

    xTaskCreate(
     vTask1
    ,  "Task1"                  /* Nome da Task */
    ,  2048                     /* Stack Size*/
    ,  (void*)valor             /* parametro passado para a task*/
    ,  2                        /* Prioridade da task*/
    ,  &xTask1Handle             /* handle da task*/
    );       

    xTaskCreate(
        vtaskBlinkLed
    ,  "LED1"                   /* Nome da Task */
    ,  2048                     /* Stack Size*/
    ,  (void*)LED                 /* parametro passado para a task*/
    ,  2                        /* Prioridade da task*/
    ,  &xTaskLED1Handle            /* handle da task*/

    );

    xTaskCreate(
        vtaskBlinkLed
    ,  "LED2"                   /* Nome da Task */
    ,  2048                     /* Stack Size*/
    ,  (void*)LED2              /* parametro passado para a task*/
    ,  2                        /* Prioridade da task*/
    ,  &xTaskLED2Handle         /* handle da task*/

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


/* Task print */
void vTask1(void *pvParameters )
{
  int count = (int)pvParameters;

  while(1)
  {
    Serial.println("Task 2: " + String(count++));
    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 1 segundos */
  }
}

/* Task blink LED */
void vtaskBlinkLed(void *pvParameters){
  
  int pin = (int)pvParameters; /*lê o parametro passado*/
  pinMode(pin,OUTPUT); /*Configura o Pino como Saída*/
  Serial.println(pcTaskGetTaskName(NULL)); /*Pega o nome da Task*/

  while(1)
  {
    digitalWrite(pin,HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(pin,LOW);
    vTaskDelay(pdMS_TO_TICKS(500));    
  }
}
