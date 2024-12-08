#include "Community/WiFiControll.h"

#include "Community/ArduinoCommunity.h"
#include "Community/ClientCommunity.h"
#include "Community/CouldCommunity.h"


class Community : public System
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
    queryData += String(var->workVar.addBaseCount) + ",";
    queryData += String(var->workVar.addAcidCount);

  

    clientComunity->sendUpdateApp(queryData);
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

  void onClientJoin(){
    var->hardwareIO->buzzer->on(500);
  }


  public:
  WiFiControll *wifi_controll;
  ArduinoComunity *ardunoComunity;
  ClientComunity *clientComunity;
  CouldComunity *couldComunity;

  Community(Variable *var){
    this->var = var;

    wifi_controll = new WiFiControll(var);
    ardunoComunity = new ArduinoComunity(var);
    clientComunity = new ClientComunity(var,wifi_controll);
    couldComunity = new CouldComunity(var,wifi_controll);


    //clientComunity->clientComunityCallback.onTimeUpdateAPP = std::bind(&Community::onTimeUpdateAPP,this);
    clientComunity->clientComunityCallback.onClientSerialAvailable = std::bind(&Community::onClientSerialAvailable,this,std::placeholders::_1);
    ardunoComunity->arduinoComunityCallback.onArduinoAvailable = std::bind(&Community::onArduinoAvailable,this,std::placeholders::_1);
    clientComunity->clientComunityCallback.onClientJoin = std::bind(&Community::onClientJoin,this);
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


String Community::encodeWorkTimer()
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

String Community::encodeFileDir(String path)
{
  return var->hardwareIO->sdcard->listDir(path.c_str(), 0);
}
