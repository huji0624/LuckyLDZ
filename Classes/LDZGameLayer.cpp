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

#define ANI_TIME 0.3f

#define MODE_BEGAIN 4
#define MODE_VIEW 0
#define MODE_POWER 1
#define MODE_FLY 2
#define MODE_END 3

#define GAS 9.8f
#define MOVE_FACTOR 12.0f
#define DECRE_FACTOR 2.0f

LDZGameLayer::~LDZGameLayer(){
    this->getEventDispatcher()->removeEventListenersForTarget(this);
}

void LDZGameLayer::initLevel(int level){
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    _mode = MODE_BEGAIN;
    
    TMXTiledMap *map = TMXTiledMap::create("back.tmx");
    _map = map;
    _mapSize = Size(map->getMapSize().width * map->getTileSize().width , map->getMapSize().height * map->getTileSize().height);
    map->setPosition(Vec2(0, 0));
    this->addChild(map);
    
    //main char
    _mainC = Sprite::createWithSpriteFrameName("1.png");
    _mainC->setPosition(_mapSize.width/2, _mainC->getContentSize().height/2 + 10);
    this->addChild(_mainC);
    
    //progress
    auto progress = ProgressTimer::create(Sprite::createWithSpriteFrameName("tempb.png"));
    float progx = _mapSize.width/2 - _mainC->getContentSize().width/2 - progress->getContentSize().width/2 - 5;
    float progy = _mainC->getContentSize().height/2 + 10;
    progress->setPosition( progx, progy);
    progress->setType(cocos2d::ProgressTimer::Type::BAR);
    progress->setBarChangeRate(Vec2(0, 1));
    progress->setMidpoint(Vec2(0, 0));
    auto progback = Sprite::createWithSpriteFrameName("temp.png");
    progback->setPosition(progx, progy);
    this->addChild(progback);
    this->addChild(progress,1);
    progress->setVisible(false);
    progback->setVisible(false);
    _powerProg = progress;
    
    //limit
    float height = 400/level*1.5;
    float uy = level*150+800;
    float dy = uy - height;
    _upLimit = Sprite::createWithSpriteFrameName("limit.png");
    _upLimit->setPosition(_mapSize.width/2, uy);
    this->addChild(_upLimit);
    _downLimit = Sprite::createWithSpriteFrameName("limit.png");
    _downLimit->setPosition(_mapSize.width/2, dy);
    this->addChild(_downLimit);
    
    //guide
    auto gt = ui::Text::create(LHLocalizedCString("guidetext"), Common_Font, 35);
    gt->setColor(Color3B::RED);
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
                float focusMargin = 20;
                this->focusOn(_mainC->getPosition(), _mainC->getContentSize().height + focusMargin, true);
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
            float toy = _focusCenter.y + (last.y - cu.y);
            if (toy+_focusLen/2 > _upLimit->getPositionY() + 20) {
                toy = _upLimit->getPositionY()+20 - _focusLen/2;
            }
            this->focusOn(Vec2(_focusCenter.x , toy),_focusLen, false);
        }else if (_mode == MODE_POWER){
            float dy = fabsf(cu.y - last.y);
            _powerProg->setPercentage(_powerProg->getPercentage()+dy/(_powerProg->getPercentage()+MOVE_FACTOR));
        }else if (_mode == MODE_FLY){
            this->update(0.1f);
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
    auto dl2 = DelayTime::create(1);
    CallFunc *call = CallFunc::create([this](){
        auto tmpcall = CallFunc::create([this](){
            _mode = MODE_VIEW;
        });
        this->focusOn(_mainC->getPosition(), _focusLen, true , tmpcall);
    });
    auto sq = Sequence::create(dl2,call, NULL);
    this->runAction(sq);
}

void LDZGameLayer::popOutMainC(){
    _mainCVSpeed = 3 * _powerProg->getPercentage();
}

void LDZGameLayer::update(float delta){
    if (_mode == MODE_FLY || _mode == MODE_END) {
        float dy = _mainCVSpeed*delta;
        _mainC->setPosition(_mainC->getPositionX(), _mainC->getPositionY()+dy);
        _mainCVSpeed -= GAS*delta;
        
        if (_mode == MODE_FLY) {
            if (_mainC->getPositionY()<_upLimit->getPositionY()) {
                Size vs = Director::getInstance()->getVisibleSize();
//                float focusY = (_upLimit->getPositionY() + _mainC->getBoundingBox().getMinY())/2;
                float focusL = _upLimit->getPositionY()+20 - _mainC->getBoundingBox().getMinY();
                if (focusL < vs.width) {
                    focusOn(_mainC->getPosition(), focusL, false);
                }else{
                    focusOn(_mainC->getPosition(), vs.width, false);
                }
            }else{
                focusOn(Vec2(_mainC->getPositionX(),_mainC->getPositionY()), _focusLen, false);
            }
        }
        
        if (_mode == MODE_FLY) {
            if (_mainCVSpeed<=0) {
                _mode = MODE_END;
                this->gameEnd(_mainC->getPositionY());
            }else if(_mainC->getPositionY()>(_upLimit->getPositionY()+150)){
                _mode = MODE_END;
                this->gameEnd(-1);
            }
            
        }
    }else if(_mode == MODE_POWER){
        float toper = _powerProg->getPercentage() - _powerProg->getPercentage() * delta / DECRE_FACTOR;
        _powerProg->setPercentage(toper);
        int intper = toper/2+1;
        if (intper>50) {
            intper=50;
        }
        _mainC->setSpriteFrame(StringUtils::format("%d.png",(intper)));
    }
}

void LDZGameLayer::gameEnd(float high){
    if (high>0) {
        Sprite* mark = Sprite::createWithSpriteFrameName("endline.png");
        mark->setPosition(_mapSize.width/2, high);
        this->addChild(mark);
    }
    
    if (_mainC->getPositionY() > _upLimit->getPositionY() ) {
        onGameOver(false);
    }else if( _mainC->getPositionY() < _downLimit->getPositionY()){
        onGameOver(true);
    }else{
        onGamePass();
    }
}

void LDZGameLayer::focusOn(cocos2d::Vec2 center, float len, bool animate, cocos2d::CallFunc *complete){
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    float toscale = 1.0f;
    if (len<vs.width) {
        toscale = vs.width/len;
    }else if(len>vs.height){
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
        if (complete==nullptr) {
            this->runAction(conc);
        }else{
            auto sq = Sequence::create(conc,complete, NULL);
            this->runAction(sq);
        }
    }else{
        this->setPosition(tox, toy);
        this->setScale(toscale);
    }
    
    _focusCenter = Vec2(center.x, (sccenter.y - toy)/toscale);
    _focusLen = len;
}

void LDZGameLayer::focusOn(cocos2d::Vec2 center, float len, bool animate){
    this->focusOn(center, len, animate, nullptr);
}