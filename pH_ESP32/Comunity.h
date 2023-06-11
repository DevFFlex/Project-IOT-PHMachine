// #include "HardwareIO.h"

class Comunity {

private:
  ServerPH* serverPH;
  VarObject* varObject;
  HardwareIO* hardwareIO;
  StringManage* stringManage;

public:
  Comunity(VarObject* varObjectIn, HardwareIO* hardwareIOIn,StringManage* stringManageIn) {
    serverPH = new ServerPH();
    varObject = varObjectIn;
    hardwareIO = hardwareIOIn;
    stringManage = stringManageIn;
  }

  ~Comunity(){
    delete serverPH;
  }

  void onClientMessage(String message);

  //------------ send to client function ------------------
  void sendInputPH();
  void sendMixTankPH();
  void sendUseTankPH();
  void sendTimeList();
  void sendOutputText(String output_text);
  void sendRTCTime();

  //------------ get value from client and set variable server function ----------------
  void recvInputPH(String value);
  void recvTimelist(String value);
  void recvTimerAutoWork(String value);

  void setup();
  void loop();



  //------------ help function ----------------

  String formatS2CTimelist(TimerAutoWork* timerautowork);
  String* timeboardFromCToSFormatToString(String queryStringFromClient);
  byte* timeboardFromCToSFormatToByte(String queryStringFromClient);
  String formatS2CrtcTime();
};


void Comunity::setup(){
  serverPH->setup();
  serverPH->setOnMessageListener(std::bind(&Comunity::onClientMessage,this, std::placeholders::_1));
}

void Comunity::loop(){
  serverPH->loop();
}

void Comunity::onClientMessage(String str_trim) {

  String databox1[2];
  stringManage->split(databox1, str_trim, ":", 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  stringManage->split(databox2, commands, "=", 2);
  String command = databox2[0];
  String value = databox2[1];

  Serial.print("str_trim = " + str_trim + "       ");
  Serial.print("header = " + header + "   commands = " + commands + "       ");
  Serial.println("command = " + command + "   value = " + value);


  if (header == "SET") {

    if (command == "TIME_BOARD") recvTimelist(value);
    if (command == "INPUT_PH") recvInputPH(value);
    if (command == "TIME_LIST") recvTimerAutoWork(value);


  } else if (header == "GET") {

    if (commands == "INPUT_PH") sendInputPH();
    if (commands == "MIXTANK_PH") sendMixTankPH();
    if (commands == "USETANK_PH") sendUseTankPH();
    if (commands == "TIME_LIST") sendTimeList();
  }

  Serial.println("-------------------Timelist----------------------");
  Serial.println(varObject->getTimeListToString());

  Serial.println("-------------------Timerlist----------------------");
  for (int i = 0; i < 4; i++) {
    Serial.println(varObject->timerautowork[i].toString());
  }

}

void Comunity::sendOutputText(String output_text) {
  serverPH->send("SET:OUTPUT=" + output_text);
}


void Comunity::sendInputPH() {
  serverPH->send("SET:INPUT_PH=" + String(varObject->getInputPH()));
}

void Comunity::sendMixTankPH() {
  serverPH->send("SET:MIXTANK_PH=" + String(varObject->getMixTankpH()));
}

void Comunity::sendUseTankPH() {
  serverPH->send("SET:USETANK_PH=" + String(varObject->getUseTankPH()));
}

void Comunity::sendTimeList() {

  serverPH->send("SET:TIME_LIST=" + formatS2CTimelist(varObject->timerautowork));
}


void Comunity::sendRTCTime() {
  serverPH->send("SET:RTC_TIME=" + formatS2CrtcTime());
}





void Comunity::recvInputPH(String value) {
  varObject->setInputPH(value.toFloat());
  sendOutputText("Ok,Server recv value " + String(value.toFloat()) + " from Client");
}

void Comunity::recvTimelist(String value) {
  byte hour = byte(timeboardFromCToSFormatToString(value)[0].toInt());
  byte minute = byte(timeboardFromCToSFormatToString(value)[1].toInt());
  byte second = byte(timeboardFromCToSFormatToString(value)[2].toInt());
  byte dayofweek = byte(timeboardFromCToSFormatToString(value)[3].toInt());
  byte dayofmonth = byte(timeboardFromCToSFormatToString(value)[4].toInt());
  byte month = byte(timeboardFromCToSFormatToString(value)[5].toInt());
  byte year = byte(timeboardFromCToSFormatToString(value)[6].toInt());


  hardwareIO->rtc->setTime(hour, minute, second, dayofweek, dayofmonth, month, year);
}


void Comunity::recvTimerAutoWork(String value){
  String datalayer1[4];
  stringManage->split(datalayer1, value, "|", 4);

  for (int i = 0; i < 4; i++) {
    String datalayer2[5];
    stringManage->split(datalayer2, datalayer1[i], ",", 5);

    varObject->timerautowork[i].setHour(datalayer2[0].toInt());
    varObject->timerautowork[i].setMinute(datalayer2[1].toInt());
    varObject->timerautowork[i].setSecond(datalayer2[2].toInt());
    varObject->timerautowork[i].setStatus((datalayer2[3] == "true") ? true : false);
    varObject->timerautowork[i].setPH(datalayer2[4].toFloat());
    varObject->timerautowork[i].setDelete((datalayer2[0].toInt() == -1) ? true : false);
  }

}

String Comunity::formatS2CrtcTime() {
  String dout = "";
  dout += String(hardwareIO->rtc->getHour()) + ",";
  dout += String(hardwareIO->rtc->getMinute()) + ",";
  dout += String(hardwareIO->rtc->getSecond()) + ",";
  dout += String(hardwareIO->rtc->getDayOfWeek()) + ",";
  dout += String(hardwareIO->rtc->getDayOfMouth()) + ",";
  dout += String(hardwareIO->rtc->getMonth()) + ",";
  dout += String(hardwareIO->rtc->getYear());

  return dout;
}




String Comunity::formatS2CTimelist(TimerAutoWork* timerlist) {
  String format = "";
  for (int i = 0; i < 4; i++) {
    format += String(timerlist[i].getHour()) + ",";
    format += String(timerlist[i].getMinute()) + ",";
    format += String(timerlist[i].getSecond()) + ",";
    format += (timerlist[i].getStatus()) ? "true," : "false,";
    format += String(timerlist[i].getPH()) + ",";
    format += (timerlist[i].getDelete()) ? "true" : "false";
    if (i != 3) format += "#";
  }

  return format;
}



String* Comunity::timeboardFromCToSFormatToString(String queryStringFromClient) {
  byte numsize = 7;
  String* item = new String[numsize];
  stringManage->split(item, queryStringFromClient, ",", 7);
  return item;
}


byte* Comunity::timeboardFromCToSFormatToByte(String queryStringFromClient) {
  byte numsize = 7;
  String* item = new String[numsize];
  stringManage->split(item, queryStringFromClient, ",", 7);

  byte itemByte[numsize];
  int itemInt[numsize];

  for (int i = 0; i < numsize; i++) {
    itemInt[i] = item[i].toInt();
    itemByte[i] = byte(itemInt[i]);
  }

  return itemByte;
}


