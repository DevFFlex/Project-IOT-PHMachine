#include "Server.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include <WiFi.h>
#include <functional>
#include <SD.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncHTTPClient.h>




class WiFiControll {
private:
  WiFiServer *server;

  String AP_SSID = "PPC";
  String AP_PASS = "12345678";

  String STA_SSID = "MEE1";
  String STA_PASS = "43854953";

  bool displayDataTranfer = false;


  void APMode_Start();
  void STAMode_Start();


public:

  WiFiControll() {
    server = new WiFiServer(80);
  }

  ~WiFiControll() {
    delete server;
  }

  void setup();
  void loop();

  WiFiServer* getServer(){
    return server;
  }

  void setDisplayDataTranfer(bool status){
     displayDataTranfer = status;
  }

  bool getDisplayDataTranfer(){
    return displayDataTranfer;
  }


  bool getSTAWifiStatusConnected(){
    return WiFi.status() != WL_CONNECTED;
  }
};

void WiFiControll::setup() {

  WiFi.mode(WIFI_AP_STA);

  APMode_Start();
  STAMode_Start();
}
void WiFiControll::loop() {
  
}



void WiFiControll::APMode_Start(){
  Serial.println("hospot : " + AP_SSID + "   " + AP_PASS);
  WiFi.softAP(AP_SSID.c_str(), AP_PASS.c_str());
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  Serial.println("------------------------------------");

  server->begin();
}


void WiFiControll::STAMode_Start(){
  WiFi.begin(STA_SSID.c_str(),STA_PASS.c_str());
  while(WiFi.status() != WL_CONNECTED)
  {
        Serial.print(".");
        delay(100);
  }
  IPAddress STA_IP = WiFi.localIP();
  Serial.print("STA IP address: ");
  Serial.println(STA_IP);
  Serial.println("------------------------------------");

  

}
