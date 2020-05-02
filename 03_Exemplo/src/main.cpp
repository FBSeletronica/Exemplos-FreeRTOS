/*********************************************************
* Exemplo para Deletar uma Task
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*Definição do Pino do LED*/
#define LED 2

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTask1Handle,xTask2Handle;

/* Protótipo das Tasks*/
void vTask1(void *pvParameters ); 
void vTask2(void *pvParameters ); 

/* Funções auxiliares */
void vInitHW(void);

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  
  vInitHW();      /* Configuração do Hardware */

  /* Cria a vTask1 */
  xTaskCreate(
              vTask1                              /* Ponteiro para a função vTask1*/
              ,  "Task1"                          /* Nome da Task */
              ,  configMINIMAL_STACK_SIZE + 1048  /* Stack Size*/
              ,  NULL                             /* Parametro passado para a task*/
              ,  1                                /* Prioridade da task*/
              ,  &xTask1Handle                    /* Handle da task*/
              );    

    /* Cria a vTask2 */
    xTaskCreate(
              vTask2                              /* Ponteiro para a função vTask2 */ 
              ,  "Task2"                          /* Nome da Task */
              ,  configMINIMAL_STACK_SIZE + 1048  /* Stack Size*/
              ,  NULL                             /* Parametro passado para a task*/
              ,  2                                /* Prioridade da task*/
              ,  &xTask2Handle                    /* Handle da task*/
              );       
}

/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* Configura pino do LED como saída Digital */ 

}

/* Função loop */
void loop() { 
    vTaskDelay(pdMS_TO_TICKS(3000));    /* Delay de 3 segundos */
}

/* Task 1 -  Blink LED */
void vTask1(void *pvParameters )
{
  (void) pvParameters;

  while(1)
  {
    digitalWrite(LED,HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));     /*Delay de 100 ms*/
    digitalWrite(LED,LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
  }
}

/* Task Blink LED */
void vTask2(void *pvParameters )
{

  (void) pvParameters;  /* Apenas para o Compilador não retornar warnings */
  uint8_t count = 0;      /* declara e inicializa variável contador com 0*/

  while(1)
  {
    /* Imprime valor do contador na Serial*/
    Serial.println("Task 2: " + String(count++));  

    /*Verifica valor do contador*/
    if(count>=10) /*se maior ou igual a 10*/
    {
        if(xTask1Handle != NULL)    /*verifica se o handle da task é diferente de NULL*/
        {
          Serial.println("Task 1 Deletada!"); /*Mensagem na serial*/
          vTaskDelete(xTask1Handle);          /*Deleta task 1*/  
          digitalWrite(LED,LOW);              /*Garante que LED ficará apagado*/
          xTask1Handle = NULL;                /*atualiza valor da variável para NULL*/
        }
    }
    vTaskDelay(pdMS_TO_TICKS(500));    /* Delay de 0.5 segundos */
  }
}
