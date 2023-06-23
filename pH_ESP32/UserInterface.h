#include <functional>

class UserInterface
{
private:
  Variable *var;
  HardwareIO *hardwareIO;
  Comunity *comunity;
  Timer update_display;

public:
  UserInterface(Variable *varObjectIn, HardwareIO *hardwareIOIn,Comunity *comunityIn) : update_display(1000)
  {
    var = varObjectIn;
    hardwareIO = hardwareIOIn;
    comunity = comunityIn;

    hardwareIO->keypadInput->setOnKeypressListener(std::bind(&UserInterface::onKeypress, this, std::placeholders::_1, std::placeholders::_2));
    hardwareIO->keypadInput->setOnKeyEnterListener(std::bind(&UserInterface::onEnter, this, std::placeholders::_1));
  }

  void setup(){}

  void loop()
  {
    if (update_display.isExpired())
    {

      var->mixTank_pH = hardwareIO->pHSensor->getPH();
      // hardwareIO->buzzer->on();

      hardwareIO->lcdOutput->printL("PH = " + String(hardwareIO->pHSensor->getPH()) + " | " + hardwareIO->pHSensor->getPHString(), 0);

      hardwareIO->lcdOutput->printL(hardwareIO->rtc->getTimeToString(), 3);
    }
  }

  void onEnter(String text);
  void onKeypress(char c, String textNow);
};

void UserInterface::onEnter(String text_buffer)
{
}

void UserInterface::onKeypress(char key, String text_buffer)
{
  if (key == 'C')
  {
    hardwareIO->keypadInput->clearTextNow();
    hardwareIO->lcdOutput->printL("INPUT : ", 1);
    return;
  }

  hardwareIO->lcdOutput->printL("INPUT : " + text_buffer, 1);
}




x


x