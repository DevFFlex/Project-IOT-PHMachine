
#define ADDR_RELAY      0x21
#define RELAY_ALL_PIN 25

class Relay6CH {
private:
  PCF8574 RELAY;
  
  bool status_setup = true;
  Timer t_active;

  Timer timelife1,timelife2,timelife3,timelife4,timelife5,timelife6;

  int relayPinList[6] = {
    P0,
    P1,
    P2,
    P3,
    P4,
    P5,
  };

  int status_onTimeout[6] = {
    0,0,0,0,0,0
  };

  

public:

  int status[6] = {
    false,
    false,
    false,
    false,
    false,
    false
  };

  Relay6CH() : RELAY(ADDR_RELAY) , t_active(3000), timelife1(5000000), timelife2(5000000), timelife3(5000000), timelife4(5000000), timelife5(5000000), timelife6(5000000){

    RELAY.pinMode(relayPinList[0], OUTPUT);
    RELAY.pinMode(relayPinList[1], OUTPUT);
    RELAY.pinMode(relayPinList[2], OUTPUT);
    RELAY.pinMode(relayPinList[3], OUTPUT);
    RELAY.pinMode(relayPinList[4], OUTPUT);
    RELAY.pinMode(relayPinList[5], OUTPUT);
    RELAY.begin();


    pinMode(RELAY_ALL_PIN,OUTPUT);
  }

  void setup();
  void loop();

  void active();
  void deactive();

  void onTimeout(int pin,float time);
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


  if(timelife1.isExpired() && status_onTimeout[0]){
    off(0);
    status_onTimeout[0] = 0;
  }
  if(timelife2.isExpired() && status_onTimeout[1]){
    off(1);
    status_onTimeout[1] = 0;
  }
  if(timelife3.isExpired() && status_onTimeout[2]){
    off(2);
    status_onTimeout[2] = 0;
  }
  if(timelife4.isExpired() && status_onTimeout[3]){
    off(3);
    status_onTimeout[3] = 0;
  }
  if(timelife5.isExpired() && status_onTimeout[4]){
    off(4);
    status_onTimeout[4] = 0;
  }
  if(timelife6.isExpired() && status_onTimeout[5]){
    off(5);
    status_onTimeout[5] = 0;
  }
\
  
}

void Relay6CH::active(){
  digitalWrite(RELAY_ALL_PIN,HIGH);
  Serial.println("Relay Active");
}

void Relay6CH::deactive(){
  digitalWrite(RELAY_ALL_PIN,LOW);
  Serial.println("Relay DeActive");
}

void Relay6CH::onTimeout(int pin,float time){

  if(time == -1){
    toggle(pin);
    return;
  }

  status[pin] = false;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);

  switch (pin)
  {
  case 0:
    timelife1.setInterval((int)time * 1000);
    timelife1.reset();
    status_onTimeout[pin] = 1;
    break;
  case 1:
    timelife2.setInterval((int)time * 1000);
    timelife2.reset();
    break;
  case 2:
    timelife3.setInterval((int)time * 1000);
    timelife3.reset();
    break;
  case 3:
    timelife4.setInterval((int)time * 1000);
    timelife4.reset();
    break;
  case 4:
    timelife5.setInterval((int)time * 1000);
    timelife5.reset();
    break;
  case 5:
    timelife6.setInterval((int)time * 1000);
    timelife6.reset();
    break;
  
  default:
    break;
  }

  status_onTimeout[pin] = 1;

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


