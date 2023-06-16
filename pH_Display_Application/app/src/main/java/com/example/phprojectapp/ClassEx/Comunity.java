package com.example.phprojectapp.ClassEx;

import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;


public class Comunity extends Client{
    private Variable variable;
    final public String SERVER_IP = "192.168.4.1"; //192.168.4.1
    final public int SERVER_PORT = 80; //80


    private ComunityListener listener;
    public void setListener(ComunityListener listener) {
        this.listener = listener;
    }

    public Comunity(Variable variable){
        this.variable = variable;
        this.setListener(this::onMessangeEvent);
    }

    public void getInputPH(){
        sendToServer("GET:INPUT_PH");
    }
    public void setInputPH(float value){
        sendToServer("SET:INPUT_PH=" + String.valueOf(value));
    }

    public void getMixTankPH(){
        sendToServer("GET:MIXTANK_PH");
    }
    public void getUseTankPH(){
        sendToServer("GET:USETANK_PH");
    }

    public void setTimeBoard(String queryString){
        sendToServer("SET:TIME_BOARD=" + queryString);
    }


    public void setToggleRelay(int index){
        if (index < 0 || index > 6)return;
        sendToServer("SET:TOGGLE_RELAY=" + String.valueOf(index));
    }

    public void setTimeAutoWork(String value_str){
        sendToServer("SET:TIME_AUTO_WORK=" + value_str);
    }
    public void getTimeAutoWork(){
        sendToServer("GET:TIME_AUTO_WORK=NULL");
    }

    public void sendMessageChat(String name,String data){
        sendToServer("MESSAGE:" + name + "=" + data);
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


    private void onMessangeEvent(String header,String command,String value){

        if(header.equals("SET")){

            if (command.equals("OUTPUT")){
                variable.outout_text = value;
            }

            if (command.equals("INPUT_PH")){
                try{
                    variable.inputPH = Float.parseFloat(value);
                }catch (Exception e){}
            }

            if (command.equals("MIXTANK_PH")){
                try{
                    variable.mixtankPH = Float.parseFloat(value);
                }catch (Exception e){ }
            }

            if (command.equals("USETANK_PH")){
                try{
                    variable.usetankPH = Float.parseFloat(value);
                }catch (Exception e){ }
            }

            if (command.equals("TIME_AUTO_WORK")){

                String[] timeLayer1 = value.split("#");

                int i = 0;
                variable.timeObjectList.clearAllItem();
                for (String element:timeLayer1) {
                    String val[] = element.split(",");

                    int hour = Integer.valueOf(val[0]);
                    int minute = Integer.valueOf(val[1]);
                    int second = Integer.valueOf(val[2]);
                    boolean status = Boolean.valueOf(val[3]);
                    float ph = Float.valueOf(val[4]);
                    boolean delete_status = Boolean.valueOf(val[5]);

                    System.out.println(String.valueOf(i) + " |" + String.valueOf(hour) +":" + String.valueOf(minute) +":" + String.valueOf(second));
                    i++;

                    if(delete_status)continue;
                    variable.timeObjectList.addItem(hour,minute,second,status,ph);
                }
                System.out.println(value);
            }


        }else if(header.equals("MESSAGE")){
            listener.onMessageFromUser(command,value);
        }
    }

}