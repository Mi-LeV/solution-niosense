#ifndef LIGHT
#define LIGHT

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "Arduino.h"

#define LED_G1 21
#define LED_Y1 17
#define LED_R1 16
#define LED_G2 25
#define LED_Y2 33
#define LED_R2 32

#define ON  1
#define OFF 0

#define EVENT_1  1
#define EVENT_2  2
#define NO_EVENT 0

#define GREEN   0
#define YELLOW  1
#define RED     2

#define TIME_GREEN   4
#define TIME_YELLOW  2
#define TIME_RED     4

#define MAX_SIZE_STACK 256

typedef enum 
{
    green_n,
    yellow_n,
    red_n,
    event_1_n,
    event_2_n,
    
}situation;

void init_light(void);

void set_light(uint8_t light);

uint8_t event(void);

void algo_light(volatile uint8_t* timer, uint8_t event);

#endif