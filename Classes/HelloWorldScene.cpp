#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "ThirdPartyHelper.h"
#include "UIButton.h"
#include "DeveloperInfoScence.h"
#include "LHLeaderBoard.h"
#include "UILayout.h"
#include "LHShareButton.h"
#include "UIText.h"
#include "LocalizedString.h"
#include "LHMacros.h"

USING_NS_CC;
USING_NS_UM_SOCIAL;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->initWithColor(Color4B::WHITE);
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    ThirdPartyHelper::setAd(SET_AD_SHOW);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::createWithSpriteFrameName("bg.png");
    bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(bg);
    
//    auto panel = Sprite::create("panel.png");
//    panel->setPosition(bg->getPosition());
//    panel->setScaleX(1.5);
//    panel->setScaleY(2);
//    this->addChild(panel);

    float margin = 40.0f;
    
    auto appname = cocos2d::ui::Text::create(LHLocalizedCString("appname"), Common_Font, 50);
    appname->setColor(Color3B::BLACK);
    appname->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + appname->getContentSize().height/2 + margin + origin.y));
    this->addChild(appname);
    
    auto play = ui::Button::create("play.png");
    play->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 - play->getContentSize().height/2 - margin + origin.y));
    play->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            ThirdPartyHelper::setAd(SET_AD_HID);
            Director::getInstance()->replaceScene(PlayScene::createScene(nullptr));
        }
    });
    this->addChild(play);
    
    auto debt = DeveloperInfo::DevInfoButton("devinfo.png");
    debt->setPosition(Vec2(appname->getPositionX() + appname->getContentSize().width/2 + 40, appname->getPositionY() + appname->getContentSize().height/2 + 20));
    this->addChild(debt);
    
    auto leader = LHLeaderBoard::defaultButton("lb.png");
    leader->setPosition(Vec2(visibleSize.width/3*2 + origin.x, visibleSize.height/4 + origin.y));
    this->addChild(leader);
    
    auto goReview = ui::Button::create("goreview.png");
    goReview->setPosition(Vec2(visibleSize.width/3 + origin.x, visibleSize.height/4 + origin.y));
    goReview->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            ThirdPartyHelper::goReview();
        }
    });
    this->addChild(goReview);
    
    return true;
}
