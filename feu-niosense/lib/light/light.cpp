#include "light.h"

uint8_t tab_event[MAX_SIZE_STACK] = {0};
uint8_t tab_size = 0;
situation sit_lum = green_n;
uint8_t flag_reset = false;
uint8_t last_light = GREEN;

extern uint8_t algo;

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
    pinMode(LED_G1, OUTPUT);
    pinMode(LED_Y1, OUTPUT);
    pinMode(LED_R1, OUTPUT);
    pinMode(LED_G2, OUTPUT);
    pinMode(LED_Y2, OUTPUT);
    pinMode(LED_R2, OUTPUT);

}

void set_light(uint8_t light)
{   
    digitalWrite(LED_G1, OFF);
    digitalWrite(LED_Y1, OFF);
    digitalWrite(LED_R1, OFF);
    digitalWrite(LED_G2, OFF);
    digitalWrite(LED_Y2, OFF);
    digitalWrite(LED_R2, OFF);

    if(light == GREEN)
    {
        digitalWrite(LED_G1, HIGH);
        digitalWrite(LED_R2, HIGH);
    }
    else if(light == YELLOW)
    {
        digitalWrite(LED_Y1, HIGH);
        digitalWrite(LED_Y2, HIGH);
    }
    else if(light == RED)
    {
        digitalWrite(LED_R1, HIGH);
        digitalWrite(LED_G2, HIGH);
    }
}

uint8_t event(void)
{
    uint8_t event_return = 0;
    if((algo == ALGO_DEMANDE) && (((slave_payload[0].position < DISCONNECT_HIGH) && (slave_payload[0].position > DISCONNECT_LOW)) ||
        ((slave_payload[1].position < DISCONNECT_HIGH) && (slave_payload[1].position > DISCONNECT_LOW))))
    {
        event_return = 1;
    }
    return event_return;
}

void algo_light(volatile uint8_t* timer, uint8_t event)
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
            master_payload.traffic_light_state = GREEN;
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
            master_payload.traffic_light_state = YELLOW;
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
            master_payload.traffic_light_state = RED;
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
            flag_reset = false;
            erase_first(tab_event,&tab_size);
            sit_lum = yellow_n;
        break;
        case event_2_n:
            last_light = GREEN;
            flag_reset = false;
            erase_first(tab_event,&tab_size);
            sit_lum = yellow_n;
        break;
    }
}