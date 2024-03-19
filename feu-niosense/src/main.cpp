
#include "comm_wifi.h"
// #include "comm_nrf24.h"

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  init_comm_wifi();

  // init_comm_nrf24();
}

void loop(){

  master_payload.command = (master_payload.command ++) % 4;
  master_payload.connection_status != master_payload.connection_status;
  master_payload.traffic_light_state = (master_payload.traffic_light_state ++) % 3;

  send_comm_wifi();

  while(1);
  delay(1000);
}