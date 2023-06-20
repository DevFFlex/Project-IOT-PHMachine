package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.phprojectapp.ClassEx.Variable;
import com.example.phprojectapp.R;

import android.graphics.Color;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.AxisBase;
import com.github.mikephil.charting.components.Description;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IAxisValueFormatter;
import com.github.mikephil.charting.formatter.ValueFormatter;

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


    public GraphMeter(Variable variable) {
        this.variable = variable;
    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);



        handler.post(new Runnable() {
            @Override
            public void run() {
                update();
                handler.postDelayed(this,500);
//                updateGraph();
            }
        });
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

        entries.add(new Entry(0, 0));

        dataSet = new LineDataSet(entries, "PH");  // อ้างอิงตัวแปร dataSet เดิม

        dataSet.setColors(Color.RED);
        dataSet.setLineWidth(5f);
        dataSet.setCircleColors(Collections.singletonList(Color.BLUE));
        dataSet.setMode(LineDataSet.Mode.CUBIC_BEZIER);

        lineData = new LineData(dataSet);  // อ้างอิงตัวแปร lineData เดิม

        lineChart.setData(lineData);
        lineChart.setDrawGridBackground(false);

        return view;
    }

    private void update() {

        float yValue = random.nextFloat() * 10;
        variable.extension.printDebug("GraphMeter","float = " + String.valueOf(entries.size()));
        addEntry(yValue);

        if (entries.size() > 20) {
            entries.remove(0);
        }

        LineData data = lineChart.getData();
        if (data != null) {
            data.notifyDataChanged();
            lineChart.notifyDataSetChanged();
            lineChart.setVisibleXRangeMaximum(20);
            lineChart.moveViewToX(data.getEntryCount());
            lineChart.invalidate();
        }


    }

    private void addEntry(float yValue) {
        LineData data = lineChart.getData();
        LineDataSet dataSet = (LineDataSet) data.getDataSetByIndex(0);

        if (dataSet == null) {
            dataSet = createDataSet();
            data.addDataSet(dataSet);
        }

        data.addEntry(new Entry(dataSet.getEntryCount(), yValue), 0);
        data.notifyDataChanged();

        lineChart.notifyDataSetChanged();
        lineChart.setVisibleXRangeMaximum(10);
        lineChart.moveViewToX(data.getEntryCount());


    }

    private LineDataSet createDataSet() {
        LineDataSet dataSet = new LineDataSet(null, "Realtime Data");
        return dataSet;
    }


}