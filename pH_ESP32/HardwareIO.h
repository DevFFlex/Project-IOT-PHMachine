#include "esp32-hal-gpio.h"
#include <PCF8574.h>


#include "ParentClass/HardwareObject.h"

#include "Hardware/KeypadClass.h"
#include "Hardware/LcdClass.h"

#include "Hardware/RTC_Class.h"
#include "Hardware/PHSensorClass.h"
#include "Hardware/WaterSensor_Class.h"
#include "Hardware/SD_Class.h"
#include "Hardware/Buzzer.h"
#include "Hardware/DHT_Sensor.h"
#include "Hardware/Relay6CH.h"

class HardwareIO : public System{
public:
  LcdOutput *lcdOutput;
  KeypadInput *keypadInput;
  RTC *rtc;
  Relay6CH *relay;
  PHSensor *pHSensor;
  WaterSensor *waterSensor;
  SDCard *sdcard;
  Buzzer *buzzer;
  DHTSensor *dhtsensor;

  HardwareIO()
  {

    lcdOutput = new LcdOutput();
    keypadInput = new KeypadInput();
    rtc = new RTC();
    relay = new Relay6CH();
    pHSensor = new PHSensor();
    waterSensor = new WaterSensor();
    sdcard = new SDCard();
    buzzer = new Buzzer();
    dhtsensor = new DHTSensor();
  }

  ~HardwareIO()
  {
    delete lcdOutput;
    delete keypadInput;
    delete rtc;
    delete pHSensor;
    delete waterSensor;
    delete sdcard;
    delete buzzer;
    delete dhtsensor;
  }

  void setup() override {
    lcdOutput->setup();
    keypadInput->setup();
    rtc->setup();
    relay->setup();
    pHSensor->setup();
    waterSensor->setup();
    sdcard->setup();
    buzzer->setup();
    dhtsensor->setup();
  }

  void loop() override {
    lcdOutput->loop();
    keypadInput->loop();
    rtc->loop();
    relay->loop();
    pHSensor->loop();
    waterSensor->loop();
    sdcard->loop();
    buzzer->loop();
    dhtsensor->loop();
  }

  
};

