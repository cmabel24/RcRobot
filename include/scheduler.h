#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <TaskScheduler.h>

// #define _TASK_TIMECRITICAL      // Enable monitoring scheduling overruns
#define _TASK_SLEEP_ON_IDLE_RUN // Enable 1 ms SLEEP_IDLE powerdowns between tasks if no callback methods were invoked during the pass
#define _TASK_STATUS_REQUEST    // Compile with support for StatusRequest functionality - triggering tasks on status change events in addition to time only
// #define _TASK_WDT_IDS           // Compile with support for wdt control points and task ids
// #define _TASK_LTS_POINTER       // Compile with support for local task storage pointer
// #define _TASK_PRIORITY          // Support for layered scheduling priority
// #define _TASK_MICRO_RES         // Support for microsecond resolution
// #define _TASK_STD_FUNCTION      // Support for std::function (ESP8266 and ESP32 ONLY)
// #define _TASK_DEBUG             // Make all methods and variables public for debug purposes
// #define _TASK_INLINE            // Make all methods "inline" - needed to support some multi-tab, multi-file implementations
// #define _TASK_TIMEOUT           // Support for overall task timeout
// #define _TASK_OO_CALLBACKS      // Support for dynamic callback method binding

// Debug and Test options
#define _DEBUG_
// #define _TEST_

#ifdef _DEBUG_
#define _PP(a) Serial.print(a);
#define _PL(a) Serial.println(a);
#else
#define _PP(a)
#define _PL(a)
#endif /* _DEBUG_ */

// Method definitions
#define DURATION 10000
#define PERIOD 600
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define LED 11

// Scheduler
Scheduler ts;

int i = 0;
int pwm = 255;
int rate = 25;
bool LED_state = false;

// Prototypes
void blink1CB();
inline void LEDOn() {
  digitalWrite( LED_BUILTIN, HIGH );
}

inline void LEDOff() {
  digitalWrite( LED_BUILTIN, LOW );
}

// Tasks
Task tBlink1 ( PERIOD * TASK_MILLISECOND, TASK_FOREVER, &blink1CB, &ts, true );

void blink1CB() {
  // if ( tBlink1.isFirstIteration() ) {
  //   _PP(millis());
  //   _PL(": Blink1 - simple flag driven");
  //   LED_state = false;
  // }

  if ( LED_state ) {
    LEDOff();
    LED_state = false;
  }
  else {
    LEDOn();
    LED_state = true;
  }

}


//   // // turn the LED on (HIGH is the voltage level)
//   // digitalWrite(LED_BUILTIN, HIGH);

//   // // wait for a second
//   // delay(1000);

//   // // turn the LED off by making the voltage LOW
//   // digitalWrite(LED_BUILTIN, LOW);

//   //  // wait for a second
//   // delay(1000);

// void loop()
// {
//   for(i = 0;i<pwm;i++){
//     analogWrite(LED,i);
//     delay(((60000/rate)*.1)/pwm);
//   }
//   for(i = pwm;i>0;i--){
//     analogWrite(LED,i);
//     delay(((60000/rate)*.2)/pwm);
//   }
//   for(i=0;i<pwm;i++){
//     analogWrite(LED,i);
//     delay(((60000/rate)*.1)/pwm);
//   }
//   for(i=pwm;i>0;i--){
//     analogWrite(LED,i);
//     delay(((60000/rate)*.6)/pwm);
//   }
// }


#endif /* SCHEDULER_H */
