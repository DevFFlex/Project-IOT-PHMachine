#include "Server/ClientList.h"

typedef struct InterfaceEvent_ClientComunity{
  std::function<void(void)> onTimeUpdateAPP = NULL;
  std::function<void(String)> onClientSerialAvailable = NULL;
  std::function<void(void)> onClientJoin = NULL;
}ClientComunityCallback;


class ClientComunity
{
private:
    Variable *var;

    Timer autosendTimer;
    ClientList *clientlist;

  

    bool condition_req(String clientDataReq,String target){
      return (clientDataReq.indexOf(target) != -1);
    }

public:
    ClientComunityCallback clientComunityCallback;

    bool debugDisplayDataRecv = false;

    void onClientMessageCallback(String message);
    void onClientJoinCallback(String clientName);

    ClientComunity(Variable *var,WiFiControll *wifiC) : autosendTimer(1000){
      clientlist = new ClientList(wifiC->getServer());
      this->var = var;
    }

    void setup()
    {
      clientlist->setOnClientJoinListener(std::bind(&ClientComunity::onClientJoinCallback, this, std::placeholders::_1));
      clientlist->setOnMessageListener(std::bind(&ClientComunity::onClientMessageCallback, this, std::placeholders::_1));
    }

    void loop()
    {
      clientlist->loop();
      //clientlist->checkAvailable();
      if (autosendTimer.isExpired())
      {
          if(clientComunityCallback.onTimeUpdateAPP != NULL)clientComunityCallback.onTimeUpdateAPP();
      }
    }

    //------------ send to client function ------------------

    void sendOutput(String output_str);
    void sendStepText(String stepText);
    void sendUpdateApp(String queryString);

    void H_MessageProcess(String command_arg, String value);

    String getClientString(){
      return clientlist->getClientString();
    }    
};





void ClientComunity::onClientMessageCallback(String str_trim)
{
  String databox1[2];
  splitString(databox1, str_trim, ":", 2);
  String header = databox1[0];
  String commands = databox1[1];

  String databox2[2];
  splitString(databox2, commands, "=", 2);
  String command = databox2[0];
  String value = databox2[1];

  String str_clientname = header;
  String str_command = command;
  String str_value = value;

  if(var->datadebug.debug_client_comunity)
    Serial.println(str_clientname + ":" + str_command + ":" + str_value);

  if((str_command,"SERIAL") && clientComunityCallback.onClientSerialAvailable != NULL)
    clientComunityCallback.onClientSerialAvailable(str_value);
  

}

void ClientComunity::onClientJoinCallback(String clientName){
  Serial.println("Client '"+ clientName +"' ---------------- Join");
  if(clientComunityCallback.onClientJoin != NULL)clientComunityCallback.onClientJoin();
}

void ClientComunity::sendUpdateApp(String queryString){
  clientlist->send("SERVER:UPDATE=" + queryString);
}
void ClientComunity::sendOutput(String output_text)
{
  clientlist->send("SERVER:OUTPUT=" + output_text);
}
