#include "OtherClass/TimerClass.h"
#include "OtherClass/StringManager.h"

#include "Variable.h"
#include "HardwareIO.h"
#include "Database.h"
#include "ArduinoComunity.h"

#include "Comunity.h"
#include "SerialInput.h"
#include "UserInterface.h"


//------------- Class ------------------------

Variable *var = new Variable();
HardwareIO *hardwareIO = new HardwareIO();
Database *db = new Database(var,hardwareIO);
ArduinoComunity *ardunoComunity = new ArduinoComunity(var,hardwareIO);

Comunity *comunity = new Comunity(var,hardwareIO,db,ardunoComunity);


SerialInput *sInput = new SerialInput(var,hardwareIO,comunity,ardunoComunity);
UserInterface *ui = new UserInterface(var,hardwareIO);


Timer t1(1000);


//---------------------------------------------- setup loop -----------------------------------

void setup() {
  Serial.begin(115200);
  db->setup();
  // Serial.setTimeout(50);
  comunity->setup();
  hardwareIO->setup();
  ardunoComunity->setup();
  sInput->setup();
}

void loop() {
  db->loop();

  comunity->loop();
  hardwareIO->loop();
  ardunoComunity->loop();
  sInput->loop();
  ui->loop();
  

  if (t1.isExpired()) {

    var->mixTank_pH = hardwareIO->pHSensor->getPH();
    // hardwareIO->buzzer->on();

    hardwareIO->lcdOutput->printL("PH = " + String(hardwareIO->pHSensor->getPH()) + " | " + hardwareIO->pHSensor->getPHString(), 0);

    hardwareIO->lcdOutput->printL(hardwareIO->rtc->getTimeToString(), 3);

  
    

  }
}

//---------------------------------------------------------------------------------------------

bool timerAutoWork_Compare_Rtctime(int index) {
  byte active_hour = var->timerautowork[index].getHour();
  byte active_minute = var->timerautowork[index].getMinute();

  if (active_hour == -1)
    return false;

  return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute);
}
