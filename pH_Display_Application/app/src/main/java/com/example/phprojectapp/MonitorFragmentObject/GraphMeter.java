package com.example.phprojectapp.MonitorFragmentObject;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

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
import java.util.List;
import java.util.Random;


public class GraphMeter extends Fragment {
    private View view;
    private LineChart lineChart;

    private LineDataSet dataSet;
    private LineData lineData;
    private Random random = new Random();
    private Handler handler = new Handler();


    public GraphMeter() {

    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);



    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view  = inflater.inflate(R.layout.fragment_graph_meter, container, false);
        lineChart = view.findViewById(R.id.lineChart);

        lineChart.setDrawGridBackground(false);

        List<Entry> entries = new ArrayList<>();
        dataSet = new LineDataSet(entries, "Realtime Data");
        lineData = new LineData(dataSet);

        lineChart.setData(lineData);

        startRealtimeUpdate();

//        Description description = new Description();
//        description.setText("Realtime Chart");
//        lineChart.setDescription(description);


//        // สร้างข้อมูลสำหรับกราฟเส้น
//        List<Entry> entries = new ArrayList<>();
//        entries.add(new Entry(0, 4));
//        entries.add(new Entry(1, 8));
//        entries.add(new Entry(2, 6));
//        entries.add(new Entry(3, 2));
//        entries.add(new Entry(4, 7));
//
//        // สร้างชุดข้อมูลเส้น
//        LineDataSet dataSet = new LineDataSet(entries, "Label");
//        dataSet.setColor(Color.BLUE);
//        dataSet.setCircleColor(Color.RED);
//
//        // สร้างข้อมูลเส้นทั้งหมด
//        LineData lineData = new LineData(dataSet);
//
//        // กำหนดข้อมูลเส้นให้กับกราฟ
//        lineChart.setData(lineData);
//
//        // กำหนดคำอธิบายกราฟ
//        Description description = new Description();
//        description.setText("Realtime Chart");
//        lineChart.setDescription(description);
//
//        // กำหนดการตั้งค่าอื่นๆ ตามความต้องการ
//
//        // อัพเดทแผนภาพ
//        lineChart.invalidate();

        return view;
    }

    private void startRealtimeUpdate() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true){
//                    float yValue = random.nextFloat() * 10; // สุ่มค่า Y ใหม่
                    addEntry(20);

                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        throw new RuntimeException(e);
                    }
                }
            }
        }).start();
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
        lineChart.setVisibleXRangeMaximum(5); // แสดงข้อมูลล่าสุด 5 ค่าเท่านั้น
        lineChart.moveViewToX(data.getEntryCount()); // เลื่อนกราฟไปที่ค่าล่าสุด
    }

    private LineDataSet createDataSet() {
        LineDataSet dataSet = new LineDataSet(null, "Realtime Data");
        dataSet.setDrawValues(true);
        dataSet.setDrawCircles(true);
        dataSet.setMode(LineDataSet.Mode.CUBIC_BEZIER);
        dataSet.setLineWidth(10f);
        dataSet.setColor(getResources().getColor(R.color.bg_IADD_SC));
        return dataSet;
    }


}