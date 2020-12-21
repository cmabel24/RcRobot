// Debug and Test options
// #define _DEBUG_
// #define _TEST_

#include "scheduler.h"
#include "motorDriver.h"

void setup() {
  Serial.begin(115200);
  md.init();
  md.calibrateCurrentOffsets(); 
}

void loop() {
  ts.execute();
}
