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



        // Version2

        /*String inputString = data;
        char targetChar = ' ';
        int charCount = 0;

        for (int i = 0; i < inputString.length(); i++) {
            if (inputString.charAt(i) == targetChar) {
            charCount++;
            }
        }

        String args[9];

        if(charCount > 10)return;

        String command = split_buffer[0];
        

        if(charCount == 0){
            args[0] = split_buffer[1];
        }else{
            for(int i = 1;i<charCount;i++){
                splitString(split_buffer,data," ",2);
                args[i] = split_buffer[i+1];
            }
        }

        

        

        if(command == "restart"){
            ESP.restart();
        }*/





        if (data != "")
        {
            if(data.indexOf("restart") != -1){
              ESP.restart();
            }



            if (data.indexOf("relay_active") != -1)
            {
                var->hardwareIO->relay->active();
            }
            else if (data.indexOf("relay_deactive") != -1)
            {
                var->hardwareIO->relay->deactive();
            }
            else if (data.indexOf("relay_on") != -1)
            {
                data.replace("relay on", "");
                var->hardwareIO->relay->on(0,data.toInt());

            }
            else if (data.indexOf("relay off") != -1)
            {
                data.replace("relay off", "");
                var->hardwareIO->relay->off(data.toInt());
            }
            else if (data.indexOf("relay toggle") != -1)
            {
                data.replace("relay toggle", "");
                var->hardwareIO->relay->toggle(data.toInt());
            }


            if (data.indexOf("buzzer") != -1)
            {
                data.replace("buzzer", "");
                // var->hardwareIO->buzzer->freq = data.toInt();
                var->hardwareIO->buzzer->on(data.toInt());
            }

        
            if (data.indexOf("sd rem ") != -1){
                data.replace("sd rem ","");
                var->hardwareIO->sdcard->removeDir(data.c_str());
            }

            else if (data.indexOf("sd dir ") != -1){
                data.replace("sd dir ","");
                String path = data;
                String data2 = data.substring(data.indexOf(" "),data.length());
                Serial.println("data2 = " + data2);
                var->hardwareIO->sdcard->listDir(data.c_str(),0);
            }

            else if (data.indexOf("sd mkdir ") != -1){
                data.replace("sd mkdir ","");
                var->hardwareIO->sdcard->createDir(data.c_str());
            }

            else if (data.indexOf("sd del ") != -1){
                data.replace("sd del ","");
                var->hardwareIO->sdcard->deleteFile(data.c_str());
            }

            if (data.indexOf("saveTimerSet") != -1){
                data.replace("saveTAW","");
                var->db->writeTimeAutoWork(var->timerautowork);
            }

            else if (data.indexOf("readTimerSet") != -1){
                data.replace("readTAW","");
                Serial.println(var->db->readTimeAutoWork(var->timerautowork));
            }

            else if (data.indexOf("showTimerSet") != -1){
                String out = "";
                out += var->timerautowork[0].toString() + "\n";
                out += var->timerautowork[1].toString() + "\n";
                out += var->timerautowork[2].toString() + "\n";
                out += var->timerautowork[3].toString() + "\n";
                Serial.println(out);
            }

            if (data.indexOf("scanI2C") != -1){
                var->i2cScan.scan();
            }

            if (data.indexOf("connectWifi") != -1){
                comunity->wifi_controll->connectWiFi();
            }

            if (data.indexOf("writeWifiSSID ") != -1){
                data.replace("writeWifiSSID ","");

                var->db->writeWifiData(data,"");
            }

            if (data.indexOf("writeWifiPASS ") != -1){
                data.replace("writeWifiPASS ","");

                var->db->writeWifiData("",data);
            }

            if (data.indexOf("readWifi") != -1){
                String ssid_read,pass_read;

                var->db->readWifiData(&ssid_read,&pass_read);

                Serial.println("ssid:" + ssid_read + "     pass:" + pass_read);
            }

            

            if (data.indexOf(DEBUG_KEY1) != -1){
                var->datadebug.debug_wifiConnection = !var->datadebug.debug_wifiConnection;
            }

            else if (data.indexOf(DEBUG_KEY2) != -1){
                var->datadebug.debug_client_comunity = !var->datadebug.debug_client_comunity;
            }

            if(data.indexOf(DEBUG_KEY3) != -1){
                var->datadebug.debug_arduino_comunity = !var->datadebug.debug_arduino_comunity;
            }

            if(data.indexOf(DEBUG_KEY4) != -1){
                var->datadebug.debug_could_comunity = !var->datadebug.debug_could_comunity;
            }


            if(data.indexOf("debugStatus") != -1){
                Serial.println("-------------------------------------------");
                Serial.println("debug WifiConnect : " + String(var->datadebug.debug_wifiConnection));
                Serial.println("debug ClientComunity : " + String(var->datadebug.debug_client_comunity));
                Serial.println("debug ArduinoComunity : " + String(var->datadebug.debug_arduino_comunity));
                Serial.println("debug CouldComunity : " + String(var->datadebug.debug_could_comunity));
                // Serial.println("debug wificonnect : " + String(var->datadebug.debug_wifiConnection));
                Serial.println("-------------------------------------------");
            }
        
        }
    }
}