//------.h
#ifndef __GAMEOVERSCENE__
#define __GAMEOVERSCENE__
#include "cocos2d.h"
class GameOverScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~GameOverScene();
	CREATE_FUNC(GameOverScene);
};
#endif