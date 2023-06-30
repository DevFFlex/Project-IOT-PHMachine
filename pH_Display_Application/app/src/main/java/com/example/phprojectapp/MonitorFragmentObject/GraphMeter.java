package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.R;

import android.graphics.Color;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Description;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;


public class GraphMeter extends Fragment {
    private View view;
    private LineChart lineChart;
    private Variable variable;

    private LineDataSet dataSet;
    private LineData lineData;

    private Random random = new Random();
    private Handler handler = new Handler();
    List<Entry> entries = new ArrayList<>();

    private float time = 0;
    private int MAX_RANGE = 10;


    public GraphMeter(Variable variable) {
        this.variable = variable;
    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        handler.removeCallbacksAndMessages(null);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view  = inflater.inflate(R.layout.fragment_graph_meter, container, false);
        lineChart = view.findViewById(R.id.lineChart);


        entries.clear();
        for(int i = 0;i<10;i++)entries.add(new Entry(i, 7));


        dataSet = new LineDataSet(entries, "PH");  // อ้างอิงตัวแปร dataSet เดิม
        dataSet.setFillColor(Color.WHITE);
        dataSet.setValueTextColor(Color.WHITE);

        dataSet.clear();
        lineChart.clear();
        time = 0;
        dataSet.setColors(Color.RED);
        dataSet.setLineWidth(5f);
        dataSet.setValueTextSize(10f);
        dataSet.setCircleColors(Collections.singletonList(Color.BLUE));
        dataSet.setMode(LineDataSet.Mode.CUBIC_BEZIER);


        lineData = new LineData(dataSet);  // อ้างอิงตัวแปร lineData เดิม

        lineChart.setData(lineData);
//        lineChart.setDrawGridBackground(false);
        lineChart.setScaleEnabled(false);
        lineChart.setDragEnabled(false);
        lineChart.setGridBackgroundColor(Color.BLUE);
        lineChart.setBackgroundColor(Color.BLACK);

        XAxis xAxis = lineChart.getXAxis();
        xAxis.setTextSize(12f);
        xAxis.setTextColor(Color.YELLOW);
        xAxis.setEnabled(false);

        YAxis yAxis = lineChart.getAxisLeft();
        yAxis.setAxisMinimum(0);
        yAxis.setAxisMaximum(14);
        yAxis.setEnabled(false);

        YAxis yAxisR = lineChart.getAxisRight();
        yAxisR.setAxisMinimum(0);
        yAxisR.setAxisMaximum(14);
        yAxisR.setTextColor(Color.YELLOW);

        Description description = new Description();
        description.setText("Graph");
        description.setTextColor(Color.WHITE);
        description.setTextSize(14f);

        lineChart.setDescription(description);


        handler.post(new Runnable() {
            @Override
            public void run() {
                update();
                handler.postDelayed(this,1000);
//                updateGraph();
            }
        });

        return view;
    }

    private void update() {

//        float yValue = random.nextInt(140) / 10;
        float yValue = variable.mixtankPH;
//        variable.extension.printDebug("GraphMeter","float = " + String.valueOf(entries.size()));

        variable.extension.printDebug("GraphMeter", "time = " + String.valueOf(time));

        if (dataSet.getEntryCount() > MAX_RANGE) {
            dataSet.removeEntry(0);
        }
        addEntry(yValue);

        time += 1;

    }

    private void addEntry(float yValue) {
        LineData data = lineChart.getData();

        data.addEntry(new Entry(time, yValue), 0);
        data.notifyDataChanged();

        lineChart.notifyDataSetChanged();
        lineChart.invalidate();


    }
}