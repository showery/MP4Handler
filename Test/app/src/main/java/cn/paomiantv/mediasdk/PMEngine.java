package cn.paomiantv.mediasdk;

import android.os.Build;

/**
 * Created by John on 2017/7/20.
 */

public class PMEngine {

    static {
        switch(Build.VERSION.SDK_INT)
        {
            case Build.VERSION_CODES.JELLY_BEAN:
                System.loadLibrary("native_codec16");
                break;
            case Build.VERSION_CODES.JELLY_BEAN_MR1:
                System.loadLibrary("native_codec17");
                break;
            case Build.VERSION_CODES.JELLY_BEAN_MR2:
                System.loadLibrary("native_codec18");
                break;
            case Build.VERSION_CODES.KITKAT:
                System.loadLibrary("native_codec19");
                break;
        }
        System.loadLibrary("mp4v2");
        System.loadLibrary("render");
        System.loadLibrary("mediasdk");
        System.loadLibrary("interface");
    }

    private PMEngine() {
    }

    // 变量定义
    static class PMEngineHolder {
        private final static PMEngine mInstance = new PMEngine();
    }

    public static PMEngine getInstance() {
        return PMEngineHolder.mInstance;
    }

    public boolean init() {
        return _init();
    }

    public void uninit() {
        _uninit();
    }

    public void setDataSource(PMStoryboard storyboard) {
        _setDataSource(storyboard);
    }

    public void start(boolean isSave) {
        _start(isSave);
    }

    public void seekTo(int clipIndex) {
        _seekTo(clipIndex);
    }

    public void pause() {
        _pause();
    }

    public void resume() {
        _resume();
    }

    public void cancel() {
        _cancel();
    }

    private native boolean _init();

    private native void _uninit();

    private native void _setDataSource(PMStoryboard storyboard);

    private native void _start(boolean isSave);

    private native void _seekTo(int clipIndex);

    private native void _pause();

    private native void _resume();

    private native void _cancel();

    // member accessed by native methods.
    private int mNativeEngineAddress;
}
