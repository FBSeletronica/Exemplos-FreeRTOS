/*********************************************************
* Exemplo de passagem de parametro para Tasks
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

  xTaskCreatePinnedToCore(vTask1,"Task1",2048,NULL,1, &xTask1Handle,APP_CPU_NUM);
  xTaskCreatePinnedToCore(vTask2,"Task2",2048,NULL,1, &xTask1Handle,PRO_CPU_NUM); 
}

/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(9600);
    pinMode(LED,OUTPUT);
}

/* Função loop */
void loop() { 
    Serial.print(__func__);
    Serial.print(" : ");
    Serial.print("Nucleo =");
    Serial.println(xPortGetCoreID());

    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 0.5 segundos */  
}

/* Task1*/
void vTask1(void *pvParameters )
{
  (void)pvParameters;

  while(1)
  {
    Serial.print(__func__);
    Serial.print(" : ");
    Serial.print("Nucleo =");
    Serial.println(xPortGetCoreID());
    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 0.5 segundos */  
  }
}

/* Task2*/
void vTask2(void *pvParameters )
{
  (void)pvParameters;

  while(1)
  {
    Serial.print(__func__);
    Serial.print("   : ");
    Serial.print("Nucleo =");
    Serial.println(xPortGetCoreID());
    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 0.5 segundos */  
  }
}

