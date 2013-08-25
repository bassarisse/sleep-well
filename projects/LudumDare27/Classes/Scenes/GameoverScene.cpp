#include "GameoverScene.h"
#include "SimpleAudioEngine.h"
#include "TitleScene.h"
#include "LevelTransition.h"
#include "../GameState.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* GameoverScene::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameoverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameoverScene::init()  {
	
	if ( !BaseLayer::init() )
    {
        return false;
    }
    
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
    
	auto bgSprite = Sprite::create("gameover.jpg");
	bgSprite->setPosition(Point(this->getContentSize().width / 2,
                              this->getContentSize().height / 2));

	auto scoreLabel = LabelBMFont::create(
		String::createWithFormat("Your score: %d", 0)->getCString(), "MainFont.fnt", 600, Label::HAlignment::LEFT);

	scoreLabel->setAnchorPoint(Point(0.0, 1.0));
	scoreLabel->setPosition(Point(10, this->getContentSize().height - 10));
 
	auto startOpt = MenuItemImage::create("startover.png", "startover.png", [](Object* obj) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
        GameState::getInstance()->clearActTimes();
		Scene *pScene = LevelTransition::scene();
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, pScene));
	});

	auto returnOpt = MenuItemImage::create("return.png", "return.png", [](Object* obj) {
		Scene *pScene = TitleScene::scene();
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, pScene));
	});

	_menu = Menu::create(startOpt, returnOpt, NULL);
	_menu->setPosition(Point(this->getContentSize().width - 170, this->getContentSize().height * 0.22f));
	_menu->alignItemsVertically();
    
	this->addChild(bgSprite);
	this->addChild(scoreLabel);
	this->addChild(_menu);
    
	return true;
}

void GameoverScene::enableMenus(bool enabled) {
    
    _menu->setEnabled(enabled);
    
}