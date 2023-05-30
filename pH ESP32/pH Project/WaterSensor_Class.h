#define POWER_PIN 17   // ESP32 pin GIOP17 connected to sensor's VCC pin
#define SIGNAL_PIN 35  // ESP32 pin GIOP36 (ADC0) connected to sensor's signal pin


class WaterSensor {
private:
  int value;


public:
  WaterSensor() {
    value = 0;
  }

  void setup();
  void loop();
};


void WaterSensor::setup() {
  pinMode(POWER_PIN, OUTPUT);    // configure pin as an OUTPUT
  digitalWrite(POWER_PIN, LOW);  // turn the sensor OFF
}


void WaterSensor::loop() {
  digitalWrite(POWER_PIN, HIGH);   // turn the sensor ON
  delay(10);                       // wait 10 milliseconds
  value = analogRead(SIGNAL_PIN);  // read the analog value from sensor
  digitalWrite(POWER_PIN, LOW);    // turn the sensor OFF

  Serial.print("The water sensor value: ");
  Serial.println(value);

  delay(1000);
}
