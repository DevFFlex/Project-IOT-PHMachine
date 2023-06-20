class Database
{
  // String SETTING_PATH =
  Variable *var;
  HardwareIO *hardwareIO;
  Timer t1;

public:
  Database(Variable *varIn, HardwareIO *hardware) : t1(3000)
  {
    var = varIn;
    hardwareIO = hardware;
  }

  void setup();
  void loop();

  String getDir();
};

void Database::setup()
{
  Serial.println("before ----------------------------");
  hardwareIO->sdcard->listDir("/", 0);

  hardwareIO->sdcard->removeDir("LOST.DIR");
  hardwareIO->sdcard->removeDir("/Music");
  // hardwareIO->sdcard->removeDir("Movies");
  // hardwareIO->sdcard->removeDir("Pictures");
  // hardwareIO->sdcard->removeDir("Android");


  Serial.println("after ----------------------------");
  hardwareIO->sdcard->listDir("/", 1);
}

void Database::loop()
{

  if (t1.isExpired())
  {
    
  }
}



String Database::getDir(){

}