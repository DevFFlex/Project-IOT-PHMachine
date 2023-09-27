package com.example.phprojectapp.ClassEx;


import com.example.phprojectapp.Variable.TimeBoardObject;
import com.example.phprojectapp.Variable.Variable;


public class Comunity extends Client{

    private OnComunityEventListener onComunityEventListener;
    public void setOnComunityEventListener(OnComunityEventListener onComunityEventListener) {
        this.onComunityEventListener = onComunityEventListener;
    }

    private Variable var;

    public Comunity(Variable var){
        super(var);
        this.var = var;

        this.setListener(this::onMessangeEvent);

    }
    public void serverSendSerial(String value){
        String string = String.format("%s%s%s%s%s",DEFAULT_USERNAME,":","SERIAL","=",value);
        sendToServer(string);
        var.extension.printDebug("Comunity",value);
    }
    public void serverSendMessageChat(String name,String data){
        sendToServer("MESSAGE:" + name + "=" + data);
    }



    public void serverStartAdjustPH(float pH,int T){
        serverSendSerial(String.format("system startAdjust %.2f %d",pH,T));
    }

    public void serverStopAdjustPH(){
        serverSendSerial("system stopAdjust");
    }

    public void serverSetRelay(String header,int index,double time){
        if (index < 0 || index > 6)return;
        serverSendSerial(String.format("relay toggle %d",index));
    }

    public void serverSetWorkTimer(int index,int hour,int minute,float pH,int T,boolean active_status,boolean delete_status){
        serverSendSerial(String.format("var setWorkTime %d %d %d %.2f %d %d %d",index,hour,minute,pH,T,(active_status) ? 1 : 0,(delete_status) ? 1 : 0));
    }

    public void serverSetTimeOfBoard(String value){

    }

    public void serverSetInternetSSID(String ssid){
        serverSendSerial("internet setSSID " + ssid);
    }

    public void serverSetInternetPASS(String pass){
        serverSendSerial("internet setPASS " + pass);
    }

    public void serverSetVarWaitSTRPump(String value){
        serverSendSerial("var setWAIT_STR_PUMP " + value);
    }

    public void serverSetVarWaitPHStabilize(String value){
        serverSendSerial("var setWAIT_PH_STABILIZE " + value);
    }

    public void serverSetVarWaitBaseUseTime(String value){
        serverSendSerial("var setWAIT_BASEUSETIME " + value);
    }

    public void serverSetVarWaitAcidUseTime(String value){
        serverSendSerial("var setWAIT_ACIDUSETIME " + value);
    }

    public void serverSetVarLimiteUseBase(String value){
        serverSendSerial("var setLIMITE_USE_BASE " + value);
    }

    public void serverSetVarLimiteUseAcid(String value){
        serverSendSerial("var setLIMITE_USE_ACID " + value);
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
    }

    private void dataAuto(String header,String command,String value){
        if(command.equals("UPDATE")){
            String d1[] = value.split(",");

            var.mixtankPH = Float.valueOf(d1[0]);
            if(!d1[1].equals("nan"))var.tempC = Float.valueOf(d1[1]);
            if(!d1[2].equals("nan"))var.humidity = Float.valueOf(d1[2]);

            var.relay_status[0] = (d1[3].equals("0")) ? false : true;
            var.relay_status[1] = (d1[4].equals("0")) ? false : true;
            var.relay_status[2] = (d1[5].equals("0")) ? false : true;
            var.relay_status[3] = (d1[6].equals("0")) ? false : true;
            var.relay_status[4] = (d1[7].equals("0")) ? false : true;
            var.relay_status[5] = (d1[8].equals("0")) ? false : true;

            var.timeBoardObject.hour = Integer.parseInt(d1[9]);
            var.timeBoardObject.minute = Integer.parseInt(d1[10]);
            var.timeBoardObject.second = Integer.parseInt(d1[11]);

            var.fsw[0] = Integer.parseInt(d1[12]);
            var.fsw[1] = Integer.parseInt(d1[13]);

            var.internet_connected = (d1[14].equals("0")) ? false : true;

            String[] timer_layer0 = d1[15].split("\\|");
            for(int i = 0;i<4;i++){
                String[] item = timer_layer0[i].split("SP");
                var.workTimerList.workTimers_item.get(i).HOUR = Integer.parseInt(item[0]);
                var.workTimerList.workTimers_item.get(i).MINUTE = Integer.parseInt(item[1]);
                var.workTimerList.workTimers_item.get(i).PH = Float.parseFloat(item[2]);
                var.workTimerList.workTimers_item.get(i).T = Integer.parseInt(item[3]);
                var.workTimerList.workTimers_item.get(i).ACTIVE_STATUS = Boolean.parseBoolean(item[4]);
                var.workTimerList.workTimers_item.get(i).DELETE_STATUS = Boolean.parseBoolean(item[5]);
            }

            var.step = Integer.valueOf(d1[16]);
            var.work_status = (d1[17].equals("0")) ? false : true;
            var.pH_space_rate = Float.valueOf(d1[18]);
            var.adjustCurrentPH = Float.valueOf(d1[19]);
            var.wait_stirringPump = Integer.valueOf(d1[20]);
            var.wait_pHStabilize = Integer.valueOf(d1[21]);
            var.acidUseTime = Integer.valueOf(d1[22]);
            var.baseUseTime = Integer.valueOf(d1[23]);
            var.limite_use_base = Integer.valueOf(d1[24]);
            var.limite_use_acid = Integer.valueOf(d1[25]);
            var.adjustT_Counter = Integer.valueOf(d1[26]);
            var.addBaseCount = Integer.valueOf(d1[27]);
            var.addAcidCount = Integer.valueOf(d1[28]);

        }

        if(command.equals("OUTPUT")){
            var.outout_text = value;
            var.extension.printDebug("Comunity","OUTPUT");
        }

        if(command.equals("SETSTEPTEXT")){
            var.stepText = value;
        }
    }





}