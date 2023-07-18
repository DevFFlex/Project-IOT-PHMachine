package com.example.phprojectapp.Variable;

import android.content.Context;

import com.example.phprojectapp.AdminFragment;
import com.example.phprojectapp.AnimationOption;
import com.example.phprojectapp.ChatFragment;
import com.example.phprojectapp.ClassEx.CMVMObject;
import com.example.phprojectapp.ClassEx.Comunity;
import com.example.phprojectapp.ClassEx.Extension;
import com.example.phprojectapp.ClassEx.SoundEffect;
import com.example.phprojectapp.FileFragment;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragment;
import com.example.phprojectapp.SettingsFragment;
import com.example.phprojectapp.MonitorFragmentObject.*;

import java.util.ArrayList;

public class Variable{
    public Context context;

    public float inputPH = -1;
    public float mixtankPH = 0;
    public float tempC = 0;
    public float humidity = 0;

    public int step = 0;
    public boolean work_status = false;
    public String stepText = "";

    public boolean relay_status[] = {
            false,
            false,
            false,
            false,
            false,
            false
    };

    public CMVMObject cmvmObject;
    public TimeBoardObject timeBoardObject;



//    public boolean working_ph = false;
    public TimeObjectList timeObjectList;
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


    public MMain step3;

    public Variable(Context context){
        this.context = context;

        monitorFragment = new MonitorFragment(this);
        settingsFragment = new SettingsFragment();
        adminFragment = new AdminFragment(this);
        chatFragment = new ChatFragment(this);
        fileFragment = new FileFragment(this);


        step3 = new MMain(this);


        this.timeObjectList = new TimeObjectList();
        this.cmvmObject = new CMVMObject();


        animationOption = new AnimationOption(context);
        soundEffect = new SoundEffect(context);
        extension = new Extension(context);
        comunity = new Comunity(this);

    }

}
