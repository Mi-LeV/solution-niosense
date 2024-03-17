#include <esp_now.h>
#include <WiFi.h>

static const char *TAG = "comm_wifi";
// REPLACE WITH YOUR RECEIVER MAC Address
const uint8_t mac_adress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // master MAC adress

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
esp_now_peer_info_t peerInfo;

void init_comm_wifi();

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len);