#include "ParentClass/System.h"

#include "OtherClass/TimerClass.h"
#include "OtherClass/StringManager.h"
#include "OtherClass/I2CScanner.h"

#include "Variable.h"
#include "Community.h"
#include "SerialInput.h"
#include "LCDMonitor.h"
#include "PHAdjustmentProcess.h"

Variable *var = new Variable();
Community *community = new Community(var);
PHAdjustmentProcess *phap = new PHAdjustmentProcess(var,community);
SerialInput *sInput = new SerialInput(var,community);
LCDMonitor *ui = new LCDMonitor(var,community);


void setup() {
  Serial.begin(115200);
  
  var->setup();
  community->setup();
  phap->setup();
  sInput->setup();
  ui->setup();
}

void loop() {

  var->loop();
  community->loop();
  phap->loop();
  sInput->loop();
  ui->loop();

}