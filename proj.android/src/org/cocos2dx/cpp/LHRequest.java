package org.cocos2dx.cpp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.SharedPreferences;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.util.Log;

public class LHRequest {
	
	public static final String LHLDURL = "http://lhgameleaderboard.sinaapp.com/";
	
	public static final String DEVICE = "device";
	public static final String HIGH = "high";
	public static final String USER = "user";
	public static final String VT = "vt";
	public static final String GAME = "game";
	public static final String SETUSER = "setuser"; 
			
	private static final String VT_B = "b";//保存最大值
	private static final String VT_S = "s";//保存最小值
	
	public static final String VT_V = VT_B;
	
	private String mUrl;
	private LHRequestHandler mRequestHandler;
	private Thread mThread;
	private Handler mHandler = new Handler(){
		public void handleMessage(Message msg) {
			if (msg==null) {
				mRequestHandler.response(LHRequestHandler.REQUEST_FAIL, null);
				return;
			}
			
			JSONObject jo = null;
			if (msg.obj!=null) {
				jo = (JSONObject)msg.obj;
			}
			mRequestHandler.response(msg.what, jo);
		};
	};

	public LHRequest(String url){
		mUrl = url;
	}
	
	public void requestWitUrl(LHRequestHandler rh){
		
		mRequestHandler = rh;
		mThread = new Thread(){
			@Override
			public void run() {
				// TODO Auto-generated method stub
				super.run();
				Message msg = new Message();
				HttpClient client = new DefaultHttpClient();
				HttpGet get = new HttpGet(mUrl);
				Log.d("lhgame", "url->" + mUrl);
				try {
					HttpResponse response = client.execute(get);
					HttpEntity entity = response.getEntity();
					if (entity != null) {
					    InputStream instream = entity.getContent();
					    String resStr = readStream(instream);
					    Log.d("lhgame", "res->" + resStr);
					    JSONObject js = new JSONObject(resStr);
					    msg.obj = js;
					    msg.what = LHRequestHandler.REQUEST_OK;
					    mHandler.sendMessage(msg);
					    return;
					}
				} catch (ClientProtocolException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (JSONException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				msg.what = LHRequestHandler.REQUEST_FAIL;
				mHandler.sendMessage(msg);
				return;
			}
		};
		mThread.start();
	}
	
	public void cancel(){
		mThread.stop();
		mHandler.sendEmptyMessage(LHRequestHandler.REQUEST_CANCEL);
	}
	
	private String readStream(InputStream in) throws IOException {
		StringBuilder sb = new StringBuilder();
		BufferedReader r = new BufferedReader(new InputStreamReader(in), 1000);
		for (String line = r.readLine(); line != null; line = r.readLine()) {
			sb.append(line);
		}
		in.close();
		return sb.toString();
	}
	
	public static String urlWithGetPair(String url,String...getpair){
		StringBuilder sb = new StringBuilder();
		sb.append(url);
		sb.append("?");
		int i=0;
		for (String string : getpair) {
			if (i%2==0) {
				sb.append(string);
				sb.append("=");
			}else{
				sb.append(string);
				sb.append("&");
			}
			i++;
		}
		
		sb.append(VT);
		sb.append("=");
		sb.append(VT_V);
		
		return sb.toString();
	}
	
	public static String getDeviceID(Context context){
		String did = null;
		
		TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
		did = tm.getDeviceId();
		
		if (isEmpty(did)) {
			WifiManager wifi = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
			WifiInfo info = wifi.getConnectionInfo();
			String wifiMac = info.getMacAddress();
			did = wifiMac;
		}else{
			return did;
		}
		
		if (isEmpty(did)) {
			String seri = tm.getSimSerialNumber();
			did = seri;
		}else{
			return did;
		}
		
		if (isEmpty(did)) {
			return null;
		}
		
		return did;
	}
	
	public static boolean isEmpty(String did){
		if (did==null) {
			return true;
		}
		
		if (did.trim().length()==0) {
			return true;
		}
		
		return false;
	}
	
	public static String getUser(Context context){
		SharedPreferences sp = context.getSharedPreferences("lhgame", Context.MODE_WORLD_READABLE);
		return sp.getString("user", null);
	}
	
	public static String getGame(Context context){
		return context.getPackageName();
	}
}
