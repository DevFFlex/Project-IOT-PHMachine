#include "Server.h"
#include "ServerClass.h"


#define H_GET "GET"
#define H_SET "SET"
#define H_MESSAGE "MESSAGE"

#define SYMBOL_L1 ":"
#define SYMBOL_L2 "="


class Comunity : public ServerPH {
  Variable *var;
  HardwareIO *hardwareIO;
  Database *db;
  ArduinoComunity *ar_com;

private:

  String SSID_AP = "PH-Project-Controller";
  String PASS_AP = "12345678";

#define COMMAND_SIZE 10
  String COMMAND[COMMAND_SIZE] = {
    "INPUT_PH",
    "MIXTANK_PH",
    "USETANK_PH",
    "TIME_AUTO_WORK",
    "RTC_TIME",
    "OUTPUT",
    "TOGGLE_RELAY",
    "FILE_DIR",
    "DEL_FILE_DIR",
    "CMVM"

  };

  void onClientMessage(String message);

public:
  Comunity(Variable *varIn,HardwareIO *hardwareIOIn,Database *dbIn,ArduinoComunity *ar_comIn)
    : ServerPH(&SSID_AP, &PASS_AP) {
      var = varIn;
      hardwareIO = hardwareIOIn;
      db = dbIn;
      ar_com = ar_comIn;
  }

  ~Comunity() {
  }

  //------------ send to client function ------------------
  void sendInputPH();
  void sendMixTankPH();
  void sendTimeAutoWork();
  void sendTimeBoard();
  
  void sendOutputText(String output_text);
  void sendOther(String data_str);

  String encodeTimeAutoWork();
  String encodeTimeBoard();
  String encodeFileDir(String path);

  //------------ get value from client and set variable server function ----------------
  void recvInputPH(String value);
  void recvTimeBoard(String value);
  void recvTimerAutoWork(String value);
  void recvToggleRelay(String value);

  void setup();
  void loop();

  void H_GetSetProcess(String header_arg, String command_arg, String value, int index);
  void H_MessageProcess(String command_arg, String value);
};


void Comunity::setup() {
  ServerPH::setup();
  ServerPH::setOnMessageListener(std::bind(&Comunity::onClientMessage, this, std::placeholders::_1));
}

void Comunity::loop() {
  ServerPH::loop();
}

void Comunity::onClientMessage(String str_trim) {

  String databox1[2];
  var->strManager->split(databox1, str_trim, SYMBOL_L1, 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  var->strManager->split(databox2, commands, SYMBOL_L2, 2);
  String command = databox2[0];
  String value = databox2[1];

  Serial.println("-------------------------------------");
  Serial.println("header = " + header);
  Serial.println("command = " + command);
  Serial.println("value = " + value);

  for (int i = 0; i < COMMAND_SIZE; i++) {
    if (header == H_GET || header == H_SET) {
      if(command == COMMAND[i]){
        H_GetSetProcess(header, COMMAND[i], value, i);
        break;
      }
    } else if (header == H_MESSAGE) {
      H_MessageProcess(command, value);
      break;
    }
  }
}


void Comunity::H_GetSetProcess(String header_arg, String command_arg, String value, int index) {
  String format_header = (header_arg == H_GET) ? H_SET : H_GET;
  String format = format_header + String(SYMBOL_L1) + command_arg + String(SYMBOL_L2);
  Serial.println("index = " + String(index));

  switch (index) {
    
    
    
    case 0://INPUT_PH
      if (header_arg == H_GET) {
        format += var->input_ph;
        ServerPH::send(format);
      } else if (header_arg == H_SET) {
        recvInputPH(value);
        sendOutputText("Ok,Server set ph value " + String(value.toFloat()) + " success");
      }
      break;
    
    
    
    case 1://MIXTANK_PH
      break;
    
    
    
    case 2://USETANK_PH
      break;
    
    
    
    
    case 3://TIME_AUTO_WORK
      if (header_arg == H_GET) {
        format += encodeTimeAutoWork();
        ServerPH::send(format);
      } else if (header_arg == H_SET) {
        recvTimerAutoWork(value);
        sendOutputText("Ok,Server set timeautowork success");
      }
      break;
    
    
    
    case 4://RTC_TIME
      if (header_arg == H_GET) {
        format += encodeTimeBoard();
        ServerPH::send(format);
      } else if (header_arg == H_SET) {
        recvTimeBoard(value);
        sendOutputText("Ok,Server set rtctime success");
      }
      break;
    
    
    
    
    case 5://OUTPUT
      break;
    
    
    
    
    case 6://TOGGLE_RELAY
      recvToggleRelay(value);
      sendOutputText("Ok,Server toggle relay " + String(value.toFloat()) + " success");
      break;

    
    
    
    case 7://FILE_DIR
      if(header_arg == H_GET){
        format += encodeFileDir(value);
        ServerPH::send(format);
      }else{

      }
      break;
    case 8://DEL_FILE_DIR
      if(header_arg == H_GET){

      }else{

      }
      break;
    case 9://CMVM
      if(header_arg == H_GET){
        ar_com->getValueAll();
        
      }else{

      }
      break;

  }
}

void Comunity::H_MessageProcess(String command_arg, String value) {
  String data = String(H_MESSAGE) + String(SYMBOL_L1) + command_arg + String(SYMBOL_L2) + value;
  Serial.println("Comunity : " + data);
  ServerPH::send(data);
}



void Comunity::sendOutputText(String output_text) {
  ServerPH::send("SET:OUTPUT=" + output_text);
}

void Comunity::sendTimeBoard() {
  ServerPH::send("SET:RTC_TIME=" + encodeTimeBoard());
}

void Comunity::sendOther(String data_str) {
  ServerPH::send(data_str);
}

void Comunity::sendMixTankPH(){
  ServerPH::send("SET:MIXTANK_PH=" + String(var->mixTank_pH));
}






void Comunity::recvInputPH(String value) {
  var->input_ph = value.toFloat();
}

void Comunity::recvTimeBoard(String queryStringFromClient) {
  byte numsize = 7;
  String* item = new String[numsize];
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

void Comunity::recvTimerAutoWork(String value) {
  String datalayer1[4];
  var->strManager->split(datalayer1, value, "|", 4);

  for (int i = 0; i < 4; i++) {
    String datalayer2[5];
    var->strManager->split(datalayer2, datalayer1[i], ",", 5);

    var->timerautowork[i].setHour(datalayer2[0].toInt());
    var->timerautowork[i].setMinute(datalayer2[1].toInt());
    var->timerautowork[i].setSecond(datalayer2[2].toInt());
    var->timerautowork[i].setStatus((datalayer2[3] == "true") ? true : false);
    var->timerautowork[i].setPH(datalayer2[4].toFloat());
    var->timerautowork[i].setDelete((datalayer2[0].toInt() == -1) ? true : false);
  }
}

void Comunity::recvToggleRelay(String value){
  hardwareIO->relay->toggle(value.toInt());
}





String Comunity::encodeTimeBoard() {
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

String Comunity::encodeTimeAutoWork() {

  String format = "";
  for (int i = 0; i < 4; i++) {
    format += String(var->timerautowork[i].getHour()) + ",";
    format += String(var->timerautowork[i].getMinute()) + ",";
    format += String(var->timerautowork[i].getSecond()) + ",";
    format += (var->timerautowork[i].getStatus()) ? "true," : "false,";
    format += String(var->timerautowork[i].getPH()) + ",";
    format += (var->timerautowork[i].getDelete()) ? "true" : "false";
    if (i != 3) format += "#";
  }
  Serial.println(format);
  return format;
}

String Comunity::encodeFileDir(String path){
  return hardwareIO->sdcard->listDir(path.c_str(),0);
  
}

