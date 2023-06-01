
#include "ExClass.h"
#include "F_Timer.h"
#include "controllerFunction.h"

#include "server.h"
#include "VarObject.h"
#include "HardwareIO.h"

#include "Comunity.h"
#include "ArduinoComunity.h"




int step = 1;

//------------- Class ------------------------

ServerPH *serverPH = new ServerPH();
VarObject *varObject = new VarObject();
HardwareIO *hardwareIO = new HardwareIO();

Comunity *comunity = new Comunity(serverPH, varObject, hardwareIO);

ArduinoComunity *ardunoComunity = new ArduinoComunity();
StringManage stringManage;

Timer changeMixtankTimer(1000);
Timer t1(1000);
Timer timerActiveRelay(5000);
Timer timerPushWater(30000);

ScanI2C scanI2C;

//-------------------------- IO ---------------------------
void onKeypass(char c, String textNow) {
  if (c == 'C') {
    hardwareIO->keypadInput->clearTextNow();
    hardwareIO->lcdOutput->printL(1, "INPUT : ", 1);
    return;
  }

  hardwareIO->lcdOutput->printL(1, "INPUT : " + textNow, 1);
}

void onEnterkey(String text) {
  hardwareIO->lcdOutput->printL(1, "Val = " + text, 2);
  serverPH->send("S_RESP_MTPH=" + String(text));
  varObject->setInputPH(text.toFloat());
}


void onClientMessage(String str_trim) {

  String databox1[2];
  stringManage.split(databox1, str_trim, ":", 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  stringManage.split(databox2, commands, "=", 2);
  String command = databox2[0];
  String value = databox2[1];

  Serial.print("str_trim = " + str_trim + "       ");
  Serial.print("header = " + header + "   commands = " + commands + "       ");
  Serial.println("command = " + command + "   value = " + value);


  if (header == "SET") {

    if (command == "TIME_BOARD") comunity->recvTimelist(value);
    if (command == "INPUT_PH") comunity->recvInputPH(value);
    if (command == "TIME_LIST") varObject->setTimeList(value);


  } else if (header == "GET") {

    if (commands == "INPUT_PH") comunity->sendInputPH();
    if (commands == "MIXTANK_PH") comunity->sendMixTankPH();
    if (commands == "USETANK_PH") comunity->sendUseTankPH();
    if (commands == "TIME_LIST") comunity->sendTimeList();
  }

  showall();
}

void showall() {
  Serial.println("-------------------Timelist----------------------");
  Serial.println(varObject->getTimeListToString());

  Serial.println("-------------------Timerlist----------------------");
  for (int i = 0; i < 4; i++) {
    Serial.println(varObject->timerlist[i].toString());
  }
}


void InputSerial() {

  if (Serial.available() > 0) {
    String data = Serial.readString();
    data.trim();

    if (data != "") {
      if (data.indexOf("t") != -1) {
        data.replace("t", "");

        Serial.print("set time");
      }

      if (data.indexOf("d") != -1) {
      }
    }
  }
}

//---------------------------------------------- setup loop -----------------------------------

void setup() {
  Serial.begin(115200);
  // Serial.setTimeout(50);

  serverPH->setup();
  serverPH->setOnMessageListener(onClientMessage);

  hardwareIO->setup();

  hardwareIO->keypadInput->setOnKeypressListener(onKeypass);
  hardwareIO->keypadInput->setOnKeyEnterListener(onEnterkey);

  hardwareIO->pHSensor->setup();



  // scanI2C.setup();
  ardunoComunity->setup();
}

void loop() {
  ardunoComunity->loop();

  InputSerial();
  serverPH->loop();

  hardwareIO->loop();


  if (t1.isExpired()) {
    varObject->setMixTankpH(hardwareIO->pHSensor->getPH());
    comunity->sendMixTankPH();
    hardwareIO->lcdOutput->printL(1, "PH = " + String(hardwareIO->pHSensor->getPH()), 0);
    hardwareIO->lcdOutput->printL(1, "Volt = " + String(hardwareIO->pHSensor->getVolt()), 1);
    hardwareIO->lcdOutput->printL(1, "WaterSensor = " + String(hardwareIO->waterSensor->getValue()), 2);
    hardwareIO->lcdOutput->printL(1, hardwareIO->rtc->getTimeToString(), 3);

    // hardwareIO->relay->toggle(2);
  }


  switch (step) {
    case 1:
      if (hardwareIO->waterSensor->getValue() < 600) {
        hardwareIO->relay->on(5);
        Serial.println("peris on");
        
      } else {
        hardwareIO->relay->off(5);
        Serial.println("peris off");
        step = 2;
        timerPushWater.reset();
      }
      break;

    case 2:
      if (timerPushWater.isExpired()){
        step = 1;
        hardwareIO->relay->off(3);
      }else{
        hardwareIO->relay->on(3);
      }
      break;
  }

  timerOpenRelayLoop();
}





bool timelist_compare_rtctime(int index) {
  byte active_hour = varObject->timerlist[index].getHour();
  byte active_minute = varObject->timerlist[index].getMinute();
  byte active_second = varObject->timerlist[index].getSecond();

  if (active_hour == -1) return false;

  return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && hardwareIO->rtc->getSecond() == active_second);
}

void timerOpenRelayLoop() {

  static bool active_finish = false;
  static int offdelay = 30000;

  if ((timelist_compare_rtctime(0) || timelist_compare_rtctime(1) || timelist_compare_rtctime(2) || timelist_compare_rtctime(3)) && !active_finish) {

    hardwareIO->relay->on(0);
    active_finish = true;
    timerActiveRelay.reset();
    timerActiveRelay.setInterval(offdelay);
    Serial.println("is Time");
  }

  if (active_finish) {

    if (timerActiveRelay.isExpired()) {
      hardwareIO->relay->off(0);
      Serial.println("Active = false");
      active_finish = false;
    }
  }
}

//---------------------------------------------------------------------------------------------
