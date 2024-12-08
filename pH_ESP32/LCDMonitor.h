#include <functional>

enum PageName
{
  MAIN,
  INPUT_PH,
  FUNCTION_SELECT,
  WORKING,
  ADMIN
};

class LCDMonitor : public System
{
private:
  Variable *var;
  HardwareIO *hardwareIO;
  Community *community;

  Timer update_display;

  PageName pagename;

  byte function_item_cursor = 0;
  bool onPageSetup = true;
  bool pageUpdate = false;
  String function_item[4] = {
      "Start Adjusting pH",
      "Working Info",
      "_...",
      "_..."};

  String data_buffer = "";

  void displayPage()
  {
    String pHText = "";
    if (var->mixTank_pH > 7)
      pHText = "Base";
    else if (var->mixTank_pH > 6)
      pHText = "Mid";
    else if (var->mixTank_pH > 0)
      pHText = "Acid";

    String WIFIText = "";
    if (community->wifi_controll->getSTAWifiStatusConnected())
      WIFIText = "R";
    else
      WIFIText = "E";

    String StepText = "";
    StepText = String(var->workVar.step);

    String SDCardText = "";
    SDCardText = (var->hardwareIO->sdcard->isReady()) ? "R" : "E";


    if(pagename == MAIN){
      hardwareIO->lcdOutput->printL("pH = " + String(var->mixTank_pH) + " | " + pHText, 0);
      hardwareIO->lcdOutput->printL("--------------------", 1);
      hardwareIO->lcdOutput->printL("WIFI:" + WIFIText + " SD:" + SDCardText, 2);
      hardwareIO->lcdOutput->printL(hardwareIO->rtc->getTimeToString(), 3);
    }else if(pagename == INPUT_PH){
      if(pageUpdate){
        hardwareIO->lcdOutput->printL("Enter ph value 4-10", 0);
        hardwareIO->lcdOutput->printL("D = enter,C = Clear", 1);
        hardwareIO->lcdOutput->printL("pH : " + data_buffer, 2);
      }
    }else if(pagename == FUNCTION_SELECT){
      if (pageUpdate)
      {
        onPageSetup = false;
        for (int i = 0; i < 4; i++)
        {
          if (function_item_cursor == i)
            hardwareIO->lcdOutput->printL(">" + function_item[i], i);
          else
            hardwareIO->lcdOutput->printL(" " + function_item[i], i);
        }
      }
    }else if(pagename == WORKING){
      hardwareIO->lcdOutput->printL("Step = " + String(var->workVar.step) + "       " + var->workVar.outputText1, 0);
      hardwareIO->lcdOutput->printL("pH_Tank = " + String(var->mixTank_pH), 1);
      hardwareIO->lcdOutput->printL("pH_I = " + String(var->workVar.adjustCurrentpH), 2);
      if (var->workVar.adjustCurrentpH >= 3 && var->workVar.adjustCurrentpH)hardwareIO->lcdOutput->printL("pH_O = " + String(var->workVar.adjustCurrentpH - var->workVar.pH_space_rate) + " - " + String(var->workVar.adjustCurrentpH + var->workVar.pH_space_rate), 3);
      else hardwareIO->lcdOutput->printL("pH_O = NoCal", 3);
    }
  }

public:
  LCDMonitor(Variable *varObjectIn, Community *community) : update_display(100)
  {
    var = varObjectIn;
    hardwareIO = var->hardwareIO;
    this->community = community;

    hardwareIO->keypadInput->setOnKeypressListener(std::bind(&LCDMonitor::onKeypress, this, std::placeholders::_1, std::placeholders::_2));
    hardwareIO->keypadInput->setOnKeyEnterListener(std::bind(&LCDMonitor::onEnter, this, std::placeholders::_1, std::placeholders::_2));
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

  void changePage(PageName pagenameIn)
  {
    hardwareIO->lcdOutput->clear();
    hardwareIO->keypadInput->clearBuffer();
    pagename = pagenameIn;

    pageUpdate = true;
      
  }

  void onEnter(char key, String text_buffer)
  {
    pageUpdate = true;

    if(pagename == MAIN){

    }else if(pagename == FUNCTION_SELECT){
      if (function_item_cursor == 0)changePage(INPUT_PH);
      else if (function_item_cursor == 1)changePage(WORKING);
    }else if(pagename == INPUT_PH){
      data_buffer.replace(String(key), "");
      var->workVar.startAdjustPH(data_buffer.toFloat(),1);
      changePage(WORKING);
    }else if(pagename == WORKING){
      
    }
    
  }
  void onKeypress(char key, String text_buffer)
  {
    pageUpdate = true;
    if (key == '#')
    {
      var->workVar.stopAdjustPH();
    }

    switch (pagename)
    {
    case MAIN:
      if (key == 'B')
        changePage(FUNCTION_SELECT);
      break;
    case FUNCTION_SELECT:
      if (key == '2' && function_item_cursor < 3)
      {
        function_item_cursor++;
        Serial.println("FunctionSelect : Click 2 = down");
      }
      else if (key == '1' && function_item_cursor > 0)
      {
        function_item_cursor--;
        Serial.println("FunctionSelect : Click 1 = up");
      }
      else if (key == 'B')
        changePage(MAIN);
      break;
    case INPUT_PH:
      if (key == 'B')
      {
        changePage(FUNCTION_SELECT);
      }
      else if (key == 'C')
      {
        hardwareIO->keypadInput->clearBuffer();
        data_buffer = "";
        return;
      }
      else
      {
        data_buffer = text_buffer;
      }
      break;
    case WORKING:
      if (key == 'B')
      {
        changePage(FUNCTION_SELECT);
      }
      break;
    }
  }
};
