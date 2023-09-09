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
    if (Serial.available() > 0 || var->serial_buffer!= "")
    {
    
        String data = "";

        if(var->serial_buffer != ""){
          data = var->serial_buffer;
          var->serial_buffer = "";
          
        }else{
          data = Serial.readString();
        }

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
        // Serial.println("charCount = " + String(charCount));
        // Serial.println("command = " + command);
        // for(int i = 0;i<9;i++) if(args[i] != "")Serial.println("args["+String(i)+"] = " + String(args[i]));
       

        if(command == "system"){
          if(args[0] == "restart"){
            ESP.restart();
          }else if(args[0] == "scanI2C"){
            var->i2cScan.scan();
          }else if(args[0] == "startAdjust"){
            if(args[1] != "")var->workVar.startAdjustPH(args[1].toFloat(),(args[2] == "") ? 1 : args[2].toInt());
          }else if(args[0] == "stopAdjust"){
            var->workVar.stopAdjustPH();
          }
        }else if(command == "relay"){
          if(args[0] == "active"){
            var->hardwareIO->relay->active();
          }else if(args[0] == "deactive"){
            var->hardwareIO->relay->deactive();
          }else if(args[0] == "on"){
            if(args[2] == "")var->hardwareIO->relay->on(args[1].toInt());
            else var->hardwareIO->relay->on(args[1].toInt(),args[2].toInt());
          }else if(args[0] == "off"){
            if(args[2] == "")var->hardwareIO->relay->off(args[1].toInt());
            else var->hardwareIO->relay->off(args[1].toInt(),args[2].toInt());
          }else if(args[0] == "toggle"){
            var->hardwareIO->relay->toggle(args[1].toInt());
          }else if(args[0] == "show"){
            for(int i = 0;i<6;i++)Serial.print(String(var->hardwareIO->relay->status[i]) + " ");
            Serial.println("");
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
          if(args[0] == "show_eeprom"){
            var->db->showEEPROM();
          }else if(args[0] == "clear_eeprom"){
            var->db->clearEEPROM();
          }
        }else if(command == "internet"){
          if(args[0] == "connect"){
            comunity->wifi_controll->connectWiFi();
            Serial.println("connect public wifi....");
          }else if(args[0] == "setInternetSSID"){
            var->wifipublic.SSID = args[1];
            var->db->writeInternetSSID(args[1]);
          }else if(args[0] == "setInternetPASS"){
            var->wifipublic.PASS = args[1];
            var->db->writeInternetPASS(args[1]);
          }else if(args[0] == "getSSID"){
            var->wifipublic.SSID = var->db->readInternetSSID();
          }else if(args[0] == "getPASS"){
            var->wifipublic.PASS = var->db->readInternetPASS();
          }else if(args[0] == "isconnected"){
            String status_str = (var->wifipublic.isconnected()) ? "connected" : "no connected";
            Serial.println("wifipubblic : " + status_str);
          }else if(args[0] == "show"){
            var->wifipublic.show();
          }
        }else if(command == "var"){
          if(args[0] == "setFSW"){
            if(args[1] == "1"){
              var->fsw.mixTank_Up = (args[2] == "1") ? true : false;
            }
            else if(args[1] == "2"){
              var->fsw.mixTank_Down = (args[2] == "1") ? true : false;
            }
          }else if(args[0] == "setINPUTPH"){
            var->workVar.adjustCurrentpH = args[1].toFloat();
          }else if(args[0] == "setMIXTANKPH"){
            var->mixTank_pH = args[1].toFloat();
          }else if(args[0] == "setPHSPACERATE"){
            var->workVar.pH_space_rate = args[1].toFloat();
            var->db->writePSR(args[1]);
          }else if(args[0] == "setLIMITE_USE_BASE"){
            var->workVar.limite_use_base = args[1].toInt();
            var->db->writeLimiteBase(args[1]);
          }else if(args[0] == "setLIMITE_USE_ACID"){
            var->workVar.limite_use_acid = args[1].toInt();
            var->db->writeLimiteAcid(args[1]);
          }else if(args[0] == "setWAIT_STR_PUMP"){
            var->workVar.wait_stirring_pump = args[1].toInt();
            var->db->writeWaitSTRPump(args[1]);
          }else if(args[0] == "setWAIT_PH_STABILIZE"){
            var->workVar.wait_pH_stabilize = args[1].toInt();
            var->db->writeWaitPHStabilize(args[1]);
          }else if(args[0] == "setWAIT_BASEUSETIME"){
            var->workVar.wait_baseUseTime = args[1].toInt();
            var->db->writeWaitBaseUseTime(args[1]);
          }else if(args[0] == "setWAIT_ACIDUSETIME"){
            var->workVar.wait_acidUseTime = args[1].toInt();
            var->db->writeWaitAcidUseTime(args[1]);
          }else if(args[0] == "setWorkTime"){
            int index = args[1].toInt();
            int hour = args[2].toInt();
            int minute = args[3].toInt();
            float pH = args[4].toFloat();
            int T = args[5].toInt();
            bool active_status = (args[6] == "1") ? true:false;
            bool delete_status = (args[7] == "1") ? true:false;
            
            var->workTimer[index].HOUR = hour;
            var->workTimer[index].MINUTE = minute;
            var->workTimer[index].PH = pH;
            var->workTimer[index].T = T;
            var->workTimer[index].ACTIVE_STATUS = active_status;
            var->workTimer[index].DELETE_STATUS = delete_status;
            var->db->writeWorkTimer(index,hour,minute,pH,T,active_status,delete_status);
          }else if(args[0] == "getWorkTime"){
            Serial.println(var->db->readWorkTimer(args[1].toInt()));
          }
          else if(args[0] == "show"){
            var->showVar();
            return;
          }
          var->showVar();
        }else if(command == "cloud"){
          if(args[0] == "testSend"){
            //https://script.google.com/macros/s/AKfycbw3nnrGL_atUT3M80KyVXajTiDQbgJd2CY9JQu-g6H2cFefiAVmI2thYtT3MS-qjZD4/exec?header=pHAll&p1=-1&p2=-1&p3=-1&p4=-1&p5=-1&p6=-1&p7=-1&p8=-1&p9=-1&p10=TEST
            https://script.google.com/macros/s/AKfycbwuIkUzj5qahBBlUJ96eSqGnCx4pTXWiRcttWDo2Xs/dev?header=pHAll&p1=-1&p2=-1&p3=-1&p4=-1&p5=-1&p6=-1&p7=-1&p8=-1&p9=-1&p10=TEST
            // header="pHAll"&p1="1"&p2="2"&p3="3"&p4="4"&p5="5"&p6="6"
            comunity->couldComunity->sendPH_All("-1","-1,-1","-1","-1","-1","-1","-1","-1","-1","TEST");
          }
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