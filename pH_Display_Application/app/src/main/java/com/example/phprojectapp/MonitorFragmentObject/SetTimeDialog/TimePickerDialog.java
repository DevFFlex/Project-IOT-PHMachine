package com.example.phprojectapp.MonitorFragmentObject.SetTimeDialog;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TimePicker;

import androidx.appcompat.app.AlertDialog;

import com.example.phprojectapp.ClassEx.TimeObject;
import com.example.phprojectapp.R;

import java.util.List;

class TimePickerDialog extends AlertDialog.Builder{
    View view;
    AlertDialog dialog;

    public Button inputtime_btnOk;
    public EditText inputtime_etPH;
    public TimePicker inputtime_tpk;

    SetTimeDialogEventListener listener;


    public TimePickerDialog(Context context,SetTimeDialogEventListener listener) {
        super(context);
        this.listener = listener;

        view = LayoutInflater.from(context).inflate(R.layout.input_time,null);

        inputtime_btnOk = view.findViewById(R.id.inputtime_btnOk);
        inputtime_etPH = view.findViewById(R.id.inputtime_etPH);
        inputtime_tpk = view.findViewById(R.id.inputtime_tpk);

        inputtime_tpk.setIs24HourView(true);


        inputtime_btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                TimeObject timeObject = new TimeObject();

                timeObject.setHour(inputtime_tpk.getCurrentHour());
                timeObject.setMinute(inputtime_tpk.getCurrentMinute());
                timeObject.setSecond(0);

                float ph_nd;
                try {
                    ph_nd = Float.parseFloat(inputtime_etPH.getText().toString());
                }catch (Exception e){
                    return;
                }

                timeObject.setPh(ph_nd);
                timeObject.setStatus(false);

                listener.onClickAddTimeOk(timeObject);

                dialog.dismiss();


            }
        });




        this.setView(view);
        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.layout_style1);
        dialog.show();

    }
}