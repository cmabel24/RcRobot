// Debug and Test options
// #define _DEBUG_
// #define _TEST_

#include "scheduler.h"
#include "motorDriver.h"

uint8_t i;

void setup() {
  Serial.begin(115200);
  md.init();
  md.calibrateCurrentOffsets(); 
}

void loop() {
  if (i == 255)
    Serial.print(i);
  i++;
  ts.execute();
}
