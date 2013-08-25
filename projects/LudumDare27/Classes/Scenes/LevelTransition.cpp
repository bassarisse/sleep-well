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
    
    int level = GameState::getInstance()->getActLevel();

	auto scoreLabel = LabelBMFont::create(
		String::createWithFormat("Act %d", level)->getCString(), "MainFont.fnt", 600, Label::HAlignment::CENTER);

	scoreLabel->setAnchorPoint(Point(0.5f, 0.5f));
	scoreLabel->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
    
	this->addChild(scoreLabel);
    
    this->runAction(Sequence::create(
                                     DelayTime::create(2.0f),
                                     CallFunc::create([]() {
        Director::getInstance()->replaceScene(GamePlay::scene());
    }),
                                     NULL));
    
	return true;
}