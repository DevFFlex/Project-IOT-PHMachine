#include <TimerOne.h>
#include <SoftwareSerial.h>

#define RELAY_PIN 9

SoftwareSerial esp32(10, 11); 


float calibration_value = 21.34 + 7 - 0.7;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;

float delayReadPH = 1000000 * 0.5;

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
  float volt = (float)avgval * 5.0 / 1024 / 6;
  ph_act = -5.70 * volt + calibration_value;

  // Serial.println("SENSOR_SET_PH=" + String(ph_act));
  // const char *data_str = "SENSOR_SET_PH=";
  String data_str = "SE_PH=" + String(ph_act) + "\n";
  int str_len = data_str.length() + 1;   
  char char_array[str_len];
  data_str.toCharArray(char_array, str_len);
  const char *dataoutput = "Hello World";
  esp32.write(char_array);

  static int cccc = 0;
  Serial.println("send " + String(data_str));
}

void setup() {
  Serial.begin(9600);
  Timer1.initialize(delayReadPH);  // 1 วินาที = 1000000 ไมครอน
  Timer1.attachInterrupt(onreadph);
  esp32.begin(57600);

  pinMode(RELAY_PIN, OUTPUT);

  delay(3000);
  digitalWrite(RELAY_PIN, HIGH);

}
void loop() {


}
