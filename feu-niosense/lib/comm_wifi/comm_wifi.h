#ifndef COMM_WIFI_H
#define COMM_WIFI_H

#include <esp_now.h>
#include <WiFi.h>

#define NB_SLAVES 1

const char* ssid = "ETS-Invites";

const char* password = "xxx";
// REPLACE WITH YOUR RECEIVER MAC Address
// master mac adress 0xFC,0xB4,0x67,0xF0,0xBB,0xEC
const uint8_t mac_adresses[NB_SLAVES][6] = {{0xFC,0xB4,0x67,0xF0,0xC8,0x08}}; // slaves MAC adresses

// payloads to be sent and received by the nrf
struct MasterPayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint8_t traffic_light_state; // 0 : red, 1 : yellow, 2 : green
  uint8_t command; // 0 : idle, 1 : initialize, 2 : go, 3 : stop
};

typedef struct SlavePayloadStruct {
  bool connection_status; // 0 : No conn, 1 : connection
  uint16_t position; // position on the test bed, ranging from 0 to 65536
  uint8_t command_response; // 0 : NA, 1 : reset finished, 2 : stopped
} SlavePayloadStruct;

MasterPayloadStruct master_payload;
SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads
esp_now_peer_info_t peerInfo;

void init_comm_wifi();

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void OnDataReceived(const uint8_t * mac, const uint8_t *incomingData, int len);

void send_comm_wifi();

#endif