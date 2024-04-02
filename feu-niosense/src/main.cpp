#include <Arduino.h>
#include "server.h"
#include "comm_nrf24.h"

// Boutons et paramètres de l'interface (déclarés dans le module server)
extern bool btn_start, btn_pause, btn_stop;
extern int vitesse1_desire, vitesse2_desire, distance_desire;

/*** TEMPORAIRE : statut de connexion des deux camions ***/
// (Devrait venir du module NRF)
bool status1 = true, status2 = true;

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  //init_comm_nrf24();
  init_server();
}

void loop(){

  //get_etape_test() ( go, stop)
  //position_camions = slave_payloads
  //if (calcul_interferences()) : master_payload.ping = True
  //create_master_payload() ( status led, command)



  //send_and_receive_comm_nrf();


  //algo_light()
  handle_client();
  //upload_data()
}
