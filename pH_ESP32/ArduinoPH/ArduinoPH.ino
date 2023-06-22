#include <TimerOne.h>
#include <SoftwareSerial.h>

#define RELAY_PIN 9

#define DEFAULT_CALIBRATION_VAL 21.34 + 7 - 0.7
#define DEFAULT_M -5.70
#define DEFAULT_VOLTIN 5.0
#define DEFAULT_MAX_ANALOG 1024

SoftwareSerial esp32(10, 11);

float calibration_value = DEFAULT_CALIBRATION_VAL;
float m = DEFAULT_M;
float voltin = DEFAULT_VOLTIN;
float max_analog = DEFAULT_MAX_ANALOG;

int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;

float delayReadPH = 1000000 * 1.0;

void onreadph()
{
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    // Serial.println(buffer_arr[i]);
    delay(30);
  }

  // sort
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * voltin / max_analog / 6; // Volt = 14 * 5 / 1000 / 6
  ph_act = m * volt + calibration_value;                // pH = m * Volt + Calibration

  String data_str = "";
  data_str += "SE_PH=" + String(ph_act) + ",";
  data_str += "SE_CLB=" + String(calibration_value) + ",";
  data_str += "SE_MP=" + String(m) + ",";
  data_str += "SE_VOLTIN=" + String(voltin) + ",";
  data_str += "SE_MAX_ANALOG=" + String(max_analog) + ",";
  data_str += "SE_ANALOG_AVG=" + String(avgval / 6) + "\n";

  Serial.println(data_str);

  esp32.write(data_str.c_str());
}

void setup()
{
  Serial.begin(9600);
  Timer1.initialize(delayReadPH); // 1 วินาที = 1000000 ไมครอน
  Timer1.attachInterrupt(onreadph);
  esp32.begin(57600);
}

void loop()
{

  static String data_buffer = "";
  while (esp32.available())
  {
    char c = (char)esp32.read();
    data_buffer += String(c);
  }

  if (data_buffer != "")
  {
    String data_str = data_buffer;
    data_str.trim();
    Serial.println("data_str = " + data_str);

    if (data_str.indexOf("SDEFAULT") != -1)
    {
      calibration_value = DEFAULT_CALIBRATION_VAL;
      m = DEFAULT_M;
      voltin = DEFAULT_VOLTIN;
      max_analog = DEFAULT_MAX_ANALOG;
    }
    else
    {
      String data[4];
      split(data, data_str, ",", 4);

      for (int i = 0; i < 4; i++)
      {
        String c_v[2];
        split(c_v, data[i], "=", 2);
        recv(c_v[0], c_v[1]);

        Serial.println("c_v[0] = " + c_v[0] + "    c_v[1] = " + c_v[1]);
      };
    }

    data_buffer = "";
  }
}

void recv(String command, String value)
{
  if (command.indexOf("S_CLB") != -1)
  {
    calibration_value = value.toFloat();
  }
  else if (command.indexOf("G_CLB"))
  {
    String data_out = "S_CLB=" + String(calibration_value);
    esp32.write(data_out.c_str());
  }

  if (command.indexOf("S_M") != -1)
  {
    m = value.toFloat();
  }
  else if (command.indexOf("G_M"))
  {
    String data_out = "S_M=" + String(m);
    esp32.write(data_out.c_str());
  }

  if (command.indexOf("S_VI") != -1)
  {
    voltin = value.toFloat();
  }
  else if (command.indexOf("G_VI"))
  {
    String data_out = "S_VI=" + String(voltin);
    esp32.write(data_out.c_str());
  }

  if (command.indexOf("S_AMAX") != -1)
  {
    max_analog = value.toFloat();
  }
  else if (command.indexOf("G_AMAX"))
  {
    String data_out = "S_AMAX=" + String(max_analog);
    esp32.write(data_out.c_str());
  }
}

void split(String *databox, String text, String symbol, int size)
{

  char charArrayText[text.length() + 1];
  text.toCharArray(charArrayText, text.length() + 1);

  char charArraySymbol[symbol.length() + 1];
  symbol.toCharArray(charArraySymbol, symbol.length() + 1);

  char *symbol1 = charArraySymbol;

  char *part = strtok(charArrayText, symbol1);
  for (int i = 0; i < size; i++)
  {
    databox[i] = part;

    part = strtok(NULL, symbol1);
  }
}