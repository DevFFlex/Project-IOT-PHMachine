package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.TextView;

import com.example.phprojectapp.R;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.Variable.WorkTimer;

public class WorkTimerItemLayout extends LinearLayout {
    View view;
    Variable var;
    int index;
    TextView Time,PH,T;
    ImageView Active_Status;
    Button Btn_Edit;

    WorkTimer workTimer;

    public WorkTimerItemLayout(Context context,int index, Variable var, WorkTimer workTimer) {
        super(context);
        this.var = var;
        this.index = index;
        this.workTimer = workTimer;
        view = LayoutInflater.from(context).inflate(R.layout.worktimer_item,this, true);

        Time = view.findViewById(R.id.item_workTimerView_tvTime);
        PH = view.findViewById(R.id.item_workTimerView_tvPH);
        T = view.findViewById(R.id.item_workTimerView_tvT);
        Active_Status = view.findViewById(R.id.item_workTimerView_ivAS);
        Btn_Edit = view.findViewById(R.id.item_workTimerView_btnEdit);

        Btn_Edit.setOnClickListener(this::onClickEdit);

        display();

    }

    public void display(){

        Time.setText(workTimer.timeFormattoString());
        PH.setText(String.valueOf(workTimer.PH));
        T.setText(String.valueOf(workTimer.T));
        if(workTimer.ACTIVE_STATUS)Active_Status.setBackgroundResource(R.drawable.status_circle_green);
        else Active_Status.setBackgroundResource(R.drawable.status_circle_red);
        this.invalidate();
    }

    public void onClickEdit(View v){
        DialogEditWorkTimerClass tpd = new DialogEditWorkTimerClass(v.getContext(),workTimer);
        tpd.setTimePickerDialogEvent(new DialogEditWorkTimerEvent() {
            @Override
            public void onClickOk(WorkTimer workTimer) {
                var.extension.printDebug("WorkTimerItemLayout","onClick  OK");
                var.comunity.serverSetWorkTimer(index,workTimer.HOUR,workTimer.MINUTE,workTimer.PH,workTimer.T,workTimer.ACTIVE_STATUS,workTimer.DELETE_STATUS);
            }

            @Override
            public void onClickDelete() {
                var.extension.printDebug("WorkTimerItemLayout","onClick   Delete");
                var.comunity.serverSetWorkTimer(index,-1,-1,-1,-1,false,true);
            }
        });
    }


}
