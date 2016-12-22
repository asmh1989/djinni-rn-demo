package com.smobiler.RNStml;

import android.util.Log;
import com.smobiler.djinni.LogInterface;

/**
 * Created by sun on 2016/12/21.
 */

public class LogImpl extends LogInterface {

    @Override
    public void d(String tag, String msg) {
        Log.d(tag, msg);
    }

    @Override
    public void w(String tag, String msg) {
        Log.w(tag, msg);
    }

    @Override
    public void e(String tag, String msg) {
        Log.e(tag, msg);
    }

    @Override
    public void i(String tag, String msg) {
        Log.i(tag, msg);
    }
}
