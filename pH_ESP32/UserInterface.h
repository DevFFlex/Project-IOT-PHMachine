#include <functional>

enum PageName
{
  MAIN,
  INPUT_PH,
  FUNCTION_SELECT,
  WORKING
};

class UserInterface : public System{
private:
  Variable *var;
  HardwareIO *hardwareIO;
  Comunity *comunity;

  Timer update_display;

  PageName pagename;

  byte function_item_cursor = 0;
  bool function_setup = true;
  String function_item[4] = {
      "Home",
      "Start Adjusting pH",
      "Working Info",
      "_..."
  };

  String data_buffer = "";

  void displayPage(){
    String pHText = "";
    if(var->mixTank_pH > 7)pHText = "Base";
    else if(var->mixTank_pH > 6) pHText = "Mid";
    else if(var->mixTank_pH > 0) pHText = "Acid";

    String DTHText = "";
    if(hardwareIO->dhtsensor->isReady())DTHText = "R";
    else DTHText = "E";

    String WIFIText = "";
    if(comunity->wifi_controll->getSTAWifiStatusConnected())WIFIText = "R";
    else WIFIText = "E";

    String StepText = "";
    StepText = String(var->workVar.step);

    String SDCardText = "";
    SDCardText = (var->hardwareIO->sdcard->isReady()) ? "R" : "E";


    switch (pagename)
    {
    case MAIN:
      hardwareIO->lcdOutput->printL("PH = " + String(var->mixTank_pH) + " | " + pHText, 0);
      hardwareIO->lcdOutput->printL("FSW_U=" + String(var->fsw_mixTank_Up) + " FSW_D=" + String(var->fsw_mixtank_Down),1);
      hardwareIO->lcdOutput->printL("DTH:" + DTHText + " WIFI:" + WIFIText + " SD:" + SDCardText, 2);
      hardwareIO->lcdOutput->printL(hardwareIO->rtc->getTimeToString(), 3);
      break;

    case INPUT_PH:
      hardwareIO->lcdOutput->printL("Enter ph value 0-14", 0);
      hardwareIO->lcdOutput->printL("A = enter,C = Clear", 1);
      hardwareIO->lcdOutput->printL("pH : " + data_buffer , 2);
      break;

    case FUNCTION_SELECT:
      if (function_setup)
      {
        function_setup = false;
        for (int i = 0; i < 4; i++)
        {
          if (function_item_cursor == i)hardwareIO->lcdOutput->printL(">" + function_item[i], i);
          else hardwareIO->lcdOutput->printL(" " + function_item[i], i);
        }
      }
      break;

    case WORKING:
      hardwareIO->lcdOutput->printL("Step = " + String(var->workVar.step),0);
      hardwareIO->lcdOutput->printL("pH = " + String(var->mixTank_pH),1);
      hardwareIO->lcdOutput->printL("pH_input = " + String(var->input_ph), 2);
      hardwareIO->lcdOutput->printL(var->workVar.outputText1, 3);
      break;
    }
  }

public:
  UserInterface(Variable *varObjectIn,Comunity *comunity) : update_display(1000)
  {
    var = varObjectIn;
    hardwareIO = var->hardwareIO;
    this->comunity = comunity;

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
      displayPage();
    }
  }

  void changePage(PageName pagenameIn);

  void onEnter(char key, String text_buffer);
  void onKeypress(char key, String text_buffer);
};

void UserInterface::changePage(PageName pagenameIn)
{
  hardwareIO->lcdOutput->clear();
  hardwareIO->keypadInput->clearBuffer();
  pagename = pagenameIn;

  if (pagenameIn == FUNCTION_SELECT)
    function_setup = true;
}

void UserInterface::onEnter(char key, String text_buffer)
{
  switch(pagename){
    case MAIN:
      changePage(FUNCTION_SELECT);
      break;
    case FUNCTION_SELECT:
      if(function_item_cursor == 0)changePage(MAIN);
      else if(function_item_cursor == 1)changePage(INPUT_PH);
      else if(function_item_cursor == 2)changePage(WORKING);
      break;
    case INPUT_PH:
      data_buffer.replace(String(key), "");
      var->input_ph = data_buffer.toFloat();
      var->workVar.working_status = true;
      changePage(MAIN);
      break;
  }
}

void UserInterface::onKeypress(char key, String text_buffer)
{

  switch(pagename){
    case MAIN:
      break;
    case FUNCTION_SELECT:
      if (key == 'C' && function_item_cursor < 3)
      {
        function_item_cursor++;
        function_setup = true;
        Serial.println("FunctionSelect : Click C = down");
      }
      else if (key == 'B' && function_item_cursor > 0){
        function_item_cursor--;
        function_setup = true;
        Serial.println("FunctionSelect : Click B = up");
      }
      break;
    case INPUT_PH:
      if (key == 'B'){
        changePage(FUNCTION_SELECT);
      }  
      else if (key == 'C')
      {
        hardwareIO->keypadInput->clearBuffer();
        data_buffer = "";
        return;
      }else{
        data_buffer = text_buffer;
      }
      break;
    case WORKING:
      if (key == 'B'){
        changePage(FUNCTION_SELECT);
      }
      break;
  }

}
