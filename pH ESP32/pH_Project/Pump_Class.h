#include "esp32-hal-gpio.h"
#include "Arduino.h"


class Pump{
private:
  byte pumpPin;
  boolean status = false;


public:
  Pump(byte pin){
    pumpPin = pin;

    pinMode(pumpPin, OUTPUT);
  }

  void on(){
    digitalWrite(pumpPin, HIGH);
    status = true;
  }

  void off(){
    digitalWrite(pumpPin, LOW);
    status = false;
  }

  boolean getStatus(){
    return status;
  }


};