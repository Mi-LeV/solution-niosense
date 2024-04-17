#ifndef COMM_NRF24
#define COMM_NRF24

#include <SPI.h>
#include "RF24.h"
#include <nRF24l01.h>

#define CE_PIN 15
#define CSN_PIN 5

#define NB_SLAVES 2
#define DEBUG_MODE false
#define RF_CHANNEL 0x76

typedef enum 
{
    green,
    yellow,
    red
    
}traffic_light_color;

typedef enum 
{
    master_idle,
    master_init,
    master_go,
    master_stop
    
}master_command;

typedef enum 
{
    slave_NA,
    slave_init_finished
    
}slave_command_response;

// payloads to be sent and received by the nrf
struct MasterPayloadStruct {
  bool connection_status [NB_SLAVES]; // 0 : No conn, 1 : connection
  uint8_t desired_speed[NB_SLAVES];
  traffic_light_color traffic_light_state;
  master_command command;
};

struct SlavePayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint16_t position; // position on the test bed, ranging from 0 to 65536
  slave_command_response command_response;
};

extern MasterPayloadStruct master_payload;
extern SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads

bool init_comm_nrf24();

bool send_and_receive_comm_nrf();

#endif