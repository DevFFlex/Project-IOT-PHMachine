#include "Server.h"
#include "Server/ServerClass.h"

class Comunity : public ServerPH
{
  Variable *var;
  HardwareIO *hardwareIO;
  Database *db;
  ArduinoComunity *ar_com;

  Timer autosendTimer;

private:
  String SSID_AP = "PH-Project-Controller";
  String PASS_AP = "12345678";

  void onClientMessage(String message);

public:
  Comunity(Variable *varIn, HardwareIO *hardwareIOIn, Database *dbIn, ArduinoComunity *ar_comIn)
      : ServerPH(&SSID_AP, &PASS_AP), autosendTimer(1000)
  {
    var = varIn;
    hardwareIO = hardwareIOIn;
    db = dbIn;
    ar_com = ar_comIn;
  }

  ~Comunity()
  {
  }

  //------------ send to client function ------------------

  void setC_FileDir(String path);
  void setC_CMVM();
  void setC_Output(String output_str);

  void sendOther(String data_str);

  String encodeTimeAutoWork();
  String encodeTimeBoard();
  String encodeFileDir(String path);
  String encodeCMVM();

  //------------ get value from client and set variable server function ----------------
  void recvInputPH(String value);
  void recvTimeBoard(String value);
  void recvTimerAutoWork(String value);
  void recvToggleRelay(String value);
  void recvCMVM(String value);

  void setup();
  void loop();

  void updateApp();

  void H_MessageProcess(String command_arg, String value);
};

void Comunity::setup()
{
  ServerPH::setup();
  ServerPH::setOnMessageListener(std::bind(&Comunity::onClientMessage, this, std::placeholders::_1));
}

void Comunity::loop()
{
  ServerPH::loop();

  if (autosendTimer.isExpired())
  {
    updateApp();
  }
}

void Comunity::onClientMessage(String str_trim)
{

  String databox1[2];
  var->strManager->split(databox1, str_trim, ":", 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  var->strManager->split(databox2, commands, "=", 2);
  String command = databox2[0];
  String value = databox2[1];

  String str_clientname = header;
  String str_command = command;
  String str_value = value;

  if (str_command.indexOf("MESSAGE") != -1)
  {
    String data = str_clientname + ":" + str_command + "=" + value;
    Serial.println("Comunity : " + data);
    ServerPH::send(data);
  }

  if (str_command.indexOf("RELAY") != -1)
  {
    hardwareIO->relay->toggle(str_value.toInt());
    ServerPH::send(str_clientname + ":" + str_command + "_RES=" + str_value + "," + String(hardwareIO->relay->status[str_value.toInt()]));
    
  }

  if(str_command.indexOf("INPUT_PH") != -1){
    var->workVar.working_status = true;
  }
}

void Comunity::updateApp()
{
  String data = "SERVER:UPDATE=";

  // MainVal
    data += String(var->mixTank_pH) + ",";
    data += String(var->tempC) + ",";
    data += String(var->humidity) + ",";

    // pHCalibreteVal
    data += String(var->phCalibrateSet.calibrate) + ",";
    data += String(var->phCalibrateSet.m) + ",";
    data += String(var->phCalibrateSet.voltin) + ",";
    data += String(var->phCalibrateSet.max_analog) + ",";
    data += String(var->phCalibrateSet.analogAvg) + ",";

    // WorkVal
    data += String(var->workVar.step) + ",";
    data += String(var->workVar.working_status);

    ServerPH::send(data);
    // Serial.println(data);
}

void Comunity::setC_FileDir(String path)
{
  ServerPH::send("SET:FILE_DIR=" + encodeFileDir(path));
}

void Comunity::setC_Output(String output_text)
{
  ServerPH::send("SET:OUTPUT=" + output_text);
}

void Comunity::sendOther(String data_str)
{
  ServerPH::send(data_str);
}

void Comunity::recvInputPH(String value)
{
  if (value.indexOf("stop") != -1)
  {
    var->workVar.working_status = false;
  }
  else
  {
    var->input_ph = value.toFloat();
    var->workVar.working_status = true;
  }
}

void Comunity::recvTimerAutoWork(String value)
{
  String datalayer1[4];
  var->strManager->split(datalayer1, value, "|", 4);

  for (int i = 0; i < 4; i++)
  {
    String datalayer2[4];
    var->strManager->split(datalayer2, datalayer1[i], ",", 4);

    var->timerautowork[i].setHour(datalayer2[0].toInt());
    var->timerautowork[i].setMinute(datalayer2[1].toInt());
    var->timerautowork[i].setStatus((datalayer2[2] == "true") ? true : false);
    var->timerautowork[i].setPH(datalayer2[3].toFloat());
    var->timerautowork[i].setDelete((datalayer2[0].toInt() == -1) ? true : false);
  }
}

void Comunity::recvTimeBoard(String queryStringFromClient)
{
  byte numsize = 7;
  String *item = new String[numsize];
  var->strManager->split(item, queryStringFromClient, ",", 7);

  byte hour = byte(item[0].toInt());
  byte minute = byte(item[1].toInt());
  byte second = byte(item[2].toInt());
  byte dayofweek = byte(item[3].toInt());
  byte dayofmonth = byte(item[4].toInt());
  byte month = byte(item[5].toInt());
  byte year = byte(item[6].toInt());

  hardwareIO->rtc->setTime(hour, minute, second, dayofweek, dayofmonth, month, year);
}

void Comunity::recvToggleRelay(String value)
{
  hardwareIO->relay->toggle(value.toInt());
}

void Comunity::recvCMVM(String value)
{
  String data[4];
  var->strManager->split(data, value, ",", 4);
  Serial.println(data[0]);

  if (data[0] == "-9999" && data[1] == "-9999" && data[2] == "-9999" && data[3] == "-9999")
  {
    ar_com->setValueAllDefault();
    return;
  }

  String clb = (data[0] == "") ? String(var->phCalibrateSet.calibrate) : data[0];
  String m = (data[1] == "") ? String(var->phCalibrateSet.m) : data[1];
  String vi = (data[2] == "") ? String(var->phCalibrateSet.voltin) : data[2];
  String ma = (data[3] == "") ? String(var->phCalibrateSet.max_analog) : data[3];

  ar_com->setValueAll(clb, m, vi, ma);
}

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
  return hardwareIO->sdcard->listDir(path.c_str(), 0);
}
