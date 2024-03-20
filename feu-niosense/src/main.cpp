
//#include "comm_wifi.h"
 #include "comm_nrf24.h"

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  //init_comm_wifi();

  init_comm_nrf24();
}

void loop(){

  //send_comm_wifi();
  send_and_receive_comm_nrf();
  delay(1000);
}