package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TimePicker;

import androidx.appcompat.app.AlertDialog;

import com.example.phprojectapp.ClassEx.TimeObject;
import com.example.phprojectapp.R;

interface TimePickerDialogEvent{
    void onClickOk(TimeObject timeObject);
}

class TimePickerDialog extends AlertDialog.Builder{
    View view;
    AlertDialog dialog;

    TimePickerDialogEvent timePickerDialogEvent;
    public void setTimePickerDialogEvent(TimePickerDialogEvent timePickerDialogEvent) {
        this.timePickerDialogEvent = timePickerDialogEvent;
    }

    public Button inputtime_btnOk;
    public EditText inputtime_etPH;
    public TimePicker inputtime_tpk;


    public TimePickerDialog(Context context) {
        super(context);

        view = LayoutInflater.from(context).inflate(R.layout.input_time,null);

        inputtime_btnOk = view.findViewById(R.id.inputtime_btnOk);
        inputtime_etPH = view.findViewById(R.id.inputtime_etPH);
        inputtime_tpk = view.findViewById(R.id.inputtime_tpk);

        inputtime_tpk.setIs24HourView(true);

        inputtime_btnOk.setOnClickListener(this::ok);

        this.setView(view);
        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.layout_style1);
        dialog.show();

    }

    private void ok(View v){
        TimeObject timeObject = new TimeObject();

        timeObject.setHour(inputtime_tpk.getCurrentHour());
        timeObject.setMinute(inputtime_tpk.getCurrentMinute());

        float ph_nd;
        try {
            ph_nd = Float.parseFloat(inputtime_etPH.getText().toString());
        }catch (Exception e){
            return;
        }

        timeObject.setPh(ph_nd);
        timeObject.setStatus(false);

        timePickerDialogEvent.onClickOk(timeObject);

        dialog.dismiss();
    }
}