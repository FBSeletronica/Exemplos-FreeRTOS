/*********************************************************
* Código para Enviar dado via ESP NOW
* Por: Fábio Souza
*********************************************************/

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// MACADDRESS da placa a ser conectada
uint8_t broadcastAddress[] = {0x30, 0xAE, 0xA4, 0x0B, 0x66, 0x34};


//variável para envio
uint8_t data =0;

// Callback quando dado é enviado
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("\r\nPacote enviado para: "); 
  Serial.println(macStr);
  Serial.print("Pacote: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Enviado com sucesso" : "Falha no Envio");
}
 
void setup() {
  // Inicia comunicação Serial
  Serial.begin(9600);
 
  // Configura Wi-Fi como Station
  WiFi.mode(WIFI_STA);

  // Inicia ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro para inicializar ESP-NOW");
    return;
  }

  //Função de Callback para verificar status do envio
  esp_now_register_send_cb(OnDataSent);

  // Registra peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Adiciona peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Falha para Adicionar peer");
    return;
  }
}
 
void loop() {
  
  // envia mensagem
  Serial.print("Enviando: "); 
  Serial.println(data);
  esp_now_send(broadcastAddress, &data, sizeof(data));
  data++;
  delay(1000);
}


