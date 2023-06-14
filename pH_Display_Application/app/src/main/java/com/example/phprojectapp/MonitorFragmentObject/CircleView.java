package com.example.phprojectapp.MonitorFragmentObject;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Handler;
import android.util.AttributeSet;
import android.view.View;

import androidx.core.content.ContextCompat;

import com.example.phprojectapp.R;

import java.util.ArrayList;
import java.util.List;

class CirclePHBall{ //------------------------------------------------------------------

    int[] colors = {
            Color.rgb(255, 0, 0), //0
            Color.rgb(255, 47, 0), // 1
            Color.rgb(255, 68, 0), // 2
            Color.rgb(255, 94, 0), // 3
            Color.rgb(255, 123, 0), // 4
            Color.rgb(255, 166, 0), // 5
            Color.rgb(255, 213, 0), // 6
            Color.rgb(204, 255, 0), // 7
            Color.rgb(115, 255, 0), // 8
            Color.rgb(0, 255, 195), // 9
            Color.rgb(0, 238, 255), // 10
            Color.rgb(0, 30, 255), // 11
            Color.rgb(68, 0, 255), // 12
            Color.rgb(106, 0, 255), // 13
            Color.rgb(153, 0, 255) // 14

    };

    private View view;

    public float getX() {
        return x;
    }
    public float getY() {
        return y;
    }

    private float speedX = 1;
    private float speedY = 1;
    private float startx;
    private float starty;
    private float x;
    private float y;
    public float radius_default = 50;
    private float radiusUse = radius_default;
    private float radiesNeed = radius_default;

    private int currentPH;
    CirclePHBall(View view,float x,float y,float startx,float starty,int currentPH){
        this.view = view;
        this.x = x;
        this.y = y;
        this.currentPH = currentPH;
        this.startx = (startx >= 0 ) ? startx : x;
        this.starty = (starty >= 0) ? starty : y;
    }

    public void show(Canvas canvas){
        bodyDisplay(canvas,currentPH,x,y,radiusUse,colors[currentPH]);
    }

    public void bigFocus(){
        radiesNeed = 100;
    }

    public void bigDefault(){
        radiesNeed = radius_default;
    }

    public void update(){
        //size controll
        if (radiusUse > radiesNeed)radiusUse--;
        if (radiusUse < radiesNeed)radiusUse++;

        //movement controll
        if (x > startx)x-=speedX;
        else if (x < startx)x+=speedX;

        if (y > starty)y-=speedY;
        else if(y < starty)y+=speedY;

        //movement speed constroll
        speedX = Math.abs(x - startx) / 2;
        speedY = Math.abs(y - starty) / 2;
    }

    private void bodyDisplay(Canvas canvas,int number,float offsetX,float offsetY,float radiusIn,int color){

        float radius = radiusIn;
        float spaceText = 15;

        Paint circle1_paint = new Paint();
        circle1_paint.setColor(color);

        Paint circle2_paint = new Paint();
        circle2_paint.setColor(ContextCompat.getColor(view.getContext(), R.color.bg_simulate));

        Paint text_paint = new Paint();
        text_paint.setColor(color);
        text_paint.setTextSize(50);

        canvas.drawCircle(offsetX,offsetY,radius,circle1_paint);
        canvas.drawCircle(offsetX,offsetY,radius - 10,circle2_paint);
        if(number < 10)canvas.drawText(String.valueOf(number),offsetX - spaceText,offsetY + spaceText,text_paint);
        else canvas.drawText(String.valueOf(number),offsetX - spaceText*2,offsetY + spaceText,text_paint);

    }
}

class CircleAnimation{ //------------------------------------------------------------------
    private View view;
    private Canvas canvas;

    private List<CirclePHBall> circlePHBallList;
    public List<Float> degreeList;

    Paint line_paint;
    float line_startX;
    float line_startY;
    float line_endX;
    float line_endY;

    float line_degree = 180;
    float line_degreeSpeed = 4;
    float line_width = 350;
    boolean line_degreeIsFirstAnim = true;
    float line_degreeCurrent = 90;
    float line_degreeMoveSpeed = 1.2f;

    CircleAnimation(View view){
        this.view = view;

        circlePHBallList = new ArrayList<>();
        degreeList = new ArrayList<Float>();

        line_paint = new Paint();
        line_paint.setStyle(Paint.Style.FILL);
        line_paint.setAntiAlias(true);
        line_paint.setColor(Color.RED);
        line_paint.setStrokeWidth(10);

        line_startX = 0;
        line_startY = 0;
        line_endX   = 0;
        line_endY   = 0;

        Handler handler = new Handler();
        handler.post(new Runnable() {
            @Override
            public void run() {
                onUpdate();
                view.invalidate();
                handler.postDelayed(this, 1);
            }
        });
    }

    public void draw(Canvas canvas){
        this.canvas = canvas;
        line_startX = view.getWidth()  / 2;
        line_startY = view.getHeight() /2;

        canvas.drawLine(line_startX,line_startY,line_endX,line_endY,line_paint);

        float range = 50;
        for (CirclePHBall circlePHBall:circlePHBallList) {
            circlePHBall.show(canvas);
            range = circlePHBall.radius_default/2;

            if (line_endX <= circlePHBall.getX() + range && line_endX > circlePHBall.getX() - range && line_endY <= circlePHBall.getY() + range && line_endY > circlePHBall.getY() - range){
                circlePHBall.bigFocus();
            }else{
                circlePHBall.bigDefault();
            }

            circlePHBall.update();

        }



    }
    int currentPH = 0;
    public void onUpdate(){
        line_endX = (float) (line_width * Math.cos(Math.toRadians(line_degree))) + line_startX;
        line_endY = (float) (line_width * Math.sin(Math.toRadians(line_degree))) + line_startY;



        if (line_degreeIsFirstAnim){
            line_degree+=line_degreeSpeed;
        }else if(!line_degreeIsFirstAnim && line_degree != line_degreeCurrent){
            line_degreeMoveSpeed = Math.abs(line_degreeCurrent - line_degree) / 12;
            line_degree += line_degreeMoveSpeed * (((line_degreeCurrent - line_degree) > 0) ? 1f : -1f);
        }

        if (line_degree % 24 == 0 && currentPH <= 14){
            degreeList.add(line_degree);

            float line_endX_start = (float) ((line_width+300) * Math.cos(Math.toRadians(line_degree))) + line_startX;
            float line_endY_start = (float) ((line_width+300) * Math.sin(Math.toRadians(line_degree))) + line_startY;


            circlePHBallList.add(new CirclePHBall(view,line_endX_start,line_endY_start,line_endX,line_endY,currentPH));
            currentPH++;

            if(currentPH > 14)line_degreeIsFirstAnim = false;

        }

    }


    public void setCurrentPH(float ph_input){
        //192-528
        float ph = map(ph_input,0,14,192,528);

        this.line_degreeCurrent = ph;


    }

    private static float map(float input, float inputStart, float inputEnd, float outputStart, float outputEnd) {
        return ((outputEnd - outputStart) / (inputEnd - inputStart)) * (input - inputStart) + outputStart;
    }




}

public class CircleView extends View { //------------------------------------------------------------------
    private Paint paint;
    private Canvas canvas;


    private CircleAnimation circleAnimation;
    public CircleAnimation getCircleAnimation() {
        return circleAnimation;
    }


    public CircleView(Context context, AttributeSet attrs) {
        super(context, attrs);

        circleAnimation = new CircleAnimation(this);
    }


    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        this.canvas = canvas;

        circleAnimation.draw(canvas);
    }

}