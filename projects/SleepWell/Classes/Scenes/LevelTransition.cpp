#include "LevelTransition.h"
#include "SimpleAudioEngine.h"
#include "../GameState.h"
#include "../GamePlay.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* LevelTransition::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LevelTransition::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool LevelTransition::init()  {
	
	if ( !BaseLayer::init() )
    {
        return false;
    }
    
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_breathing.wav");
    
    auto winSize = Director::getInstance()->getWinSize();
    int level = GameState::getInstance()->getActLevel();
    int totalTime = GameState::getInstance()->getTotalTime();
    int hours = GameState::getInstance()->getHours();
    int minutes = GameState::getInstance()->getMinutes();
    int seconds = GameState::getInstance()->getSeconds();
    
    seconds += totalTime + (level - 1) * kSecondsPerLevel;
    
    int remainingSeconds = seconds % 60;
    
    minutes += (seconds - remainingSeconds) / 60.0f;
    
    if (minutes >= 60) {
        minutes -= 60;
        hours += 1;
    }
    
    auto timeText = String::createWithFormat("%i:%02i:%02i AM", hours, minutes, remainingSeconds)->getCString();
    
    auto levelText = String::createWithFormat("Suspension of breathing\n%d / %i", level, kMaxLevel)->getCString();
    
	auto timeLabel = LabelBMFont::create(timeText, "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
    timeLabel->getTexture()->setAliasTexParameters();
	timeLabel->setAnchorPoint(Point(0.5f, 0.5f));
	timeLabel->setPosition(Point(winSize.width / 2, winSize.height * 0.8f));
    timeLabel->setColor(Color3B(40, 80, 160));
    
	auto levelLabel = LabelBMFont::create(levelText, "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
    levelLabel->getTexture()->setAliasTexParameters();
	levelLabel->setAnchorPoint(Point(0.5f, 0.5f));
	levelLabel->setPosition(Point(winSize.width / 2, winSize.height / 2));
    
	this->addChild(timeLabel);
	this->addChild(levelLabel);
    
    SimpleAudioEngine::getInstance()->playEffect("sfx_breathing.wav");
    
	return true;
}

void LevelTransition::onEnterTransitionDidFinish() {
    BaseLayer::onEnterTransitionDidFinish();
    
    this->runAction(Sequence::create(
                                     DelayTime::create(1.5f),
                                     CallFunc::create([]() {
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GamePlay::scene()));
    }),
                                     NULL));
    
}