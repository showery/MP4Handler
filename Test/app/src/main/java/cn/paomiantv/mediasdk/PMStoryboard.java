package cn.paomiantv.mediasdk;

import android.os.Handler;
import android.os.Looper;

/**
 * Created by John on 2017/7/25.
 */

public class PMStoryboard {


    private Handler mainHandler = new Handler(Looper.getMainLooper());

    private PMStoryboard.ProcessListener mListener = null;

    public PMStoryboard(String dst) {
        _init(dst);
    }

    public void destory() {
        _uninit();
    }


    public boolean addClip(PMClip clip) {
        return _addClip(clip);

    }


    public PMClip getClip(int index) {
        return _getClip(index);
    }

    public boolean removeClip(int index) {
        PMClip clip = _removeClip(index);
        clip.destory();
        return true;
    }

    public boolean swapClip(int a, int b) {
        return _swapClip(a, b);
    }

    public boolean process() {
        return _procsss();
    }

    public void setProcessListener(PMStoryboard.ProcessListener listener) {
        mListener = listener;
    }

    public void resetListener() {
        mListener = null;
    }

    public void cancel() {
        _cancel();
    }

    protected void fireOnFailed(final PMStoryboard storyboard, final int error, final String description) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onFailed(storyboard, error, description);
                    }
                });

            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    protected void fireOnProgress(final PMStoryboard storyboard, final int progress) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onProgress(storyboard, progress);
                    }
                });
            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    protected void fireOnSuccess(final PMStoryboard storyboard, final String path) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onSuccess(storyboard, path);
                    }
                });

            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    private native boolean _init(String dstPath);

    private native boolean _uninit();

    private native boolean _addClip(PMClip clip);

    private native PMClip _removeClip(int index);

    private native PMClip _getClip(int index);

    private native boolean _swapClip(int a, int b);

    private native boolean _procsss();

    private native boolean _cancel();

    // member accessed by native methods.
    private int mNativeStoryboardAddress;

    // 回调函数
    public interface ProcessListener {
        public void onProgress(PMStoryboard storyboard, int progress);

        public void onFailed(PMStoryboard storyboard, int err, String description);

        public void onSuccess(PMStoryboard storyboard, String path);

        public void onAlways(PMStoryboard storyboard);
    }
}
