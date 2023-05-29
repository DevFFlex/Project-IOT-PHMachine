package com.example.phprojectapp.ClassEx;

import java.util.ArrayList;
import java.util.List;

public class TimeObjectList {
    public List<TimeObject> timeObjectList;

    TimeObjectList(){
        timeObjectList = new ArrayList<TimeObject>();
    }

    public void addItem(int hour,int minute,int second,boolean status,float ph){
        TimeObject timeObject = new TimeObject();
        timeObject.setHour(hour);
        timeObject.setMinute(minute);
        timeObject.setSecond(second);
        timeObject.setStatus(status);
        timeObject.setPh(ph);

        timeObjectList.add(timeObject);
    }

    public TimeObject getTimeObject(TimeObject timeObject){
        return timeObjectList.get(timeObjectList.indexOf(timeObject));
    }

    public void clearAllItem(){
        this.timeObjectList.clear();
    }

    public int getSize(){
        return this.timeObjectList.size();
    }

}
