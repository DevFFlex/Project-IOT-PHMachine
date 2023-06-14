package com.example.phprojectapp.MonitorFragmentObject;

import android.app.AlertDialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.example.phprojectapp.R;

interface TestDialogEvent{
    void onClickOk(String str);
}

public class TestDialog extends AlertDialog.Builder {
    private TestDialogEvent testDialogEvent;
    public void setTestDialogEvent(TestDialogEvent testDialogEvent) {
        this.testDialogEvent = testDialogEvent;
    }

    private View view;
    private EditText et1;
    private Button btn1;

    private AlertDialog dialog;


    public TestDialog(Context context) {
        super(context);

        view = LayoutInflater.from(context).inflate(R.layout.testdialog,null);

        btn1 = view.findViewById(R.id.testdialog_btn1);
        et1 = view.findViewById(R.id.testdialog_et1);


        btn1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String inputText = et1.getText().toString();

                if (!inputText.equals("")){
                    testDialogEvent.onClickOk(inputText);
                    et1.setText("");
                }
            }
        });

        this.setView(view);
        dialog = this.create();
        dialog.show();
    }
}
