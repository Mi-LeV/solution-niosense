#include "comm_nrf24.h"

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);
//VSPI is default, SPI1 is HSPI

// choose the right node name corresponding to the slave id
// Camion1 <-> Feu on node named Camion1
// Camion2 <-> Feu on node named Camion2
const uint8_t nodeAddress[8] = "Camion1" ;

SlavePayloadStruct slave_payload; // slave payload
MasterPayloadStruct master_payload;

// Init comm NRF24
void init_comm_nrf24() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding");
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
          //long start = millis();
          radio.read( &master_payload, sizeof(master_payload) );

          slave_payload.connection_status = !slave_payload.connection_status;
          slave_payload.position ++;
          slave_payload.command_response = ( slave_payload.command_response + 1) % 3;

          /*
          Serial.println( "RECEIVED MASTER PAYLOAD : ");
          Serial.print("\tConn status : ");
          Serial.println( master_payload.connection_status);
          Serial.print("\tLight state : ");
          Serial.println( master_payload.traffic_light_state);
          Serial.print("\tCommand : ");
          Serial.println( master_payload.command);


          Serial.println( "SENDING SLAVE PAYLOAD : ");
          Serial.print("\tConn status : ");
          Serial.println( slave_payload.connection_status);
          Serial.print("\tPosition : ");
          Serial.println( slave_payload.position);
          Serial.print("\tCommand response : ");
          Serial.println( slave_payload.command_response);
          */
          

          // update the node count after sending ack payload - provides continually changing data
          radio.writeAckPayload(1, &slave_payload, sizeof(slave_payload));
          //long stop = millis();
          //Serial.print("\tResponse time : ");
          //Serial.println(stop-start, 0);
    }
}