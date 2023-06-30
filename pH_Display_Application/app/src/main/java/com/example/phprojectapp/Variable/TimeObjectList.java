package com.example.phprojectapp.Variable;

import com.example.phprojectapp.ClassEx.TimeObject;

import java.util.ArrayList;
import java.util.List;


public class TimeObjectList {
    public ArrayList<TimeObject> data = new ArrayList<TimeObject>();

    TimeObjectList(){

    }

    public void addItem(int hour,int minute,boolean status,float ph){
        TimeObject timeObject = new TimeObject();
        timeObject.setHour(hour);
        timeObject.setMinute(minute);
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
