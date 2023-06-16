package com.example.phprojectapp.ClassEx;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Random;


interface ClientEventListener{
    void onMessage(String header,String command,String value);
}

public class Client{
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

    public Client(){

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

                    System.out.println("isConnected ggggg");

                } catch (IOException e) { e.printStackTrace(); }
            }
        }).start();
    }

    public void disconnect() throws IOException {
        socket.close();
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
                                if (message != ""){
                                    if (!message.contains(":"))return;

                                    String[] data = message.split(":");
                                    String header = data[0].trim();
                                    String commands = data[1];

                                    if(!commands.contains("="))return;
                                    String[] data2 = commands.split("=");
                                    String command = data2[0];
                                    String value = data[1];

                                    listener.onMessage(header,command,value);
                                }
                                message = "";
                            }else{
                                message += String.valueOf(c);
                            }
                            Thread.sleep(10);
                        } catch (IOException | InterruptedException e) {
//                            isConnect = false;
                            e.printStackTrace();
                            System.out.println("recv error----------------------------");
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
                        System.out.println("send error----------------------------");
                    }
                }
            }
        }).start();
    }



}
