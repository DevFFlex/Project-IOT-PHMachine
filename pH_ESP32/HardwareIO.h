#include "esp32-hal-gpio.h"
#include <PCF8574.h>

#include "ParentClass/HardwareObject.h"

#include "Hardware/KeypadClass.h"
#include "Hardware/LcdClass.h"

#include "Hardware/RTC_Class.h"
#include "Hardware/SD_Class.h"
#include "Hardware/Buzzer.h"
#include "Hardware/Relay6CH.h"

class HardwareIO : public System{
public:
  LcdOutput *lcdOutput;
  KeypadInput *keypadInput;
  RTC *rtc;
  Relay6CH *relay;
  SDCard *sdcard;
  Buzzer *buzzer;

  HardwareIO()
  {

    lcdOutput = new LcdOutput();
    keypadInput = new KeypadInput();
    rtc = new RTC();
    relay = new Relay6CH();
    sdcard = new SDCard();
    buzzer = new Buzzer();
  }

  ~HardwareIO()
  {
    delete lcdOutput;
    delete keypadInput;
    delete rtc;
    delete relay;
    delete sdcard;
    delete buzzer;
  }

  void setup() override {
    lcdOutput->setup();
    keypadInput->setup();
    rtc->setup();
    relay->setup();
    sdcard->setup();
    buzzer->setup();
  }

  void loop() override {
    keypadInput->loop();
    rtc->loop();
    relay->loop();
    sdcard->loop();
    buzzer->loop();
  }

  
};

