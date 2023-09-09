package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.example.phprojectapp.Variable.WorkTimer;
import com.example.phprojectapp.R;

interface ItemLayoutEvent{
    void onDelete(WorkTimer workTimer);
    void onSwitch(WorkTimer workTimer, boolean status);
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

    WorkTimer workTimer;

    private String default_status_on  = "เปิด" ;
    private String default_status_off = "ปิด" ;

    public ItemLayout(Context context, WorkTimer workTimer) {
        super(context);
        this.workTimer = workTimer;

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

    public void setValue(){

        int hour = workTimer.HOUR;
        int minute = workTimer.MINUTE;
        float ph = workTimer.PH;
        boolean active_status = workTimer.ACTIVE_STATUS;


        if(active_status)switch_status.setText(default_status_on);
        else switch_status.setText(default_status_off);

        switch_status.setChecked(active_status);
        textView_time.setText(String.format("%02d:%02d:**",hour,minute));

        textView_ph.setText(String.valueOf(ph));
    }


    private void onClickDelete(View v){
        itemLayoutEvent.onDelete(workTimer);
    }

    private void onClickSwitch(CompoundButton compoundButton, boolean b){
        if (b) switch_status.setText(default_status_on);
        else switch_status.setText(default_status_off);

        if(itemLayoutEvent != null)itemLayoutEvent.onSwitch(workTimer,b);
    }


}