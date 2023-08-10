package com.example.phprojectapp.ClassEx;

import com.example.phprojectapp.Variable.Variable;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


interface ClientEventListener{
    void onMessage(String header,String command,String value);
}

public class Client{
    private Variable var;
    private PrintWriter output;
    private BufferedReader input;

    private ClientEventListener listener;
    public void setListener(ClientEventListener listener) {
        this.listener = listener;
    }

    private Socket socket;
    public String DEFAULT_IP = "192.168.4.1";
    public int DEFAULT_PORT = 80;
    public String DEFAULT_USERNAME = "PEOPLE";

    private boolean thread_loop = false;
    private boolean activeDisconnect = false;
    public boolean isConnect = false;


    public Client(Variable v){
        this.var = v;

        TimerMillis timerReconnect = new TimerMillis(1000, new TimerMillisInterface() {
            @Override
            public void isExpired() {
                if(!isConnect)reconnect();
            }
        });
    }
    public void connect(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                String USE_IP = var.preferences.getString("ip",DEFAULT_IP);
                int USE_PORT = var.preferences.getInt("port",DEFAULT_PORT);
                String USE_USERNAME = var.preferences.getString("username",DEFAULT_USERNAME);

                try {
                    socket = new Socket(USE_IP, USE_PORT);

                    output = new PrintWriter(socket.getOutputStream());
                    input = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                    sendToServer("SET:CLIENT_NAME=" + USE_USERNAME);
                    isConnect = true;

                    if(isConnect){
                        thread_loop = true;
                        recv();
                        var.outout_text = "เชื่อมต่อบอร์ดสำเร็จ";
                        return;
                    }

                    var.extension.printDebug("Client","isConnect = True");



                } catch (IOException e) {
                    var.extension.printDebug("Client","Socket Connect Fail");
                }

                var.outout_text = "เชื่อมต่อบอร์ดไม่สำเร็จ";
            }
        }).start();
    }


    private void recv(){

        new Thread(new Runnable() {

            public String message = "";
            public char c = ' ';

            private char endKeyword = '$';
            @Override
            public void run() {
                while (thread_loop) {
                    if (isConnect){
                        try {
                            char c = (char) input.read();
                            if (c == endKeyword){
                                if (message != ""){
                                    if(!message.contains(":"))return;
                                    String[] data = message.split(":");
                                    String header = data[0].trim();
                                    String commands = data[1];

                                    if(!commands.contains("="))return;
                                    String[] data2 = commands.split("=");
                                    String command = data2[0];
                                    String value = data2[1];

                                    var.extension.printDebug("CCC",value);
                                    if(listener != null)listener.onMessage(header,command,value);
                                }
                                message = "";
                            }else message += String.valueOf(c);

                            Thread.sleep(10);
                        } catch (IOException | InterruptedException e) {
                            isConnect = false;
                            var.extension.printError("Client","recv message,fail connected");
                        }
                    }
                }


            }
        }).start();
    }

    public void sendToServer(String textSend){
        new Thread(new Runnable() {

            private String message = textSend + "$";

            @Override
            public void run() {
                if(isConnect){
                    try{
                        output.write(message);
                        output.flush();
//                        var.extension.printDebug("Client","Send ---- " + message);
                    }catch (Exception e){
                        isConnect = false;
                        var.extension.printError("Client","send message,fail connected");
                    }
                }
            }
        }).start();
    }

    public void disconnect() {
        if(activeDisconnect){
            var.extension.printDebug("Client","activeDisconnect");
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                activeDisconnect = true;
                try {
                    thread_loop = false;
                    Thread.sleep(2000);
                    if(socket != null)socket.close();
                } catch (IOException e) {
                    throw new RuntimeException(e);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                socket = null;
                isConnect = false;
                activeDisconnect = false;
            }
        }).start();
    }

    public void reconnect(){
//        disconnect();
        connect();
    }

}