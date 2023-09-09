#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <TimerOne.h>
#include <SoftwareSerial.h>

SoftwareSerial esp32(10, 11);

#define PH_PIN A1
float voltage, phValue, temperature = 25;
DFRobot_PH ph;

int fsw_r = 5;
int fsw_g = 6;
int fsw_b = 7;

void setup() {
  Serial.begin(115200);
  ph.begin();
  esp32.begin(57600);
}

void loop() {
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 2000U) {
    timepoint = millis();
    //temperature = readTemperature();
    voltage = analogRead(PH_PIN) / 1024.0 * 5000;
    phValue = ph.readPH(voltage, temperature);
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.print("^C  pH:");
    Serial.println(phValue, 2);
    Serial.println("Analog pH = " + String(analogRead(PH_PIN)));

    String data_str = String(phValue) + "," + String(voltage) + "," + String(temperature) + "," + String(digitalRead(fsw_r)) + "," + String(digitalRead(fsw_g)) + "," + String(digitalRead(fsw_b));
    esp32.write(data_str.c_str());
  }

  ph.calibration(voltage, temperature);
}