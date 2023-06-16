package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.ComunityListener;
import com.example.phprojectapp.ClassEx.Variable;

import java.util.ArrayList;


public class ChatFragment extends Fragment {

    private Variable var;
    private View view;

    private ArrayList<String> chat_his = new ArrayList<String>();

    private Button send;
    private TextView output;
    private EditText input;

    public ChatFragment(Variable variable) {
        this.var = variable;
        var.comunity.setListener(new ComunityListener() {
            @Override
            public void onMessageFromUser(String client_name, String value) {
                String output_text = output.getText().toString();
                output_text += "\n";
                output_text += client_name + "\t|\t" + value + "\n";
                String finalOutput_text = output_text;
                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        output.setText(finalOutput_text);
                    }
                });
            }
        });
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_chat, container, false);

        send = view.findViewById(R.id.chat_sendbtn);
        input = view.findViewById(R.id.chat_et);
        output = view.findViewById(R.id.chat_tv);

        send.setOnClickListener(this::onSend);

        return view;
    }


    public void onSend(View v){
        String text = input.getText().toString();
        if(text.equals(""))return;
        var.comunity.sendMessageChat(var.comunity.USERNAME,text);
        var.extension.printAlert("sending");
        input.setText("");
    }
}