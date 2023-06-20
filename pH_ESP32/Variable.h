#include "VarObject/TimerAutoWork.h"

class Variable
{

public:
  float input_ph;
  float mixTank_pH;
  float useTank_ph;
  TimerAutoWork *timerautowork = new TimerAutoWork[4];

  StringManager *strManager = new StringManager();

  float onClientRequestStatus = false;
  Variable()
  {
    input_ph = 0;
    mixTank_pH = 0;
    useTank_ph = 0;
  }

  // void setTimeList(String queryStirng);

};


// void Variable::setTimeList(String queryStirng)
// {
//   String datalayer1[4];
//   strManager->split(datalayer1, queryStirng, "|", 4);

//   for (int i = 0; i < 4; i++)
//   {
//     String datalayer2[5];
//     strManager->split(datalayer2, datalayer1[i], ",", 5);

//     timerautowork[i].setHour(datalayer2[0].toInt());
//     timerautowork[i].setMinute(datalayer2[1].toInt());
//     timerautowork[i].setSecond(datalayer2[2].toInt());
//     timerautowork[i].setStatus((datalayer2[3] == "true") ? true : false);
//     timerautowork[i].setPH(datalayer2[4].toFloat());
//     timerautowork[i].setDelete((datalayer2[0].toInt() == -1) ? true : false);
//   }
// }

