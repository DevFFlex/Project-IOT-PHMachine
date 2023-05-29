#include "KeypadClass.h"
#include "LcdClass.h"

#include "RTC_Class.h"
#include "Pump_Class.h"
#include "PCF_Class.h"
#include "PHSensorClass.h"


class HardwareIO {
public:
  LcdOutput *lcdOutput;
  KeypadInput *keypadInput;
  RTC *rtc;
  POUT *relay;
  PHSensor *pHSensor;

  HardwareIO() {
    lcdOutput = new LcdOutput();
    keypadInput = new KeypadInput();
    rtc = new RTC();
    relay = new POUT();
    pHSensor = new PHSensor();
  }

  void setup();


  void loop();


  ~HardwareIO() {
    Serial.println("hello WOrld");
    delete lcdOutput;
    delete keypadInput;
    delete rtc;
    delete pHSensor;
  }
};


void HardwareIO::setup(){
  lcdOutput->setup();
    keypadInput->setup();
    rtc->setup();
    relay->setup();
    pHSensor->setup();
}


void HardwareIO::loop(){

}