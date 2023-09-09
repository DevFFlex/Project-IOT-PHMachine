package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.OnComunityEventListener;
import com.example.phprojectapp.Variable.Variable;
import com.example.phprojectapp.Dialog.DialogEventListener;
import com.example.phprojectapp.Dialog.TestDialog;

import java.util.ArrayList;


public class AdminFragment extends Fragment {
    private Variable var;
    private Button btn_relay[] = new Button[6];
    private Button btn_sendData;
    private Button btn_sendSerial;
    private EditText et_serialinput;
    private TextView[] tv_fsw = new TextView[3];
    private TextView waitStirringPump,waitPHStabilize,useBaseTime,useAcidTime,ph_space_rate,limite_use_base,limite_use_acid;

    private Spinner spn_commandlist;
    private String commandlists[] = {
            "var setFSW <ตัวที่ int> <สถานะ 0,1 int>",
            "var setWAIT_STR_PUMP <เวลา int>",
            "var setWAIT_PH_STABILIZE <เวลา int>",
            "var setWAIT_BASEUSETIME <เวลา int>",
            "var setWAIT_ACIDUSETIME <เวลา int>",
            "var setLIMITE_USE_BASE <จำนวนลิมิต int>",
            "var setLIMITE_USE_ACID <จำนวนลิมิต int>",
            "var setINPUTPH  <ตัวเลข float>",
            "var setMIXTANKPH <ตัวเลข float>",
            "var setPHSPACERATE <ตัวเลข float>",
            "var setInternetSSID <ชื่อไวไฟ String>",
            "var setInternetPASS <รหัสไวไฟ String>",
            "buzzer <ความถี่ int>",
            "cloud testSend",

    };
    private ArrayAdapter<String> arrayFormat(String data[]){
        ArrayList<String> itemList = new ArrayList<>();

        for (String item:data) itemList.add(item);

        ArrayAdapter<String> adapter = new ArrayAdapter<>(getContext(), android.R.layout.simple_spinner_dropdown_item, itemList);

        return adapter;
    }



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

        tv_fsw[0] = v.findViewById(R.id.admin_tvFSW1);
        tv_fsw[1] = v.findViewById(R.id.admin_tvFSW2);

        waitStirringPump = v.findViewById(R.id.admin_tvWaitStirringPump);
        waitPHStabilize = v.findViewById(R.id.admin_tvWaitpHStabilize);
        useBaseTime = v.findViewById(R.id.admin_tvUsebaseTime);
        useAcidTime = v.findViewById(R.id.admin_tvUseracidTime);
        limite_use_base = v.findViewById(R.id.admin_tvLimiteBase);
        limite_use_acid = v.findViewById(R.id.admin_tvLimiteAcid);
        ph_space_rate = v.findViewById(R.id.admin_tvPH_Space_Rate);

        btn_sendSerial = v.findViewById(R.id.admin_btnSendSerial);
        et_serialinput = v.findViewById(R.id.admin_etSerialInput);

        spn_commandlist = v.findViewById(R.id.admin_commandlist);
        spn_commandlist.setAdapter(arrayFormat(commandlists));
        spn_commandlist.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                et_serialinput.setText(commandlists[i]);
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        btn_sendSerial.setOnClickListener(this::onSendSerial);


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

                for(int i = 0;i<6;i++){
                    if(!var.relay_status[i])btn_relay[i].setBackgroundResource(R.drawable.btn_style4);
                    else btn_relay[i].setBackgroundResource(R.drawable.btn_style3);
                }

                tv_fsw[0].setText(String.valueOf(var.fsw[0]));
                tv_fsw[1].setText(String.valueOf(var.fsw[1]));

                waitStirringPump.setText(String.valueOf(var.wait_stirringPump));
                waitPHStabilize.setText(String.valueOf(var.wait_pHStabilize));
                useBaseTime.setText(String.valueOf(var.baseUseTime));
                useAcidTime.setText(String.valueOf(var.acidUseTime));
                limite_use_base.setText(String.valueOf(var.limite_use_base));
                limite_use_acid.setText(String.valueOf(var.limite_use_acid));
                ph_space_rate.setText(String.valueOf(var.pH_space_rate));
                handler.postDelayed(this,1000);
            }
        });


        return v;
    }


    private void onSendSerial(View v){
        var.extension.printDebug("G",et_serialinput.getText().toString());
        var.comunity.serverSendSerial(et_serialinput.getText().toString());
    }

    private void toggleRelay(int index){
        var.comunity.serverSetRelay("toggle",index,-1);
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


}