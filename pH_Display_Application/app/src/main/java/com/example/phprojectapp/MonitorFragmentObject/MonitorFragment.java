package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Switch;
import android.widget.TextView;

import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.R;
import com.example.phprojectapp.Variable.WorkTimer;

import java.util.ArrayList;

public class MonitorFragment extends Fragment {
    private TextView monitor_tvPHNeeded,monitor_tvPHNeededOutputWorking,monitor_tvStepText,monitor_tvTimeBoard,tv_tempC,monitor_tvAddBaseCount,monitor_tvAddAcidCount;
    private Button monitor_btnChangePH,monitor_btnStopChangePH,monitor_btnStartWork,monitor_btnaddworktimer;
    private LinearLayout nonWorkLayout,workingLayout;

    private RadioGroup radioGroupMode;

    private LinearLayout layoutAutoMode,layoutTimerMode;

    private View monitorFragmentView;

    private Variable var;

    public MonitorFragment(Variable var) {
        this.var = var;;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                FragmentManager fragmentManager = getFragmentManager();
                if (fragmentManager != null) {
                    Fragment currentFragment = fragmentManager.findFragmentById(R.id.fragment_container);

                    if (currentFragment != null) {
                      updateUI();
                      handler.postDelayed(this, 1000);
                    }
                }
            }
        });


    }

    private void updateUI(){
        if (var != null){
            monitor_tvPHNeeded.setText((var.inputPH == -1) ? "_" : String.valueOf(var.inputPH));
            monitor_tvPHNeededOutputWorking.setText((var.adjustCurrentPH == -1) ? "_" : String.format("%.1f - %.1f",var.adjustCurrentPH - var.pH_space_rate,var.adjustCurrentPH + var.pH_space_rate));
            monitor_tvAddBaseCount.setText(String.valueOf(var.addBaseCount));
            monitor_tvAddAcidCount.setText(String.valueOf(var.addAcidCount));


            String stepIndexText[] = {
                    "...",
                    "กำลังปั้มนํ้าเข้าถังผสม...",
                    String.format("กำลังปั้มนํ้าไปยัง pH Sensor (เป็นเวลา %d วินาที)",(var.wait_stirringPump / 1000)),
                    String.format("รอ pH Sensor นิ่ง (เป็นเวลา %d วินาที)",(var.wait_pHStabilize / 1000)),
                    "กำลังเติมสาร...",
                    "กำลังปั้มนํ้าออกจากถังผสม...",
            };
            monitor_tvStepText.setText(stepIndexText[var.step]);

            monitor_tvTimeBoard.setText(String.format("%01d:%01d:%01d",var.timeBoardObject.hour,var.timeBoardObject.minute,var.timeBoardObject.second));

            if ((var.inputPH == -1 || var.inputPH < 4 || var.inputPH > 10))monitor_btnStartWork.setVisibility(View.INVISIBLE);
            else monitor_btnStartWork.setVisibility(View.VISIBLE);
        }

        if(var.work_status){
            nonWorkLayout.setVisibility(View.GONE);
            workingLayout.setVisibility(View.VISIBLE);
        }else{
            nonWorkLayout.setVisibility(View.VISIBLE);
            workingLayout.setVisibility(View.GONE);
        }

        tv_tempC.setText(String.format("%.1f C",var.tempC));



        LinearLayout layout = monitorFragmentView.findViewById(R.id.monitor_layout_mlti);
        layout.removeAllViewsInLayout();
        int delete_count = 0;
        for(int i = 0;i<4;i++){
            WorkTimerItemLayout workTimerItemLayout = new WorkTimerItemLayout(getContext(),i,var,var.workTimerList.getWorkTimer(i));
            if(!var.workTimerList.getWorkTimer(i).DELETE_STATUS) {
                layout.addView(workTimerItemLayout);
                delete_count++;
            }

        }

        if(delete_count >= 4)monitor_btnaddworktimer.setVisibility(View.GONE);
        else monitor_btnaddworktimer.setVisibility(View.VISIBLE);


    }




    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        monitorFragmentView = inflater.inflate(R.layout.fragment_monitor, container, false);


        monitor_btnChangePH = monitorFragmentView.findViewById(R.id.monitor_btnChangePH);
        monitor_btnStopChangePH = monitorFragmentView.findViewById(R.id.monitor_btnStopChangePH);
        monitor_btnStartWork = monitorFragmentView.findViewById(R.id.monitor_btnStartWork);

        monitor_tvPHNeeded = monitorFragmentView.findViewById(R.id.monitor_tvPHNeeded);
        monitor_tvTimeBoard = monitorFragmentView.findViewById(R.id.monitor_tvTimeBoard);
        tv_tempC = monitorFragmentView.findViewById(R.id.monitor_tvTemp);
        monitor_tvPHNeededOutputWorking = monitorFragmentView.findViewById(R.id.monitor_tvPHNeededOutputWorking);
        monitor_tvStepText = monitorFragmentView.findViewById(R.id.monitor_step_text);
        monitor_tvAddBaseCount = monitorFragmentView.findViewById(R.id.monitor_addbasecount_text);
        monitor_tvAddAcidCount = monitorFragmentView.findViewById(R.id.monitor_addacidcount_text);


        nonWorkLayout = monitorFragmentView.findViewById(R.id.monitor_layout_noneWork);
        workingLayout = monitorFragmentView.findViewById(R.id.monitor_layout_Working);

        layoutAutoMode = monitorFragmentView.findViewById(R.id.monitor_layout_FunctionAuto);
        layoutTimerMode = monitorFragmentView.findViewById(R.id.monitor_layout_FunctionTimer);

        monitor_btnaddworktimer = monitorFragmentView.findViewById(R.id.monitor_btnAddWorkTimer);


        radioGroupMode = monitorFragmentView.findViewById(R.id.monitor_radioGroupMode);
        radioGroupMode.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int checkedId) {
                RadioButton radioButton = monitorFragmentView.findViewById(checkedId);

                if(radioButton.getText().toString().equals("Trigger")){
                    layoutAutoMode.setVisibility(View.VISIBLE);
                    layoutTimerMode.setVisibility(View.GONE);
                }else if(radioButton.getText().toString().equals("Timer")){
                    layoutAutoMode.setVisibility(View.GONE);
                    layoutTimerMode.setVisibility(View.VISIBLE);
                }

            }
        });

        monitor_btnChangePH.setOnClickListener(this::onClickSetPH);
        monitor_btnStopChangePH.setOnClickListener(this::onClickStopPHWork);
        monitor_btnStartWork.setOnClickListener(this::onClickStartPHWork);
        monitor_tvTimeBoard.setOnClickListener(this::onClickTimeBoard);


        monitor_btnaddworktimer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                for(int i = 0;i<4;i++){
                    WorkTimer workTimer = var.workTimerList.getWorkTimer(i);
                    if(workTimer.DELETE_STATUS){
                        var.comunity.serverSetWorkTimer(i,workTimer.HOUR,workTimer.MINUTE,workTimer.PH,workTimer.T,workTimer.ACTIVE_STATUS,false);
                        break;
                    }
                }
            }
        });

//        animationOption.startShuffleSlideIn(new ArrayList<View>(Arrays.asList(monitor_btnChangePH,monitor_btnSetTime)));
//        animationOption.startAnim(monitor_tvPH,R.anim.zoomin);
//        animationOption.startAnimation(monitor_imgUnderBar,R.animator.animator_set1);

//        animationOption.startAnim(monitor_layoutPHmeter,R.anim.fadein);

        getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var.step3).commit();




        return monitorFragmentView;
    }

    private void onClickStartPHWork(View v){
        var.comunity.serverStartAdjustPH(var.inputPH,var.inputT);
    }

    private void onClickStopPHWork(View v){
        var.comunity.serverStopAdjustPH();
    }

    private void onClickSetPH(View v){
        var.soundEffect.play_glitch();
        var.animationOption.startAnim(monitor_btnChangePH,R.anim.btnclick_animation);
        SetPHDialog setPHDialog = new SetPHDialog(getContext(),var.animationOption);
        setPHDialog.setSetPHDialogEvent(new SetPHDialogEvent() {
            @Override
            public void onClickOk(float pH, int T) {
                var.inputPH = pH;
                var.inputT = T;
            }
        });
    }

    private void onClickTimeBoard(View v){
        SetTimeBoard setTimeBoard = new SetTimeBoard(v.getContext());
        setTimeBoard.setSetTimeBoardEvent(new SetTimeBoardEvent() {
            @Override
            public void onClickOk(TimeBoardObject timeBoardObject) {
                String queryString = var.comunity.timeBoardObjectToQueryString(timeBoardObject);
                var.comunity.serverSetTimeOfBoard(queryString);
                var.extension.printDebug("Monitor","onClickTimeBoard");
            }
        });
    }





}