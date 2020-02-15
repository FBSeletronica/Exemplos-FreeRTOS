/*********************************************************
* Exemplo que demonstra como criar uma fila, adicionar e ler
* dados da fila através de duas tasks
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


/*definição dos pinos*/
#define LED 2       /*LED ligado ao GPIO2*/

QueueHandle_t xFila; /*cria objeto fila */

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;

/* Protótipo das Tasks*/
void vTask1(void *pvParameters ); 
void vTask2(void *pvParameters ); 


/* Funções auxiliares */
void vInitHW(void);

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  
  BaseType_t xReturned;

  vInitHW();      /* Configuração do Hardware */

  xFila = xQueueCreate(5, sizeof(int));//Cria a fila com 5 slots com tamanho de um int
  
  if( xFila == NULL )
  {
    Serial.println("Não foi possivel criar a fila");
    while(1);
  }
  /* Cria a vTask1 */
  xReturned = xTaskCreate(vTask1,  "Task1",  configMINIMAL_STACK_SIZE + 1024,  NULL ,  1,  &xTask1Handle ); 

  if( xReturned == pdFAIL)
  {
      Serial.println("Não foi possivel criar a fTask 1");
      while(1);
  }

    /* Cria a vTask2 */
  xReturned = xTaskCreate(vTask2,  "Task2",  configMINIMAL_STACK_SIZE + 1024,  NULL ,  1,  &xTask2Handle ); 
  if( xReturned == pdFAIL)
  {
      Serial.println("Não foi possivel criar a fTask 1");
      while(1);
  }
}

/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
}

/* Função loop */
void loop() { 
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
}

/* Task1 */
void vTask1(void *pvParameters )
{
  (void) pvParameters;
  int count = 0;

  while(1)
  {

    if(count <10){
        xQueueSend(xFila, &count, portMAX_DELAY) ;/* envia valor atual de count para fila*/
        count++;    /*incrementa contador*/
    }
    else
    {
      count = 0; /* reinicia contador*/
      vTaskDelay(pdMS_TO_TICKS(5000)); /*aguarda 5 segundos */
    }
    vTaskDelay(pdMS_TO_TICKS(500)); /* Aguarda 500 ms antes de uma nova iteração*/
  }
}

/* Task2  */
void vTask2(void *pvParameters )
{
  (void) pvParameters;  /* Apenas para o Compilador não retornar warnings */
  int valor_recebido = 0;
  while(1)
  {
      if(xQueueReceive(xFila, &valor_recebido, pdMS_TO_TICKS(1000)) == pdTRUE) //verifica se há valor na fila para ser lido. Espera 1 segundo
      {
          Serial.println("Valor Recebido:" + String(valor_recebido)); //imprime valor recebido
      }
      else
      {
          Serial.println("TIMEOUT");    //ocorreu timeout
      }
  }
}
