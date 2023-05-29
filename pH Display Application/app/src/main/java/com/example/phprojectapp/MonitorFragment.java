package com.example.phprojectapp;

import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.TimePicker;

import com.example.phprojectapp.ClassEx.ClientClass;
import com.example.phprojectapp.ClassEx.Comunity;
import com.example.phprojectapp.ClassEx.Extension;
import com.example.phprojectapp.ClassEx.SoundEffect;
import com.example.phprojectapp.ClassEx.TimeBoardObject;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragmentOnEvent;
import com.example.phprojectapp.ClassEx.Variable;
import com.example.phprojectapp.MonitorFragmentObject.SetPHDialog;
import com.example.phprojectapp.MonitorFragmentObject.SetTimeBoard;
import com.example.phprojectapp.MonitorFragmentObject.SetTimeDialog.SetTimeDialog;
import com.example.phprojectapp.MonitorFragmentObject.TestDialog;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;

public class MonitorFragment extends Fragment implements MonitorFragmentOnEvent {

//    private PH_Meter_Animation ph_meter_animation;
    private TextView monitor_tvPH, monitor_tvOutputDebug, monitor_tvPHNeeded,monitor_tvTimeBoard;
    private Button monitor_btnChangePH, monitor_btnSetTime,monitor_btnTest;
    private ImageView ph_arrow,monitor_imgUnderBar;
    private RelativeLayout monitor_layoutPHmeter;
    CircleView monitor_CircleView;

    View monitorFragmentView;

    private ClientClass clientClass;
    public void setClientClass(ClientClass clientClass) {
        this.clientClass = clientClass;
    }

    private Comunity comunity;
    public void setComunity(Comunity comunity) {
        this.comunity = comunity;
    }

    private Variable variable;
    public void setVariable(Variable variable) {
        this.variable = variable;
    }


    private AnimationOption animationOption;
    public void setAnimationOption(AnimationOption animationOption) {
        this.animationOption = animationOption;
    }

    private SoundEffect soundEffect;
    public void setSoundEffect(SoundEffect soundEffect) {
        this.soundEffect = soundEffect;
    }

    private Extension extension;
    public void setExtension(Extension extension) {
        this.extension = extension;
    }



    public MonitorFragment() {

    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                updateUI();
                handler.postDelayed(this, 10);
            }
        });


    }


    private void updateUI(){
        if (variable != null){
            monitor_tvPHNeeded.setText(String.valueOf(variable.getInputPH()));
            monitor_tvPH.setText(String.valueOf(variable.getMixtankPH()));
            monitor_CircleView.getCircleAnimation().setCurrentPH(variable.getMixtankPH());
            monitor_tvOutputDebug.setText(variable.getOutout_text());
        }
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        monitorFragmentView = inflater.inflate(R.layout.fragment_monitor, container, false);

        monitor_CircleView = monitorFragmentView.findViewById(R.id.monitor_CircleView);

        monitor_imgUnderBar = monitorFragmentView.findViewById(R.id.monitor_imgUnderBar);

        monitor_btnChangePH = monitorFragmentView.findViewById(R.id.monitor_btnChangePH);
        monitor_btnSetTime = monitorFragmentView.findViewById(R.id.monitor_btnSetTime);
        monitor_btnTest = monitorFragmentView.findViewById(R.id.monitor_btnTest);


        monitor_tvPH = monitorFragmentView.findViewById(R.id.monitor_tvPH);
        monitor_tvOutputDebug = monitorFragmentView.findViewById(R.id.monitor_tvOutputDebug);
        monitor_tvPHNeeded = monitorFragmentView.findViewById(R.id.monitor_tvPHNeeded);
        monitor_tvTimeBoard = monitorFragmentView.findViewById(R.id.monitor_tvTimeBoard);


        monitor_btnChangePH.setOnClickListener(this::onClickSetPH);
        monitor_btnSetTime.setOnClickListener(this::onClickSetTime);
        monitor_btnTest.setOnClickListener(this::onClickTest);

        monitor_tvTimeBoard.setOnClickListener(this::onClickTimeBoard);

        animationOption.startShuffleSlideIn(new ArrayList<View>(Arrays.asList(monitor_btnChangePH,monitor_btnSetTime,monitor_btnTest)));
        animationOption.startAnim(monitor_tvPH,R.anim.zoomin);
        animationOption.startAnimation(monitor_imgUnderBar,R.animator.animator_set1);

//        animationOption.startAnim(monitor_layoutPHmeter,R.anim.fadein);



        return monitorFragmentView;
    }


    public void onClickSetPH(View v){
        soundEffect.play_glitch();
        animationOption.startAnim(monitor_btnChangePH,R.anim.btnclick_animation);
        SetPHDialog setPHDialog = new SetPHDialog(getContext(),MonitorFragment.this,animationOption);
    }


    public void onClickSetTime(View v){
        soundEffect.play_glitch();
        animationOption.startAnim(monitor_btnSetTime,R.anim.btnclick_animation);

        comunity.getTimeList();

        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                new SetTimeDialog(getContext(),MonitorFragment.this,variable);
            }
        },2000);
    }


    public void onClickTest(View v){
        TestDialog testDialog = new TestDialog(getContext());
        testDialog.setMonitorFragment_onEventet(MonitorFragment.this);
    }

    public void onClickTimeBoard(View v){

        new SetTimeBoard(v.getContext(),this);

    }


    @Override
    public void dialogSetPH_onclick_OK(float ph_value) {
        extension.printAlert(String.valueOf(monitor_CircleView.getCircleAnimation().degreeList.get(0)));
        variable.setInputPH(ph_value);

        comunity.setInputPH(ph_value);
    }

    @Override
    public void dialogTest_onclick_OK(String text) {
        System.out.println("from edittext text = " + text);
        clientClass.sendToServer("C_TEST=" + text);
    }

    @Override
    public void dialogSetTime_onclick_Save(String time_str) {
        System.out.println(time_str);
        comunity.setTimeList(time_str);
    }

    @Override
    public void dialogSetTimeBoard_onClick_OK(TimeBoardObject timeBoardObject) {
        String queryString = comunity.timeBoardObjectToQueryString(timeBoardObject);
        comunity.setTimeBoard(queryString);
    }


}