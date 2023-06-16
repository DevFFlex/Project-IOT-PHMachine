package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import com.example.phprojectapp.ClassEx.Variable;


public class AdminFragment extends Fragment {
    private Variable var;
    private Button btn_relay1,btn_relay2,btn_relay3,btn_relay4,btn_relay5,btn_relay6;


    public AdminFragment(Variable variable) {
        this.var = variable;
    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.fragment_admin, container, false);

        btn_relay1 = v.findViewById(R.id.admin_btn_relay1);
        btn_relay2 = v.findViewById(R.id.admin_btn_relay2);
        btn_relay3 = v.findViewById(R.id.admin_btn_relay3);
        btn_relay4 = v.findViewById(R.id.admin_btn_relay4);
        btn_relay5 = v.findViewById(R.id.admin_btn_relay5);
        btn_relay6 = v.findViewById(R.id.admin_btn_relay6);

        btn_relay1.setOnClickListener(this::onToggleRelay1);
        btn_relay2.setOnClickListener(this::onToggleRelay2);
        btn_relay3.setOnClickListener(this::onToggleRelay3);
        btn_relay4.setOnClickListener(this::onToggleRelay4);
        btn_relay5.setOnClickListener(this::onToggleRelay5);
        btn_relay6.setOnClickListener(this::onToggleRelay6);

        return v;
    }

    private void toggleRelay(int index){
        var.comunity.setToggleRelay(index);
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