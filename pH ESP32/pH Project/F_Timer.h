class Timer {
  public:
    Timer(unsigned long interval);
    boolean isExpired();
    void reset();
    void setInterval(unsigned long interval);
  private:
    unsigned long _interval;
    unsigned long _previousMillis;
};

Timer::Timer(unsigned long interval) {
  _interval = interval;
  _previousMillis = 0;
}

boolean Timer::isExpired() {
  unsigned long currentMillis = millis();
  if ((unsigned long)(currentMillis - _previousMillis) >= _interval) {
    _previousMillis = currentMillis;
    return true;
  }
  return false;
}

void Timer::reset() {
  _previousMillis = millis();
}

void Timer::setInterval(unsigned long interval) {
  _interval = interval;
}
