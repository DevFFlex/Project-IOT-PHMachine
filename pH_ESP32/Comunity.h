#include "Comunity/WiFiControll.h"

#include "Comunity/ArduinoComunity.h"
#include "Comunity/ClientComunity.h"
#include "Comunity/CouldComunity.h"


class Comunity : public System
{
  private:
  Variable *var;
  String *item_parameter = new String[6];

  String encodeWorkTimer();
  String encodeTimeBoard();
  String encodeFileDir(String path);

  void onTimeUpdateAPP(){
    String queryData = "";
    queryData += String(var->mixTank_pH) + ",";
    queryData += String(var->tempC) + ",";
    queryData += String(var->humidity) + ",";
    

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
    queryData += String(var->fsw.mixTank_Up) + ",";
    queryData += String(var->fsw.mixTank_Down) + ",";

    // Wifi Connected
    queryData += String(var->wifipublic.connect_status) + ",";

    // WorkTimer
    queryData += encodeWorkTimer() + ",";
    
    // WorkVal
    queryData += String(var->workVar.step) + ",";
    queryData += String(var->workVar.working_status) + ",";
    queryData += String(var->workVar.pH_space_rate) + ",";
    queryData += String(var->workVar.adjustCurrentpH) + ",";
    queryData += String(var->workVar.wait_stirring_pump) + ",";
    queryData += String(var->workVar.wait_pH_stabilize) + ",";
    queryData += String(var->workVar.wait_acidUseTime) + ",";
    queryData += String(var->workVar.wait_baseUseTime) + ",";
    queryData += String(var->workVar.limite_use_base) + ",";
    queryData += String(var->workVar.limite_use_acid) + ",";
    queryData += String(var->workVar.adjustT_Counter) + ",";

  

    clientComunity->sendUpdateApp(queryData);
  }




  void onClientAdjPH(String data){
    String datalist[2];
    splitString(datalist, data, ",", 2);
    if (datalist[0].indexOf("stop") != -1)
    {
      var->workVar.stopAdjustPH();
    }
    else
    {
      var->workVar.startAdjustPH(datalist[0].toFloat(),datalist[1].toInt());
    }
  }

  void onClientSetTimeAutoWork(String data){
    /*String datalayer1[4];
    splitString(datalayer1, datalayer1[i], ",", 6);

    var->workTimer[i].HOUR = datalayer2[0].toInt();
    var->workTimer[i].MINUTE = datalayer2[1].toInt();
    var->workTimer[i].PH = datalayer2[2].toFloat();
    var->workTimer[i].T = datalayer2[3].toInt();
    var->workTimer[i].ACTIVE_STATUS = (datalayer2[4] == "true") ? true : false;
    var->workTimer[i].DELETE_STATUS = (datalayer2[5] == "true") ? true : false;

    var->db->writeWorkTimer(var->workTimer);
    clientComunity->sendOutput("Server Set TimeAutoWork Success");*/

  }

  void onClientGetTimeAutoWork(String data){
    clientComunity->sendTimeAutoWork(encodeWorkTimer());
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

  void onClientSerialAvailable(String data){
    Serial.println(data);
    var->serial_buffer = data;
    
  }

  void onArduinoAvailable(String databuffer){
    String item[6];
    splitString(item, databuffer, ",", 6);

    if(item[0].toFloat() == 0)return;

    var->mixTank_pH = item[0].toFloat();
    var->fsw.mixTank_Up = (item[3] == "1") ? true : false;
    var->fsw.mixTank_Down = (item[4] == "1") ? true : false;

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
    clientComunity->clientComunityCallback.onClientSerialAvailable = std::bind(&Comunity::onClientSerialAvailable,this,std::placeholders::_1);
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







String Comunity::encodeWorkTimer()
{
  String format = "";
  for (int i = 0; i < 4; i++)
  {
    format += String(var->workTimer[i].HOUR) + "SP";
    format += String(var->workTimer[i].MINUTE) + "SP";
    format += String(var->workTimer[i].PH) + "SP";
    format += String(var->workTimer[i].T) + "SP";
    format += (var->workTimer[i].ACTIVE_STATUS) ? "trueSP" : "falseSP";
    format += (var->workTimer[i].DELETE_STATUS) ? "true" : "false";
    if (i != 3)
      format += "|";
  }
  return format;
}

String Comunity::encodeFileDir(String path)
{
  return var->hardwareIO->sdcard->listDir(path.c_str(), 0);
}
