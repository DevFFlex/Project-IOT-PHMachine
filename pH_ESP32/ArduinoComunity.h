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
  int test_count = 0;
  float ph_old = 0.0;
  float ph_last = 0.0;

  float var_calibrate = 0;
  float var_m = 0;
  float var_voltin = 0;
  float var_max_analog = 0;

public:
  String text = "NULL";

  ArduinoComunity(Variable *varIn, HardwareIO *hardwareIOIn)
  {
    var = varIn;
    hardwareIO = hardwareIOIn;
  }

  void setup();
  void loop();
  void displayCMVM();

  void onRecv(String data_command, String data_value);

  void getValueCalibrate();
  void getValueM();
  void getValueVoltIn();
  void getValueMaxAnalog();
  void getValueAll();
};

void ArduinoComunity::setup()
{
  Serial2.begin(57600, SERIAL_8N1, RX2pin, TX2pin);
}

void ArduinoComunity::loop()
{

  while (Serial2.available())
  {
    char d = (char)Serial2.read();
    data_buffer += String(d);
    delay(10);
  }

  if (data_buffer != "")
  {
    data_buffer.trim();

    String *item = new String[2];

    var->strManager->split(item, data_buffer, "=", 2);

    onRecv(item[0], item[1]);

    data_buffer = "";
  }
}

void ArduinoComunity::onRecv(String command, String value)
{
  if (command.indexOf("SE_PH") != -1)
  {
    ph_old = ph_last;
    ph_last = value.toFloat();

    hardwareIO->pHSensor->setPH(ph_last);
  }

  if(command.indexOf("S_CLB") != -1){
    var_calibrate = value.toFloat();
  }

  if(command.indexOf("S_M")!= -1){
    var_m = value.toFloat();
  }

  if(command.indexOf("S_VI")!= -1){
    var_voltin = value.toFloat();
  }

  if(command.indexOf("S_AMAX")!= -1){
    var_max_analog = value.toFloat();
  }
}

void ArduinoComunity::getValueCalibrate()
{
  Serial2.write("G_CLB");
}

void ArduinoComunity::getValueM()
{
  Serial2.write("G_M");
}

void ArduinoComunity::getValueVoltIn()
{
  Serial2.write("G_VI");
}

void ArduinoComunity::getValueMaxAnalog()
{
  Serial2.write("G_AMAX");
}


void ArduinoComunity::getValueAll(){
  getValueCalibrate();
  getValueM();
  getValueVoltIn();
  getValueMaxAnalog();
}

void ArduinoComunity::displayCMVM(){
  Serial.println("----------------------------------------------");
  Serial.println("Calibrate : " + String(var_calibrate));
  Serial.println("M : " + String(var_m));
  Serial.println("VoltIn : " + String(var_voltin));
  Serial.println("Max Analog : " + String(var_max_analog));
}