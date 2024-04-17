
//#include "comm_wifi.h"
#include "comm_nrf24.h"

uint8_t desired_speed = 0;
traffic_light_color traffic_light_state = red;
uint16_t position;

void setup(){
  Serial.begin(115200);
  Serial.print("BOARD : CAMION");
  Serial.println(SLAVE_ID+1);
  

  init_comm_nrf24();

  Serial.println("Waiting for payload from master...");
}

void loop(){

  // get current desired speed
  desired_speed = master_payload.desired_speed[SLAVE_ID];
  // get current traffic light state
  traffic_light_state = master_payload.traffic_light_state;


  // mirror the simulated RF comm
  slave_payload.connection_status = master_payload.connection_status[SLAVE_ID];


  if ( master_payload.command == master_init){
    //go to initial position
    //if (at_initial_position){slave_payload.command_response = init_finished}
    
    if ( master_payload.command == master_go){
      //go at desired speed
      slave_payload.command_response = slave_na;
  }else{// command stop
    //stop motor
    slave_payload.command_response = slave_na;
  }
  }
  slave_payload.position = position; // update current position

  radioCheckAndReply();
}