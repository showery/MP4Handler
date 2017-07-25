package cn.paomiantv.mediasdk;

import android.os.Handler;
import android.os.Looper;

/**
 * Created by John on 2017/7/21.
 */

public class Mp4Clip {
    private ClipParam param;
    private Handler mainHandler = new Handler(Looper.getMainLooper());

    public Mp4Clip(ClipParam param) {
        this.param =param;
    }

    // 回调函数
    public interface ClipListener {
        public void onProgress(int progress);

        public void onFailed(int err, String description);

        public void onSuccess(String path);
    }

    private ClipListener mListener = null;

    public Mp4Clip setClipListener(ClipListener listener) {
        this.mListener = listener;
        return this;
    }

    public Mp4Clip resetClipListener() {
        this.mListener = null;
        return this;
    }

    public boolean clip() {
        return clipMp4(param.mSrc, param.mDst, param.mStart, param.mDuration);
    }

    public void start() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                clip();
            }
        }).start();
    }

    protected void fireOnFailed(final int error, final String description) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onFailed(error, description);
                    }
                });

            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    protected void fireOnProgress(final int progress) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onProgress(progress);
                    }
                });
            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    protected void fireOnSuccess(final String path) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onSuccess(path);
                    }
                });

            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    public static class ClipParam {
        public String mSrc;
        public String mDst;
        public long mStart;
        public long mDuration;
    }

    private native boolean clipMp4(String src, String dst, long start, long duration);
}
