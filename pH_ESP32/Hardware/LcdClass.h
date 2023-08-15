#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

#define ADDR1 0x27
#define HSize1 20
#define VSize1 4




class LcdOutput {
private:
  LiquidCrystal_I2C lcd1;
  // LiquidCrystal_I2C lcd2;

  byte line1 = 0;
  byte line2 = 0;

  
  byte page = 0;

public:
  LcdOutput()
    : lcd1(ADDR1, HSize1, VSize1) {

  }

  void setup();
  void loop();
  void printL(String text, byte l);
  void clear();

};

void LcdOutput::setup() {
  lcd1.begin();
  lcd1.backlight();
  lcd1.home();
}

void LcdOutput::loop() {
}


void LcdOutput::printL(String text, byte l) {
  lcd1.setCursor(0, l);
  for (int i = 0; i < HSize1; i++) lcd1.print(" ");
  lcd1.setCursor(0, l);
  lcd1.print(text);
}


void LcdOutput::clear(){
  lcd1.clear();
}
