#include "AppDelegate.h"
#include "Scenes/TitleScene.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;

//#include "GameState.h"
//#include "Scenes/LevelTransition.h"
//#include "Scenes/GameoverScene.h"

using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    //SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
    //SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    
    std::vector<std::string> searchPaths;
    searchPaths.push_back("main");
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    // initialize director
    Director* director = Director::getInstance();
    EGLView* eglView = EGLView::getInstance();

    director->setOpenGLView(eglView);
	
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    Scene *scene = TitleScene::scene();
    /*
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    GameState::getInstance()->addActTime(1);
    Scene *scene = GameoverScene::scene();
     */
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
