#include "HudApneaBar.h"

bool HudApneaBar::init() {
	if ( !Layer::init() )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();

	this->setContentSize(Size(256,40));
	this->setPosition(25, winSize.height - 48);

	_bar = LayerColor::create(Color4B(30, 230, 30, 255));
	_bar->setPosition(5, 5);
    
    this->update(0);
	
	this->addChild(_bar);

	return true;
}


void HudApneaBar::setLevel(float level) {
    
	_bar->setContentSize(Size(200 * (level / 100), 36));
    
}

void HudApneaBar::blinkBar() {
    
	Blink* blink = Blink::create(0.3f, 2);
    
	this->stopAllActions();
	this->runAction(Sequence::create(Show::create(), blink, NULL));
    
}