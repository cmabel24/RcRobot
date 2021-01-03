#ifndef ROVER_H
#define ROVER_H

#include <pb_encode.h>
#include <pb_decode.h>
#include <pb.h>
#ifdef _TEST_
#include <unity.h>
#endif // _TEST_ 

#include "serialCommunications.pb.h"
#include "scheduler.h"
#include "motorDriver.h"

void readByte();
void decodeCommand();
void executeCommand();
void sendTellemetry();

// Tasks
Task tRead(TASK_IMMEDIATE, TASK_ONCE, &readByte, &ts);
Task tDecode(TASK_IMMEDIATE, TASK_ONCE, &decodeCommand, &ts);
Task tExecute(TASK_IMMEDIATE, TASK_ONCE, &executeCommand, &ts);
Task tTellemetry(1000, TASK_FOREVER, &sendTellemetry, &ts, true);



class Controller {
  private:
    ControllerCommands message = ControllerCommands_init_default;
    uint8_t buffer[ControllerCommands_size];
    int idx;
    size_t rlen;
  public:
    void read();
    bool decode();
    void execute();
} Controller;



class Rover {
  private:
    RoverTellemetry message = RoverTellemetry_init_default;
    void read();
  public:
    void send();
} Rover;



void readByte() {Controller.read();}
void decodeCommand() {
  if (!Controller.decode())
    tExecute.restart();
}
void executeCommand() {Controller.execute();}
void sendTellemetry() {Rover.send();}



void Controller::read() {
  uint8_t recievedByte = Serial.read();
  if (recievedByte == '\r') {
    idx = 0;
    tDecode.restart();
  } else if (idx < ControllerCommands_size) {
    buffer[idx] = recievedByte;
    idx++;
  }
}

bool Controller::decode() {
  pb_istream_t stream = pb_istream_from_buffer(buffer, sizeof(buffer));
  bool status = pb_decode(&stream, ControllerCommands_fields, &message);
#ifdef _DEBUG_ 
  if (status){
    char buff[128];
    sprintf (buff, "Decoding failed: %s\n", PB_GET_ERROR(&stream));
    _PP(buff);
  }
#endif /* _DEBUG_ */

  return status;
}

void Controller::execute() {
  Speeds.ls = message.a3;
  Speeds.rs = message.a0;
  lsEnable.restart();
  rsEnable.restart();
}



void Rover::read() {
  message.faultLS = md.getM1Fault();
  message.faultRS = md.getM2Fault();
  message.motorAmpsLS = md.getM1CurrentMilliamps();
  message.motorAmpsRS = md.getM2CurrentMilliamps();
  // message.vBatt = ;
}

void Rover::send() {
  uint8_t buffer[RoverTellemetry_size];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  read();
  bool status = pb_encode(&stream, RoverTellemetry_fields, &message);

  if (!status) {
#ifdef _DEBUG_
    char buff[128];
    sprintf (buff, "Encoding failed: %s\n", PB_GET_ERROR(&stream));
    _PP(buff);
#endif /* _DEBUG_ */
  }
  else
    Serial.write(buffer, sizeof(buffer));
    Serial.write('\r');
}

#endif /* ROVER_H */ 
