#include "comm_nrf24.h"
 #include "light.h"


hw_timer_t * timer = NULL;

volatile uint8_t timer_instance = 0;

bool status1 = false;
bool status2 = false;
int nb_deconnexions_1, nb_deconnexions_2;

bool test_initialise = true;

// Boutons et paramètres de l'interface (déclarés dans le module server)
extern uint8_t etat_btn;
extern int vitesse1_desire, vitesse2_desire, distance_desire;

uint8_t algo = ALGO_FIXE;

void IRAM_ATTR timer_isr() 
{
    timer_instance++;
    // This code will be executed every 1000000 ticks, 1s?
}

void setup(){
  Serial.begin(115200);
  //Serial.println("BOARD : MASTER");
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

  //get_etape_test() ( go, stop)
  //position_camions = slave_payloads
  //if (calcul_interferences()) : master_payload.ping = True
  //create_master_payload() ( status led, command)


  algo_light(&timer_instance, event());
  send_and_receive_comm_nrf();

  // test en cours
  if(etat_btn == START){
    // initialisation du test
    /*if ( test_initialise ){
      master_payload.command = master_init;
      master_payload.desired_speed = vitesse1_desire;
    // deroulement normal
    }else{*/
      master_payload.command = master_go;
      master_payload.desired_speed_1 = vitesse1_desire;
      master_payload.desired_speed_2 = vitesse2_desire;

    //}
  // test mis sur pause
  }else if(etat_btn == PAUSE){
    master_payload.command = master_stop;
  // test arrêté
  }else{
    master_payload.command = master_stop;
  }
  
  handle_client();
  //algo_light()
  //serve_webpage()
  //upload_data()
}