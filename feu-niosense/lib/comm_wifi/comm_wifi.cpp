#include "comm_wifi.h"

// Log debug informations about transmisson 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGD(TAG, "\r\nLast Packet Send Status:\t");
    if(status == ESP_NOW_SEND_SUCCESS){
        ESP_LOGE(TAG,"Data Delivery Fail");
    }
}

// The slave_payloads are received asynchronously, and saved inside the slave_payload[] list
void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
  uint8_t slave_id = 2;

  for (int n = 0 ; n < 6 ; n++){
    if ( mac[n] != mac_adresses[0][n]){
      slave_id = 0;
      break;
    }
    if ( mac[n] != mac_adresses[1][n]){
      slave_id = 1;
      break;
    }
  }
  if (slave_id == 2){
    ESP_LOGE(TAG, "MAC adress of received message is not in the list : ");
    ESP_LOGE(TAG, "%02x:%02x:%02x:%02x:%02x:%02x : ",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }

  memcpy(&slave_payload[slave_id], incomingData, sizeof(slave_payload));
  
  ESP_LOGI(TAG," Recieved ");
  ESP_LOGI(TAG, len);
  ESP_LOGI(TAG," bytes from %02x:%02x:%02x:%02x:%02x:%02x : ",mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  ESP_LOGI(TAG, slave_payload[slave_id].connection_status);
  ESP_LOGI(TAG, slave_payload[slave_id].command_response);
  ESP_LOGI(TAG, slave_payload[slave_id].position);

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
  for (int slave_nb = 0 ; slave_nb < NB_SLAVES ; slave_nb++){
    // Register peer
    memcpy(peerInfo.peer_addr, mac_adresses[slave_nb], 6);
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      ESP_LOGE(TAG,"Failed to add slave %s",slave_nb);
      return;
    }
  }

}

// Send the current master_payload struct to each slave
void send_comm_wifi(){
  
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