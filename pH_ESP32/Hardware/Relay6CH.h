
#define ADDR_RELAY      0x21
#define RELAY_ALL_PIN 25

#define ON_STATUS false
#define OFF_STATUS true

class Relay6CH {
private:
  PCF8574 RELAY;
  
  bool status_setup = true;
  Timer t_active;

  Timer timelife_on[6];
  Timer timelife_off[6];
  Timer timelife_toggle[6];

  bool timelife_on_status[6] = {false,false,false,false,false,false};
  bool timelife_off_status[6] = {false,false,false,false,false,false};
  bool timelife_toggle_status[6] = {false,false,false,false,false,false};

  int relayPinList[6] = {P0,P1,P2,P3,P4,P5,};


public:

  int status[6] = {false,false,false,false,false,false};

  Relay6CH() : RELAY(ADDR_RELAY) , t_active(3000){

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

  void on(int pin);
  void on(int pin,int time_millis);
  void off(int pin);
  void off(int pin,int time_millis);
  void toggle(int pin);
  void toggle(int pin,int time_millis);
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

  for(int i = 0;i<6;i++){
    if(timelife_on[i].isExpired() && timelife_on_status[i]){
      timelife_on_status[i] = false;
      off(i);
    }

    if(timelife_off[i].isExpired() && timelife_off_status[i]){
      timelife_off_status[i] = false;
      on(i);
    }

    if(timelife_toggle[i].isExpired() && timelife_toggle_status[i]){
      timelife_toggle_status[i] = false;
      toggle(i);
    }
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
  status[pin] = ON_STATUS;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
}

void Relay6CH::on(int pin,int time_millis){
  timelife_on_status[pin] = true;
  timelife_on[pin].setInterval(time_millis);
  timelife_on[pin].reset();
  Serial.println("timelife_on["+String(pin)+"].setInterval("+String(time_millis)+")");
  status[pin] = ON_STATUS;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
}


void Relay6CH::off(int pin) {
  status[pin] = OFF_STATUS;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
}

void Relay6CH::off(int pin,int time_millis){
  timelife_off_status[pin] = true;
  timelife_off[pin].setInterval(time_millis);
  timelife_off[pin].reset();
  Serial.println("timelife_off["+String(pin)+"].setInterval("+String(time_millis)+")");
  status[pin] = OFF_STATUS;
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
}


void Relay6CH::toggle(int pin) {
  status[pin] = !status[pin];
  RELAY.digitalWrite(relayPinList[pin],status[pin]);
  Serial.println("pin " + String(pin) + "  status = " + String(status[pin]));
}

void Relay6CH::toggle(int pin,int time_millis){
  timelife_toggle_status[pin] = true;
  timelife_toggle[pin].setInterval(time_millis);
  timelife_toggle[pin].reset();
  Serial.println("timelife_toggle["+String(pin)+"].setInterval("+String(time_millis)+")");
  status[pin] = !status[pin];
  RELAY.digitalWrite(relayPinList[pin], status[pin]);
}


void Relay6CH::reset(){
  for(int i = 0;i<8;i++)off(i);
}


