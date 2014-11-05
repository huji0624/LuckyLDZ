#ifndef __DEVELOPERINFO_SCENE_H__
#define __DEVELOPERINFO_SCENE_H__

#include "cocos2d.h"
#include "UIButton.h"

class DeveloperInfo : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    static cocos2d::ui::Button* DevInfoButton(const std::string &normalImage);
    
    // implement the "static create()" method manually
    CREATE_FUNC(DeveloperInfo);
};

#endif // __DEVELOPERINFO_SCENE_H__
