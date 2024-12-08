#include "Server.h"

#include "WiFiClient.h"
#include "WiFiServer.h"
#include <WiFi.h>
#include <functional>



class WiFiControll {
private:
  Variable *var;
  WiFiServer *server;

  String AP_SSID = "Star-Link_";
  String AP_PASS = "12345678star";

  bool displayDataTranfer = false;
  bool wifiConnected = false;


  void APMode_Start();
  void STAMode_Start();


public:

  WiFiControll(Variable *var) {
    server = new WiFiServer(80);
    this->var = var;
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
    return WiFi.status() == WL_CONNECTED;
  }

  void connectWiFi(){
    STAMode_Start();
    
  }
};

void WiFiControll::setup() {

  WiFi.mode(WIFI_AP_STA);

  APMode_Start();
  // STAMode_Start();
}
void WiFiControll::loop() {
  var->wifipublic.connect_status = getSTAWifiStatusConnected();
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
  WiFi.begin(var->wifipublic.SSID.c_str(),var->wifipublic.PASS.c_str());  
}
