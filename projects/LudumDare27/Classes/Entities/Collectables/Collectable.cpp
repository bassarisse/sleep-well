
#include "Collectable.h"

void Collectable::addFixtures() {
	float width = _node->getContentSize().width * 0.6f;
    //float height = _node->getContentSize().height * 0.6f;
    
    //this->addRectangularFixture(width, height);
    this->addCircularFixture(width / 2);
}

bool Collectable::init(b2World *world, Dictionary *properties) {
    
    if (!GameObject::init(world, properties, true))
        return false;
    
    ActionInterval *action = Sequence::create(
                                              EaseInOut::create(MoveBy::create(1.2f, Point(0, 15)), 1.5f),
                                              EaseInOut::create(MoveBy::create(1.2f, Point(0, -15)), 1.5f),
                                              NULL
                                              );
    
    this->getNode()->runAction(RepeatForever::create(action));
    
    return true;
}

void Collectable::update(float dt) {
    if(_state == GameObjectStateDying || _state == GameObjectStateDead)
		return;
    
    GameObject::update(dt);
    
    Sprite *sprite = (Sprite *)_node;
    if (sprite->getOpacity() < 255) {
        int newOpacity = sprite->getOpacity() + dt * 600;
        if (newOpacity > 255) newOpacity = 255;
        sprite->setOpacity(newOpacity);
    }
    
}