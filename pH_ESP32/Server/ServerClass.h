#include "ClientList.h"

class ServerPH {
private:
  WiFiServer *server;
  ClientList *clients;


  String *SSID, *PASS;

  bool openWifiStatus = false;
  bool displayDataTranfer = false;


public:

  ServerPH(String *_SSID, String *_PASS) {
    SSID = _SSID;
    PASS = _PASS;
    server = new WiFiServer(80);
    clients = new ClientList(server);
  }

  ~ServerPH() {
    delete clients;
  }

  void setup();
  void loop();
  void setDisplayDataTranfer(bool status);
  bool getDisplayDataTranfer();
  void send(String send_str);

  void setOnMessageListener(std::function<void(String)> callback) {
    clients->setOnMessageListener(callback);
  }

  void setOnClientJoinListener(std::function<void(String)> callback) {
    clients->setOnClientJoinListener(callback);
  }

  bool isClient();
  String getClientString();
};


void ServerPH::setDisplayDataTranfer(bool status){
  displayDataTranfer = status;
}

bool ServerPH::getDisplayDataTranfer(){
  return displayDataTranfer;
}

String ServerPH::getClientString(){
  return clients->getClientString();
}


void ServerPH::setup() {
  Serial.println("hospot : " + *SSID + "   " + *PASS);
  WiFi.softAP(SSID->c_str(), PASS->c_str());

  IPAddress IP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(IP);

  server->begin();


  openWifiStatus = true;
}
void ServerPH::loop() {
  clients->loop();
  clients->checkAvailable();
}
void ServerPH::send(String send_str) {
  clients->send(send_str);
  if(displayDataTranfer)Serial.println("ESP32 To Client ---> " + send_str);
}
