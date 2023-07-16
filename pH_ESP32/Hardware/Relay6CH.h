
#define ADDR_RELAY      0x21
#define RELAY_ALL_PIN 25

class Relay6CH {
private:
  PCF8574 RELAY;
  
  bool status_setup = true;
  Timer t_active;

  int relayPinList[8] = {
    P0,
    P1,
    P2,
    P3,
    P4,
    P5,
    P6,
    P7
  };

  

public:

  int status[8] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false
  };

  Relay6CH() : RELAY(ADDR_RELAY) , t_active(3000) {

    RELAY.pinMode(relayPinList[0], OUTPUT);
    RELAY.pinMode(relayPinList[1], OUTPUT);
    RELAY.pinMode(relayPinList[2], OUTPUT);
    RELAY.pinMode(relayPinList[3], OUTPUT);
    RELAY.pinMode(relayPinList[4], OUTPUT);
    RELAY.pinMode(relayPinList[5], OUTPUT);
    RELAY.pinMode(relayPinList[6], OUTPUT);
    RELAY.pinMode(relayPinList[7], OUTPUT);
    RELAY.begin();


    pinMode(RELAY_ALL_PIN,OUTPUT);
  }

  void setup();
  void loop();

  void active();
  void deactive();

  void on(int pin);
  void off(int pin);
  void toggle(int pin);
  void reset();
};

void Relay6CH::setup() {
  for (int i = 0;i<8;i++)off(relayPinList[i]);
}

void Relay6CH::loop() {
  if(t_active.isExpired() && status_setup){
    status_setup = false;
    active();
  }
}

void Relay6CH::active(){
  digitalWrite(RELAY_ALL_PIN,HIGH);
  Serial.println("Relay Active");
}

void Relay6CH::deactive(){
  digitalWrite(RELAY_ALL_PIN,LOW);
  Serial.println("Relay DeActive");
}


void Relay6CH::on(int pin) {
  status[pin] = false;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
}


void Relay6CH::off(int pin) {
  status[pin] = true;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
  
}

void Relay6CH::toggle(int pin) {
  
  status[pin] = !status[pin];
  RELAY.digitalWrite(relayPinList[pin],status[pin]);
  Serial.println("pin " + String(pin) + "  status = " + String(status[pin]));
}


void Relay6CH::reset(){
  for(int i = 0;i<8;i++)off(i);
}


