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
    leveltext -> setPosition(Vec2(vs.width/2 + vo.x, vs.height - leveltext->getContentSize().height/2 + vo.y));
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
    
    auto pass =ui::Text::create("Big Cong!", Common_Font, 50);
    pass->setPosition(Vec2(vs.width/2 , vs.height/3*2 ));
    lo->addChild(pass);
    
    auto bt = ui::Button::create("rb.png");
    bt->setTitleText(LHLocalizedCString("next"));
    bt->setPosition(Vec2(vs.width/2 , vs.height/3 ));
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
    int bestlevel = UserDefault::getInstance()->getIntegerForKey("best",0);
    leveltext->setString(StringUtils::format("Level-%d Best:%d",(_curLevel),bestlevel));
    
    Size vs = Director::getInstance()->getVisibleSize();
    Vec2 vo = Director::getInstance()->getVisibleOrigin();
    
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
    _gameLayer->onGameOver = [this,vs,vo](){
        auto fail = ui::Text::create("You Fail!", Common_Font, 50);
        fail->setPosition(Vec2(vs.width*2 + vo.x, vs.height/2 + vo.y));
        this->addChild(fail);
        
        auto dl1 = DelayTime::create(1);
        auto mt = MoveTo::create(0.3, Vec2(vs.width/2 + vo.x, vs.height/2 + vo.y));
        auto dl2 = DelayTime::create(2);
        CallFunc *call = CallFunc::create([this](){
            auto dict = Dictionary::create();
            dict->setObject(CCInteger::create(_scoreValue), "score");
            auto tran = TransitionSlideInR::create(0.4, GameOverScene::createScene(dict));
            Director::getInstance()->replaceScene(tran);
        });
        auto sq = Sequence::create(dl1,mt,dl2,call, NULL);
        fail->runAction(sq);
    };
}