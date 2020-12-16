/*
*
* 
*/

#include "scheduler.h"

void setup() {
#if defined(_DEBUG_) || defined(_TEST_)
  Serial.begin(115200);
  delay(TASK_SECOND);
  _PL("TaskScheduler Blink example");
  _PL("Blinking for 10 seconds using various techniques\n");
  delay(2 * TASK_SECOND);
#endif

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED, OUTPUT); 
}

void loop() {
  ts.execute();
}
