#include "Comunity/WiFiControll.h"

#include "Comunity/ArduinoComunity.h"
#include "Comunity/ClientComunity.h"
#include "Comunity/CouldComunity.h"


class Comunity : public System
{
  private:
  Variable *var;
  String *item_parameter = new String[6];

  String encodeTimeAutoWork();
  String encodeTimeBoard();
  String encodeFileDir(String path);

  void onTimeUpdateAPP(){
    String queryData = "";
    queryData += String(var->mixTank_pH) + ",";
    queryData += String(var->tempC) + ",";
    queryData += String(var->humidity) + ",";
    // WorkVal
    queryData += String(var->workVar.step) + ",";
    queryData += String(var->workVar.working_status) + ",";

    // relay-status
    queryData += String(var->hardwareIO->relay->status[0]) + ",";
    queryData += String(var->hardwareIO->relay->status[1]) + ",";
    queryData += String(var->hardwareIO->relay->status[2]) + ",";
    queryData += String(var->hardwareIO->relay->status[3]) + ",";
    queryData += String(var->hardwareIO->relay->status[4]) + ",";
    queryData += String(var->hardwareIO->relay->status[5]) + ",";

    // timeBoard
    queryData += String(var->hardwareIO->rtc->getHour()) + ",";
    queryData += String(var->hardwareIO->rtc->getMinute()) + ",";
    queryData += String(var->hardwareIO->rtc->getSecond()) + ",";

    // FloatSwitch
    queryData += String(var->fsw_mixTank_Up) + ",";
    queryData += String(var->fsw_mixtank_Down) + ",";

    // Wifi Connected
    queryData += String(wifi_controll->getSTAWifiStatusConnected());

    clientComunity->sendUpdateApp(queryData);
  }




  void onClientAdjPH(String data){
    if (data.indexOf("stop") != -1)
    {
      Serial.println("หยุดปรับ");
      var->workVar.working_status = false;
    }
    else
    {
      Serial.println("เริ่มปรับ");
      var->input_ph = data.toFloat();
      var->workVar.working_status = true;

    }
  }

  void onClientSetTimeAutoWork(String data){
    String datalayer1[4];
    splitString(datalayer1, data, "|", 4);

    for (int i = 0; i < 4; i++)
    {
      String datalayer2[4];
      splitString(datalayer2, datalayer1[i], ",", 4);

      var->timerautowork[i].setHour(datalayer2[0].toInt());
      var->timerautowork[i].setMinute(datalayer2[1].toInt());
      var->timerautowork[i].setStatus((datalayer2[2] == "true") ? true : false);
      var->timerautowork[i].setPH(datalayer2[3].toFloat());
      var->timerautowork[i].setDelete((datalayer2[0].toInt() == -1) ? true : false);
    }

    var->db->writeTimeAutoWork(var->timerautowork);
    clientComunity->sendOutput("Server Set TimeAutoWork Success");

  }

  void onClientGetTimeAutoWork(String data){
    clientComunity->sendTimeAutoWork(encodeTimeAutoWork());
  }

  void onClientSetTimeBoard(String data){
    byte numsize = 7;
    String *item = new String[numsize];
    splitString(item, data, ",", 7);

    byte hour = byte(item[0].toInt());
    byte minute = byte(item[1].toInt());
    byte second = byte(item[2].toInt());
    byte dayofweek = byte(item[3].toInt());
    byte dayofmonth = byte(item[4].toInt());
    byte month = byte(item[5].toInt());
    byte year = byte(item[6].toInt());

    var->hardwareIO->rtc->setTime(hour, minute, second, dayofweek, dayofmonth, month, year);
  }

  void onClientRelayControll(String data){

    splitString(item_parameter,data,",",3);
    String header = item_parameter[0];
    String pin = item_parameter[1];
    String time = item_parameter[2];

    if(header == "on"){
      if(time == "-1")var->hardwareIO->relay->on(pin.toInt());
      else var->hardwareIO->relay->on(pin.toInt(),time.toInt());
    }else if(header == "off"){
      if(time == "-1")var->hardwareIO->relay->off(pin.toInt());
      else var->hardwareIO->relay->off(pin.toInt(),time.toInt());
    }else if(header == "toggle"){
      if(time == "-1")var->hardwareIO->relay->toggle(pin.toInt());
      else var->hardwareIO->relay->toggle(pin.toInt(),time.toInt());
    }

  }


  void onArduinoAvailable(String databuffer){
    String item[4];
    splitString(item, databuffer, ",", 6);
    var->mixTank_pH = item[0].toFloat();
    var->fsw_mixTank_Up = (item[3] == "1") ? true : false;
    var->fsw_mixtank_Down = (item[4] == "1") ? true : false;
  }


  public:
  WiFiControll *wifi_controll;
  ArduinoComunity *ardunoComunity;
  ClientComunity *clientComunity;
  CouldComunity *couldComunity;

  Comunity(Variable *var){
    this->var = var;

    wifi_controll = new WiFiControll(var);
    ardunoComunity = new ArduinoComunity(var);
    clientComunity = new ClientComunity(var,wifi_controll);
    couldComunity = new CouldComunity(var,wifi_controll);


    clientComunity->clientComunityCallback.onTimeUpdateAPP = std::bind(&Comunity::onTimeUpdateAPP,this);
    clientComunity->clientComunityCallback.onClientRelayControll = std::bind(&Comunity::onClientRelayControll,this,std::placeholders::_1);
    clientComunity->clientComunityCallback.onClientAdjPHStart = std::bind(&Comunity::onClientAdjPH,this,std::placeholders::_1);
    clientComunity->clientComunityCallback.onClientSetTimeBoard = std::bind(&Comunity::onClientSetTimeBoard,this,std::placeholders::_1);
    clientComunity->clientComunityCallback.onClientSetTimeAutoWork = std::bind(&Comunity::onClientSetTimeAutoWork,this,std::placeholders::_1);
    clientComunity->clientComunityCallback.onClientGetTimeAutoWork = std::bind(&Comunity::onClientGetTimeAutoWork,this,std::placeholders::_1);

    ardunoComunity->arduinoComunityCallback.onArduinoAvailable = std::bind(&Comunity::onArduinoAvailable,this,std::placeholders::_1);
  }


  void setup() override{
    wifi_controll->setup();

    ardunoComunity->setup();
    clientComunity->setup();
    couldComunity->setup();
  }

  void loop() override{
    wifi_controll->loop();

    ardunoComunity->loop();
    clientComunity->loop();
    couldComunity->loop();

  }


};







String Comunity::encodeTimeAutoWork()
{
  String format = "";
  for (int i = 0; i < 4; i++)
  {
    format += String(var->timerautowork[i].getHour()) + ",";
    format += String(var->timerautowork[i].getMinute()) + ",";
    format += (var->timerautowork[i].getStatus()) ? "true," : "false,";
    format += String(var->timerautowork[i].getPH()) + ",";
    format += (var->timerautowork[i].getDelete()) ? "true" : "false";
    if (i != 3)
      format += "#";
  }
  return format;
}

String Comunity::encodeFileDir(String path)
{
  return var->hardwareIO->sdcard->listDir(path.c_str(), 0);
}
