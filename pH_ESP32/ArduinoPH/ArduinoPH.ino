#include <TimerOne.h>
#include <SoftwareSerial.h>

#define RELAY_PIN 9

#define DEFAULT_CALIBRATION_VAL 21.34
#define DEFAULT_M -5.70
#define DEFAULT_VOLTIN 5.0
#define DEFAULT_MAX_ANALOG 1024

SoftwareSerial esp32(10, 11); 


float calibration_value = DEFAULT_CALIBRATION_VAL + 7 - 0.7;
float m = DEFAULT_M;
float voltin = DEFAULT_VOLTIN;
float max_analog = DEFAULT_MAX_ANALOG;

int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;

float delayReadPH = 1000000 * 1.0;

void onreadph() {
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(A0);
    // Serial.println(buffer_arr[i]);
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
  float volt = (float)avgval * voltin / max_analog / 6;
  ph_act = m * volt + calibration_value;

  // Serial.println("SENSOR_SET_PH=" + String(ph_act));
  // const char *data_str = "SENSOR_SET_PH=";
  String data_str = "SE_PH=" + String(ph_act) + "\n";
  int str_len = data_str.length() + 1;   
  // char char_array[str_len];
  // data_str.toCharArray(char_array, str_len);
  // const char *dataoutput = "Hello World";
  esp32.write(data_str.c_str());

  static int cccc = 0;
  Serial.println("send " + String(data_str));
}

void setup() {
  Serial.begin(9600);
  Timer1.initialize(delayReadPH);  // 1 วินาที = 1000000 ไมครอน
  Timer1.attachInterrupt(onreadph);
  esp32.begin(57600);

}
void loop() {

  if(Serial.available() > 0){
    String data_str = Serial.readString();
    data_str.trim();

    if(data_str.indexOf("S_CLB=") != -1){
      data_str.replace("S_CLB=","");
      calibration_value = data_str.toFloat();
    }else if(data_str.indexOf("G_CLB")){
      String data_out = "S_CLB=" + String(calibration_value);
      esp32.write(data_out.c_str());
    }

    if(data_str.indexOf("S_M=") != -1){
      data_str.replace("S_M=","");
      m = data_str.toFloat();
    }else if(data_str.indexOf("G_M")){
      String data_out = "S_M=" + String(m);
      esp32.write(data_out.c_str());
    }

    if(data_str.indexOf("S_VI=") != -1){
      data_str.replace("S_VI=","");
      voltin = data_str.toFloat();
    }else if(data_str.indexOf("G_VI")){
      String data_out = "S_VI=" + String(voltin);
      esp32.write(data_out.c_str());
    }

    if(data_str.indexOf("S_AMAX=") != -1){
      data_str.replace("S_AMAX=","");
      max_analog = data_str.toFloat();
    }else if(data_str.indexOf("G_AMAX")){
      String data_out = "S_AMAX=" + String(max_analog);
      esp32.write(data_out.c_str());
    }
  }

}
