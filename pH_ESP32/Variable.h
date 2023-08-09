#include "VarObject/TimerAutoWork.h"

#include "HardwareIO.h"
#include "Database.h"

typedef struct WorkVarStruct {
  int step = 0;
  bool working_status = false;
  bool working_status_setup = true;
  bool working_step_setup = false;
} WorkVar;

class Variable : public System{

public:
  ScanI2C i2cScan;

  HardwareIO *hardwareIO = new HardwareIO();
  Database *db = new Database();

  float input_ph = 0;
  float mixTank_pH = 0;
  float tempC = 0;
  float humidity = 0;

  bool fsw_mixTank_Up = false;
  bool fsw_mixtank_Down = false;
  bool fsw_waterTank_Down = false;

  WorkVar workVar;

  TimerAutoWork *timerautowork = new TimerAutoWork[4];

  float onClientRequestStatus = false;


  void setup() override {
    hardwareIO->setup();
    db->setup();
  }

  void loop() override {
    hardwareIO->loop();
    db->loop();
  }

};
