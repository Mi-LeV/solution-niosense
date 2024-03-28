#include "stdio.h"
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freeRTOS\freeRTOS.h"
#include "freeRTOS\task.h"

#include "light.h"

#define CONST_TEMPS 1000000 // 1sec

static void periodic_timer_callback(void* arg);

volatile uint8_t timer_instance = 0;

void app_main() 
{
    //creer les arguments pour les timers
    const esp_timer_create_args_t periodic_timer_args = 
    {
        .callback = &periodic_timer_callback,
        .name = "periodic"
    }; 
    
    esp_timer_handle_t periodic_timer;
    //creer le timer
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));

    //demarre le timer
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, CONST_TEMPS));

    uint8_t event_value;

    init_light();

    while(1)
    {
        algo_light(&timer_instance, event());
    }
}

static void periodic_timer_callback(void* arg)
{
    timer_instance++;
}