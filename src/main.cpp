#include "scheduler.h"
#include "motorDriver.h"

int i;
bool down;

void speedTest();

Task testSpeed(2, TASK_FOREVER, &speedTest, &ts, true);

void setup() {
#if defined(_DEBUG_) || defined(_TEST_)
  Serial.begin(115200);
  // delay(TASK_SECOND);
  _PL("Trial Speed Control using TaskScheduler\n");
  // _PL("Blinking for 10 seconds using various techniques\n");
  // delay(2 * TASK_SECOND);
#endif /*_DEBUG_ || _TEST_*/
  md.init();
  md.calibrateCurrentOffsets(); 
}

void loop() {
  ts.execute();
}

void speedTest(){
  if (down) i--;
  else i++;
  if (i == 400)
    down = true;
  else if (i == -400)
    down = false;
  Speeds.set(i, i);
  lsEnable.restart();
  rsEnable.restart();
}
