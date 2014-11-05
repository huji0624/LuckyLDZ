//------.cpp
#include "GameOverScene.h"
USING_NS_CC;
Scene* GameOverScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

GameOverScene::~GameOverScene(){
}
bool GameOverScene::initDict(cocos2d::CCDictionary *dic)
{
    
	return true;
}