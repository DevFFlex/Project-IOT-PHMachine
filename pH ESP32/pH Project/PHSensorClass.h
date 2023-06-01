
#define SensorPin 34  //pH meter Analog output to Arduino Analog Input 0   //deviation compensate

class PHSensor {
private:
  unsigned long int avgValue;
  float ph,volt;
  Timer t_delayRead;

  float c_offset;
  float m_slope;

  void display();

public:

  PHSensor()
    : t_delayRead(1000) {
    ph = 0;
    volt = 0;
    c_offset = 21.34;
    m_slope = -1.0;
  }

  void setup();
  void loop();

  float getPH();
  float getVolt();

  void setOffset(float offsetIn);
  void setSlope(float slopeIn);
};


void PHSensor::setup() {
}


void PHSensor::loop() {

  if (t_delayRead.isExpired()) {
    int buf[10];                  //buffer for read analog
    for (int i = 0; i < 10; i++)  //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
    }

    // float phRead = analogRead(SensorPin);


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
    volt = (float)(avgValue * (3.3 / 4096)) / 6;  //convert the analog into millivolt
    ph = m_slope * volt + c_offset;

    // display();
  }
}

void PHSensor::display() {
  Serial.print("pH :");
  Serial.print(ph, 2);
  Serial.print(",");
  Serial.print("      c_offset = ");
  Serial.print(c_offset);
  Serial.print("      m_slope = ");
  Serial.println(m_slope);
}

float PHSensor::getPH() {
  return ph;
}


float PHSensor::getVolt(){
  return volt;
}


void PHSensor::setOffset(float offsetIn) {
  c_offset = offsetIn;
}


void PHSensor::setSlope(float slopeIn) {
  m_slope = slopeIn;
}