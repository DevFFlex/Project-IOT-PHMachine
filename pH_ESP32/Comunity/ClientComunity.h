#include "Server/ClientList.h"



typedef struct InterfaceEvent_ClientComunity{
  std::function<void(void)> onTimeUpdateAPP = NULL;
  std::function<void(String)> onClientMessageText = NULL;
  std::function<void(String)> onClientAdjPHStart = NULL;
  std::function<void(String)> onClientSetTimeAutoWork = NULL;
  std::function<void(String)> onClientGetTimeAutoWork = NULL;
  std::function<void(String)> onClientSetTimeBoard = NULL;
  std::function<void(String)> onClientRelayControll = NULL;
  std::function<void(String)> onClientSerialAvailable = NULL;


}ClientComunityCallback;



class ClientComunity
{
private:
    Variable *var;

    Timer autosendTimer;
    ClientList *clientlist;

  

    bool condition_req(String clientDataReq,String target);

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
      clientlist->checkAvailable();
      if (autosendTimer.isExpired())
      {
          if(clientComunityCallback.onTimeUpdateAPP != NULL)clientComunityCallback.onTimeUpdateAPP();
      }
    }

    //------------ send to client function ------------------

    void sendFileDir(String path);
    void sendOutput(String output_str);
    void sendStepText(String stepText);
    void sendUpdateApp(String queryString);
    void sendTimeAutoWork(String queryString);

    void H_MessageProcess(String command_arg, String value);

    String getClientString(){
      return clientlist->getClientString();
    }    
};


bool ClientComunity::condition_req(String clientDataReq,String target){
  return (clientDataReq.indexOf(target) != -1);
}


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

  if(var->datadebug.debug_client_comunity)Serial.println(str_clientname + ":" + str_command + ":" + str_value);

  if (condition_req(str_command,"MESSAGE"))
  {
    if(clientComunityCallback.onClientMessageText != NULL)clientComunityCallback.onClientMessageText(str_value);
  }

  if (condition_req(str_command,"RELAY"))
    if(clientComunityCallback.onClientRelayControll != NULL)clientComunityCallback.onClientRelayControll(str_value);

  if(condition_req(str_command,"ADJ_PH"))
    if(clientComunityCallback.onClientAdjPHStart != NULL)clientComunityCallback.onClientAdjPHStart(str_value);

  if(condition_req(str_command,"TIME_BOARD"))
    if(clientComunityCallback.onClientSetTimeBoard != NULL)clientComunityCallback.onClientSetTimeBoard(str_value);

  if(condition_req(str_command,"SET_TIME_AUTO_WORK"))
    if(clientComunityCallback.onClientSetTimeBoard != NULL)clientComunityCallback.onClientSetTimeAutoWork(str_value);

  if(condition_req(str_command,"GET_TIME_AUTO_WORK"))
    if(clientComunityCallback.onClientGetTimeAutoWork != NULL)clientComunityCallback.onClientGetTimeAutoWork(str_value);
  
  if(condition_req(str_command,"SERIAL"))
    if(clientComunityCallback.onClientSerialAvailable != NULL)clientComunityCallback.onClientSerialAvailable(str_value);
  

}

void ClientComunity::onClientJoinCallback(String clientName){
  Serial.println("Client '"+ clientName +"' ---------------- Join");
}


void ClientComunity::sendUpdateApp(String queryString){
  clientlist->send("SERVER:UPDATE=" + queryString);
}

void ClientComunity::sendFileDir(String encode_path)
{
  clientlist->send("SET:FILE_DIR=" + encode_path);
}

void ClientComunity::sendOutput(String output_text)
{
  clientlist->send("SERVER:OUTPUT=" + output_text);
}

void ClientComunity::sendStepText(String step_text){
  clientlist->send("SERVER:SETSTEPTEXT=" + step_text);
}

void ClientComunity::sendTimeAutoWork(String queryString){
  clientlist->send("SERVER:GET_TIME_AUTO_WORK_RES=" + queryString);
}

