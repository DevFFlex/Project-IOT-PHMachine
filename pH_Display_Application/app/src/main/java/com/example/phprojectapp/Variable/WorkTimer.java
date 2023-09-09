package com.example.phprojectapp.Variable;

public class WorkTimer {
    public int HOUR = -1;
    public int MINUTE = -1;
    public float PH = -1;
    public int T = -1;
    public boolean ACTIVE_STATUS = false;
    public boolean DELETE_STATUS = true;


    public String timeFormattoString(){
        String out = "";



        return String.format("%02d", HOUR) + ":" + String.format("%02d", MINUTE) + ":--";
    }

    public String show(){
        String out = String.valueOf(HOUR) + "\t";
        out += String.valueOf(MINUTE) + "\t";
        out += String.valueOf(PH) + "\t";
        out += String.valueOf(T) + "\t";
        out += String.valueOf(ACTIVE_STATUS) + "\t";
        out += String.valueOf(DELETE_STATUS) + "\t";

        return out;
    }


}
