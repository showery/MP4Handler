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

    public int getmNativeFilterAddress(){
        return this.mNativeFilterAddress;
    }

    private native boolean _init(String src, long start, long duration);
    private native boolean _uninit();

    private native String _getSrc();

    private native void _setSrc(String src) ;

    private native long _getStart();

    private native void _setStart(long start);

    private native long _getDuration();

    private native void _setDuration(long duration);


    // member accessed by native methods.
    private int mNativeFilterAddress;
}
