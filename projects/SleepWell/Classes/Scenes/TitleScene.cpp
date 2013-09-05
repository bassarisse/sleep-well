#include "TitleScene.h"
#include "SimpleAudioEngine.h"
#include "LevelTransition.h"

#include "../Common.h"
#include "../GameState.h"
#include "../HudApneaBar.h"

USING_NS_CC;

using namespace CocosDenshion;

TitleScene::~TitleScene() {
	CC_SAFE_RELEASE(_help);
	CC_SAFE_RELEASE(_credits);
	CC_SAFE_RELEASE(_apneaExplanation);
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
    
    _helpShown = false;
    _apneaExplanationShown = false;
	_creditsShown = false;
    
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_select.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_back.wav");
    
    GameState::getInstance()->clearActTimes();
    
    auto winSize = Director::getInstance()->getWinSize();
    
	Sprite* bgSprite = Sprite::create("title_bg.png");
	bgSprite->setPosition(Point((winSize.width / 2), winSize.height / 2));
    
	Sprite* titleSprite = Sprite::create("title_text.png");
	titleSprite->setPosition(Point((winSize.width / 2), winSize.height * 0.8));
    
	Sprite* bedSprite = Sprite::create("bed.png");
	bedSprite->setPosition(Point((winSize.width * 0.5f), winSize.height * 0.5f));
    bedSprite->setOpacity(100);
    
    auto scaleFactor = 0.08f;
    
    auto bgAction = Sequence::create(
                                     EaseInOut::create(FadeTo::create(4.3f, 100.0f), 2.0f),
                                     EaseInOut::create(FadeTo::create(4.3f, 255.0f), 2.0f),
                                     NULL);
    
    auto titleAction = Sequence::create(
                                        EaseInOut::create(ScaleTo::create(3.0f, 1.0f - scaleFactor), 2.0f),
                                        EaseInOut::create(ScaleTo::create(3.0f, 1.0f), 2.0f),
                                        NULL);
    
    auto bedAction = Sequence::create(
                                      EaseInOut::create(ScaleTo::create(2.0f, 1.0f + scaleFactor), 2.0f),
                                      EaseInOut::create(ScaleTo::create(2.0f, 1.0f), 2.0f),
                                      NULL);
    
    bgSprite->runAction(RepeatForever::create(bgAction));
    titleSprite->runAction(RepeatForever::create(titleAction));
    bedSprite->runAction(RepeatForever::create(bedAction));
    
    auto startLabel = LabelBMFont::create("Start exam", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    auto helpLabel = LabelBMFont::create("Help", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    auto apneaLabel = LabelBMFont::create("Apnea?", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    auto aboutLabel = LabelBMFont::create("About", "MiniFont.fnt", 300, Label::HAlignment::CENTER);
    
    auto menuColor = Color3B(255, 255, 180);
    
    startLabel->setColor(menuColor);
    helpLabel->setColor(menuColor);
    apneaLabel->setColor(menuColor);
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
    
	auto apneaOpt = MenuItemLabel::create(apneaLabel, [this](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
		_apneaExplanationShown = true;
		this->addChild(_apneaExplanation);
        this->enableMenus(false);
	});
    
	auto aboutOpt = MenuItemLabel::create(aboutLabel, [this](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
		_creditsShown = true;
		this->addChild(_credits);
        this->enableMenus(false);
	});

	_menu = Menu::create(startOpt, helpOpt, aboutOpt, apneaOpt, NULL);
	_menu->setPosition(Point(winSize.width - 150, winSize.height * 0.2f));
	_menu->alignItemsVerticallyWithPadding(10);
    
	this->addChild(bgSprite);
	this->addChild(bedSprite);
	this->addChild(titleSprite);
	this->addChild(_menu);
    
    this->setTouchEnabled(true);
    
    // CONTROLS ----------
    
    auto controlsLabelsColor = Color3B(140, 140, 140);
    
	auto controlsTitle = LabelBMFont::create("CONTROLS", "MicroFont.fnt", winSize.width * 0.3f, Label::HAlignment::CENTER);
	controlsTitle->setPosition(Point(winSize.width * 0.21f, winSize.height * 0.14f));
    controlsTitle->setAnchorPoint(Point(0.5f, 0.5f));
    controlsTitle->setColor(controlsLabelsColor);
    
	auto controlsText = LabelBMFont::create("Movement \nEnergy pulse ", "MicroFont.fnt", winSize.width * 0.3f, Label::HAlignment::RIGHT);
	controlsText->setPosition(Point(winSize.width * 0.19f, winSize.height * 0.08f));
    controlsText->setAnchorPoint(Point(1, 0.5f));
    controlsText->setColor(controlsLabelsColor);
    
	auto controlsText2 = LabelBMFont::create("- WASD/Arrows/Hold mouse click\n- Space/X", "MicroFont.fnt", winSize.width * 0.7f, Label::HAlignment::LEFT);
	controlsText2->setPosition(Point(winSize.width * 0.2f, winSize.height * 0.08f));
    controlsText2->setAnchorPoint(Point(0, 0.5f));
    controlsText2->setColor(controlsLabelsColor);
    
	this->addChild(controlsTitle);
	this->addChild(controlsText);
	this->addChild(controlsText2);
    
    // CREDITS ----------
    
	auto creditsText = LabelBMFont::create("ABOUT\n \nGame by:\nBruno Assarisse\n \nSpecial Thanks:\nCaroline Rodolfo <3\n \nMade for Ludum Dare 27\nwithin 72 hours :D", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	creditsText->setPosition(Point(winSize.width / 2, winSize.height / 2));
	
	_credits = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_credits->retain();
	_credits->addChild(creditsText);
    
    // APNEA EXPLANATION ----------
    
	auto apneaExplanationText = LabelBMFont::create("WHAT IS APNEA?\n \nApnea is a desease.", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	apneaExplanationText->setPosition(Point(winSize.width / 2, winSize.height / 2));
	
	_apneaExplanation = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_apneaExplanation->retain();
	_apneaExplanation->addChild(apneaExplanationText);
    
    // HELP ----------
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("general.plist");
    
	auto helpTitle = LabelBMFont::create("HELP", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	helpTitle->setPosition(Point(winSize.width * 0.1f, winSize.height * 0.92f));
    
	auto helpNerveImage = Sprite::create("nerve.png");
    helpNerveImage->setAnchorPoint(Point(0.5f, 1));
	helpNerveImage->setPosition(Point(winSize.width * 0.72f, winSize.height));
    
    auto helpNeuronImage = Sprite::createWithSpriteFrameName("neuron_1.png");
	helpNeuronImage->setPosition(Point(winSize.width * 0.08f, winSize.height * 0.57f));
    helpNeuronImage->setFlipX(true);
    
    auto helpBadNeuronImage = Sprite::createWithSpriteFrameName("badneuron_1.png");
	helpBadNeuronImage->setPosition(Point(winSize.width * 0.94f, winSize.height * 0.37f));
    helpBadNeuronImage->setFlipY(true);
    
    auto helpApneaBar = new HudApneaBar();
    helpApneaBar->autorelease();
    helpApneaBar->init();
    helpApneaBar->setLevel(75);
    helpApneaBar->setPosition(Point(winSize.width * 0.07f, winSize.height * 0.25f));
    
	auto helpInstruction = LabelBMFont::create("Shoot energy pulses on this brain connection to reduce the apnea level and make the body breathe again.", "MicroFont.fnt", winSize.width * 0.5f, Label::HAlignment::CENTER);
	helpInstruction->setPosition(Point(winSize.width * 0.72f, winSize.height * 0.76f));
    helpInstruction->getTexture()->setAliasTexParameters();
    
	auto helpInstruction2 = LabelBMFont::create("This is your hero neuron!\nHelp him get your apnea level to 0 before 10 seconds!", "MicroFont.fnt", winSize.width * 0.39f, Label::HAlignment::LEFT);
    helpInstruction2->setAnchorPoint(Point(0, 0.5f));
	helpInstruction2->setPosition(Point(winSize.width * 0.13f, winSize.height * 0.57f));
    
	auto helpInstruction3 = LabelBMFont::create("These guys aren't bad, they're just... doing the wrong thing! Avoid them and you'll be fine.", "MicroFont.fnt", winSize.width * 0.3f, Label::HAlignment::RIGHT);
    helpInstruction3->setAnchorPoint(Point(1, 0.5f));
	helpInstruction3->setPosition(Point(winSize.width * 0.89f, winSize.height * 0.37f));
    
	auto helpInstruction4 = LabelBMFont::create("You don't want apnea, you want a good night of sleep, right? So, to achieve that, finish all the 10 suspensions of breathing as fast as you can. You'll receive the result of your exam after that.", "MicroFont.fnt", winSize.width * 0.75f, Label::HAlignment::CENTER);
    helpInstruction4->setAnchorPoint(Point(0.5f, 0.5f));
	helpInstruction4->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.09f));
    
	auto helpInstruction5 = LabelBMFont::create("Oh, the apnea level?\nIt's the blue bar at the top of the screen.\nIt decreases automatically, but really slowly!", "MicroFont.fnt", winSize.width * 0.55f, Label::HAlignment::LEFT);
    helpInstruction5->setAnchorPoint(Point(0, 0.5f));
	helpInstruction5->setPosition(Point(winSize.width * 0.02f, winSize.height * 0.35f));
	
	_help = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_help->retain();
	_help->addChild(helpTitle);
	_help->addChild(helpNerveImage);
	_help->addChild(helpNeuronImage);
	_help->addChild(helpBadNeuronImage);
	_help->addChild(helpApneaBar);
	_help->addChild(helpInstruction);
	_help->addChild(helpInstruction2);
	_help->addChild(helpInstruction3);
	_help->addChild(helpInstruction4);
	_help->addChild(helpInstruction5);

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
    
	if (_apneaExplanationShown) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_back.wav");
		_apneaExplanationShown = false;
		this->removeChild(_apneaExplanation);
        this->enableMenus(true);
	}
	
}

void TitleScene::enableMenus(bool enabled) {
    
    _menu->setEnabled(enabled);
    
}