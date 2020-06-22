/*********************************************************
* Exemplo para demonstrar o uso de semaforo binário
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

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTaskADCHandle;

/* Protótipo das Tasks*/
void vTaskADC(void *pvParameters );

SemaphoreHandle_t xBinarySemaphore;

/* Funções auxiliares */
void vInitHW(void);

void setup() {

  vInitHW();

  xBinarySemaphore = xSemaphoreCreateBinary(); //Cria o semáfor binário

  if( xBinarySemaphore == NULL )
  {
    Serial.println("Não foi possivel criar o Semáforo");
    while(1);
  }

  /* Cria a vTaskADC */
  xTaskCreatePinnedToCore(vTaskADC,  "TaskADC",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskADCHandle,APP_CPU_NUM);
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  Serial.println("Essa Mensagem veio do LOOP e ocorre sempre que o LED é invertido");
  Serial.println("Valor do LED: " + String(digitalRead(LED)) +"\r\n");
  vTaskDelay(pdMS_TO_TICKS(1000));
  xSemaphoreGive(xBinarySemaphore);
}



/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
}

/* impementação da TaskADC */
void vTaskADC(void *pvParameters )
{
  (void) pvParameters;

  int adcValue;
  xSemaphoreGive(xBinarySemaphore);
  while(1)
  {
    xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
    adcValue = analogRead(34);
    Serial.println("ADC:" + String(adcValue) +"\r\n"); //imprime valor da convesão AD
  }
}
