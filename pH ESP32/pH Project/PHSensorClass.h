
#define SensorPin 34  //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00   //deviation compensate

class PHSensor {
private:
  unsigned long int avgValue;
  Timer t_delayRead;

public:

  PHSensor()
    : t_delayRead(1000) {
  }

  void setup();
  void loop();
};


void PHSensor::setup() {
}


void PHSensor::loop() {

  if (t_delayRead.isExpired()) {
    int buf[10];                  //buffer for read analog
    for (int i = 0; i < 10; i++)  //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
      // delay(10);
    }


    for (int i = 0; i < 9; i++)  //sort the analog from small to large
    {
      for (int j = i + 1; j < 10; j++) {
        if (buf[i] > buf[j]) {
          int temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++)  //take the average value of 6 center sample
      avgValue += buf[i];
    float phValue = (float)avgValue * 3.3 / 4096 / 6;  //convert the analog into millivolt
    phValue = 3.5 * phValue + Offset;                  //convert the millivolt into pH value
    Serial.print("pH :");
    Serial.println(phValue, 2);
  }
}