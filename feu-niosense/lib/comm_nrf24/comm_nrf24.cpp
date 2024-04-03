#include "comm_nrf24.h"

//defining the two nodes names
// each half-duplex comm will be happening on their respective node
// Camion1 <-> Feu on node named 1Camion
// Camion2 <-> Feu on node named 2Camion
const uint8_t addresses[][8] = { "1Camion" , "2Camion"};

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

MasterPayloadStruct master_payload;
SlavePayloadStruct slave_payload[NB_SLAVES]; // 2 slave payloads

long start,stop;

// Init comm NRF24
bool init_comm_nrf24() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding");
    return false;
  }

  radio.setPALevel(RF24_PA_LOW);  // power level of the radio

  // set RF datarate - lowest rate for longest range capability
  radio.setDataRate(RF24_250KBPS);
    
  // set radio channel to use - ensure all slaves match this
  radio.setChannel(RF_CHANNEL);
    
  // set time between retries and max no. of retries
  radio.setRetries(4, 10);

  // to use ACK payloads, we need to enable dynamic payload lengths (for all nodes)
  radio.enableDynamicPayloads();  // ACK payloads are dynamically sized

  // Acknowledgement packets have no payloads by default. We need to enable
  // this feature for all nodes (TX & RX) to use ACK payloads.
  radio.enableAckPayload();

  // initialize the master_payload to inoffensive values
  for (int n = 0 ; n < NB_SLAVES ; n++){
    master_payload.connection_status[n] = false;
  }
  master_payload.traffic_light_state = 0;
  master_payload.command = 0;

  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data

  return true;
}

/*
Sends the current master_payload to each slave,
and waits for each to return a slave_payload 
which will the corresponding struct in the slave_payload[] list
*/
bool send_and_receive_comm_nrf(){
  bool send_state = true;
  if (DEBUG_MODE){
    long start = millis();
  }
  for (uint8_t node = 0 ; node < NB_SLAVES ; node++){
    // setup a write pipe to current sensor node - must match the remote node listening pipe

        if (DEBUG_MODE){

        Serial.println( "SENDING MASTER PAYLOAD : ");
        Serial.print("\tConn status : ");
        for (int i = 0 ; i < NB_SLAVES ; i++){
          Serial.print( master_payload.connection_status[0]);
          Serial.print(" , ");
        }
        Serial.print("\n");
        Serial.print("\tLight state : ");
        Serial.println( master_payload.traffic_light_state);
        Serial.print("\tCommand : ");
        Serial.println( master_payload.command);
        }
        
        radio.openWritingPipe(addresses[node]);
        bool response = radio.write(&master_payload, sizeof(master_payload));  // transmit & save the report
        if ((response)||(radio.isAckPayloadAvailable() )) {
          radio.read(&slave_payload[node], sizeof(slave_payload[node]));

          if (DEBUG_MODE){
            Serial.print( "RECEIVED SLAVE PAYLOAD FROM Camion");
            Serial.println(node+1);
            Serial.print("\tConn status : ");
            Serial.println( slave_payload[node].connection_status);
            Serial.print("\tPosition : ");
            Serial.println( slave_payload[node].position);
            Serial.print("\tCommand response : ");
            Serial.println( slave_payload[node].command_response);
          }

        } else {
          
          Serial.print("TRANSMISSION FAILED Camion");  // payload was not delivered
          Serial.println(node+1);
          //
          //
          send_state = false;
        }
        radio.flush_tx();
        radio.flush_rx();
    }
    if (DEBUG_MODE){
      long stop = millis();
      Serial.print("\tResponse time : ");
      Serial.println(stop-start, 0);
    }
    return send_state;
}
