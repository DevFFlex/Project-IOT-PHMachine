#define MAX_STEP 4

enum Substance{
  ACID,
  BASS
};


class PHAdjustmentProcess {
private:
  Variable *var;
  HardwareIO *hardwareIO;

  void step1();
  void step2();
  void step3();
  void step4();
  void stepAddSubstance(Substance sub);

public:
  PHAdjustmentProcess(Variable *varIn) {
    var = varIn;
    hardwareIO = var->hardwareIO;
  }

  void setup();
  void loop();
  
  void nextStep();
  void goStep(int step);
  void stop();
  bool timerAutoWork_Compare_Rtctime(int index);
};

void PHAdjustmentProcess::setup() {
}

void PHAdjustmentProcess::loop() {
  if (var->workVar.working_status) {
    if (var->workVar.working_status_setup) {
      var->workVar.working_status_setup = false;
      hardwareIO->buzzer->on(300);
      nextStep();
    }

    switch (var->workVar.step) {
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
  } else if (!var->workVar.working_status && !var->workVar.working_status_setup) {
    stop();
  }
}

void PHAdjustmentProcess::nextStep() {
  var->workVar.step += 1;

  if (var->workVar.step > MAX_STEP) {
    var->workVar.step = 0;
    var->workVar.working_status = false;
    var->workVar.working_status_setup = true;
    hardwareIO->buzzer->on(200);
    Serial.println("adjust process success,you need = " + String(var->input_ph) + "    pH now = " + String(var->mixTank_pH));
    return;
  }

  hardwareIO->buzzer->on(300);

  var->workVar.working_step_setup = true;
}

void PHAdjustmentProcess::goStep(int step){
  var->workVar.step = step;

  if (var->workVar.step > MAX_STEP) {
    var->workVar.step = 0;
    var->workVar.working_status = false;
    var->workVar.working_status_setup = true;
    hardwareIO->buzzer->on(200);
    return;
  }

  hardwareIO->buzzer->on(300);

  var->workVar.working_step_setup = true;
}

void PHAdjustmentProcess::stop() {
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

bool PHAdjustmentProcess::timerAutoWork_Compare_Rtctime(int index) {
  byte active_hour = var->timerautowork[index].getHour();
  byte active_minute = var->timerautowork[index].getMinute();

  if (active_hour == -1)
    return false;

  return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && var->timerautowork[index].getStatus());
}



void PHAdjustmentProcess::step1() {
  bool condition_nextstep = var->fsw_mixTank_Up;

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 1 : water in");
  }

  hardwareIO->relay->on(2);
  if (condition_nextstep) {
    hardwareIO->relay->off(2);
    nextStep();
  }
}

void PHAdjustmentProcess::step2() {
  static Timer step2_wait(20000);
  
  bool condition_nextstep = step2_wait.isExpired();

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 2 : suck water to phsensor");
    
    step2_wait.reset();
  }

  hardwareIO->relay->on(0);
  hardwareIO->relay->on(5);

  if (condition_nextstep) {
    hardwareIO->relay->off(0);
    hardwareIO->relay->off(5);
    nextStep();
  }
}

void PHAdjustmentProcess::step3() {
  static Timer step3_wait(90000);
  bool condition_nextstep = step3_wait.isExpired();

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 3 : wait 1.30 , measure ph");

    step3_wait.reset();
  }

  if(condition_nextstep){
    
    if(var->mixTank_pH <= var->input_ph + 0.5 && var->mixTank_pH >= var->input_ph - 0.5){
        nextStep();
    }else{
      
      float phDiff = var->input_ph - var->mixTank_pH;

      Serial.println("---------------------------------------------------");
      Serial.println("input - current = pHDiff");
      Serial.println( String(var->input_ph) + " - " + String(var->mixTank_pH) + " = " + String(phDiff));
    
      if(phDiff > 0){
        Serial.println("Add Bass");
        stepAddSubstance(BASS);
      }else if (phDiff < 0){
        Serial.println("Add Acid");
        stepAddSubstance(ACID);
      }
      Serial.println("---------------------------------------------------");
    }
      
  }

}

void PHAdjustmentProcess::stepAddSubstance(Substance sub){
  static Timer wait_Addsubstance(2000);

  bool condition_nextstep = wait_Addsubstance.isExpired();

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("stepAddSubstance 1 sec");

    wait_Addsubstance.reset();

    if(sub == BASS)hardwareIO->relay->on(3,1000);
    else if(sub == ACID)hardwareIO->relay->on(4,1000);
  }


  if (condition_nextstep) {
    goStep(2);
  }
}

void PHAdjustmentProcess::step4() {
  bool condition_nextstep = var->fsw_mixtank_Down;

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 4 : water out");
  }

  hardwareIO->relay->on(1);
  if (condition_nextstep) {
    hardwareIO->relay->off(1);
    nextStep();
  }
}


