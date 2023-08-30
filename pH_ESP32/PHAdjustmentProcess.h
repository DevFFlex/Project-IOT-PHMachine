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

  Substance substance;

  Timer looptimer;

  long startTime = 0;
  long endTime = 0;

  void step1()
  {
    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 1 : setup");
      var->workVar.outputText1 = "WI";
    }

    hardwareIO->relay->on(2);

    if (var->fsw.mixTank_Up)
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
      var->workVar.outputText1 = String(step2_wait.getCurrentTime()) + "s";
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
      var->workVar.outputText1 = String(step3_wait.getCurrentTime()) + "s";
    }

    if (step3_wait.isExpired() && var->workVar.step == 3)
    {

      
      if (var->mixTank_pH <= var->input_ph + var->workVar.pH_space_rate && var->mixTank_pH >= var->input_ph - var->workVar.pH_space_rate)
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
          var->workVar.outputText1 = "AB";
          substance = BASS;
        }
        else if (phDiff < 0)
        {
          Serial.println("Add Acid");
          var->workVar.outputText1 = "AC";

          substance = ACID;
        }
        Serial.println("---------------------------------------------------");
      }

      Serial.println("step 3 : end");
    }
  }
  void step4()
  {
    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 4 : setup");
    }

    hardwareIO->relay->on(1);
    if (var->fsw.mixTank_Down)
    {
      hardwareIO->relay->off(1);
      nextStep();
      Serial.println("step 4 : end");
    }
  }
  void stepAddSubstance()
  {
    static Timer wait_Addsubstance(2000);

    if (var->workVar.working_step_setup)
    {
      var->workVar.working_step_setup = false;
      Serial.println("stepAddSubstance : setup");

      wait_Addsubstance.reset();

      if (substance == BASS)
      {
        hardwareIO->relay->on(3);
        var->workVar.outputText1 = "AB";
        var->workVar.addBaseCount++;
      }
      else if (substance == ACID)
      {
        hardwareIO->relay->on(4);
        var->workVar.outputText1 = "AC";
        var->workVar.addAcidCount++;
      }
    }

    if (wait_Addsubstance.isExpired())
    {
      hardwareIO->relay->off(3);
      hardwareIO->relay->off(4);
      goStep(2);
      Serial.println("stepAddSubstance : end");

      var->workVar.T++;
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
        startTime = millis();
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
      case 10:
        stepAddSubstance();
        break;
      }
    }
    else if (!var->workVar.working_status && !var->workVar.working_status_setup){
      var->workVar.validity_status = "CANCEL";
      stop();
    }


  }

  void nextStep()
  {
    var->workVar.step += 1;

    if (var->workVar.step > MAX_STEP)
    {
      var->workVar.validity_status = "SUCCESS";
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
    

    hardwareIO->relay->off(0);
    hardwareIO->relay->off(1);
    hardwareIO->relay->off(2);
    hardwareIO->relay->off(3);
    hardwareIO->relay->off(4);
    hardwareIO->relay->off(5);

    Serial.println("stop pH AdjProcess");

    
    
    endTime = millis();
    var->workVar.useTime = (endTime - startTime) / 1000;


    Serial.println("---------------------------------------");
    Serial.println("T is : " + String(var->workVar.T));
    Serial.println("Add Base : " + String(var->workVar.addBaseCount));
    Serial.println("Add Acid : " + String(var->workVar.addAcidCount));
    Serial.println("AllTime is : " + String(var->workVar.useTime) + " s");
    Serial.println("Validity Status : " + String(var->workVar.validity_status));
    Serial.println("---------------------------------------");

    var->workVar.resetData();
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
