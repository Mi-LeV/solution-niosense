
//#include "comm_wifi.h"
 #include "comm_nrf24.h"
 #include "unity.h"

int j;
void test_initilization(){
  Serial.begin(115200);
  delay(2000);
  TEST_ASSERT_TRUE(init_comm_nrf24());
}

void test_comm(){
  for(int i = 0 ; i < 100 ; i++){
        TEST_ASSERT_TRUE(send_and_receive_comm_nrf());
        delay(j);
  }
}
int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initilization);

  for(j = 50 ; j>0 ; j-=5){
    Serial.print("Testing for ");
    Serial.print(j);
    Serial.print(" delay\n");
    RUN_TEST(test_comm);
  }
  
  return UNITY_END();
}

void setup(){

  delay(2000);


  Serial.begin(115200);
  Serial.println("BOARD : MASTER");

  //init_comm_wifi();
  
  runUnityTests();
}

void loop(){
}
