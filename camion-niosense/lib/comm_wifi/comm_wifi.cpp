#include "comm_wifi.h"

// Log debug informations about transmisson 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.println("Last Packet Send Status:\t");
    if(status == ESP_NOW_SEND_SUCCESS){
        Serial.println("Data Delivery Fail");
    }
    else{
      Serial.println("Data Delivery Success");
    }
}

// When the master payload is received, store it in master_payload global struct,
// and immediately send the slave_payload global struct to the master
void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&master_payload, incomingData, sizeof(master_payload));
  
  Serial.println(" Recieved ");
  Serial.println( len);
  Serial.print(" from ");
  Serial.print(mac[0]);
  Serial.print(" : ");
  Serial.print(mac[1]);
  Serial.print(" : ");
  Serial.print(mac[2]);
  Serial.print(" : ");
  Serial.print(mac[3]);
  Serial.print(" : ");
  Serial.print(mac[4]);
  Serial.print(" : ");
  Serial.println(mac[5]);
  Serial.println( master_payload.connection_status);
  Serial.println( master_payload.traffic_light_state);
  Serial.println( master_payload.command);

  esp_err_t result = esp_now_send(mac_adress, (uint8_t *) &slave_payload, sizeof(slave_payload));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

// Initialize ESP_NOW comm
void init_comm_wifi() {

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println( "Error initializing ESP-NOW");
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
    Serial.println("Failed to add master peer");
    return;
   }
  else{
      Serial.println("Added master ");

    }
}