#include <Arduino.h>
#include "server.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  init_server();
}

void loop() {
  // put your main code here, to run repeatedly:
  handle_client();
}

