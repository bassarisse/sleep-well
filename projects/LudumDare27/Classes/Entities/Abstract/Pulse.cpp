

#include "Pulse.h"

#include "../Player.h"

void Pulse::addFixtures() {
	float width = _node->getContentSize().width;
    
    this->addCircularFixture(width / 4);
}

bool Pulse::init(b2World *world, Dictionary *properties, Player *player, float angle, float power) {
    
    auto playerNode = player->getNode();
    auto position = Point(playerNode->getPosition().x, playerNode->getPosition().y);
    
    _node = Sprite::createWithSpriteFrameName("neuron_1.png");
    _node->setPosition(position);
	
	this->setType(GameObjectTypePulse);
    
    _player = player;
    _power = power;
    
    if (!GameObject::init(world, properties, true, true))
        return false;
    
    ((Sprite *)_node)->setColor(Color3B(100, 255, 100));
    
    _shouldFlipSprite = false;
    
    _body->SetGravityScale(0);
    _node->setRotation(angle);
    
    auto completionAction = CallFunc::create([this]() {
        this->setState(GameObjectStateDead);
    });
    
    auto action = Sequence::create(
                                   FadeOut::create(kPulseWaveTime),
                                   completionAction,
                                   NULL);
    
    float force = (kPulseForce + _power * kPulseForcePowerFactor);
    
    float aImpulseX = force * cos(angle * M_PI / 180.0f);
    float aImpulseY = force * sin(angle * M_PI / 180.0f);
    
    b2Vec2 aImpulse = b2Vec2(aImpulseX, aImpulseY);
    
    _body->ApplyLinearImpulse(aImpulse, _body->GetWorldCenter());
    
    this->getNode()->runAction(action);
    
    return true;
}