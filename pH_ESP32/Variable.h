#include "VarObject/TimerAutoWork.h"
#include "VarObject/PHCalibrateSet.h"

class Work_Variable{
  public:
  int step = 0;
  bool working_status = false;
  bool working_status_setup = true;
  bool change_step = false;
};

class Variable {

public:
  float input_ph = 0;
  float mixTank_pH = 0;
  float temp = 0;
  TimerAutoWork *timerautowork = new TimerAutoWork[4];
  PHCalibrateSet *phCalibrateSet = new PHCalibrateSet();

  StringManager *strManager = new StringManager();
  
  Work_Variable workVar;

  float onClientRequestStatus = false;

  Variable() {
  }
};
