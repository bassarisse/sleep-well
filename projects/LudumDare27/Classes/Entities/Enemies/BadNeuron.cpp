#include "BadNeuron.h"

#include "../../GameState.h"
#include "SimpleAudioEngine.h"

void BadNeuron::addFixtures() {
    
    float width = _node->getContentSize().width * 0.32f;
    //float height = _node->getContentSize().height * 0.2f;
    
    this->addCircularFixture(width / 2);

} 

bool BadNeuron::init(b2World *world, Dictionary *properties, Player *ref) {
	
	_node = Sprite::createWithSpriteFrameName("badneuron_1.png");
    _spriteFrameName = "badneuron";

	this->setType(GameObjectTypeBadNeuron);
	
	if (!Enemy::init(world, properties, ref))
        return false;
    
    int level = GameState::getInstance()->getActLevel();
    
	_speedFactor = -3.4f + level * 0.1f;
    _useIdleFrame = false;
    
    return true;
}

void BadNeuron::die() {
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("death_man.wav");
    Enemy::die();
}