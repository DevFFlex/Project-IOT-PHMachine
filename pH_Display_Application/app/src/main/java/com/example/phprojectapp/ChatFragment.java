package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.phprojectapp.ClassEx.ComunityListener;
import com.example.phprojectapp.ClassEx.Variable;

import org.w3c.dom.Text;

import java.util.ArrayList;


public class ChatFragment extends Fragment {

    private Variable var;
    private View view;

    private ArrayList<String> chat_his = new ArrayList<String>();

    private Button send;
    private LinearLayout chat_message_block;
    private EditText input;

    public void updateItem(){

    }

    public ChatFragment(Variable variable) {
        this.var = variable;


        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
//                var.extension.printDebug("ChatFrag", "on update chat " + String.valueOf(chat_message_block == null));
                if (chat_message_block != null) {
                    try {
                        chat_message_block.removeAllViews();
                        String old_name = "";
                        for (String[] chat : var.chat_his) {
                            String client_name = chat[0];
                            String message = chat[1];

                            if (client_name.equals(var.comunity.USERNAME)) {
                                View message_item_viwe = LayoutInflater.from(variable.context).inflate(R.layout.chat_message_item_r, null);
                                TextView tr = message_item_viwe.findViewById(R.id.chat_item_tvR);
                                tr.setText(message);
                                chat_message_block.addView(message_item_viwe);
                            } else {
                                View message_item_viwe = LayoutInflater.from(variable.context).inflate(R.layout.chat_message_item_l, null);
                                TextView tl = message_item_viwe.findViewById(R.id.chat_item_tvL);
                                TextView tln = message_item_viwe.findViewById(R.id.chat_item_tvLN);
                                tl.setText(message);
                                if(client_name.equals(old_name)){
                                    tln.setVisibility(View.GONE);
                                }
                                else{
                                    tln.setText(client_name);
                                }

                                chat_message_block.addView(message_item_viwe);
                            }

                            old_name = client_name;
                        }
//                        var.extension.printDebug("ChatFrag","Update Chat sucess");
                    }catch (Exception e){
//                        var.extension.printError("ChatFrag","Error Array");
                    }

                }

                handler.postDelayed(this, 1000);
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

        chat_message_block = view.findViewById(R.id.chat_message_block);
//        WebView myWebView = (WebView) view.findViewById(R.id.webview);
//        myWebView.loadUrl("http://www.google.com");


        return view;
    }


}