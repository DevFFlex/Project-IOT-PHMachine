class Timer {
  public:
    Timer(unsigned long interval = 100000);
    boolean isExpired();
    void reset();
    void setInterval(unsigned long interval);
    long getCurrentTime();
  private:
    unsigned long _interval;
    unsigned long _previousMillis;
    long second = 0;
};

Timer::Timer(unsigned long interval) {
  _interval = interval;
  _previousMillis = 0;
}

boolean Timer::isExpired() {
  unsigned long currentMillis = millis();
  second = (currentMillis - _previousMillis) / 1000;
  if ((unsigned long)(currentMillis - _previousMillis) >= _interval) {
    _previousMillis = currentMillis;
    
    return true;
  }
  return false;
}


long Timer::getCurrentTime(){
  return second;
}

void Timer::reset() {
  _previousMillis = millis();
}

void Timer::setInterval(unsigned long interval) {
  _interval = interval;
}
