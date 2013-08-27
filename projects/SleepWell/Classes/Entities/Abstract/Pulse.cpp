

#include "Pulse.h"

#include "../Player.h"

void Pulse::addFixtures() {
	float width = _node->getContentSize().width;
    
    this->addCircularFixture(width / 4);
}

bool Pulse::init(b2World *world, Dictionary *properties, Player *player, float angle, float power) {
    
    auto playerNode = player->getNode();
    auto position = Point(playerNode->getPosition().x, playerNode->getPosition().y);
    
    _node = Sprite::createWithSpriteFrameName("pulse.png");
    _node->setPosition(position);
	
	this->setType(GameObjectTypePulse);
    
    _player = player;
    _nerveHit = false;
    _power = power;
    
    if (!GameObject::init(world, properties, true, true))
        return false;
    
    _shouldFlipSprite = false;
    
    float nodeAngle = 360 - angle;
    
    _body->SetGravityScale(0);
    _node->setRotation(nodeAngle);
    _node->setScale(0.5f + power / 200.0f);
    
    float force = (kPulseForce + _power * kPulseForcePowerFactor);
    
    float aImpulseX = force * cos(angle * M_PI / 180.0f);
    float aImpulseY = force * sin(angle * M_PI / 180.0f);
    
    b2Vec2 aImpulse = b2Vec2(aImpulseX, aImpulseY);
    
    _body->ApplyLinearImpulse(aImpulse, _body->GetWorldCenter());
    
    auto completionAction = CallFunc::create([this]() {
        this->setState(GameObjectStateDead);
    });
    
    auto action = Sequence::create(
                                   FadeOut::create(kPulseWaveTime),
                                   completionAction,
                                   NULL);
    
    this->getNode()->runAction(action);
    
    return true;
}