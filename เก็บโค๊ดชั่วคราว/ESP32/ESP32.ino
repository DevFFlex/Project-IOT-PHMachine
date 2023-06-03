#define RX2pin 16
#define TX2pin 17
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX2pin, TX2pin);
}
void loop() {
    Serial.println("Message Received: ");
    Serial.println(Serial2.readString());
}
