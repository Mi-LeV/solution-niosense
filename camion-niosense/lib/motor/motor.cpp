#include "motor.h"


void init_motor(void)
{
	pinMode(GPIO_ENABLE, OUTPUT);
	pinMode(GPIO_DIR, OUTPUT);
	pinMode(GPIO_STEP, OUTPUT);
	pinMode(GPIO_MS1, OUTPUT);
	pinMode(GPIO_MS2, OUTPUT);

	digitalWrite(GPIO_MS1, LOW);
	digitalWrite(GPIO_MS2, HIGH);

}