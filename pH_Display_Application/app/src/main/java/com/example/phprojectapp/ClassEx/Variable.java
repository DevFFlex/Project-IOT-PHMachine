package com.example.phprojectapp.ClassEx;

import android.content.Context;

import com.example.phprojectapp.AdminFragment;
import com.example.phprojectapp.AnimationOption;
import com.example.phprojectapp.ChatFragment;
import com.example.phprojectapp.FileFragment;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragment;
import com.example.phprojectapp.SettingsFragment;

import java.util.ArrayList;

public class Variable{
    public Context context;

    public float inputPH;
    public float mixtankPH;
    public float usetankPH;
    public TimeObjectList timeObjectList;
    public TimeBoardObject timeBoardObject;
    public String outout_text = "";
    public ArrayList<String[]> chat_his = new ArrayList<String[]>();
    public ArrayList<String[]> file_list = new ArrayList<>();

    public AnimationOption animationOption;
    public SoundEffect soundEffect;
    public Extension extension;
    public Comunity comunity;

    public MonitorFragment monitorFragment;
    public SettingsFragment settingsFragment;
    public AdminFragment adminFragment;
    public ChatFragment chatFragment;
    public FileFragment fileFragment;

    public Variable(Context context){
        this.context = context;

        monitorFragment = new MonitorFragment(this);
        settingsFragment = new SettingsFragment();
        adminFragment = new AdminFragment(this);
        chatFragment = new ChatFragment(this);
        fileFragment = new FileFragment(this);

        this.inputPH = 0;
        this.mixtankPH = 7;
        this.timeObjectList = new TimeObjectList();

        animationOption = new AnimationOption(context);
        soundEffect = new SoundEffect(context);
        extension = new Extension(context);
        comunity = new Comunity(this);

    }

}
