#include <functional>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define I2CADDR 0x20
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

class KeypadInput {
private:
  Keypad_I2C keypad;

  char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
  };

  byte rowPins[KEYPAD_ROWS] = { 0, 1, 2, 3 };
  byte colPins[KEYPAD_COLS] = { 4, 5, 6, 7 };

  String text_buffer = "";

  void (*onEnterListener)(String) = NULL;
  void (*onKeypessListener)(char, String) = NULL;

  std::function<void(char,String)> onKeypressCallback;
  std::function<void(String)> onEnterCallback;

public:

  KeypadInput()
    : keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS, I2CADDR, KEYPAD_PCF8574) {
  }

  void setOnKeyEnterListener(std::function<void(String)> callback);
  void setOnKeypressListener(std::function<void(char,String)> callback);

  void setup();
  void loop();

  void clearTextNow();
};

void KeypadInput::setup() {
  Wire.begin();
  keypad.begin(makeKeymap(keys));
}

void KeypadInput::loop() {
  char key = keypad.getKey();

  if (key) {

    char key_char = key;
    if (key == 'D') {
      if (onEnterCallback != NULL)onEnterCallback(text_buffer);
      text_buffer = "";
      if (onKeypressCallback != NULL)onKeypressCallback(key,text_buffer);
      return;
    } else if (key == '*') key_char = '.';



    text_buffer += String(key_char);

    if (onKeypressCallback != NULL)onKeypressCallback(key,text_buffer);
  }
}

void KeypadInput::clearTextNow() {
  text_buffer = "";
}

void KeypadInput::setOnKeyEnterListener(std::function<void(String)> callback) {
  onEnterCallback = callback;
}
void KeypadInput::setOnKeypressListener(std::function<void(char,String)> callback) {
  onKeypressCallback = callback;
}