package com.smobiler.RNStml;

import android.support.annotation.Nullable;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.smobiler.djinni.Stml;
import com.smobiler.djinni.StmlListener;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by sun on 2016/12/21.
 */

public class StmlManagerModule extends ReactContextBaseJavaModule {

    private ReactApplicationContext reactContext;

    private Stml mStmlImpl;
    private LogImpl mLogImpl = new LogImpl();

    private String mServer;
    private int mPort;
    private AndroidEventLoop mEventLooper = new AndroidEventLoop();
    private AndroidThreadLauncher mThreadLaucher = new AndroidThreadLauncher();

    private static final String StmlEventConnecting = "stml_connecting";
    private static final String StmlEventConnected = "stml_connected";
    private static final String StmlEventDisconnect = "stml_disconnected";
    private static final String StmlEventSending="stml_sending";
    private static final String StmlEventSended="stml_sended";
    private static final String StmlEventReceived="stml_received";
    static {
        System.loadLibrary("stml");
    }

    public StmlManagerModule(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;
        mStmlImpl = Stml.create(mLogImpl, null, mEventLooper, mThreadLaucher);
        mStmlImpl.setListener(new StmlListenerImpl());
    }

    @Override
    public String getName() {
        return "StmlManager";
    }

    @Override
    public Map<String, Object> getConstants() {
        final Map<String, Object> constants = new HashMap<>();
        constants.put("StmlEventConnecting", StmlEventConnecting);
        constants.put("StmlEventConnected", StmlEventConnected);
        constants.put("StmlEventDisconnect", StmlEventDisconnect);
        constants.put("StmlEventSending", StmlEventSending);
        constants.put("StmlEventSended", StmlEventSended);
        constants.put("StmlEventReceived", StmlEventReceived);

        return constants;
    }

    private void sendEvent(ReactContext reactContext,
                           String eventName,
                           @Nullable WritableMap params) {
        reactContext
                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                .emit(eventName, params);
    }

    @ReactMethod
    public void start(String host, int port){
        mServer = host;
        mPort = port;

        mStmlImpl.start(host, port);
    }

    @ReactMethod
    public void post(String msg)
    {
        mStmlImpl.post(msg, false);
    }


    class StmlListenerImpl extends StmlListener{

        @Override
        public void connecting() {
            WritableMap params = Arguments.createMap();
            sendEvent(reactContext, StmlEventConnecting, params);
        }

        @Override
        public void connected() {
            WritableMap params = Arguments.createMap();
            sendEvent(reactContext, StmlEventConnected, params);
        }

        @Override
        public void disconnect(String error) {
            WritableMap params = Arguments.createMap();
            params.putString("error", error);
            sendEvent(reactContext, StmlEventDisconnect, params);
        }

        @Override
        public void received(String msg) {
            WritableMap params = Arguments.createMap();
            params.putString("msg", msg);
            sendEvent(reactContext, StmlEventReceived, params);
        }

        @Override
        public void sended(String msg) {
            WritableMap params = Arguments.createMap();
            sendEvent(reactContext, StmlEventSended, params);
        }

        @Override
        public void sending(String msg, boolean showloading) {
            WritableMap params = Arguments.createMap();
            params.putBoolean("showloading", showloading);
            sendEvent(reactContext, StmlEventSending, params);
        }
    }

}
