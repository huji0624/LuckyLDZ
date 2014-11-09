package org.cocos2dx.cpp;

import org.json.JSONObject;

public interface LHRequestHandler {
	public static final int REQUEST_OK = 0;
	public static final int REQUEST_FAIL = 1;
	public static final int REQUEST_CANCEL = 2;
	public void response(int requestcode,JSONObject obj);
}
