package com.example.phprojectapp.MonitorFragmentObject;

import com.example.phprojectapp.ClassEx.TimeBoardObject;

public interface MonitorFragmentOnEvent {



    void dialogSetPH_onclick_OK(float ph_value);

    void dialogTest_onclick_OK(String text);

    void dialogSetTime_onclick_Save(String time_str);

    void dialogSetTimeBoard_onClick_OK(TimeBoardObject timeBoardObject);

}
