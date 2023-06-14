package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.phprojectapp.AnimationOption;
import com.example.phprojectapp.ClassEx.Comunity;
import com.example.phprojectapp.ClassEx.Extension;
import com.example.phprojectapp.ClassEx.SoundEffect;
import com.example.phprojectapp.ClassEx.TimeBoardObject;
import com.example.phprojectapp.ClassEx.Variable;
import com.example.phprojectapp.R;

import java.util.ArrayList;
import java.util.Arrays;

public class MonitorFragment extends Fragment {

//    private PH_Meter_Animation ph_meter_animation;
    private TextView monitor_tvPH, monitor_tvOutputDebug, monitor_tvPHNeeded,monitor_tvTimeBoard;
    private Button monitor_btnChangePH, monitor_btnSetTime,monitor_btnTest;
    private ImageView monitor_imgUnderBar;

    CircleView monitor_CircleView;

    View monitorFragmentView;

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
            monitor_tvPHNeeded.setText(String.valueOf(variable.inputPH));
            monitor_tvPH.setText(String.valueOf(variable.mixtankPH));
            monitor_CircleView.getCircleAnimation().setCurrentPH(variable.mixtankPH);
            monitor_tvOutputDebug.setText(variable.outout_text);
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


        monitor_tvPH = monitorFragmentView.findViewById(R.id.monitor_tvPH);
        monitor_tvOutputDebug = monitorFragmentView.findViewById(R.id.monitor_tvOutputDebug);
        monitor_tvPHNeeded = monitorFragmentView.findViewById(R.id.monitor_tvPHNeeded);
        monitor_tvTimeBoard = monitorFragmentView.findViewById(R.id.monitor_tvTimeBoard);


        monitor_btnChangePH.setOnClickListener(this::onClickSetPH);
        monitor_btnSetTime.setOnClickListener(this::onClickSetTime);

        monitor_tvTimeBoard.setOnClickListener(this::onClickTimeBoard);

        animationOption.startShuffleSlideIn(new ArrayList<View>(Arrays.asList(monitor_btnChangePH,monitor_btnSetTime)));
        animationOption.startAnim(monitor_tvPH,R.anim.zoomin);
        animationOption.startAnimation(monitor_imgUnderBar,R.animator.animator_set1);

//        animationOption.startAnim(monitor_layoutPHmeter,R.anim.fadein);

        return monitorFragmentView;
    }

    private void onClickSetPH(View v){
        soundEffect.play_glitch();
        animationOption.startAnim(monitor_btnChangePH,R.anim.btnclick_animation);
        SetPHDialog setPHDialog = new SetPHDialog(getContext(),animationOption);
        setPHDialog.setSetPHDialogEvent(new SetPHDialogEvent() {
            @Override
            public void onClickOk(float value) {
                extension.printAlert(String.valueOf(monitor_CircleView.getCircleAnimation().degreeList.get(0)));
                variable.inputPH = value;

                comunity.setInputPH(value);
            }
        });
    }

    private void onClickSetTime(View v){
        soundEffect.play_glitch();
        animationOption.startAnim(monitor_btnSetTime,R.anim.btnclick_animation);

        comunity.getTimeList();

        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
               SetTimeDialog std = new SetTimeDialog(getContext(),variable);
               std.setSetTimeDialogEvent(new SetTimeDialogEvent() {
                   @Override
                   public void onClickSave(String output) {
                       System.out.println(output);
                       comunity.setTimeList(output);
                   }
               });
            }
        },2000);
    }

    private void onClickTimeBoard(View v){
        SetTimeBoard setTimeBoard = new SetTimeBoard(v.getContext());
        setTimeBoard.setSetTimeBoardEvent(new SetTimeBoardEvent() {
            @Override
            public void onClickOk(TimeBoardObject timeBoardObject) {
                String queryString = comunity.timeBoardObjectToQueryString(timeBoardObject);
                comunity.setTimeBoard(queryString);
            }
        });
    }


}