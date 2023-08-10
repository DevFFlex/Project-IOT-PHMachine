package com.example.phprojectapp.ClassEx;

public class TimerMillis {
    long startTime = 0;
    long currentTime = 0;
    double delay;
    boolean run = true;

    public TimerMillis(double delay, TimerMillisInterface callback){
        this.delay = delay;

        new Thread(new Runnable() {
            @Override
            public void run() {
                startTime = System.currentTimeMillis();
                while (run){
                    try {
                        if(currentTime - startTime > delay){
                            startTime = currentTime;
                            callback.isExpired();
                        }

                        Thread.sleep(10);
                        currentTime = System.currentTimeMillis();
//                        System.out.println("current Time = " + String.valueOf(currentTime));
                    }catch (Exception e){

                    }
                }
            }
        }).start();
    }


    public void reset(){
        startTime = System.currentTimeMillis();
    }

    public void stop(){
        this.run = false;
    }
}
