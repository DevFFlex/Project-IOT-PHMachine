package com.example.phprojectapp;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.View;

import java.util.Random;

class AttrOption{

    private float position_x,position_y;
    private float size_x,size_y;
    private float speed_x,speed_y;

    public AttrOption(){

    }

    public float getPosition_x() {
        return position_x;
    }

    public float getPosition_y() {
        return position_y;
    }
}

public class PH_Canvas extends View {

    private Paint paint;
    private int x = 500, y = 500, dx = 1, dy = 1;
    private int speed_x = 5,speed_y = 5;
    private static final int RADIUS = 50;

    private AttrOption ball1;

    Random randomObject;

    public PH_Canvas(Context context, AttributeSet attrs) {
        super(context, attrs);
        randomObject = new Random();

        paint = new Paint();
        paint.setColor(Color.RED);
        paint.setStyle(Paint.Style.FILL);

        final Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                update();
                invalidate();
                handler.postDelayed(this, 1);
            }
        });
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        canvas.drawCircle(x, y, RADIUS, paint);
    }

    private void update() {
        x += speed_x * dx;
        y += speed_y * dy;

        if (x < 0 + RADIUS|| x > getWidth() - RADIUS - speed_x) {
            dx = -dx;
            speed_x = randomObject.nextInt(10);
//            speed_y = randomObject.nextInt(10);

        }
        if (y < 0 + RADIUS || y > getHeight() - RADIUS - speed_y) {
            dy = -dy;
//            speed_x = randomObject.nextInt(10);
            speed_y = randomObject.nextInt(10);
        }

        if (speed_x < 5)speed_x = 5;
        if (speed_y < 5)speed_y = 5;
    }


}
