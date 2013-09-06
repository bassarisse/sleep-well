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
    
	auto creditsTitle = LabelBMFont::create("ABOUT", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	creditsTitle->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.88f));
	creditsTitle->setAnchorPoint(Point(0.5f, 0.5f));
    
	auto creditsText = LabelBMFont::create("Game by:\nBruno Assarisse\n \nSpecial thanks:\nCaroline Rodolfo <3\n \nMade for Ludum Dare 27\nwithin 72 hours :D", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	creditsText->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.45f));
	
	_credits = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_credits->retain();
	_credits->addChild(creditsTitle);
	_credits->addChild(creditsText);
    
    // APNEA EXPLANATION ----------
    
	auto apneaExplanationTitle = LabelBMFont::create("WHAT IS APNEA?", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	apneaExplanationTitle->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.88f));
	apneaExplanationTitle->setAnchorPoint(Point(0.5f, 0.5f));
    
	auto apneaExplanationText = LabelBMFont::create("Generally speaking, apnea is the state when a person, while sleeping, stops breathing for a certain ammount of time. The more it happens, and more time the person takes to breathe again, greater is the harm that can be made to the body.\n \nWhile asleep, all muscles of the body are relaxed, but, in some cases, the brain \"forgets\" to control the muscles necessary for breathing. That's one of the reasons why apnea occurs, and you're fighting against it in this game.\n \nThe level of an apnea case is measured by how many times a person stops breathing for more than 10 seconds, in a hour, during a sleep cicle. Here, you control the neuron for about 15 minutes of sleep.\n \nHaving a good night of sleep is as important as eating and exercising, so take care!", "MicroFont.fnt", winSize.width * 0.7f, Label::HAlignment::CENTER);
	apneaExplanationText->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.45f));
	
	_apneaExplanation = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_apneaExplanation->retain();
	_apneaExplanation->addChild(apneaExplanationTitle);
	_apneaExplanation->addChild(apneaExplanationText);
    
    // HELP ----------
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("general.plist");
    
	auto helpTitle = LabelBMFont::create("HELP", "MiniFont.fnt", winSize.width, Label::HAlignment::CENTER);
	helpTitle->setPosition(Point(winSize.width * 0.1f, winSize.height * 0.88f));
	helpTitle->setAnchorPoint(Point(0, 0.5f));
    
	auto helpNerveImage = Sprite::create("nerve.png");
    helpNerveImage->setAnchorPoint(Point(0.5f, 1));
	helpNerveImage->setPosition(Point(winSize.width * 0.72f, winSize.height));
    
	auto helpInstruction = LabelBMFont::create("Shoot energy pulses on this brain connection to reduce the apnea level.", "MicroFont.fnt", winSize.width * 0.3f, Label::HAlignment::CENTER);
    helpInstruction->setAnchorPoint(Point(0, 0.5f));
	helpInstruction->setPosition(Point(winSize.width * 0.7f, winSize.height * 0.75f));
    helpInstruction->getTexture()->setAliasTexParameters();
    
    auto helpApneaBar = new HudApneaBar();
    helpApneaBar->autorelease();
    helpApneaBar->init();
    helpApneaBar->setLevel(75);
    helpApneaBar->setPosition(Point(winSize.width * 0.07f, winSize.height * 0.58f));
    
	auto helpInstruction5 = LabelBMFont::create("The body will breathe again when your apnea level runs out. It decreases automatically but slowly!", "MicroFont.fnt", winSize.width * 0.45f, Label::HAlignment::LEFT);
    helpInstruction5->setAnchorPoint(Point(0, 0.5f));
	helpInstruction5->setPosition(Point(winSize.width * 0.02f, winSize.height * 0.68f));
    
    auto helpNeuronImage = Sprite::createWithSpriteFrameName("neuron_1.png");
	helpNeuronImage->setPosition(Point(winSize.width * 0.50f, winSize.height * 0.4f));
    helpNeuronImage->setFlipX(true);
    helpNeuronImage->setFlipY(true);
    
    auto helpPulseImage = Sprite::createWithSpriteFrameName("pulse.png");
	helpPulseImage->setPosition(Point(winSize.width * 0.58f, winSize.height * 0.62f));
    helpPulseImage->setRotation(300);
    
	auto helpInstruction2 = LabelBMFont::create("This is your hero neuron!\nHelp him deplete the apnea level before 10 seconds!", "MicroFont.fnt", winSize.width * 0.39f, Label::HAlignment::RIGHT);
    helpInstruction2->setAnchorPoint(Point(1, 0.5f));
	helpInstruction2->setPosition(Point(winSize.width * 0.43f, winSize.height * 0.36f));
    
    auto helpBadNeuronImage = Sprite::createWithSpriteFrameName("badneuron_1.png");
	helpBadNeuronImage->setPosition(Point(winSize.width * 0.88f, winSize.height * 0.42f));
    
	auto helpInstruction3 = LabelBMFont::create("These guys aren't bad, they're just... doing the wrong thing! Avoid them and you'll be fine.", "MicroFont.fnt", winSize.width * 0.38f, Label::HAlignment::RIGHT);
    helpInstruction3->setAnchorPoint(Point(1, 0.5f));
	helpInstruction3->setPosition(Point(winSize.width * 0.98f, winSize.height * 0.3f));
    
	auto helpInstruction4 = LabelBMFont::create("Finish all the 10 suspensions of breathing as fast as you can.\nYou'll receive the result of your exam after that.", "MicroFont.fnt", winSize.width * 0.75f, Label::HAlignment::CENTER);
    helpInstruction4->setAnchorPoint(Point(0.5f, 0.5f));
	helpInstruction4->setPosition(Point(winSize.width * 0.5f, winSize.height * 0.1f));
	
	_help = LayerColor::create(Color4B(0, 0, 0, kOverlayOpacity));
	_help->retain();
	_help->addChild(helpTitle);
	_help->addChild(helpNerveImage);
	_help->addChild(helpNeuronImage);
	_help->addChild(helpBadNeuronImage);
	_help->addChild(helpApneaBar);
	_help->addChild(helpPulseImage);
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