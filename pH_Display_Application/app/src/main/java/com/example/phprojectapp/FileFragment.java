package com.example.phprojectapp;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.example.phprojectapp.Variable.Variable;


public class FileFragment extends Fragment {
    View view;
    Variable var;

    LinearLayout box_item_layout;
    TextView tv_type,tv_name;
    Button btn_del;

    String current_path = "";


    public FileFragment(Variable var) {
        this.var = var;
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);




    }

    public void update(){
        box_item_layout.removeAllViews();
        for(int i = 0;i<var.file_list.size();i++){
            View item_view = LayoutInflater.from(getContext()).inflate(R.layout.file_item,null);

            TextView type_view = item_view.findViewById(R.id.file_tv_type);
            Button name_view = item_view.findViewById(R.id.file_tv_name);
            Button   btn_del = item_view.findViewById(R.id.file_btn_del);

            String []data = var.file_list.get(i);
            type_view.setText(data[0]);
            name_view.setText(data[1]);

            name_view.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    current_path += "/" + data[1];
                }
            });

            btn_del.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    var.file_list.remove(data);
                    update();
                }
            });

            box_item_layout.addView(item_view);
            box_item_layout.invalidate();

        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_file, container, false);

        box_item_layout = view.findViewById(R.id.file_filelayout);

        update();

        return view;
    }
}