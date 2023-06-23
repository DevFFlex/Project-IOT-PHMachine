#include "WiFiClient.h"
#include "WiFiServer.h"
#include <WiFi.h>
#include <functional>
#include <SD.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define CLIENT_LIMITE 4

typedef struct clientObject_struct {
  WiFiClient client;
  String name;
  String ip;
  int alive;
} ClientObject;

class ClientList {
private:
  Timer timer2;

  WiFiServer *server;
  ClientObject clients[CLIENT_LIMITE];
  WiFiClient client;
  const int client_limite = CLIENT_LIMITE;
  const int _timeout = 3000;


public:
  ClientList(WiFiServer *serverIn)
    : timer2(1000) {
    server = serverIn;
  }

  void loop() {
    client = server->available();
    if (client) {
      add(&client, client.localIP().toString());
    }
    checkConnected();
  }

  String getClientString(){
    int count = 0;
    String namelist = "";
    for (int i = 0;i<CLIENT_LIMITE;i++){
        if(clients[i].client){
            count++;
            namelist += clients[i].name + ",";
        }
    }
    String out = "" + String(count)+ "|"+ namelist;
    out = out.substring(0,out.lastIndexOf(","));
    return out;
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
          // if(dataStr.indexOf("SET:MIXTANK_PH=") == -1)Serial.println("send to " + clients[i].name + "    :   " + send_str);
          // Serial.println("send to " + clients[i].name + "    :   " + send_str);
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

        if (data.indexOf("SET:CLIENT_NAME=") != -1) {
          data.replace("SET:CLIENT_NAME=", "");
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