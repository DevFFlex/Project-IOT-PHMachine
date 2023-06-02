// #include <HardwareSerial.h>

#define RXp2 16
#define TXp2 17


class ArduinoComunity{
  public:
  String text = "NULL";

  void setup();
  void loop();
};


void ArduinoComunity::setup(){
  // SerialTwo.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}


void ArduinoComunity::loop(){

  if(Serial2.available() > 0){
    text = Serial2.readString();
  }

}