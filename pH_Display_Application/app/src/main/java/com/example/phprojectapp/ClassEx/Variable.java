package com.example.phprojectapp.ClassEx;

public class Variable{

    public float inputPH;
    public float mixtankPH;
    public float usetankPH;
    public TimeObjectList timeObjectList;
    public TimeBoardObject timeBoardObject;
    public String outout_text = "";

    public Variable(){
        this.inputPH = 0;
        this.mixtankPH = 0;
        this.usetankPH = 0;

        this.timeObjectList = new TimeObjectList();
    }


}
