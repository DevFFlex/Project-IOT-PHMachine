#include "Arduino.h"


#define DEBUG_KEY1 "debugWifiConnect"
#define DEBUG_KEY2 "debugClientComunity"
#define DEBUG_KEY3 "debugArduinoComunity"
#define DEBUG_KEY4 "debugCouldComunity"



class SerialInput : public System{
private:
    Variable *var;
    Comunity *comunity;

    String *split_buffer = new String[10];
    String command = "";
    String args[9] = {"","","","","","","","",""};

    void onInputAvailable();

public:
    SerialInput(Variable *varIn,Comunity *comIn)
    {
        var = varIn;
        comunity = comIn;
    }

    void setup() override {

    }

    void loop() override {
        onInputAvailable();
    }


};

void SerialInput::onInputAvailable()
{
    if (Serial.available() > 0)
    {
        String data = Serial.readString();
        data.trim();

        String inputString = data;
        char targetChar = ' ';
        int charCount = 0;

        for (int i = 0; i < inputString.length(); i++) if (inputString.charAt(i) == targetChar)charCount++;

        if(charCount > 10)return;

        if(charCount == 0)command = data;
        else{
            splitString(split_buffer,data," ",charCount + 1);
            command = split_buffer[0];
            for(int i = 0;i<charCount ;i++)args[i] = split_buffer[i + 1];
        }
        Serial.println("charCount = " + String(charCount));
        Serial.println("command = " + command);
        for(int i = 0;i<9;i++) if(args[i] != "")Serial.println("args["+String(i)+"] = " + String(args[i]));
       

        if(command == "system"){
          if(args[0] == "restart"){
            ESP.restart();
          }else if(args[0] == "scanI2C"){
            var->i2cScan.scan();
          }
        }else if(command == "relay"){
          if(args[0] == "active"){
            var->hardwareIO->relay->active();
          }else if(args[0] == "deactive"){
            var->hardwareIO->relay->deactive();
          }else if(args[0] == "on"){
            var->hardwareIO->relay->on(args[1].toInt());
          }else if(args[0] == "off"){
            var->hardwareIO->relay->off(args[1].toInt());
          }else if(args[0] == "toggle"){
            var->hardwareIO->relay->toggle(args[1].toInt());
          }
        }else if(command == "buzzer"){
          var->hardwareIO->buzzer->on(data.toInt());
        }else if(command == "sdcard"){
          if(args[0] == "rem"){
            var->hardwareIO->sdcard->removeDir(args[1].c_str());
          }else if(args[0] == "dir"){
            var->hardwareIO->sdcard->listDir(args[1].c_str(),0);
          }else if(args[0] == "mkdir"){
            var->hardwareIO->sdcard->createDir(args[1].c_str());
          }else if(args[0] == "del"){
            var->hardwareIO->sdcard->deleteFile(args[1].c_str());
          }
        }else if(command == "db"){
          if(args[0] == "saveTimerSet"){
            var->db->writeTimeAutoWork(var->timerautowork);
          }else if(args[0] == "readTimerSet"){
            Serial.println(var->db->readTimeAutoWork(var->timerautowork));
          }else if(args[0] == "showTimerSet_In_ESP32"){
            Serial.println("----------------- TimerSet In ESP32 -------------------");
            String out = var->timerautowork[0].toString() + "\n";
            out += var->timerautowork[1].toString() + "\n";
            out += var->timerautowork[2].toString() + "\n";
            out += var->timerautowork[3].toString() + "\n";
            Serial.println(out);
            Serial.println("-------------------------------------------------------");
          }else if(args[0] == "showTimerSet_In_ESP32ROM"){
            Serial.println("----------------- TimerSet In ESP32ROM -------------------");
            Serial.println(var->db->readTimeAutoWork(NULL));
            Serial.println("----------------------------------------------------------");
          }
        }else if(command == "wifipublic"){
          if(args[0] == "connect"){
            comunity->wifi_controll->connectWiFi();
          }else if(args[0] == "setSSID"){
            var->db->writeWifiData(args[0],"");
          }else if(args[0] == "setPASS"){
            var->db->writeWifiData("",args[0]);
          }else if(args[0] == "getData"){
            String ssid_read,pass_read;
            var->db->readWifiData(&ssid_read,&pass_read);
            Serial.println("ssid:" + ssid_read + "     pass:" + pass_read);
          }
        }else if(command == "variable"){
          if(args[0] == "setFSW1"){
            var->fsw.mixTank_Up = (args[1] == "1") ? true : false;
          }else if(args[0] == "setFSW2"){
            var->fsw.mixTank_Down = (args[1] == "1") ? true : false;
          }else if(args[0] == "setInputPH"){
            var->input_ph = args[1].toFloat();
          }else if(args[0] == "setMixtankPH"){
            var->mixTank_pH = args[1].toFloat();
          }else if(args[0] == "show"){
            var->showVar();
          }
          var->showVar();
        }else if(command == "debug"){
          if(args[0] == "wificonnect"){
            var->datadebug.debug_wifiConnection = !var->datadebug.debug_wifiConnection;
          }else if(args[0] == "clientcm"){
            var->datadebug.debug_client_comunity = !var->datadebug.debug_client_comunity;
          }else if(args[0] == "arduinocm"){
           var->datadebug.debug_arduino_comunity = !var->datadebug.debug_arduino_comunity;
          }else if(args[0] == "cloudcm"){
            var->datadebug.debug_could_comunity = !var->datadebug.debug_could_comunity;
          }else if(args[0] == "show"){
            var->showVar();
          }
          
        }

    }
}