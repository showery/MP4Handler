package cn.paomiantv.mediasdk;

/**
 * Created by John on 2017/7/21.
 */

public class PMClip {

    public PMClip(String src, long start, long duration) {
        _init(src, start, duration);
    }

    public boolean destory() {
        return _uninit();
    }

    public String getSrc() {
        return _getSrc();
    }

    public void setSrc(String src) {
        _setSrc(src);
    }

    public long getStart() {
        return _getStart();
    }

    public void setmStart(long startTm) {
        _setStart(startTm);
    }

    public long getDuration() {
        return _getDuration();
    }

    public void setDuration(long duration) {
        _setDuration(duration);
    }

    public PMFilter getFilter(int position) {
        return _getFilter(position);
    }

    public PMTransition getTransition(int position) {
        return _getTransition(position);
    }

    public void addFilter(PMFilter filter) {
        _addFilter(filter);
    }

    public PMFilter removeFilter(int position) {
        return _removeFilter(position);
    }

    public void addTransition(PMTransition transition) {
        _addTransition(transition);
    }

    public PMTransition removeTransition(int position) {
        return _removeTransition(position);
    }

    public void getFilterCount() {
        _getFilterCount();
    }

    public void getTransitionCount() {
        _getTransitionCount();
    }

    public int getNativeClipAddress() {
        return this.mNativeClipAddress;
    }

    private native boolean _init(String mSrc, long mStart, long mDuration);

    private native boolean _uninit();

    private native String _getSrc();

    private native void _setSrc(String src);

    private native long _getStart();

    private native void _setStart(long mStart);

    private native long _getDuration();

    private native void _setDuration(long mDuration);


    private native boolean _addFilter(PMFilter filter);

    private native PMFilter _getFilter(int index);

    private native PMFilter _removeFilter(int position);

    private native int _getFilterCount();


    private native boolean _addTransition(PMTransition transition);

    private native PMTransition _getTransition(int position);

    private native PMTransition _removeTransition(int position);

    private native int _getTransitionCount();

    // member accessed by native methods.
    private int mNativeClipAddress;
}
