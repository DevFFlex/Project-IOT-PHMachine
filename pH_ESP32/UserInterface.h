class UserInterface {
private:
  VarObject *varObject;
  Comunity *comunity;
  HardwareIO *hardwareIO;

public:

  UserInterface(VarObject *varObjectIn, Comunity *comunityIn, HardwareIO *hardwareIOIn) {
    varObject = varObjectIn;
    comunity = comunityIn;
    hardwareIO = hardwareIOIn;


    hardwareIO->keypadInput->setOnKeypressListener(std::bind(&UserInterface::onKeypress,this,std::placeholders::_1,std::placeholders::_2));
    hardwareIO->keypadInput->setOnKeyEnterListener(std::bind(&UserInterface::onEnter,this,std::placeholders::_1));
  }

  void loop(){

  }


  void onEnter(String text);
  void onKeypress(char c, String textNow);

};

void UserInterface::onEnter(String text_buffer){
  
}

void UserInterface::onKeypress(char key,String text_buffer){
  if (key == 'C') {
      hardwareIO->keypadInput->clearTextNow();
      hardwareIO->lcdOutput->printL("INPUT : ", 1);
      return;
    }

    hardwareIO->lcdOutput->printL("INPUT : " + text_buffer, 1);
}