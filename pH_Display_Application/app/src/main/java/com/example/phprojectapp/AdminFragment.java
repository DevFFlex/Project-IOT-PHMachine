package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.example.phprojectapp.ClassEx.Variable;
import com.example.phprojectapp.Dialog.DialogEventListener;
import com.example.phprojectapp.Dialog.TestDialog;


public class AdminFragment extends Fragment {
    private Variable var;
    private Button btn_relay1,btn_relay2,btn_relay3,btn_relay4,btn_relay5,btn_relay6,btn_sendData;
    private Button btn_setArduinoValue,btn_getArduinoDefault;
    private EditText et_calibrate,et_m,et_voltin,et_maxanalog;


    public AdminFragment(Variable variable) {
        this.var = variable;
    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.fragment_admin, container, false);

        btn_relay1 = v.findViewById(R.id.admin_btn_relay1);
        btn_relay2 = v.findViewById(R.id.admin_btn_relay2);
        btn_relay3 = v.findViewById(R.id.admin_btn_relay3);
        btn_relay4 = v.findViewById(R.id.admin_btn_relay4);
        btn_relay5 = v.findViewById(R.id.admin_btn_relay5);
        btn_relay6 = v.findViewById(R.id.admin_btn_relay6);
        btn_sendData = v.findViewById(R.id.admin_btnSendTest);

        btn_setArduinoValue = v.findViewById(R.id.admin_btn_arduinoSetvalue);
        btn_getArduinoDefault = v.findViewById(R.id.admin_btn_arduinoGetdefault);

        et_calibrate = v.findViewById(R.id.admin_et_calibration);
        et_m = v.findViewById(R.id.admin_et_m);
        et_voltin = v.findViewById(R.id.admin_et_voltin);
        et_maxanalog = v.findViewById(R.id.admin_et_maxAnalog);

        btn_setArduinoValue.setOnClickListener(this::onClickSetArduinoValue);
        btn_getArduinoDefault.setOnClickListener(this::onClickGetArduinoDefaultValue);


        btn_relay1.setOnClickListener(this::onToggleRelay1);
        btn_relay2.setOnClickListener(this::onToggleRelay2);
        btn_relay3.setOnClickListener(this::onToggleRelay3);
        btn_relay4.setOnClickListener(this::onToggleRelay4);
        btn_relay5.setOnClickListener(this::onToggleRelay5);
        btn_relay6.setOnClickListener(this::onToggleRelay6);
        btn_sendData.setOnClickListener(this::onSendData);


        return v;
    }

    private void toggleRelay(int index){
        var.comunity.setToggleRelay(index);
    }

    public void onToggleRelay1(View v){
        toggleRelay(0);
    }
    public void onToggleRelay2(View v){
        toggleRelay(1);
    }
    public void onToggleRelay3(View v){
        toggleRelay(2);
    }
    public void onToggleRelay4(View v){
        toggleRelay(3);
    }
    public void onToggleRelay5(View v){
        toggleRelay(4);
    }
    public void onToggleRelay6(View v){
        toggleRelay(5);
    }

    public void onSendData(View v){
        TestDialog testDialog = new TestDialog(getContext());
        testDialog.setListener(new DialogEventListener() {
            @Override
            public void onTestDialog_Ok(String str) {
                var.comunity.sendToServer(str);
            }
        });
    }


    public void onClickSetArduinoValue(View v){
        var.extension.printDebug("AdminFrag","on Click Set Arduino Value");
    }

    public void onClickGetArduinoDefaultValue(View v){
        var.extension.printDebug("adminFrag","on Click get Arduino default value");

        var.comunity.getCMVM();
    }

}