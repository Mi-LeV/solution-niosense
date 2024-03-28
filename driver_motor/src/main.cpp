#include <Arduino.h>
#include "AccelStepper.h"

#define GPIO_ENABLE GPIO_NUM_25
#define GPIO_DIR GPIO_NUM_32
#define GPIO_STEP GPIO_NUM_33

#define GPIO_MS1 GPIO_NUM_26
#define GPIO_MS2 GPIO_NUM_27

uint64_t nb_steps = 200;
uint8_t direction = 1;
AccelStepper stepper(AccelStepper::DRIVER, GPIO_STEP, GPIO_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5


// put function declarations here:
int myFunction(int, int);

void setup() {
	pinMode(GPIO_ENABLE, OUTPUT);
	pinMode(GPIO_DIR, OUTPUT);
	pinMode(GPIO_STEP, OUTPUT);
	pinMode(GPIO_MS1, OUTPUT);
	pinMode(GPIO_MS2, OUTPUT);
	pinMode(GPIO_NUM_2, OUTPUT);
	// stepper.setMaxSpeed(1000);
	// stepper.setSpeed(50);	

	//stepper.setEnablePin(GPIO_ENABLE);
    //stepper.enableOutputs();
    stepper.setMaxSpeed(1000000);
    stepper.setSpeed(100000);
	stepper.setAcceleration(500);
	// put your setup code here, to run once:
	// int result = myFunction(2, 3);
	// uint64_t nb_steps = 200;
	// uint8_t direction = 1;
	digitalWrite(GPIO_MS1, HIGH);
	digitalWrite(GPIO_MS2, HIGH);
}

void loop() {
	// put your main code here, to run repeatedly:
	//stepper.runSpeed();
	stepper.moveTo(1000000);
  while (stepper.currentPosition() != 500000) // Full speed up to 300
    stepper.run();
  stepper.runToNewPosition(0); // Cause an overshoot then back to 0

	digitalWrite(GPIO_NUM_2, HIGH);
	// digitalWrite(GPIO_DIR, direction);
	// for(uint64_t i = 0; i < nb_steps; i++)
	// {
	// 	if(i < nb_steps)
	// 	digitalWrite(GPIO_STEP, HIGH);
	// 	// gpio_set_level(GPIO_NUM_2, 1);
	// 	usleep(1000);
	// 	digitalWrite(GPIO_STEP, LOW);
	// 	// gpio_set_level(GPIO_NUM_2, 0);
	// 	usleep(1000);
	// }
	// digitalWrite(GPIO_ENABLE, HIGH);
	//usleep(1000000);
}

// put function definitions here:
int myFunction(int x, int y) {
	return x + y;
}

// defines pins numbers
// const int stepPin = 3;
// const int directionPin = 4;
// const int enablePin = 5;
