// #include "HardwareSerial.h"

#include "ClientList.h"


class ServerPH {
private:
  WiFiServer *server;
  ClientList *clients;
  AsyncWebServer *aws;


  String *SSID, *PASS;

  bool openWifiStatus = false;

  std::function<void(String)> onMessageCallback;


public:

  ServerPH(String *_SSID, String *_PASS) {
    SSID = _SSID;
    PASS = _PASS;
    server = new WiFiServer(80);
    clients = new ClientList(server);
    aws = new AsyncWebServer(5000);
  }

  ~ServerPH() {
    delete clients;
  }

  void setup();
  void loop();

  void send(String send_str);

  void setOnMessageListener(std::function<void(String)> callback) {
    onMessageCallback = callback;
  }

  bool isClient();
  String getClientString();
};

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

  aws->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // request->send(200, "text/plain", "Hello, ESP32!");
    File file = SD.open("/index.html");
    if (file) {
      String content = file.readString();
      file.close();
      request->send(200, "text/html", content);
    } else {
      request->send(404);
    }
  });

  aws->begin();

  openWifiStatus = true;
}
void ServerPH::loop() {
  clients->loop();
  clients->checkAvailable(onMessageCallback);
}
void ServerPH::send(String send_str) {
  clients->send(send_str);
}
