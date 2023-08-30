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
  float pH_space_rate = 0.5;

  int T = 0;
  int addBaseCount = 0;
  int addAcidCount = 0;
  float useTime = 0;
  String validity_status = "";

  void resetData(){
    step = 0;
    working_status = false;
    working_status_setup = true;

    T = 0;
    addBaseCount = 0;
    addAcidCount = 0;
    useTime = 0;
    validity_status = "";
  }


} WorkVar;


typedef struct WifiPublicStruct{
  String SSID = "";
  String PASS = "";

  void show(){
    Serial.println("SSID = " + String(SSID));
    Serial.println("PASS = " + String(PASS));
  }
}WifiPublic;


typedef struct FloatSwitchStruct{
  bool mixTank_Up = false;
  bool mixTank_Down = false;

  void show(){
    Serial.println("fsw.mixTank_Up = " + String(mixTank_Up));
    Serial.println("fsw.mixTank_Down = " + String(mixTank_Down));
  }
}FloatSwitchVal;


typedef struct DebugStatusStruct{
  bool debug_wifiConnection = false;
  bool debug_client_comunity = false;
  bool debug_could_comunity = false;
  bool debug_arduino_comunity = false;
  void show(){
    Serial.println("debug_wifiConnection = " + String(debug_wifiConnection));
    Serial.println("debug_client_comunity = " + String(debug_client_comunity));
    Serial.println("debug_could_comunity = " + String(debug_could_comunity));
    Serial.println("debug_arduino_comunity = " + String(debug_arduino_comunity));
  }
} DebugData;

class Variable : public System{

public:
  ScanI2C i2cScan;
  
  HardwareIO *hardwareIO = new HardwareIO();
  Database *db = new Database();

  float input_ph = 0;
  float mixTank_pH = 0;
  float tempC = 0;
  float humidity = 0;

  FloatSwitchVal fsw;
  WorkVar workVar;
  WifiPublic wifipublic;
  DebugData datadebug;

  TimerAutoWork *timerautowork = new TimerAutoWork[4];

  void setup() override {
    hardwareIO->setup();
    db->setup();
  }

  void loop() override {
    hardwareIO->loop();
    db->loop();
  }

  void showVar(){
    Serial.println("---------------------------------------");
    Serial.println("input_ph = " + String(input_ph));
    Serial.println("mixTank_pH = " + String(mixTank_pH));
    Serial.println("tempC = " + String(tempC));
    Serial.println("humidity = " + String(humidity));
    fsw.show();
    datadebug.show();
    wifipublic.show();
    Serial.println("---------------------------------------");
  }

};
