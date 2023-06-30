package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.example.phprojectapp.R;
import com.example.phprojectapp.Variable.Variable;

import java.util.ArrayList;


public class Step3 extends Fragment {

    View step3_view;
    Variable var;

    CircleMeter circleMeterFragment;
    GraphMeter graphMeter;

    Button btn_meterForward,btn_meterBack;

    ArrayList<Fragment> meterfraglist = new ArrayList<Fragment>();
    private int meterfrag_cursor = 0;

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

        btn_meterBack.setOnClickListener(this::onClickMeterBack);
        btn_meterForward.setOnClickListener(this::onClickMeterNext);

        func_updateMeterPage();

        return step3_view;
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