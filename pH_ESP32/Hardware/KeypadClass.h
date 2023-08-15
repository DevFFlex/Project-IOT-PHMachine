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

  char enterKey = 'D';
  

  byte rowPins[KEYPAD_ROWS] = { 0, 1, 2, 3 };
  byte colPins[KEYPAD_COLS] = { 4, 5, 6, 7 };

  String text_buffer = "";

  std::function<void(char,String)> onKeypressCallback = NULL;
  std::function<void(char,String)> onEnterCallback = NULL;

public:

  KeypadInput()
    : keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS, I2CADDR, KEYPAD_PCF8574) {
  }

  void setOnKeypressListener(std::function<void(char,String)> callback);
  void setOnKeyEnterListener(std::function<void(char,String)> callback);
  

  void setup();
  void loop();

  void clearBuffer();
};

void KeypadInput::setup() {
  Wire.begin();
  keypad.begin(makeKeymap(keys));
}

void KeypadInput::loop() {
  char key = keypad.getKey();

  if (key) {

    char key_char = key;
    
    if (key == '*') key_char = '.';
    if(key == enterKey){
      if(onEnterCallback != NULL)onEnterCallback(key,text_buffer);
      return;
    }

    text_buffer += String(key_char);

    if (onKeypressCallback != NULL)onKeypressCallback(key,text_buffer);
  }
}

void KeypadInput::clearBuffer() {
  text_buffer = "";
}

void KeypadInput::setOnKeypressListener(std::function<void(char,String)> callback) {
  onKeypressCallback = callback;
}

void KeypadInput::setOnKeyEnterListener(std::function<void(char,String)> callback) {
  onEnterCallback = callback;
}
