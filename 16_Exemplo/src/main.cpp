/*********************************************************
* Exemplo para demonstrar o uso de software timer
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"


/*definição dos pinos*/
#define LED 2       /*LED ligado ao GPIO2*/
#define LED2 14       /*LED ligado ao GPIO14*/
#define BT 12       /*Botão ligado ao pino GPIO12*/

TaskHandle_t xTask1;           /*hadle da task1*/
TimerHandle_t xTimer1,xTimer2; /*handle dos timers*/


/*protótipo da Task1*/
void vTask1( void * pvParameters );

/* Funções auxiliares */
void vInitHW(void);

void callBackTimer1(TimerHandle_t pxTimer );
void callBackTimer2(TimerHandle_t pxTimer );

void setup() 
{
  /*Inicia Hardware */
  vInitHW();

  /*Cria o timer1 com 1000ms de periodo com auto-reaload*/
  xTimer1 = xTimerCreate("Timer1", pdMS_TO_TICKS(1000), pdTRUE, 0, callBackTimer1);
  if( xTimer1 == NULL )
  {
    Serial.println("Não foi possível criar o timer 1");
    while(1);
  }
  else{
    Serial.println("Timer 1 Criado");
  }

  /*Cria o timer2 com 1000ms de periodo com one shot*/
  xTimer2 = xTimerCreate("Timer2", pdMS_TO_TICKS(10000), pdFALSE, 0, callBackTimer2);
  
  /* Cria task1 */
  xTaskCreatePinnedToCore(vTask1,  "Task1",  configMINIMAL_STACK_SIZE + 1024,  NULL, 1,  &xTask1,APP_CPU_NUM);    

  /*Inicia o timer1*/
  xTimerStart(xTimer1, 0);

}

void loop() {
 vTaskDelay(pdMS_TO_TICKS(3000));
}


/* Task1 */
void vTask1( void * pvParameters )
{
  (void) pvParameters;

   uint8_t debouncingTime = 0;

  /* loop forever */
  while(1)
  {
    if(digitalRead(BT)==LOW){
      debouncingTime++;
      if(debouncingTime >= 10 )
      {
          digitalWrite(LED2,HIGH);
          debouncingTime = 0;
          Serial.println("Timer 2 Start");
          xTimerStart(xTimer2, 0);
      }
    }
    else{
      debouncingTime = 0;
    }
    
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
    pinMode(LED2,OUTPUT);
    pinMode(BT, INPUT_PULLUP); /*configura pino do Botão */ 
}

void callBackTimer1(TimerHandle_t pxTimer )
{
   digitalWrite(LED,!digitalRead(LED));
}
void callBackTimer2(TimerHandle_t pxTimer )
{
  digitalWrite(LED2,LOW);//apaga LED  
}
