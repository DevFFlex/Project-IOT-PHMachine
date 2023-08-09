#include <HTTPClient.h>

class CouldComunity{
    WiFiControll *wifiC;
    HTTPClient  httpClient;
    String hostname = "https://script.google.com/macros/s/AKfycbwa2m6GroNpSCca4XJ9Qj9R9prn5H8ucxe8opHCYRJPgziR6_ViDEQXvUPJxpnkH2ui/exec";


    Timer timer_senddata;
    public:
    CouldComunity(WiFiControll *wifiC) : timer_senddata(10000){
        this->wifiC = wifiC;
    }


    void setup(){

    }

    

    void loop(){
        static int count = 0;
        if(timer_senddata.isExpired()){
            // Serial.println("onCouldSend");

            String pH = String(count * 1.2);
            String temp = String(count * 3.2);
            String serverPath = hostname + "?&ph=" + pH + "&temp=" + temp;

            count++;

            Serial.println(serverPath);
            httpClient.begin(serverPath);
            httpClient.setTimeout(5000);
            httpClient.GET();
            Serial.println("--------- get finish ---------");
            
        }
    
    
        
    }

};