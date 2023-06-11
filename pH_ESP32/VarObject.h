class TimerAutoWork {
private:
  int hour;
  int minute;
  int second;
  float ph;
  bool status;
  bool delete_;

public:
  TimerAutoWork() {
    hour = -1;
    minute = -1;
    second = -1;
    ph = -1;
    status = false;
    delete_ = true;
  }


  void setHour(int hourIn) {
    hour = hourIn;
  }

  void setMinute(int minuteIn) {
    minute = minuteIn;
  }

  void setSecond(int secondIn) {
    second = secondIn;
  }

  void setPH(float phIn) {
    ph = phIn;
  }

  void setStatus(bool statusIn) {
    status = statusIn;
  }

  void setDelete(bool deleteIn) {
    delete_ = deleteIn;
  }

  int getHour() {
    return hour;
  }

  int getMinute() {
    return minute;
  }

  int getSecond() {
    return second;
  }

  float getPH() {
    return ph;
  }

  bool getStatus() {
    return status;
  }

  bool getDelete() {
    return delete_;
  }



  String toString() {
    char buffer[50];
    
    sprintf(buffer, "%02d:%02d:%02d  status = %s   ph = %.1f   delete = %s", hour,minute, second,String(status),ph,String(delete_));

    String result = String(buffer);

    return result;
  }
};



typedef struct TimeObjectStruct {
  int hour = -1;
  int minute = -1;
  int second = -1;
  float ph = -1;
  boolean status = false;
  boolean delete_ = true;
} TimeObject;



class VarObject {
private:
  float input_ph;
  float mixTank_pH;
  float useTank_ph;
  
  StringManage stringManage;

public:

  TimerAutoWork *timerautowork = new TimerAutoWork[4];

  VarObject() {
    input_ph = 0;
    mixTank_pH = 0;
    useTank_ph = 0;
  }


  void setInputPH(float input_phIn);
  void setMixTankpH(float mixTank_pHIn);
  void setUseTankPH(float useTank_phIn);
  void setTimeList(String queryStirng);

  float getInputPH();
  float getMixTankpH();
  float getUseTankPH();

  String getTimeListToString();
  int getTimeListSize();
};







void VarObject::setTimeList(String queryStirng) {
  String datalayer1[4];
  stringManage.split(datalayer1, queryStirng, "|", 4);

  for (int i = 0; i < 4; i++) {
    String datalayer2[5];
    stringManage.split(datalayer2, datalayer1[i], ",", 5);

    timerautowork[i].setHour(datalayer2[0].toInt());
    timerautowork[i].setMinute(datalayer2[1].toInt());
    timerautowork[i].setSecond(datalayer2[2].toInt());
    timerautowork[i].setStatus((datalayer2[3] == "true") ? true : false);
    timerautowork[i].setPH(datalayer2[4].toFloat());
    timerautowork[i].setDelete((datalayer2[0].toInt() == -1) ? true : false);
  }
}

void VarObject::setInputPH(float input_phIn) {
  input_ph = input_phIn;
}

void VarObject::setMixTankpH(float mixTank_pHIn) {
  mixTank_pH = mixTank_pHIn;
}

void VarObject::setUseTankPH(float useTank_phIn) {
  useTank_ph = useTank_phIn;
}


float VarObject::getInputPH() {
  return input_ph;
}

float VarObject::getMixTankpH() {
  return mixTank_pH;
}

float VarObject::getUseTankPH() {
  return useTank_ph;
}

String VarObject::getTimeListToString() {
  String dataout = "";
  for (int i = 0; i < 4; i++) {
    dataout += stringManage.intToText(timerautowork[i].getHour(), 2) + ":" + stringManage.intToText(timerautowork[i].getMinute(), 2) + ":" + stringManage.intToText(timerautowork[i].getSecond(), 2);
    dataout += "      ph=" + String(timerautowork[i].getPH());
    dataout += "      statusActive=" + String(timerautowork[i].getStatus());
    dataout += "      statusDelete=" + String(timerautowork[i].getDelete());
    dataout += "\n";
  }

  return dataout;
}

int VarObject::getTimeListSize(){
  int count = 0;
  for (int i = 0;i<4;i++)if(timerautowork[i].getHour() != -1)count++;
  return count;
}
