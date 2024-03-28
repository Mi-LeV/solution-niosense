 #include "comm_nrf24.h"
 #include "light.h"

hw_timer_t * timer = NULL;

volatile uint8_t timer_instance = 0;

void IRAM_ATTR timer_isr() 
{
    timer_instance++;
    // This code will be executed every 1000000 ticks, 1s?
}

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
}

void loop(){

  //get_etape_test() ( go, stop)
  //position_camions = slave_payloads
  //if (calcul_interferences()) : master_payload.ping = True
  //create_master_payload() ( status led, command)


  algo_light(&timer_instance, 0);
  send_and_receive_comm_nrf();
  

  //algo_light()
  //serve_webpage()
  //upload_data()
}