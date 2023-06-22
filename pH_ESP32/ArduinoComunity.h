#include "HardwareSerial.h"

#define RX2pin 16
#define TX2pin 17

// SoftwareSerial arduino(10, 11);

class ArduinoComunity
{
private:
  Variable *var;
  HardwareIO *hardwareIO;

  String data_buffer = "";
  String *item = new String[6];
  String *c_v = new String[2];

public:

  ArduinoComunity(Variable *varIn, HardwareIO *hardwareIOIn)
  {
    var = varIn;
    hardwareIO = hardwareIOIn;
  }

  void setup();
  void loop();
  void onRecv(String data_command, String data_value);
  void setValueAll(String clb,String m,String voltIn,String max_analog);
  void setValueAllDefault();

  void displayCMVM();
};

void ArduinoComunity::setup()
{
  Serial2.begin(57600, SERIAL_8N1, RX2pin, TX2pin);
}

void ArduinoComunity::loop()
{

  while (Serial2.available())
  {
    char c = (char)Serial2.read();
    data_buffer += String(c);
    delay(5);
    // Serial.print(String(c));
  }

  if (data_buffer != "")
  {
    data_buffer.trim();
    // Serial.println(""); 

    var->strManager->split(item, data_buffer, ",", 6);
  
    for (int i = 0;i < 6;i++){
      var->strManager->split(c_v,item[i],"=",2);
      onRecv(c_v[0], c_v[1]);
    }


    data_buffer = "";
  }
}

void ArduinoComunity::onRecv(String command, String value)
{
  if (command.indexOf("SE_PH") != -1)
  {
    hardwareIO->pHSensor->setPH(value.toFloat());
  }

  if(command.indexOf("SE_CLB") != -1){
    var->phCalibrateSet->calibrate = value.toFloat();
  }

  if(command.indexOf("SE_MP")!= -1){
    var->phCalibrateSet->m = value.toFloat();
  }

  if(command.indexOf("SE_VOLTIN")!= -1){
    var->phCalibrateSet->voltin = value.toFloat();
  }

  if(command.indexOf("SE_MAX_ANALOG")!= -1){
    var->phCalibrateSet->max_analog = value.toFloat();
  }

  if(command.indexOf("SE_ANALOG_AVG") != -1){
    var->phCalibrateSet->analogAvg = value.toFloat();
  }

}

void ArduinoComunity::setValueAllDefault(){
  Serial2.write("SDEFAULT");
}

void ArduinoComunity::setValueAll(String clb = "DE",String m = "DE",String voltIn = "DE",String max_analog = "DE"){
  String set_str = "";
  set_str += "S_CLB=" + clb + ",";
  set_str += "S_M=" + m + ",";
  set_str += "S_VI=" + voltIn + ",";
  set_str += "S_AMAX=" + max_analog + "\n";
  Serial2.write(set_str.c_str());
}
void ArduinoComunity::displayCMVM(){
  Serial.println("----------------------------------------------");
  Serial.println("Calibrate : " + String(var->phCalibrateSet->calibrate));
  Serial.println("M : " + String(var->phCalibrateSet->m));
  Serial.println("VoltIn : " + String(var->phCalibrateSet->voltin));
  Serial.println("Max Analog : " + String(var->phCalibrateSet->max_analog));
}