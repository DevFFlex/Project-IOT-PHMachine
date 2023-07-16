#include "esp32-hal-gpio.h"
#include <PCF8574.h>

#include "Hardware/KeypadClass.h"
#include "Hardware/LcdClass.h"

#include "Hardware/RTC_Class.h"
#include "Hardware/PHSensorClass.h"
#include "Hardware/WaterSensor_Class.h"
#include "Hardware/SD_Class.h"
#include "Hardware/Buzzer.h"
#include "Hardware/DHT_Sensor.h"
#include "Hardware/FloatSwitch.h"
#include "Hardware/Relay6CH.h"

class HardwareIO
{
private:
  Variable *var;

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
  FloatSwitch *floatswitch;

  Timer t_update;

  HardwareIO(Variable *varIn) : t_update(500)
  {
    var = varIn;

    lcdOutput = new LcdOutput();
    keypadInput = new KeypadInput();
    rtc = new RTC();
    relay = new Relay6CH();
    pHSensor = new PHSensor();
    waterSensor = new WaterSensor();
    sdcard = new SDCard();
    buzzer = new Buzzer();
    dhtsensor = new DHTSensor();
    floatswitch = new FloatSwitch();
  }

  void setup();
  void loop();
  void updateVar();

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
    delete floatswitch;
  }
};

void HardwareIO::setup()
{
  lcdOutput->setup();
  keypadInput->setup();
  rtc->setup();
  relay->setup();
  pHSensor->setup();
  waterSensor->setup();
  sdcard->setup();
  buzzer->setup();
  dhtsensor->setup();
  floatswitch->setup();
}

void HardwareIO::loop()
{
  lcdOutput->loop();
  keypadInput->loop();
  rtc->loop();
  relay->loop();
  pHSensor->loop();
  waterSensor->loop();
  sdcard->loop();
  buzzer->loop();
  dhtsensor->loop();
  floatswitch->loop();
}

void HardwareIO::updateVar()
{
  if (t_update.isExpired())
  {
    var->mixTank_pH = pHSensor->getPH();
    var->humidity = dhtsensor->getHumidity();
    var->tempC = dhtsensor->getTempC();

  
  }
}