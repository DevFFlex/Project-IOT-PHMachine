#define MAX_STEP 4

class Work
{
private:
    Variable *var;
    HardwareIO *hardwareIO;
    Comunity *comunity;

    Timer t_step1;
    Timer t_step2;
    Timer t_step3;
    Timer t_step4;

    String step_desc[MAX_STEP]{
        "S1 ดูดนํ้าจากแปลงผักเข้าถังเก็บนํ้า",
        "S2 ดูดนํ้าจากถังเก็บนํ้าเข้าถังผสม",
        "S3 ปรับนํ้า-วนนํ้า",
        "S4 ได้ค่า pH ที่ต้องการเเล้ว,ดูดนํ้าออกจากถังผสมไปแปลงผัก"};

public:
    Work(Variable *varIn, HardwareIO *hardwareIOIn, Comunity *comunityIn) : t_step1(10000), t_step2(10000), t_step3(10000), t_step4(10000)
    {
        var = varIn;
        hardwareIO = hardwareIOIn;
        comunity = comunityIn;
    }

    void setup();
    void loop();
    void nextStep();
    void stop();
    bool timerAutoWork_Compare_Rtctime(int index);
};

void Work::setup()
{
}

void Work::loop()
{

    if (var->workVar.working_status)
    {
        if (var->workVar.working_status_setup)
        {
            var->workVar.working_status_setup = false;
            hardwareIO->buzzer->on(300);
            t_step1.reset();
            nextStep();
        }

        switch (var->workVar.step)
        {
        case 1:
            if (var->workVar.change_step)
            {
                var->workVar.change_step = false;
                comunity->setC_Output(step_desc[0]);
            }
            Serial.println("step 1");
            hardwareIO->lcdOutput->printL("step 1", 1);
            hardwareIO->relay->on(2);
            if (t_step1.isExpired())
            {
                hardwareIO->relay->off(2);
                nextStep();
            }
            break;
        case 2:
            if (var->workVar.change_step)
            {
                var->workVar.change_step = false;
                comunity->setC_Output(step_desc[1]);
            }
            Serial.println("step 2");
            hardwareIO->lcdOutput->printL("step 2", 1);
            hardwareIO->relay->on(0);

            hardwareIO->lcdOutput->printL("wf = " + String(hardwareIO->waterSensor->getValue()), 2);

            if (t_step2.isExpired())
            {
                hardwareIO->relay->off(0);
                nextStep();
            }
            break;
        case 3:
            if (var->workVar.change_step)
            {
                var->workVar.change_step = false;
                comunity->setC_Output(step_desc[2]);
            }
            Serial.println("step 3");
            hardwareIO->lcdOutput->printL("step 3", 1);
            hardwareIO->relay->on(5);

            if (t_step3.isExpired())
            {
                hardwareIO->relay->off(5);
                nextStep();
            }
            break;

        case 4:
            if (var->workVar.change_step)
            {
                var->workVar.change_step = false;
                comunity->setC_Output(step_desc[3]);
            }
            Serial.println("step 4");
            hardwareIO->lcdOutput->printL("step 4", 1);
            hardwareIO->relay->on(1);

            if (t_step4.isExpired())
            {
                hardwareIO->relay->off(1);
                nextStep();
            }
            break;

        default:
            break;
        }
    }
    else if (!var->workVar.working_status && !var->workVar.working_status_setup)
    {
        Serial.println("stop------");
        stop();
    }
}

void Work::nextStep()
{
    var->workVar.step += 1;

    if (var->workVar.step > MAX_STEP)
    {
        hardwareIO->lcdOutput->printL("finish......", 1);
        var->workVar.step = 0;
        var->workVar.working_status = false;
        var->workVar.working_status_setup = true;
        hardwareIO->buzzer->on(200);

        comunity->setC_Output("ปรับค่า pH, สิ้นสุดการทำงาน");
        comunity->setC_InputPH_Finish();
        return;
    }

    hardwareIO->lcdOutput->printL("step : " + String(var->workVar.step), 1);
    hardwareIO->buzzer->on(300);

    t_step1.reset();
    t_step2.reset();
    t_step3.reset();
    t_step4.reset();

    var->workVar.change_step = true;
}

void Work::stop()
{
    var->workVar.step = 0;
    var->workVar.working_status = false;
    var->workVar.working_status_setup = true;
}

bool Work::timerAutoWork_Compare_Rtctime(int index)
{
    byte active_hour = var->timerautowork[index].getHour();
    byte active_minute = var->timerautowork[index].getMinute();

    if (active_hour == -1)
        return false;

    return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && var->timerautowork[index].getStatus());
}
