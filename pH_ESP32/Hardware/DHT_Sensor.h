#include "DHT.h"

#define DHTPIN 33

#define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

class DHTSensor
{
private:
    Timer t_update_temp;
    float h = 0;
    float c = 0;
    float f = 0;

    bool status_ready = false;

public:
    DHTSensor() : t_update_temp(1000)
    {
    }

    void setup();
    void loop();

    float getHumidity();
    float getTempC();
    bool isReady();
    void calculator();
};


void DHTSensor::calculator(){
    h = dht.readHumidity();
    c = dht.readTemperature();
    f = dht.readTemperature(true);

    if (isnan(h) || isnan(c) || isnan(f))
    {
        status_ready = false;
        // Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(c, h, false);
    status_ready = true;
}


bool DHTSensor::isReady(){
    return status_ready;
}

float DHTSensor::getHumidity(){
    return h;
}

float DHTSensor::getTempC(){
    return c;
}

void DHTSensor::setup()
{
    dht.begin();
    calculator();
}

void DHTSensor::loop()
{
    if (t_update_temp.isExpired())
    {
        calculator();
    }
}