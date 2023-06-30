package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.R;

import java.util.ArrayList;

public class MonitorFragment extends Fragment {
    private TextView monitor_tvPHNeeded,monitor_tvTimeBoard,tv_tempC,tv_ec;
    private Button monitor_btnChangePH,monitor_btnStopChangePH, monitor_btnSetTime,btn_nextstep;
    private ImageView monitor_imgUnderBar;
    private FrameLayout workingSpaceLayout;

    View monitorFragmentView;
    int current_step = 0;


    private Variable var;

    public MonitorFragment(Variable var) {
        this.var = var;;
        current_step = var.step;
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
        if (var != null){
            monitor_tvPHNeeded.setText(String.valueOf(var.inputPH));
        }

        if(var.working_ph){
            monitor_btnChangePH.setVisibility(View.GONE);
            monitor_btnStopChangePH.setVisibility(View.VISIBLE);
        }else{
            monitor_btnChangePH.setVisibility(View.VISIBLE);
            monitor_btnStopChangePH.setVisibility(View.GONE);
        }

        if(current_step != var.step){
            if(var.step == 0){
                workingSpaceLayout.removeAllViews();
            }else if(var.step == 1){
                workingSpaceLayout.setVisibility(View.VISIBLE);
                getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var.step1).commit();
            }
            else if(var.step == 2){
                workingSpaceLayout.setVisibility(View.VISIBLE);
                getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var.step2).commit();
            }
            else if(var.step == 3){
                workingSpaceLayout.setVisibility(View.VISIBLE);
                getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var.step3).commit();
            }
            else if(var.step == 4){
                workingSpaceLayout.setVisibility(View.VISIBLE);
                getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var.step4).commit();
            }
            current_step = var.step;
        }


        tv_tempC.setText(String.format("%.1f C",var.tempC));

    }

    public void onNextStep(View v){
        if(var.step < 4)var.step++;
        else var.step = 0;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        monitorFragmentView = inflater.inflate(R.layout.fragment_monitor, container, false);

        monitor_imgUnderBar = monitorFragmentView.findViewById(R.id.monitor_imgUnderBar);

        monitor_btnChangePH = monitorFragmentView.findViewById(R.id.monitor_btnChangePH);
        monitor_btnStopChangePH = monitorFragmentView.findViewById(R.id.monitor_btnStopChangePH);
        monitor_btnSetTime = monitorFragmentView.findViewById(R.id.monitor_btnSetTime);


        btn_nextstep = monitorFragmentView.findViewById(R.id.monitor_btnNextStep);

        workingSpaceLayout = monitorFragmentView.findViewById(R.id.monitor_workingSpace);

        monitor_tvPHNeeded = monitorFragmentView.findViewById(R.id.monitor_tvPHNeeded);
        monitor_tvTimeBoard = monitorFragmentView.findViewById(R.id.monitor_tvTimeBoard);
        tv_tempC = monitorFragmentView.findViewById(R.id.monitor_tvTemp);
        tv_ec = monitorFragmentView.findViewById(R.id.monitor_tvEC);

        monitor_btnChangePH.setOnClickListener(this::onClickSetPH);
        monitor_btnStopChangePH.setOnClickListener(this::onClickStopPHWork);
        monitor_btnSetTime.setOnClickListener(this::onClickSetTime);
        monitor_tvTimeBoard.setOnClickListener(this::onClickTimeBoard);

        btn_nextstep.setOnClickListener(this::onNextStep);

//        animationOption.startShuffleSlideIn(new ArrayList<View>(Arrays.asList(monitor_btnChangePH,monitor_btnSetTime)));
//        animationOption.startAnim(monitor_tvPH,R.anim.zoomin);
//        animationOption.startAnimation(monitor_imgUnderBar,R.animator.animator_set1);

//        animationOption.startAnim(monitor_layoutPHmeter,R.anim.fadein);

//        getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var).commit();


        return monitorFragmentView;
    }

    private void onClickStopPHWork(View v){
        var.comunity.setInputPH_STOP();
        var.working_ph = false;
    }

    private void onClickSetPH(View v){
        var.soundEffect.play_glitch();
        var.animationOption.startAnim(monitor_btnChangePH,R.anim.btnclick_animation);
        SetPHDialog setPHDialog = new SetPHDialog(getContext(),var.animationOption);
        setPHDialog.setSetPHDialogEvent(new SetPHDialogEvent() {
            @Override
            public void onClickOk(float value) {
//                var.extension.printAlert(String.valueOf(monitor_CircleView.getCircleAnimation().degreeList.get(0)));
                var.inputPH = value;

                var.comunity.setInputPH(value);

                var.working_ph = true;

            }
        });
    }

    private void onClickSetTime(View v){
        var.soundEffect.play_glitch();
        var.animationOption.startAnim(monitor_btnSetTime,R.anim.btnclick_animation);

        var.comunity.getTimeAutoWork();
        var.extension.printAlert("on get time auto work");

        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
               SetTimeDialog std = new SetTimeDialog(getContext(),var);
               std.setSetTimeDialogEvent(new SetTimeDialogEvent() {
                   @Override
                   public void onClickSave(String output) {
                       var.comunity.setTimeAutoWork(output);
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
                String queryString = var.comunity.timeBoardObjectToQueryString(timeBoardObject);
                var.comunity.setTimeBoard(queryString);
            }
        });
    }



}