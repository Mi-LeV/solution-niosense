#include "comm_nrf24.h"

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);
//VSPI is default, SPI1 is HSPI

//defining the two nodes names
// each half-duplex comm will be happening on their respective node
// Camion1 <-> Feu on node named 1Camion
// Camion2 <-> Feu on node named 2Camion
const uint8_t addresses[][8] = { "1Camion" , "2Camion"};

SlavePayloadStruct slave_payload; // slave payload
MasterPayloadStruct master_payload;

long start,stop;

// Init comm NRF24
void init_comm_nrf24() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    //Serial.println("Radio hardware is not responding");
    while (1); // wait indefinitely
  }

  radio.setPALevel(RF24_PA_LOW);  // power level of the radio

  // to use ACK payloads, we need to enable dynamic payload lengths (for all nodes)
  radio.enableDynamicPayloads();  // ACK payloads are dynamically sized

  // set RF datarate - lowest rate for longest range capability
  radio.setDataRate(RF24_250KBPS);
    
  // set radio channel to use - ensure all slaves match this
  radio.setChannel(RF_CHANNEL);

  radio.openReadingPipe(SLAVE_ID, addresses[SLAVE_ID]);   

  // Acknowledgement packets have no payloads by default. We need to enable
  // this feature for all nodes (TX & RX) to use ACK payloads.
  radio.enableAckPayload();

  if(DEBUG_MODE){
    slave_payload.connection_status = false;
    if (SLAVE_ID){
      slave_payload.position = 2 ;
      slave_payload.command_response = 2 ;
    }
    else{
      slave_payload.position = 0 ;
      slave_payload.command_response = 0 ;
    }
  }
  else{
    slave_payload.connection_status = false;
    slave_payload.position = 0 ;
    slave_payload.command_response = 0 ;
  }
  
  

  // start listening on radio
  radio.startListening();
  // initialize the master_payload to inoffensive values
  radio.writeAckPayload(SLAVE_ID, &slave_payload, sizeof(slave_payload));
  if (DEBUG_MODE){
    //radio.printPrettyDetails(); // (larger) function that prints human readable data
  }
   
}


// When the master payload is received, store it in master_payload global struct,
// and immediately send the slave_payload global struct to the master
void radioCheckAndReply(void)
{
    // check for radio message and send sensor data using auto-ack
    if ( radio.available() ) {
          if (DEBUG_MODE){
            long start = millis();
          }
          radio.read( &master_payload, sizeof(master_payload) );

          
          if (DEBUG_MODE){

            //Serial.println( "RECEIVED MASTER PAYLOAD : ");
            //Serial.print("\tConn status : ");
            for (int i = 0 ; i < NB_SLAVES ; i++){
              //Serial.print( master_payload.connection_status[0]);
              //Serial.print(" , ");
            }
            //Serial.print("\n");
            //Serial.print("\tLight state : ");
            //Serial.println( master_payload.traffic_light_state);
            //Serial.print("\tCommand : ");
            //Serial.println( master_payload.command);


            //Serial.println( "SENDING SLAVE PAYLOAD : ");
            //Serial.print("\tConn status : ");
            //Serial.println( slave_payload.connection_status);
            //Serial.print("\tPosition : ");
            //Serial.println( slave_payload.position);
            //Serial.print("\tCommand response : ");
           // Serial.println( slave_payload.command_response);

            if (slave_payload.position % 2 == 0){
            slave_payload.position ++ ;
            slave_payload.command_response ++ ;
          }
          else{
            slave_payload.position -- ;
            slave_payload.command_response -- ;
          }
          }

          // update the node count after sending ack payload - provides continually changing data
          radio.writeAckPayload(SLAVE_ID, &slave_payload, sizeof(slave_payload));

          if(DEBUG_MODE){
            long stop = millis();
            //Serial.print("\tResponse time : ");
           // Serial.println(stop-start, 0);
          }
          
    }
}