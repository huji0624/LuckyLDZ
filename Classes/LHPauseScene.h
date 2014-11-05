//------.h
#ifndef __LHPAUSESCENE__
#define __LHPAUSESCENE__
#include "cocos2d.h"
class LHPauseScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~LHPauseScene();
	CREATE_FUNC(LHPauseScene);
};
#endif