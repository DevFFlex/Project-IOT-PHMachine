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

  void setPage(byte page);
  byte getPage();
};

void LcdOutput::setup() {
  lcd1.begin();
  lcd1.backlight();
  lcd1.home();
  // lcd1.print("LCD " + String(HSize1) + "x" + String(VSize1));
  // lcd1.setCursor(0, 1);
  // lcd1.print("PH Monitor");


  // delay(1000);
  // lcd1.clear();
}

void LcdOutput::loop() {
}

void LcdOutput::setPage(byte pageIn) {
  page = pageIn;
}

byte LcdOutput::getPage() {
  return page;
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
