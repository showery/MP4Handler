package cn.paomiantv.test;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class GLView extends GLSurfaceView {
	public GLView(Context context) {
		super(context);
		init();

	}

	public GLView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		init();
	}
	private void init(){
		setEGLConfigChooser(new cn.paomiantv.test.EGLConfigChooser(8, 8, 8, 8, 0, 0));
		setEGLWindowSurfaceFactory(new cn.paomiantv.test.EGLWindowSurfaceFactory());
		setEGLContextFactory(new cn.paomiantv.test.EGLContextFactory());
		setEGLContextClientVersion(2);
		setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR
				| GLSurfaceView.DEBUG_LOG_GL_CALLS);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		// TODO Auto-generated method stub
		return super.onTouchEvent(event);

	}

}
