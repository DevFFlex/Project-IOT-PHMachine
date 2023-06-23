#include "OtherClass/TimerClass.h"
#include "OtherClass/StringManager.h"

#include "Variable.h"
#include "HardwareIO.h"
#include "Database.h"
#include "ArduinoComunity.h"

#include "Comunity.h"
#include "SerialInput.h"
#include "UserInterface.h"

#include "Work.h"


Variable *var = new Variable();
HardwareIO *hardwareIO = new HardwareIO();
Database *db = new Database(var, hardwareIO);
ArduinoComunity *ardunoComunity = new ArduinoComunity(var, hardwareIO);

Comunity *comunity = new Comunity(var, hardwareIO, db, ardunoComunity);


SerialInput *sInput = new SerialInput(var, hardwareIO, comunity, ardunoComunity);
UserInterface *ui = new UserInterface(var, hardwareIO, comunity);

Work *work = new Work(var,hardwareIO,comunity);



void setup() {
  Serial.begin(115200);

  hardwareIO->setup();
  db->setup();
  ardunoComunity->setup();
  comunity->setup();
  sInput->setup();
  ui->setup();
  work->setup();
}

void loop() {
  
  hardwareIO->loop();
  db->loop();
  ardunoComunity->loop();
  comunity->loop();
  sInput->loop();
  ui->loop();
  work->loop();

}


