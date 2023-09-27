#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>

SoftwareSerial esp32(10, 11);

#define PH_PIN A1
float voltage, phValue, temperature = 25;
DFRobot_PH ph;

#define FSW_R 5
#define FSW_G 6

void setup() {
  Serial.begin(115200);
  esp32.begin(57600);
  ph.begin();
}

void loop() {
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 500U) {
    timepoint = millis();
    onTriggerTimer();
  }

  ph.calibration(voltage, temperature);
}


void onTriggerTimer(){
  voltage = analogRead(PH_PIN) / 1024.0 * 5000;
  phValue = ph.readPH(voltage, temperature);
  Serial.print("temperature:");
  Serial.print(temperature, 1);
  Serial.print("^C  pH:");
  Serial.println(phValue, 2);

  String data_str = String(phValue) + "," + String(voltage) + "," + String(temperature) + "," + String(digitalRead(FSW_R)) + "," + String(digitalRead(FSW_G));
  esp32.write(data_str.c_str());
}