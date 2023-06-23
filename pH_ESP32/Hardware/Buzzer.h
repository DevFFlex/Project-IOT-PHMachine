#include <Arduino.h>

#define BUZZER_PIN 27

#define BUZZER_DF_TIMEOFF_MS 1000

class Buzzer {
private:

public:
  bool status_buzzer = false;
  bool is_first = true;
  int freq = 1000;
  Timer t_buzzer;

  Buzzer()
    : t_buzzer(BUZZER_DF_TIMEOFF_MS) {

    ledcSetup(0, 1E5, 12);
    ledcAttachPin(BUZZER_PIN, 0);
  }

  void setup(){
    on();
  }

  void loop() {

    if (t_buzzer.isExpired()) {
      off();
    }
    
  }

  void on() {
    t_buzzer.reset();

    status_buzzer = true;
    ledcWriteTone(0, freq);
    
  }

  void on(int freqIn){
    t_buzzer.reset();
    
    status_buzzer = true;
    ledcWriteTone(0, freqIn);
  }

  void off() {
    ledcWriteTone(0, 0);
  }
};
