package com.example.phprojectapp.ClassEx;

import java.util.ArrayList;
import java.util.List;


public class TimeObjectList {
    public ArrayList<TimeObject> data = new ArrayList<TimeObject>();

    TimeObjectList(){

    }

    public void addItem(int hour,int minute,int second,boolean status,float ph){
        TimeObject timeObject = new TimeObject();
        timeObject.setHour(hour);
        timeObject.setMinute(minute);
        timeObject.setSecond(second);
        timeObject.setStatus(status);
        timeObject.setPh(ph);

        data.add(timeObject);
    }

    public TimeObject getTimeObject(TimeObject timeObject){
        return data.get(data.indexOf(timeObject));
    }

    public void clearAllItem(){
        this.data.clear();
    }

    public int getSize(){
        return this.data.size();
    }

}