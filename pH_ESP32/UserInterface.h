#include <functional>

typedef void (*fff)();

enum PageName
{
  MAIN,
  INPUT_PH,
  FUNCTION_SELECT
};

class UserInterface : public System{
private:
  Variable *var;
  HardwareIO *hardwareIO;

  Timer update_display;

  PageName pagename;

  byte function_item_cursor = 0;
  bool function_setup = true;
  String function_item[4] = {
      "start adjusting pH",
      "_...",
      "_...",
      "_..."
  };

  String data_buffer = "";

public:
  UserInterface(Variable *varObjectIn) : update_display(1000)
  {
    var = varObjectIn;
    hardwareIO = var->hardwareIO;

    hardwareIO->keypadInput->setOnKeypressListener(std::bind(&UserInterface::onKeypress, this, std::placeholders::_1, std::placeholders::_2));
    hardwareIO->keypadInput->setOnKeyEnterListener(std::bind(&UserInterface::onEnter, this, std::placeholders::_1,std::placeholders::_2));
  }

  void setup() override
  {
    pagename = MAIN;
  }

  void loop() override
  {

    if (update_display.isExpired())
    {

      switch (pagename)
      {
      case MAIN:
        hardwareIO->lcdOutput->printL("PH = " + String(var->mixTank_pH) + " | " + hardwareIO->pHSensor->getPHString(), 0);
        hardwareIO->lcdOutput->printL(String(var->fsw_mixTank_Up) + String(var->fsw_mixtank_Down) + String(var->fsw_waterTank_Down), 1);
        if(hardwareIO->dhtsensor->isReady())hardwareIO->lcdOutput->printL("H = " + String(var->humidity) + "% T = " + String(var->tempC), 2);
        else hardwareIO->lcdOutput->printL("DHT Error", 2);
        hardwareIO->lcdOutput->printL(hardwareIO->rtc->getTimeToString(), 3);
        break;

      case INPUT_PH:
        // Serial.println("setup INPUT Page");
        hardwareIO->lcdOutput->printL("Enter ph value 0-14", 0);
        hardwareIO->lcdOutput->printL("A = enter,C = Clear", 1);
        hardwareIO->lcdOutput->printL("pH : " + data_buffer , 2);
        break;

      case FUNCTION_SELECT:
        // Serial.println("setup FUNCTION Page");
        if (function_setup)
        {
          function_setup = false;
          for (int i = 0; i < 4; i++)
          {
            if (function_item_cursor == i)
            {
              hardwareIO->lcdOutput->printL(">" + function_item[i], i);
            }
            else
              hardwareIO->lcdOutput->printL(" " + function_item[i], i);
          }
        }
        break;
      }
    }
  }

  void changePage(PageName pagenameIn);

  void onEnter(char key, String text_buffer);
  void onKeypress(char key, String text_buffer);
};

void UserInterface::changePage(PageName pagenameIn)
{
  hardwareIO->lcdOutput->clear();
  pagename = pagenameIn;
  hardwareIO->keypadInput->clearBuffer();

  if (pagenameIn == FUNCTION_SELECT)
    function_setup = true;
}

void UserInterface::onEnter(char key, String text_buffer)
{
}

void UserInterface::onKeypress(char key, String text_buffer)
{
  if (pagename == MAIN)
  {
    if (key == 'D')
      changePage(FUNCTION_SELECT);
  }
  else if (pagename == FUNCTION_SELECT)
  {
    if (key == 'D')
      changePage(MAIN);
    else if (key == 'C' && function_item_cursor < 3)
    {
      function_item_cursor++;
      function_setup = true;

    }
    else if (key == 'B' && function_item_cursor > 0){
      function_item_cursor--;
      function_setup = true;
    }

    else if (key == 'A'){
      if(function_item_cursor == 0)changePage(INPUT_PH);
    }
      
  }
  else if (pagename == INPUT_PH)
  {
    if (key == 'D')
      changePage(FUNCTION_SELECT);

    if (key == 'A')
    {
      data_buffer.replace("A", "");
      var->input_ph = data_buffer.toFloat();
      var->workVar.working_status = true;
    }

    if (key == 'C')
    {
      hardwareIO->keypadInput->clearBuffer();
      data_buffer = text_buffer;
      return;
    }

    data_buffer = text_buffer;
  }

}
