#include "HudPowerBar.h"

bool HudPowerBar::init() {
	if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();

	this->setContentSize(Size(256,40));
	this->setPosition(25, winSize.height - 83);

	_bar = LayerColor::create(Color4B(230, 30, 30, 255));
	_bar->setPosition(5, 3);

	this->addChild(_bar);
    
	return true;
}


void HudPowerBar::setLevel(float level) {
    
	_bar->setContentSize(Size(200 * (level / 100), 25));
    
}

void HudPowerBar::blinkBar() {
    
	Blink* blink = Blink::create(0.3f, 2);
    
	this->stopAllActions();
	this->runAction(Sequence::create(Show::create(), blink, NULL));
    
}