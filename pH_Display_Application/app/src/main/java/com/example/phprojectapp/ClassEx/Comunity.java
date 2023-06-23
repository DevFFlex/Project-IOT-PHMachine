package com.example.phprojectapp.ClassEx;


public class Comunity extends Client{

    private OnComunityEventListener onComunityEventListener;
    public void setOnComunityEventListener(OnComunityEventListener onComunityEventListener) {
        this.onComunityEventListener = onComunityEventListener;
    }

    private Variable var;
    final public String SERVER_IP = "192.168.4.1"; //192.168.4.1
    final public int SERVER_PORT = 80; //80
    final public String[] COMMANS = {
            "INPUT_PH",
            "MIXTANK_PH",
            "USETANK_PH",
            "TIME_AUTO_WORK",
            "RTC_TIME",
            "OUTPUT",
            "TOGGLE_RELAY",
            "FILE_DIR",
            "CMVM"
    };

    final public String H_GET = "GET";
    final public String H_SET = "SET";
    final public String H_MESSAGE = "MESSAGE";

    final public String SYMBOL_L1 = ":";
    final public String SYMBOL_L2 = "=";


    private ComunityListener listener;
    public void setListener(ComunityListener listener) {
        this.listener = listener;
    }

    public Comunity(Variable var){
        super(var);
        this.var = var;
        this.setListener(this::onMessangeEvent);
    }

    private void send(String h_sender,String command,String value){
        String string = String.format("%s%s%s%s%s",h_sender,SYMBOL_L1,command,SYMBOL_L2,value);
        sendToServer(string);
        var.extension.printDebug("Comunity",string);
    }

    public void getInputPH(){

    }
    public void setInputPH(float value){
        send(H_SET,COMMANS[0],String.valueOf(value));
    }
    public void setInputPH_STOP(){
        send(H_SET,COMMANS[0],"stop");
    }

    public void setTimeAutoWork(String value){
        send(H_SET,COMMANS[3],String.valueOf(value));
    }

    public void setTimeBoard(String value){
        send(H_SET,COMMANS[4],String.valueOf(value));
    }

    public void setToggleRelay(int index){
        if (index < 0 || index > 6)return;
        send(H_SET,COMMANS[6],String.valueOf(index));
    }

    public void setCMVM(String value){sendToServer("SET:CMVM=" + value);}

    public void getTimeAutoWork(){
        sendToServer("GET:TIME_AUTO_WORK=NULL");
    }

    public void getSDDir(String path){
        sendToServer("GET:FILE_DIR=" + path);
    }

    public void getCMVM(){
        sendToServer("GET:CMVM=NULL");
    }

    public void sendMessageChat(String name,String data){
        sendToServer("MESSAGE:" + name + "=" + data);
        System.out.println("on comunity : data = " + data);
    }

    public String timeBoardObjectToQueryString(TimeBoardObject timeBoardObject){
        String out = "";
        out += String.valueOf(timeBoardObject.getHour()) + ",";
        out += String.valueOf(timeBoardObject.getMinute()) + ",";
        out += String.valueOf(timeBoardObject.getSecond()) + ",";
        out += String.valueOf(timeBoardObject.getDayofweek()) + ",";
        out += String.valueOf(timeBoardObject.getDayofmonth()) + ",";
        out += String.valueOf(timeBoardObject.getMonth()) + ",";
        out += String.valueOf(timeBoardObject.getYear() % 2000);
        return out;
    }

    private void H_GetSetProcess(String header,int index,String value){
        String head_sender = (header.equals(H_GET)) ? H_SET : H_GET;
        String format = head_sender + SYMBOL_L1 + COMMANS[index] + SYMBOL_L2;
        switch (index){


            case 0:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){
                    if(value.contains("FINISH")){
                        var.working_ph = false;
                    }else{
                        try{
                            var.inputPH = Float.parseFloat(value);
                        }catch (Exception e){}
                    }
                }
                break;





            case 1:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){
                    try{
                        var.mixtankPH = Float.parseFloat(value);
                    }catch (Exception e){ }
                }
                break;





            case 2:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){
                    try{
                        var.usetankPH = Float.parseFloat(value);
                    }catch (Exception e){ }
                }
                break;





            case 3:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){
                    String[] timeLayer1 = value.split("#");

                    int i = 0;
                    var.timeObjectList.clearAllItem();
                    for (String element:timeLayer1) {
                        String val[] = element.split(",");

                        int hour = Integer.valueOf(val[0]);
                        int minute = Integer.valueOf(val[1]);
                        boolean status = Boolean.valueOf(val[2]);
                        float ph = Float.valueOf(val[3]);
                        boolean delete_status = Boolean.valueOf(val[4]);

                        var.extension.printDebug("Comunity",String.valueOf(i) + " |" + String.valueOf(hour) +":" + String.valueOf(minute) + "\t" + String.valueOf(status) + "\t" + String.valueOf(ph) + "\t" + String.valueOf(delete_status));
                        i++;

                        if(delete_status)continue;
                        var.timeObjectList.addItem(hour,minute,status,ph);
                    }
                    System.out.println(value);
                }
                break;





            case 4:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){

                }
                break;



            case 5:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){
                    var.outout_text = value;
                }
                break;



            case 6:
                if(header.equals(H_GET)){

                }else if(header.equals(H_SET)){

                }
                break;


            case 7:
                if(header.equals(H_GET)){}
                else if(header.equals(H_SET)){
                    var.extension.printDebug("Comunity",value);
                    var.outout_text = value;

                    var.file_list.clear();
                    String[] data = value.split(",");
                    for (int i = 0;i<data.length;i++) {
                        String []data2 = data[i].split("\\|");
                        var.extension.printDebug("Comunity","data[i] = " + data[i]);
                        var.extension.printDebug("Comunity","data[0] = " + data2[0] + "\t\tdata[1] = " + data2[1]);

                        var.file_list.add(data2);
                    }
                }
                break;

            case 8:
                if(header.equals(H_SET)){


                    String[] data = value.split(",");
//                    var.cmvmObject.calibration = Float.parseFloat(data[0]);
//                    var.cmvmObject.m = Float.parseFloat(data[1]);
//                    var.cmvmObject.voltin = Float.parseFloat(data[2]);
//                    var.cmvmObject.max_analog = Float.parseFloat(data[3]);
//                    var.cmvmObject.analogAvg = Float.parseFloat(data[4]);


                    if(onComunityEventListener != null){
                        onComunityEventListener.onCMVM(Float.parseFloat(data[0]),Float.parseFloat(data[1]),Float.parseFloat(data[2]),Float.parseFloat(data[3]),Float.parseFloat(data[4]));
                    }

                }
                break;

        }
    }

    private void H_MessageProcess(String command,String value){
        if(!command.equals(var.comunity.USERNAME))var.outout_text = command + " | " + value;

        String []messageObj = {
                command
                ,value
        };
        var.chat_his.add(messageObj);
        if(var.chat_his.size() > 8)var.chat_his.remove(0);
    }


    private void onMessangeEvent(String header,String command,String value){

        if(header.equals(H_GET) || header.equals(H_SET)){
            for(int i = 0;i<COMMANS.length;i++){

                if(command.equals(COMMANS[i]))H_GetSetProcess(header,i,value);
            }
        }


        if(header.equals(H_MESSAGE))H_MessageProcess(command,value);
    }

}