#include "comm_nrf24.h"
#include "light.h"
#include "calcul_dist.h"
#include "wifi_server.h"

hw_timer_t * timer = NULL;
volatile uint8_t timer_instance = 0;

void IRAM_ATTR timer_isr() 
{
    timer_instance++;
    // This code will be executed every 1000000 ticks, 1s?
}

// Boutons et paramètres de l'interface (déclarés dans le module server)
extern uint8_t etat_btn;
extern int vitesse1_desire, vitesse2_desire, distance_desire;
extern situation sit_lum;

bool test_initialise = true;

/*** TEMPORAIRE : statut de connexion des deux camions ***/
// (Devrait venir du module NRF)
bool status1 = true, status2 = true;
int nb_deconnexions_1, nb_deconnexions_2;

// Structure pour la communication avec les camions
extern MasterPayloadStruct master_payload;

//variable de logique de la loop
uint8_t current_node;
bool previous_slave_conn_status [NB_SLAVES] = {false, false};

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  uint8_t timer_id = 0;
  uint16_t prescaler = 80; // Between 0 and 65 535
  int threshold = 1000000; // 64 bits value (limited to int size of 32bits)

  timer = timerBegin(timer_id, prescaler, true);
  timerAttachInterrupt(timer, &timer_isr, true);
  timerAlarmWrite(timer, threshold, true);
  timerAlarmEnable(timer);

  init_comm_nrf24();
  init_light();
  init_server();
}

void loop(){

  // test en cours
  if(etat_btn == START){
    // initialisation du test
    if ( test_initialise ){
      master_payload.command = master_init;
    // deroulement normal
    }else{
      master_payload.command = master_go;
      
      for ( current_node = 0 ; current_node < NB_SLAVES ; current_node++){
        long camion[2];// A MODIFIER AVEC LA VRAIE VALEUR
        /////ATTTTTTENTION
        camionAtFeu(camion[current_node]);
        master_payload.connection_status[current_node] = compute_comm(camion[current_node],distance_desire ,RF_SIMUL_PROB_INTERF); 
        // distance_desiree est en cm
      }

    }
  // test mis sur pause
  }else if(etat_btn == PAUSE){
    master_payload.command = master_stop;
  // test arrêté
  }else{
    master_payload.command = master_stop;
  }
  // update transmitted traffic_light state
  master_payload.traffic_light_state = (traffic_light_color) sit_lum ; 
  
  send_and_receive_comm_nrf(); // comm with slaves

  //see reponse from slaves

  //test if there is change in connection status for camions 
  for (current_node = 0 ; current_node < NB_SLAVES ; current_node++){
      if(slave_payload[current_node].connection_status != previous_slave_conn_status[NB_SLAVES]){
        new_line(CONN_STATUS_CHANGED);
        previous_slave_conn_status[NB_SLAVES] = slave_payload[current_node].connection_status ;
      }
    }
  //position_camions = slave_payloads

  // test for end of initialisation from slaves
  if (test_initialise){
    bool temp_init = false;
    //test for end of initialisation response from both payload
    for (uint8_t i = 0 ; i < NB_SLAVES ; i++){
      if(slave_payload[i].command_response != 1){
        temp_init = true;
      }
    }
    test_initialise = temp_init;
  }
  

  algo_light(&timer_instance, 0);
  handle_client();

}