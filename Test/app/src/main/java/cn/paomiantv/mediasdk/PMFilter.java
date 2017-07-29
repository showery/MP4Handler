package cn.paomiantv.mediasdk;

/**
 * Created by John on 2017/7/28.
 */

public class PMFilter {
    public PMFilter(String src) {
        _init(src, 0, -1);
    }
    public PMFilter(String src, long start) {
        _init(src, start, -1);
    }

    public PMFilter(String src, long start, long duration) {
        _init(src, start, duration);
    }

    public boolean destory(){
        return  _uninit();
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

    public String getFilter(int index) {
        return _getFilter(index);
    }

    public String getTransition(int index) {
        return _getTransition(index);
    }

    public void addFilter(String filter) {
        _addFilter(filter);
    }

    public void removeFilter(String filter) {
        _removeFilter(filter);
    }

    public void addTransition(String transition) {
        _addTransition(transition);
    }

    public void removeTransition(String transition) {
        _removeTransition(transition);
    }

    public int getNativeClipAddress(){
        return this.mNativeClipAddress;
    }

    private native boolean _init(String mSrc, long mStart, long mDuration);
    private native boolean _uninit();

    private native String _getSrc();

    private native void _setSrc(String src) ;

    private native long _getStart();

    private native void _setStart(long mStart);

    private native long _getDuration();

    private native void _setDuration(long mDuration);


    private native String _getFilter(int index);

    private native boolean _addFilter(String filter);

    private native boolean _removeFilter(String filter);

    private native int _getFilterCount();


    private native boolean _addTransition(String transition);

    private native String _getTransition(int index);

    private native boolean _removeTransition(String transition);

    private native int _getTransitionCount();

    // member accessed by native methods.
    private int mNativeClipAddress;
}
