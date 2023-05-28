#include "KeypadClass.h"
#include "LcdClass.h"

#include "RTC_Class.h"
#include "Pump_Class.h"
#include "PCF_Class.h"


class HardwareIO {
public:
  LcdOutput *lcdOutput;
  KeypadInput *keypadInput;
  RTC *rtc;
  POUT *relay;

  HardwareIO() {
    lcdOutput = new LcdOutput();
    keypadInput = new KeypadInput();
    rtc = new RTC();
    relay = new POUT();
  }

  void setup(){
    lcdOutput->setup();
    keypadInput->setup();
    rtc->setup();
    relay->setup();

  }


  ~HardwareIO() {
    Serial.println("hello WOrld");
    delete lcdOutput;
    delete keypadInput;
    delete rtc;
  }
};