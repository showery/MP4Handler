package cn.paomiantv.mediasdk;

import java.util.ArrayList;

/**
 * Created by John on 2017/7/20.
 */

public class PMEngine {
    static {
        System.loadLibrary("mp4v2");
    }
    private PMEngine() {
    }

    // 变量定义
    static class Mp4v2HelperHolder {
        private final static PMEngine mInstance = new PMEngine();
    }

    public static PMEngine getInstance() {
        return Mp4v2HelperHolder.mInstance;
    }

    public void init() {
        _init();
    }

    public void uninit() {
        _uninit();
    }

    public PMStoryboard createStoryboard(String dst){
        return new PMStoryboard(dst);
    }

    public void destoryStoryboard(PMStoryboard storyboard){
        storyboard.destory();
    }

    private native void _init();
    private native void _uninit();
    // member accessed by native methods.
    private int mNativeEngineAddress;
}
