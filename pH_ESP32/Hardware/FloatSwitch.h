#define FLOATSWITCH_TANK 26
#define FLOATSWITCH_MIXTANK 32
#define FLOATSWITCH_PLOT 34

class FloatSwitch
{
private:
    Timer t_update;

    bool fsw_tank = false;
    bool fsw_mixtank = false;
    bool fsw_plot = false;

public:
    FloatSwitch() : t_update(10)
    {
    }

    void setup();
    void loop();

    bool getF1();
    bool getF2();
    bool getF3();
};

void FloatSwitch::setup()
{
    pinMode(FLOATSWITCH_TANK, INPUT);
    pinMode(FLOATSWITCH_MIXTANK, INPUT);
    pinMode(FLOATSWITCH_PLOT, INPUT);
}

void FloatSwitch::loop()
{
    int f1 = digitalRead(FLOATSWITCH_TANK);
    int f2 = digitalRead(FLOATSWITCH_MIXTANK);
    int f3 = digitalRead(FLOATSWITCH_PLOT);

    fsw_tank = (f1) ? true : false;
    fsw_mixtank = (f2) ? true : false;
    fsw_plot = (f3) ? true : false;

    // Serial.print(String(fsw_tank));
    // Serial.print(String(fsw_mixtank));
    // Serial.println(String(fsw_plot));
}

bool FloatSwitch::getF1()
{
    return fsw_tank;
}

bool FloatSwitch::getF2()
{
    return fsw_mixtank;
}
bool FloatSwitch::getF3()
{
    return fsw_plot;
}