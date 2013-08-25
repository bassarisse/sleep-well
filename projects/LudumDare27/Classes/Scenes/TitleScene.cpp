#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "LevelTransition.h"

#include "../Common.h"
#include "../GameState.h"

USING_NS_CC;

using namespace CocosDenshion;

TitleScene::~TitleScene() {
	CC_SAFE_RELEASE(credits);
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

	Sprite* bgSprite = Sprite::create("officerampage.png");
	bgSprite->setPosition(Point((this->getContentSize().width / 2), this->getContentSize().height / 2));

	MenuItemImage* startOpt = MenuItemImage::create("gettowork.png", "gettowork.png", [](Object* obj) {
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
		Scene *pScene = LevelTransition::scene();
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, pScene));
	});
    
	MenuItemImage* creditsOpt = MenuItemImage::create("credits.png", "credits.png",  [this](Object* obj) {
		_creditsShown = true;
		this->addChild(credits);
        this->enableMenus(false);
	});
    
	LabelBMFont* creditsText = LabelBMFont::create("CREDITS\n \nCoding:\n Bruno Assarisse\nMurilo Clemente\n \nArt:\nCamila Christie\n \nMade in CampJam 2013\n within 48 hours :)", "MainFont.fnt", 750, Label::HAlignment::CENTER);
	creditsText->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	
	credits = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	credits->retain();
	credits->setPosition(Point(0,0));
	credits->setContentSize(Size(1024, 768));
	credits->setAnchorPoint(Point(0, 0));
	credits->setPosition(Point(0,0));
	credits->addChild(creditsText);

	_menu = Menu::create(startOpt, creditsOpt, NULL);
	_menu->setPosition(Point(this->getContentSize().width / 2, 60));
	_menu->alignItemsHorizontallyWithPadding(670);

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
		this->removeChild(credits);
        this->enableMenus(true);
	}
	
}

void TitleScene::enableMenus(bool enabled) {
    
    _menu->setEnabled(enabled);
    
}