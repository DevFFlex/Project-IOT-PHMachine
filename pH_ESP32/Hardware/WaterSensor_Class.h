#define POWER_PIN 33   // ESP32 pin GIOP17 connected to sensor's VCC pin
#define SIGNAL_PIN 35  // ESP32 pin GIOP36 (ADC0) connected to sensor's signal pin


class WaterSensor {
private:
  float value;
  Timer timer1;


public:
  WaterSensor()
    : timer1(50) {
    value = 0;
  }

  void setup();
  void loop();

  float getValue();
};


void WaterSensor::setup() {
}


void WaterSensor::loop() {

  if (timer1.isExpired()) {
    value = analogRead(SIGNAL_PIN);
    // Serial.print("The water sensor value: ");
    // Serial.println(value);
  }
}


float WaterSensor::getValue() {
  return value;
}
