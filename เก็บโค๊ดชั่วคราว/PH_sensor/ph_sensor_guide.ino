#define BLYNK_TEMPLATE_ID "TMPL6wE0vbDKh"
#define BLYNK_TEMPLATE_NAME "farm test"
#define BLYNK_AUTH_TOKEN "DfSOcwlrD8wYqKwuJVMKjXUjTv7zI3TS"
#define BLYNK_PRINT Serial
#define DHTPIN 27
#define DHTTYPE DHT22
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "Smart IoT";
char pass[] = "113333555555";

WidgetLED ledmain(V9);
WidgetLED leda(V10);
WidgetLED ledb(V11);


BlynkTimer timer;

int WaterSensor = 35;

float calibration_value = 21.34;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

int Relay_1 = 19;
int Relay_2 = 18;
int Relay_3 = 5;
int Relay_4 = 17;
int Relay_5 = 16;

int autosw;
int state1;
int state2;
int state3;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.println(F("DHTxx test!"));
  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  pinMode(Relay_5, OUTPUT);

  digitalWrite(Relay_1, HIGH);
  digitalWrite(Relay_2, HIGH);
  digitalWrite(Relay_3, HIGH);
  digitalWrite(Relay_4, HIGH);
  digitalWrite(Relay_5, HIGH);

  autosw = 0;
  state1 = 0;
  state2 = 0;
  state3 = 0;

  timer.setInterval(1000L, DHTSENSOR);
  timer.setInterval(1000L, PHSENSOR);
  // timer.setInterval(1000L, WATERSENSOR);
  timer.setInterval(1500L, AUTOMAIN);
  timer.setInterval(1500L, AUTOAB);
}

/*------------------------------------------------------------v-Switch Function-v--------------------------------------------------------------------------------------*/

BLYNK_WRITE(V3) {
  int Auto = param.asInt();
  if (Auto == 1) {
    autosw = 1;
  } else {
    autosw = 0;
  }
}

BLYNK_WRITE(V4) {
  int pumpmain = param.asInt();
  if (pumpmain == 1) {
    digitalWrite(Relay_1, LOW);
    ledmain.on();
    state1 = 1;
  } else {
    digitalWrite(Relay_1, HIGH);
    ledmain.off();
    state1 = 0;
  }
}

BLYNK_WRITE(V5) {
  int pumpa = param.asInt();
  if (pumpa == 1) {
    digitalWrite(Relay_2, LOW);
    leda.on();
    state2 = 1;
  } else {
    digitalWrite(Relay_2, HIGH);
    leda.off();
    state2 = 0;
  }
}

BLYNK_WRITE(V6) {
  int pumpb = param.asInt();
  if (pumpb == 1) {
    digitalWrite(Relay_3, LOW);
    ledb.on();
    state3 = 1;
  } else {
    digitalWrite(Relay_3, HIGH);
    ledb.off();
    state3 = 0;
  }
}

BLYNK_WRITE(V7) {
  int pumpc = param.asInt();
  if (pumpc == 1) {
    digitalWrite(Relay_4, LOW);
  } else {
    digitalWrite(Relay_4, HIGH);
  }
}

/*------------------------------------------------------------v-Sensor Function-v--------------------------------------------------------------------------------------*/

/*DHT*/
void DHTSENSOR() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int temperature1 = dht.readTemperature();
  int humidity1 = dht.readHumidity();
  // if (isnan(temperature) || isnan(humidity)) {
  //   Serial.println("Failed to read from DHT sensor!");
  //   return;
  // }
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature1);
  lcd.print("C");
  // Serial.print("Temp: ");
  // Serial.print(temperature1);
  // Serial.print(" C");
  // Serial.println();
  // Serial.print("Hum: ");
  // Serial.print(humidity1);
  // Serial.print(" %");
  // Serial.println();



  lcd.setCursor(9, 0);
  lcd.print("Hum:");
  lcd.print(humidity1);
  lcd.print("%");
}

/*pH*/
void PHSENSOR() {
  int maxAnalogValue = 14;
  int minAnalogValue = 0;
  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(34);
    delay(30);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }

  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1025 / 6;
  float ph_act = -5.70 * volt + calibration_value;
  int ssph = ph_act;

  if (ssph > maxAnalogValue) {
    ssph = maxAnalogValue;
  }

  if (ssph < minAnalogValue) {
    ssph = minAnalogValue;
  }

  Blynk.virtualWrite(V2, ssph);
  lcd.setCursor(0, 1);
  lcd.print("pH:");
  lcd.print(ssph);
}

/*Water*/
void WATERSENSOR() {
  int sensorValue = map(analogRead(WaterSensor), 0, 750, 0, 100);

  Blynk.virtualWrite(V8, sensorValue);

  lcd.setCursor(6, 1);
  lcd.print("Water:");
  lcd.print(sensorValue);
  lcd.print("%");

  Serial.println(sensorValue);

  if (sensorValue >= 95) {
    digitalWrite(Relay_1, HIGH);
    digitalWrite(Relay_2, HIGH);
    digitalWrite(Relay_3, HIGH);
  }
}



/*-------------------------------------------------------------AUTO Function---------------------------------------------------------------------------------------*/
/*Auto Main*/
void AUTOMAIN() {

  static unsigned long previousTime = 0;
  const unsigned long interval1 = 20000;
  const unsigned long interval2 = 10000;

  if ((autosw == 1) && (state1 == 0)) {
    if ((millis() - previousTime) < interval1) {
      digitalWrite(Relay_1, LOW);
      ledmain.on();
    } else if ((millis() - previousTime) < interval1 + interval2) {
      digitalWrite(Relay_1, HIGH);
      ledmain.off();
    } else {
      previousTime = millis();
    }
  } else if (state1 == 0) {
    digitalWrite(Relay_1, HIGH);
    ledmain.off();
  }

  Serial.println();
  Serial.print("autosw: ");
  Serial.print(autosw);
  Serial.println();
  Serial.print("state1: ");
  Serial.print(state1);
  Serial.println();
  Serial.print("state2: ");
  Serial.print(state2);
  Serial.println();
  Serial.print("state3: ");
  Serial.print(state3);
  Serial.println();
}

/*Auto AB*/
void AUTOAB() {

  static unsigned long previousTime1 = 0;
  const unsigned long interval_1 = 2000;
  const unsigned long interval_2 = 60000;

  if ((autosw == 1) && (state2 == 0)) {
    if ((millis() - previousTime1) < interval_1) {
      digitalWrite(Relay_2, LOW);
      digitalWrite(Relay_3, LOW);
      leda.on();
      ledb.on();
    } else if ((millis() - previousTime1) < interval_1 + interval_2) {
      digitalWrite(Relay_2, HIGH);
      digitalWrite(Relay_3, HIGH);
      leda.off();
      ledb.off();
    } else {
      previousTime1 = millis();
    }
  } else if ((state2 == 0) && (state3 == 0)) {
    digitalWrite(Relay_2, HIGH);
    digitalWrite(Relay_3, HIGH);
    leda.off();
    ledb.off();
  }
}




void loop() {
  int sensorValue = map(analogRead(WaterSensor), 0, 750, 0, 100);

  Blynk.virtualWrite(V8, sensorValue);

  lcd.setCursor(6, 1);
  lcd.print("Water:");
  lcd.print(sensorValue);
  lcd.print("%");

  Serial.println(sensorValue);

  if (sensorValue >= 95) {
    digitalWrite(Relay_2, HIGH);
    digitalWrite(Relay_3, HIGH);
  }
  Blynk.run();
  timer.run();
}