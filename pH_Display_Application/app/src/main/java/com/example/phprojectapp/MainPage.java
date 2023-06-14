package com.example.phprojectapp;


import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;
import androidx.preference.PreferenceManager;

import java.io.IOException;

import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
import com.example.phprojectapp.ClassEx.*;

public class MainPage extends AppCompatActivity {

    private SoundEffect soundEffect;
    private Extension extension;
    private AnimationOption animationOption;
    private SharedPreferences preferences;
    private Variable varriable;

    private ClientClass clientClass;
    private Comunity comunity;
    final String SERVER_IP = "192.168.4.1"; //192.168.4.1
    final int SERVER_PORT = 80; //80

    Button btn_connectServer,btn_disonnectServer,btn1,btn2,btn3;
    TextView mainpage_tvStatusConnected;
    FrameLayout fragment_container;

    private MonitorFragment monitorFragment;
    private SettingsFragment settingsFragment;
    private AdminFragment adminFragment;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.mainpage);


        varriable = new Variable();
        preferences = PreferenceManager.getDefaultSharedPreferences(this);
        animationOption = new AnimationOption(this);
        soundEffect = new SoundEffect(this);
        extension = new Extension(this);
        clientClass = new ClientClass();

        comunity = new Comunity(clientClass,varriable);

        clientClass.setClientOnEventListener(comunity);

        monitorFragment = new MonitorFragment();
        monitorFragment.setClientClass(clientClass);
        monitorFragment.setVariable(varriable);
        monitorFragment.setAnimationOption(animationOption);
        monitorFragment.setSoundEffect(soundEffect);
        monitorFragment.setExtension(extension);
        monitorFragment.setComunity(comunity);

        settingsFragment = new SettingsFragment();

        adminFragment = new AdminFragment();
        adminFragment.setComunity(comunity);
        adminFragment.setVariable(varriable);
        adminFragment.setClientClass(clientClass);




//        btn_connectServer = findViewById(R.id.btn_connect2server);
        mainpage_tvStatusConnected = findViewById(R.id.mainpage_tvStatusConnected);

        btn1 = findViewById(R.id.mainpage_btn1);
        btn2 = findViewById(R.id.mainpage_btn2);
        btn3 = findViewById(R.id.mainpage_btn3);
        btn_connectServer = findViewById(R.id.mainpage_btnConnect);
        btn_disonnectServer = findViewById(R.id.mainpage_btnDisonnect);
        fragment_container = findViewById(R.id.fragment_container);

        //animation
        animationOption.startAnim(btn3,R.anim.fadein_slide_right);
        animationOption.startAnim(btn1,R.anim.fadein_slide_center);
        animationOption.startAnim(btn2,R.anim.fadein_slide_left);

        //start fragment
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, monitorFragment).commit();
        YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));


        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, monitorFragment).commit();
                YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

            }
        });

        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, settingsFragment).commit();
                YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));

            }
        });

        btn3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, adminFragment).commit();
                YoYo.with(Techniques.FadeIn).duration(1000).playOn(findViewById(R.id.fragment_container));
            }
        });




        btn_connectServer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String ip = preferences.getString("ip",SERVER_IP);
                String port = preferences.getString("port",String.valueOf(SERVER_PORT));

                extension.printAlert("ip : " + ip + "\tport : " + port);
                clientClass.connect(ip,Integer.valueOf(port));
            }
        });

        btn_disonnectServer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try {
                    clientClass.disconnect();
                    Log.d("Complete","dissconnect");
                } catch (IOException e) {
                    e.printStackTrace();
                    Log.e("Error","can not dissconnect");
                }
            }
        });




        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                updateUI();
                handler.postDelayed(this, 10);
            }
        });

        /*
        Intent serviceIntent = new Intent(this, MyService.class);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            this.startForegroundService(serviceIntent);
        } else {
            this.startService(serviceIntent);
        }

        boolean isMyServiceRunning = MyServiceChecker.isServiceRunning(this, MyService.class);
        if (isMyServiceRunning) {
            // บริการกำลังทำงาน
            extension.printAlert("Service On");
        } else {
            // บริการไม่ได้ทำงาน
            extension.printAlert("Service Off");
        }
        */

    }


    int count = 0;
    public void updateUI() {


//        String ipValue = preferences.getString("ip", "0.0.0.0");


//        System.out.println("value : " + ipValue + "\t" + String.valueOf(count++));

//        extension.printAlert(String.valueOf(clientClass.getConnected()));

        if (!clientClass.getConnected()) {
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




}