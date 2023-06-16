#include "HelpClass.h"
#include "TimerClass.h"

#include "VarObject.h"
#include "ServerClass.h"

#include "HardwareIO.h"

#include "Comunity.h"
#include "ArduinoComunity.h"

#include "UserInterface.h"

#include "Database.h"




int step = 1;

//------------- Class ------------------------
StringManage *stringManage = new StringManage();
VarObject *varObject = new VarObject();

HardwareIO *hardwareIO = new HardwareIO();

Comunity *comunity = new Comunity(varObject, hardwareIO,stringManage);
ArduinoComunity *ardunoComunity = new ArduinoComunity(hardwareIO,stringManage);

UserInterface *ui = new UserInterface(varObject,comunity,hardwareIO);

Database *db = new Database();



Timer t1(1000);

ScanI2C scanI2C;



void InputSerial() {

  if (Serial.available() > 0) {
    String data = Serial.readString();
    data.trim();

    if (data != "") {
      if (data.indexOf("on") != -1) {
        data.replace("on", "");

        hardwareIO->relay->on(data.toInt());
      }

      if (data.indexOf("off") != -1) {
        data.replace("off", "");
        hardwareIO->relay->off(data.toInt());
      }

      if (data.indexOf("toggle") != -1) {
        data.replace("toggle", "");
        hardwareIO->relay->toggle(data.toInt());
      }

      if (data.indexOf("send_") != -1){
        data.replace("send_","");
        comunity->sendOther(data);
      }

      if (data.indexOf("buzzer") != -1){
        data.replace("buzzer","");
        hardwareIO->buzzer->freq = data.toInt();
      }
    }
  }
}

//---------------------------------------------- setup loop -----------------------------------

void setup() {
  Serial.begin(115200);
  // Serial.setTimeout(50);

  comunity->setup();
  hardwareIO->setup();
  // scanI2C.setup();
  ardunoComunity->setup();
}

void loop() {
  comunity->loop();
  hardwareIO->loop();
  ardunoComunity->loop();
  ui->loop();

  InputSerial();


  if (t1.isExpired()) {
    
    varObject->setMixTankpH(hardwareIO->pHSensor->getPH());
    // comunity->sendMixTankPH();
    static int count = 0;
    // comunity->sendOutputText("การบ้าน คือ " + String(count++));
    // hardwareIO->buzzer->on();

    hardwareIO->lcdOutput->printL("PH = " + String(hardwareIO->pHSensor->getPH()) + " | " + hardwareIO->pHSensor->getPHString(), 0);

    hardwareIO->lcdOutput->printL(hardwareIO->rtc->getTimeToString(), 3);
    // hardwareIO->sdcard->listDir(SD,"/",0);

  }

}


//---------------------------------------------------------------------------------------------


bool timerAutoWork_Compare_Rtctime(int index) {
  byte active_hour = varObject->timerautowork[index].getHour();
  byte active_minute = varObject->timerautowork[index].getMinute();
  byte active_second = varObject->timerautowork[index].getSecond();

  if (active_hour == -1) return false;

  return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && hardwareIO->rtc->getSecond() == active_second);
}

