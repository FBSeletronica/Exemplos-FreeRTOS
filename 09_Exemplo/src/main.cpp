/*********************************************************
* Exemplo que demonstra como enviar valores para uma fila 
* a partir de uma ISR
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
#define BT 12       /* Botão ligado ao pino 12*/

QueueHandle_t xFila; /*cria objeto fila */

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTask1Handle;

/* Protótipo das Tasks*/
void vTask1(void *pvParameters ); 


/* Funções auxiliares */
void vInitHW(void);

void trataISRBT() {
  static int val;
  BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdTRUE;
  
  val++;
  xQueueSendFromISR(xFila, &val, &xHigherPriorityTaskWoken);
}

/*Função setup - é chamada apenas uma vez na inicialização */
void setup() {
  
  vInitHW();      /* Configuração do Hardware */

  xFila = xQueueCreate(5, sizeof(int));//Cria a fila com 5 slots com tamanho de um int
  
  if( xFila == NULL )
  {
    Serial.println("Não foi possivel criar a fila");
    while(1);
  }
  /* Cria a vTask1 */
  xTaskCreate(vTask1,  "Task1",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTask1Handle);    
}

/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
    pinMode(BT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BT), trataISRBT, FALLING); //configura interrupção do pino

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
  int valor_recebido = 0;

  while(1)
  {
      xQueueReceive(xFila, &valor_recebido, portMAX_DELAY); /* Aguarda chegar valor na Fila */ 
      Serial.println("BT Pressionado: " + String(valor_recebido)); //imprime valor recebido
  }
}

