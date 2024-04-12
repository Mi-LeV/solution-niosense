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

// payloads to be sent and received by the nrf
struct MasterPayloadStruct {
  bool connection_status [NB_SLAVES]; // 0 : No conn, 1 : connection
  uint8_t traffic_light_state; // 0 : red, 1 : yellow, 2 : green
  uint8_t command; // 0 : idle, 1 : initialize, 2 : go, 3 : stop
};

struct SlavePayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint16_t position; // position on the test bed, ranging from 0 to 65536
  uint8_t command_response; // 0 : NA, 1 : reset finished, 2 : stopped
};

extern MasterPayloadStruct master_payload;
extern SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads

bool init_comm_nrf24();

bool send_and_receive_comm_nrf();

#endif