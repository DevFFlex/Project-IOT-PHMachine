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
Comunity *comunity = new Comunity(var);
PHAdjustmentProcess *phap = new PHAdjustmentProcess(var,comunity);
SerialInput *sInput = new SerialInput(var,comunity);
UserInterface *ui = new UserInterface(var,comunity);



void setup() {
  Serial.begin(115200);

  var->setup();
  comunity->setup();
  phap->setup();
  sInput->setup();
  ui->setup();
}

void loop() {
  var->loop();
  comunity->loop();
  phap->loop();
  sInput->loop();
  ui->loop();
}