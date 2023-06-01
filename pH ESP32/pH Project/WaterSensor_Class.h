#define POWER_PIN 17   // ESP32 pin GIOP17 connected to sensor's VCC pin
#define SIGNAL_PIN 35  // ESP32 pin GIOP36 (ADC0) connected to sensor's signal pin


class WaterSensor {
private:
  float value;
  Timer timer1;


public:
  WaterSensor()
    : timer1(1000) {
    value = 0;
  }

  void setup();
  void loop();

  float getValue();
};


void WaterSensor::setup() {
  pinMode(POWER_PIN, OUTPUT);    // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW);  // turn the sensor OFF
}


void WaterSensor::loop() {

  if (timer1.isExpired()) {
    digitalWrite(POWER_PIN, HIGH);   // turn the sensor ON
    value = analogRead(SIGNAL_PIN);  // read the analog value from sensor
    digitalWrite(POWER_PIN, LOW);    // turn the sensor OFF

    // Serial.print("The water sensor value: ");
    // Serial.println(value);
  }
}


float WaterSensor::getValue() {
  return value;
}
