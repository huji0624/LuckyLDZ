//
//  LDZGameLayer.h
//  LHCocosFirstGame
//
//  Created by huji on 14-11-8.
//
//

#ifndef __LHCocosFirstGame__LDZGameLayer__
#define __LHCocosFirstGame__LDZGameLayer__

#include "cocos2d.h"

class LDZGameLayer : public cocos2d::Layer{
public:
    void initLevel(int level);
    virtual ~LDZGameLayer();
    void update(float delta);
    
    std::function<void()> onGameOver;
    std::function<void()> onGamePass;
private:
    cocos2d::Size _mapSize;
    
    //game
    cocos2d::Sprite* _mainC;
    float _mainCVSpeed;
    cocos2d::Sprite* _upLimit;
    cocos2d::Sprite* _downLimit;
    cocos2d::ProgressTimer* _powerProg;
    
    cocos2d::TMXTiledMap* _map;
    int _mode;
    cocos2d::Vec2 _focusCenter;
    float _focusLen;
    
    void focusOn(cocos2d::Vec2 center,float len,bool animate);
    void popOutMainC();
    void gameEnd(float high);
};

#endif /* defined(__LHCocosFirstGame__LDZGameLayer__) */
