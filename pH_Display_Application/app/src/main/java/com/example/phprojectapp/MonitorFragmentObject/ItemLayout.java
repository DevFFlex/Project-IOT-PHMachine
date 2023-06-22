package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.TimeObject;
import com.example.phprojectapp.R;

interface ItemLayoutEvent{
    void onDelete(TimeObject timeObject);
    void onSwitch(TimeObject timeObject,boolean status);
}

class ItemLayout extends View {
    View itemView;

    ItemLayoutEvent itemLayoutEvent;
    public void setItemLayoutEvent(ItemLayoutEvent itemLayoutEvent) {
        this.itemLayoutEvent = itemLayoutEvent;
    }

    Switch switch_status;
    TextView textView_time;
    TextView textView_ph;
    Button button_delete;

    TimeObject timeObject;

    private String default_status_on  = "เปิด" ;
    private String default_status_off = "ปิด" ;

    public ItemLayout(Context context,TimeObject timeObject) {
        super(context);
        this.timeObject = timeObject;

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

    private void setValue(){

        int hour = timeObject.getHour();
        int minute = timeObject.getMinute();
        boolean status = timeObject.isStatus();
        float ph = timeObject.getPh();

        if(status)switch_status.setText(default_status_on);
        else switch_status.setText(default_status_off);
        switch_status.setChecked(timeObject.isStatus());

        textView_time.setText(String.format("%02d:%02d:00",hour,minute));

        textView_ph.setText(String.valueOf(ph));
    }


    private void onClickDelete(View v){
        itemLayoutEvent.onDelete(timeObject);
    }

    private void onClickSwitch(CompoundButton compoundButton, boolean b){
        if (b) switch_status.setText(default_status_on);
        else switch_status.setText(default_status_off);

        if(itemLayoutEvent != null)itemLayoutEvent.onSwitch(timeObject,b);
    }


}