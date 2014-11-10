//------.h
#ifndef __PLAYSCENE__
#define __PLAYSCENE__
#include "cocos2d.h"
#include "LDZGameLayer.h"
#include "UIText.h"
class PlayScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~PlayScene();
    virtual void update(float delta);
	CREATE_FUNC(PlayScene);
private:
    LDZGameLayer* _gameLayer;
    cocos2d::ui::Text* leveltext;
    int _curLevel;
    
    //func
    void initLevel(int level);
    cocos2d::Node* getPassUI();
};
#endif