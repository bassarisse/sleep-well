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
    
    auto text = String::createWithFormat("Suspension of breathing\n%d / %i", level, kMaxLevel)->getCString();

	auto scoreLabel = LabelBMFont::create(text, "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
    scoreLabel->getTexture()->setAliasTexParameters();
	scoreLabel->setAnchorPoint(Point(0.5f, 0.5f));
	scoreLabel->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
	this->addChild(scoreLabel);
    
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