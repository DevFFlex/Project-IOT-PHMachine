package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.OnComunityEventListener;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.Dialog.DialogEventListener;
import com.example.phprojectapp.Dialog.TestDialog;


public class AdminFragment extends Fragment {
    private Variable var;
    private Button btn_relay[] = new Button[6];
    private Button btn_sendData;
    private Button btn_setArduinoValue,btn_setArduinoDefault;
    private EditText et_calibrate,et_m,et_voltin,et_maxanalog;
    private TextView voltcal,phcal;


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

        btn_relay[0] = v.findViewById(R.id.admin_btn_relay1);
        btn_relay[1] = v.findViewById(R.id.admin_btn_relay2);
        btn_relay[2] = v.findViewById(R.id.admin_btn_relay3);
        btn_relay[3] = v.findViewById(R.id.admin_btn_relay4);
        btn_relay[4] = v.findViewById(R.id.admin_btn_relay5);
        btn_relay[5] = v.findViewById(R.id.admin_btn_relay6);
        btn_sendData = v.findViewById(R.id.admin_btnSendTest);

        btn_setArduinoValue = v.findViewById(R.id.admin_btn_arduinoSetvalue);
        btn_setArduinoDefault = v.findViewById(R.id.admin_btn_arduinoGetdefault);

        et_calibrate = v.findViewById(R.id.admin_et_calibration);
        et_m = v.findViewById(R.id.admin_et_m);
        et_voltin = v.findViewById(R.id.admin_et_voltin);
        et_maxanalog = v.findViewById(R.id.admin_et_maxAnalog);

        voltcal = v.findViewById(R.id.admin_tv_voltcal);
        phcal   = v.findViewById(R.id.admin_tvPHcal);

        btn_setArduinoValue.setOnClickListener(this::onClickSetArduinoValue);
        btn_setArduinoDefault.setOnClickListener(this::onClickSetArduinoDefaultValue);


        btn_relay[0].setOnClickListener(this::onToggleRelay1);
        btn_relay[1].setOnClickListener(this::onToggleRelay2);
        btn_relay[2].setOnClickListener(this::onToggleRelay3);
        btn_relay[3].setOnClickListener(this::onToggleRelay4);
        btn_relay[4].setOnClickListener(this::onToggleRelay5);
        btn_relay[5].setOnClickListener(this::onToggleRelay6);
        btn_sendData.setOnClickListener(this::onSendData);

        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                voltcal.setText(var.cmvmObject.calVoltString());
                phcal.setText(var.cmvmObject.calPHString());

                for(int i = 0;i<6;i++){
                    if(var.relay_status[i])btn_relay[i].setBackgroundResource(R.drawable.btn_style4);
                    else btn_relay[i].setBackgroundResource(R.drawable.btn_style3);
                }

                handler.postDelayed(this,1000);
            }
        });


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
        String clb = et_calibrate.getText().toString();
        String m   = et_m.getText().toString();
        String voltin = et_voltin.getText().toString();
        String max_analog = et_maxanalog.getText().toString();

        String clb_s = (clb.equals("")) ? String.valueOf(var.cmvmObject.calibration) : clb;
        String m_s = (m.equals("")) ? String.valueOf(var.cmvmObject.m) : m;
        String voltin_s = (voltin.equals("")) ? String.valueOf(var.cmvmObject.voltin) : voltin;
        String ma_s = (max_analog.equals("")) ? String.valueOf(var.cmvmObject.max_analog) : max_analog;

        String format = "";
        format += clb_s + ",";
        format += m_s + ",";
        format += voltin_s + ",";
        format += ma_s;

        var.comunity.setCMVM(format);
    }

    public void onClickSetArduinoDefaultValue(View v){

        String format = "";
        format += "-9999,";
        format += "-9999,";
        format += "-9999,";
        format += "-9999";

        var.comunity.setCMVM(format);
    }

}