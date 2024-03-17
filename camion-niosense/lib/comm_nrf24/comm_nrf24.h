#include <SPI.h>
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8

#define RF_CHANNEL 0x76

static const char *TAG = "comm_nrf24";

// choose the right node name corresponding to the slave id
// Camion1 <-> Feu on node named Camion1
// Camion2 <-> Feu on node named Camion2
const uint8_t nodeAddress[8] = "Camion1" ;

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

// payloads to be sent and received by the nrf
struct MasterPayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint8_t traffic_light_state; // 0 : red, 1 : yellow, 2 : green
  uint8_t command; // 0 : idle, 1 : initialize, 2 : go, 3 : stop
};
MasterPayloadStruct master_payload;

struct SlavePayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint16_t position; // position on the test bed, ranging from 0 to 65536
  uint8_t command_response; // 0 : NA, 1 : reset finished, 2 : stopped
};
SlavePayloadStruct slave_payload; // slave payload

void init_comm_nrf24();

void radioCheckAndReply();