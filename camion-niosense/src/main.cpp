//#include "comm_wifi.h"
#include "comm_nrf24.h"
#include "motor.h"

#define LIMIT_SWITCH_0 14
#define GPIO_BLUE_LED GPIO_NUM_2

uint64_t nb_steps = 200;
uint8_t direction = DIR_AVANT;
AccelStepper stepper(AccelStepper::DRIVER, GPIO_STEP, GPIO_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
uint64_t go_to_pos = 0;
uint8_t flag_pressed = false;

uint64_t cnt_printf = 0;

void setup(){
  Serial.begin(115200);
  Serial.println("BOARD : CAMION1");
  
  pinMode(LIMIT_SWITCH_0, INPUT);
  pinMode(GPIO_BLUE_LED, OUTPUT);

  stepper.setMaxSpeed(MAX_SPEED);
	stepper.setAcceleration(ACCELERATION);

  init_comm_nrf24();
  init_motor();

	stepper.moveTo(END_POINT);

  Serial.println("Waiting for payload from master...");

}

void loop(){
  /*stepper.setSpeed(800);
  go_to_pos = END_POINT;
 // if(go_to_pos == END_POINT)
  //  go_to_pos = START_POINT;
  //else
 //   go_to_pos = END_POINT;
	stepper.moveTo(go_to_pos);
  while (stepper.currentPosition() != go_to_pos)
  {
    if(!digitalRead(LIMIT_SWITCH_0))
    {
      stepper.stop();
    }
    else
      stepper.run();
      usleep(100);
  }
  stepper.runToNewPosition(START_POINT); // Cause an overshoot then back to 0
*/
  if(!digitalRead(LIMIT_SWITCH_0) && !flag_pressed)
  {
    flag_pressed = true;
    if(direction == DIR_AVANT)
    {
      stepper.setCurrentPosition(END_POINT);
      stepper.moveTo(START_POINT);
      direction = !direction;
    }
    else
    {
      stepper.setCurrentPosition(START_POINT);
      stepper.moveTo(END_POINT);
      direction = !direction;
    }
   //stepper.stop();
  }
  else if(digitalRead(LIMIT_SWITCH_0) && flag_pressed)
  {
    flag_pressed = false;
  }
  //else
  //{
    if(stepper.distanceToGo() == 0)
    {
      if(direction == DIR_AVANT)
      {
        stepper.setCurrentPosition(END_POINT);
        stepper.moveTo(START_POINT);
        direction = !direction;
      }
      else
      {
        stepper.setCurrentPosition(START_POINT);
        stepper.moveTo(END_POINT);
        direction = !direction;
      }
    }
    stepper.run();
  //}
  digitalWrite(GPIO_BLUE_LED, flag_pressed);

  radioCheckAndReply();
  // printf("\033[2J");
  cnt_printf++;

  if(cnt_printf >= 3000)
  {
    cnt_printf = 0;
    printf("\033[2J");
    printf("status : %d\r\n", master_payload.connection_status);
    printf("light  : %d\r\n", master_payload.traffic_light_state);
    printf("command: %d\r\n\r\n", master_payload.command);
  }
}