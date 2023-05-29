#include "Arduino.h"

#include <PCF8574.h>

// PCF8574 PCF(0x21);

class POUT {
private:
  PCF8574 PCF;

  int pinlist[8] = {
    P0,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7
  };


  int status[8] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false
  };

public:
  POUT() : PCF(0x21) {

    PCF.pinMode(pinlist[0], OUTPUT);
    PCF.pinMode(pinlist[1], OUTPUT);
    PCF.pinMode(pinlist[2], OUTPUT);
    PCF.pinMode(pinlist[3], OUTPUT);
    PCF.pinMode(pinlist[4], OUTPUT);
    PCF.pinMode(pinlist[5], OUTPUT);
    PCF.pinMode(pinlist[6], OUTPUT);
    PCF.pinMode(pinlist[7], OUTPUT);
    PCF.begin();
  }

  void setup();
  void loop();

  void on(byte pin);
  void off(byte pin);
  void toggle(byte pin);
};

void POUT::setup() {
  for (int i = 0;i<8;i++)on(pinlist[i]);
}

void POUT::loop() {
}


void POUT::on(byte pin) {
  PCF.digitalWrite(pinlist[pin], HIGH);
  status[pin] = true;
}


void POUT::off(byte pin) {
  PCF.digitalWrite(pinlist[pin], LOW);
  status[pin] = false;
}

void POUT::toggle(byte pin) {
  PCF.digitalWrite(pinlist[pin], (status) ? HIGH : LOW);
  status[pin] = !status[pin];
  Serial.println("pin " + String(pin) + "  status = " + String(status[pin]));
}
