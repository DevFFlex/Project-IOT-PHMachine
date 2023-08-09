#include "HardwareSerial.h"

#define RX2pin 16
#define TX2pin 17
#define BAUD_RATE 57600

class ArduinoComunity : public System{
private:
  String data_buffer = "";
  String *item = new String[6];
  String *c_v = new String[2];

  bool displayDataTranfer = false;

public:

  ArduinoComunity()
  {
  }

  void setup() override {
    Serial2.begin(BAUD_RATE, SERIAL_8N1, RX2pin, TX2pin);
  }

  void loop() override {
    while (Serial2.available())
    {
      char c = (char)Serial2.read();
      data_buffer += String(c);
      delay(5);
    }

    if (data_buffer != "")
    {
      data_buffer.trim();
      if(displayDataTranfer)Serial.println("Data From Arduino ---> " + data_buffer ); 


      // splitString(item, data_buffer, ",", 6);
      // var->mixTank_pH = item[0].toFloat();
      // var->fsw_mixTank_Up = (item[3] == "1") ? true : false;
      // var->fsw_mixtank_Down = (item[4] == "1") ? true : false;
      // var->fsw_waterTank_Down = (item[5] == "1") ? true : false;


      data_buffer = "";
    }
  }

  void setDisplayDataTranfer(bool status);
  bool getDisplayDataTranfer();
};



void ArduinoComunity::setDisplayDataTranfer(bool status){
  displayDataTranfer = status;
}

bool ArduinoComunity::getDisplayDataTranfer(){
  return displayDataTranfer;
}
