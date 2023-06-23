#include "Server.h"
#include "Server/ServerClass.h"

#define H_GET "GET"
#define H_SET "SET"
#define H_MESSAGE "MESSAGE"

#define SYMBOL_L1 ":"
#define SYMBOL_L2 "="

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
      : ServerPH(&SSID_AP, &PASS_AP) , autosendTimer(1000)
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
  void setC_InputPH();
  void setC_InputPH_Finish();
  void setC_MixtankPH();
  void setC_Temp();
  void setC_TimeAutoWork();
  void setC_TimeBoard();
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

  void H_GetSetProcess(String header_arg, String command_arg, String value);
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

  if(autosendTimer.isExpired()){
    setC_MixtankPH();
    setC_CMVM();
  }
}

void Comunity::onClientMessage(String str_trim)
{

  String databox1[2];
  var->strManager->split(databox1, str_trim, SYMBOL_L1, 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  var->strManager->split(databox2, commands, SYMBOL_L2, 2);
  String command = databox2[0];
  String value = databox2[1];

  if (header == H_GET || header == H_SET)
  {
    H_GetSetProcess(header, command, value);
  }
  else if (header == H_MESSAGE)
  {
    H_MessageProcess(command, value);
  }
}

void Comunity::H_GetSetProcess(String header_arg, String command_arg, String value)
{
  Serial.print("H = " + header_arg);
  Serial.print("             ");
  Serial.print("C = " + command_arg);
  Serial.print("             ");
  Serial.println("V = " + value);

  if (command_arg == "INPUT_PH")
  {
    if (header_arg == H_GET)
      setC_InputPH();
    else if (header_arg == H_SET)
    {
      recvInputPH(value);
      // setC_Output("Server set ph value " + String(value.toFloat()) + " success");
    }
  }

  if (command_arg == "MIXTANK_PH")
  {
    if (header_arg == H_GET)
      setC_MixtankPH();
  }

  if (command_arg == "TEMP")
  {
    if (header_arg == H_GET)
      setC_Temp();
  }

  if (command_arg == "TIME_AUTO_WORK")
  {
    if (header_arg == H_GET)
      setC_TimeAutoWork();
    else if (header_arg == H_SET)
    {
      recvTimerAutoWork(value);
      setC_Output("Server set timeAutoWork value " + String(value.toFloat()) + " success");
    }
  }

  if (command_arg == "RTC_TIME")
  {
    if (header_arg == H_GET)
      setC_TimeBoard();
    else if (header_arg == H_SET)
    {
      recvTimeBoard(value);
      setC_Output("Server set timeBoard value " + String(value.toFloat()) + " success");
    }
  }

  if (command_arg == "TOGGLE_RELAY")
  {
    if (header_arg == H_SET)
    {
      recvToggleRelay(value);
      setC_Output("Server toggle relay " + String(value.toFloat()) + " success");
    }
  }

  if (command_arg == "FILE_DIR")
  {
    if (header_arg == H_GET)
      setC_FileDir(value);
  }

  if (command_arg == "CMVM")
  {
    if (header_arg == H_GET)
      setC_CMVM();
    else if (header_arg == H_SET)
    {
      recvCMVM(value);
      setC_Output("Server set cmvm value " + String(value.toFloat()) + " success");
    }
  }
}

void Comunity::H_MessageProcess(String command_arg, String value)
{
  String data = String(H_MESSAGE) + String(SYMBOL_L1) + command_arg + String(SYMBOL_L2) + value;
  Serial.println("Comunity : " + data);
  ServerPH::send(data);
}

void Comunity::setC_InputPH()
{
  ServerPH::send("SET:INPUT_PH=" + String(var->input_ph));
}

void Comunity::setC_InputPH_Finish()
{
  ServerPH::send("SET:INPUT_PH=FINISH");
}

void Comunity::setC_MixtankPH()
{
  ServerPH::send("SET:MIXTANK_PH=" + String(var->mixTank_pH));
}

void Comunity::setC_Temp()
{
  ServerPH::send("SET:TEMP=" + String(var->temp));
}

void Comunity::setC_TimeAutoWork()
{
  ServerPH::send("SET:TIME_AUTO_WORK=" + encodeTimeAutoWork());
}

void Comunity::setC_TimeBoard()
{
  ServerPH::send("SET:RTC_TIME=" + encodeTimeBoard());
}

void Comunity::setC_FileDir(String path)
{
  ServerPH::send("SET:FILE_DIR=" + encodeFileDir(path));
}

void Comunity::setC_CMVM()
{
  ServerPH::send("SET:CMVM=" + encodeCMVM());
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
  if(value.indexOf("stop") != -1){
    var->workVar.working_status = false;
  }else{
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


void Comunity::recvCMVM(String value){
  String data[4];
  var->strManager->split(data,value,",",4);
  Serial.println(data[0]);

  if(data[0] == "-9999" && data[1] == "-9999" && data[2] == "-9999" && data[3] == "-9999"){
    ar_com->setValueAllDefault();
    return;
  }

  String clb = (data[0] == "") ? String(var->phCalibrateSet->calibrate) : data[0];
  String m = (data[1] == "") ? String(var->phCalibrateSet->m) : data[1];
  String vi = (data[2] == "") ? String(var->phCalibrateSet->voltin) : data[2];
  String ma = (data[3] == "") ? String(var->phCalibrateSet->max_analog) : data[3];

  ar_com->setValueAll(clb,m,vi,ma);
}













String Comunity::encodeTimeBoard()
{
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

String Comunity::encodeCMVM()
{
  String d_out = "";
  d_out += String(var->phCalibrateSet->calibrate) + ",";
  d_out += String(var->phCalibrateSet->m) + ",";
  d_out += String(var->phCalibrateSet->voltin) + ",";
  d_out += String(var->phCalibrateSet->max_analog) + ",";
  d_out += String(var->phCalibrateSet->analogAvg);
  return d_out;
}
