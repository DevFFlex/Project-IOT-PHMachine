package com.example.phprojectapp.Variable;

import java.util.ArrayList;


public class WorkTimerList {
    public ArrayList<WorkTimer> workTimers_item = new ArrayList<WorkTimer>();


    WorkTimerList(){
        workTimers_item.add(new WorkTimer());
        workTimers_item.add(new WorkTimer());
        workTimers_item.add(new WorkTimer());
        workTimers_item.add(new WorkTimer());
    }

    public void AddItem(int hour,int minute,float ph,int T,boolean active_status,boolean delete_Status){
        WorkTimer workTimer = new WorkTimer();
        workTimer.HOUR = hour;
        workTimer.MINUTE = minute;
        workTimer.PH = ph;
        workTimer.T = T;
        workTimer.ACTIVE_STATUS = active_status;
        workTimer.DELETE_STATUS = delete_Status;

        workTimers_item.add(workTimer);
    }

    public WorkTimer getWorkTimer(int index){
        return workTimers_item.get(index);
    }


}
