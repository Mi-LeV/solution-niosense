#include "light.h"



uint8_t tab_event[MAX_SIZE_STACK] = {0};
uint8_t tab_size = 0;
situation sit_lum = green_n;
uint8_t flag_reset = false;
uint8_t last_light = GREEN;

static void erase_first(uint8_t* tab, uint8_t* tab_size)
{
    for(uint8_t i = 0; i < *tab_size; i++)
        tab[i] = tab[i+1];
    (*tab_size)--;
}

static void add_event(uint8_t event, uint8_t* tab, uint8_t* tab_size)
{
    tab[*tab_size++] = event;
}

void init_light(void)
{
    gpio_set_direction(LED_G1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_Y1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_R1, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_G2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_Y2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_R2, GPIO_MODE_OUTPUT);
    
    gpio_set_direction(PB_1, GPIO_MODE_INPUT);
    gpio_set_direction(PB_2, GPIO_MODE_INPUT);
}

void set_light(uint8_t light)
{   
    gpio_set_level(LED_G1, OFF);
    gpio_set_level(LED_Y1, OFF);
    gpio_set_level(LED_R1, OFF);
    gpio_set_level(LED_G2, OFF);
    gpio_set_level(LED_Y2, OFF);
    gpio_set_level(LED_R2, OFF);

    if(light == GREEN)
    {
        gpio_set_level(LED_G1, ON);
        gpio_set_level(LED_R2, ON);
    }
    else if(light == YELLOW)
    {
        gpio_set_level(LED_Y1, ON);
        gpio_set_level(LED_Y2, ON);
    }
    else if(light == RED)
    {
        gpio_set_level(LED_R1, ON);
        gpio_set_level(LED_G2, ON);
    }
}

/// @brief fonction d'un evenement à ajuster avec le nrf présentement, uniquement des boutons
/// @param  void
/// @return quel ligne demande un event
uint8_t event(void)
{
    uint8_t value = NO_EVENT;
    if(!gpio_get_level(PB_1))
        value = EVENT_1;
    else if(!gpio_get_level(PB_2))
        value = EVENT_2;
    return value;
}

void algo_light(uint8_t* timer, uint8_t event)
{
    if(event)
    {
        add_event(event,tab_event,&tab_size);
        if(event == EVENT_1)
            sit_lum = event_1_n;
        else
            sit_lum = event_2_n;
    }
    switch(sit_lum)
    {
        case green_n:
            if(!flag_reset)
            {
                flag_reset = true;
                *timer = 0;
            }
            last_light = GREEN;
            set_light(GREEN);
            if(*timer >= TIME_GREEN)
            {
                flag_reset = 0;
                sit_lum = yellow_n;
            }
        break;
        case yellow_n:
            if(!flag_reset)
            {
                flag_reset = true;
                *timer = 0;
            }
            set_light(YELLOW);
            if(*timer >= TIME_YELLOW)
            {
                flag_reset = 0;
                if(last_light == GREEN)
                {
                    last_light = YELLOW;
                    sit_lum = red_n;
                }
                else if(last_light == RED)
                {
                    last_light = YELLOW;
                    sit_lum = green_n;
                }
            }
        break;
        case red_n:
            if(!flag_reset)
            {
                flag_reset = true;
                *timer = 0;
            }
            set_light(RED);
            last_light = RED;
            if(*timer >= TIME_RED)
            {
                flag_reset = 0;
                sit_lum = yellow_n;
            }
        break;
        case event_1_n:
            last_light = RED;
            erase_first(tab_event,&tab_size);
            sit_lum = yellow_n;
        break;
        case event_2_n:
            last_light = GREEN;
            erase_first(tab_event,&tab_size);
            sit_lum = yellow_n;
        break;
    }
}