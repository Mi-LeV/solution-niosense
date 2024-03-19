#include "comm_nrf24.h"

// Init comm NRF24
void init_comm_nrf24() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    ESP_LOGE(TAG_NRF,"Radio hardware is not responding");
    while (1); // wait indefinitely
  }

  radio.setPALevel(RF24_PA_LOW);  // power level of the radio

  // to use ACK payloads, we need to enable dynamic payload lengths (for all nodes)
  radio.enableDynamicPayloads();  // ACK payloads are dynamically sized

  // set RF datarate - lowest rate for longest range capability
  radio.setDataRate(RF24_250KBPS);
    
  // set radio channel to use - ensure all slaves match this
  radio.setChannel(RF_CHANNEL);
    
  // set time between retries and max no. of retries
  radio.setRetries(4, 10);

  // Acknowledgement packets have no payloads by default. We need to enable
  // this feature for all nodes (TX & RX) to use ACK payloads.
  radio.enableAckPayload();

  // initialize the master_payload to inoffensive values
  master_payload.connection_status = false;
  master_payload.traffic_light_state = 0;
  master_payload.command = 0;

  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data
}

/*
Sends the current master_payload to each slave,
and waits for each to return a slave_payload 
which will the corresponding struct in the slave_payload[] list
*/
void send_and_receive_comm_nrf(){



  for (int node = 0 ; node < NB_SLAVES ; node++){
    // setup a write pipe to current sensor node - must match the remote node listening pipe
        radio.openWritingPipe(nodeAddresses[node]);
        bool response = radio.write(&master_payload, sizeof(master_payload));  // transmit & save the report

        if (response) {
        uint8_t pipe;
        if (radio.available(&pipe)) {  // is there an ACK payload? grab the pipe number that received it
            radio.read(&slave_payload[node], sizeof(slave_payload));  // get incoming ACK payload
            ESP_LOGI(TAG_NRF," Recieved ");
            ESP_LOGI(TAG_NRF, radio.getDynamicPayloadSize());  // print incoming payload size
            ESP_LOGI(TAG_NRF, " bytes on pipe ");
            ESP_LOGI(TAG_NRF, pipe);  // print pipe number that received the ACK
            ESP_LOGI(TAG_NRF, " : ");

            ESP_LOGI(TAG_NRF, slave_payload[node].connection_status);
            ESP_LOGI(TAG_NRF, slave_payload[node].command_response);
            ESP_LOGI(TAG_NRF, slave_payload[node].position);

        } else {
            ESP_LOGW(TAG_NRF, " Recieved: an empty ACK packet");  // empty ACK packet received
        }


        } else {
        ESP_LOGW(TAG_NRF, "Transmission failed or timed out");  // payload was not delivered
        }
    }
}