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

  void writeTimeAutoWork(TimerAutoWork *timerautowork){
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
  String readTimeAutoWork(TimerAutoWork *timerAutoWork = NULL){
    String data = "";
    for(int i = 0;i<4;i++){
      int _1 = EEPROM.read(i * 5 + 1);
      int _2 = EEPROM.read(i * 5 + 2);
      int _3 = EEPROM.read(i * 5 + 3);
      int _4 = EEPROM.read(i * 5 + 4);
      int _5 = EEPROM.read(i * 5 + 5);

      if(timerAutoWork != NULL){
        timerAutoWork[i].setHour(_1);
        timerAutoWork[i].setMinute(_2);
        timerAutoWork[i].setStatus(_3);
        timerAutoWork[i].setPH(_4 / 10);
        timerAutoWork[i].setDelete(_5);
      }
      


      data += String(_1) + " ";
      data += String(_2) + " ";
      data += String(_3) + " ";
      data += String(_4) + " ";
      data += String(_5) + "\n";
    }

    return data;
  }

  void writeWifiData(String ssid = "",String pass = ""){
    int indexstart_ssid_addr = 50;
    int indexstart_pass_addr = indexstart_ssid_addr + 30;
    
    if(ssid != ""){
      for(int i = 0;i<30;i++){
        if(i >= ssid.length()){
          break;
        }else{
          EEPROM.write(indexstart_ssid_addr + i,(char)ssid[i]);
        }
        Serial.print(String((int)ssid[i]) + " ");
      }
      Serial.println("");
    }




    if(pass != ""){
      for(int i = 0;i<30;i++){
        if(i >= pass.length()){
          EEPROM.write(indexstart_pass_addr + i,0);
        }else{
          EEPROM.write(indexstart_pass_addr + i,pass[i]);
        }
        Serial.print(String((int)pass[i]) + " ");
      }
      Serial.println("");
    }

    EEPROM.commit();
  }
  void readWifiData(String *ssidBuffer,String *passBuffer){
    int indexstart_ssid_addr = 50;
    int indexstart_pass_addr = indexstart_ssid_addr + 30;
    
    String ssid_str = "";
    for(int i = 0;i<30;i++){
      int char_int = EEPROM.read(indexstart_ssid_addr + i);
      
      ssid_str += String(char_int);
      Serial.print(String(char_int) + " ");    
    }
    Serial.println("");
    // ssid_str.trim();
    *ssidBuffer = ssid_str;

    String pass_str = "";
    for(int i = 0;i<30;i++){
      String key = String((char)EEPROM.read(indexstart_pass_addr + i));
      pass_str += key;
      Serial.print(key + " ");
    }
    // pass_str.trim();
    Serial.println("");
    *passBuffer = pass_str;
    

    Serial.println("ssid = " + ssid_str);
    Serial.println("pass = " + pass_str);


  }

};





