#define _TEST_

#ifdef _TEST_

#include <unity.h>
#include "motorDriver.h"

int i, j;
bool down;
void speedTest();
void endTest();
Task testSpeed(2, TASK_FOREVER, &speedTest, &ts, true);
Task testEnd(TASK_IMMEDIATE, TASK_ONCE, &endTest, &ts);

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  // delay(2000);
  UNITY_BEGIN();    // IMPORTANT LINE!
  RUN_TEST(speedTest);
#if defined(_DEBUG_) || defined(_TEST_)
  Serial.begin(115200);
  _PL("Trial Speed Control using TaskScheduler\n");

#endif /*_DEBUG_ || _TEST_*/
  md.init();
  md.calibrateCurrentOffsets(); 
}

void loop() {
  ts.execute();
}

void speedTest() {
  if (down) i--;
  else i++;
  if (i == 400)
    down = true;
  else if (i == -400)
    down = false;
  if (down == 0 && i == 0) {
    j++;
    Speeds.set(0, 0);
    rsEnable.restart();
    lsEnable.restart();
  } else {
    if (j == 0) {
      Speeds.set(i, 0);
      lsEnable.restart();
    } else if (j == 1) {
      Speeds.set(0, i);
      rsEnable.restart();
    } else
      testEnd.restart();  
  }
}

void endTest() {
  UNITY_END(); // stop unit testing
  ts.disableAll();
}
#endif // _TEST_
