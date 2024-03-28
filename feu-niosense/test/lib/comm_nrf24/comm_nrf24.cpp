#include "comm_nrf24.h"

//defining the two nodes names
// each half-duplex comm will be happening on their respective node
// Camion1 <-> Feu on node named Camion1
// Camion2 <-> Feu on node named Camion2
const uint8_t nodeAddresses[][8] = { "Camion1"};

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

MasterPayloadStruct master_payload;
SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads

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

  //long start = millis();

  master_payload.connection_status = !master_payload.connection_status;
  master_payload.traffic_light_state  = ( master_payload.traffic_light_state + 1) % 3;
  master_payload.command  = ( master_payload.command + 1) % 4;

  for (uint8_t node = 0 ; node < NB_SLAVES ; node++){
    // setup a write pipe to current sensor node - must match the remote node listening pipe
        /*
        Serial.println( "SENDING MASTER PAYLOAD : ");
        Serial.print("\tConn status : ");
        Serial.println( master_payload.connection_status);
        Serial.print("\tLight state : ");
        Serial.println( master_payload.traffic_light_state);
        Serial.print("\tCommand : ");
        Serial.println( master_payload.command);
        */

        radio.openWritingPipe(nodeAddresses[node]);
        bool response = radio.write(&master_payload, sizeof(master_payload));  // transmit & save the report
        if (response) { 
          radio.read(&slave_payload[node], sizeof(slave_payload[node]));
          /*
          Serial.print( "RECEIVED SLAVE PAYLOAD FROM Camion");
          Serial.println(node);
          Serial.print("\tConn status : ");
          Serial.println( slave_payload[node].connection_status);
          Serial.print("\tPosition : ");
          Serial.println( slave_payload[node].position);
          Serial.print("\tCommand response : ");
          Serial.println( slave_payload[node].command_response);
          */

        } else {
          
          Serial.println("TRANSMISSION FAILED");  // payload was not delivered
          radio.flush_rx();
          radio.flush_tx();
        }
    }

    //long stop = millis();
    //Serial.print("\tResponse time : ");
    //Serial.println(stop-start, 0);
}
