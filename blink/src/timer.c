#include <timer.h>

extern uint8_t do_steps;

uint8_t cnt = 0;
esp_timer_handle_t periodic_timer;
esp_timer_handle_t steps_timer;

void init_periodic_timer()
{
    const esp_timer_create_args_t periodic_timer_args =
	{
		.callback = &periodic_timer_callback,
		/* name is optional, but may help identify the timer when debugging */
		.name = "periodic"
	};

	ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
	/* Start the timers */
	ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 5000));
}

void init_steps_timer(uint64_t us_time)
{
    const esp_timer_create_args_t steps_timer_args =
	{
		.callback = &steps_timer_callback,
		/* name is optional, but may help identify the timer when debugging */
		.name = "periodic"
	};

	ESP_ERROR_CHECK(esp_timer_create(&steps_timer_args, &steps_timer));
	/* Start the timers */
	ESP_ERROR_CHECK(esp_timer_start_periodic(steps_timer, us_time));
}

void stop_steps_timer()
{
	ESP_ERROR_CHECK(esp_timer_stop(steps_timer));
}

void periodic_timer_callback(void* arg)
{
	cnt++;
	if(cnt == 255)
	{
		cnt = 0;
		// esp_timer_stop(periodic_timer);
	}
	// // Set duty to 50%
    // ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, cnt));
    // // Update duty to apply the new value
    // ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
	int64_t time_since_boot = esp_timer_get_time();
	printf("Periodic timer called, time since boot: %lld us\r\n", time_since_boot);
	// ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);
}

void steps_timer_callback(void* arg)
{
	do_steps++;
}
