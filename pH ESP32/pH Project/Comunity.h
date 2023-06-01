// #include "HardwareIO.h"

class Comunity {

private:
  ServerPH* serverPH;
  VarObject* varObject;
  HardwareIO* hardwareIO;
  StringManage stringManage;
public:
  Comunity(ServerPH* serverPHIn, VarObject* varObjectIn, HardwareIO* hardwareIOIn) {
    serverPH = serverPHIn;
    varObject = varObjectIn;
    hardwareIO = hardwareIOIn;
  }

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





  //------------ help function ----------------

  String formatS2CTimelist(Timerlist* timerlist);
  String* timeboardFromCToSFormatToString(String queryStringFromClient);
  byte* timeboardFromCToSFormatToByte(String queryStringFromClient);
  String formatS2CrtcTime();
};


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

  serverPH->send("SET:TIME_LIST=" + formatS2CTimelist(varObject->timerlist));
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




String Comunity::formatS2CTimelist(Timerlist* timerlist) {
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
  stringManage.split(item, queryStringFromClient, ",", 7);
  return item;
}


byte* Comunity::timeboardFromCToSFormatToByte(String queryStringFromClient) {
  byte numsize = 7;
  String* item = new String[numsize];
  stringManage.split(item, queryStringFromClient, ",", 7);

  byte itemByte[numsize];
  int itemInt[numsize];

  for (int i = 0; i < numsize; i++) {
    itemInt[i] = item[i].toInt();
    itemByte[i] = byte(itemInt[i]);
  }

  return itemByte;
}


