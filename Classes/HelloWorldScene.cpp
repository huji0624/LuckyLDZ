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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto appname = cocos2d::ui::Text::create(LHLocalizedCString("appname"), Common_Font, 40);
    appname->setColor(Color3B::BLACK);
    appname->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(appname);
    
    auto play = ui::Button::create("play.png");
    play->setPosition(Vec2(visibleSize.width/2, visibleSize.height/3));
    play->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(PlayScene::createScene(nullptr));
        }
    });
    this->addChild(play);
    
    return true;
}
