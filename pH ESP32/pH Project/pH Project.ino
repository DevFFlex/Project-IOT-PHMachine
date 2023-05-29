
#include "ExClass.h"
#include "F_Timer.h"
#include "controllerFunction.h"

#include "server.h"
#include "VarObject.h"
#include "HardwareIO.h"

#include "Comunity.h"
#include "ArduinoComunity.h"




int step = 1;

//------------- Class ------------------------

ServerPH *serverPH = new ServerPH();
VarObject *varObject = new VarObject();
HardwareIO *hardwareIO = new HardwareIO();

Comunity *comunity = new Comunity(serverPH,varObject,hardwareIO);

ArduinoComunity *ardunoComunity = new ArduinoComunity();

Timer changeMixtankTimer(1000);
Timer t1(1000);
Timer timerActiveRelay(5000);

ScanI2C scanI2C;

//-------------------------- IO ---------------------------
void onKeypass(char c, String textNow) {
  if (c == 'C') {
    hardwareIO->keypadInput->clearTextNow();
    hardwareIO->lcdOutput->printL(1,"INPUT : ", 1);
    return;
  }

  hardwareIO->lcdOutput->printL(1,"INPUT : " + textNow, 1);
}

void onEnterkey(String text) {
  hardwareIO->lcdOutput->printL(1,"Val = " + text, 2);
  serverPH->send("S_RESP_MTPH=" + String(text));
  varObject->setInputPH(text.toFloat());

}


void onClientMessage(String str_trim) {

  String databox1[2];
  split(databox1, str_trim, ":", 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  split(databox2, commands, "=", 2);
  String command = databox2[0];
  String value = databox2[1];

  Serial.print("str_trim = " + str_trim + "       ");
  Serial.print("header = " + header + "   commands = " + commands + "       ");
  Serial.println("command = " + command + "   value = " + value);


  if (header == "SET") {

    if (command == "TIME_BOARD")comunity->recvTimelist(value);
    if (command == "INPUT_PH")comunity->recvInputPH(value);
    if (command == "TIME_LIST")varObject->setTimeList(value);


  } else if (header == "GET") {

    if (commands == "INPUT_PH") comunity->sendInputPH();
    if (commands == "MIXTANK_PH") comunity->sendMixTankPH();
    if (commands == "USETANK_PH") comunity->sendUseTankPH();
    if (commands == "TIME_LIST") comunity->sendTimeList();
  }

  showall();
}

void showall() {
  Serial.println("-------------------Timelist----------------------");
  Serial.println(varObject->getTimeListToString());

  Serial.println("-------------------Timerlist----------------------");
  for(int i = 0;i<4;i++){
    Serial.println(varObject->timerlist[i].toString());
  }
  
}


void InputSerial() {

  if (Serial.available() > 0) {
    String data = Serial.readString();
    data.trim();

    if (data != "") {
      if (data.indexOf("t") != -1) {
        data.replace("t", "");

        Serial.print("set time");
      }

      if (data.indexOf("d")!= -1){
        
      }

    }
  }
}

//---------------------------------------------- setup loop -----------------------------------

void setup() {
  Serial.begin(115200);
  // Serial.setTimeout(50);

  serverPH->setup();
  serverPH->setOnMessageListener(onClientMessage);

  hardwareIO->setup();

  hardwareIO->keypadInput->setOnKeypressListener(onKeypass);
  hardwareIO->keypadInput->setOnKeyEnterListener(onEnterkey);

  hardwareIO->pHSensor->setup();


 
  // scanI2C.setup();
  ardunoComunity->setup();
}

void loop() {
  ardunoComunity->loop();

  InputSerial();
  serverPH->loop();
  
  hardwareIO->keypadInput->loop();
  hardwareIO->lcdOutput->loop();
  hardwareIO->rtc->loop();
  hardwareIO->pHSensor->loop();


  if (t1.isExpired()) {
    varObject->setMixTankpH(hardwareIO->pHSensor->getPH());
    comunity->sendMixTankPH();
    hardwareIO->lcdOutput->printL(1, "PH = " + String(hardwareIO->pHSensor->getPH()), 0);
    // hardwareIO->lcdOutput->printL(1,"S2 = " + ardunoComunity->text, 1);
    // hardwareIO->lcdOutput->printL(1,"TimeList = " + String(varObject->getTimeListSize()), 2);
    hardwareIO->lcdOutput->printL(1,hardwareIO->rtc->getTimeToString(), 3);
  }

  timerOpenRelayLoop();

  
  
}





bool timelist_compare_rtctime(int index){
  byte active_hour = varObject->timerlist[index].getHour();
  byte active_minute = varObject->timerlist[index].getMinute();
  byte active_second = varObject->timerlist[index].getSecond();

  if (active_hour == -1)return false;

  return (hardwareIO->rtc->getHour() == active_hour && 
  hardwareIO->rtc->getMinute() == active_minute && 
  hardwareIO->rtc->getSecond() == active_second );
}

void timerOpenRelayLoop(){
  
  static bool active_finish = false;
  static int offdelay = 30000;

  if ((timelist_compare_rtctime(0) || timelist_compare_rtctime(1) || timelist_compare_rtctime(2) || timelist_compare_rtctime(3)) && !active_finish) {

    hardwareIO->relay->off(0);
    active_finish = true;
    timerActiveRelay.reset();
    timerActiveRelay.setInterval(offdelay);
    Serial.println("is Time");

  }

  if (active_finish) {

    if (timerActiveRelay.isExpired()) {
      hardwareIO->relay->on(0);
      Serial.println("Active = false");
      active_finish = false;
    }

  }
}

//---------------------------------------------------------------------------------------------


/*
#define SensorPin 34            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
unsigned long int avgValue;     //Store the average value of the sensor feedback

void setup(){
  pinMode(13,OUTPUT); 
  Serial.begin(115200); 
}


void loop(){
  int buf[10];                //buffer for read analog
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  {
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        int temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*3.3/4096/6; //convert the analog into millivolt
  phValue=3.5*phValue+Offset;                      //convert the millivolt into pH value
  Serial.print("pH :"); 
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);      
  delay(800);
  digitalWrite(13, LOW);
}*/
