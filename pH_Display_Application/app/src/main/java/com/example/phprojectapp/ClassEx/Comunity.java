package com.example.phprojectapp.ClassEx;


import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.Variable.Variable;

public class Comunity extends Client{

    private OnComunityEventListener onComunityEventListener;
    public void setOnComunityEventListener(OnComunityEventListener onComunityEventListener) {
        this.onComunityEventListener = onComunityEventListener;
    }

    private Variable var;
    final public String SERVER_IP = "192.168.4.1"; //192.168.4.1
    final public int SERVER_PORT = 80; //80


    private ComunityListener listener;
    public void setListener(ComunityListener listener) {
        this.listener = listener;
    }

    public Comunity(Variable var){
        super(var);
        this.var = var;
        this.setListener(this::onMessangeEvent);
    }

    private void send(String command,String value){
        String string = String.format("%s%s%s%s%s",USERNAME,":",command,"=",value);
        sendToServer(string);
        var.extension.printDebug("Comunity",string);
    }

    public void setInputPH(float value){
        send("INPUT_PH",String.valueOf(value));
    }

    public void setInputPH_STOP(){
        send("INPUT_PH","stop");
    }

    public void setTimeAutoWork(String value){
        send("SET_TIME_AUTO_WORK",String.valueOf(value));
    }

    public void setTimeBoard(String value){
        send("TIME_BOARD",String.valueOf(value));
    }

    public void setToggleRelay(int index){
        if (index < 0 || index > 6)return;
        send("RELAY",String.valueOf(index));
    }

    public void setCMVM(String value){sendToServer("SET:CMVM=" + value);}

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

            var.cmvmObject.calibration = Float.valueOf(d1[3]);
            var.cmvmObject.m = Float.valueOf(d1[4]);
            var.cmvmObject.voltin = Float.valueOf(d1[5]);
            var.cmvmObject.max_analog = Float.valueOf(d1[6]);
            var.cmvmObject.analogAvg = Float.valueOf(d1[7]);

            var.step = Integer.valueOf(d1[8]);
            var.work_status = (d1[9].equals("0")) ? false : true;

            var.relay_status[0] = (d1[10].equals("0")) ? false : true;
            var.relay_status[1] = (d1[11].equals("0")) ? false : true;
            var.relay_status[2] = (d1[12].equals("0")) ? false : true;
            var.relay_status[3] = (d1[13].equals("0")) ? false : true;
            var.relay_status[4] = (d1[14].equals("0")) ? false : true;
            var.relay_status[5] = (d1[15].equals("0")) ? false : true;

        }

        if(command.equals("OUTPUT")){
            var.outout_text = value;
            var.extension.printDebug("Comunity","OUTPUT");
        }
    }

    private void dataResponse(String header,String command,String value){

        if(command.equals("GET_TIME_AUTO_WORK_RES")){
            var.extension.printDebug("Comunity","Recv TAW");
        }

        if(command.equals("INPUT_RES")){
            var.inputPH = Float.parseFloat(value);
        }

    }




}