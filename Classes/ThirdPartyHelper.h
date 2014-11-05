//
//  ThirdPartyHelper.h
//  LHCocosFirstGame
//
//  Created by huji on 6/19/14.
//
//

#ifndef __LHCocosFirstGame__ThirdPartyHelper__
#define __LHCocosFirstGame__ThirdPartyHelper__

#include <iostream>

#define SET_AD_SHOW 1
#define SET_AD_HID 0

class ThirdPartyHelper{
public:
    //ad
    static void setAd(int tag);
    
    //leaderboard
    static void uploadScore(int score);
    static void showLeaderBoard();
    
    //review
    static void showReviewAlertWhenOpenTime(int opentime);
    static void showReviewAlert();
    static void goReview();
};

#endif /* defined(__LHCocosFirstGame__ThirdPartyHelper__) */
