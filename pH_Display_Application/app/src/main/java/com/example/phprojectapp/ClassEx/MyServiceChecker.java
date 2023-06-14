package com.example.phprojectapp.ClassEx;

import android.app.ActivityManager;
import android.content.Context;

import java.util.List;

public class MyServiceChecker {

    public static boolean isServiceRunning(Context context, Class<?> serviceClass) {
        ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);

        if (activityManager != null) {
            // ดึงรายการบริการทั้งหมดที่กำลังทำงาน
            List<ActivityManager.RunningServiceInfo> runningServices = activityManager.getRunningServices(Integer.MAX_VALUE);

            if (runningServices != null) {
                // เช็คว่าบริการ serviceClass กำลังทำงานหรือไม่
                for (ActivityManager.RunningServiceInfo service : runningServices) {
                    if (serviceClass.getName().equals(service.service.getClassName())) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

}
