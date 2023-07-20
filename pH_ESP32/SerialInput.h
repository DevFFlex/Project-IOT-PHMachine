
class SerialInput
{
private:
    Variable *var;
    HardwareIO *hardwareIO;
    Comunity *comunity;
    ArduinoComunity *ardunoComunity;
    Database *db;
public:
    SerialInput(Variable *varIn,HardwareIO *hardIn,Comunity *comIn,ArduinoComunity *ardunoComunityIn,Database *dbIn)
    {
        var = varIn;
        hardwareIO = hardIn;
        comunity = comIn;
        ardunoComunity = ardunoComunityIn;
        db = dbIn;
    }

    void setup();
    void loop();

};

void SerialInput::setup()
{
}

void SerialInput::loop()
{
    if (Serial.available() > 0)
    {
        String data = Serial.readString();
        data.trim();

        if (data != "")
        {
            if (data.indexOf("relayall active") != -1)
            {
                hardwareIO->relay->active();
            }
            else if (data.indexOf("relayall deactive") != -1)
            {
                hardwareIO->relay->deactive();
            }
            else if (data.indexOf("relay on") != -1)
            {
                data.replace("relay on", "");
                hardwareIO->relay->on(data.toInt());
            }
            else if (data.indexOf("relay off") != -1)
            {
                data.replace("relay off", "");
                hardwareIO->relay->off(data.toInt());
            }
            else if (data.indexOf("relay toggle") != -1)
            {
                data.replace("relay toggle", "");
                hardwareIO->relay->toggle(data.toInt());
            }



            if (data.indexOf("send_") != -1)
            {
                data.replace("send_", "");
                comunity->sendOther(data);
            }

            if (data.indexOf("buzzer") != -1)
            {
                data.replace("buzzer", "");
                hardwareIO->buzzer->freq = data.toInt();
            }

        
            if (data.indexOf("sd rem ") != -1){
                data.replace("sd rem ","");
                hardwareIO->sdcard->removeDir(data.c_str());
            }

            else if (data.indexOf("sd dir ") != -1){
                data.replace("sd dir ","");
                String path = data;
                String data2 = data.substring(data.indexOf(" "),data.length());
                Serial.println("data2 = " + data2);
                hardwareIO->sdcard->listDir(data.c_str(),0);
            }

            else if (data.indexOf("sd mkdir ") != -1){
                data.replace("sd mkdir ","");
                hardwareIO->sdcard->createDir(data.c_str());
            }

            else if (data.indexOf("sd del ") != -1){
                data.replace("sd del ","");
                hardwareIO->sdcard->deleteFile(data.c_str());
            }

            if (data.indexOf("saveTAW") != -1){
                data.replace("saveTAW","");
                db->writeTimeAutoWork(var->timerautowork);
            }

            else if (data.indexOf("readTAW") != -1){
                data.replace("readTAW","");
                Serial.println(db->readTimeAutoWork(var->timerautowork));
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

            if (data.indexOf("debugServerData") != -1){
                comunity->setDisplayDataTranfer(!comunity->getDisplayDataTranfer());
                
            }

            else if (data.indexOf("debugServerDataRecv") != -1){
                comunity->debugDisplayDataRecv = !comunity->debugDisplayDataRecv;
            }


            if(data.indexOf("debugArduinoData") != -1){
                ardunoComunity->setDisplayDataTranfer(!ardunoComunity->getDisplayDataTranfer());
            }
        
        }
    }
}

