#define ADDR_FLOAT_SW   0x22

#define FSW_MIXTANK_UP_PIN       0
#define FSW_MIXTANK_DOWN_PIN     1
#define FSW_WATERTANK_DOWN_PIN   2

class FloatSwitch
{
private:
    Timer t_update;

    PCF8574 FLOAT_SW;
    // int floatSwPinList[8] = {
    //     P0,
    //     P1,
    //     P2,
    //     P3,
    //     P4,
    //     P5,
    //     P6,
    //     P7
    // };

    bool fsw_mixTank_Up = false;
    bool fsw_mixtank_Down = false;
    bool fsw_waterTank_Down = false;

    bool status = false;

public:
    FloatSwitch() : t_update(1000) , FLOAT_SW(ADDR_FLOAT_SW)
    {
        
    }

    void setup();
    void loop();
    void toggle(int pin);

    bool getFSW_MixtankUp();
    bool getFSW_MixtankDown();
    bool getFSW_watertankDown();
};

void FloatSwitch::setup()
{
        FLOAT_SW.pinMode(P0,INPUT);
        FLOAT_SW.pinMode(P1,INPUT);
        FLOAT_SW.pinMode(P2,INPUT);
        FLOAT_SW.pinMode(P3,OUTPUT);
        FLOAT_SW.pinMode(P4,OUTPUT);
        FLOAT_SW.pinMode(P5,OUTPUT);
        FLOAT_SW.pinMode(P6,OUTPUT);
        FLOAT_SW.pinMode(P7,OUTPUT);
        FLOAT_SW.begin();

        FLOAT_SW.digitalWrite(P3,HIGH); 
        FLOAT_SW.digitalWrite(P4,HIGH); 
        FLOAT_SW.digitalWrite(P5,HIGH); 
        FLOAT_SW.digitalWrite(P6,HIGH); 
        FLOAT_SW.digitalWrite(P7,HIGH); 
}

void FloatSwitch::loop()
{
    if(t_update.isExpired()){
        fsw_mixTank_Up = FLOAT_SW.digitalRead(P0);
        fsw_mixtank_Down = FLOAT_SW.digitalRead(P1);
        fsw_waterTank_Down = FLOAT_SW.digitalRead(P2);

        Serial.print("MT_U = " + String(fsw_mixTank_Up) + "    ");
        Serial.print("MT_D = " + String(fsw_mixtank_Down) + "    ");
        Serial.println("WT_D = " + String(fsw_waterTank_Down) + "    ");

        toggle(P3);
    }
   
}

void FloatSwitch::toggle(int pin){
    status = !status;
    FLOAT_SW.digitalWrite(pin,status);
}

bool FloatSwitch::getFSW_MixtankUp()
{
    return fsw_mixTank_Up;
}

bool FloatSwitch::getFSW_MixtankDown()
{
    return fsw_mixtank_Down;
}
bool FloatSwitch::getFSW_watertankDown()
{
    return fsw_waterTank_Down;
}