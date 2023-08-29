#include "HardwareSerial.h"

#define RX2pin 16
#define TX2pin 17
#define BAUD_RATE 57600


typedef struct InterfaceEvent_ArduinoComunity{
  std::function<void(String)> onArduinoAvailable = NULL;
}ArduinoComunityCallback;

class ArduinoComunity : public System{
private:
  Variable *var;
  String data_buffer = "";
  
public:
  ArduinoComunityCallback arduinoComunityCallback;

  ArduinoComunity(Variable *var)
  {
    this->var = var;
  }

  void setup() override {
    Serial2.begin(BAUD_RATE, SERIAL_8N1, RX2pin, TX2pin);
  }

  void loop() override {
    while (Serial2.available())
    {
      char c = (char)Serial2.read();
      data_buffer += String(c);
      delay(5);
    }

    if (data_buffer != "")
    {
      data_buffer.trim();
      if(var->datadebug.debug_arduino_comunity)Serial.println("Data From Arduino ---> " + data_buffer ); 
      if(arduinoComunityCallback.onArduinoAvailable != NULL)arduinoComunityCallback.onArduinoAvailable(data_buffer);

      data_buffer = "";
    }
  }
};

