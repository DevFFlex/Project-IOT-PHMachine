package com.example.phprojectapp.Variable;

public class TimeBoardObject {
    public int hour;
    public int minute;
    public int second;

    public int dayofweek;
    public int dayofmonth;
    public int month;
    public int year;

    public TimeBoardObject(){
        hour = -1;
        minute = -1;
        second = -1;

        dayofweek = -1;
        dayofmonth = -1;
        month = -1;
        year = -1;
    }



    public boolean isEmptyAll(){
        return (hour == -1 || minute == -1 || second == -1 || dayofweek == -1 || dayofmonth == -1 || month == -1 || year == -1);
    }

}
