//
//  LHShareButton.h
//  LHCocosFirstGame
//
//  Created by huji on 14-7-30.
//
//

#ifndef __LHCocosFirstGame__LHShareButton__
#define __LHCocosFirstGame__LHShareButton__

#include <iostream>
#include "UIButton.h"
#include "cocos2d.h"
#include "CCUMSocialSDK.h"

class LHShareButton : public cocos2d::ui::Button{
public:
    LHShareButton();
    
    static LHShareButton* create(const char *normalImage,
                                 const char *selectedImage, const char* umAppKey,
                                 umeng::social::ShareEventHandler callback);
    
    static LHShareButton* defaultButton(const char *normal,const char* shareText);
    /**
	 * 设置分享回调回调
	 * @param callback 分享回调
	 */
	void setShareCallback(umeng::social::ShareEventHandler callback);
    
    /**
	 * 设置友盟SDK支持的平台，分享面板上也会显示这些平板，如果这些平台没有添加相应的依赖则不会显示。平台的申明参考CCUMTypeDef.h中的Platform枚举
	 * @param platforms 友盟SDK支持的平台
	 */
	void setPlatforms(vector<int>* platforms);
    
    /**
	 * 设置要分享的文本内容
	 * @param text 要分享的文本内容
	 */
	void setShareContent(const char* text);
    
    CREATE_FUNC(LHShareButton);
    
private:
    // 友盟分享组件SDK
	umeng::social::CCUMSocialSDK* mSocialSDK;
    // 分享回调
	umeng::social::ShareEventHandler mCallback;
    
    void initWithAppKey(const char* umAppKey);
    
    std::string mShareText;
};

#endif /* defined(__LHCocosFirstGame__LHShareButton__) */
