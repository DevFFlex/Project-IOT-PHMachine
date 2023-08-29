#define MAX_STEP 4

enum Substance
{
  ACID,
  BASS
};

class PHAdjustmentProcess : public System
{
private:
  Variable *var;
  HardwareIO *hardwareIO;

  Timer looptimer;

  void step1()
  {
    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 1 : setup");
      var->workVar.outputText1 = "water in";
    }

    hardwareIO->relay->on(2);

    if (var->fsw_mixTank_Up && var->workVar.step == 1)
    {
      hardwareIO->relay->off(2);
      nextStep();
      Serial.println("step 1 : end");
    }
  }
  void step2()
  {
    static Timer step2_wait(10000);

    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 2 : setup");

      step2_wait.reset();
    }

    if (looptimer.isExpired())
    {
      var->workVar.outputText1 = "wait 10sec : " + String(step2_wait.getCurrentTime());
    }

    hardwareIO->relay->on(0);
    hardwareIO->relay->on(5);

    if (step2_wait.isExpired() && var->workVar.step == 2)
    {
      hardwareIO->relay->off(0);
      hardwareIO->relay->off(5);
      nextStep();
      Serial.println("step 2 : end");
    }
  }
  void step3()
  {
    static Timer step3_wait(20000);

    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 3 : setup");

      step3_wait.reset();
    }

    if (looptimer.isExpired())
    {
      var->workVar.outputText1 = "wait 20sec : " + String(step3_wait.getCurrentTime());
    }

    if (step3_wait.isExpired() && var->workVar.step == 3)
    {

      float space_rate = 0.3;
      if (var->input_ph + space_rate >= var->mixTank_pH >= var->input_ph - space_rate)
      {
        Serial.println("nextstep 4");
        nextStep();
      }
      else
      {
        var->workVar.step = 10;
        var->workVar.working_step_setup = true;

        float phDiff = var->input_ph - var->mixTank_pH;

        Serial.println("---------------------------------------------------");
        Serial.println("input - current = pHDiff");
        Serial.println(String(var->input_ph) + " - " + String(var->mixTank_pH) + " = " + String(phDiff));

        if (phDiff > 0)
        {
          Serial.println("Add Bass");
          var->workVar.outputText1 = "Add Bass";
          stepAddSubstance(BASS);
        }
        else if (phDiff < 0)
        {
          Serial.println("Add Acid");
          var->workVar.outputText1 = "Add Acid";

          stepAddSubstance(ACID);
        }
        Serial.println("---------------------------------------------------");
      }

      Serial.println("step 3 : end");
    }
  }
  void step4()
  {
    bool condition_nextstep = var->fsw_mixtank_Down;

    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 4 : setup");
    }

    hardwareIO->relay->on(1);
    if (condition_nextstep && var->workVar.step == 4)
    {
      hardwareIO->relay->off(1);
      nextStep();
      Serial.println("step 4 : end");
    }
  }
  void stepAddSubstance(Substance sub)
  {
    static Timer wait_Addsubstance(2000);

    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("stepAddSubstance : setup");

      wait_Addsubstance.reset();

      if (sub == BASS)
      {
        hardwareIO->relay->on(3);
        var->workVar.outputText1 = "Add Bass";
      }
      else if (sub == ACID)
      {
        hardwareIO->relay->on(4);
        var->workVar.outputText1 = "Add Acid";
      }
    }

    if (wait_Addsubstance.isExpired())
    {
      hardwareIO->relay->off(3);
      hardwareIO->relay->off(4);
      goStep(2);
      Serial.println("stepAddSubstance : end");
    }
  }

public:
  PHAdjustmentProcess(Variable *varIn)
  {
    var = varIn;
    hardwareIO = var->hardwareIO;
    looptimer.setInterval(1000);
  }

  void setup() override
  {
    
  }
  void loop() override
  {
    if (var->workVar.working_status)
    {
      if (var->workVar.working_status_setup)
      {
        var->workVar.working_status_setup = false;
        hardwareIO->buzzer->on(300);
        nextStep();
      }

      switch (var->workVar.step)
      {
      case 1:
        step1();
        break;
      case 2:
        step2();
        break;
      case 3:
        step3();
        break;
      case 4:
        step4();
        break;
      }
    }
    else if (!var->workVar.working_status && !var->workVar.working_status_setup)
    {
      stop();
    }
  }

  void nextStep()
  {
    var->workVar.step += 1;

    if (var->workVar.step > MAX_STEP)
    {
      stop();
      hardwareIO->buzzer->on(200);
      Serial.println("adjust process success,you need = " + String(var->input_ph) + "    pH now = " + String(var->mixTank_pH));
      return;
    }

    hardwareIO->buzzer->on(300);

    var->workVar.working_step_setup = true;
  }
  void goStep(int step)
  {
    var->workVar.step = step;

    if (var->workVar.step > MAX_STEP)
    {
      var->workVar.step = 0;
      var->workVar.working_status = false;
      var->workVar.working_status_setup = true;
      hardwareIO->buzzer->on(200);
      return;
    }

    hardwareIO->buzzer->on(300);

    var->workVar.working_step_setup = true;
  }
  void stop()
  {
    var->workVar.step = 0;
    var->workVar.working_status = false;
    var->workVar.working_status_setup = true;

    hardwareIO->relay->off(0);
    hardwareIO->relay->off(1);
    hardwareIO->relay->off(2);
    hardwareIO->relay->off(3);
    hardwareIO->relay->off(4);
    hardwareIO->relay->off(5);

    Serial.println("stop pH AdjProcess");
  }
  bool timerAutoWork_Compare_Rtctime(int index)
  {
    byte active_hour = var->timerautowork[index].getHour();
    byte active_minute = var->timerautowork[index].getMinute();

    if (active_hour == -1)
      return false;

    return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && var->timerautowork[index].getStatus());
  }
};
