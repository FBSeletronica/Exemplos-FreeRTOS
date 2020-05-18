/*********************************************************
* Exemplo para assinatura de tópico MQTT
* Por: Fábio Souza
*********************************************************/

/*inclusão da Biblioteca Arduino*/
#include <Arduino.h>

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/*Bibliotecas para o display OLED*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*bibliotecas para MQTT*/
#include <WiFi.h>
#include <PubSubClient.h>


/*definição dos pinos*/
#define LED 2       /*LED ligado ao GPIO2*/

/*Definição da resolução do Display OLED */
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* instacias para WIFI e client*/
WiFiClient espClient;
PubSubClient client(espClient);

/* configuraçãoes da REDE e broker MQTT*/
const char* ssid = "xxxxxxxxxxx";
const char* password =  "xxxxxxxxxxxx";
const char* mqttServer = "io.adafruit.com";
const int mqttPort = 1883;
const char* mqttUser = "xxxxxxxxxxxxxx";
const char* mqttPassword = "xxxxxxxxxxxxxxxxxxxx";

QueueHandle_t xFila; /*cria objeto fila */

/* Variáveis para armazenar o handle da Task */
TaskHandle_t xTaskADCHandle,xTaskPrintHandle,xTaskMQTTHandle;

/* Protótipo das Tasks*/
void vTaskADC(void *pvParameters ); 
void vTaskPrint(void *pvParameters);
void vTaskMQTT(void *pvParameters); 


/* Funções auxiliares */
void vInitHW(void);

//função pra reconectar ao servido MQTT
void reconect() {
  //Enquanto estiver desconectado
  while (!client.connected()) {

     if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
      //subscreve no tópico
      client.subscribe("fabiosouza_io/feeds/L1", 1); //nivel de qualidade: QoS 1
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  //armazena msg recebida em uma sring
  payload[length] = '\0';
  String strMSG = String((char*)payload);

  Serial.print("Mensagem chegou do tópico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  Serial.print(strMSG);
  Serial.println();
  Serial.println("-----------------------");

  //aciona saída conforme msg recebida
  if (String(topic) == "fabiosouza_io/feeds/L1") {
    if (strMSG == "1") {        //se msg "1"
      digitalWrite(LED, HIGH);  //coloca saída em HIGH para ligar a Lampada 
    } else if (strMSG == "0") {  //se msg "0"
      digitalWrite(LED, LOW);   //coloca saída em HIGH para desligar a Lampada 
    }
  }
}

void setup() {
  
  vInitHW();
  
  xFila = xQueueCreate(1, sizeof(int));//Cria a fila com 1 slots com tamanho de um int
  
  if( xFila == NULL )
  {
    Serial.println("Não foi possivel criar a fila");
    while(1);
  }
  /* Cria a vTaskADC */
  xTaskCreatePinnedToCore(vTaskADC,  "TaskADC",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskADCHandle,APP_CPU_NUM);    
  /* Cria a vTaskPrint */
  xTaskCreatePinnedToCore(vTaskPrint,  "TaskPrint",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskPrintHandle,APP_CPU_NUM);    
  /* Cria Task Para enviar dado via MQTT*/
  xTaskCreatePinnedToCore(vTaskMQTT,  "TaskMQTT",  configMINIMAL_STACK_SIZE + 1024,  NULL,  1,  &xTaskMQTTHandle,APP_CPU_NUM);     

}

void loop() {
  client.loop(); //atauliza conexão MQTT a cada 1 segundo
  vTaskDelay(pdMS_TO_TICKS(1000));    /* Delay de 1 segundos */
}



/* Função Init Harware */
void vInitHW(void)
{
    Serial.begin(115200); /* Inicializa comunicação serial com baudrate de 9600 bps */
    pinMode(LED,OUTPUT); /* configura pino do LED como saída*/

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("ERRO ao iniciar o display SSD1306"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando ao WiFi..");
  }
  Serial.println("Conectado na rede WiFi");

 }

/* impementação da TaskADC */
void vTaskADC(void *pvParameters )
{
  (void) pvParameters;
  
  int adcValue;
  while(1)
  {
    adcValue = analogRead(34);
    xQueueSend(xFila, &adcValue, portMAX_DELAY) ;/* envia valor atual de count para fila*/
    vTaskDelay(pdMS_TO_TICKS(5000)); /* Aguarda 5000 ms antes de uma nova iteração*/
  }
}

/* Implemntação da vTaskDisplay  */
void vTaskPrint(void *pvParameters )
{
  (void) pvParameters;  /* Apenas para o Compilador não retornar warnings */
  int valor_recebido = 0;
  while(1)
  {
      if(xQueuePeek(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) //verifica se há valor na fila para ser lido. Espera 1 segundo
      {
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print("ADC:");
        display.println(String(valor_recebido));
        display.display(); 
        //Serial.println("ADC:" + String(valor_recebido)); //imprime valor recebido
      }
      else
      {
        Serial.println("TIMEOUT");    //ocorreu timeout
      }
  }
}
/*Implementação da Task MQTT */
void vTaskMQTT(void *pvParameters){
  (void) pvParameters;
  char mensagem[30];
  int valor_recebido = 0;

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while(1)
  {
    
      if(xQueueReceive(xFila, &valor_recebido, portMAX_DELAY) == pdTRUE) //verifica se há valor na fila para ser lido. Espera 1 segundo
      {
        if(!client.connected()){
          reconect();
        }
        
        
        //Serial.println("ADC:" + String(valor_recebido)); //imprime valor recebido
        sprintf(mensagem, "%d", valor_recebido);

        //Envia a mensagem ao broker
        client.publish("fabiosouza_io/feeds/test", mensagem);
        Serial.print("Valor enviado: ");
        Serial.println(mensagem);
        
        vTaskDelay(5000);
      }
      else
      {
        Serial.println("TIMEOUT");
      }
 

  }

}
