#include "BadNeuron.h"

#include "SimpleAudioEngine.h"

void BadNeuron::addFixtures() {
    
    float width = _node->getContentSize().width * 0.32f;
    float height = _node->getContentSize().height * 0.2f;
    
    this->addCircularFixture(0, -height * 2, width / 2);
    //this->addRectangularFixture(0, -height * 2, width, height);

} 

bool BadNeuron::init(b2World *world, Dictionary *properties, Player *ref) {
	
	_node = Sprite::createWithSpriteFrameName("man_down.png");
    _spriteFrameName = "man";

	this->setType(GameObjectTypeBadNeuron);
	
	if (!Enemy::init(world, properties, ref))
        return false;
    
	_speedFactor = -3.4f;
    
    return true;
}

void BadNeuron::die() {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("death_man.wav");
    Enemy::die();
}