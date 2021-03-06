#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "LHPauseScene.h"
#include "LocalizedString.h"
#include "LHMacros.h"
#include "ThirdPartyHelper.h"
#include "PlayScene.h"

USING_NS_CC;

#define DESW 640
#define DESH 1136

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{

}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create(LHLocalizedCString("appname"));
        director->setOpenGLView(glview);
    }
#ifdef LHDEBUG
    // turn on display FPS
    director->setDisplayStats(true);
#else
    // turn on display FPS
    director->setDisplayStats(false);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    director->getOpenGLView()->setDesignResolutionSize(DESW, DESH, ResolutionPolicy::NO_BORDER);

    //preload
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ldz.plist");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("pop.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fly.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("pass.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("fail.wav");
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("wiso.wav");
    
#ifdef LHDEBUG
    auto scene = PlayScene::createScene(nullptr);
#else
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
#endif
    
    // run
    director->runWithScene(scene);
    
    ThirdPartyHelper::showReviewAlertWhenOpenTime(5);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    //CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    
//    int sceneTag = Director::getInstance()->getRunningScene()->getTag();
//    if (sceneTag > 0) {
//        Director::getInstance()->pushScene(LHPauseScene::createScene(nullptr));
//    }
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    //CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
