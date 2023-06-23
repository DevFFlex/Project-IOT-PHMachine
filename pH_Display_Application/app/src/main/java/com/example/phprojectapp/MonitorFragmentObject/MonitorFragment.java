package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.daimajia.androidanimations.library.Techniques;
import com.daimajia.androidanimations.library.YoYo;
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
    private TextView monitor_tvPH, monitor_tvPHNeeded,monitor_tvTimeBoard;
    private Button monitor_btnChangePH,monitor_btnStopChangePH, monitor_btnSetTime,monitor_btnMeterBack,monitor_btnMeterNext;
    private ImageView monitor_imgUnderBar;
    private LinearLayout monitor_layout_rtctime;


    View monitorFragmentView;

    CircleMeter circleMeterFragment;
    GraphMeter graphMeter;
    ArrayList<Fragment> meterfraglist = new ArrayList<Fragment>();
    private int meterfrag_cursor = 0;


    private Variable var;

    public MonitorFragment(Variable var) {
        this.var = var;;
        circleMeterFragment = new CircleMeter(var);
        graphMeter = new GraphMeter(var);

        meterfraglist.add(circleMeterFragment);
        meterfraglist.add(graphMeter);

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
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {

        monitorFragmentView = inflater.inflate(R.layout.fragment_monitor, container, false);

        monitor_imgUnderBar = monitorFragmentView.findViewById(R.id.monitor_imgUnderBar);

        monitor_btnChangePH = monitorFragmentView.findViewById(R.id.monitor_btnChangePH);
        monitor_btnStopChangePH = monitorFragmentView.findViewById(R.id.monitor_btnStopChangePH);
        monitor_btnSetTime = monitorFragmentView.findViewById(R.id.monitor_btnSetTime);
        monitor_btnMeterBack = monitorFragmentView.findViewById(R.id.monitor_btnMeterBack);
        monitor_btnMeterNext = monitorFragmentView.findViewById(R.id.monitor_btnMeterNext);

        monitor_layout_rtctime = monitorFragmentView.findViewById(R.id.monitor_ll_rtctime);


        monitor_tvPHNeeded = monitorFragmentView.findViewById(R.id.monitor_tvPHNeeded);
        monitor_tvTimeBoard = monitorFragmentView.findViewById(R.id.monitor_tvTimeBoard);

        monitor_btnChangePH.setOnClickListener(this::onClickSetPH);
        monitor_btnStopChangePH.setOnClickListener(this::onClickStopPHWork);
        monitor_btnSetTime.setOnClickListener(this::onClickSetTime);
        monitor_tvTimeBoard.setOnClickListener(this::onClickTimeBoard);
        monitor_btnMeterBack.setOnClickListener(this::onClickMeterBack);
        monitor_btnMeterNext.setOnClickListener(this::onClickMeterNext);

//        animationOption.startShuffleSlideIn(new ArrayList<View>(Arrays.asList(monitor_btnChangePH,monitor_btnSetTime)));
//        animationOption.startAnim(monitor_tvPH,R.anim.zoomin);
//        animationOption.startAnimation(monitor_imgUnderBar,R.animator.animator_set1);

//        animationOption.startAnim(monitor_layoutPHmeter,R.anim.fadein);

        getChildFragmentManager().beginTransaction().replace(R.id.fragMeter, circleMeterFragment).commit();

        return monitorFragmentView;
    }

    private void func_updateMeterPage(){
        if (meterfraglist.get(meterfrag_cursor).getClass() != CircleMeter.class)monitor_layout_rtctime.setVisibility(View.GONE);
        else monitor_layout_rtctime.setVisibility(View.VISIBLE);

        getChildFragmentManager().beginTransaction().replace(R.id.fragMeter, meterfraglist.get(meterfrag_cursor)).commit();
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

    private void onClickMeterBack(View v){
        meterfrag_cursor--;
        if(meterfrag_cursor < 0)meterfrag_cursor = meterfraglist.size() - 1;
        func_updateMeterPage();
    }
    private void onClickMeterNext(View v){
        meterfrag_cursor++;
        if(meterfrag_cursor > meterfraglist.size() - 1)meterfrag_cursor = 0;
        func_updateMeterPage();
    }


}