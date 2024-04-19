#include "comm_wifi.h"

// Log debug informations about transmisson 
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    //Serial.print( "\r\nLast Packet Send Status:\t");
    if(status == ESP_NOW_SEND_SUCCESS){
       // Serial.println("Data Delivery Fail");
    }
    else{
     // Serial.println("Data Delivery Success");
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
    //Serial.print( "MAC adress of received message is not in the list : ");
    for(int n = 0 ; n < 6; n++){
    //Serial.print(mac[n],HEX);
    //Serial.print(" : ");
    }
    //Serial.print("\n");
  }

  memcpy(&slave_payload[slave_id], incomingData, sizeof(slave_payload));
  
  //Serial.println(" Recieved ");
  //Serial.println( len);
  //Serial.print(" from ");
  for(int n = 0 ; n < 6; n++){
    //Serial.print(mac[n],HEX);
    //Serial.print(" : ");
  }
  //Serial.print("\n");

  //Serial.println( slave_payload[slave_id].connection_status);
  //Serial.println( slave_payload[slave_id].command_response);
  //Serial.println( slave_payload[slave_id].position);

}

// Initialize ESP_NOW comm
void init_comm_wifi() {

  //Serial.print("ESP Board MAC Address:  ");
  //Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, NULL);
  //Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print('.');
    delay(1000);
  }
  //Serial.println(WiFi.localIP());
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //Serial.println( "Error initializing ESP-NOW");
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
      //Serial.print("Failed to add slave ");
      //Serial.println(slave_nb);
    }
    else{
      //Serial.print("Added slave ");
      //Serial.print(slave_nb);
      //Serial.println(" successfully ");

    }
  }

}

// Send the current master_payload struct to each slave
void send_comm_wifi(){
  
  for (int slave_nb = 0 ; slave_nb < NB_SLAVES ; slave_nb++){
    esp_err_t result = esp_now_send(mac_adresses[slave_nb], (uint8_t *) &master_payload, sizeof(master_payload));

    if (result == ESP_OK) {
      //Serial.println("Master payload sent");
    }
    else {
      //Serial.println("Error sending the data");
    }
  }
}