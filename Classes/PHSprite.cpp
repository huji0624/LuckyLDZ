//
//  PHSprite.cpp
//  LHCocosFirstGame
//
//  Created by huji on 14-10-27.
//
//

#include "PHSprite.h"
#include "UIText.h"
#include "LHMacros.h"
USING_NS_CC;

PHSprite* PHSprite::createWithName(const std::string& name,cocos2d::Size size){
    PHSprite* sp = PHSprite::create("phsprite.png");
    Size cs = sp->getContentSize();
    float xs = size.width/cs.width;
    float ys = size.height/cs.height;
    sp->setScale(xs, ys);

    auto text = ui::Text::create(name, Common_Font, 20);
    text->setPosition(Vec2(cs.width/2, cs.height/2));
    text->setColor(Color3B::RED);
    sp->addChild(text);
    text->setTouchEnabled(false);
    return sp;
}

PHSprite* PHSprite::create(const std::string& filename){
    PHSprite *sprite = new (std::nothrow) PHSprite();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}