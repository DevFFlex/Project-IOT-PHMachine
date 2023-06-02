
#define SensorPin 34  //pH meter Analog output to Arduino Analog Input 0   //deviation compensate

class PHSensor {
private:
  unsigned long int avgValue;
  float ph, volt, analogPH;
  Timer t_delayRead;

  float calibration_value;
  unsigned long int avgval;
  int buffer_arr[10], temp;

  void display();

public:

  PHSensor()
    : t_delayRead(1000) {
    ph = 0;
    volt = 0;
    analogPH = 0;
    calibration_value = 21.34;
  }

  void setup();
  void loop();

  float getPH();
  float getVolt();

  float getAnalogPH();
};


void PHSensor::setup() {
}


void PHSensor::loop() {

  if (t_delayRead.isExpired()) {
    /*
    int buf[10];                  //buffer for read analog
    for (int i = 0; i < 10; i++)  //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
    }

    analogPH = analogRead(SensorPin);


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
    volt = (float)(analogPH * (3.3 / 4096));  //convert the analog into millivolt
    ph = m_slope * volt + c_offset;

    // display();
    */

    int maxAnalogValue = 14;
    int minAnalogValue = 0;
    for (int i = 0; i < 10; i++) {
      buffer_arr[i] = analogRead(SensorPin);
      delay(30);
    }
    for (int i = 0; i < 9; i++) {
      for (int j = i + 1; j < 10; j++) {
        if (buffer_arr[i] > buffer_arr[j]) {
          temp = buffer_arr[i];
          buffer_arr[i] = buffer_arr[j];
          buffer_arr[j] = temp;
        }
      }
    }

    analogPH = 0;
    for (int i = 2; i < 8; i++)
      analogPH += buffer_arr[i];
    volt = (float)analogPH * 3.3 / 4096 / 6;
    ph = -5.70 * volt + calibration_value;
    // int ssph = ph_act;

    // if (ssph > maxAnalogValue) {
    //   ssph = maxAnalogValue;
    // }

    // if (ssph < minAnalogValue) {
    //   ssph = minAnalogValue;
    // }
  }
}

void PHSensor::display() {
  Serial.print("pH :");
  Serial.print(ph, 2);
}

float PHSensor::getPH() {
  return ph;
}


float PHSensor::getVolt() {
  return volt;
}


float PHSensor::getAnalogPH() {
  return analogPH / 6;
}
