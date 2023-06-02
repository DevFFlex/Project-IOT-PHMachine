package com.example.phprojectapp;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;

import androidx.core.app.NotificationCompat;

import com.example.phprojectapp.R;

public class MyService extends Service {
    private static final String CHANNEL_ID = "MyForegroundServiceChannel";
    private static final int NOTIFICATION_ID = 1;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        // สร้าง Notification
        Notification notification = createNotification();

        // เรียกใช้ startForeground()
        startForeground(NOTIFICATION_ID, notification);

        // โค้ดอื่น ๆ ที่ต้องการในการเริ่มต้นบริการ
        new Thread(new Runnable() {
            int count = 0;
            @Override
            public void run() {
                while (true){
                    System.out.println("Hello world + " + String.valueOf(count++));
                }
            }
        }).start();

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        System.out.println("onDestoy");
        // ยกเลิกโหมด foreground
        stopForeground(true);

        // หยุดการทำงานของบริการ
        stopSelf();
    }

    private Notification createNotification() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // สร้าง Notification Channel (สำหรับ Android Oreo ขึ้นไป)
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, "My Foreground Service", NotificationManager.IMPORTANCE_DEFAULT);
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }

        // สร้าง Notification
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setContentTitle("My Foreground Service")
                .setContentText("Service is running...")
                .setSmallIcon(R.mipmap.ic_launcher_ph_meter);

        return builder.build();
    }
}
