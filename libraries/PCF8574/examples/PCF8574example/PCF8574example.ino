#include <PCF8574.h>

PCF8574 pcf8574(0x20);          // Set I2C Address

void setup() {
  Serial.begin(9600);
  pcf8574.pinMode(P0, OUTPUT);
  pcf8574.pinMode(P1, OUTPUT);
  pcf8574.pinMode(P2, OUTPUT);
  pcf8574.pinMode(P3, OUTPUT);
  pcf8574.pinMode(P4, OUTPUT);
  pcf8574.pinMode(P5, OUTPUT);
  pcf8574.pinMode(P6, OUTPUT);
  pcf8574.pinMode(P7, OUTPUT);
  pcf8574.begin();
  
}

void loop() {

  pcf8574.digitalWrite(P0, HIGH);
  pcf8574.digitalWrite(P1, HIGH);
  pcf8574.digitalWrite(P2, HIGH);
  pcf8574.digitalWrite(P3, HIGH);
  pcf8574.digitalWrite(P4, HIGH);
  pcf8574.digitalWrite(P5, HIGH);
  pcf8574.digitalWrite(P6, HIGH);
  pcf8574.digitalWrite(P7, HIGH);
  delay(1000);

  pcf8574.digitalWrite(P0, LOW);
  delay(1000);
  pcf8574.digitalWrite(P0, HIGH);
  pcf8574.digitalWrite(P1, LOW);
  delay(1000);
  pcf8574.digitalWrite(P1, HIGH);
  pcf8574.digitalWrite(P2, LOW);
  delay(1000);
  pcf8574.digitalWrite(P2, HIGH);
  pcf8574.digitalWrite(P3, LOW);
  delay(1000);
  pcf8574.digitalWrite(P3, HIGH);
  pcf8574.digitalWrite(P4, LOW);
  delay(1000);
  pcf8574.digitalWrite(P4, HIGH);
  pcf8574.digitalWrite(P5, LOW);
  delay(1000);
  pcf8574.digitalWrite(P5, HIGH);
  pcf8574.digitalWrite(P6, LOW);
  delay(1000);
  pcf8574.digitalWrite(P6, HIGH);
  pcf8574.digitalWrite(P7, LOW);
  delay(1000);
}
