package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.Variable;
import com.example.phprojectapp.R;

import org.w3c.dom.Text;

public class CircleMeter extends Fragment {
    private View view;
    private Variable variable;

    private CircleView circleView;
    private TextView mixtankPHView;

    public CircleMeter(Variable variable) {
        this.variable = variable;

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
        if(variable != null){
            mixtankPHView.setText(String.format("%.1f",variable.mixtankPH));
            circleView.getCircleAnimation().setCurrentPH(variable.mixtankPH);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_circle_meter, container, false);

        circleView = view.findViewById(R.id.monitor_circle_circleView);
        mixtankPHView = view.findViewById(R.id.monitor_circle_tvPH);

        return view;
    }
}