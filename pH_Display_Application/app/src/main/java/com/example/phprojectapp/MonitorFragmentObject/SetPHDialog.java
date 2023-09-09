package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import androidx.appcompat.app.AlertDialog;

import com.example.phprojectapp.AnimationOption;
import com.example.phprojectapp.R;

import org.w3c.dom.Text;

interface SetPHDialogEvent{
    void onClickOk(float pH,int T);
}

public class SetPHDialog extends AlertDialog.Builder {
    AnimationOption animationOption;
    View input_ph_view;
    AlertDialog dialog;

    private SetPHDialogEvent setPHDialogEvent;
    public void setSetPHDialogEvent(SetPHDialogEvent setPHDialogEvent) {
        this.setPHDialogEvent = setPHDialogEvent;
    }

    public SetPHDialog(Context context, AnimationOption animationOption) {
        super(context);
        this.animationOption = animationOption;

        input_ph_view = LayoutInflater.from(context).inflate(R.layout.input_ph, null);

        Button inputPH_btnOk = input_ph_view.findViewById(R.id.inputPH_btnOk);
        EditText inputPH_et1 = input_ph_view.findViewById(R.id.inputPH_et1);
        EditText inputT_et1 = input_ph_view.findViewById(R.id.inputT_et1);
        TextView inputPH_tvWarning = input_ph_view.findViewById(R.id.inputPH_tvWarning);
        TextView inputT_tvWarning = input_ph_view.findViewById(R.id.inputT_tvWarning);


        inputPH_btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String inputPH_val = inputPH_et1.getText().toString();
                String inputT_val = inputT_et1.getText().toString();

                if(inputPH_val.equals("") || inputT_val.equals("")){
                    if(inputPH_val.equals("")){
                        inputPH_tvWarning.setText("กรุณาใส่ pH");
                        inputPH_tvWarning.setVisibility(View.VISIBLE);
                    }else inputPH_tvWarning.setVisibility(View.INVISIBLE);
                    if(inputT_val.equals("")){
                        inputT_tvWarning.setText("กรุณาใส่จำนวนรอบ");
                        inputT_tvWarning.setVisibility(View.VISIBLE);
                    }else inputT_tvWarning.setVisibility(View.INVISIBLE);

                }else{
                    float pH = -1;
                    int T = -1;

                    try {

                        pH = Float.parseFloat(inputPH_val);
                        T = Integer.parseInt(inputT_val);

                        if (pH < 4 || pH > 10) {
                            inputPH_tvWarning.setText("ใส่ได้ตั้งเเต่ค่า 4 ถึง 10 เท่านั้น");
                            inputPH_tvWarning.setVisibility(View.VISIBLE);
                            return;
                        }


                    } catch (Exception e) {}

                    setPHDialogEvent.onClickOk(pH,T);
                    dialog.dismiss();
                }
            }
        });
        this.setView(input_ph_view);

        dialog = this.create();
        dialog.getWindow().setBackgroundDrawableResource(R.drawable.bg_dialog_phinput);

        Window window = dialog.getWindow();
        if (window != null)window.setWindowAnimations(R.style.MyAlertDialogAnimation);
        else System.out.println("window == NULL");
        dialog.show();


//        animationOption.startAnimation(input_ph_view,R.animator.animator_set1);

    }
}
