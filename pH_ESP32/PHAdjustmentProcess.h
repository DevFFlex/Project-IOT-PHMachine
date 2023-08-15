#define MAX_STEP 4

class PHAdjustmentProcess {
private:
  Variable *var;
  HardwareIO *hardwareIO;

  void step1();
  void step2();
  void step3();
  void step4();

public:
  PHAdjustmentProcess(Variable *varIn) {
    var = varIn;
    hardwareIO = var->hardwareIO;
  }

  void setup();
  void loop();
  
  void nextStep();
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
    Serial.println("step 1");
  }

  hardwareIO->relay->on(2);
  if (condition_nextstep) {
    hardwareIO->relay->off(2);
    nextStep();
  }
}

void PHAdjustmentProcess::step2() {
  bool condition_nextstep = var->mixTank_pH > var->input_ph + 0.5 && var->mixTank_pH < var->input_ph - 0.5;

  static Timer timeCheck(5000);

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 2");
  }

  hardwareIO->relay->on(5);

  if(timeCheck.isExpired()){
    hardwareIO->relay->toggle(0);
  }

  if (condition_nextstep) {
    hardwareIO->relay->off(5);
    nextStep();
  }
}

void PHAdjustmentProcess::step3() {
  bool condition_nextstep = var->fsw_mixtank_Down;


  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 3");
  }

  hardwareIO->relay->on(1);

  if (condition_nextstep) {
    hardwareIO->relay->off(1);
    nextStep();
  }
}

void PHAdjustmentProcess::step4() {
  bool condition_nextstep = true;

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    Serial.println("step 4");
  }


  if (condition_nextstep) {
    nextStep();
  }
}
