#ifndef MOTOR
#define MOTOR

#include <Arduino.h>
#include "AccelStepper.h"

#define GPIO_ENABLE GPIO_NUM_25
#define GPIO_DIR GPIO_NUM_32
#define GPIO_STEP GPIO_NUM_33

#define GPIO_MS1 GPIO_NUM_26
#define GPIO_MS2 GPIO_NUM_27

#define MAX_SPEED 4000
#define ACCELERATION 2000

#define DIR_ARRIERE 0
#define DIR_AVANT 1

#define START_POINT 0
#define END_POINT 100000

#define LONG_INTERSECTION (END_POINT/10)

//pris comme si le camion avance
#define DIST_START (END_POINT/5)      //distance ou la communication commence
#define DIST_LOST (DIST_START*4)      //distance ou la communication est perdu
#define DIST_PASS_LOW ((END_POINT/2)-(LONG_INTERSECTION/2))       //distance ou le camion est considéré à l'intersection
#define DIST_PASS_HIGH ((END_POINT/2)+(LONG_INTERSECTION/2))      //distance ou le camion sort de l'intersection 


void init_motor(void);

#endif