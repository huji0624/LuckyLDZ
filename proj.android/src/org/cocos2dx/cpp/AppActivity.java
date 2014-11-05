/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import java.util.Locale;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;
import com.google.android.gms.plus.Plus;
import com.umeng.social.CCUMSocialController;

public class AppActivity extends Cocos2dxActivity {

	private AdView mAdView;
	private GoogleApiClient mClient;
	private WindowManager mWm = null;

	public static final int POS_CENTER = 0;
	public static final int POS_TOP = 1;
	public static final int POS_TOP_LEFT = 2;
	public static final int POS_TOP_RIGHT = 3;
	public static final int POS_BOTTOM = 4;
	public static final int POS_BOTTOM_LEFT = 5;
	public static final int POS_BOTTOM_RIGHT = 6;

	public static void addAdView(WindowManager mWm, View adView, int pos) {
		WindowManager.LayoutParams mLayoutParams = new WindowManager.LayoutParams();
		mLayoutParams.type = WindowManager.LayoutParams.TYPE_APPLICATION_PANEL;
		mLayoutParams.width = WindowManager.LayoutParams.WRAP_CONTENT;
		mLayoutParams.height = WindowManager.LayoutParams.WRAP_CONTENT;
		mLayoutParams.flags |= WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

		switch (pos) {
		case POS_CENTER:
			mLayoutParams.gravity = Gravity.CENTER;
			break;
		case POS_TOP:
			mLayoutParams.gravity = Gravity.TOP;
			break;
		case POS_TOP_LEFT:
			mLayoutParams.gravity = Gravity.TOP | Gravity.LEFT;
			break;
		case POS_TOP_RIGHT:
			mLayoutParams.gravity = Gravity.TOP | Gravity.RIGHT;
			break;
		case POS_BOTTOM:
			mLayoutParams.gravity = Gravity.BOTTOM;
			break;
		case POS_BOTTOM_LEFT:
			mLayoutParams.gravity = Gravity.BOTTOM | Gravity.LEFT;
			break;
		case POS_BOTTOM_RIGHT:
			mLayoutParams.gravity = Gravity.BOTTOM | Gravity.RIGHT;
			break;
		default:
			break;
		}
		mWm.addView(adView, mLayoutParams);
	}

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// In the main Activity, assigning that activity as a receiver for C++
		// messages
		AndroidNDKHelper.SetNDKReciever(this);

		mAdView = new AdView(this);
		mAdView.setAdSize(AdSize.BANNER);
		mAdView.setAdUnitId("ca-app-pub-9455502179330810/2117785287");

		// Create an ad request.
		AdRequest.Builder adRequestBuilder = new AdRequest.Builder();
		// Optionally populate the ad request builder.
		adRequestBuilder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
		// Start loading the ad.
		mAdView.loadAd(adRequestBuilder.build());
		
		CCUMSocialController.initSocialSDK(this, "com.umeng.social.share");
		mClient = new GoogleApiClient.Builder(this)
        .addApi(Plus.API)
        .addScope(Plus.SCOPE_PLUS_LOGIN)
        .setAccountName("users.account.name@gmail.com")
        .build();
	}
	
	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
		mClient.connect();
	}
	
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		mClient.disconnect();
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		CCUMSocialController.onActivityResult(requestCode, resultCode, data); 
		super.onActivityResult(requestCode, resultCode, data);
	}

	public void setAd(JSONObject prms) {
		String setad = null;
		try {
			setad = prms.getString("setAd");
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (setad.equals("1")) {
			// showadd
			if (null == mWm) {
				mWm = (WindowManager) this.getSystemService("window");
			}
			addAdView(mWm, mAdView, POS_TOP);
		} else {
			// hidadd
			if (null != mWm) {
				mWm.removeView(mAdView);
			}
		}
	}
	
	private static String mLeaderBoardId = "";
	private static final int REQUEST_LEADERBOARD = 624;
	
	public void leaderBoardControl(JSONObject param){
		if (param.has("value")) {
			try {
				int value = param.getInt("value");
				Games.Leaderboards.submitScore(mClient, mLeaderBoardId, value);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}else{
			startActivityForResult(Games.Leaderboards.getLeaderboardIntent(mClient,
			        mLeaderBoardId), REQUEST_LEADERBOARD);
		}
	}
    
    public void showReviewAlert(JSONObject param){
		if (param.has("title")) {
			try {
				String title = param.getString("title");
                String message = param.getString("message");
                String ok = param.getString("ok");
                String cancel = param.getString("cancel");
                
                AlertDialog.Builder builder = new AlertDialog.Builder(this);
                builder.setMessage(message)
                .setCancelable(false)
                .setPositiveButton(ok, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        AppActivity.this.goReview(null);
                    }
                })
                .setNegativeButton(cancel, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
                AlertDialog alert = builder.create();
				alert.show();
                
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
	}

    
    public void goReview(JSONObject param){
        String appId = "com.luckyhu.game";
        String playurl = "https://play.google.com/store/apps/details?id=" + appId;
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setData(Uri.parse(playurl));
        startActivity(intent);
    }
    
    private String getLan(){
		Locale locale = getResources().getConfiguration().locale;
        String language = locale.getLanguage();
        return language;
	}

	private long mkeyTime = 0;
	public boolean onKeyDown(int keyCode, KeyEvent event) {
	    //二次返回退出
	    if (keyCode == KeyEvent.KEYCODE_BACK) {
	        if ((System.currentTimeMillis() - mkeyTime) > 2000) {
	            mkeyTime = System.currentTimeMillis();
	            String text = null;
	            if (getLan().endsWith("zh")) {
					text = "再按一次退出游戏";
				}else{
					text = "Click Again to exit";
				}
	            Toast.makeText(this, text , Toast.LENGTH_LONG).show();
	        } else {
	            finish();
	            System.exit(0);
	        }
	        return false;
	    }
	    return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
	    glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
	    return glSurfaceView;
	}
}
