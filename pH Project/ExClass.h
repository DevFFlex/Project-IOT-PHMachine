#include <Wire.h>

void split(String *databox,String text, String symbol,int size) {

  char charArrayText[text.length() + 1];
  text.toCharArray(charArrayText, text.length() + 1);

  char charArraySymbol[symbol.length() + 1];
  symbol.toCharArray(charArraySymbol, symbol.length() + 1);

  char* symbol1 = charArraySymbol;

  char* part = strtok(charArrayText, symbol1);
  for (int i = 0;i<size;i++) {
    databox[i] = part;

    part = strtok(NULL, symbol1);
  }
}


String intToText(int integer,int zeroforward){
  String zero = "";
  if(zeroforward <= 0 || integer < 0 || integer > 9)return String(integer);

  for (int i = 0;i<zeroforward - 1;i++)zero += "0";
  return zero + String(integer);
}

class LED {
private:
  int pin;

public:

  LED(int pinIn) {
    pin = pinIn;
  }

  void on() {
    digitalWrite(pin, HIGH);
  }


  void off() {
    digitalWrite(pin, LOW);
  }
};



class ScanI2C {

public:
  void setup() {
    Wire.begin();
  }


  void loop() {
    byte error, address;
    int nDevices;
    Serial.println("Scanning..."); /*ESP32 starts scanning available I2C devices*/
    nDevices = 0;
    for (address = 1; address < 127; address++) { /*for loop to check number of devices on 127 address*/
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0) {                                 /*if I2C device found*/
        Serial.print("I2C device found at address 0x"); /*print this line if I2C device found*/
        if (address < 16) {
          Serial.print("0");
        }
        Serial.println(address, HEX); /*prints the HEX value of I2C address*/
        nDevices++;
      } else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if (address < 16) {
          Serial.print("0");
        }
        Serial.println(address, HEX);
      }
    }
    if (nDevices == 0) {
      Serial.println("No I2C devices found\n"); /*If no I2C device attached print this message*/
    } else {
      Serial.println("done\n");
    }
    delay(5000); /*Delay given for checking I2C bus every 5 sec*/
  }
};