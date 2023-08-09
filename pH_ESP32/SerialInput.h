#include "Arduino.h"


#define DEBUG_KEY1 "debugServerData"
#define DEBUG_KEY2 "debugServerDataRecv"
#define DEBUG_KEY3 "debugArduinoData"



class SerialInput : public System{
private:
    Variable *var;
    Comunity *comunity;

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

        if (data != "")
        {
            if (data.indexOf("relayall active") != -1)
            {
                var->hardwareIO->relay->active();
            }
            else if (data.indexOf("relayall deactive") != -1)
            {
                var->hardwareIO->relay->deactive();
            }
            else if (data.indexOf("relay on") != -1)
            {
                data.replace("relay on", "");
                var->hardwareIO->relay->on(data.toInt());
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
                var->hardwareIO->buzzer->freq = data.toInt();
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

            if (data.indexOf("saveTAW") != -1){
                data.replace("saveTAW","");
                var->db->writeTimeAutoWork(var->timerautowork);
            }

            else if (data.indexOf("readTAW") != -1){
                data.replace("readTAW","");
                Serial.println(var->db->readTimeAutoWork(var->timerautowork));
            }

            else if (data.indexOf("showTAW") != -1){
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

            if (data.indexOf(DEBUG_KEY1) != -1){
                
            }

            else if (data.indexOf(DEBUG_KEY2) != -1){
                comunity->clientComunity->debugDisplayDataRecv = !comunity->clientComunity->debugDisplayDataRecv;
            }

            if(data.indexOf(DEBUG_KEY3) != -1){
                comunity->ardunoComunity->setDisplayDataTranfer(!comunity->ardunoComunity->getDisplayDataTranfer());
            }
        
        }
    }
}