class PHSensor {
private:

  float ph = 0;
  Timer t_delayRead;

  String ph_str = "";

  void display();





public:

  PHSensor()
    : t_delayRead(1000) {
  }

  void setup();
  void loop();

  float getPH();
  String getPHString();
  String getPHStringDigitLimite();

  void setPH(float phIn);
};


void PHSensor::setup() {
}


void PHSensor::loop() {

  if (ph > 8) ph_str = "Base";
  else if (ph >= 6) ph_str = "Middle";
  else if (ph > 0 && ph < 6) ph_str = "Acid";
}

void PHSensor::display() {
  Serial.print("pH :");
  Serial.print(ph, 2);
}

String PHSensor::getPHString() {
  return ph_str;
}

String PHSensor::getPHStringDigitLimite(){
  char buffer[10];
  sprintf(buffer, "%d.##", ph); 
  String data(buffer);

  return data;
}

float PHSensor::getPH() {
  return ph;
}

void PHSensor::setPH(float phIn) {
  ph = phIn;
}
