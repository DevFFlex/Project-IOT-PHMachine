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
  Community *community;
  HardwareIO *hardwareIO;

  Timer looptimer;

  Substance substance;
  long startTime = 0;
  long endTime = 0;

  void step1()
  {
    if (var->workVar.working_step_setup && var->workVar.step == 1)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 1 : setup");
      var->workVar.outputText1 = "WI";
    }

    hardwareIO->relay->off(2);

    if (var->fsw.mixTank_Up)
    {
      hardwareIO->relay->on(2);
      nextStep();
      Serial.println("step 1 : end");
    }
  }
  void step2()
  {
    static Timer step2_wait(10000);

    if (var->workVar.working_step_setup && var->workVar.step == 2)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 2 : setup");

      step2_wait.setInterval(var->workVar.wait_stirring_pump);
      step2_wait.reset();
    }

    if (looptimer.isExpired())
    {
      var->workVar.outputText1 = String(step2_wait.getCurrentTime()) + "s";
    }

    hardwareIO->relay->on(0);
    

    if (step2_wait.isExpired() && var->workVar.step == 2)
    {
      hardwareIO->relay->off(0);
      
      nextStep();
      Serial.println("step 2 : end");
    }
  }
  void step3()
  {
    static Timer step3_wait(20000);

    if (var->workVar.working_step_setup && var->workVar.step == 3)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 3 : setup");

      step3_wait.setInterval(var->workVar.wait_pH_stabilize);
      step3_wait.reset();
    }

    if (looptimer.isExpired())
    {
      var->workVar.outputText1 = String(step3_wait.getCurrentTime()) + "s";
    }

    if (step3_wait.isExpired() && var->workVar.step == 3)
    {
      if(var->workVar.pH_mixtankFirst == -1)var->workVar.pH_mixtankFirst = var->mixTank_pH;
      float phDiff = var->workVar.adjustCurrentpH - var->mixTank_pH;
      
      if(var->mixTank_pH >= var->workVar.adjustCurrentpH - var->workVar.pH_space_rate && var->mixTank_pH < var->workVar.adjustCurrentpH + var->workVar.pH_space_rate){
        goStep(5);
        return;
      }

      
      if (phDiff > 0)
      {
        var->workVar.outputText1 = "AB";
        substance = BASS;
      }
      else if (phDiff < 0)
      {
        var->workVar.outputText1 = "AC";
        substance = ACID;
      }
      Serial.println("step 3 : end");
      nextStep();
    }
  }
  void step4()
  {
    static Timer wait_Addsubstance(2000);

    if (var->workVar.working_step_setup && var->workVar.step == 4)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 4 : setup");

      

      if (substance == BASS)
      {
        hardwareIO->relay->on(3);
        var->workVar.outputText1 = "AB";
        var->workVar.addBaseCount++;
        var->workVar.addBase_mL += (1.3 * (var->workVar.wait_baseUseTime / 1000));
        wait_Addsubstance.setInterval(var->workVar.wait_baseUseTime);
        wait_Addsubstance.reset();
        
      }
      else if (substance == ACID)
      {
        hardwareIO->relay->on(4);
        var->workVar.outputText1 = "AC";
        var->workVar.addAcidCount++;
        var->workVar.addAcid_mL += (1.3 * (var->workVar.wait_acidUseTime / 1000));
        wait_Addsubstance.setInterval(var->workVar.wait_acidUseTime);
        wait_Addsubstance.reset();
        
      }
    }

    if (wait_Addsubstance.isExpired())
    {
      var->workVar.addBaseAcidCount++;

      hardwareIO->relay->off(3);
      hardwareIO->relay->off(4);
      hardwareIO->relay->on(5);

      if(var->workVar.addBaseCount >= var->workVar.limite_use_base || var->workVar.addAcidCount >= var->workVar.limite_use_acid){
        var->workVar.validity_status = "LIMITE_SUBTANCE";
        nextStep();
        return;
      }


      Serial.println("step 4 : end");
      goStep(2);
      

      
    }
  }
  void step5()
  {
    if (var->workVar.working_step_setup && var->workVar.step == 5)
    {
      var->workVar.working_step_setup = false;
      Serial.println("step 4 : setup");
    }

    hardwareIO->relay->on(1);
    hardwareIO->relay->off(5);
    if (var->fsw.mixTank_Down)
    {
      var->workVar.validity_status = "SUCCESS";
      hardwareIO->relay->off(1);
      nextStep();
      Serial.println("step 4 : end");
    }
  }
  
  void nextStep()
  {
    var->workVar.step += 1;

    if (var->workVar.step > MAX_STEP)
    {
      stop();
      return;
    }

    var->workVar.working_step_setup = true;
  }
  void goStep(int step)
  {
    var->workVar.step = step;

    if (var->workVar.step > MAX_STEP)
    {
      var->workVar.step--;
      nextStep();
      return;
    }

    var->workVar.working_step_setup = true;
  }
  

public:
  PHAdjustmentProcess(Variable *var,Community *community)
  {
    this->var = var;
    this->community = community;
    hardwareIO = var->hardwareIO;
    looptimer.setInterval(1000);
  }

  void setup() override
  {
    
  }
  void loop() override
  {
    for(int i = 0;i<4;i++){
      if(workTimer_Compare_Rtctime(i) && !var->workVar.work_with_timer_status){
        var->workVar.work_with_timer_status = true;
        var->workVar.startAdjustPH(var->workTimer[i].PH,var->workTimer[i].T);
      }
    }


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
      case 5:
        step5();
        break;
      }
    }
    else if (!var->workVar.working_status && !var->workVar.working_status_setup){
      var->workVar.validity_status = "CANCEL";
      stop();
    }


    


  }

  
  void stop()
  {
    hardwareIO->relay->off(0);
    hardwareIO->relay->off(1);
    hardwareIO->relay->on(2);
    hardwareIO->relay->off(3);
    hardwareIO->relay->off(4);
    hardwareIO->relay->off(5);

    Serial.println("stop pH AdjProcess");

    endTime = millis();
    var->workVar.useTime = (endTime - startTime) / 1000;


    if(var->workVar.validity_status == "LIMITE_SUBTANCE"){
      hardwareIO->buzzer->on(200);
      Serial.println("adjust process fail limite subtance ,you need = " + String(var->workVar.adjustCurrentpH) + "    pH now = " + String(var->mixTank_pH));
    }else if(var->workVar.validity_status == "SUCCESS"){
      
      hardwareIO->buzzer->on(200);
      Serial.println("adjust process success,you need = " + String(var->workVar.adjustCurrentpH) + "    pH now = " + String(var->mixTank_pH));
    
      var->workVar.adjustT_Counter--;
    
      if(var->workVar.adjustT_Counter > 0){
        Serial.println("adjust T is " + String(var->workVar.adjustT_Counter));
        
        var->workVar.startAdjustPH(var->workVar.adjustCurrentpH,var->workVar.adjustT_Counter);
      }else{
        Serial.println("Finish");
      }
    }



    Serial.println("---------------------------------------");
    Serial.println("pH Input : " + String(var->workVar.adjustCurrentpH));
    Serial.println("pH From : " + String(var->workVar.pH_mixtankFirst));
    Serial.println("pH Output : " + String(var->mixTank_pH));
    Serial.println("T is : " + String(var->workVar.addBaseAcidCount));
    Serial.println("Add Base : " + String(var->workVar.addBaseCount));
    Serial.println("Amount Base : " + String(var->workVar.addBase_mL));
    Serial.println("Add Acid : " + String(var->workVar.addAcidCount));
    Serial.println("Amount Acid : " + String(var->workVar.addAcid_mL));
    Serial.println("AllTime is : " + String(var->workVar.useTime) + " s");
    Serial.println("Validity Status : " + String(var->workVar.validity_status));
    Serial.println("---------------------------------------");

    community->couldComunity->sendPH_All(String(var->workVar.adjustCurrentpH),String(var->workVar.pH_mixtankFirst) + "," + String(var->mixTank_pH),String(var->workVar.addBaseCount),String(var->workVar.addBase_mL),String(var->workVar.addAcidCount),String(var->workVar.addAcid_mL),String(var->workVar.addBaseAcidCount),String(var->workVar.useTime),String("-1"),var->workVar.validity_status);

    var->workVar.work_with_timer_status = false;

    var->workVar.resetData();

    
  }
  bool workTimer_Compare_Rtctime(int index)
  {
    byte active_hour = var->workTimer[index].HOUR;
    byte active_minute = var->workTimer[index].MINUTE;


    if (active_hour == -1)
      return false;

    return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && var->workTimer[index].ACTIVE_STATUS);
  }
};
