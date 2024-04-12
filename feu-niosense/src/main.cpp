 #include "comm_nrf24.h"

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  init_comm_nrf24();
}

void loop(){

  // lire position camion des slave payload
  //if(go == true){
    //if (calcul_interferences()) : master_payload.ping = True
    //if (init == true) : master_command = initialize
  //  master_command = go
  //}else{
    //master_command = stop
  //}
  //create_master_payload() ( status led, command)
  


  send_and_receive_comm_nrf();
  delay(1000);

  //algo_light()
  //serve_webpage()
  //upload_data()
}