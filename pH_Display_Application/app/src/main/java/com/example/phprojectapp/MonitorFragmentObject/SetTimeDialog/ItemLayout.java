package com.example.phprojectapp.MonitorFragmentObject.SetTimeDialog;

import android.content.Context;
import android.graphics.Canvas;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.TimeObject;
import com.example.phprojectapp.R;

class ItemLayout extends View {
    View itemView;

    Switch switch_status;
    TextView textView_time;
    TextView textView_ph;
    Button button_delete;

    TimeObject timeObject;

    SetTimeDialogEventListener listener;

    private String default_status_on  = "เปิด" ;
    private String default_status_off = "ปิด" ;

    public ItemLayout(Context context,SetTimeDialogEventListener listener,TimeObject timeObject) {
        super(context);
        this.timeObject = timeObject;
        this.listener = listener;

        itemView = LayoutInflater.from(context).inflate(R.layout.list_time_item, null);

        switch_status = itemView.findViewById(R.id.listtimeitem_switch);
        textView_time = itemView.findViewById(R.id.listtimeitem_tvTime);
        textView_ph   = itemView.findViewById(R.id.listtimeitem_tvPH);
        button_delete = itemView.findViewById(R.id.listtimeitem_btnDelete);

        switch_status.setOnCheckedChangeListener(this::onClickSwitch);
        button_delete.setOnClickListener(this::onClickDelete);

        setValue();

        this.invalidate();

    }

    View getView(){
        return itemView;
    }


    void setValue(){

        int hour = timeObject.getHour();
        int minute = timeObject.getMinute();
        int second = timeObject.getSecond();
        boolean status = timeObject.isStatus();
        float ph = timeObject.getPh();

        if(status)switch_status.setText(default_status_on);
        else switch_status.setText(default_status_off);
        switch_status.setChecked(timeObject.isStatus());

        textView_time.setText(String.format("%02d:%02d:%02d",hour,minute,second));

        textView_ph.setText(String.valueOf(ph));
    }


    void onClickDelete(View v){

        listener.onClickItemDelete(timeObject);
    }

    void onClickSwitch(CompoundButton compoundButton, boolean b){
        if (b) switch_status.setText(default_status_on);
        else switch_status.setText(default_status_off);

        listener.onClickItemSwitch(timeObject,b);
    }


}