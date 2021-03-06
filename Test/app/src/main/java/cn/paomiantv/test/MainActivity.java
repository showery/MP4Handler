package cn.paomiantv.test;

import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;

import cn.paomiantv.mediasdk.PMClip;
import cn.paomiantv.mediasdk.PMStoryboard;
import cn.paomiantv.mediasdk.PMEngine;
import cn.paomiantv.render.PMRenderer;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private final String clipSrc = Environment.getExternalStorageDirectory() + "/source.mp4";
    private final String clipDst = Environment.getExternalStorageDirectory() + "/clip.mp4";

    private final String joinSrc1 = Environment.getExternalStorageDirectory() + "/part1.mp4";
    private final String joinSrc2 = Environment.getExternalStorageDirectory() + "/part2.mp4";
    private final String joinSrc3 = Environment.getExternalStorageDirectory() + "/part3.mp4";
    private final String joinDst = Environment.getExternalStorageDirectory() + "/join.mp4";
    private TextView mtvClip;
    private TextView mtvJoin;
    private Button mbtnClip;
    private Button mbtnJoin;
    private GLView mglvPlayer;
    private PMRenderer mRenderer;
    PMStoryboard mStoryboard;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        PMEngine.getInstance().init();
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        mtvClip = (TextView) findViewById(R.id.tv_clip);
        mtvJoin = (TextView) findViewById(R.id.tv_join);
        mbtnClip = (Button) findViewById(R.id.btn_clip);
        mbtnJoin = (Button) findViewById(R.id.btn_join);
        mglvPlayer = (GLView) findViewById(R.id.glv_player);


        mglvPlayer.getHolder().setFormat(PixelFormat.OPAQUE);

        mRenderer = new PMRenderer();
        mRenderer.setScale(1.0f, 1.0f, 1.0f);
        mRenderer.setListener(new PMRenderer.FrameListener() {

            public void onNewFrame() {
                mglvPlayer.requestRender();
            }
        });
        mglvPlayer.setRenderer(mRenderer);
        mglvPlayer.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        mbtnClip.setOnClickListener(this);
        mbtnJoin.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_clip: {
//                PMStoryboard storyboard = new PMStoryboard(clipDst);
//                PMClip clip = new PMClip(clipSrc, 2000l, 5000l);
//                storyboard.addClip(clip);
//                storyboard.setProcessListener(new PMStoryboard.ProcessListener() {
//                    @Override
//                    public void onProgress(PMStoryboard storyboard, int progress) {
//                        mtvClip.setText(progress);
//                    }
//
//                    @Override
//                    public void onFailed(PMStoryboard storyboard, int err, String description) {
//                        mtvClip.setText("error no: " + err + ", reason: " + description);
//                    }
//
//                    @Override
//                    public void onSuccess(PMStoryboard storyboard, String path) {
//                        mtvClip.setText(path);
//                    }
//
//                    @Override
//                    public void onAlways(PMStoryboard storyboard) {
//                        storyboard.destory();
//                    }
//                });
//                PMEngine.getInstance().setDataSource(storyboard);
//                PMEngine.getInstance().start(false);
            }
            break;
            case R.id.btn_join: {
                if (mStoryboard == null) {
                    ArrayList<String> arrSrc = new ArrayList<>();
                    arrSrc.add(joinSrc1);
                    arrSrc.add(joinSrc2);
                    arrSrc.add(joinSrc3);
                    mStoryboard = new PMStoryboard(joinDst);
                    PMClip clip = new PMClip(joinSrc1, 0l, 5000l);
                    mStoryboard.addClip(clip);
                    clip = new PMClip(joinSrc2, 0l, 5000l);
                    mStoryboard.addClip(clip);
                    clip = new PMClip(joinSrc3, 0l, 5000l);
                    mStoryboard.addClip(clip);
                    mStoryboard.setProcessListener(new PMStoryboard.ProcessListener() {
                        @Override
                        public void onProgress(PMStoryboard storyboard, int progress) {
//                            mtvJoin.setText(progress);
                        }

                        @Override
                        public void onFailed(PMStoryboard storyboard, int err, String description) {
  //                          mtvJoin.setText("error no: " + err + ", reason: " + description);
                        }

                        @Override
                        public void onSuccess(PMStoryboard storyboard, String path) {
    //                        mtvJoin.setText(path);
                        }

                        @Override
                        public void onAlways(PMStoryboard storyboard) {
                        }
                    });
                    mStoryboard.attachRenderer(mRenderer);
                }
                PMEngine.getInstance().setDataSource(mStoryboard);
                PMEngine.getInstance().start(false);
            }
            break;
            default:
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        PMEngine.getInstance().cancel();
        if (mStoryboard != null) {
            mStoryboard.detachRenderer();
            mStoryboard.destory();
        }
        if (mglvPlayer != null) {
            mglvPlayer.queueEvent(new Runnable() {

                @Override
                public void run() {
                    if (mRenderer != null) {
                        // TODO Auto-generated method stub
                        mRenderer.destroy();
                        mRenderer = null;
                    }
                }
            });
        }

        PMEngine.getInstance().uninit();
    }
}
