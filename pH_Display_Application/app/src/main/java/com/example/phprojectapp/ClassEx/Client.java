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
    public String USERNAME = "PEOPLE";

    private boolean isConnect = false;
    public boolean getConnected(){
        return this.isConnect;
    }

    public Client(Variable v){
        this.var = v;
    }
    public void connect(String SERVER_IP,int SERVER_PORT,String name){
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    socket = new Socket(SERVER_IP, SERVER_PORT);
                    output = new PrintWriter(socket.getOutputStream());
                    input = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                    sendToServer("SET:CLIENT_NAME=" + name);
                    USERNAME = name;
                    isConnect = true;

                    if(isConnect){
//                        checkServer();
                        recv();
                    }

                    var.extension.printDebug("Client","isConnect = True");



                } catch (IOException e) {
                    var.extension.printError("Client","Socket Connect Fail");
                }
            }
        }).start();
    }

    public void disconnect() {
        try {
            socket.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        socket = null;
        isConnect = false;
    }

    private void recv(){

        new Thread(new Runnable() {

            public String message = "";
            public char c = ' ';

            private char endKeyword = '$';
            @Override
            public void run() {

                while (true) {

                    if (isConnect){
                        try {
                            char c = (char) input.read();

                            if (c == endKeyword){
                                var.extension.printDebug("Client","Message = " + message);
                                if (message != ""){
                                    if (!message.contains(":") || !message.contains("="))return;

                                    String[] data = message.split(":");
                                    String header = data[0].trim();
                                    String commands = data[1];

                                    if(!commands.contains("="))return;
                                    String[] data2 = commands.split("=");
                                    String command = data2[0];
                                    String value = data[1];

                                    if(value.contains("="))value = value.split("=")[1];

                                    listener.onMessage(header,command,value);
                                }
                                message = "";
                            }else{
                                message += String.valueOf(c);
                            }
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

    private void checkServer(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true){

                    sendToServer("CHECK:SERVER=NULL");

                    try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
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
                    }catch (Exception e){
                        isConnect = false;
                        var.extension.printError("Client","send message,fail connected");
                    }
                }
            }
        }).start();
    }

}