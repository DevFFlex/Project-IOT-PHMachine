package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.os.Build;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TimePicker;

import androidx.appcompat.app.AlertDialog;

import com.example.phprojectapp.Variable.WorkTimer;
import com.example.phprojectapp.R;

interface DialogEditWorkTimerEvent{
    void onClickOk(WorkTimer workTimer);
    void onClickDelete();
}

class DialogEditWorkTimerClass extends AlertDialog.Builder{
    View view;
    AlertDialog dialog;

    DialogEditWorkTimerEvent dialogEditWorkTimerEvent;
    public void setTimePickerDialogEvent(DialogEditWorkTimerEvent dialogEditWorkTimerEvent) {
        this.dialogEditWorkTimerEvent = dialogEditWorkTimerEvent;
    }

    public Button editworktimer_btnOk,editworktimer_btnDelete;
    public EditText editworktimer_etInputPH;
    public EditText editworktimer_etInputT;
    public Switch editworktimer_swAC;
    public TimePicker editworktimer_timepicker;


    public DialogEditWorkTimerClass(Context context,WorkTimer workTimer) {
        super(context);
        view = LayoutInflater.from(context).inflate(R.layout.edit_worktimer_view,null);

        editworktimer_timepicker = view.findViewById(R.id.editworktimer_timerpicker);
        editworktimer_etInputPH = view.findViewById(R.id.editworktimer_etPH);
        editworktimer_etInputT = view.findViewById(R.id.editworktimer_etT);
        editworktimer_swAC = view.findViewById(R.id.editworktimer_swAS);
        editworktimer_btnOk = view.findViewById(R.id.editworktimer_btnOk);
        editworktimer_btnDelete = view.findViewById(R.id.editworktimer_btnDelete);

        if(workTimer.HOUR != -1){
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                editworktimer_timepicker.setHour(workTimer.HOUR);
                editworktimer_timepicker.setMinute(workTimer.MINUTE);
            }

            editworktimer_etInputPH.setText(String.valueOf(workTimer.PH));
            editworktimer_etInputT.setText(String.valueOf(workTimer.T));
            editworktimer_swAC.setChecked(workTimer.ACTIVE_STATUS);
        }

        editworktimer_timepicker.setIs24HourView(true);
        editworktimer_btnOk.setOnClickListener(this::onClickOK);
        editworktimer_btnDelete.setOnClickListener(this::onClickDelete);

        this.setView(view);
        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.layout_style1);
        dialog.show();

    }

    private void onClickOK(View v){
        WorkTimer workTimer = new WorkTimer();

        workTimer.HOUR = editworktimer_timepicker.getCurrentHour();
        workTimer.MINUTE = editworktimer_timepicker.getCurrentMinute();

        try {
            workTimer.PH = Float.parseFloat(editworktimer_etInputPH.getText().toString());
            if(workTimer.PH < 4 || workTimer.PH > 10)return;
        }catch (Exception e){
            return;
        }

        int T;
        try {
            workTimer.T = Integer.parseInt(editworktimer_etInputT.getText().toString());
            if(workTimer.T <= 0 || workTimer.T > 20)return;
        }catch (Exception e){
            return;
        }

        workTimer.ACTIVE_STATUS = editworktimer_swAC.isChecked();
        workTimer.DELETE_STATUS = false;

        dialogEditWorkTimerEvent.onClickOk(workTimer);

        dialog.dismiss();
    }

    private void onClickDelete(View v){
        dialogEditWorkTimerEvent.onClickDelete();
        dialog.dismiss();
    }
}