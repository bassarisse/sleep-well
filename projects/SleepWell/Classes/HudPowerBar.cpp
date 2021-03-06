#include "HudPowerBar.h"

bool HudPowerBar::init() {
	if ( !LayerColor::initWithColor(Color4B(180, 90, 90, 255)) )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
	this->setContentSize(Size((int)(winSize.width * 0.25f), 20));
	this->setAnchorPoint(Point(0, 0));
	this->setPosition(8, winSize.height - 56);
    
    auto thisSize = this->getContentSize();
    
	auto bg = LayerColor::create(Color4B(0, 0, 0, 255));
	bg->setAnchorPoint(Point(0, 0));
	bg->setPosition(1, 1);
	bg->setContentSize(Size((thisSize.width - 2), thisSize.height - 2));
    
	_bar = LayerGradient::create(Color4B(180, 30, 30, 255), Color4B(90, 15, 15, 255));
	_bar->setAnchorPoint(Point(0, 0));
	_bar->setPosition(1, 1);
    
    auto label = LabelBMFont::create("ENERGY", "MicroFont.fnt", 200, Label::HAlignment::LEFT);
    label->getTexture()->setAliasTexParameters();
	label->setAnchorPoint(Point(0, 0));
    label->setPosition(2, -3);
    
	this->addChild(bg);
	this->addChild(_bar);
    this->addChild(label);
    
	return true;
}


void HudPowerBar::setLevel(float level) {
    
    auto thisSize = this->getContentSize();
    
	_bar->setContentSize(Size((thisSize.width - 2) * (level / 100), thisSize.height - 2));
    
}

void HudPowerBar::blinkBar() {
    
	Blink* blink = Blink::create(0.3f, 2);
    
	this->stopAllActions();
	this->runAction(Sequence::create(Show::create(), blink, NULL));
    
}