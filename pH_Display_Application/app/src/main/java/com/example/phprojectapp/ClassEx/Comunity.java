package com.example.phprojectapp.ClassEx;


import android.os.Handler;

import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.Variable.Variable;



public class Comunity extends Client{

    private OnComunityEventListener onComunityEventListener;
    public void setOnComunityEventListener(OnComunityEventListener onComunityEventListener) {
        this.onComunityEventListener = onComunityEventListener;
    }

    private Variable var;
    private Handler handler;

    public Comunity(Variable var){
        super(var);
        this.var = var;

        this.setListener(this::onMessangeEvent);

    }

    private void send(String command,String value){
        String string = String.format("%s%s%s%s%s",DEFAULT_USERNAME,":",command,"=",value);
        sendToServer(string);
//        var.extension.printDebug("Comunity",string);
    }

    public void setInputPH(float value){
        send("ADJ_PH",String.valueOf(value));
    }

    public void setInputPH_STOP(){
        send("ADJ_PH","stop");
    }

    public void setTimeAutoWork(String value){
        send("SET_TIME_AUTO_WORK",String.valueOf(value));
    }

    public void setTimeBoard(String value){
        send("TIME_BOARD",String.valueOf(value));
    }

    public void setRelay(String header,int index,double time){
        if (index < 0 || index > 6)return;
        send("RELAY",String.format("%s,%d,%.1f",header,index,time));
    }

    public void getTimeAutoWork(){
        send("GET_TIME_AUTO_WORK","NULL");
    }

    public void getSDDir(String path){
        sendToServer("GET:FILE_DIR=" + path);
    }

    public void sendMessageChat(String name,String data){
        sendToServer("MESSAGE:" + name + "=" + data);
    }

    public String timeBoardObjectToQueryString(TimeBoardObject timeBoardObject){
        String out = "";
        out += String.valueOf(timeBoardObject.hour) + ",";
        out += String.valueOf(timeBoardObject.minute) + ",";
        out += String.valueOf(timeBoardObject.second) + ",";
        out += String.valueOf(timeBoardObject.dayofweek) + ",";
        out += String.valueOf(timeBoardObject.dayofmonth) + ",";
        out += String.valueOf(timeBoardObject.month) + ",";
        out += String.valueOf(timeBoardObject.year % 2000);
        return out;
    }



    private void onMessangeEvent(String header,String command,String value){
        //------------------------------------ Client Auto Recv -----------------
        dataAuto(header,command,value);
        //------------------------------------ Client Response Recv -----------------
        dataResponse(header,command,value);
    }

    private void dataAuto(String header,String command,String value){
        if(command.equals("UPDATE")){
            String d1[] = value.split(",");

            var.mixtankPH = Float.valueOf(d1[0]);
            if(!d1[1].equals("nan"))var.tempC = Float.valueOf(d1[1]);
            if(!d1[2].equals("nan"))var.humidity = Float.valueOf(d1[2]);

            var.step = Integer.valueOf(d1[3]);
            var.work_status = (d1[4].equals("0")) ? false : true;

            var.relay_status[0] = (d1[5].equals("0")) ? false : true;
            var.relay_status[1] = (d1[6].equals("0")) ? false : true;
            var.relay_status[2] = (d1[7].equals("0")) ? false : true;
            var.relay_status[3] = (d1[8].equals("0")) ? false : true;
            var.relay_status[4] = (d1[9].equals("0")) ? false : true;
            var.relay_status[5] = (d1[10].equals("0")) ? false : true;

            var.timeBoardObject.hour = Integer.parseInt(d1[11]);
            var.timeBoardObject.minute = Integer.parseInt(d1[12]);
            var.timeBoardObject.second = Integer.parseInt(d1[13]);

            var.fsw[0] = Integer.parseInt(d1[14]);
            var.fsw[1] = Integer.parseInt(d1[15]);

            var.wifi_board_connected = (d1[16].equals("0")) ? false : true;

        }

        if(command.equals("OUTPUT")){
            var.outout_text = value;
            var.extension.printDebug("Comunity","OUTPUT");
        }

        if(command.equals("SETSTEPTEXT")){
            var.stepText = value;
        }
    }

    private void dataResponse(String header,String command,String value){

        if(command.equals("GET_TIME_AUTO_WORK_RES")){
            var.extension.printDebug("Comunity","Recv TAW");

            String[] data1 = value.split("#");

            var.timeObjectList.clearAllItem();
            for(int i = 0;i<4;i++){
                String[] data2 = data1[i].split(",");

                if(data2[4].equals("false")){

                    var.timeObjectList.addItem(Integer.parseInt(data2[0]),Integer.parseInt(data2[1]),(data2[2].equals("false")) ? false : true,Float.parseFloat(data2[3]));
//                    var.timeObjectList.data.get(i).setHour();
//                    var.timeObjectList.data.get(i).setMinute();
//                    var.timeObjectList.data.get(i).setStatus( );
//                    var.timeObjectList.data.get(i).setPh();

                }
                System.out.println(data1[i]);

            }
        }

        if(command.equals("INPUT_RES")){
            var.inputPH = Float.parseFloat(value);
        }

    }




}