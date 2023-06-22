#include "VarObject/TimerAutoWork.h"
#include "VarObject/PHCalibrateSet.h"

class Variable
{

public:
  float input_ph = 0;
  float mixTank_pH = 0;
  float temp = 0;
  TimerAutoWork *timerautowork = new TimerAutoWork[4];
  PHCalibrateSet *phCalibrateSet = new PHCalibrateSet();

  StringManager *strManager = new StringManager();

  float onClientRequestStatus = false;
  Variable()
  {

  }

};

