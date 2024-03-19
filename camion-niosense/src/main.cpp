
#include "comm_wifi.h"
// #include "comm_nrf24.h"

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : CAMION1");
  

  init_comm_wifi();

  Serial.println("Waiting for payload from master...");
}

void loop(){
}