
#define SensorPin 34

class PHSensor {
private:
  unsigned long int avgValue;
  float ph, volt, analogPH;
  Timer t_delayRead;

  float calibration_value;
  int phval = 0;
  unsigned long int avgval;
  int buffer_arr[10], temp;
  float ph_act;
  String ph_str = "";

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
  String getPHString();
  void setPH(float phIn);

  float getAnalogPH();
};


void PHSensor::setup() {
}


void PHSensor::loop() {

  if (t_delayRead.isExpired()) {

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
    avgval = 0;
    for (int i = 2; i < 8; i++)
      avgval += buffer_arr[i];
    float volt = (float)avgval * 5.0 / 4096 / 6;
    ph_act = -5.70 * volt + calibration_value;
    // Serial.println("ph = " + String(ph_act));

    if (ph > 8)ph_str = "Base";
    else if (ph >= 6) ph_str = "Middle";
    else if (ph > 0 && ph < 6) ph_str = "Acid";

    // ph = ph_act;
  }
}

void PHSensor::display() {
  Serial.print("pH :");
  Serial.print(ph, 2);
}

String PHSensor::getPHString(){
  return ph_str;
}

float PHSensor::getPH() {
  return ph;
}

void PHSensor::setPH(float phIn) {
  ph = phIn;
}


float PHSensor::getVolt() {
  return volt;
}


float PHSensor::getAnalogPH() {
  return analogPH / 6;
}
