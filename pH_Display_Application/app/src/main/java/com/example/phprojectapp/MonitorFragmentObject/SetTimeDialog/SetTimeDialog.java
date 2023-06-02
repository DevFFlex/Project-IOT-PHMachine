package com.example.phprojectapp.MonitorFragmentObject.SetTimeDialog;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.TimePicker;

import androidx.appcompat.app.AlertDialog;

import com.example.phprojectapp.ClassEx.TimeObject;
import com.example.phprojectapp.ClassEx.Variable;
import com.example.phprojectapp.MonitorFragmentObject.MonitorFragmentOnEvent;
import com.example.phprojectapp.R;

import java.util.List;

















public class SetTimeDialog extends AlertDialog.Builder implements SetTimeDialogEventListener{
    private MonitorFragmentOnEvent monitorFragEventListener;
    private Variable variable;
    private View listtime_view;
    private AlertDialog dialog;

    private LinearLayout listtime_itemlayout;


    Button listtime_btnAdd,listtime_btnCancel,listtime_btnSave;

    private int itemCountMax = 4;

    public SetTimeDialog(Context context, MonitorFragmentOnEvent listener, Variable variable) {
        super(context);
        this.monitorFragEventListener = listener;
        this.variable = variable;

        listtime_view = LayoutInflater.from(context).inflate(R.layout.list_time,null);

        listtime_itemlayout = listtime_view.findViewById(R.id.listtime_itemView);

        listtime_btnAdd = listtime_view.findViewById(R.id.listtime_btnAdd);
        listtime_btnAdd.setOnClickListener(this::onClickAdd);

        listtime_btnCancel = listtime_view.findViewById(R.id.listtime_btnExit);
        listtime_btnCancel.setOnClickListener(this::onClickCancel);

        listtime_btnSave = listtime_view.findViewById(R.id.listtime_btnSave);
        listtime_btnSave.setOnClickListener(this::onClickSave);

        updateItemLayout();
        setView(listtime_view);

        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_listtime);

        Window window = dialog.getWindow();
        if (window != null)window.setWindowAnimations(R.style.MyAlertDialogAnimation);
        else System.out.println("window == NULL");
        dialog.show();



    }

    public void updateItemLayout(){

        listtime_itemlayout.removeAllViewsInLayout();

        for (TimeObject timeObject:variable.getTimeObjectList().timeObjectList) {
            ItemLayout itemLayout = new ItemLayout(getContext(),this,timeObject);

            listtime_itemlayout.addView(itemLayout.getView());
        }
    }

    public void onClickAdd(View v){
        if (variable.getTimeObjectList().getSize() >= itemCountMax) return;
        new TimePickerDialog(v.getContext(),this);
    }

    public void onClickCancel(View v){
        dialog.dismiss();
    }

    public void onClickSave(View v){
        String out = "";

        for (int i = 0;i<4;i++) {
            TimeObject timeObject = null;
            boolean has = false;
            try{
                timeObject = variable.getTimeObjectList().timeObjectList.get(i);
                has = true;
            }catch (Exception e){}


            if(has){
                out += String.valueOf(timeObject.getHour()) + ",";
                out += String.valueOf(timeObject.getMinute()) + ",";
                out += String.valueOf(timeObject.getSecond()) + ",";
                out += String.valueOf(timeObject.isStatus()) + ",";
                out += String.valueOf(timeObject.getPh()) + "";
            }else{
                out += String.valueOf(-1) + ",";
                out += String.valueOf(-1) + ",";
                out += String.valueOf(-1) + ",";
                out += String.valueOf(false) + ",";
                out += String.valueOf(-1) + "";
            }

            if (i <= 2) out += "|";
        }


        monitorFragEventListener.dialogSetTime_onclick_Save(out);
        dialog.dismiss();
    }


    @Override
    public void onClickAddTimeOk(TimeObject timeObject) {
        variable.getTimeObjectList().timeObjectList.add(timeObject);
        updateItemLayout();
    }

    @Override
    public void onClickItemSwitch(TimeObject timeObject,boolean b) {
        variable.getTimeObjectList().getTimeObject(timeObject).setStatus(b);
    }

    @Override
    public void onClickItemDelete(TimeObject timeObject) {
        variable.getTimeObjectList().timeObjectList.remove(timeObject);
        updateItemLayout();
    }
}
