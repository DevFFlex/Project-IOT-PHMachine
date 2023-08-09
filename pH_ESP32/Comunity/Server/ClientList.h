#define CLIENT_LIMITE 4
#define CLIENT_TIMEOUT 3000

typedef struct clientObject_struct
{
  WiFiClient client;
  String name;
  String ip;
  int alive;
} ClientObject;

class ClientList
{
private:
  Timer timer2;

  WiFiServer *server;
  ClientObject clients[CLIENT_LIMITE];
  WiFiClient client;
  const int client_limite = CLIENT_LIMITE;
  const int _timeout = CLIENT_TIMEOUT;

  std::function<void(String)> onMessageCallback = NULL;
  std::function<void(String)> onClientJoinCallback = NULL;

public:

  ClientList(WiFiServer *serverIn): timer2(1000)
  {
    server = serverIn;
  }

  void loop();

  String getClientString();
  void add(WiFiClient *client, String IPAddress);
  void send(String send_str);
  void checkAvailable();
  void checkConnected();

  void setOnMessageListener(std::function<void(String)> callback);
  void setOnClientJoinListener(std::function<void(String)> callback);
};


void ClientList::setOnMessageListener(std::function<void(String)> callback) {
    onMessageCallback = callback;
}

void ClientList::setOnClientJoinListener(std::function<void(String)> callback){
    onClientJoinCallback = callback;
}

void ClientList::loop()
{
  client = server->available();
  if (client)
  {
    add(&client, client.localIP().toString());
  }
  checkConnected();
}

void ClientList::add(WiFiClient *client, String IPAddress){
  for (int cursor = 0; cursor < client_limite; cursor++)
    {
      if (!clients[cursor].client)
      {
        String name = "";
        clients[cursor].client = *client;

        if (clients[cursor].client.available())
        {
          String data = "";
          while (true)
          {
            char ccc = clients[cursor].client.read();
            delay(10);
            if (ccc == '$')
              break;
            else
              data += String(ccc);
          }
          data.trim();

          if (data.indexOf("SET:CLIENT_NAME=") != -1)
          {
            data.replace("SET:CLIENT_NAME=", "");
            name = data;

            if(data == "")name = "UNKNOW" + String(cursor);
          }

          
        }

        clients[cursor].client.setTimeout(_timeout);
        clients[cursor].name = name;
        clients[cursor].ip = IPAddress;
        clients[cursor].alive = 1;

        Serial.println("new device connect --- IP: " + IPAddress + "   NAME: " + name);

        if (onClientJoinCallback != NULL)
          onClientJoinCallback("hello");

        *client = NULL;
        break;
      }
    }
}

void ClientList::send(String send_str){
  for (int i = 0; i < client_limite; i++)
    {
      if (clients[i].client)
      {
        if (clients[i].client.connected())
        {
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

void ClientList::checkAvailable(){
  for (int i = 0; i < client_limite; i++)
    {
      if (clients[i].client.available() && clients[i].client)
      {
        String data = "";
        while (true)
        {
          char ccc = clients[i].client.read();
          if (ccc == '$')
            break;
          else
            data += String(ccc);
        }
        data.trim();

        if (data.indexOf("SET:CLIENT_NAME=") != -1)
        {
          data.replace("SET:CLIENT_NAME=", "");
          clients[i].name = data;
          return;
        }

        if (onMessageCallback != NULL && data != "")
        {
          onMessageCallback(data);
        }
      }
    }
}

void ClientList::checkConnected(){
  if (timer2.isExpired())
    {
      for (int i = 0; i < client_limite; i++)
      {
        if (clients[i].alive)
        {
          if (!clients[i].client)
          {
            clients[i].alive = 0;
            Serial.println(clients[i].name + " : disconnected");
            clients[i].name = "";
            clients[i].ip = "";
          }
        }
      }
    }
}

String ClientList::getClientString(){
  int count = 0;
    String namelist = "";
    for (int i = 0; i < CLIENT_LIMITE; i++)
    {
      if (clients[i].client)
      {
        count++;
        namelist += clients[i].name + ",";
      }
    }
    String out = "" + String(count) + "|" + namelist;
    out = out.substring(0, out.lastIndexOf(","));
    return out;
}

