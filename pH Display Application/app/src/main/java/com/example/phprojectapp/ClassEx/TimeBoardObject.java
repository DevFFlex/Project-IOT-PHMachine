package com.example.phprojectapp.ClassEx;

public class TimeBoardObject {

    public int getHour() {
        return hour;
    }

    public void setHour(int hour) {
        this.hour = hour;
    }

    public int getMinute() {
        return minute;
    }

    public void setMinute(int minute) {
        this.minute = minute;
    }

    public int getSecond() {
        return second;
    }

    public void setSecond(int second) {
        this.second = second;
    }

    public int getDayofweek() {
        return dayofweek;
    }

    public void setDayofweek(int dayofweek) {
        this.dayofweek = dayofweek;
    }

    public int getDayofmonth() {
        return dayofmonth;
    }

    public void setDayofmonth(int dayofmonth) {
        this.dayofmonth = dayofmonth;
    }

    public int getMonth() {
        return month;
    }

    public void setMonth(int month) {
        this.month = month;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    private int hour;
    private int minute;
    private int second;

    private int dayofweek;
    private int dayofmonth;
    private int month;
    private int year;

    public TimeBoardObject(){
        hour = -1;
        minute = -1;
        second = -1;

        dayofweek = -1;
        dayofmonth = -1;
        month = -1;
        year = -1;
    }

    public void setOjbect(TimeBoardObject timeBoardObject){
        this.hour = timeBoardObject.getHour();
        this.minute = timeBoardObject.getMinute();
        this.second = timeBoardObject.getSecond();
        this.dayofweek = timeBoardObject.getDayofweek();
        this.dayofmonth = timeBoardObject.getDayofmonth();
        this.month = timeBoardObject.getMonth();
        this.year = timeBoardObject.getYear();
    }

    public String getTimeFormat(){
        return String.format("%02d:%02d:%02d",hour,minute,second);
    }

    public String getDateTimeFormat(){
        return String.format("%02d:%02d:%02d   %02d/%02d/%02d  %02d",hour,minute,second,dayofmonth,month,year,dayofweek);
    }

    public boolean isEmptyAll(){
        return (hour == -1 || minute == -1 || second == -1 || dayofweek == -1 || dayofmonth == -1 || month == -1 || year == -1);
    }

}
