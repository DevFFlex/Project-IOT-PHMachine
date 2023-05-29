#include <LiquidCrystal_I2C.h>

#define ADDR1 0x27
#define HSize1 20
#define VSize1 4

// #define ADDR2 0x57
// #define HSize2 16
// #define VSize2 2


class LcdOutput {
private:
  LiquidCrystal_I2C lcd1;
  // LiquidCrystal_I2C lcd2;

  byte line1 = 0;
  byte line2 = 0;

  byte page = 1;


public:
  LcdOutput()
    : lcd1(ADDR1, HSize1, VSize1)
  // , lcd2(ADDR2, HSize2, VSize2)
  {
  }

  void setup();
  void loop();
  void printL(byte lcd, String text, byte l);

  void setPage(byte page);
};

void LcdOutput::setup() {
  lcd1.begin();
  lcd1.backlight();
  lcd1.home();
  lcd1.print("LCD " + String(HSize1) + "x" + String(VSize1));
  lcd1.setCursor(0, 1);
  lcd1.print("www.FFlex.com");


  // lcd2.begin();
  // lcd2.backlight();
  // lcd2.home();
  // lcd2.print("LCD " + String(HSize2) + "x" + String(VSize2));
  // lcd2.setCursor(0, 1);
  // lcd2.print("www.FFlex.com");

  delay(1000);
  lcd1.clear();
  // lcd2.clear();
}

void LcdOutput::loop() {
}

void LcdOutput::printL(byte page, String text, byte l) {

  lcd1.setCursor(0, l);
  for (int i = 0; i < HSize1; i++) lcd1.print(" ");
  lcd1.setCursor(0, l);
  lcd1.print(text);
}


void LcdOutput::setPage(byte pageIn) {
  page = pageIn;
  lcd1.clear();
}