/*********************************************************
* Código para Receber dado via ESP NOW
* Por: Fábio Souza
*********************************************************/

#include <Arduino.h>

/*bibliotecas para ESPNOW*/
#include <esp_now.h>
#include <WiFi.h>

// função callback para tratar dado recebido
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Pacote recebido de: "); 
  Serial.println(macStr);
  Serial.print("Dado Recebido: "); 
  Serial.println(*data);
  Serial.println("");
}

void setup() {
  // Inicia comunicação Serial
  Serial.begin(9600);
 
  // Configura Wi-Fi como Station
  WiFi.mode(WIFI_STA);

  // Inicia ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  
}


