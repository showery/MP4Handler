package cn.paomiantv.mediasdk;

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

    public void seekTo(long position) {
        _seekTo(position);
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

    private native void _seekTo(long position);

    private native void _pause();

    private native void _resume();

    private native void _cancel();

    // member accessed by native methods.
    private int mNativeEngineAddress;
}
