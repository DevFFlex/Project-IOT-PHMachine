package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.phprojectapp.R;
import com.example.phprojectapp.Variable.Variable;

import android.widget.FrameLayout;

public class Step4 extends Fragment {
    private View view;

    private Variable var;
    private FrameLayout step4_workspace;

    public Step4(Variable var) {
        this.var = var;

    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_step4, container, false);

        step4_workspace = view.findViewById(R.id.step4_workspace);


        return view;
    }
}