// #include "HardwareSerial.h"
// #include <HardwareSerial.h>

#define RX2pin 16
#define TX2pin 17


class ArduinoComunity{
  private:
  HardwareIO *hardwareIO;
  StringManage *stringManage;

  public:
  String text = "NULL";

  ArduinoComunity(HardwareIO *hardwareIOIn,StringManage *stringManageIn){
    hardwareIO = hardwareIOIn;
    stringManage = stringManageIn;
  }

  void setup();
  void loop();
};


void ArduinoComunity::setup(){
  Serial2.begin(9600, SERIAL_8N1, RX2pin, TX2pin);
}


void ArduinoComunity::loop(){

  if(Serial2.available()){
    String data =Serial2.readString();
    // Serial.println(data);

    String* item = new String[2];

    stringManage->split(item,data,"=",2);

    hardwareIO->lcdOutput->printL(1, "Serial = " + data, 2);
    hardwareIO->lcdOutput->printL(1, "item[0] = " + item[0], 1);

    // Serial.println(data_split[0]);
  }

  // Serial.println();

}