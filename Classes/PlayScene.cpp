//------.cpp
#include "PlayScene.h"
#include "GameOverScene.h"
#include "LHMacros.h"
#include "UILayout.h"
#include "UIButton.h"
#include "LocalizedString.h"
#include "ThirdPartyHelper.h"
USING_NS_CC;
Scene* PlayScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = PlayScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

PlayScene::~PlayScene(){
}

void PlayScene::update(float delta){
    
}

bool PlayScene::initDict(cocos2d::CCDictionary *dic)
{
    
    _gameLayer = nullptr;
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    leveltext = ui::Text::create("Level", Common_Font, 35);
    leveltext -> setColor(Color3B::BLACK);
    this->addChild(leveltext,1);
    
    initLevel(1);
    
	return true;
}

Node* PlayScene::getPassUI(){
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    auto lo = ui::Layout::create();
    lo->setSize(vs);
    
    float margin = 10;
    
    auto pass =ui::Text::create(LHLocalizedCString("pass"), Common_Font, 50);
    pass->setPosition(Vec2(vs.width/2 , vs.height/2 + pass->getContentSize().height/2 + margin ));
    pass->setColor(Color3B(62,164,57));
    lo->addChild(pass);
    
    auto bt = ui::Button::create("next.png");
    bt->setPosition(Vec2(vs.width/2 , vs.height/2 - bt->getContentSize().height/2 - margin));
    bt->addTouchEventListener([this,lo](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            this->initLevel(_curLevel+1);
            lo->removeFromParent();
        }
    });
    lo->addChild(bt);
    
    
    return lo;
}

void PlayScene::initLevel(int level){
    
    _curLevel = level;
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
    int bestlevel = UserDefault::getInstance()->getIntegerForKey("best",0);
    leveltext->setString(StringUtils::format("%s:%d %s:%d",LHLocalizedCString("level"),(_curLevel),LHLocalizedCString("best"),bestlevel));
    leveltext -> setPosition(Vec2(vs.width/2 + vo.x, vs.height/2+ vo.y));
    leveltext->setScale(2);
    float time = 0.2f;
    DelayTime *ldl = DelayTime::create(1);
    MoveTo *mv = MoveTo::create(time, Vec2(vs.width/2 + vo.x, vs.height - leveltext->getContentSize().height/2 + vo.y));
    ScaleTo *sc = ScaleTo::create(time, 1.0f);
    Spawn *sp = Spawn::create(mv,sc, NULL);
    Sequence *sq = Sequence::create(ldl,sp, NULL);
    leveltext->runAction(sq);
    
    if (_gameLayer!=nullptr) {
        _gameLayer->removeFromParent();
        delete _gameLayer;
    }
    
    _gameLayer = new LDZGameLayer();
    _gameLayer->initLevel(level);
    this->addChild(_gameLayer);
    
    _gameLayer->onGamePass = [this,vs,vo](){
        
        int best = UserDefault::getInstance()->getIntegerForKey("best", 0);
        if (_curLevel>best) {
            best = _curLevel;
            UserDefault::getInstance()->setIntegerForKey("best", best);
            ThirdPartyHelper::uploadScore(best);
        }
        
        auto passui = getPassUI();
        passui->setPosition(Vec2(vs.width + vo.x,  vo.y));
        this->addChild(passui);
        auto mt = MoveTo::create(0.3, Vec2(vo.x,  vo.y));
        passui->runAction(mt);
    };
    _gameLayer->onGameOver = [this,vs,vo](bool r){
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("fail.wav");
        
        auto fail = ui::Text::create("", Common_Font, 50);
        if (r) {
            fail->setString(LHLocalizedCString("noenough"));
        }else{
            fail->setString(LHLocalizedCString("toomuch"));
        }
        fail->setPosition(Vec2(vs.width*2 + vo.x, vs.height/2 + vo.y));
        fail->setColor(Color3B::RED);
        this->addChild(fail);
        
        auto mt = MoveTo::create(0.3, Vec2(vs.width/2 + vo.x, vs.height/2 + vo.y));
        auto dl2 = DelayTime::create(1.5);
        CallFunc *call = CallFunc::create([this](){
            auto dict = Dictionary::create();
            dict->setObject(CCInteger::create(_curLevel), "score");
            Director::getInstance()->replaceScene(GameOverScene::createScene(dict));
        });
        auto sq = Sequence::create(mt,dl2,call, NULL);
        fail->runAction(sq);
    };
}