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
    String gsheet_hostname = "https://script.google.com/macros/s/AKfycbwa2m6GroNpSCca4XJ9Qj9R9prn5H8ucxe8opHCYRJPgziR6_ViDEQXvUPJxpnkH2ui/exec";


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
    CouldComunity(Variable *var,WiFiControll *wifiC) : timer_senddata(10000),timer_reconnect_wifi(15000){
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

    void sendPH_All(String pHNeed,String pHBaseUsing,String pHAcidUsing,String pHSensorTime,String timeAdjustAll,String temp = "-1"){

        if(wifiC->getSTAWifiStatusConnected()){
             String serverPath = gsheet_hostname + "?&header=\"pHAll\"&p1=" + pHNeed + "&p2=" + pHBaseUsing + "&p3=" + pHAcidUsing + "&p4=" + pHSensorTime + "&p5=" + timeAdjustAll + "&p6=" + temp;
            httpClient.begin(serverPath);
            httpClient.setTimeout(5000);
            httpClient.GET();
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