//
//  LHLeaderBoard.h
//  LHCocosFirstGame
//
//  Created by huji on 7/30/14.
//
//

#ifndef __LHCocosFirstGame__LHLeaderBoard__
#define __LHCocosFirstGame__LHLeaderBoard__

#include <iostream>
#include "cocos2d.h"
#include "UIButton.h"

class LHLeaderBoard{
public:
    static cocos2d::ui::Button* defaultButton(const std::string &normal);
};

#endif /* defined(__LHCocosFirstGame__LHLeaderBoard__) */
