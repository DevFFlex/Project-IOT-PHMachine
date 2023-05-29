package com.example.phprojectapp.ClassEx;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.util.DisplayMetrics;
import android.widget.Toast;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import com.example.phprojectapp.MainPage;
import com.example.phprojectapp.R;

import java.util.ArrayList;
import java.util.List;

public class Extension{

    Context context;

    public Extension(Context context){
        this.context = context;
    }

    public List<Integer> get_mobileResolusion(MainPage mainPage){
        List<Integer> dataOut = new ArrayList<Integer>();

        DisplayMetrics displayMetrics = new DisplayMetrics();
        mainPage.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int width = displayMetrics.widthPixels;
        int height = displayMetrics.heightPixels;

        dataOut.add(width);
        dataOut.add(height);

        return dataOut;
    }


    public void printAlert(String stringIn){
        Toast.makeText(context,stringIn,Toast.LENGTH_SHORT).show();
    }

    public void openWebLink(String url){
        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
        context.startActivity(intent);
    }

    public void print_notify(String header,String description){

        final String CHANNEL_ID = "my_channel_id";
        NotificationCompat.Builder notify_builder = new NotificationCompat.Builder(context, CHANNEL_ID)
                .setSmallIcon(R.drawable.icon_png1)
                .setContentTitle(header)
                .setContentText(description)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT);

        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
        notificationManager.notify(1, notify_builder.build());
    }


    public String getWifiIpAddress(Context context) {
        WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();
        int ipAddress = wifiInfo.getIpAddress();

        // แปลง IP Address เป็นรูปแบบ String
        String ipString = String.format(
                "%d.%d.%d.%d",
                (ipAddress & 0xff),
                (ipAddress >> 8 & 0xff),
                (ipAddress >> 16 & 0xff),
                (ipAddress >> 24 & 0xff)
        );

        return ipString;
    }

}

