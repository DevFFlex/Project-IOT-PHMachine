
class SerialInput
{
private:
    Variable *var;
    HardwareIO *hardwareIO;
    Comunity *comunity;
    ArduinoComunity *ardunoComunity;
public:
    SerialInput(Variable *varIn,HardwareIO *hardIn,Comunity *comIn,ArduinoComunity *ardunoComunityIn)
    {
        var = varIn;
        hardwareIO = hardIn;
        comunity = comIn;
        ardunoComunity = ardunoComunityIn;
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
            if (data.indexOf("on") != -1)
            {
                data.replace("on", "");

                hardwareIO->relay->on(data.toInt());
            }

            if (data.indexOf("off") != -1)
            {
                data.replace("off", "");
                hardwareIO->relay->off(data.toInt());
            }

            if (data.indexOf("toggle") != -1)
            {
                data.replace("toggle", "");
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

            if (data.indexOf("relay active") != -1)
            {
                data.replace("relay active", "");
                hardwareIO->relay->active();
                Serial.println("r ac");
            }

            if (data.indexOf("relay deactive") != -1)
            {
                data.replace("relay deactive", "");
                hardwareIO->relay->deactive();
                Serial.println("r da");
            }
        
            if (data.indexOf("sd rem ") != -1){
                data.replace("sd rem ","");
                hardwareIO->sdcard->removeDir(data.c_str());
            }

            if (data.indexOf("sd dir ") != -1){
                data.replace("sd dir ","");
                String path = data;
                String data2 = data.substring(data.indexOf(" "),data.length());
                Serial.println("data2 = " + data2);
                hardwareIO->sdcard->listDir(data.c_str(),0);
            }

            if (data.indexOf("sd mkdir ") != -1){
                data.replace("sd mkdir ","");
                hardwareIO->sdcard->createDir(data.c_str());
            }

            if (data.indexOf("sd del ") != -1){
                data.replace("sd del ","");
                hardwareIO->sdcard->deleteFile(data.c_str());
            }

            if (data.indexOf("set cmvm") != -1){
              data.replace("set cmvm","");
              ardunoComunity->setValueAll("10", "20", "30", "40");
            }
        
        }
    }
}

