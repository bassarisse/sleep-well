#include "HudPowerBar.h"

bool HudPowerBar::init() {
	if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();

	this->setContentSize(Size(256,40));
	this->setPosition(25, winSize.height - 83);

	_gaugeSprite = Sprite::createWithSpriteFrameName("coffeebar.png");
	_gaugeSprite->setAnchorPoint(Point(0, 0));
	_gaugeSprite->setPosition(Point(0, 0));

	_bar = LayerColor::create(Color4B(12, 12, 12, 255));
	_bar->setPosition(5, 3);
	_bar->setContentSize(Size(_gaugeSprite->getContentSize().width - 3, 25));
	
	_icon = Sprite::createWithSpriteFrameName("coffee.png");;
	_icon->setPosition(Point(0, this->getContentSize().height / 2));
	_icon->setOpacity(240);

	this->addChild(_bar);
	this->addChild(_gaugeSprite);
	this->addChild(_icon);
    
	return true;
}


void HudPowerBar::setLevel(float level) {
    
	int bgbarMaxWidth = _gaugeSprite->getContentSize().width - 5;
    
	_bar->setContentSize(Size(bgbarMaxWidth * (level / 100), 25));
    
}

void HudPowerBar::blinkBar() {
    
	Blink* blink = Blink::create(0.3f, 2);
    
	this->stopAllActions();
	this->runAction(Sequence::create(Show::create(), blink, NULL));
    
}

void HudPowerBar::setPeaBerry(bool isPeaBerry) {
    if (isPeaBerry)
        _icon->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("special_coffee.png"));
    else
        _icon->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("coffee.png"));
}