 #include "comm_nrf24.h"

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  init_comm_nrf24();
}

void loop(){

  //get_etape_test() ( go, stop)
  //position_camions = slave_payloads
  //if (calcul_interferences()) : master_payload.ping = True
  //create_master_payload() ( status led, command)



  send_and_receive_comm_nrf();


  //algo_light()
  //serve_webpage()
  //upload_data()
}