
#include "OtherClass/TimerClass.h"
#include "OtherClass/StringManager.h"
#include "OtherClass/I2CScanner.h"

#include "Variable.h"
#include "HardwareIO.h"
#include "Database.h"
#include "ArduinoComunity.h"

#include "Comunity.h"
#include "SerialInput.h"
#include "UserInterface.h"

#include "PHAdjustmentProcess.h"


Variable *var = new Variable();
HardwareIO *hardwareIO = new HardwareIO(var);
Database *db = new Database(var, hardwareIO);
ArduinoComunity *ardunoComunity = new ArduinoComunity(var, hardwareIO);

Comunity *comunity = new Comunity(var, hardwareIO, db, ardunoComunity);

SerialInput *sInput = new SerialInput(var, hardwareIO, comunity, ardunoComunity,db);
UserInterface *ui = new UserInterface(var, hardwareIO, comunity);

PHAdjustmentProcess *phAP = new PHAdjustmentProcess(var,hardwareIO,comunity);


void setup() {
  Serial.begin(115200);

  hardwareIO->setup();
  db->setup();
  ardunoComunity->setup();
  comunity->setup();
  sInput->setup();
  ui->setup();
  phAP->setup();
}

void loop() {
  
  hardwareIO->loop();
  db->loop();
  ardunoComunity->loop();
  comunity->loop();
  sInput->loop();
  ui->loop();
  phAP->loop();

}