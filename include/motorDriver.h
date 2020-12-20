#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <DualG2HighPowerMotorShield.h>
#include "scheduler.h"

#define IDLE_CONTROL 1000
#define IDLE_SHUTDOWN 1000
#define FAULT_PERIOD 100

// Classes
DualG2HighPowerMotorShield18v18 md;

class SpeedSetting {
  public:
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
#if defined(_DEBUG_) || defined(_TEST_)
  if (Speeds.ls%200 == 100) {
    _PP("Left side current: ");
    _PL(md.getM1CurrentMilliamps());
  }
#endif /*_DEBUG_ || _TEST_*/
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
#if defined(_DEBUG_) || defined(_TEST_)
  if (Speeds.rs%200 == 100) {
    _PP("Right side current: ");
    _PL(md.getM2CurrentMilliamps());
  }
#endif /*_DEBUG_ || _TEST_*/
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
  if (md.getM1Fault())
    _PL("Left side fault");
  if (md.getM2Fault())
    _PL("Right side fault");
  while (1);
}

#endif /* MOTORDRIVER_H */
