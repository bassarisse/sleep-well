#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "LevelTransition.h"

#include "../Common.h"
#include "../GameState.h"

USING_NS_CC;

using namespace CocosDenshion;

TitleScene::~TitleScene() {
	CC_SAFE_RELEASE(_help);
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
    
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_select.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_back.wav");
    
    GameState::getInstance()->clearActTimes();
    
    auto winSize = Director::getInstance()->getWinSize();
    
	Sprite* bgSprite = Sprite::create("title_bg.png");
	bgSprite->setPosition(Point((this->getContentSize().width / 2), this->getContentSize().height / 2));
    
	Sprite* titleSprite = Sprite::create("title_text.png");
	titleSprite->setPosition(Point((this->getContentSize().width / 2), this->getContentSize().height * 0.8));
    
	Sprite* bedSprite = Sprite::create("bed.png");
	bedSprite->setPosition(Point((this->getContentSize().width * 0.5f), this->getContentSize().height * 0.5f));
    bedSprite->setOpacity(100);
    
    auto scaleFactor = 0.08f;
    
    auto titleAction = Sequence::create(
                                      EaseInOut::create(ScaleTo::create(3.0f, 1.0f - scaleFactor), 2.0f),
                                      EaseInOut::create(ScaleTo::create(3.0f, 1.0f), 2.0f),
                                      NULL);
    
    auto bedAction = Sequence::create(
                                      EaseInOut::create(ScaleTo::create(2.0f, 1.0f + scaleFactor), 2.0f),
                                      EaseInOut::create(ScaleTo::create(2.0f, 1.0f), 2.0f),
                                      NULL);
    
    titleSprite->runAction(RepeatForever::create(titleAction));
    bedSprite->runAction(RepeatForever::create(bedAction));
    
    auto startLabel = LabelBMFont::create("Start exam", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    auto helpLabel = LabelBMFont::create("Help", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    auto aboutLabel = LabelBMFont::create("About", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    
    auto menuColor = Color3B(255, 255, 180);
    
    startLabel->setColor(menuColor);
    helpLabel->setColor(menuColor);
    aboutLabel->setColor(menuColor);

	auto startOpt = MenuItemLabel::create(startLabel, [](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LevelTransition::scene()));
	});
    
	auto helpOpt = MenuItemLabel::create(helpLabel, [this](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
		_helpShown = true;
		this->addChild(_help);
        this->enableMenus(false);
	});
    
	auto aboutOpt = MenuItemLabel::create(aboutLabel, [this](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
		_creditsShown = true;
		this->addChild(_credits);
        this->enableMenus(false);
	});

	_menu = Menu::create(startOpt, helpOpt, aboutOpt, NULL);
	_menu->setPosition(Point(this->getContentSize().width - 150, this->getContentSize().height * 0.2f));
	_menu->alignItemsVerticallyWithPadding(10);
    
	this->addChild(bgSprite);
	this->addChild(bedSprite);
	this->addChild(titleSprite);
	this->addChild(_menu);
    
    this->setTouchEnabled(true);
    
    _helpShown = false;
	_creditsShown = false;
    
    // CREDITS ----------
    
	auto creditsText = LabelBMFont::create("ABOUT\n \nGame by:\nBruno Assarisse\n \nSpecial Thanks:\nCaroline Rodolfo <3\n \nMade for Ludum Dare 27\nwithin 72 hours :D", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	creditsText->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	
	_credits = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_credits->retain();
	_credits->addChild(creditsText);
    
    // HELP ----------
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("general.plist");
    
	auto helpTitle = LabelBMFont::create("HELP", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	helpTitle->setPosition(Point(this->getContentSize().width * 0.1f, this->getContentSize().height * 0.92f));
    
	auto helpNerveImage = Sprite::create("nerve.png");
    helpNerveImage->setAnchorPoint(Point(0.5f, 1));
	helpNerveImage->setPosition(Point(this->getContentSize().width * 0.65f, this->getContentSize().height));
    
    auto helpNeuronImage = Sprite::createWithSpriteFrameName("neuron_1.png");
	helpNeuronImage->setPosition(Point(this->getContentSize().width * 0.11f, this->getContentSize().height * 0.45f));
    helpNeuronImage->setFlipX(true);
    
	auto helpInstruction = LabelBMFont::create("Shoot energy pulses on this brain connection to reduce the apnea level and make the body breathe again.", "MicroFont.fnt", winSize.width * 0.4f, Label::HAlignment::LEFT);
	helpInstruction->setPosition(Point(this->getContentSize().width * 0.65f, this->getContentSize().height * 0.72f));
    helpInstruction->getTexture()->setAliasTexParameters();
    
	auto helpInstruction2 = LabelBMFont::create("This is your hero neuron!\nHelp him avoid the other neurons and get your apnea level to 0 before 10 seconds!", "MicroFont.fnt", winSize.width * 0.4f, Label::HAlignment::LEFT);
    helpInstruction2->setAnchorPoint(Point(0, 0.5f));
	helpInstruction2->setPosition(Point(this->getContentSize().width * 0.16f, this->getContentSize().height * 0.45f));
    
	auto helpText = LabelBMFont::create("WASD/Arrows \nHold mouse click \nSpace ", "MiniFont.fnt", winSize.width / 2, Label::HAlignment::RIGHT);
	helpText->setPosition(Point(this->getContentSize().width * 0.4825f, this->getContentSize().height * 0.14f));
    helpText->setAnchorPoint(Point(1, 0.5f));
    
	auto helpText2 = LabelBMFont::create("- Movement\n- Movement\n- Energy pulse", "MiniFont.fnt", winSize.width / 2, Label::HAlignment::LEFT);
	helpText2->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height * 0.14f));
    helpText2->setAnchorPoint(Point(0, 0.5f));
	
	_help = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_help->retain();
	_help->addChild(helpTitle);
	_help->addChild(helpNerveImage);
	_help->addChild(helpNeuronImage);
	_help->addChild(helpInstruction);
	_help->addChild(helpInstruction2);
	_help->addChild(helpText);
	_help->addChild(helpText2);

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
    
	if (pressed)
		return;
    
	if (_creditsShown) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_back.wav");
		_creditsShown = false;
		this->removeChild(_credits);
        this->enableMenus(true);
	}
    
	if (_helpShown) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_back.wav");
		_helpShown = false;
		this->removeChild(_help);
        this->enableMenus(true);
	}
	
}

void TitleScene::enableMenus(bool enabled) {
    
    _menu->setEnabled(enabled);
    
}