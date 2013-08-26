#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "LevelTransition.h"

#include "../Common.h"
#include "../GameState.h"

USING_NS_CC;

using namespace CocosDenshion;

TitleScene::~TitleScene() {
	CC_SAFE_RELEASE(_credits);
}

Scene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    TitleScene *layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool TitleScene::init()  {
	
	if ( !BaseLayer::init() )
    {
        return false;
    }
    
    GameState::getInstance()->clearActTimes();
    
	_creditsShown = false;

	Sprite* bgSprite = Sprite::create("title.png");
	bgSprite->setPosition(Point((this->getContentSize().width / 2), this->getContentSize().height / 2));
    
    auto startLabel = LabelBMFont::create("Start", "MainFont.fnt", 200, Label::HAlignment::CENTER);
    auto helpLabel = LabelBMFont::create("Help", "MainFont.fnt", 200, Label::HAlignment::CENTER);
    auto aboutLabel = LabelBMFont::create("About", "MainFont.fnt", 200, Label::HAlignment::CENTER);

	auto startOpt = MenuItemLabel::create(startLabel, [](Object* obj) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
		Scene *pScene = LevelTransition::scene();
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, pScene));
	});
    
	auto helpOpt = MenuItemLabel::create(helpLabel, [this](Object* obj) {
        
	});
    
	auto aboutOpt = MenuItemLabel::create(aboutLabel, [this](Object* obj) {
		_creditsShown = true;
		this->addChild(_credits);
        this->enableMenus(false);
	});
    
	auto creditsText = LabelBMFont::create("ABOUT\n \nGame by:\nBruno Assarisse\n \nSpecial Thanks:\nCaroline Rodolfo <3\n \nMade for Ludum Dare 27\nwithin 72 hours :D", "MiniFont.fnt", 750, Label::HAlignment::CENTER);
	creditsText->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	
	_credits = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_credits->retain();
	_credits->setPosition(Point(0,0));
	_credits->setContentSize(Size(1024, 768));
	_credits->setAnchorPoint(Point(0, 0));
	_credits->setPosition(Point(0,0));
	_credits->addChild(creditsText);

	_menu = Menu::create(startOpt, helpOpt, aboutOpt, NULL);
	_menu->setPosition(Point(this->getContentSize().width - 170, this->getContentSize().height * 0.28f));
	_menu->alignItemsVerticallyWithPadding(10);

	this->addChild(bgSprite);
	this->addChild(_menu);
    
    this->setTouchEnabled(true);
	
    SimpleAudioEngine::getInstance()->playBackgroundMusic("title_bgm.mp3", true);

	return true;
}

void TitleScene::registerWithTouchDispatcher()
{
    Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool TitleScene::ccTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    return true;
}

void TitleScene::ccTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) {
    this->buttonAny(false);
}

void TitleScene::buttonAny(bool pressed) {
    
	if(pressed)
		return;
    
	if(_creditsShown)
	{
		_creditsShown = false;
		this->removeChild(_credits);
        this->enableMenus(true);
	}
	
}

void TitleScene::enableMenus(bool enabled) {
    
    _menu->setEnabled(enabled);
    
}