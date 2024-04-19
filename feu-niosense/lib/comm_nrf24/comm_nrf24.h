#ifndef COMM_NRF24
#define COMM_NRF24

#include <SPI.h>
#include "RF24.h"
#include <nRF24l01.h>
#include "includes.h"
 #include "wifi_server.h"


#define CE_PIN 15
#define CSN_PIN 5

#define DEBUG_MODE false
#define RF_CHANNEL 0x76

// ATTENTION : À CHANGER DANS LIGHT.H
#define DISCONNECT_HIGH 26000
#define DISCONNECT_LOW 4000

typedef enum 
{
    master_idle,
    master_init,
    master_go,
    master_stop
    
}master_command;



extern MasterPayloadStruct master_payload;
extern SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads
extern bool status1;
extern bool status2;


extern String new_line(event_t event);

bool init_comm_nrf24();

bool send_and_receive_comm_nrf();

#endif