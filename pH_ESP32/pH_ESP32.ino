#include "ParentClass/System.h"

#include "OtherClass/TimerClass.h"
#include "OtherClass/StringManager.h"
#include "OtherClass/I2CScanner.h"

#include "Variable.h"
#include "Comunity.h"
#include "SerialInput.h"
#include "UserInterface.h"
#include "PHAdjustmentProcess.h"

Variable *var = new Variable();
PHAdjustmentProcess *phap = new PHAdjustmentProcess(var);
Comunity *comunity = new Comunity(var);
SerialInput *sInput = new SerialInput(var,comunity);
UserInterface *ui = new UserInterface(var,comunity);




void setup() {
  Serial.begin(115200);

  var->setup();
  phap->setup();
  comunity->setup();
  sInput->setup();
  ui->setup();
}

void loop() {
  var->loop();
  phap->loop();
  comunity->loop();
  sInput->loop();
  ui->loop();
  

}