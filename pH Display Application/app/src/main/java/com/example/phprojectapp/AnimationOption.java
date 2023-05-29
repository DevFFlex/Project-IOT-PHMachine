package com.example.phprojectapp;

import android.animation.Animator;
import android.animation.AnimatorInflater;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.animation.TimeInterpolator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.TextView;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Random;

public class AnimationOption {

    private Context context;

    public AnimationOption(Context context){
        this.context = context;
    }

    public void startAnim(View viewObj,int id){
        Animation animation = AnimationUtils.loadAnimation(context, id);
        viewObj.startAnimation(animation);
    }

    public void startAnimation(View view_obj,int id){
        Animator animator = AnimatorInflater.loadAnimator(context,id);
        animator.setTarget(view_obj);
        animator.start();
    }

    public void startValueAnimationCounter(TextView textViewValue){
        ValueAnimator valueAnimator = (ValueAnimator) AnimatorInflater.loadAnimator(context, R.animator.counter);
        valueAnimator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                float animatedValue = (float) animation.getAnimatedValue();
                textViewValue.setText(new DecimalFormat("#.#").format(animatedValue));
            }
        });
        valueAnimator.start();
    }


    public void startShuffleSlideIn(ArrayList<View> views) {

        final int startPos = 1000;
        final int delay = 500;

        int i = 0;
        for (View view:views){
            view.setTranslationX(((i % 2 == 0) ? -1: 1)*startPos);
            i++;
        }

        i = 0;

        for (View view:views) {
            AnimatorSet animatorSet = new AnimatorSet();

            ObjectAnimator tranX = ObjectAnimator.ofFloat(view,"translationX",((i % 2 == 0) ? -1: 1)*startPos,view.getWidth());
            tranX.setDuration(delay);
            tranX.setRepeatCount(0);
            tranX.setStartDelay(i * delay);

            int delayMal = new Random().nextInt(1000);
            ObjectAnimator alpha = ObjectAnimator.ofFloat(view,"alpha",0,1);
            alpha.setDuration(20);
            alpha.setRepeatCount(30);
            alpha.setStartDelay((i + 2) * (delay));

            ObjectAnimator tranX2 = ObjectAnimator.ofFloat(view,"translationX",-10,10);
            tranX2.setDuration(20);
            tranX2.setRepeatCount(30);
            tranX2.setStartDelay((i + 2) * (delay));


            ObjectAnimator loopTranX = ObjectAnimator.ofFloat(view,"translationY",view.getHeight(),view.getHeight() - 10);
            loopTranX.setDuration(2000);
            loopTranX.setRepeatCount(ValueAnimator.INFINITE);
            loopTranX.setStartDelay((i + 2) * (delay));
            loopTranX.setInterpolator(new AccelerateDecelerateInterpolator());
            loopTranX.setRepeatMode(ValueAnimator.REVERSE);

            animatorSet.playTogether(tranX,alpha,tranX2,loopTranX);
            animatorSet.start();
            i++;
        }

    }


}
