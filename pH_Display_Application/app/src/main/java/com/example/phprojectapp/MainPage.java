package com.example.phprojectapp;


import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.preference.PreferenceManager;

import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
import com.example.phprojectapp.ClassEx.TimerMillis;
import com.example.phprojectapp.ClassEx.TimerMillisInterface;
import com.example.phprojectapp.Variable.Variable;

public class MainPage extends AppCompatActivity {


    private Variable var;

    Button btn_connectServer,btn_disonnectServer,btnHome,btnSetting,btnAdmin,btnChat,btnFile,btn_chatsend,btnWifiManager;
    TextView mainpage_tvStatusConnected,main_tvOutputDisplay;
    LinearLayout mainpage_chatInputLayout,mainpage_outputDisplayLayout;
    EditText input;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainpage);


        var = new Variable(this);



        mainpage_tvStatusConnected = findViewById(R.id.mainpage_tvStatusConnected);
        main_tvOutputDisplay = findViewById(R.id.main_tvOutputDisplay);
        mainpage_chatInputLayout = findViewById(R.id.mainpage_chatInputLayout);
        mainpage_outputDisplayLayout = findViewById(R.id.mainpage_outputDisplayLayout);

        btnHome = findViewById(R.id.mainpage_btnHome);
        btnSetting = findViewById(R.id.mainpage_btnSetting);
        btnAdmin = findViewById(R.id.mainpage_btnAdmin);
        btnChat = findViewById(R.id.mainpage_btnChat);
        btnFile = findViewById(R.id.mainpage_btnFile);
        btnWifiManager = findViewById(R.id.mainpage_btnWifiSet);
        btn_chatsend =findViewById(R.id.mainpage_btnsendchat);
        input = findViewById(R.id.mainpage_inputchat);
        btn_connectServer = findViewById(R.id.mainpage_btnConnect);
        btn_disonnectServer = findViewById(R.id.mainpage_btnDisonnect);

        //animation
        var.animationOption.startAnim(btnAdmin,R.anim.fadein_slide_right);
        var.animationOption.startAnim(btnSetting,R.anim.fadein_slide_right);
        var.animationOption.startAnim(btnHome,R.anim.fadein_slide_right);
        var.animationOption.startAnim(btnChat,R.anim.fadein_slide_left);
        var.animationOption.startAnim(btnWifiManager,R.anim.fadein_slide_left);
        var.animationOption.startAnim(btnFile,R.anim.fadein_slide_left);

        //start fragment
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.monitorFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));
        mainpage_chatInputLayout.setVisibility(View.GONE);


        btnAdmin.setOnClickListener(this::onClickMenu_Admin);
        btnSetting.setOnClickListener(this::onClickMenu_Setting);
        btnHome.setOnClickListener(this::onClickMenu_Home);
        btnWifiManager.setOnClickListener(this::onClickMenu_WifiManager);
        btnChat.setOnClickListener(this::onClickMenu_Chat);
        btnFile.setOnClickListener(this::onClickMenu_File);

        btn_connectServer.setOnClickListener(this::onClickConnect);
        btn_disonnectServer.setOnClickListener(this::onClickDisconnect);

        btnChat.setEnabled(false);
        btnFile.setEnabled(false);


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

        if (!var.comunity.isConnect) {
            mainpage_tvStatusConnected.setText("ยังไม่ได้เชื่อมต่อ");
            mainpage_tvStatusConnected.setTextColor(Color.rgb(255,0,0));
//            btn_connectServer.setVisibility(View.VISIBLE);
            btn_disonnectServer.setVisibility(View.GONE);
        } else {
            mainpage_tvStatusConnected.setText("เชื่อมต่อเเล้ว");
            mainpage_tvStatusConnected.setTextColor(Color.rgb(0,255,0));
            btn_connectServer.setVisibility(View.GONE);
//            btn_disonnectServer.setVisibility(View.VISIBLE);
        }


        if(!var.outout_text.equals("") && !var.output_text_isShowing){
            var.output_text_isShowing = true;
            mainpage_outputDisplayLayout.setVisibility(View.VISIBLE);
            main_tvOutputDisplay.setText(var.outout_text);
            new Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    mainpage_outputDisplayLayout.setVisibility(View.GONE);
                    var.output_text_isShowing = false;
                }
            },5000);
            var.outout_text = "";
        }

        if (getSupportFragmentManager().findFragmentByTag("chat_fragment") == null){
            mainpage_chatInputLayout.setVisibility(View.GONE);
        }else{
            mainpage_chatInputLayout.setVisibility(View.VISIBLE);
        }





    }


    public void onClickMenu_Home(View v) {
        if (getSupportFragmentManager().findFragmentByTag("monitor_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.monitorFragment, "monitor_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_Setting(View v) {
        if (getSupportFragmentManager().findFragmentByTag("settings_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.settingsFragment, "settings_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_Admin(View v) {
        if (getSupportFragmentManager().findFragmentByTag("admin_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.adminFragment, "admin_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_Chat(View v) {
        if (getSupportFragmentManager().findFragmentByTag("chat_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.chatFragment, "chat_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));


//            mainpage_chatInputLayout.setVisibility(View.VISIBLE);
        }
    }

    public void onClickMenu_File(View v) {
        if (getSupportFragmentManager().findFragmentByTag("file_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.fileFragment, "file_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }
    }

    public void onClickMenu_WifiManager(View v) {
        if (getSupportFragmentManager().findFragmentByTag("file_fragment") == null) {
            getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, var.fileFragment, "file_fragment").commit();
            YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

//            mainpage_chatInputLayout.setVisibility(View.GONE);
        }

    }



    public void onClickConnect(View v){
        var.comunity.connect();
    }

    public void onClickDisconnect(View v){
        var.comunity.disconnect();
    }

    public void onChatSend(View v){
        String text = input.getText().toString();
        if (text.equals("")) return;
        var.comunity.sendMessageChat(var.preferences.getString("username",var.comunity.DEFAULT_USERNAME), text);
        var.extension.printAlert("sending");
        input.setText("");
    }




}