#include <EEPROM.h>

#define EEPROM_SIZE 512

#define ADDR_WORKTIMER 0
#define WORKTIMER_SIZE 19


#define ADDR_INTERNET_SSID 100
#define INTERNET_SSID_SIZE 30

#define ADDR_INTERNET_PASS 150
#define INTERNET_PASS_SIZE 30

#define ADDR_PHSPACERATE 200
#define PHSPACETATE_SIZE 9

#define ADDR_LIMITEACID 210
#define LIMITEACID_SIZE 9

#define ADDR_LIMITEBASE 220
#define LIMITEBASE_SIZE 9

#define ADDR_WAITSTRPUMP 230
#define WAITSTRPUMP_SIZE 9

#define ADDR_WAITPHSTABILIZE 240
#define WAITPHSTABILIZE_SIZE 9

#define ADDR_WAITBASEUSETIME 250
#define WAITBASEUSETIME_SIZE 9

#define ADDR_WAITACIDUSETIME 260
#define WAITACIDUSETIME_SIZE 9



class Database : public System {
private:
  void writeFunction(String DATA,int ADDR,int SIZE){
    if (!DATA.isEmpty()) {
      for (int i = 0; i < SIZE; i++) {
        if (i >= DATA.length()) {
          EEPROM.write(ADDR + i, 0);
          // Serial.print("e ");
        } else {
          EEPROM.write(ADDR + i, (int)DATA[i]);
          // Serial.print("w ");
        }
        // Serial.print(String((int)ssid[i]) + " ");
      }
      // Serial.println("");

      EEPROM.commit();
      // Serial.println("Commit");
    }
  }

  String readFunction(int ADDR,int SIZE){
    String ssid_str = "";
    for (int i = 0; i < SIZE; i++) {
      int char_int = EEPROM.read(ADDR + i);

      ssid_str += String((char)char_int);
      Serial.print(String(char_int) + " ");
    }
    Serial.println("");

    return ssid_str;
  }

public:
  Database() {
    
  }

  void setup() override {
    EEPROM.begin(EEPROM_SIZE);
    Serial.println("database constructor");
  }

  void loop() override {
  }

  void clearEEPROM(){
    for(int i = 0;i<512;i++)EEPROM.write(i,0);
  }

  void showEEPROM(){
    const String FRONLINE_GAP = "\t | \t\t";
    const String ITEM_HORIZONTAL_GAP = "\t";
    const int COLUMN = 10;
    int ROW = 0;

    Serial.print(FRONLINE_GAP);
    for (int i = 0;i<COLUMN;i++){
      Serial.print(String(i+1)+ITEM_HORIZONTAL_GAP);
    }
    Serial.println("");
    Serial.print(FRONLINE_GAP);
    Serial.println("----------------------------------------");
    
    Serial.print(String(ROW) + FRONLINE_GAP);

    for(int i = 0;i<EEPROM_SIZE;i++){
      int c_read = EEPROM.read(i);
      if(c_read == 0)Serial.print("?" + ITEM_HORIZONTAL_GAP);
      else Serial.print(String((char)c_read) + ITEM_HORIZONTAL_GAP);
      
      if(i % 10 == 0){
        Serial.println("");
        Serial.println(FRONLINE_GAP);
        Serial.print(String(++ROW) + FRONLINE_GAP);
      }
    }
  }

  

  void writeWorkTimer(int index,int hour,int minute,float pH,int T,bool active_status,bool delete_status) {

    String workTimerStr = String(hour) + "," + String(minute) + "," + String(pH) + "," + String(T) + "," + String(active_status) + "," + String(delete_status);
    
    writeFunction(workTimerStr,ADDR_WORKTIMER + (index * WORKTIMER_SIZE + 1), WORKTIMER_SIZE);
    
  }
  String readWorkTimer(int index) {
    String workTimerStrBuffer = readFunction(ADDR_WORKTIMER + (index * WORKTIMER_SIZE + 1), WORKTIMER_SIZE);
    return workTimerStrBuffer;
  }

  
  void writeInternetSSID(String ssid) {
    writeFunction(ssid, ADDR_INTERNET_SSID, INTERNET_SSID_SIZE);
  }
  void writeInternetPASS(String pass) {
    writeFunction(pass, ADDR_INTERNET_PASS, INTERNET_PASS_SIZE);
  }
  void writePSR(String pH_space_rate){
    writeFunction(pH_space_rate,ADDR_PHSPACERATE,PHSPACETATE_SIZE);
  }
  void writeLimiteBase(String limite_use_base){
    writeFunction(limite_use_base,ADDR_LIMITEBASE,LIMITEBASE_SIZE);
  }
  void writeLimiteAcid(String limite_use_acid){
    writeFunction(limite_use_acid,ADDR_LIMITEACID,LIMITEACID_SIZE);
  }
  void writeWaitSTRPump(String wait_str_pump){
    writeFunction(wait_str_pump,ADDR_WAITSTRPUMP,WAITSTRPUMP_SIZE);
  }
  void writeWaitPHStabilize(String wait_pH_stabilize){
    writeFunction(wait_pH_stabilize, ADDR_WAITPHSTABILIZE, WAITPHSTABILIZE_SIZE);
  }
  void writeWaitBaseUseTime(String wait_baseUseTime){
    writeFunction(wait_baseUseTime, ADDR_WAITBASEUSETIME, WAITBASEUSETIME_SIZE);
  }
  void writeWaitAcidUseTime(String wait_acidUseTime){
    writeFunction(wait_acidUseTime, ADDR_WAITACIDUSETIME, WAITACIDUSETIME_SIZE);
  }

  

  String readInternetSSID() {
    return readFunction(ADDR_INTERNET_SSID, INTERNET_SSID_SIZE);
  }
  String readInternetPASS() {
    return readFunction(ADDR_INTERNET_PASS, INTERNET_PASS_SIZE);
  }
  String readPSR(){
   return readFunction(ADDR_PHSPACERATE, PHSPACETATE_SIZE);
  }
  String readLimiteBase(){
    return readFunction(ADDR_LIMITEBASE, LIMITEBASE_SIZE);
  }
  String readLimiteAcid(){
    return readFunction(ADDR_LIMITEACID, LIMITEACID_SIZE);
  }
  String readWaitSTRPump(){
    return readFunction(ADDR_WAITSTRPUMP, WAITSTRPUMP_SIZE);
  }
  String readWaitPHStabilize(){
    return readFunction(ADDR_WAITPHSTABILIZE, WAITPHSTABILIZE_SIZE);
  }
  String readWaitBaseUseTime(){
    return readFunction(ADDR_WAITBASEUSETIME, WAITBASEUSETIME_SIZE);
  }
  String readWaitAcidUseTime(){
    return readFunction(ADDR_WAITACIDUSETIME, WAITACIDUSETIME_SIZE);
  }
};
