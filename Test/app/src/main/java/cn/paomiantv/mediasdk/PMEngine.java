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
    static class Mp4v2HelperHolder {
        private final static PMEngine mInstance = new PMEngine();
    }

    public static PMEngine getInstance() {
        return Mp4v2HelperHolder.mInstance;
    }

    public void init() {
    }

    public void uninit() {
    }

    public PMStoryboard createStoryboard(String dst){
        return new PMStoryboard(dst);
    }

    public void destoryStoryboard(PMStoryboard storyboard){
        storyboard.destory();
    }
}
