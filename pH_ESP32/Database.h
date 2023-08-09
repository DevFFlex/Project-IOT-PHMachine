#include <EEPROM.h>

#define DB_ADDR_TIMEAUTOWORK 0


class Database : public System{
public:
  Database()
  {
    EEPROM.begin(512);
  }

  void setup() override{
    
  }

  void loop() override{

  }

  void writeTimeAutoWork(TimerAutoWork *timerautowork);
  String readTimeAutoWork(TimerAutoWork *timerAutoWork);

};



void Database::writeTimeAutoWork(TimerAutoWork *timerautowork){
  
  Serial.println("database write");

  for(int i = 0;i<4;i++){
    int hour = timerautowork[i].getHour();
    int minute = timerautowork[i].getMinute();
    bool active = timerautowork[i].getStatus();
    float pH = timerautowork[i].getPH();
    bool empty = timerautowork[i].getDelete();

    int _1 = (hour >= 0 && hour <= 23) ? hour : 255;
    int _2 = (minute >= 0 && minute <= 59) ? minute : 255;
    int _3 = (active) ? active : 0;
    int _4 = (pH >= 0 && pH <= 14) ? pH*10 : 255;
    int _5 = (empty) ? empty : 0;

    Serial.println(String(_1) + "  " + String(_2) + "  " + String(_3) +  "  " + String(_4) + "  " + String(_5));

    EEPROM.write(i * 5 + 1,_1);
    EEPROM.write(i * 5 + 2,_2);
    EEPROM.write(i * 5 + 3,_3);
    EEPROM.write(i * 5 + 4,_4);
    EEPROM.write(i * 5 + 5,_5);
  }

  EEPROM.commit();
}

String Database::readTimeAutoWork(TimerAutoWork *timerAutoWork = NULL){
  String data = "";
  for(int i = 0;i<4;i++){
    int _1 = EEPROM.read(i * 5 + 1);
    int _2 = EEPROM.read(i * 5 + 2);
    int _3 = EEPROM.read(i * 5 + 3);
    int _4 = EEPROM.read(i * 5 + 4);
    int _5 = EEPROM.read(i * 5 + 5);

    timerAutoWork[i].setHour(_1);
    timerAutoWork[i].setMinute(_2);
    timerAutoWork[i].setStatus(_3);
    timerAutoWork[i].setPH(_4 / 10);
    timerAutoWork[i].setDelete(_5);


    data += String(_1) + " ";
    data += String(_2) + " ";
    data += String(_3) + " ";
    data += String(_4) + " ";
    data += String(_5) + "\n";
  }

  return data;
}

