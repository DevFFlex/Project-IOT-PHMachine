package com.example.phprojectapp.ClassEx;

public class TimeObject {
    private int hour;
    private int minute;
    private boolean status;
    private float ph;

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

    public boolean isStatus() {
        return status;
    }

    public void setStatus(boolean status) {
        this.status = status;
    }

    public float getPh() {
        return ph;
    }

    public void setPh(float ph) {
        this.ph = ph;
    }

    public String toString(){
        String out = "";

        out+= (hour < 10) ? "0":""+ String.valueOf(hour) + ":";
        out+= (minute < 10) ? "0":""+ String.valueOf(minute) + ":";

        return out;
    }


}
