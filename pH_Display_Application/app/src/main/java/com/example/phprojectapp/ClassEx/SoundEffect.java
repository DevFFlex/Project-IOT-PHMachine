package com.example.phprojectapp.ClassEx;

import android.content.Context;
import android.media.MediaPlayer;

import com.example.phprojectapp.MainPage;
import com.example.phprojectapp.R;

public class SoundEffect {

    private MediaPlayer sound_bubble,sound_glitch;

    public SoundEffect(Context context) {
        sound_bubble = MediaPlayer.create(context, R.raw.ef_bubble3);
        sound_glitch = MediaPlayer.create(context, R.raw.glitch);
    }


    public void play_bubble(){
        sound_bubble.start();
    }

    public void play_glitch(){
        sound_glitch.start();
    }
}
