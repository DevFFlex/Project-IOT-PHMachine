package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.phprojectapp.Variable.Variable;


public class InternetManagerFragment extends Fragment {

    View view;
    Variable var;


    public InternetManagerFragment(Variable var) {
        // Required empty public constructor
        this.var = var;
    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_internetmanager, container, false);


        return view;
    }
}