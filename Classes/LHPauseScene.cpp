//------.cpp
#include "LHPauseScene.h"
#include "UIButton.h"
USING_NS_CC;
Scene* LHPauseScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = LHPauseScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

LHPauseScene::~LHPauseScene(){
}
bool LHPauseScene::initDict(cocos2d::CCDictionary *dic)
{
    Size vs = Director::getInstance()->getVisibleSize();
    auto bt = ui::Button::create("play.png");
    bt->setPosition(Vec2(vs.width/2, vs.height/2));
    bt->addTouchEventListener([](Ref*,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->popScene();
        }
    });
    this->addChild(bt,1);
    log("Game Pause");
	return true;
}