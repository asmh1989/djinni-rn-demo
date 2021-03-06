// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from stml.djinni

package com.smobiler.djinni;

import java.util.concurrent.atomic.AtomicBoolean;

/** cache */
public abstract class Cache {
    public abstract String get(String key, String defaultVaue);

    public abstract boolean find(String key);

    public abstract boolean del(String key);

    public abstract boolean put(String key, String value);

    public abstract String getRootPath();

    private static final class CppProxy extends Cache
    {
        private final long nativeRef;
        private final AtomicBoolean destroyed = new AtomicBoolean(false);

        private CppProxy(long nativeRef)
        {
            if (nativeRef == 0) throw new RuntimeException("nativeRef is zero");
            this.nativeRef = nativeRef;
        }

        private native void nativeDestroy(long nativeRef);
        public void destroy()
        {
            boolean destroyed = this.destroyed.getAndSet(true);
            if (!destroyed) nativeDestroy(this.nativeRef);
        }
        protected void finalize() throws java.lang.Throwable
        {
            destroy();
            super.finalize();
        }

        @Override
        public String get(String key, String defaultVaue)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_get(this.nativeRef, key, defaultVaue);
        }
        private native String native_get(long _nativeRef, String key, String defaultVaue);

        @Override
        public boolean find(String key)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_find(this.nativeRef, key);
        }
        private native boolean native_find(long _nativeRef, String key);

        @Override
        public boolean del(String key)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_del(this.nativeRef, key);
        }
        private native boolean native_del(long _nativeRef, String key);

        @Override
        public boolean put(String key, String value)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_put(this.nativeRef, key, value);
        }
        private native boolean native_put(long _nativeRef, String key, String value);

        @Override
        public String getRootPath()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getRootPath(this.nativeRef);
        }
        private native String native_getRootPath(long _nativeRef);
    }
}
