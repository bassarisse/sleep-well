#include "HudApneaBar.h"

bool HudApneaBar::init() {
	if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();

	this->setContentSize(Size(256,40));
	this->setPosition(25, winSize.height - 48);

	_gaugeSprite = Sprite::createWithSpriteFrameName("lifebar.png");
	_gaugeSprite->setAnchorPoint(Point(0, 0));
	_gaugeSprite->setPosition(Point(0, 0));

	_bar = LayerColor::create(Color4B(30, 230, 30, 255));
	_bar->setPosition(5, 5);
    
    this->update(0);

	Sprite* healthIcon = Sprite::createWithSpriteFrameName("donut.png");
	healthIcon->setPosition(Point(0, this->getContentSize().height / 2));
	healthIcon->setOpacity(240);
	
	this->addChild(_bar);
	this->addChild(_gaugeSprite);
	this->addChild(healthIcon);

	return true;
}


void HudApneaBar::setLevel(float level) {
    
	int bgbarMaxWidth = _gaugeSprite->getContentSize().width - 7;
    
	_bar->setContentSize(Size(bgbarMaxWidth * (level / 100), 36));
    
}

void HudApneaBar::blinkBar() {
    
	Blink* blink = Blink::create(0.3f, 2);
    
	this->stopAllActions();
	this->runAction(Sequence::create(Show::create(), blink, NULL));
    
}