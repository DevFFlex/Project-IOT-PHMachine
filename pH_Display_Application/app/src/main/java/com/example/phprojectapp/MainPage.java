package com.example.phprojectapp;


import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentTransaction;
import androidx.preference.PreferenceManager;

import java.io.IOException;

import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
import com.example.phprojectapp.ClassEx.*;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragment;

public class MainPage extends AppCompatActivity {

    private SharedPreferences preferences;
    private Variable var;

    Button btn_connectServer,btn_disonnectServer,btn1,btn2,btn3,btn4,btn5,btn_chatsend;
    TextView mainpage_tvStatusConnected,main_tvOutputDisplay;
    LinearLayout mainpage_chatInputLayout,mainpage_outputDisplayLayout;
    EditText input;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainpage);


        var = new Variable(this);
        preferences = PreferenceManager.getDefaultSharedPreferences(this);


        mainpage_tvStatusConnected = findViewById(R.id.mainpage_tvStatusConnected);
        main_tvOutputDisplay = findViewById(R.id.main_tvOutputDisplay);
        mainpage_chatInputLayout = findViewById(R.id.mainpage_chatInputLayout);
        mainpage_outputDisplayLayout = findViewById(R.id.mainpage_outputDisplayLayout);

        btn1 = findViewById(R.id.mainpage_btn1);
        btn2 = findViewById(R.id.mainpage_btn2);
        btn3 = findViewById(R.id.mainpage_btn3);
        btn4 = findViewById(R.id.mainpage_btn4);
        btn5 = findViewById(R.id.mainpage_btn5);
        btn_chatsend =findViewById(R.id.mainpage_btnsendchat);
        input = findViewById(R.id.mainpage_inputchat);
        btn_connectServer = findViewById(R.id.mainpage_btnConnect);
        btn_disonnectServer = findViewById(R.id.mainpage_btnDisonnect);

        //animation
        var.animationOption.startAnim(btn3,R.anim.fadein_slide_right);
        var.animationOption.startAnim(btn1,R.anim.fadein_slide_center);
        var.animationOption.startAnim(btn2,R.anim.fadein_slide_left);

        //start fragment
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.monitorFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));
        mainpage_chatInputLayout.setVisibility(View.GONE);


        btn1.setOnClickListener(this::onClickMenu_1);
        btn2.setOnClickListener(this::onClickMenu_2);
        btn3.setOnClickListener(this::onClickMenu_3);
        btn4.setOnClickListener(this::onClickMenu_4);
        btn5.setOnClickListener(this::onClickMenu_5);
        btn_chatsend.setOnClickListener(this::onChatSend);

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


        if(!var.outout_text.equals("")){
            mainpage_outputDisplayLayout.setVisibility(View.VISIBLE);
            main_tvOutputDisplay.setText(var.outout_text);
            new Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    mainpage_outputDisplayLayout.setVisibility(View.GONE);
                }
            },2000);
            var.outout_text = "";
        }

        if (getSupportFragmentManager().findFragmentByTag("chat_fragment") == null){
            mainpage_chatInputLayout.setVisibility(View.GONE);
        }else{
            mainpage_chatInputLayout.setVisibility(View.VISIBLE);
        }




    }


    public void onClickMenu_1(View v) {
        if (getSupportFragmentManager().findFragmentByTag("monitor_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.monitorFragment, "monitor_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_2(View v) {
        if (getSupportFragmentManager().findFragmentByTag("settings_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.settingsFragment, "settings_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_3(View v) {
        if (getSupportFragmentManager().findFragmentByTag("admin_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.adminFragment, "admin_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_4(View v) {
        if (getSupportFragmentManager().findFragmentByTag("chat_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.chatFragment, "chat_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));


//            mainpage_chatInputLayout.setVisibility(View.VISIBLE);
        }
    }

    public void onClickMenu_5(View v) {
        if (getSupportFragmentManager().findFragmentByTag("file_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.fileFragment, "file_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }



    public void onClickConnect(View v){
        String ip = preferences.getString("ip",var.comunity.SERVER_IP);
        String port = preferences.getString("port",String.valueOf(var.comunity.SERVER_PORT));
        String name = preferences.getString("username",var.comunity.USERNAME);

        var.comunity.connect(ip,Integer.valueOf(port),name);
        var.extension.printDebug("MainPage","onClickConeect");
    }

    public void onClickDisconnect(View v){
        var.comunity.disconnect();
    }

    public void onChatSend(View v){
        String text = input.getText().toString();
        if (text.equals("")) return;
        var.comunity.sendMessageChat(var.comunity.USERNAME, text);
        var.extension.printAlert("sending");
        input.setText("");
    }




}