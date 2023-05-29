package com.example.phprojectapp.ClassEx;

import java.io.IOException;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;

public class Comunity implements ClientOnEventListener {
    private ClientClass clientClass;
    private Variable variable;

    public Comunity(ClientClass clientClass,Variable variable){
        this.clientClass = clientClass;
        this.variable = variable;
    }

    public void getInputPH(){
        if(clientClass.getConnected())
            clientClass.sendToServer("GET:INPUT_PH");
    }

    public void getMixTankPH(){
        if(clientClass.getConnected())
            clientClass.sendToServer("GET:MIXTANK_PH");
    }

    public void getUseTankPH(){
        if(clientClass.getConnected())
            clientClass.sendToServer("GET:USETANK_PH");
    }

    public void setInputPH(float value){
        if(clientClass.getConnected())
            clientClass.sendToServer("SET:INPUT_PH=" + String.valueOf(value));
    }


    public void setTimeBoard(String queryString){
        if(clientClass.getConnected())
            clientClass.sendToServer("SET:TIME_BOARD=" + queryString);
    }

    public void setTimeList(String value_str){
        if(clientClass.getConnected())
            clientClass.sendToServer("SET:TIME_LIST=" + value_str);
    }

    public void getTimeList(){
        if(clientClass.getConnected())
            clientClass.sendToServer("GET:TIME_LIST");
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


    @Override
    public void onMessage(String message) {
        if (!message.contains(":"))return;

        String[] data = message.split(":");
        String header = data[0].trim();
        String commands = data[1];

        if(header.equals("RESP")){

            if (commands.equals("S_KICK")){
                try {
                    clientClass.getSocket().close();
                    clientClass.setConnect(false);
                } catch (IOException e) { }
            }

        }else if(header.equals("SET")){
            if(!commands.contains("="))return;

            String data2[] = commands.split("=");
            String command = data2[0].trim();
            String value = data2[1].trim();

            String output = "message = " + message + "\n";
            output += "command = " + command + "\n";
            output += "value = " + value + "\n";

//            System.out.println(output);

            if (command.equals("OUTPUT")){
                variable.setOutout_text(value);
            }

            if (command.equals("INPUT_PH")){
                try{
                    variable.setInputPH(Float.parseFloat(value));
                }catch (Exception e){}
            }

            if (command.equals("MIXTANK_PH")){
                try{
                    variable.setMixtankPH(Float.parseFloat(value));
                }catch (Exception e){ }
            }

            if (command.equals("USETANK_PH")){
                try{
                    variable.setUsetankPH(Float.parseFloat(value));
                }catch (Exception e){ }
            }

            if (command.equals("TIME_LIST")){

                String[] timeLayer1 = value.split("#");

                int i = 0;
                variable.getTimeObjectList().clearAllItem();
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
                    variable.getTimeObjectList().addItem(hour,minute,second,status,ph);
                }
                System.out.println(value);
//                System.out.println(timeLayer1[1]);
//                System.out.println(timeLayer1.length);
            }


            if (command.equals("TIME_BOARD")){

            }

        }







    }
}


//class A{
//    private B b;
//
//    A() {
//        // คอนสตรักเตอร์ A ที่ไม่รับพารามิเตอร์
//    }
//
//    A(B b){
//        this.b = b;
//    }
//}
//
//
//class B extends A{
//    B(){
//        super(this);
//    }
//}
