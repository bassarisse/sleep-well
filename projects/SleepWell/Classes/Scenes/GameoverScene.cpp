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
    
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_select.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sfx_breathing.wav");
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("bgm_gameover.wav");
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    auto winSize = Director::getInstance()->getWinSize();
    
	auto bgSprite = Sprite::create("gameover.png");
    bgSprite->getTexture()->setAliasTexParameters();
	bgSprite->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(bgSprite);
    
	auto scoreHighlight = Sprite::create("score_highlight.png");
    scoreHighlight->getTexture()->setAliasTexParameters();
	scoreHighlight->setPosition(Point(this->getContentSize().width * 0.43f, this->getContentSize().height * 0.373f));
	this->addChild(scoreHighlight);
    
    int apneaCount = 0;
    int apneaLevel = 0;
    int score = 0;
    auto actTimes = GameState::getInstance()->getActTimes();
    auto maxSize = actTimes.size();
    
    auto titleColor = Color3B(50, 100, 200);
    auto textColor = Color3B(40, 40, 40);
    auto problemColor = Color3B(230, 30, 30);
    auto menuColor = Color3B(255, 255, 180);
    
    int x = 220;
    int y = 547;
    int ySpace = 20;
    int xSpace = 120;
    
    y -= ySpace;
    
    auto resultsLabel = LabelBMFont::create("Results", "MicroFont.fnt", 100);
    resultsLabel->getTexture()->setAliasTexParameters();
    resultsLabel->setAnchorPoint(Point(0, 0));
    resultsLabel->setPosition(x - xSpace * 0.2f, y);
    resultsLabel->setColor(titleColor);
    
    this->addChild(resultsLabel);
    
    for (auto i = maxSize - maxSize; i < maxSize; i++) {
        float actTime = actTimes[i];
        
        float scoreBase = 100.0f;
        float difference = 15.0f;
        bool isApnea = actTime > 10.0f;
        
        if (isApnea) {
            
            scoreBase = 50.0f;
            difference = 20.0f;
            apneaCount++;
            
        }
        
        int addedScore = scoreBase * (difference - actTime);
        
        score += addedScore;
        
        y -= ySpace;
        
        auto actNumberLabel = LabelBMFont::create(String::createWithFormat("%ld", i + 1)->getCString(), "MicroFont.fnt", 100);
        actNumberLabel->setAnchorPoint(Point(1, 0));
        actNumberLabel->setPosition(x, y);
        actNumberLabel->setColor(textColor);
        
        auto timeLabel = LabelBMFont::create(String::createWithFormat("%.1f", actTime)->getCString(), "MicroFont.fnt", 100);
        timeLabel->setAnchorPoint(Point(1, 0));
        timeLabel->setPosition(x + xSpace, y);
        if (isApnea)
            timeLabel->setColor(problemColor);
        else
            timeLabel->setColor(textColor);
        
        auto scoreLabel = LabelBMFont::create(String::createWithFormat("%i", addedScore)->getCString(), "MicroFont.fnt", 100);
        scoreLabel->setAnchorPoint(Point(1, 0));
        scoreLabel->setPosition(x + xSpace * 2, y);
        if (addedScore < 0)
            scoreLabel->setColor(problemColor);
        else
            scoreLabel->setColor(textColor);
        
        this->addChild(actNumberLabel);
        this->addChild(timeLabel);
        this->addChild(scoreLabel);
        
    }
    
    if (apneaCount > 1)
        apneaLevel++;
    if (apneaCount > 4)
        apneaLevel++;
    if (apneaCount > 7)
        apneaLevel++;
    
    int scoreMultiplier = 4 - apneaLevel;
    if (apneaCount == 0)
        scoreMultiplier++;
    int finalScore = score * scoreMultiplier;
    
    if (score < 0)
        score = 0;
    if (finalScore < 0)
        finalScore = 0;
    
    int bestScore = UserDefault::getInstance()->getIntegerForKey("Record", 0);
    
    bool isRecord = finalScore > bestScore;
    
    if (isRecord) {
        UserDefault::getInstance()->setIntegerForKey("Record", finalScore);
        UserDefault::getInstance()->flush();
    }
    
    y -= ySpace;
    y -= ySpace;
    
    auto subtotalLabel = LabelBMFont::create("Subtotal", "MicroFont.fnt", 100);
    subtotalLabel->setAnchorPoint(Point(1, 0));
    subtotalLabel->setPosition(x + xSpace, y);
    subtotalLabel->setColor(textColor);
    
    auto scoreLabel = LabelBMFont::create(String::createWithFormat("%i", score)->getCString(), "MicroFont.fnt", 100);
    scoreLabel->setAnchorPoint(Point(1, 0));
    scoreLabel->setPosition(x + xSpace * 2, y);
    scoreLabel->setColor(textColor);
    
    y -= ySpace;
    
    auto multiplierLabel = LabelBMFont::create("Multiplier", "MicroFont.fnt", 100);
    multiplierLabel->setAnchorPoint(Point(1, 0));
    multiplierLabel->setPosition(x + xSpace, y);
    multiplierLabel->setColor(textColor);
    
    auto multiplierNumberLabel = LabelBMFont::create(String::createWithFormat("x%i", scoreMultiplier)->getCString(), "MicroFont.fnt", 100);
    multiplierNumberLabel->setAnchorPoint(Point(1, 0));
    multiplierNumberLabel->setPosition(x + xSpace * 2, y);
    multiplierNumberLabel->setColor(textColor);
    
    y -= ySpace;
    y -= ySpace;
    
    auto totalLabel = LabelBMFont::create("Total", "MicroFont.fnt", 100);
    totalLabel->setAnchorPoint(Point(1, 0));
    totalLabel->setPosition(x + xSpace, y);
    totalLabel->setColor(textColor);
    
    auto finalScoreLabel = LabelBMFont::create(String::createWithFormat("%i", finalScore)->getCString(), "MicroFont.fnt", 100);
    finalScoreLabel->setAnchorPoint(Point(1, 0));
    finalScoreLabel->setPosition(x + xSpace * 2, y);
    finalScoreLabel->setColor(textColor);
    
    y -= ySpace;
    
    if (isRecord) {
        
        auto recordLabel = LabelBMFont::create("Record!", "MicroFont.fnt", 100);
        recordLabel->setAnchorPoint(Point(1, 0));
        recordLabel->setPosition(x + xSpace * 2, y);
        recordLabel->setColor(Color3B(30, 180, 60));
        
        this->addChild(recordLabel);
        
    }
    
    y -= ySpace;
    y -= ySpace;
    
    auto conclusionLabel = LabelBMFont::create("Conclusion", "MicroFont.fnt", 100);
    conclusionLabel->setAnchorPoint(Point(0, 0));
    conclusionLabel->setPosition(x - xSpace * 0.2f, y);
    conclusionLabel->setColor(titleColor);
    
    auto conclusionTextLabel = LabelBMFont::create("", "MicroFont.fnt", 282, Label::HAlignment::LEFT);
    conclusionTextLabel->setAnchorPoint(Point(0, 1));
    conclusionTextLabel->setPosition(x - xSpace * 0.2f, y + 4);
    conclusionTextLabel->setColor(textColor);
    
    switch (apneaLevel) {
        case 0:
            conclusionTextLabel->setString("Good, you don't have apnea!");
            break;
        case 1:
            conclusionTextLabel->setString("A light case of apnea was detected.");
            break;
        case 2:
            conclusionTextLabel->setString("You have a moderate sleep apnea.");
            break;
        case 3:
            conclusionTextLabel->setString("A severe apnea case was diagnosed!");
            break;
            
        default:
            break;
    }
    
    this->addChild(subtotalLabel);
    this->addChild(scoreLabel);
    this->addChild(multiplierLabel);
    this->addChild(multiplierNumberLabel);
    this->addChild(totalLabel);
    this->addChild(finalScoreLabel);
    this->addChild(conclusionLabel);
    this->addChild(conclusionTextLabel);
    
    auto tipLabel = LabelBMFont::create("", "MicroFont.fnt", (int)(winSize.width * 0.3f), Label::HAlignment::CENTER);
    tipLabel->setAnchorPoint(Point(0.5f, 0.5f));
    tipLabel->setPosition((int)(winSize.width * 0.78f), (int)(winSize.height * 0.75f));
    
    int maxTip = 8;
    
    if (scoreMultiplier > 1)
        maxTip -= 3;
    
    int randomTip = rand() % maxTip;
    
    switch (randomTip) {
        case 0:
            tipLabel->setString("TIP:\nUse your energy pulse to move faster");
            break;
        case 1:
            tipLabel->setString("TIP:\nFinish before 10 seconds to get a higher score multiplier");
            break;
        case 2:
            tipLabel->setString("TIP:\nThe more energy you have when hitting the brain connection, the more your apnea will decrease");
            break;
        case 3:
            tipLabel->setString("TIP:\nPosition yourself accordingly to make the enemies push you the direction you want");
            break;
        case 4:
            tipLabel->setString("TIP:\nFor each suspension of breathing that you pass, it will be harder to go up");
            break;
            
        case 5:
            tipLabel->setString("TIP:\nShoot your energy at the other neurons to stop them for some time");
            break;
        case 6:
            tipLabel->setString("TIP:\nUse 'Space' to shoot energy pulses and hit the brain connection at top of the screen");
            break;
        case 7:
            tipLabel->setString("TIP:\nCheck the 'Help' screen if you're having too much trouble");
            break;
            
        default:
            break;
    }
    
    this->addChild(tipLabel);
    
    GameState::getInstance()->clearActTimes();
    
    auto restartLabel = LabelBMFont::create("Restart", "MiniFont.fnt", 200, Label::HAlignment::CENTER);
    auto returnLabel = LabelBMFont::create("Return", "MiniFont.fnt", 200, Label::HAlignment::CENTER);
    
    restartLabel->setColor(menuColor);
    returnLabel->setColor(problemColor);
 
	auto restartOpt = MenuItemLabel::create(restartLabel, [](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LevelTransition::scene()));
	});

	auto returnOpt = MenuItemLabel::create(returnLabel, [](Object* obj) {
        SimpleAudioEngine::getInstance()->playEffect("sfx_select.wav");
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, TitleScene::scene()));
	});

	_menu = Menu::create(restartOpt, returnOpt, NULL);
	_menu->setPosition(Point((int)(winSize.width * 0.78f), (int)(winSize.height * 0.22f)));
	_menu->alignItemsVerticallyWithPadding(10);
    
	this->addChild(_menu);
    
    SimpleAudioEngine::getInstance()->playEffect("sfx_breathing.wav");
    
	return true;
}

void GameoverScene::onEnterTransitionDidFinish() {
    BaseLayer::onEnterTransitionDidFinish();
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm_gameover.wav", false);
    
}

void GameoverScene::enableMenus(bool enabled) {
    
    _menu->setEnabled(enabled);
    
}