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

#define START_POINT 0
#define END_POINT 10000

#define DIR_ARRIERE 0
#define DIR_AVANT 1


void init_motor(void);

#endif