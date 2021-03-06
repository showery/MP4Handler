package cn.paomiantv.mediasdk;

import android.os.Handler;
import android.os.Looper;

import cn.paomiantv.render.PMRenderer;

/**
 * Created by John on 2017/7/25.
 */

public class PMStoryboard {


    private Handler mainHandler = new Handler(Looper.getMainLooper());

    private ProcessListener mListener = null;

    public PMStoryboard(String dst) {
        _init(dst);
    }

    public void destory() {
        _uninit();
    }

    public boolean setBGM(String srcPath, long startCutTm, long durationCutTm, long startTm, long endTm) {
        return _setBGM(srcPath, startCutTm, durationCutTm, startTm, endTm);

    }

    public boolean addClip(PMClip clip) {
        return _addClip(clip);

    }

    public boolean replaceClip(int position, PMClip clip) {
        return _replaceClip(position, clip);

    }

    public boolean insertClip(int position, PMClip clip) {
        return _intertClip(position, clip);

    }

    public PMClip getClip(int index) {
        return _getClip(index);
    }

    public PMClip removeClip(int index) {
        return _removeClip(index);
    }

    public boolean swapClip(int a, int b) {
        return _swapClip(a, b);
    }

    public int getClipCount() {
        return _getClipCount();
    }

    public void setProcessListener(PMStoryboard.ProcessListener listener) {
        mListener = listener;
    }

    public void resetListener() {
        mListener = null;
    }

    public boolean attachRenderer(PMRenderer pmRenderer) {
        return _attachRenderer(pmRenderer);
    }

    public void detachRenderer() {
        _detachRenderer();
    }

    protected void fireOnFailed(final int error, final String description) {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onFailed(PMStoryboard.this, error, description);
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
                        mListener.onProgress(PMStoryboard.this, progress);
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
                        mListener.onSuccess(PMStoryboard.this, path);
                    }
                });

            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    protected void fireOnAlways() {
        try {
            if (mListener != null) {
                mainHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        //已在主线程中，可以更新UI
                        mListener.onAlways(PMStoryboard.this);
                    }
                });

            }
        } catch (Exception ex) {
            // Log.e(TAG, "fire fireOnMessage failed: " + ex.getMessage());
        }
    }

    private native boolean _init(String dstPath);

    private native boolean _uninit();

    private native boolean _setBGM(String srcPath, long startCutTm, long durationCutTm, long startTm, long endTm);

    private native boolean _addClip(PMClip clip);

    private native boolean _replaceClip(int position, PMClip clip);

    private native boolean _intertClip(int position, PMClip clip);

    private native PMClip _removeClip(int position);

    private native PMClip _getClip(int position);

    private native boolean _swapClip(int positionA, int positionB);

    private native int _getClipCount();

    private native boolean _attachRenderer(PMRenderer pmRenderer);

    private native void _detachRenderer();

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
