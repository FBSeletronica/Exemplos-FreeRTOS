/*********************************************************
* Exemplo para demonstrar o uso de semaforo contador dentro
* de uma ISR
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
#define BT 12       /*Botão ligado ao pino GPIO12

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTaskTrataISRBTHandle;

/* Protótipo das Tasks*/
void vTaskTrataISRBT( void *pvParameters );

/*handle do semaforo contador*/
SemaphoreHandle_t xCountingSemaphore;

/* Protótipo das Tasks*/
void vTaskTrataISRBT( void *pvParameters );

/* Funções auxiliares */
void vInitHW(void);

/* interrupt function callback */
void ISRcallback() {

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  /*incrementa o semáforo contador*/
  xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
}

void setup() 
{
  /*Inicia Hardware */
  vInitHW();
  /* Cria semáforo contador com 10 posições*/
  xCountingSemaphore = xSemaphoreCreateCounting( 255, 0 );
  
  /* Cria task para tratar BT */
  xTaskCreatePinnedToCore(vTaskTrataISRBT,  "Task Trata ISR BT",  configMINIMAL_STACK_SIZE + 1024,  NULL,  4,  &xTaskTrataISRBTHandle,APP_CPU_NUM);    

}

void loop() {
 
}


/* Task para tratar BT pressionado*/
void vTaskTrataISRBT( void * pvParameters )
{
  (void) pvParameters;
  UBaseType_t x;
  /* loop forever */
  while(1)
  {
    /* Aguarda a liberação do Semaforo pela ISR*/
    xSemaphoreTake( xCountingSemaphore, portMAX_DELAY );
    Serial.println("Tratando a interrupção");
    x = uxSemaphoreGetCount(xCountingSemaphore);
    Serial.println(x);
    /*inverte estado do LED*/
    digitalWrite(LED, !digitalRead(LED));
    /* elay here to see LED blinky */
    delay(1000);
  }
}

/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
    pinMode(BT, INPUT_PULLUP); /*configura pino do Botão */ 
    /*Configura interrupção do pino do botão*/
    attachInterrupt(digitalPinToInterrupt(BT), ISRcallback, FALLING);
}
