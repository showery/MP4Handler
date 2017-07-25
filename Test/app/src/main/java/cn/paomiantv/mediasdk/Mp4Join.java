package cn.paomiantv.mediasdk;

import android.os.Handler;
import android.os.Looper;

import java.util.ArrayList;

/**
 * Created by John on 2017/7/25.
 */

public class Mp4Join {


    private String mDst;
    private ArrayList<String> mArrSrc;
    private Handler mainHandler = new Handler(Looper.getMainLooper());

    public Mp4Join(String mDst, ArrayList<String> mArrSrc) {
        this.mDst = mDst;
        this.mArrSrc = mArrSrc;
    }

    // 回调函数
    public interface JoinListener {
        public void onProgress(int progress);

        public void onFailed(int err, String description);

        public void onSuccess(String path);
    }

    private Mp4Join.JoinListener mListener = null;

    public Mp4Join setClipListener(Mp4Join.JoinListener listener) {
        this.mListener = listener;
        return this;
    }

    public Mp4Join resetClipListener() {
        this.mListener = null;
        return this;
    }

    public boolean join(){

        for(int i = 0; i < mArrSrc.size(); i++)
        {
            joinMp4(mArrSrc.get(i),mDst);
        }
        return true;
    }

    public void start(){
        new Thread(new Runnable() {
            @Override
            public void run() {
                join();
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

    private native boolean joinMp4(String src, String dst);

}
