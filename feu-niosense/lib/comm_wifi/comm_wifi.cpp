#include "comm_wifi.h"

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGD(TAG, "\r\nLast Packet Send Status:\t");
    ESP_LOGD(TAG, status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
  /*
  The slave_payloads are received asynchronously, and saved inside the slave_payload[] list
  */
  memcpy(&slave_temp_payload, incomingData, sizeof(slave_temp_payload));
  uint8_t slave_id = slave_temp_payload.id;
  memcpy(&slave_payload[slave_id], &slave_temp_payload, sizeof(slave_payload));
  
  ESP_LOGI(TAG," Recieved ");
  ESP_LOGI(TAG, len);
  ESP_LOGI(TAG, " bytes : ");

  ESP_LOGI(TAG, slave_payload[slave_id].id);
  ESP_LOGI(TAG, slave_payload[slave_id].connection_status);
  ESP_LOGI(TAG, slave_payload[slave_id].command_response);
  ESP_LOGI(TAG, slave_payload[slave_id].position);

}
 
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
  for (int slave_nb = 0 ; slave_nb < NB_SLAVES ; slave_nb++){
    // Register peer
    memcpy(peerInfo.peer_addr, mac_adresses[slave_nb], 6);
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      ESP_LOGE(TAG,"Failed to add peer1");
      return;
    }
  }

}
 
void send_comm_wifi(){
  /*
  Sends the current master_payload to each slave

  To use it : 
  1. Modify master_payload
  2. Call this function
*/

  for (int slave_nb = 0 ; slave_nb < NB_SLAVES ; slave_nb++){
    esp_err_t result = esp_now_send(mac_adresses[slave_nb], (uint8_t *) &master_payload, sizeof(master_payload));

    if (result == ESP_OK) {
      ESP_LOGI(TAG,"Sent with success");
    }
    else {
      ESP_LOGE(TAG,"Error sending the data");
    }
  }
}