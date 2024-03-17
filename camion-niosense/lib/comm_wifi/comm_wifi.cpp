#include "comm_wifi.h"

// Log debug informations about transmisson 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGD(TAG, "\r\nLast Packet Send Status:\t");
    if(status == ESP_NOW_SEND_SUCCESS){
        ESP_LOGE(TAG,"Data Delivery Fail");
    }
}

// When the master payload is received, store it in master_payload global struct,
// and immediately send the slave_payload global struct to the master
void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&master_payload, incomingData, sizeof(master_payload));
  
  ESP_LOGI(TAG," Recieved ");
  ESP_LOGI(TAG, len);
  ESP_LOGI(TAG," bytes from  %02x:%02x:%02x:%02x:%02x:%02x : ",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  ESP_LOGI(TAG, master_payload.connection_status);
  ESP_LOGI(TAG, master_payload.traffic_light_state);
  ESP_LOGI(TAG, master_payload.command);

  esp_err_t result = esp_now_send(mac_adress, (uint8_t *) &slave_payload, sizeof(slave_payload));

  if (result == ESP_OK) {
    ESP_LOGI(TAG,"Sent with success");
  }
  else {
    ESP_LOGE(TAG,"Error sending the data");
  }
}

// Initialize ESP_NOW comm
void init_comm_wifi() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    ESP_LOGE(TAG, "Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_recv_cb(OnDataReceived);
  esp_now_register_send_cb(OnDataSent);
  
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, mac_adress, 6);
    
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    ESP_LOGE(TAG,"Failed to add master peer");
    return;
   }

}