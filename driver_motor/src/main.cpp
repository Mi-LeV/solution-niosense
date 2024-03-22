#include <Arduino.h>

#define GPIO_ENABLE GPIO_NUM_25
#define GPIO_DIR GPIO_NUM_32
#define GPIO_STEP GPIO_NUM_32

#define GPIO_MS1 GPIO_NUM_27
#define GPIO_MS2 GPIO_NUM_26

uint64_t nb_steps = 200;
uint8_t direction = 1;

// put function declarations here:
int myFunction(int, int);

void setup() {
	pinMode(GPIO_ENABLE, OUTPUT);
	pinMode(GPIO_DIR, OUTPUT);
	pinMode(GPIO_STEP, OUTPUT);
	// put your setup code here, to run once:
	// int result = myFunction(2, 3);
	// uint64_t nb_steps = 200;
	// uint8_t direction = 1;
}

void loop() {
	// put your main code here, to run repeatedly:
	digitalWrite(GPIO_ENABLE, LOW);
	digitalWrite(GPIO_DIR, direction);
	for(uint64_t i = 0; i < nb_steps; i++)
	{
		digitalWrite(GPIO_STEP, HIGH);
		// gpio_set_level(GPIO_NUM_2, 1);
		usleep(1000);
		digitalWrite(GPIO_STEP, LOW);
		// gpio_set_level(GPIO_NUM_2, 0);
		usleep(1000);
	}
	digitalWrite(GPIO_ENABLE, HIGH);
	usleep(1000000);
}

// put function definitions here:
int myFunction(int x, int y) {
	return x + y;
}