#include "KeypadClass.h"
#include "LcdClass.h"

#include "RTC_Class.h"
#include "PCF_Class.h"
#include "PHSensorClass.h"
#include "WaterSensor_Class.h"
#include "SD_Class.h"
#include "Buzzer.h"


class HardwareIO {
public:
  LcdOutput *lcdOutput;
  KeypadInput *keypadInput;
  RTC *rtc;
  POUT *relay;
  PHSensor *pHSensor;
  WaterSensor *waterSensor;
  SDCard *sdcard;
  Buzzer *buzzer;

  HardwareIO() {
    lcdOutput = new LcdOutput();
    keypadInput = new KeypadInput();
    rtc = new RTC();
    relay = new POUT();
    pHSensor = new PHSensor();
    waterSensor = new WaterSensor();
    sdcard = new SDCard();
    buzzer = new Buzzer();
  }

  void setup();

  void loop();


  ~HardwareIO() {
    delete lcdOutput;
    delete keypadInput;
    delete rtc;
    delete pHSensor;
    delete waterSensor;
    delete sdcard;
    delete buzzer;
  }
};


void HardwareIO::setup() {
  lcdOutput->setup();
  keypadInput->setup();
  rtc->setup();
  relay->setup();
  pHSensor->setup();
  waterSensor->setup();
  sdcard->setup();
  buzzer->setup();
  
}


void HardwareIO::loop() {
  lcdOutput->loop();
  keypadInput->loop();
  rtc->loop();
  relay->loop();
  pHSensor->loop();
  waterSensor->loop();
  // sdcard->loop)_;
  buzzer->loop();
}