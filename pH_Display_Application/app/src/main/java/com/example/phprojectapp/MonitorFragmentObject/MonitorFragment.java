package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

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
import android.widget.TextView;

import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.R;

import java.util.ArrayList;

public class MonitorFragment extends Fragment {
    private TextView monitor_tvPHNeeded,monitor_tvPHNeededOutput,monitor_tvPHNeededOutputWorking,monitor_stepText,monitor_tvTimeBoard,tv_tempC,tv_ec;
    private Button monitor_btnChangePH,monitor_btnStopChangePH,monitor_btnStartWork, monitor_btnSetTime;
    private LinearLayout nonWorkLayout,workingLayout;


    private EditText manualETTime;
    private Button btn_relay[] = new Button[6];

    private RadioGroup radioGroupMode;

    private LinearLayout layoutManualMode,layoutAutoMode;

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
            monitor_tvPHNeeded.setText((var.inputPH == -1) ? "_" : String.valueOf(var.inputPH));
            monitor_tvPHNeededOutput.setText((var.inputPH == -1) ? "_" : String.format("%.1f - %.1f",var.inputPH - 0.5,var.inputPH + 0.5));
            monitor_tvPHNeededOutputWorking.setText((var.inputPH == -1) ? "_" : String.format("%.1f - %.1f",var.inputPH - 0.5,var.inputPH + 0.5));
            monitor_stepText.setText(var.stepText);
            monitor_tvTimeBoard.setText(String.format("%01d:%01d:%01d",var.timeBoardObject.hour,var.timeBoardObject.minute,var.timeBoardObject.second));

            for(int i = 0;i<6;i++){
                if(!var.relay_status[i])btn_relay[i].setBackgroundResource(R.drawable.btn_style4);
                else btn_relay[i].setBackgroundResource(R.drawable.btn_style3);
            }

            if ((var.inputPH == -1 || var.inputPH < 4 || var.inputPH > 10)) {
                monitor_btnStartWork.setVisibility(View.INVISIBLE);
            } else {
                monitor_btnStartWork.setVisibility(View.VISIBLE);
            }
        }

        if(var.work_status){
            nonWorkLayout.setVisibility(View.GONE);
            workingLayout.setVisibility(View.VISIBLE);
        }else{
            nonWorkLayout.setVisibility(View.VISIBLE);
            workingLayout.setVisibility(View.GONE);
        }

        tv_tempC.setText(String.format("%.1f C",var.tempC));

    }




    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        monitorFragmentView = inflater.inflate(R.layout.fragment_monitor, container, false);


        monitor_btnChangePH = monitorFragmentView.findViewById(R.id.monitor_btnChangePH);
        monitor_btnStopChangePH = monitorFragmentView.findViewById(R.id.monitor_btnStopChangePH);
        monitor_btnSetTime = monitorFragmentView.findViewById(R.id.monitor_btnSetTime);
        monitor_btnStartWork = monitorFragmentView.findViewById(R.id.monitor_btnStartWork);

        monitor_tvPHNeeded = monitorFragmentView.findViewById(R.id.monitor_tvPHNeeded);
        monitor_tvPHNeededOutput = monitorFragmentView.findViewById(R.id.monitor_tvPHNeededOutput);
        monitor_tvTimeBoard = monitorFragmentView.findViewById(R.id.monitor_tvTimeBoard);
        tv_tempC = monitorFragmentView.findViewById(R.id.monitor_tvTemp);
        tv_ec = monitorFragmentView.findViewById(R.id.monitor_tvEC);
        monitor_tvPHNeededOutputWorking = monitorFragmentView.findViewById(R.id.monitor_tvPHNeededOutputWorking);
        monitor_stepText = monitorFragmentView.findViewById(R.id.monitor_step_text);


        nonWorkLayout = monitorFragmentView.findViewById(R.id.monitor_layout_noneWork);
        workingLayout = monitorFragmentView.findViewById(R.id.monitor_layout_Working);

        layoutManualMode = monitorFragmentView.findViewById(R.id.monitor_layout_FunctionManual);
        layoutAutoMode = monitorFragmentView.findViewById(R.id.monitor_layout_FunctionAuto);


        manualETTime = monitorFragmentView.findViewById(R.id.monitor_manualEtTime);
        btn_relay[0] = monitorFragmentView.findViewById(R.id.monitor_manualBtnR1);
        btn_relay[1] = monitorFragmentView.findViewById(R.id.monitor_manualBtnR2);
        btn_relay[2] = monitorFragmentView.findViewById(R.id.monitor_manualBtnR3);
        btn_relay[3] = monitorFragmentView.findViewById(R.id.monitor_manualBtnR4);
        btn_relay[4] = monitorFragmentView.findViewById(R.id.monitor_manualBtnR5);
        btn_relay[5] = monitorFragmentView.findViewById(R.id.monitor_manualBtnR6);

        radioGroupMode = monitorFragmentView.findViewById(R.id.monitor_radioGroupMode);
        radioGroupMode.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int checkedId) {
                RadioButton radioButton = monitorFragmentView.findViewById(checkedId);

                if(radioButton.getText().toString().equals("Manual")){
                    layoutManualMode.setVisibility(View.VISIBLE);
                    layoutAutoMode.setVisibility(View.GONE);
                }else if(radioButton.getText().toString().equals("Auto")){
                    layoutManualMode.setVisibility(View.GONE);
                    layoutAutoMode.setVisibility(View.VISIBLE);
                }
            }
        });

        monitor_btnChangePH.setOnClickListener(this::onClickSetPH);
        monitor_btnStopChangePH.setOnClickListener(this::onClickStopPHWork);
        monitor_btnStartWork.setOnClickListener(this::onClickStartPHWork);
        monitor_btnSetTime.setOnClickListener(this::onClickSetTime);
        monitor_tvTimeBoard.setOnClickListener(this::onClickTimeBoard);

        btn_relay[0].setOnClickListener(this::onToggleRelay1);
        btn_relay[1].setOnClickListener(this::onToggleRelay2);
        btn_relay[2].setOnClickListener(this::onToggleRelay3);
        btn_relay[3].setOnClickListener(this::onToggleRelay4);
        btn_relay[4].setOnClickListener(this::onToggleRelay5);
        btn_relay[5].setOnClickListener(this::onToggleRelay6);



//        animationOption.startShuffleSlideIn(new ArrayList<View>(Arrays.asList(monitor_btnChangePH,monitor_btnSetTime)));
//        animationOption.startAnim(monitor_tvPH,R.anim.zoomin);
//        animationOption.startAnimation(monitor_imgUnderBar,R.animator.animator_set1);

//        animationOption.startAnim(monitor_layoutPHmeter,R.anim.fadein);

        getChildFragmentManager().beginTransaction().replace(R.id.monitor_workingSpace, var.step3).commit();


        return monitorFragmentView;
    }

    private void onClickStartPHWork(View v){
        var.comunity.setInputPH(var.inputPH);
    }

    private void onClickStopPHWork(View v){
        var.comunity.setInputPH_STOP();

    }

    private void onClickSetPH(View v){
        var.soundEffect.play_glitch();
        var.animationOption.startAnim(monitor_btnChangePH,R.anim.btnclick_animation);
        SetPHDialog setPHDialog = new SetPHDialog(getContext(),var.animationOption);
        setPHDialog.setSetPHDialogEvent(new SetPHDialogEvent() {
            @Override
            public void onClickOk(float value) {
                var.inputPH = value;
//
            }
        });
    }

    private void onClickSetTime(View v){
        var.soundEffect.play_glitch();
        var.animationOption.startAnim(monitor_btnSetTime,R.anim.btnclick_animation);

        var.comunity.getTimeAutoWork();

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
                var.extension.printDebug("Monitor","onClickTimeBoard");
            }
        });
    }




    private void toggleRelay(int index){
        String time_str = manualETTime.getText().toString();
        manualETTime.setText("");
        if(time_str.equals("")){

        }else{
            double time = Double.parseDouble(time_str);
            var.comunity.setToggleRelay(index,time);
        }
    }

    public void onToggleRelay1(View v){
        toggleRelay(0);
    }
    public void onToggleRelay2(View v){
        toggleRelay(1);
    }
    public void onToggleRelay3(View v){
        toggleRelay(2);
    }
    public void onToggleRelay4(View v){
        toggleRelay(3);
    }
    public void onToggleRelay5(View v){
        toggleRelay(4);
    }
    public void onToggleRelay6(View v){
        toggleRelay(5);
    }



}