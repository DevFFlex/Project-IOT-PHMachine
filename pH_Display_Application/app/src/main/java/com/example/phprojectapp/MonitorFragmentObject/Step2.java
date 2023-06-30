package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.phprojectapp.R;
import com.example.phprojectapp.Variable.Variable;

public class Step2 extends Fragment {

    private Variable var;

    public Step2(Variable var) {
        this.var = var;
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_step2, container, false);
    }
}