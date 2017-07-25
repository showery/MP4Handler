package cn.paomiantv.mediasdk;

import java.util.ArrayList;

/**
 * Created by John on 2017/7/20.
 */

public class Mp4v2Helper {
    static {
        System.loadLibrary("mp4v2");
    }

    private Mp4v2Helper() {
    }

    // 变量定义
    static class Mp4v2HelperHolder {
        private final static Mp4v2Helper mInstance = new Mp4v2Helper();
    }

    public static Mp4v2Helper getInstance() {
        return Mp4v2HelperHolder.mInstance;
    }

    public void cropMp4(Mp4Clip.ClipParam param, Mp4Clip.ClipListener listener) {
        new Mp4Clip(param)
                .setClipListener(listener)
                .start();
    }
    public void joinMp4(String dst, ArrayList<String> src, Mp4Join.JoinListener listener) {
        new Mp4Join(dst,src)
                .setClipListener(listener)
                .start();
    }

}
