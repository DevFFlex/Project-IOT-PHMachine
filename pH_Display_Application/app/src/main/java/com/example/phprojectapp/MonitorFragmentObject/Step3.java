package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;

import com.example.phprojectapp.R;
import com.example.phprojectapp.Variable.Variable;

import java.util.ArrayList;


public class Step3 extends Fragment {

    View step3_view;
    Variable var;
    Handler handler = new Handler();

    CircleMeter circleMeterFragment;
    GraphMeter graphMeter;

    Button btn_meterForward,btn_meterBack;

    ImageView iv_step1,iv_step2,iv_step3,iv_step4;


    ArrayList<Fragment> meterfraglist = new ArrayList<Fragment>();
    private int meterfrag_cursor = 0;
    private int now_step = 0;

    public Step3(Variable var) {
        this.var = var;

        circleMeterFragment = new CircleMeter(var);
        graphMeter = new GraphMeter(var);

        meterfraglist.add(circleMeterFragment);
        meterfraglist.add(graphMeter);



    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        step3_view = inflater.inflate(R.layout.fragment_step3, container, false);

        btn_meterBack = step3_view.findViewById(R.id.step3_btnMeterBack);
        btn_meterForward = step3_view.findViewById(R.id.step3_btnMeterNext);

        iv_step1 = step3_view.findViewById(R.id.iv_status_step1);
        iv_step2 = step3_view.findViewById(R.id.iv_status_step2);
        iv_step3 = step3_view.findViewById(R.id.iv_status_step3);
        iv_step4 = step3_view.findViewById(R.id.iv_status_step4);



        btn_meterBack.setOnClickListener(this::onClickMeterBack);
        btn_meterForward.setOnClickListener(this::onClickMeterNext);

        func_updateMeterPage();


        handler.post(new Runnable() {
            @Override
            public void run() {
                step3_update_function();
                handler.postDelayed(this,1000);
            }
        });

        step3_update_function();

        return step3_view;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        var.extension.printDebug("Step3","on destroy");
        handler.removeCallbacksAndMessages(null);
    }

    private int count = 0;
    private void step3_update_function(){
        System.out.println("on step3 update function : " + String.valueOf(count++));

//        if(now_step != var.step){
//            now_step = var.step;
//            statusActive(now_step);
//        }
        statusActive(var.step);

    }

    private void statusActive(int step){
        switch (step){
            case 0:
                iv_step1.setBackgroundResource(R.drawable.status_circle_red);
                iv_step2.setBackgroundResource(R.drawable.status_circle_red);
                iv_step3.setBackgroundResource(R.drawable.status_circle_red);
                iv_step4.setBackgroundResource(R.drawable.status_circle_red);
                break;
            case 1:
                iv_step1.setBackgroundResource(R.drawable.status_circle_green);
                iv_step2.setBackgroundResource(R.drawable.status_circle_red);
                iv_step3.setBackgroundResource(R.drawable.status_circle_red);
                iv_step4.setBackgroundResource(R.drawable.status_circle_red);
                break;
            case 2:
                iv_step1.setBackgroundResource(R.drawable.status_circle_red);
                iv_step2.setBackgroundResource(R.drawable.status_circle_green);
                iv_step3.setBackgroundResource(R.drawable.status_circle_red);
                iv_step4.setBackgroundResource(R.drawable.status_circle_red);
                break;
            case 3:
                iv_step1.setBackgroundResource(R.drawable.status_circle_red);
                iv_step2.setBackgroundResource(R.drawable.status_circle_red);
                iv_step3.setBackgroundResource(R.drawable.status_circle_green);
                iv_step4.setBackgroundResource(R.drawable.status_circle_red);
                break;
            case 4:
                iv_step1.setBackgroundResource(R.drawable.status_circle_red);
                iv_step2.setBackgroundResource(R.drawable.status_circle_red);
                iv_step3.setBackgroundResource(R.drawable.status_circle_red);
                iv_step4.setBackgroundResource(R.drawable.status_circle_green);
                break;
        }
    }

    private void func_updateMeterPage(){
        getChildFragmentManager().beginTransaction().replace(R.id.monitor_layout_step2, meterfraglist.get(meterfrag_cursor)).commit();
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