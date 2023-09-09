package com.example.phprojectapp.Variable;

import android.content.Context;
import android.content.SharedPreferences;

import androidx.preference.PreferenceManager;

import com.example.phprojectapp.AdminFragment;
import com.example.phprojectapp.AnimationOption;
import com.example.phprojectapp.ChatFragment;
import com.example.phprojectapp.ClassEx.Comunity;
import com.example.phprojectapp.ClassEx.Extension;
import com.example.phprojectapp.ClassEx.SoundEffect;
import com.example.phprojectapp.FileFragment;
import com.example.phprojectapp.InternetManagerFragment;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragment;
import com.example.phprojectapp.SettingsFragment;
import com.example.phprojectapp.MonitorFragmentObject.*;

import java.util.ArrayList;

public class Variable{
    public Context context;
    public SharedPreferences preferences;

    public float inputPH = -1;
    public int inputT = -1;
    public float mixtankPH = 7;
    public float tempC = 0;
    public float humidity = 0;

    public int step = 0;
    public boolean work_status = false;
    public float pH_space_rate = 0;
    public float adjustCurrentPH = 0;
    public int wait_stirringPump = 0;
    public int wait_pHStabilize = 0;
    public int acidUseTime = 0;
    public int baseUseTime = 0;
    public int limite_use_acid = 0;
    public int limite_use_base = 0;
    public int adjustT_Counter = 0;
    public String stepText = "";

    public boolean internet_connected = false;

    public int[] fsw = new int[3];

    public boolean relay_status[] = {
            false,
            false,
            false,
            false,
            false,
            false
    };

    public TimeBoardObject timeBoardObject;



//    public boolean working_ph = false;
    public WorkTimerList workTimerList;
    public String outout_text = "";
    public boolean output_text_isShowing = false;

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
    public InternetManagerFragment internetManagerFragment;


    public MMain step3;

    public Variable(Context context){
        this.context = context;

        preferences = PreferenceManager.getDefaultSharedPreferences(context);

        monitorFragment = new MonitorFragment(this);
        settingsFragment = new SettingsFragment();
        adminFragment = new AdminFragment(this);
        chatFragment = new ChatFragment(this);
        fileFragment = new FileFragment(this);
        internetManagerFragment = new InternetManagerFragment(this);


        step3 = new MMain(this);


        this.workTimerList = new WorkTimerList();

        timeBoardObject = new TimeBoardObject();


        animationOption = new AnimationOption(context);
        soundEffect = new SoundEffect(context);
        extension = new Extension(context);
        comunity = new Comunity(this);

    }

}
