package com.example.phprojectapp.ClassEx;

import android.content.Context;
import com.example.phprojectapp.AnimationOption;

public class Variable{

    public float inputPH;
    public float mixtankPH;
    public float usetankPH;
    public TimeObjectList timeObjectList;
    public TimeBoardObject timeBoardObject;
    public String outout_text = "";

    public AnimationOption animationOption;
    public SoundEffect soundEffect;
    public Extension extension;
    public Comunity comunity;

    public Variable(Context context){
        this.inputPH = 0;
        this.mixtankPH = 0;
        this.usetankPH = 0;
        this.timeObjectList = new TimeObjectList();

        animationOption = new AnimationOption(context);
        soundEffect = new SoundEffect(context);
        extension = new Extension(context);
        comunity = new Comunity(this);
    }


}
