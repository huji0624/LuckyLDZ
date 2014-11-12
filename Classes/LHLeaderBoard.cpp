//
//  LHLeaderBoard.cpp
//  LHCocosFirstGame
//
//  Created by huji on 7/30/14.
//
//

#include "LHLeaderBoard.h"
#include "ThirdPartyHelper.h"

cocos2d::ui::Button* LHLeaderBoard::defaultButton(const std::string &normal){
    auto bt = cocos2d::ui::Button::create(normal);
    bt->addTouchEventListener([](cocos2d::Ref* ps,cocos2d::ui::Widget::TouchEventType type){
        if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
            ThirdPartyHelper::showLeaderBoard();
        }
    });
    return bt;
}