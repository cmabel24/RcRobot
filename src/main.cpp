/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include <Arduino.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif
#define LED 11
int i = 0;
int pwm = 255;
int rate = 25;

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED, OUTPUT); 
}

void loop()
{
  for(i = 0;i<pwm;i++){
    analogWrite(LED,i);
    delay(((60000/rate)*.1)/pwm);
  }
  for(i = pwm;i>0;i--){
    analogWrite(LED,i);
    delay(((60000/rate)*.2)/pwm);
  }
  for(i=0;i<pwm;i++){
    analogWrite(LED,i);
    delay(((60000/rate)*.1)/pwm);
  }
  for(i=pwm;i>0;i--){
    analogWrite(LED,i);
    delay(((60000/rate)*.6)/pwm);
  }
  // // turn the LED on (HIGH is the voltage level)
  // digitalWrite(LED_BUILTIN, HIGH);

  // // wait for a second
  // delay(1000);

  // // turn the LED off by making the voltage LOW
  // digitalWrite(LED_BUILTIN, LOW);

  //  // wait for a second
  // delay(1000);
}