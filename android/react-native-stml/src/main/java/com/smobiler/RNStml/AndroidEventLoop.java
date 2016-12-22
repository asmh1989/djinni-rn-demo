package com.smobiler.RNStml;

import android.os.Handler;
import android.os.Looper;

import com.smobiler.djinni.AsyncTask;
import com.smobiler.djinni.EventLoop;

public class AndroidEventqLoop extends EventLoop {
    Handler mHandler;
    public AndroidEventLoop() {
        mHandler = new Handler(Looper.getMainLooper());
    }
    public void post(final AsyncTask task) {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                task.execute();
            }
        });
    }
}
