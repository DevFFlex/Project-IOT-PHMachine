#include <HTTPClient.h>


typedef struct InterfaceEvent_CouldComunity{
  std::function<void(void)> onWifiConnect = NULL;
  std::function<void(void)> onWifiDisconnect = NULL;
}CouldControllCallback;

class CouldComunity{
    private:
    Variable *var;

    WiFiControll *wifiC;
    HTTPClient  httpClient;
    String gsheet_hostname = "https://script.google.com/macros/s/AKfycbw3nnrGL_atUT3M80KyVXajTiDQbgJd2CY9JQu-g6H2cFefiAVmI2thYtT3MS-qjZD4/exec?";


    Timer timer_senddata;
    Timer timer_reconnect_wifi;

    void autoReconnect(){
        if(timer_reconnect_wifi.isExpired()){
            if(!wifiC->getSTAWifiStatusConnected()){
                wifiC->connectWiFi();
                if(var->datadebug.debug_could_comunity)Serial.println("reconnect wifi");
            }else{
                if(var->datadebug.debug_could_comunity)Serial.println("wifi is connected");
            }
        }
    }

    public:
    CouldComunity(Variable *var,WiFiControll *wifiC) : timer_senddata(10000),timer_reconnect_wifi(10000){
        this->var = var;
        this->wifiC = wifiC;
    }

    void setup(){
    }

    void loop(){

        static int count = 0;
        if(timer_senddata.isExpired()){
            String pH = String(count * 1.2);
            String temp = String(count * 3.2);
            String serverPath = gsheet_hostname + "?&ph=" + pH + "&temp=" + temp;

            count++;        
        } 

        autoReconnect();
    }

    void sendPH_All(String pHNeed,String pHMixtankFirstLast,String pHBaseUsing,String amountBase,String pHAcidUsing,String amountAcid,String pHSensorTime,String timeAdjustAll,String temp,String validity_status){

        if(wifiC->getSTAWifiStatusConnected()){
             String serverPath = gsheet_hostname + "header=pHAll&p1=" + pHNeed + "&p2=" + pHMixtankFirstLast + "&p3=" + pHBaseUsing + "&p4=" + amountBase + "&p5=" + pHAcidUsing + "&p6=" + amountAcid + "&p7=" + pHSensorTime + "&p8=" + timeAdjustAll + "&p9=" + temp + "&p10=" + validity_status;
            httpClient.begin(serverPath);
            httpClient.setTimeout(5000);
            httpClient.GET();
            Serial.println("sending to cloud");
        }
    }

    void sendPH_Adjust(String t,String t_count,String discharge,String discharge_time,String before_ph,String after_ph){
        if(wifiC->getSTAWifiStatusConnected()){
             String serverPath = gsheet_hostname + "?&header=pHAdjust&p1=" + t + "&p2=" + t_count + "&p3=" + discharge + "&p4=" + discharge_time + "&p5=" + before_ph + "&p6=" + after_ph;
            httpClient.begin(serverPath);
            httpClient.setTimeout(5000);
            httpClient.GET();
        }
    }
};