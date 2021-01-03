// Debug and Test options
// #define _DEBUG_
// #define _TEST_

#include "rover.h"

void setup() {
  Serial.begin(115200);
  md.init();
  md.calibrateCurrentOffsets();
}

void loop() {
  if (Serial.available())
    tRead.restart();
  ts.execute();
}
