#include <esp_now.h>
#include <WiFi.h>

static const char *TAG = "comm_wifi";
#define NB_SLAVES 2

// REPLACE WITH YOUR RECEIVER MAC Address
const uint8_t mac_adresses[2][6] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
                    ,{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

// payloads to be sent and received by the nrf
struct MasterPayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint8_t traffic_light_state; // 0 : red, 1 : yellow, 2 : green
  uint8_t command; // 0 : idle, 1 : initialize, 2 : go, 3 : stop
};
MasterPayloadStruct master_payload;

struct SlavePayloadStruct {
  uint8_t id; // id of the slave
  bool connection_status; // 0 : No conn, 1 : connection
  uint16_t position; // position on the test bed, ranging from 0 to 65536
  uint8_t command_response; // 0 : NA, 1 : reset finished, 2 : stopped
};
SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads
SlavePayloadStruct slave_temp_payload; // temporary slave payload

esp_now_peer_info_t peerInfo;

void init_comm_wifi();

void OnDataSent();

void send_comm_wifi();