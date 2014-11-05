#include "DeveloperInfoScence.h"
#include "UILayout.h"
#include "LocalizedString.h"
#include "UIText.h"
#include "LHMacros.h"
USING_NS_CC;

Scene* DeveloperInfo::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DeveloperInfo::create();
   
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool DeveloperInfo::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    std::vector<std::string> infos;
    __String *gd = LHLocalizedString("gamedesign");
    infos.push_back(StringUtils::format("%s: huji",gd->getCString()));
    infos.push_back(StringUtils::format("%s: huji",LHLocalizedCString("program")));
    infos.push_back(StringUtils::format("%s: huji",LHLocalizedCString("art")));
    infos.push_back(StringUtils::format("%s: huji0624@gmail.com",LHLocalizedCString("email")));
    
    auto layout = ui::Layout::create();
    layout->setBackGroundColor(Color3B::BLACK);
    layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    layout->setSize(visibleSize);
    this->addChild(layout);
    
    int half = infos.size()/2;
    for (int i =0 ; i<infos.size(); i++) {
        auto info = ui::Text::create(infos[i], Common_Font, 35);
        info->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        info->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + (half-i)*info->getContentSize().height));
        layout->addChild(info);
    }
    
    layout->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->popScene();
        }
    });
    
    return true;
}

ui::Button* DeveloperInfo::DevInfoButton(const std::string &normalImage){
    auto devbt = ui::Button::create(normalImage);
    devbt->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            auto tran = TransitionMoveInB::create(0.3, DeveloperInfo::createScene());
            Director::getInstance()->pushScene(tran);
        }
    });
    return devbt;
}