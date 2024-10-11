#include "../inc/definitions.h"

void test_commPortLockig()
{
  uint32_t ts_toggle = 0;
  bool pinState = LOW;
  while(1)
  {
    if(millis() - ts_toggle > 1000)
    {
      ts_toggle = millis();
      pinState = !pinState;
      digitalWrite(LEFT_OUT, pinState);
      digitalWrite(RIGHT_OUT, pinState);
    }

    Serial.print("L_IN=");
    Serial.print(digitalRead(LEFT_IN));
    Serial.print("\tL_SEN=");
    Serial.print(digitalRead(LEFT_SENSE));
    Serial.print("\tR_IN=");
    Serial.print(digitalRead(RIGHT_IN));
    Serial.print("\tR_SEN=");
    Serial.print(digitalRead(RIGHT_SENSE));
    Serial.print("\n");
    delay(50);
  }
}