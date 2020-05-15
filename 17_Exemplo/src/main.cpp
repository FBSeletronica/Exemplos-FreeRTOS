/*********************************************************
* Exemplo para demonstrar o uso de grupo de eventos
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

/*definição dos pinos*/
#define LED 2       /*LED ligado ao GPIO2*/
#define LED2 14       /*LED ligado ao GPIO14*/
#define BT 12       /*Botão ligado ao pino GPIO12*/

/* define event bits */
#define TASK_1_BIT        ( 1 << 0 ) //1
#define TASK_2_BIT        ( 1 << 1 ) //10


TaskHandle_t xTask1,xTask2;    /*hadle das tasks*/
TimerHandle_t xTimer1,xTimer2; /*handle dos timers*/

/* create event group */
EventGroupHandle_t xEventos;

/* Funções auxiliares */
void vInitHW(void);

//Protótipo de funções
void callBackTimer1(TimerHandle_t pxTimer );

void vTask1( void * pvParameters );
void vTask2( void * pvParameters );
void vTask3( void * pvParameters );

int8_t count = 0;

void setup() {

 /*Inicia Hardware */
  vInitHW();

  xEventos = xEventGroupCreate();

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
  
  

 /* Cria tasks */
  xTaskCreatePinnedToCore(vTask1,  "Task1",  configMINIMAL_STACK_SIZE + 1024,  NULL, 1,  &xTask1,APP_CPU_NUM);    
  xTaskCreatePinnedToCore(vTask2,  "Task2",  configMINIMAL_STACK_SIZE + 1024,  NULL, 1,  &xTask2,APP_CPU_NUM);    

  /*Inicia o timer1*/
  xTimerStart(xTimer1, 0);

}

void loop() {

}

void vTask1( void * parameter )
{

  while(1)
  {
    /* wait forever until event bit of task 1 is set */
    EventBits_t xbit = xEventGroupWaitBits(xEventos, TASK_1_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task 1 saiu do estado de bloqueio");

  }

  vTaskDelete( NULL );
}

/* this task is similar to sendTask1 */
void vTask2( void * parameter )
{

  for(;;){
    /* wait forever until event bit of task 2 is set */
    EventBits_t xbit = xEventGroupWaitBits(xEventos, TASK_2_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task 2 saiu do estado de bloqueio");
  }
  vTaskDelete( NULL );
}

//callback do timer
void callBackTimer1(TimerHandle_t pxTimer )
{
  
  count++;
  if(count == 5){
    xEventGroupSetBits(xEventos,TASK_1_BIT);
  }else if(count == 10){
    xEventGroupSetBits(xEventos,TASK_2_BIT);
  }
  else if(count == 15){
      count = 0;
      xEventGroupSetBits(xEventos,TASK_1_BIT|TASK_2_BIT);
  }
}

/* Função Init Hardware */
void vInitHW(void)
{
    Serial.begin(9600); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/
    pinMode(BT, INPUT_PULLUP); /*configura pino do Botão */ 
}