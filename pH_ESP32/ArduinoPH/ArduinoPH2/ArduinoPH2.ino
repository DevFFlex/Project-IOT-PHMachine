#include <TimerOne.h>
#include <SoftwareSerial.h>


#define PHSensorPin  A0
int AnalogValue;
double VoltageInput;
float PHvalue;


SoftwareSerial esp32(10, 11);

void onreadph()
{
  AnalogValue = analogRead(PHSensorPin);
  Serial.print("Analog Value Input = ");
  Serial.println(AnalogValue);
  VoltageInput = 5/1024.0 * AnalogValue;
  Serial.print("Volatge = ");
  Serial.println(VoltageInput, 3);
  PHvalue = 7 + ((2.5 - VoltageInput) / 0.18);
  Serial.print("PH Value : " );
  Serial.println(PHvalue, 3);
  Serial.println(" ");
  delay(3000);               // pH = m * Volt + Calibration

  String data_str = String(PHvalue);

  Serial.println(data_str);

  esp32.write(data_str.c_str());

  
}

void setup()
{
  Serial.begin(9600);
  Timer1.initialize(1000000 * 1); // 1 วินาที = 1000000 ไมครอน
  Timer1.attachInterrupt(onreadph);
  esp32.begin(57600);
}

void loop()
{
  
}
