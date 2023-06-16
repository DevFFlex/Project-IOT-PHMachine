package com.example.phprojectapp;


import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.preference.PreferenceManager;

import java.io.IOException;

import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
import com.example.phprojectapp.ClassEx.*;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragment;

public class MainPage extends AppCompatActivity {

    private SharedPreferences preferences;
    private Variable var;

    Button btn_connectServer,btn_disonnectServer,btn1,btn2,btn3,btn4;
    TextView mainpage_tvStatusConnected;

    private MonitorFragment monitorFragment;
    private SettingsFragment settingsFragment;
    private AdminFragment adminFragment;
    private ChatFragment chatFragment;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainpage);


        var = new Variable(this);
        preferences = PreferenceManager.getDefaultSharedPreferences(this);

        monitorFragment = new MonitorFragment(var);
        settingsFragment = new SettingsFragment();
        adminFragment = new AdminFragment(var);
        chatFragment = new ChatFragment(var);

        mainpage_tvStatusConnected = findViewById(R.id.mainpage_tvStatusConnected);

        btn1 = findViewById(R.id.mainpage_btn1);
        btn2 = findViewById(R.id.mainpage_btn2);
        btn3 = findViewById(R.id.mainpage_btn3);
        btn4 = findViewById(R.id.mainpage_btn4);
        btn_connectServer = findViewById(R.id.mainpage_btnConnect);
        btn_disonnectServer = findViewById(R.id.mainpage_btnDisonnect);

        //animation
        var.animationOption.startAnim(btn3,R.anim.fadein_slide_right);
        var.animationOption.startAnim(btn1,R.anim.fadein_slide_center);
        var.animationOption.startAnim(btn2,R.anim.fadein_slide_left);

        //start fragment
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, monitorFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));


        btn1.setOnClickListener(this::onClickMenu_1);
        btn2.setOnClickListener(this::onClickMenu_2);
        btn3.setOnClickListener(this::onClickMenu_3);
        btn4.setOnClickListener(this::onClickMenu_4);

        btn_connectServer.setOnClickListener(this::onClickConnect);
        btn_disonnectServer.setOnClickListener(this::onClickDisconnect);


        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                updateUI();
                handler.postDelayed(this, 10);
            }
        });

    }

    public void updateUI() {
//        String ipValue = preferences.getString("ip", "0.0.0.0");

        if (!var.comunity.getConnected()) {
            mainpage_tvStatusConnected.setText("ยังไม่ได้เชื่อมต่อ");
            mainpage_tvStatusConnected.setTextColor(Color.rgb(255,0,0));
            btn_connectServer.setVisibility(View.VISIBLE);
            btn_disonnectServer.setVisibility(View.GONE);
        } else {
            mainpage_tvStatusConnected.setText("เชื่อมต่อเเล้ว");
            mainpage_tvStatusConnected.setTextColor(Color.rgb(0,255,0));
            btn_connectServer.setVisibility(View.GONE);
            btn_disonnectServer.setVisibility(View.VISIBLE);
        }



    }


    public void onClickMenu_1(View v){
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, monitorFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

    }

    public void onClickMenu_2(View v){
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, settingsFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));
    }


    public void onClickMenu_3(View v){
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, adminFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));
    }

    public void onClickMenu_4(View v){
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, chatFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));
    }


    public void onClickConnect(View v){
        String ip = preferences.getString("ip",var.comunity.SERVER_IP);
        String port = preferences.getString("port",String.valueOf(var.comunity.SERVER_PORT));
        String name = preferences.getString("username",var.comunity.USERNAME);

        var.extension.printAlert("ip : " + ip + "\tport : " + port);
        var.comunity.connect(ip,Integer.valueOf(port),name);
    }

    public void onClickDisconnect(View v){
        try {
            var.comunity.disconnect();
            Log.d("Complete","dissconnect");
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("Error","can not dissconnect");
        }
    }




}