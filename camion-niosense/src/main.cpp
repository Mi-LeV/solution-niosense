
#include "comm_wifi.h"
// #include "comm_nrf24.h"

//const char* ssid = "Redmi Note 9";
const char* ssid = "ETS-Invites";

const char* password = "xxxxx";

MasterPayloadStruct master_payload;
SlavePayloadStruct slave_payload;
esp_now_peer_info_t peerInfo;

void setup(){
  Serial.begin(115200);

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, NULL);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  init_comm_wifi();

  Serial.println("Waiting for payload from master...");
}

void loop(){
}