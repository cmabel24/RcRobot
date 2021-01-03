#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <DualG2HighPowerMotorShield.h>
#include "scheduler.h"

#ifdef _TEST_
#include <unity.h>
#endif // _TEST_ 

#define IDLE_CONTROL 1000
#define IDLE_SHUTDOWN 1000
#define FAULT_PERIOD 100

// Classes
DualG2HighPowerMotorShield18v18 md;

class SpeedSetting {
  public:
    int8_t _M1nSLEEP = 2;
    int8_t _M1nFAULT = 6;
    int8_t _M1DIR = 7;
    int8_t _M1PWM = 9;
    int8_t _M1CS = A0;

    int8_t _M2nSLEEP = 4;
    int8_t _M2nFAULT = 12;
    int8_t _M2DIR = 8;
    int8_t _M2PWM = 10;
    int8_t _M2CS = A1;

    int ls;
    int rs;
    void set(int m1, int m2) {
      ls = m1; 
      rs = m2;
    }
} Speeds;

// Variables
bool lsEnabled = false;
bool rsEnabled = false;

// Prototypes
void lsOnEnable();
void rsOnEnable();
void lsRun();
void rsRun();
void lsOnDisable();
void rsOnDisable();
bool stopIfFault();
void printFault();

#ifdef _TEST_
DualG2HighPowerMotorShield mdpins;
void lsTest();
void rsTest();
#endif // _TEST_

// Tasks
Task lsEnable(TASK_IMMEDIATE, TASK_ONCE, &lsOnEnable, &ts);
Task lsSpeed(TASK_IMMEDIATE, TASK_ONCE, &lsRun, &ts);
Task lsDisable(TASK_IMMEDIATE, TASK_ONCE, &lsOnDisable, &ts);
Task rsEnable(TASK_IMMEDIATE, TASK_ONCE, &rsOnEnable, &ts);
Task rsSpeed(TASK_IMMEDIATE, TASK_ONCE, &rsRun, &ts);
Task rsDisable(TASK_IMMEDIATE, TASK_ONCE, &rsOnDisable, &ts);

// Methods
// Left Side
void lsOnEnable() {
  if (lsDisable.isEnabled())
    lsDisable.disable();
  if(lsEnabled)
    lsSpeed.restart();
  else {
    md.enableM1Driver();
    lsSpeed.restartDelayed(1);
    lsEnabled = true;
  }
}

void lsRun() {
  md.setM1Speed(Speeds.ls);
  lsDisable.enableDelayed(IDLE_CONTROL);
  if (stopIfFault())
    printFault();
#ifdef _TEST_
  lsTest();
#endif // _TEST_

#ifdef _DEBUG_
  if (Speeds.ls%200 == 100) {
    _PP("Left side current: ");
    _PL(md.getM1CurrentMilliamps());
  }
#endif // _DEBUG_
}

void lsOnDisable() {
  md.setM1Speed(0);
  md.disableM1Driver();
  lsEnabled = false;
}

// Right Side
void rsOnEnable() {
  if (rsDisable.isEnabled())
    rsDisable.disable();
  if(rsEnabled)
    rsSpeed.restart();
  else {
    md.enableM2Driver();
    rsSpeed.restartDelayed(1);
    rsEnabled = true;
  }
}

void rsRun() {
  md.setM2Speed(Speeds.rs);
  rsDisable.enableDelayed(IDLE_CONTROL);
  if (stopIfFault())
    printFault();
#ifdef _TEST_
  rsTest();
#endif // _TEST_

#ifdef _DEBUG_
  if (Speeds.rs%200 == 100) {
    _PP("Right side current: ");
    _PL(md.getM2CurrentMilliamps());
  }
#endif // _DEBUG_ 
}

void rsOnDisable() {
  md.setM2Speed(0);
  md.disableM2Driver();
  rsEnabled = false;
}

// Faults
bool stopIfFault()
{
  if (md.getM1Fault() && lsEnabled) {
    lsOnDisable();
    rsOnDisable();
    return true;
  }
  else if (md.getM2Fault() && rsEnabled) {
    lsOnDisable();
    rsOnDisable();
    return true;
  }
  return false;
}

void printFault() {
#ifdef _DEBUG_
  if (md.getM1Fault())
    _PL("Left side fault");
  if (md.getM2Fault())
    _PL("Right side fault");
  while (1);
#endif // _DEBUG_
}

#ifdef _TEST_
void lsTest() {
  // TEST_ASSERT_EQUAL(false, md.getM1Fault());
  // TEST_ASSERT_EQUAL(HIGH, digitalRead(Speeds._M1nSLEEP));
  // if (Speeds.ls == 0)
  //   TEST_ASSERT_EQUAL(LOW, digitalRead(Speeds._M1PWM));
  // if (Speeds.ls > 0)
  //   TEST_ASSERT_EQUAL(HIGH, digitalRead(Speeds._M1DIR));
  // if (Speeds.ls < 0)
  //   TEST_ASSERT_EQUAL(LOW, digitalRead(Speeds._M1DIR));
  // if (Speeds.ls == 400 || Speeds.ls == -400)
  //   TEST_ASSERT_EQUAL(HIGH, digitalRead(Speeds._M1PWM));
}

void rsTest() {
  // TEST_ASSERT_EQUAL(false, md.getM2Fault());
  // TEST_ASSERT_EQUAL(HIGH, digitalRead(Speeds._M2nSLEEP));
  // if (Speeds.rs == 0)
  //   TEST_ASSERT_EQUAL(LOW, digitalRead(Speeds._M2PWM));
  // if (Speeds.rs > 0)
  //   TEST_ASSERT_EQUAL(HIGH, digitalRead(Speeds._M2DIR));
  // if (Speeds.rs < 0)
  //   TEST_ASSERT_EQUAL(LOW, digitalRead(Speeds._M2DIR));
  // if (Speeds.rs == 400 || Speeds.rs == -400)
  //   TEST_ASSERT_EQUAL(HIGH, digitalRead(Speeds._M2PWM));
}
#endif // _TEST_

#endif /* MOTORDRIVER_H */
