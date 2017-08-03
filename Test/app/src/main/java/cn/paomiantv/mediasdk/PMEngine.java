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

    public boolean init() {
        return _init();
    }

    public void uninit() {
        _uninit();
    }

    public void startPreview(PMStoryboard storyboard) {
        _startPreview(storyboard);
    }
    public void pausePreview() {
        _pausePreview();
    }
    public void stopPreview() {
        _stopPreview();
    }

    public void process(PMStoryboard storyboard) {
        _progress(storyboard);
    }
    public void pause() {
        _pause();
    }

    public void cancel() {
        _cancel();
    }

    public PMStoryboard createStoryboard(String dst){
        return new PMStoryboard(dst);
    }

    public void destoryStoryboard(PMStoryboard storyboard){
        storyboard.destory();
    }

    private native boolean _init();
    private native void _uninit();
    private native void _startPreview(PMStoryboard storyboard);
    private native void _pausePreview();
    private native void _stopPreview();
    private native void _progress(PMStoryboard storyboard);
    private native void _pause();
    private native void _cancel();
    // member accessed by native methods.
    private int mNativeEngineAddress;
}
