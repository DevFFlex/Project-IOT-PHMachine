package com.example.phprojectapp.ClassEx;

public class Variable{

    public float getInputPH() {
        return inputPH;
    }

    public void setInputPH(float inputPH) {
        this.inputPH = inputPH;
    }

    public float getMixtankPH() {
        return mixtankPH;
    }

    public void setMixtankPH(float mixtankPH) {
        this.mixtankPH = mixtankPH;
    }

    public float getUsetankPH() {
        return usetankPH;
    }

    public void setUsetankPH(float usetankPH) {
        this.usetankPH = usetankPH;
    }

    public TimeBoardObject getTimeBoardObject() {
        return timeBoardObject;
    }

    public void setTimeBoardObject(TimeBoardObject timeBoardObject) {
        this.timeBoardObject = timeBoardObject;
    }

    public Variable(){
        this.inputPH = 0;
        this.mixtankPH = 0;
        this.usetankPH = 0;

        this.timeObjectList = new TimeObjectList();
    }

    private float inputPH;
    private float mixtankPH;
    private float usetankPH;
    private TimeObjectList timeObjectList;



    private TimeBoardObject timeBoardObject;
    private String outout_text = "";


    public String getOutout_text() {
        return outout_text;
    }

    public void setOutout_text(String outout_text) {
        this.outout_text = outout_text;
    }

    public TimeObjectList getTimeObjectList(){
        return this.timeObjectList;
    }


}
