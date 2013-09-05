#include "HudApneaBar.h"

bool HudApneaBar::init() {
	if ( !LayerColor::initWithColor(Color4B(90, 150, 180, 255)) )
    {
        return false;
    }
    
    auto winSize = Director::getInstance()->getWinSize();

	this->setContentSize(Size((int)(winSize.width * 0.25f), 20));
	this->setAnchorPoint(Point(0, 0));
	this->setPosition(8, winSize.height - 28);
    
    auto thisSize = this->getContentSize();
    
	auto bg = LayerColor::create(Color4B(0, 0, 0, 255));
	bg->setAnchorPoint(Point(0, 0));
	bg->setPosition(1, 1);
	bg->setContentSize(Size((thisSize.width - 2), thisSize.height - 2));

	_bar = LayerGradient::create(Color4B(30, 130, 220, 255), Color4B(15, 65, 110, 255));
	_bar->setAnchorPoint(Point(0, 0));
	_bar->setPosition(1, 1);
    
    auto label = LabelBMFont::create("APNEA", "MicroFont.fnt", 200, Label::HAlignment::LEFT);
    label->getTexture()->setAliasTexParameters();
	label->setAnchorPoint(Point(0, 0));
    label->setPosition(2, -3);
    
	this->addChild(bg);
	this->addChild(_bar);
    this->addChild(label);

	return true;
}


void HudApneaBar::setLevel(float level) {
    
    auto thisSize = this->getContentSize();
    
	_bar->setContentSize(Size((thisSize.width - 2) * (level / 100), thisSize.height - 2));
    
}

void HudApneaBar::blinkBar() {
    
	Blink* blink = Blink::create(0.3f, 2);
    
	this->stopAllActions();
	this->runAction(Sequence::create(Show::create(), blink, NULL));
    
}