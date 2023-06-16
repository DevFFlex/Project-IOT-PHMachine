#define BUZZER_PIN 27
#include <Arduino.h>


class Buzzer {
private:

public:
  bool status_buzzer = false;
  bool is_first = true;
  int freq = 1000;
  Timer t_buzzer;

  Buzzer()
    : t_buzzer(1000) {

    ledcSetup(0, 1E5, 12);
    ledcAttachPin(BUZZER_PIN, 0);
  }

  void setup(){
    on();
  }

  void loop() {

    if (t_buzzer.isExpired()) {
      Serial.println("buzzer toggle");
      off();
    }
    // uint8_t octave = 1;
    // ledcWriteNote(0, NOTE_C, octave);
    
  }

  void on() {
    status_buzzer = true;
    ledcWriteTone(0, freq);
  }
  void off() {
    ledcWriteTone(0, 0);
  }
};
