//
//  PHSprite.h
//  LHCocosFirstGame
//
//  Created by huji on 14-10-27.
//
//

#ifndef __LHCocosFirstGame__PHSprite__
#define __LHCocosFirstGame__PHSprite__

#include "cocos2d.h"

#define PHSP(name) PHSprite::createWithName(name,cocos2d::Size(64,64));

class PHSprite : public cocos2d::Sprite{
public:
    static PHSprite* createWithName(const std::string& name,cocos2d::Size size);
    
    static PHSprite* create(const std::string& filename);
};

#endif /* defined(__LHCocosFirstGame__PHSprite__) */
