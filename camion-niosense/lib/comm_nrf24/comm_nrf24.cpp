#include "comm_nrf24.h"

// Init comm NRF24
void init_comm_nrf24() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    ESP_LOGE(TAG,"Radio hardware is not responding");
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

  radio.openReadingPipe(1, nodeAddress);   

  // Acknowledgement packets have no payloads by default. We need to enable
  // this feature for all nodes (TX & RX) to use ACK payloads.
  radio.enableAckPayload();

  slave_payload.connection_status = false;
  slave_payload.position = 0;
  slave_payload.command_response = 0;
  radio.writeAckPayload(1, &slave_payload, sizeof(slave_payload));

  // start listening on radio
  radio.startListening();
  // initialize the master_payload to inoffensive values
  

  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data
}


// When the master payload is received, store it in master_payload global struct,
// and immediately send the slave_payload global struct to the master
void radioCheckAndReply(void)
{
    // check for radio message and send sensor data using auto-ack
    if ( radio.available() ) {
          radio.read( &master_payload, sizeof(master_payload) );
          ESP_LOGI(TAG, "Received master payload, sending slave payload");

          // update the node count after sending ack payload - provides continually changing data
          radio.writeAckPayload(1, &slave_payload, sizeof(slave_payload));
    }
}