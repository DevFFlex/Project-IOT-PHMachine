#include "DHT.h"

#define DHTPIN 33

#define DHTTYPE DHT11 // DHT 11
// #define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
// #define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

class DTHSensor
{
private:
    Timer t_update_temp;
    float h = 0;
    float c = 0;

public:
    DTHSensor() : t_update_temp(1000)
    {
    }

    void setup();
    void loop();

    float getHumidity();
    float getTempC();
};

float DTHSensor::getHumidity(){
    return h;
}

float DTHSensor::getTempC(){
    return c;
}

void DTHSensor::setup()
{
    dht.begin();
}

void DTHSensor::loop()
{
    if (t_update_temp.isExpired())
    {
        h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        c = dht.readTemperature();
        // Read temperature as Fahrenheit (isFahrenheit = true)
        float f = dht.readTemperature(true);

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(c) || isnan(f))
        {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        // Compute heat index in Fahrenheit (the default)
        float hif = dht.computeHeatIndex(f, h);
        // Compute heat index in Celsius (isFahreheit = false)
        float hic = dht.computeHeatIndex(c, h, false);

        // Serial.print(F("Humidity: "));
        // Serial.print(h);
        // Serial.print(F("%  Temperature: "));
        // Serial.print(t);
        // Serial.print(F("°C "));
        // Serial.print(f);
        // Serial.print(F("°F  Heat index: "));
        // Serial.print(hic);
        // Serial.print(F("°C "));
        // Serial.print(hif);
        // Serial.println(F("°F"));
    }
}