#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68


class RTC {
private:
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

  byte decToBcd(byte val);
  byte bcdToDec(byte val);

  void updateTime();
  void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);
  // void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);

public:

  RTC() {}

  void setup();
  void loop();

  byte getHour();
  byte getMinute();
  byte getSecond();
  byte getDayOfWeek();
  byte getDayOfMouth();
  byte getMonth();
  byte getYear();
  
  void setTime(byte hour, byte minute, byte second, byte dayOfWeek, byte dayOfMonth, byte month, byte year);
  String getTimeToString();
  void displayTime();
};

//-------------------------------------------------------------------------------------------

void RTC::setup() {
  Wire.begin();

  // DS3231 seconds, minutes, hours, day, date, month, year


  byte set_hour = 16;
  byte set_minute = 18;
  byte set_second = 0;
  byte set_dayofweek = 7;
  byte set_date = 5;
  byte set_month = 5;
  byte set_year = 23;
  // setDS3231time(set_second, set_minute, set_hour, set_dayofweek, set_date, set_month, set_year);
}


void RTC::loop() {
}

void RTC::updateTime() {
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
}

byte RTC::decToBcd(byte val) {
  return ((val / 10 * 16) + (val % 10));
}

byte RTC::bcdToDec(byte val) {
  return ((val / 16 * 10) + (val % 16));
}


void RTC::readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year) {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}


void RTC::setTime(byte hour,byte minute,byte second, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  // sets time and date data to DS3231
  Serial.println("Wire.beginTransmission(DS3231_I2C_ADDRESS);");

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);                     // set next input to start at the seconds register
  Wire.write(decToBcd(second));      // set seconds
  Wire.write(decToBcd(minute));      // set minutes
  Wire.write(decToBcd(hour));        // set hours
  Wire.write(decToBcd(dayOfWeek));   // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth));  // set date (1 to 31)
  Wire.write(decToBcd(month));       // set month
  Wire.write(decToBcd(year));        // set year (0 to 99)
  Wire.endTransmission();

  Serial.println("Wire.endTransmission()");
}

String RTC::getTimeToString() {
  String timeformat_str = "";

  updateTime();

  String hour_str = (hour < 10) ? "0" + String(hour) : String(hour);
  String minute_str = (minute < 10) ? "0" + String(minute) : String(minute);
  String second_str = (second < 10) ? "0" + String(second) : String(second);

  String dayofweek_str = "";
  switch (dayOfWeek) {
    case 1:
      dayofweek_str = "Sunday";
      break;
    case 2:
      dayofweek_str = "Monday";
      break;
    case 3:
      dayofweek_str = "Tuesday";
      break;
    case 4:
      dayofweek_str = "Wednesday";
      break;
    case 5:
      dayofweek_str = "Thursday";
      break;
    case 6:
      dayofweek_str = "Friday";
      break;
    case 7:
      dayofweek_str = "Saturday";
      break;
  }

  String dayofmonth_str = (dayOfMonth < 10) ? "0" + String(dayOfMonth) : String(dayOfMonth);
  String month_str = (month < 10) ? "0" + String(month) : String(month);
  String year_str = String(year);


  timeformat_str += hour_str + ":" + minute_str + ":" + second_str + " ";
  timeformat_str += dayofmonth_str + "/" + month_str + "/" + year_str + " ";
  // timeformat_str += dayofweek_str + "";


  return timeformat_str;
}

void RTC::displayTime() {
  Serial.println(getTimeToString());
}


byte RTC::getHour() {
  updateTime();
  return hour;
}

byte RTC::getMinute() {
  updateTime();
  return minute;
}

byte RTC::getSecond() {
  updateTime();
  return second;
}

byte RTC::getDayOfWeek() {
  updateTime();
  return dayOfWeek;
}

byte RTC::getDayOfMouth() {
  updateTime();
  return dayOfMonth;
}

byte RTC::getMonth() {
  updateTime();
  return month;
}

byte RTC::getYear() {
  updateTime();
  return year;
}
