//------.cpp
#include "GuideScene.h"
#include "LHMacros.h"
#include "UILayout.h"
USING_NS_CC;
Scene* GuideScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = GuideScene::create();
	layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

GuideScene::~GuideScene(){
}


ui::Button* GuideScene::guideButton(const std::string &name){
    auto bt = ui::Button::create(name);
    bt->addTouchEventListener([](Ref *rf,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            auto tran = TransitionSlideInR::create(0.3, GuideScene::createScene(nullptr));
            Director::getInstance()->pushScene(tran);
        }
    
    });
    bt->setPosition(Vec2(100, 200));
    return bt;
}

bool GuideScene::initDict(cocos2d::CCDictionary *dic)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto layout = ui::Layout::create();
    layout->setBackGroundColor(Color3B::WHITE);
    layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    layout->setSize(visibleSize);
    layout->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->popScene();
        }
    });
    this->addChild(layout);
    
	return true;
}
