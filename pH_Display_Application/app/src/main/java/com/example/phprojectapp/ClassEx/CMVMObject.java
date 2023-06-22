package com.example.phprojectapp.ClassEx;

public class CMVMObject {
    public float calibration = 0;
    public float m = 0;
    public float voltin = 0;
    public float max_analog = 0;
    public float analogAvg = 0;

    public float calVolt(){
        return analogAvg * voltin / max_analog;
    }

    public String calVoltString(){
        return String.format("%.2f = %.2f * %.2f / %.2f",calVolt(),analogAvg,voltin,max_analog);
    }

    public float calPH(){
        return m * calVolt() + calibration;
    }

    public String calPHString(){
        return String.format("%.2f = %.2f * %.2f + %.2f",calPH(),m,calVolt(),calibration);
    }
}
