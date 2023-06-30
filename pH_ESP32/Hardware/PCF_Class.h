#include "esp32-hal-gpio.h"

#include <PCF8574.h>

// PCF8574 PCF(0x21);

#define RELAY_ALL_PIN 25

class POUT {
private:
  PCF8574 PCF;
  bool status_setup = true;
  Timer t_active;

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


public:

  int status[8] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false
  };

  POUT() : PCF(0x21) , t_active(3000) {

    PCF.pinMode(pinlist[0], OUTPUT);
    PCF.pinMode(pinlist[1], OUTPUT);
    PCF.pinMode(pinlist[2], OUTPUT);
    PCF.pinMode(pinlist[3], OUTPUT);
    PCF.pinMode(pinlist[4], OUTPUT);
    PCF.pinMode(pinlist[5], OUTPUT);
    PCF.pinMode(pinlist[6], OUTPUT);
    PCF.pinMode(pinlist[7], OUTPUT);
    PCF.begin();

    pinMode(RELAY_ALL_PIN,OUTPUT);
  }

  void setup();
  void loop();

  void active();
  void deactive();

  void on(int pin);
  void off(int pin);
  void toggle(int pin);
  void reset();
};

void POUT::setup() {
  for (int i = 0;i<8;i++)off(pinlist[i]);
}

void POUT::loop() {
  if(t_active.isExpired() && status_setup){
    status_setup = false;
    active();
  }
}

void POUT::active(){
  digitalWrite(RELAY_ALL_PIN,HIGH);
  Serial.println("Relay Active");
}

void POUT::deactive(){
  digitalWrite(RELAY_ALL_PIN,LOW);
  Serial.println("Relay DeActive");
}


void POUT::on(int pin) {
  status[pin] = false;
  PCF.digitalWrite(pinlist[pin], status[pin]);
}


void POUT::off(int pin) {
  status[pin] = true;
  PCF.digitalWrite(pinlist[pin], status[pin]);
  
}

void POUT::toggle(int pin) {
  
  status[pin] = !status[pin];
  PCF.digitalWrite(pinlist[pin],status[pin]);
  Serial.println("pin " + String(pin) + "  status = " + String(status[pin]));
}


void POUT::reset(){
  for(int i = 0;i<8;i++)off(i);
}


