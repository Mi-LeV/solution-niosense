#include <stdio.h>
#include "driver/gpio.h"
// #include "esp_timer.h"
#include "driver/ledc.h"
#include "timer.h"

#define PIN_ON 1
#define PIN_OFF 0
// #include "esp_log.h"
// #include "esp_http_server.h"
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          GPIO_NUM_2 // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_8_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (127) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

// static void periodic_timer_callback(void* arg);
uint8_t do_steps;

static void example_ledc_init(void)
{
	// Prepare and then apply the LEDC PWM timer configuration
	ledc_timer_config_t ledc_timer = {
		.speed_mode       = LEDC_MODE,
		.duty_resolution  = LEDC_DUTY_RES,
		.timer_num        = LEDC_TIMER,
		.freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
		.clk_cfg          = LEDC_AUTO_CLK
	};
	ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

	// Prepare and then apply the LEDC PWM channel configuration
	ledc_channel_config_t ledc_channel = {
		.speed_mode     = LEDC_MODE,
		.channel        = LEDC_CHANNEL,
		.timer_sel      = LEDC_TIMER,
		.intr_type      = LEDC_INTR_DISABLE,
		.gpio_num       = LEDC_OUTPUT_IO,
		.duty           = 0, // Set duty to 0%
		.hpoint         = 0
	};
	ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void app_main()
{
	// ESP_ERROR_CHECK(nvs_flash_init());
	
	printf("Hello world!\n");
	gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
	gpio_set_level(GPIO_NUM_2, 0);

	//init_timer();
	init_steps_timer(500000);

	// Set the LEDC peripheral configuration
    // example_ledc_init();
    // Set duty to 50%
    // ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    // // Update duty to apply the new value
    // ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
	do_steps = 100;

	while(1)
	{
		if(do_steps)
		{
			//uint8_t level = 0;
			//level = gpio_get_level(GPIO_NUM_2);
			do_steps -= ((do_steps == 2)*2);
			gpio_set_level(GPIO_NUM_2, do_steps);
			// gpio_set_level(GPIO_NUM_2, !gpio_get_level(GPIO_NUM_2));
			//do_steps--;
		}
		//gpio_set_level(GPIO_NUM_2, 0);
		//printf("Hello world!\n");
		//gpio_set_level(GPIO_NUM_2, cnt%2);
	}
}

// move(uint8_t dir, uint32_t steps)
// {

// }
