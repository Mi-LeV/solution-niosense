#ifndef INCLUDES_H
#define INCLUDES_H
#include <Arduino.h>
#define NB_SLAVES 2

// d'autres événements peuvent être ajoutés
 typedef enum { START = 0, PAUSE, STOP, VIT1_CHANGED, VIT2_CHANGED, DIST_CHANGED,
 CAMION_BY_FEU, CONN_STATUS_CHANGED_1,CONN_STATUS_CHANGED_2 } event_t;

// payloads to be sent and received by the nrf
struct MasterPayloadStruct {
  bool connection_status [NB_SLAVES]; // 0 : No conn, 1 : connection
  uint8_t desired_speed_1;
  uint8_t desired_speed_2;
  uint8_t traffic_light_state; // 0 : red, 1 : yellow, 2 : green
  uint8_t command; // 0 : idle, 1 : initialize, 2 : go, 3 : stop
};

struct SlavePayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint16_t position; // position on the test bed, ranging from 0 to 65536
  uint8_t command_response; // 0 : NA, 1 : reset finished, 2 : stopped
};

#endif