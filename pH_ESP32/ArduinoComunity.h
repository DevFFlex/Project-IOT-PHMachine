#include "HardwareSerial.h"
// #include "HardwareSerial.h"
// #include <HardwareSerial.h>
// #include <SoftwareSerial.h>

#define RX2pin 16
#define TX2pin 17

// SoftwareSerial arduino(10, 11);


class ArduinoComunity {
private:
  HardwareIO *hardwareIO;
  StringManage *stringManage;
  String data_buffer = "";
  int test_count = 0;
  float ph_old = 0.0;
  float ph_last = 0.0;

public:
  String text = "NULL";

  ArduinoComunity(HardwareIO *hardwareIOIn, StringManage *stringManageIn) {
    hardwareIO = hardwareIOIn;
    stringManage = stringManageIn;
  }

  void setup();
  void loop();
};


void ArduinoComunity::setup() {
  Serial2.begin(57600, SERIAL_8N1, RX2pin, TX2pin);
}


void ArduinoComunity::loop() {

  while (Serial2.available()) {
    char d = (char)Serial2.read();
    data_buffer += String(d);
    delay(10);
  }


  if (data_buffer != "") {
    data_buffer.trim();
    
    String *item = new String[2];

    stringManage->split(item, data_buffer, "=", 2);
    hardwareIO->lcdOutput->printL("Serial = " + data_buffer, 2);

    ph_old = ph_last;
    ph_last = item[1].toFloat();

    Serial.println("ph_old = " + String(ph_old) + "    ph_last = " + String(ph_last));

    if (ph_old != 0 && ph_last != 0 && abs(ph_last - ph_old) < 1) hardwareIO->pHSensor->setPH(ph_last);

    data_buffer = "";
  }
}