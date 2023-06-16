#include "HardwareSerial.h"
#include "WiFiClient.h"
#include "WiFiServer.h"
#include <WiFi.h>
#include <functional>

#define CLIENT_LIMITE 4

typedef struct clientObject_struct {
  WiFiClient client;
  String name;
  String ip;
  int alive;
} ClientObject;

class ClientList {
private:
  Timer time1, timer2;

  WiFiServer *server;
  ClientObject clients[CLIENT_LIMITE];
  WiFiClient client;
  const int client_limite = CLIENT_LIMITE;
  const int _timeout = 3000;

  void display() {
    if (time1.isExpired()) {
      Serial.println('---------client list----------');
      for (int i = 0; i < client_limite; i++) {
        Serial.println("client [" + String(i) + "] = " + String(clients[i].client) + "   name = " + clients[i].name + "     ip = " + clients[i].ip);
      }
    }
  }

public:
  ClientList(WiFiServer *serverIn)
    : time1(1000), timer2(1000) {
    server = serverIn;
  }

  void loop() {
    client = server->available();
    if (client) {
      add(&client, client.localIP().toString());
    }

    // display();

    checkConnected();
  }


  void add(WiFiClient *client, String IPAddress) {
    for (int cursor = 0; cursor < client_limite; cursor++) {
      if (!clients[cursor].client) {
        String name = "";
        clients[cursor].client = *client;

        if (clients[cursor].client.available()) {
          String data = "";
          while (true) {
            char ccc = clients[cursor].client.read();
            delay(10);
            if (ccc == '$') break;
            else data += String(ccc);
          }
          data.trim();

          if (data.indexOf("SET:CLIENT_NAME=") != -1) {
            data.replace("SET:CLIENT_NAME=", "");
            name = data;
          }
        }


        clients[cursor].client.setTimeout(_timeout);
        clients[cursor].name = name;
        clients[cursor].ip = IPAddress;
        clients[cursor].alive = 1;

        Serial.println("new device connect --- IP: " + IPAddress + "   NAME: " + name);

        *client = NULL;
        break;
      }
    }
  }

  void send(String send_str) {
    for (int i = 0; i < client_limite; i++) {
      if (clients[i].client) {
        if (clients[i].client.connected()) {
          String dataStr = send_str + "$";
          byte buffer_size = dataStr.length() + 1;
          char buffer[buffer_size];


          dataStr.toCharArray(buffer, buffer_size);
          clients[i].client.write(buffer);
          clients[i].client.flush();
          Serial.println("send to " + clients[i].name + "    :   " + send_str);
        }
      }
    }
  }

  void checkAvailable(std::function<void(String)> callback = NULL) {
    for (int i = 0; i < client_limite; i++) {
      if (clients[i].client.available() && clients[i].client) {
        String data = "";
        while (true) {
          char ccc = clients[i].client.read();
          if (ccc == '$') break;
          else data += String(ccc);
        }
        data.trim();

        if(data.indexOf("SET:CLIENT_NAME=") != -1){
          data.replace("SET:CLIENT_NAME=","");
          clients[i].name = data;
          return;
        }

        if (callback != NULL && data != "") {
          callback(data);
        }
      }
    }
  }
  void checkConnected() {
    if (timer2.isExpired()) {
      for (int i = 0; i < client_limite; i++) {
        if (clients[i].alive) {
          if (!clients[i].client) {
            clients[i].alive = 0;
            Serial.println(clients[i].name + " : disconnected");
            clients[i].name = "";
            clients[i].ip = "";
          }
        }
      }
    }
  }
};

class ServerPH {
private:
  WiFiServer *server;
  ClientList *clients;


  String *SSID,*PASS;

  bool openWifiStatus = false;

  std::function<void(String)> onMessageCallback;


public:

  ServerPH(String *_SSID,String *_PASS) {
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

  void send(String send_str);

  void setOnMessageListener(std::function<void(String)> callback) {
    onMessageCallback = callback;
  }

  bool isClient();
};


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
  clients->checkAvailable(onMessageCallback);
}
void ServerPH::send(String send_str) {
  clients->send(send_str);
}
