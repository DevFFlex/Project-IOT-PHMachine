//---------------------------- controller function --------------------

#define Acid_Valve_PIN NULL
#define Bass_Valve_PIN NULL
#define WaterIn_Valve_PIN NULL
#define WaterOut_Valve_PIN NULL

enum Valve{
  ACID_VALVE,
  BASE_VALVE,
  WATER_IN_VALVE,
  WATER_OUT_VALVE
};

void ValveControll(Valve valvename,boolean status){

  if(valvename == ACID_VALVE){
    digitalWrite(Acid_Valve_PIN,status);
  }else if (valvename == BASE_VALVE){
    digitalWrite(Bass_Valve_PIN,status);
  }else if (valvename == WATER_IN_VALVE){
    digitalWrite(WaterIn_Valve_PIN,status);
  }else if (valvename == WATER_OUT_VALVE){
    digitalWrite(WaterOut_Valve_PIN,status);
  }

}

#define Turbine_PIN NULL
void TurbineControll(boolean status){
  digitalWrite(Turbine_PIN,status);
}


#define FloatSWTOP_PIN NULL // 1
#define FloatSWBOTTOM_PIN NULL // 2
boolean FloatSW_Get_Status(byte selectSW){

  switch(selectSW){
    case 1:
      return digitalRead(FloatSWTOP_PIN);
      break;
    case 2:
      return digitalRead(FloatSWBOTTOM_PIN);
      break;
  }

}


#define pHSensorPIN NULL
float pHSensorRead(){
  float C = 25.85; //Constant of straight line (Y = mx + C)
  float m = -6.80; // Slope of straight line (Y = mx + C)

  float phAvg = 0,phTot = 0;
  float V = 5.0; // Volt

  for(int x=0; x<20 ; x++){
    phTot += analogRead(pHSensorPIN);
    delay(10);
  }

  phAvg = phTot / 20;
  
  float phVoltage =  phAvg * (V / 1023.0);
  
  float pHValue = phVoltage * m+C;


  return pHValue;

}