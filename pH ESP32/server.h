#include <WiFi.h>

class ServerPH {
private:
  WiFiServer server;

  const char* ssid = "FFlex";
  const char* password = "123456789";
  WiFiClient client;
  bool first_client = true;
  bool openWifiStatus = false;

  void (*onMessageListener)(String) = NULL;


public:

  ServerPH() {
    server = WiFiServer(80);
  }

  void setup();
  void loop();

  void send(String send_str);
  void setOnMessageListener(void (*function)(String));

  bool isClient();
};


void ServerPH::setup() {
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();

  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();

  client = server.available();

  openWifiStatus = true;
}
void ServerPH::loop() {
  if (openWifiStatus) {
    if (!isClient()) client = server.available();
    else {

      if (first_client) {
        first_client = false;
        Serial.println("new client ip = " + client.localIP().toString());
      }

      if (isClient()) {

        if (client.available()) {
          String data = "";
          while (true) {
            char ccc = client.read();
            if (ccc == '$') break;
            else data += String(ccc);
          }
          // Serial.println(data);

          // String data = client.readString();

          data.trim();

          if (onMessageListener != NULL && data != "") {
            onMessageListener(data);
          }
        }


      } else {
        client.stop();
        Serial.println("client disconnected");
        first_client = true;
        client = NULL;
      }
    }
  }
}
void ServerPH::send(String send_str) {
  if (isClient()) {
    if (client.connected()) {
      String dataStr = send_str + "$";
      byte buffer_size = dataStr.length() + 1;
      char buffer[buffer_size];

      dataStr.toCharArray(buffer, buffer_size);
      client.write(buffer);
      client.flush();
    }
  }
}
bool ServerPH::isClient() {
  return (client) ? true : false;
}
void ServerPH::setOnMessageListener(void (*function)(String)) {
  onMessageListener = function;
}