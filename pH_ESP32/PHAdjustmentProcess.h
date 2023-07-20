#define MAX_STEP 4

class PHAdjustmentProcess {
private:
  Variable *var;
  HardwareIO *hardwareIO;
  Comunity *comunity;

  Timer t_step1;
  Timer t_step2;
  Timer t_step3;
  Timer t_step4;

  String step_desc[MAX_STEP]{
    "ดูดนํ้าจากแปลงผักเข้าถังผสม",
    "ปรับนํ้า-วนนํ้า",
    "ดูดนํ้าที่ปรับเสร็จเเล้วเข้าถังเก็บนํ้า",
    "ได้ค่า pH ที่ต้องการเเล้ว,ดูดนํ้าออกจากถังผสมไปแปลงผัก"
  };

  void step1();
  void step2();
  void step3();
  void step4();

public:
  PHAdjustmentProcess(Variable *varIn, HardwareIO *hardwareIOIn, Comunity *comunityIn)
    : t_step1(10000), t_step2(30000), t_step3(10000), t_step4(10000) {
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

void PHAdjustmentProcess::setup() {
}

void PHAdjustmentProcess::loop() {

  if (var->workVar.working_status) {
    if (var->workVar.working_status_setup) {
      var->workVar.working_status_setup = false;
      hardwareIO->buzzer->on(300);
      t_step1.reset();
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
    Serial.println("stop------");
    hardwareIO->relay->off(0);
    hardwareIO->relay->off(1);
    hardwareIO->relay->off(2);
    hardwareIO->relay->off(3);
    hardwareIO->relay->off(4);
    hardwareIO->relay->off(5);
    stop();
  }
}

void PHAdjustmentProcess::nextStep() {
  var->workVar.step += 1;

  if (var->workVar.step > MAX_STEP) {
    // hardwareIO->lcdOutput->printL("finish......", 1);
    var->workVar.step = 0;
    var->workVar.working_status = false;
    var->workVar.working_status_setup = true;
    hardwareIO->buzzer->on(200);

    comunity->setC_Output("ปรับค่า pH, สิ้นสุดการทำงาน");
    return;
  }

  // hardwareIO->lcdOutput->printL("step : " + String(var->workVar.step), 1);
  hardwareIO->buzzer->on(300);

  t_step1.reset();
  t_step2.reset();
  t_step3.reset();
  t_step4.reset();

  var->workVar.working_step_setup = true;
}

void PHAdjustmentProcess::stop() {
  var->workVar.step = 0;
  var->workVar.working_status = false;
  var->workVar.working_status_setup = true;
}

bool PHAdjustmentProcess::timerAutoWork_Compare_Rtctime(int index) {
  byte active_hour = var->timerautowork[index].getHour();
  byte active_minute = var->timerautowork[index].getMinute();

  if (active_hour == -1)
    return false;

  return (hardwareIO->rtc->getHour() == active_hour && hardwareIO->rtc->getMinute() == active_minute && var->timerautowork[index].getStatus());
}


void PHAdjustmentProcess::step1() {
  // bool condition_nextstep = t_step1.isExpired();
  bool condition_nextstep = !var->fsw_mixTank_Up;

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    comunity->setC_Output(step_desc[0]);
    comunity->setStepText(step_desc[0]);
    Serial.println("step 1");
  }

  hardwareIO->relay->on(0);
  if (condition_nextstep) {
    hardwareIO->relay->off(0);
    nextStep();
  }
}

void PHAdjustmentProcess::step2() {
  bool condition_nextstep = t_step2.isExpired();


  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    comunity->setC_Output(step_desc[1]);
    comunity->setStepText(step_desc[1]);
    Serial.println("step 2");
  }

  hardwareIO->relay->on(5);

  if (condition_nextstep) {
    hardwareIO->relay->off(5);
    nextStep();
  }
}

void PHAdjustmentProcess::step3() {
  bool condition_nextstep = var->fsw_mixtank_Down;


  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    comunity->setC_Output(step_desc[2]);
    comunity->setStepText(step_desc[2]);
    Serial.println("step 3");
  }

  hardwareIO->relay->on(1);

  if (condition_nextstep) {
    hardwareIO->relay->off(1);
    nextStep();
  }
}

void PHAdjustmentProcess::step4() {
  bool condition_nextstep = !var->fsw_waterTank_Down;

  if (var->workVar.working_step_setup) {
    var->workVar.working_step_setup = false;
    comunity->setC_Output(step_desc[3]);
    comunity->setStepText(step_desc[3]);
    Serial.println("step 4");
  }

  hardwareIO->relay->on(2);

  if (condition_nextstep) {
    hardwareIO->relay->off(2);
    nextStep();
  }
}
