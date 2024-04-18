//#include "comm_wifi.h"
#include <stdlib.h>
#include "comm_nrf24.h"
#include "motor.h"

#define LIMIT_SWITCH_0 14
#define LIMIT_SWITCH_1 13
#define GPIO_BLUE_LED GPIO_NUM_2

#define GREEN   0
#define YELLOW  1
#define RED     2

uint64_t nb_steps = 200;
uint8_t direction = DIR_AVANT;
AccelStepper stepper(AccelStepper::DRIVER, GPIO_STEP, GPIO_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
uint64_t go_to_pos = 0;
uint8_t flag_pressed = false;
uint8_t flag_red_light = false;
uint8_t flag_init = false;
uint8_t flag_go = false;
uint8_t flag_debut = false;
uint8_t flag_fin = false;
uint8_t speed = 50;
long track_lenght = END_POINT;
long distance_start = DIST_START;
long distance_lost = DIST_LOST;
long distance_pass_low = DIST_PASS_LOW;
long distance_pass_high = DIST_PASS_HIGH;


uint64_t cnt_printf = 0;

void setup(){
  Serial.begin(115200);
 // Serial.println("BOARD : CAMION1");
  
  pinMode(LIMIT_SWITCH_0, INPUT);
  pinMode(LIMIT_SWITCH_1, INPUT);
  pinMode(GPIO_BLUE_LED, OUTPUT);

  stepper.setMaxSpeed(MAX_SPEED*speed);
	stepper.setAcceleration(ACCELERATION);

  init_comm_nrf24();
  init_motor();

	//stepper.moveTo(track_lenght);

  Serial.println("Waiting for payload from master...");

  stepper.setCurrentPosition(track_lenght + 5000); //5000 est un test, à changer
  // printf("\033[2J");

}

void loop()
{
  // printf("\033[0;0H");
  /*stepper.setSpeed(800);
  go_to_pos = track_lenght;
 // if(go_to_pos == track_lenght)
  //  go_to_pos = START_POINT;
  //else
 //   go_to_pos = track_lenght;
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
  if(!flag_init)
  {
    if(((!digitalRead(LIMIT_SWITCH_0))||(!digitalRead(LIMIT_SWITCH_1))) && !flag_pressed)  //si la limit switch est presse 
    {
      flag_pressed = true;
      if(direction == DIR_AVANT)
      {
        track_lenght = stepper.currentPosition() + 100;
        stepper.setCurrentPosition(track_lenght);
        stepper.moveTo(START_POINT);
        direction = !direction;
        distance_start = track_lenght/5;
        distance_lost = distance_start*4;
        distance_pass_low = ((track_lenght/2)-((track_lenght/3)/2));
        distance_pass_high = ((track_lenght/2)+((track_lenght/3)/2));
        flag_init = true;
        flag_go = true;
        //printf("%d\r\n",0);
      }
      else
      {
        stepper.setCurrentPosition(START_POINT);
        stepper.moveTo(track_lenght+5000); //5000 est un test, à changer
        direction = !direction;
        flag_debut = true;
      //  printf("%d\r\n",1);
      }
    }
    else if(digitalRead(LIMIT_SWITCH_0) && digitalRead(LIMIT_SWITCH_1) && flag_pressed)//si la limit switch n'est plus presse
    {
      flag_pressed = false;
       // printf("%d\r\n",2);
    }

    else if(!flag_debut)
    {
      direction = DIR_ARRIERE;
      stepper.moveTo(START_POINT);
      stepper.run();
      //printf("%d\r\n",3);
    }
    else 
    {
      direction = DIR_AVANT;
      stepper.run();
       // printf("%d\r\n",4);
    }
  }

  else if(flag_go && master_payload.command==master_go)
  {
    if(((!digitalRead(LIMIT_SWITCH_0))||(!digitalRead(LIMIT_SWITCH_1)))&& !flag_pressed)
    {
      flag_pressed = true;
      if(direction == DIR_AVANT)
      {
        stepper.setCurrentPosition(track_lenght);
        stepper.moveTo(START_POINT);
        direction = !direction;
       // printf("%d\r\n",5);
      }
      else
      {
        stepper.setCurrentPosition(START_POINT);
        stepper.moveTo(track_lenght);
        direction = !direction;
        //printf("%d\r\n",6);
      }
    //stepper.stop();
    }
    else if(digitalRead(LIMIT_SWITCH_0) && digitalRead(LIMIT_SWITCH_1) && flag_pressed)
    {
      flag_pressed = false;
       // printf("%d\r\n",7);
    }
    //else
    //{
      // printf("dist to go: %d\r\n", stepper.distanceToGo());
      //si la lumiere est rouge, qu'il va vert l'avant et qu'il ne n'a pas passe l'intersection
      if((master_payload.traffic_light_state == RED) && (direction == DIR_AVANT) && (stepper.currentPosition() < distance_pass_low))
      {
        stepper.moveTo(distance_pass_low);
        flag_red_light = true;
        stepper.run();
       // printf("%d\r\n",8);
      }
      //si la lumiere est rouge, qu'il va vert l'arriere et qu'il ne n'a pas passe l'intersection
      else if((master_payload.traffic_light_state == RED) && (direction == DIR_ARRIERE) && (stepper.currentPosition() > distance_pass_high))
      {
        stepper.moveTo(distance_pass_high);
        flag_red_light = true;
        stepper.run();
        //printf("%d\r\n",9);
      }
      else if((master_payload.traffic_light_state == RED) && (direction == DIR_AVANT) && (stepper.currentPosition() > distance_pass_low))
      {
        if(!stepper.distanceToGo())
        {
          if(direction == DIR_AVANT)
          {
              stepper.setCurrentPosition(track_lenght);
              stepper.moveTo(START_POINT);
              direction = !direction;
             // printf("%d\r\n",10); 
          }
          else
          {
            stepper.setCurrentPosition(START_POINT);
            stepper.moveTo(track_lenght);
            direction = !direction;
            //printf("%d\r\n",11);
          }
        }
        //printf("%d\r\n",20);
        stepper.run();
      }
      else if((master_payload.traffic_light_state == RED) && (direction == DIR_ARRIERE) && (stepper.currentPosition() < distance_pass_high))
      {
        if(!stepper.distanceToGo())
        {
          if(direction == DIR_AVANT)
          {
              stepper.setCurrentPosition(track_lenght);
              stepper.moveTo(START_POINT);
              direction = !direction;
             // printf("%d\r\n",12);
          }
          else
          {
            stepper.setCurrentPosition(START_POINT);
            stepper.moveTo(track_lenght);
            direction = !direction;
           // printf("%d\r\n",13);
          }
        }
        stepper.run();
      }
      else if(master_payload.traffic_light_state != RED)
      {
        if(!stepper.distanceToGo())
        {
          if(direction == DIR_AVANT)
          {
            if(flag_red_light)
            {
              stepper.moveTo(track_lenght);
              flag_red_light = false;
            //printf("%d\r\n",14);
            }
            else
            {
              stepper.setCurrentPosition(track_lenght);
              stepper.moveTo(START_POINT);
              direction = !direction;
            //printf("%d\r\n",15);
            }
          }
          else
          {
            if(flag_red_light)
            {
              stepper.moveTo(START_POINT);
              flag_red_light = false;
            //printf("%d\r\n",16);
            }
            else
            {
              stepper.setCurrentPosition(START_POINT);
              stepper.moveTo(track_lenght);
              direction = !direction;
           // printf("%d\r\n",17);
            }
          }
        }
        stepper.setMaxSpeed(MAX_SPEED * speed);
        stepper.run();
      }
      //else
      //{
      // stepper.setMaxSpeed(0);
      //}
    //}
    digitalWrite(GPIO_BLUE_LED, flag_pressed);
  }
    radioCheckAndReply();
    cnt_printf++;
    if(!SLAVE_ID)
    {
      speed = master_payload.desired_speed_1;
    }
    else
    {
      speed = master_payload.desired_speed_2;
    }
    if(cnt_printf >= 3000)
    {
      
      cnt_printf = 0;
      /*
      printf("status : %d     \r\n", master_payload.connection_status);
      printf("light  : %d     \r\n", master_payload.traffic_light_state);
      printf("command: %d     \r\n\r\n", master_payload.command);
      printf("dir: %s\r\n",(direction == DIR_AVANT)?"AVANT":"ARRIERE");
      printf("pos: %d\r\n",stepper.currentPosition());
      printf("track_lenght: %d\r\n",track_lenght);
      printf("mode: %d\r\n",flag_init);
*/
      /*printf("pos: %d\r\n",stepper.currentPosition());
      printf("low: %d\r\n",distance_pass_low);
      printf("high: %d\r\n",distance_pass_high);*/
      //fflush(stdout);
    }
}