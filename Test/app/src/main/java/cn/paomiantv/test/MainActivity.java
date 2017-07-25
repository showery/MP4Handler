package cn.paomiantv.test;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;

import cn.paomiantv.mediasdk.Mp4Clip;
import cn.paomiantv.mediasdk.Mp4Join;
import cn.paomiantv.mediasdk.Mp4v2Helper;

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        mtvClip = (TextView) findViewById(R.id.tv_clip);
        mtvJoin = (TextView) findViewById(R.id.tv_join);
        mbtnClip = (Button) findViewById(R.id.btn_clip);
        mbtnJoin = (Button) findViewById(R.id.btn_clip);
        mbtnClip.setOnClickListener(this);
        mbtnJoin.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.btn_clip:
                Mp4Clip.ClipParam param = new Mp4Clip.ClipParam();
                param.mSrc = clipSrc;
                param.mDst = clipDst;
                param.mStart = 2000l;
                param.mDuration = 5000l;
                Mp4v2Helper.getInstance().cropMp4(param, new Mp4Clip.ClipListener() {
                    @Override
                    public void onProgress(int progress) {
                        mtvClip.setText(progress);
                    }

                    @Override
                    public void onFailed(int err, String description) {
                        mtvClip.setText("error no: "+err+", reason: "+description);
                    }

                    @Override
                    public void onSuccess(String path) {
                        mtvClip.setText(path);
                    }
                });
                break;
            case R.id.btn_join:
                ArrayList<String> arrSrc = new ArrayList<>();
                arrSrc.add(joinSrc1);
                arrSrc.add(joinSrc2);
                arrSrc.add(joinSrc3);
                Mp4v2Helper.getInstance().joinMp4(joinDst, arrSrc, new Mp4Join.JoinListener(){

                    @Override
                    public void onProgress(int progress) {
                        mtvJoin.setText(progress);
                    }

                    @Override
                    public void onFailed(int err, String description) {
                        mtvJoin.setText("error no: "+err+", reason: "+description);
                    }

                    @Override
                    public void onSuccess(String path) {
                        mtvJoin.setText(path);
                    }
                } );
                break;
            default:
                break;
        }
    }
}
