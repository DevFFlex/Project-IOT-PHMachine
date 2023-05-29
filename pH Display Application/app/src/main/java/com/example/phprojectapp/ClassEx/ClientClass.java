package com.example.phprojectapp.ClassEx;

import com.example.phprojectapp.MainPage;

import java.io.BufferedReader;
import java.io.Console;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.util.Random;


public class ClientClass{

    private ClientOnEventListener listener;
    public void setClientOnEventListener(ClientOnEventListener listener) {
        this.listener = listener;
    }

    private PrintWriter output;
    private BufferedReader input;

    private Socket socket;
    public Socket getSocket() {
        return socket;
    }

    private Random random_obj;

    private boolean isConnect = false;
    public boolean getConnected(){
        return this.isConnect;
    }
    public void setConnect(boolean connect) {
        isConnect = connect;
    }


    public ClientClass(){
        random_obj = new Random();
    }


    public void connect(String SERVER_IP,int SERVER_PORT){
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    socket = new Socket(SERVER_IP, SERVER_PORT);
                    output = new PrintWriter(socket.getOutputStream());
                    input = new BufferedReader(new InputStreamReader(socket.getInputStream()));



                    String[] namelist = {
                            "Name1",
                            "Money",
                            "Day",
                            "Account",
                            "Data",
                            "AI",
                            "Socket",
                            "Flow"
                    };

                    sendToServer(namelist[random_obj.nextInt(namelist.length)]);

                    isConnect = true;

                    if(isConnect){

                        recv();
//                        recv_ph_mixtank();
                    }

                    System.out.println("isConnected ggggg");

                } catch (IOException e) { e.printStackTrace(); }
            }
        }).start();
    }

    private void recv(){

        new Thread(new Runnable() {

            public String message = "";
            public char c = ' ';

            private char endKeyword = '$';
            private int count = 0;
            @Override
            public void run() {

                while (isConnect) {


                    try {
                        char c = (char) input.read();
//                        System.out.println("count = " + String.valueOf(count++) + "\tsocket = " + String.valueOf(c));

                        if (c == endKeyword){
                            if (message != "")listener.onMessage(message);
                            message = "";
                        }else{
                            message += String.valueOf(c);
                        }



                        Thread.sleep(10);
                    } catch (IOException | InterruptedException e) {
                        e.printStackTrace();
                    }
                }


            }
        }).start();
    }


    private void recv_ph_mixtank(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true){

                    sendToServer("C_GET_MTPH=GET");

                    try {
                        Thread.sleep(200);
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
                output.write(message);
                output.flush();
            }
        }).start();
    }



}
