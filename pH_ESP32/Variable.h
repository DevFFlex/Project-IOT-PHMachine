#include "VarObject/TimerAutoWork.h"
#include "VarObject/PHCalibrateSet.h"


typedef struct WorkVarStruct {
  int step = 0;
  bool working_status = false;
  bool working_status_setup = true;
  bool working_step_setup = false;
} WorkVar;

class Variable {

public:
  StringManager *strManager = new StringManager();
  ScanI2C i2cScan;

  float input_ph = 0;
  float mixTank_pH = 0;
  float tempC = 0;
  float humidity = 0;

  WorkVar workVar;

  TimerAutoWork *timerautowork = new TimerAutoWork[4];
  PHCalibrateSet phCalibrateSet;

  float onClientRequestStatus = false;

  Variable() {
  }
};
