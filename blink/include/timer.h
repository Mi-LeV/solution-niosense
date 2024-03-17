#ifndef TIMER_H
#define TIMER_H
#include "esp_timer.h"

void init_periodic_timer();
void init_steps_timer(uint64_t time);
void stop_steps_timer();
void steps_timer_callback(void* arg);
void periodic_timer_callback(void* arg);

#endif