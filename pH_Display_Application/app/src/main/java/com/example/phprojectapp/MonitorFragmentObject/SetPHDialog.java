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
interface SetPHDialogEvent{
    void onClickOk(float value);
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
        TextView inputPH_tvWarning = input_ph_view.findViewById(R.id.inputPH_tvWarning);


        inputPH_btnOk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String value = inputPH_et1.getText().toString();

                if(value.equals("")){
                    inputPH_tvWarning.setText("กรุณาใส่ pH");
                    inputPH_tvWarning.setVisibility(View.VISIBLE);
                    return;
                }

                try {

                    final float value_float = Float.parseFloat(value);

                    if (value_float < 4 || value_float > 10) {
                        inputPH_tvWarning.setText("ใส่ได้ตั้งเเต่ค่า 4 ถึง 10 เท่านั้น");
                        inputPH_tvWarning.setVisibility(View.VISIBLE);
                        return;
                    }
                    setPHDialogEvent.onClickOk(value_float);
                    dialog.dismiss();


                } catch (Exception e) {}

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
