

#include "Pulse.h"

#include "Player.h"

void Pulse::addFixtures() {
	float width = _node->getContentSize().width;
    
    this->addCircularFixture(width / 2);
}

bool Pulse::init(b2World *world, Dictionary *properties, Player *player, float angle) {
    
    auto playerNode = player->getNode();
    auto position = Point(playerNode->getPosition().x, playerNode->getPosition().y - playerNode->getContentSize().height * 0.4f);
    
    _node = Sprite::createWithSpriteFrameName("coffee.png");
    _node->setPosition(position);
	
	this->setType(GameObjectTypePulse);
    
    _player = player;
    
    if (!GameObject::init(world, properties, true, true))
        return false;
    
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
    
    float aImpulseX = kPulseForce * cos(angle * M_PI / 180.0f);
    float aImpulseY = kPulseForce * sin(angle * M_PI / 180.0f);
    
    b2Vec2 aImpulse = b2Vec2(aImpulseX, aImpulseY);
    
    _body->ApplyLinearImpulse(aImpulse, _body->GetWorldCenter());
    
    this->getNode()->runAction(action);
    
    return true;
}