#include "VarObject/TimerAutoWork.h"

#include "HardwareIO.h"
#include "Database.h"

typedef struct WorkVarStruct {
  int step = 0;
  bool working_status = false;
  bool working_status_setup = true;
  bool working_step_setup = false;
  String outputText1 = "";
  String outputText2 = "";
  String outputText3 = "";

} WorkVar;


typedef struct DebugStatusStruct{
  bool debug_wifiConnection = false;
  bool debug_client_comunity = false;
  bool debug_could_comunity = false;
  bool debug_arduino_comunity = false;
} DebugData;

class Variable : public System{

public:
  ScanI2C i2cScan;
  DebugData datadebug;
  HardwareIO *hardwareIO = new HardwareIO();
  Database *db = new Database();

  float input_ph = 0;
  float mixTank_pH = 0;
  float tempC = 0;
  float humidity = 0;

  bool fsw_mixTank_Up = false;
  bool fsw_mixtank_Down = false;

  WorkVar workVar;

  TimerAutoWork *timerautowork = new TimerAutoWork[4];

  void setup() override {
    hardwareIO->setup();
    db->setup();
  }

  void loop() override {
    hardwareIO->loop();
    db->loop();
  }

};
