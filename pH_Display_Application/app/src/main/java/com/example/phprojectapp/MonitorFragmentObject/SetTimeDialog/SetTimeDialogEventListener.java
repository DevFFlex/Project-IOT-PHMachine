package com.example.phprojectapp.MonitorFragmentObject.SetTimeDialog;

import com.example.phprojectapp.ClassEx.TimeObject;

public interface SetTimeDialogEventListener {
    void onClickAddTimeOk(TimeObject timeObject);

    void onClickItemSwitch(TimeObject timeObject,boolean b);
    void onClickItemDelete(TimeObject timeObject);


}
