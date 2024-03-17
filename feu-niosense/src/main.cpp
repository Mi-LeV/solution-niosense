
#include "comm_wifi.h"
// #include "comm_nrf24.h"

//const char* ssid = "Redmi Note 9";
const char* ssid = "ETS-Invites";

const char* password = "xxx";

MasterPayloadStruct master_payload;
SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads
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

  // init_comm_nrf24();
}

void loop(){

  master_payload.command = (master_payload.command ++) % 4;
  master_payload.connection_status != master_payload.connection_status;
  master_payload.traffic_light_state = (master_payload.traffic_light_state ++) % 3;

  send_comm_wifi();

  delay(1000);
}