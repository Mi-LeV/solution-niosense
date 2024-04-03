
//#include "comm_wifi.h"
#include "comm_nrf24.h"

void setup(){
  Serial.begin(115200);
  Serial.print("BOARD : CAMION");
  Serial.println(SLAVE_ID+1);
  

  init_comm_nrf24();

  Serial.println("Waiting for payload from master...");
}

void loop(){
  radioCheckAndReply();
}