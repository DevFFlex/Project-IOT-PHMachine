package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;

import androidx.appcompat.app.AlertDialog;

import com.example.phprojectapp.Variable.WorkTimer;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.R;


interface SetTimeDialogEvent{
    void onClickSave(String output);
}

public class SetTimeDialog extends AlertDialog.Builder{
    private View view;
    private AlertDialog dialog;
    private Variable variable;

    private SetTimeDialogEvent setTimeDialogEvent;
    public void setSetTimeDialogEvent(SetTimeDialogEvent setTimeDialogEvent) {
        this.setTimeDialogEvent = setTimeDialogEvent;
    }


    private LinearLayout box;
    Button btnAdd,btnCancel,btnSave;

    private int itemCountMax = 4;

    public SetTimeDialog(Context context, Variable variable) {
        super(context);
        this.variable = variable;

        view = LayoutInflater.from(context).inflate(R.layout.list_time,null);

        box = view.findViewById(R.id.listtime_itemView);

        btnAdd = view.findViewById(R.id.btnAdd);
        btnAdd.setOnClickListener(this::onClickAdd);

        btnCancel = view.findViewById(R.id.listtime_btnExit);
        btnCancel.setOnClickListener(this::onClickCancel);

        btnSave = view.findViewById(R.id.btnSave);
        btnSave.setOnClickListener(this::onClickSave);

        updateItemLayout();
        setView(view);

        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_listtime);

        Window window = dialog.getWindow();
        if (window != null)window.setWindowAnimations(R.style.MyAlertDialogAnimation);
        else System.out.println("window == NULL");
        dialog.show();



    }

    public void updateItemLayout(){
        /*box.removeAllViewsInLayout();
        for (WorkTimer workTimer :variable.workTimerList.data) {
            WorkTimer ttt = workTimer;
            ItemLayout itemLayout = new ItemLayout(getContext(), workTimer);
            itemLayout.setItemLayoutEvent(new ItemLayoutEvent() {
                @Override
                public void onDelete(WorkTimer workTimer) {
                    variable.workTimerList.data.remove(ttt);
                    updateItemLayout();
                }

                @Override
                public void onSwitch(WorkTimer workTimer, boolean status) {
                    variable.workTimerList.getTimeObject(ttt).ACTIVE_STATUS = status;

                }
            });

            box.addView(itemLayout.itemView);
        }*/
    }

    private void onClickAdd(View v){
//        if (variable.workTimerList.getSize() >= itemCountMax) return;

    }

    private void onClickCancel(View v){
        dialog.dismiss();
    }

    private void onClickSave(View v){
        String out = "";

        for (int i = 0;i<4;i++) {
            WorkTimer workTimer = null;
            boolean has = false;
            try{
//                workTimer = variable.workTimerList.slot_list[i];
                has = true;
            }catch (Exception e){}


            if(has){
                out += String.valueOf(workTimer.HOUR) + ",";
                out += String.valueOf(workTimer.MINUTE) + ",";
                out += String.valueOf(workTimer.ACTIVE_STATUS) + ",";
                out += String.valueOf(workTimer.PH) + "";
            }else{
                out += String.valueOf(-1) + ",";
                out += String.valueOf(-1) + ",";
                out += String.valueOf(-1) + ",";
                out += String.valueOf(false) + ",";
                out += String.valueOf(-1) + "";
            }

            if (i <= 2) out += "|";
        }


        setTimeDialogEvent.onClickSave(out);
        dialog.dismiss();
    }


}
