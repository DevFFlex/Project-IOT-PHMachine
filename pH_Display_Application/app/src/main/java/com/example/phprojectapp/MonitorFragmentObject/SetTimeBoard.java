package com.example.phprojectapp.MonitorFragmentObject;

import android.app.AlertDialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.R;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.TimeZone;

interface SetTimeBoardEvent{
    void onClickOk(TimeBoardObject timeBoardObject);
}

public class SetTimeBoard extends AlertDialog.Builder {

    SetTimeBoardEvent setTimeBoardEvent;
    public void setSetTimeBoardEvent(SetTimeBoardEvent setTimeBoardEvent) {
        this.setTimeBoardEvent = setTimeBoardEvent;
    }

    View stb_view;
    AlertDialog dialog;

    EditText et_hour,et_minute,et_second;
    EditText et_dayofmonth;
    Spinner spin_dayofweek,spin_month,spin_year;
    Button stb_btnOk;

    String dayofweekString[] = {
            "อาทิตย์","จันทร์","อังคาร","พุธ","พฤหัสบดี","ศุกร์","เสาร์",
    };

    String monthString[] = {
            "มกราคม","กุมภาพันธ์","มีนาคม","เมษายน","พฤษภาคม","มิถุนายน","กรกฎาคม","สิงหาคม","กันยายน","ตุลาคม","พฤศจิกายน","ธันวาคม",
    };

    String yearString[] = {
            "2020","2021","2022","2023","2024","2025","2026"
    };

    TimeBoardObject timeBoardObject;

    public SetTimeBoard(Context context) {
        super(context);
        timeBoardObject = new TimeBoardObject();

        stb_view = LayoutInflater.from(context).inflate(R.layout.settimeboard,null);

        et_hour = stb_view.findViewById(R.id.stb__hour);
        et_minute = stb_view.findViewById(R.id.stb__minute);
        et_second = stb_view.findViewById(R.id.stb__second);
        et_dayofmonth = stb_view.findViewById(R.id.stb_etDayOfMonth);
        spin_dayofweek = stb_view.findViewById(R.id.stb_spinDayOfWeek);
        spin_month = stb_view.findViewById(R.id.stb_spinMonth);
        spin_year = stb_view.findViewById(R.id.stb_spinYear);
        stb_btnOk = stb_view.findViewById(R.id.stb_btnOk);

        stb_btnOk.setOnClickListener(this::onClickOk);

        spin_dayofweek.setAdapter(arrayFormat(dayofweekString));
        spin_month.setAdapter(arrayFormat(monthString));
        spin_year.setAdapter(arrayFormat(yearString));

        spin_dayofweek.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                setObjectFormat(i,spin_dayofweek);

            }
            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        spin_month.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                setObjectFormat(i,spin_month);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });


        spin_year.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                setObjectFormat(i,spin_year);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        Calendar calendar = Calendar.getInstance();
        TimeZone timeZone = TimeZone.getTimeZone("Asia/Bangkok");
        calendar.setTimeZone(timeZone);

        et_hour.setText(String.valueOf(calendar.get(Calendar.HOUR_OF_DAY)));
        et_minute.setText(String.valueOf(calendar.get(Calendar.MINUTE)));
        et_second.setText(String.valueOf(calendar.get(Calendar.SECOND)));
        et_dayofmonth.setText(String.valueOf(calendar.get(Calendar.DAY_OF_MONTH)));

        int dayofweek_select = (calendar.get(Calendar.DAY_OF_WEEK) == 6) ? 0 : calendar.get(Calendar.DAY_OF_WEEK) - 1;
        spin_dayofweek.setSelection(dayofweek_select);

        spin_month.setSelection(calendar.get(Calendar.MONTH));
        spin_year.setSelection(3);


        setView(stb_view);
        dialog = this.create();
        dialog.show();


    }

    private void setObjectFormat(int index,Spinner spinner){
        if(spinner.equals(spin_dayofweek)){
            timeBoardObject.dayofweek = index + 1;
        }else if(spinner.equals(spin_month)){
            timeBoardObject.month = index + 1;
        }else if(spinner.equals(spin_year)){
            timeBoardObject.year = Integer.valueOf(yearString[index]);
        }
    }

    private ArrayAdapter<String> arrayFormat(String data[]){
        ArrayList<String> itemList = new ArrayList<>();

        for (String item:data) itemList.add(item);

        ArrayAdapter<String> adapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_spinner_dropdown_item, itemList);

        return adapter;
    }


    private void onClickOk(View v){

        timeBoardObject.hour = Integer.valueOf(et_hour.getText().toString());
        timeBoardObject.minute = Integer.valueOf(et_minute.getText().toString());
        timeBoardObject.second = Integer.valueOf(et_second.getText().toString());

        timeBoardObject.dayofmonth = Integer.valueOf(et_dayofmonth.getText().toString());

        if(timeBoardObject.isEmptyAll())return;

        setTimeBoardEvent.onClickOk(timeBoardObject);

    }

}
