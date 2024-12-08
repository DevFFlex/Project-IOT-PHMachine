typedef struct WorkVarStruct {
  int step = 0;
  float adjustCurrentpH = 0;
  int   adjustT = 0;
  int   adjustT_Counter = 0;
  float pH_mixtankFirst = -1;
  bool working_status = false;
  bool working_status_setup = true;
  bool working_step_setup = true;
  String outputText1 = "";
  String outputText2 = "";
  String outputText3 = "";
  bool work_with_timer_status = false;

  //--------------------- static variable -----------------------
  float pH_space_rate = 0.5; //ระยะห่างของ Output ที่จะได้
  int limite_use_base = 10; //จำกัดการปล่อยสารเบส
  int limite_use_acid = 10; //จำกัดการปล่อยสารกรด
  int wait_stirring_pump = 10000; //เวลาในการรอปั้มกวนเสร็จ
  int wait_pH_stabilize = 20000; //เวลาในการรอ pH Sensor นิ่ง
  int wait_acidUseTime = 2000; //เวลาในการปล่อยสารกรด
  int wait_baseUseTime = 2000; //เวลาในการปล่อยสารเบส

  //--------------------- no static variable -----------------------
  int addBaseAcidCount = 0;
  int addBaseCount = 0;
  int addAcidCount = 0;
  float addBase_mL = 0;
  float addAcid_mL = 0;
  float useTime = 0;
  String validity_status = "";




  void resetData(){
    step = 0;
    working_status = false;
    working_status_setup = true;
    working_step_setup = true;

    pH_mixtankFirst = -1;
    addBaseAcidCount = 0;
    addBaseCount = 0;
    addAcidCount = 0;
    addBase_mL = 0;
    addAcid_mL = 0;
    useTime = 0;
    validity_status = "";
  }


  void startAdjustPH(float pH,int T){
    Serial.println("เริ่มปรับ");
    adjustCurrentpH = pH;
    adjustT = T;
    adjustT_Counter = T;
    working_status = true;
  }

  void stopAdjustPH(){
    Serial.println("หยุดปรับ");
    working_status = false;
    step = 0;


  }

  void show(){
    Serial.println("step = " + String(step));
    Serial.println("adjustCurrentpH = " + String(adjustCurrentpH));
    Serial.println("adjustT = " + String(adjustT));
    Serial.println("adjustT_Counter = " + String(adjustT_Counter));
    Serial.println("pH_mixtankFirst = " + String(pH_mixtankFirst));
    Serial.println("working_status = " + String(working_status));
    Serial.println("working_status_setup = " + String(working_status_setup));
    Serial.println("working_step_setup = " + String(working_step_setup));
    Serial.println("outputText1 = " + String(outputText1));
    Serial.println("pH_space_rate = " + String(pH_space_rate));
    Serial.println("limite_use_base = " + String(limite_use_base));
    Serial.println("limite_use_acid = " + String(limite_use_acid));
    Serial.println("wait_stirring_pump = " + String(wait_stirring_pump));
    Serial.println("wait_pH_stabilize = " + String(wait_pH_stabilize));
    Serial.println("wait_acidUseTime = " + String(wait_acidUseTime));
    Serial.println("wait_baseUseTime = " + String(wait_baseUseTime));
    Serial.println("addBaseAcidCount = " + String(addBaseAcidCount));
    Serial.println("addBaseCount = " + String(addBaseCount));
    Serial.println("addAcidCount = " + String(addAcidCount));
    Serial.println("amountBase = " + String(addBase_mL));
    Serial.println("amountAcid = " + String(addAcid_mL));
    Serial.println("useTime = " + String(useTime));
    Serial.println("validity_status = " + String(validity_status)); 
  }


} WorkVar;


typedef struct WifiPublicStruct{
  String SSID = "";
  String PASS = "";
  bool connect_status = false;

  bool isconnected(){
    return connect_status;
  }

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


typedef struct WorkTimerStruct{
  int HOUR = -1;
  int MINUTE = -1;
  float PH = -1;
  int T = -1;
  bool ACTIVE_STATUS = false;
  bool DELETE_STATUS = true;

  void show(){
    Serial.print(HOUR);Serial.print("\t");
    Serial.print(MINUTE);Serial.print("\t");
    Serial.print(PH);Serial.print("\t");
    Serial.print(T);Serial.print("\t");
    Serial.print(ACTIVE_STATUS);Serial.print("\t");
    Serial.println(DELETE_STATUS);
    
  }
}WorkTimer;


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


#include "HardwareIO.h"
#include "Database.h"

class Variable : public System{

public:
  ScanI2C i2cScan;
  
  HardwareIO *hardwareIO = new HardwareIO();
  Database *db = new Database();

  float mixTank_pH = 7;

  FloatSwitchVal fsw;
  WorkVar workVar;
  WifiPublic wifipublic;
  DebugData datadebug;
  WorkTimer workTimer[4];


  String serial_buffer = "";

  void setup() override {
    hardwareIO->setup();
    db->setup();

    setDefaultVariable();
  }

  void loop() override {
    hardwareIO->loop();
    db->loop();

  }

  void setDefaultVariable(){
    for(int i = 0;i<4;i++){
      String datareadstr = db->readWorkTimer(i);
      String databuffer[6];
      splitString(databuffer,datareadstr,",",6);
      workTimer[i].HOUR = databuffer[0].toInt();
      workTimer[i].MINUTE = databuffer[1].toInt();
      workTimer[i].PH = databuffer[2].toFloat();
      workTimer[i].T = databuffer[3].toInt();
      workTimer[i].ACTIVE_STATUS = (databuffer[4] == "1") ? true : false;
      workTimer[i].DELETE_STATUS = (databuffer[5] == "1") ? true : false;
    }


    wifipublic.SSID = db->readInternetSSID();
    wifipublic.PASS = db->readInternetPASS();


    String buffer = "";
    buffer = db->readPSR();
    workVar.pH_space_rate = (buffer != "") ? buffer.toFloat() : 0.0;

    buffer = db->readLimiteBase();
    workVar.limite_use_base = (buffer != "") ? buffer.toInt() : 0;

    buffer = db->readLimiteAcid();
    workVar.limite_use_acid = (buffer != "") ? buffer.toInt() : 0;

    buffer = db->readWaitSTRPump();
    workVar.wait_stirring_pump = (buffer != "") ? buffer.toInt() : 0;

    buffer = db->readWaitPHStabilize();
    workVar.wait_pH_stabilize = (buffer != "") ? buffer.toInt() : 0;

    buffer = db->readWaitBaseUseTime();
    workVar.wait_baseUseTime = (buffer != "") ? buffer.toInt() : 0;

    buffer = db->readWaitAcidUseTime();
    workVar.wait_acidUseTime = (buffer != "") ? buffer.toInt() : 0;



    
  }

  void showVar(){
    Serial.println("---------------------------------------");
    Serial.println("mixTank_pH = " + String(mixTank_pH));
    fsw.show();
    datadebug.show();
    wifipublic.show();
    workVar.show();
    Serial.println("---------------------------------------");
  }

};
