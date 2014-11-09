//
//  LDZGameLayer.cpp
//  LHCocosFirstGame
//
//  Created by huji on 14-11-8.
//
//

#include "LDZGameLayer.h"
#include "PHSprite.h"
#include "UIText.h"
#include "LocalizedString.h"
#include "LHMacros.h"
USING_NS_CC;

#define ANI_TIME 0.2f

#define MODE_VIEW 0
#define MODE_POWER 1
#define MODE_FLY 2
#define MODE_END 3

#define GAS 9.8f
#define MOVE_FACTOR 12.0f

LDZGameLayer::~LDZGameLayer(){
    this->getEventDispatcher()->removeEventListenersForTarget(this);
}

void LDZGameLayer::initLevel(int level){
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    _mode = MODE_VIEW;
    
    TMXTiledMap *map = TMXTiledMap::create("back.tmx");
    _map = map;
    _mapSize = Size(map->getMapSize().width * map->getTileSize().width , map->getMapSize().height * map->getTileSize().height);
    map->setPosition(Vec2(0, 0));
    this->addChild(map);
    
    //main char
    _mainC = (Sprite*)PHSP("香蕉");
    _mainC->setPosition(_mapSize.width/2, _mainC->getContentSize().height/2 + 10);
    this->addChild(_mainC);
    
    //progress
    float progx = _mapSize.width/2;
    float progy = _mainC->getContentSize().height/2 + 30;
    auto progress = ProgressTimer::create(Sprite::create("rr.png"));
    progress->setPosition( progx, progy);
    progress->setType(cocos2d::ProgressTimer::Type::BAR);
    progress->setBarChangeRate(Vec2(0, 1));
    progress->setMidpoint(Vec2(0, 0));
    auto progback = Sprite::create("rb.png");
    progback->setPosition(progx, progy);
    this->addChild(progback);
    this->addChild(progress,1);
    progress->setVisible(false);
    progback->setVisible(false);
    _powerProg = progress;
    
    //limit
    float uy = 800;
    float dy = 650;
    _upLimit = (Sprite*)PHSP("UP");
    _upLimit->setPosition(_mapSize.width/2, uy);
    this->addChild(_upLimit);
    _downLimit = (Sprite*)PHSP("DOWN");
    _downLimit->setPosition(_mapSize.width/2, dy);
    this->addChild(_downLimit);
    
    //guide
    auto gt = ui::Text::create(LHLocalizedCString("guidetext"), Common_Font, 35);
    gt->setColor(Color3B(250, 201, 11));
    gt->setPosition(Vec2(_mainC->getPosition().x , _mainC->getBoundingBox().getMaxY() + gt->getContentSize().height/2));
    this->addChild(gt);
    auto bl = Blink::create(1, 1);
    auto rp = RepeatForever::create(bl);
    gt->runAction(rp);
    
    auto lis = EventListenerTouchOneByOne::create();
    lis->onTouchBegan = [this,gt,progress,progback](Touch* tmpTouch, Event*){
        if (_mode == MODE_VIEW) {
            Vec2 loca = tmpTouch->getLocation();
            loca = this->convertToNodeSpace(loca);
            if (_mainC->getBoundingBox().containsPoint(loca)) {
                float focusMargin = 80;
                this->focusOn(_mainC->getPosition(), _mainC->getContentSize().width + focusMargin, true);
                _mode = MODE_POWER;
                
                gt->stopAllActions();
                gt->removeFromParent();
                
                progress->setVisible(true);
                progback->setVisible(true);
                progress->setPercentage(0);
                
                this->scheduleUpdate();
            }
        }
        
        return true;
    };
    lis->onTouchMoved = [this](Touch* tmpTouch, Event*){
        Vec2 cu = tmpTouch->getLocation();
        Vec2 last = tmpTouch->getPreviousLocation();
        if (_mode == MODE_VIEW) {
            this->focusOn(Vec2(_focusCenter.x , _focusCenter.y + (last.y - cu.y)),_focusLen, false);
        }else if (_mode == MODE_POWER){
            float dy = fabsf(cu.y - last.y);
            _powerProg->setPercentage(_powerProg->getPercentage()+dy/(_powerProg->getPercentage()+MOVE_FACTOR));
        }
    };
    lis->onTouchEnded = [this,progback,progress](Touch*, Event*){
        if (_mode == MODE_POWER) {
            this->popOutMainC();
            _mode = MODE_FLY;
            
            progress->setVisible(false);
            progback->setVisible(false);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis, this);
    
    focusOn(Vec2(_mapSize.width/2,(uy+dy)/2), vs.width, false);
}

void LDZGameLayer::popOutMainC(){
    _mainCVSpeed = 3 * _powerProg->getPercentage();
}

void LDZGameLayer::update(float delta){
    if (_mode == MODE_FLY || _mode == MODE_END) {
        float dy = _mainCVSpeed*delta;
        _mainC->setPosition(_mainC->getPositionX(), _mainC->getPositionY()+dy);
        
        if (_mode == MODE_FLY) {
            Size vs = Director::getInstance()->getVisibleSize();
            focusOn(Vec2(_mainC->getPositionX(),_mainC->getPositionY()), vs.width, false);
        }
        
        _mainCVSpeed -= GAS*delta;
        
        if (_mainCVSpeed<=0 && _mode == MODE_FLY) {
            _mode = MODE_END;
            this->gameEnd(_mainC->getPositionY());
        }
    }else if(_mode == MODE_POWER){
        float toper = _powerProg->getPercentage() - _powerProg->getPercentage() * delta;
        _powerProg->setPercentage(toper);
    }
}

void LDZGameLayer::gameEnd(float high){
    PHSprite* mark = PHSprite::createWithName("-", Size(500, 50));
    mark->setPosition(_mapSize.width/2, high);
    this->addChild(mark);
    
    if (high <= _upLimit->getPositionY() && high >= _downLimit->getPositionY()) {
        onGamePass();
    }else{
        onGameOver();
    }
}

void LDZGameLayer::focusOn(cocos2d::Vec2 center, float len, bool animate){
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    float toscale = 1.0f;
    if (len<vs.height) {
        toscale = vs.width/len;
    }else{
        toscale = vs.height/len;
    }
    
    Vec2 sccenter = Vec2(vs.width/2, vs.height/2 + vo.y);
    
    float tox = sccenter.x - center.x*toscale;
    float toy = sccenter.y - center.y*toscale;
    
    if (toy>vo.y) {
        toy = vo.y;
    }else if (toy<vs.height-toscale*_mapSize.height+vo.y){
        toy = vs.height-toscale*_mapSize.height+vo.y;
    }
    
    if (animate) {
        auto scale = ScaleTo::create(ANI_TIME, toscale);
        auto mv = MoveTo::create(ANI_TIME, Vec2(tox, toy));
        auto conc = Spawn::create(scale,mv, NULL);
        this->runAction(conc);
    }else{
        this->setPosition(tox, toy);
        this->setScale(toscale);
    }
    
    _focusCenter = Vec2(center.x, (sccenter.y - toy)/toscale);
    _focusLen = len;
}