package org.cocos2dx.cpp;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.lhgame.luckypuzzle.R;

public class LHLeaderBoardActivity {

	private TextView mMyValue;
	private ListView mListView;
	private AlertDialog mAlert;

	private Context mContext;

	public LHLeaderBoardActivity(Context context) {
		mContext = context;
	}
	
	public void popSetUser(){
		String message = null;
		String ok = null;
		String cancel = null;
		if (isChinese()) {
			message = "设置您的排行榜用户名";
			ok = "确定";
			cancel = "取消";
		}else{
			message = "Set your user name in LeaderBoard";
			ok = "ok";
			cancel = "cancel";
		}
		
		final EditText et = new EditText(mContext);		
		
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
        builder.setMessage(message)
        .setView(et)
        .setPositiveButton(ok, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
            	final String username = et.getText().toString();
            	if (username.length() == 0) {
					return;
				}
            	String url = LHRequest.urlWithGetPair(LHRequest.LHLDURL, LHRequest.SETUSER,username,LHRequest.DEVICE,LHRequest.getDeviceID(mContext));
            	LHRequest request = new LHRequest(url);
    			request.requestWitUrl(new LHRequestHandler() {
					
					@Override
					public void response(int requestcode, JSONObject obj) {
						// TODO Auto-generated method stub
						if (requestcode == LHRequestHandler.REQUEST_OK) {
							try {
								if (obj.getString("res").equals("ok")) {
									setUser(username);
								}
							} catch (JSONException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
						}
					}
				});
            }
        })
        .setNegativeButton(cancel, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.cancel();
            }
        });
        AlertDialog alert = builder.create();
		alert.show();
	}
	
	public void setUser(String user){
		SharedPreferences sp = mContext.getSharedPreferences("lhgame", Context.MODE_WORLD_READABLE);
		Editor ed = sp.edit();
		ed.putString("user", user);
		ed.commit();
	}

	public void showLeaderBoard() {
		// TODO Auto-generated method stub
		
		View view=LayoutInflater.from(mContext).inflate(R.layout.lhleaderboard, null);

		mMyValue = (TextView) view.findViewById(R.id.myrank);
		mListView = (ListView) view.findViewById(R.id.toprank);

		String url = LHRequest.urlWithGetPair(LHRequest.LHLDURL,
				LHRequest.DEVICE, LHRequest.getDeviceID(mContext), LHRequest.USER,
				LHRequest.getUser(mContext),LHRequest.GAME,LHRequest.getGame(mContext));
		LHRequest request = new LHRequest(url);
		request.requestWitUrl(new LHRequestHandler() {

			@Override
			public void response(int requestcode, JSONObject obj) {
				// TODO Auto-generated method stub
				if (requestcode == LHRequestHandler.REQUEST_OK) {
					try {
						JSONObject my = obj.getJSONObject("my");
						String myhigh = my.getString("high");
						String myrank = my.getString("rank");
						if (myhigh.equals("null")) {
							mMyValue.setText("--");
						}else{
							String mystring = null;
							if (isChinese()) {
								mystring = formatHigh("我", myhigh, myrank);
							} else {
								mystring = formatHigh("Me", myhigh, myrank);
							}
							mMyValue.setText(mystring);
						}

						// allrank
						JSONArray list = obj.getJSONArray("list");
						ArrayList<HashMap<String, Object>> listItem = new ArrayList<HashMap<String, Object>>();
						for (int i = 0; i < list.length(); i++) {
							JSONObject tmpobj = list.getJSONObject(i);
							String tmpuser = tmpobj.getString(LHRequest.USER);
							String tmphigh = tmpobj.getString(LHRequest.HIGH);
							String tmprank = "" + (i+1);
							HashMap<String, Object> map = new HashMap<String, Object>();
							map.put("text",
									formatHigh(tmpuser, tmphigh, tmprank));
							listItem.add(map);
						}
						SimpleAdapter adp = new SimpleAdapter(
								mContext, listItem,
								R.layout.lhld_item, new String[] { "text" },
								new int[] { R.id.lhld });
						mListView.setAdapter(adp);
					} catch (JSONException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		});
		
		AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
        builder.setView(view);
        AlertDialog alert = builder.create();
        alert.show();
        mAlert = alert;
	}
	
	public boolean isShow(){
		return mAlert.isShowing();
	}
	
	public void dismiss(){
		mAlert.dismiss();
	}

	private String formatHigh(String user, String high, String rank) {
		StringBuilder sb = new StringBuilder();
		sb.append(user);
		if (isChinese()) {
			sb.append(" 排名: ");
			sb.append(rank);
			sb.append(" , 成绩: ");
			sb.append(high);
		} else {
			sb.append(" rank: ");
			sb.append(rank);
			sb.append(" result: ");
			sb.append(high);
		}
		return sb.toString();
	}

	private boolean isChinese() {
		return getLan().endsWith("zh");
	}

	private String getLan() {
		Locale locale = mContext.getResources().getConfiguration().locale;
		String language = locale.getLanguage();
		return language;
	}
}
