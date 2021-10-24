package com.juhang.green;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.widget.Toast;

public class StartActivity  extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        checkPermission();
    }


    private void checkPermission() {
        // if SDK level > 23 then check permission
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {

            // 检查该权限是否已经获取
            int i = ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
            // 权限是否已经 授权 GRANTED---授权  DINIED---拒绝
            if (i != PackageManager.PERMISSION_GRANTED) {
                // 如果没有授予该权限，就去提示用户请求
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
            }
            else {
                startGame();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode) {
            case 1: {
                // If request is cancelled, the result arrays are empty.
                for (int i = 0; i < permissions.length; i++) {
                    String p = permissions[i];

                    if (p.equals(Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                        Log.d("mame4m", "WRITE_EXTERNAL_STORAGE : " + grantResults[i]);
                        if (grantResults[i] == PackageManager.PERMISSION_GRANTED) {
                            startGame();
                        }
                        else if (ActivityCompat.shouldShowRequestPermissionRationale((Activity) this, p)) {
                            Toast.makeText(this, "쓰기 권한을 허용해 주세요", Toast.LENGTH_SHORT).show();
                        }
                    }
                }

                break;

            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    void startGame()
    {
        startActivity(new Intent(this, MyGame.class));
    }
}
