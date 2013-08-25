
#include "Enemy.h"
#include "Box2D/Box2D.h"

bool Enemy::init(b2World *world, Dictionary *properties, Player *ref) {
    
    //_node = Sprite::createWithSpriteFrameName("right1.png");
	_playerReference = ref;
    
    _sightRange = 0.0f;
    _randomMoveOnly = false;
    _randomMovingTime = 0.0f;
    _isRandomMoving = false;
    _walkingPoint = Point(0, 0);
    
    String *randomOnly = (String *)properties->objectForKey("Random");
    if (randomOnly)
        _randomMoveOnly = true;
    
    String *sight = (String *)properties->objectForKey("Sight");
    if (sight)
        _sightRange = sight->intValue();

    if (!GameObject::init(world, properties))
        return false;
    
    if (rand() % 6 > 0)
        this->setSightRange(100 + rand() % 600);
    this->setRandomMoveOnly(rand() % 8 == 4);
    
    ((Sprite *)_node)->setOpacity(0);
    
    _body->SetGravityScale(0);
    
    return true;
}

void Enemy::handleMovement() {
    
    if (!_randomMoveOnly && this->isNearPlayer()) {
        _isRandomMoving = false;
        this->handleMovement(this->getAngleForPoint(_playerReference->getNode()->getPosition()));
    } else {
        
        Point thisPosition = this->getNode()->getPosition();
        
        if (!_isRandomMoving || _randomMovingTime <= 0) {
            _walkingPoint = Point(thisPosition.x, thisPosition.y);
            _randomMovingTime = kEnemyRandomTime;
        }
        
        while (abs(_walkingPoint.x - thisPosition.x) <= 5 && abs(_walkingPoint.y - thisPosition.y) <= 5) {
            int x = (thisPosition.x + rand() % (kEnemyRandomRange + 1)) - kEnemyRandomRange / 2;
            int y = (thisPosition.y + rand() % (kEnemyRandomRange + 1)) - kEnemyRandomRange / 2;
            _walkingPoint = Point(x, y);
        }
        
        _isRandomMoving = true;
        GameObject::handleMovement(this->getAngleForPoint(_walkingPoint));
    }
}

void Enemy::handleMovement(float angle) {
    
    angle += 40;
    if (angle > 360)
        angle -= 360;
    
    GameObject::handleMovement(angle);
}

void Enemy::update(float dt) {
    if(_state == GameObjectStateDying || _state == GameObjectStateDead)
		return;

    GameObject::update(dt);
    
    Sprite *sprite = (Sprite *)_node;
    if (sprite->getOpacity() < 255) {
        int newOpacity = sprite->getOpacity() + dt * 500;
        if (newOpacity > 255) newOpacity = 255;
        sprite->setOpacity(newOpacity);
    }
    
    _randomMovingTime -= dt;
    
    this->executeWalkAnimation();
    
}

void Enemy::updatePosition(Point position) {
    position.y += 6;
    _node->setPosition(position);
}

void Enemy::die() {
    
    this->setState(GameObjectStateDying);
    
    FadeOut* dying = FadeOut::create(1.5f);
    JumpTo* deathLeap = JumpTo::create(0.75f, Point(_node->getPosition().x, _node->getPosition().y), 40, 1);
    Sequence* deathSequence = Sequence::create(dying, CallFunc::create([this]() {
        this->setState(GameObjectStateDead);
    }),
                                               NULL);
    Spawn* deathRitual = Spawn::create(deathLeap, deathSequence, NULL);
    
    _node->runAction(deathRitual);
    
}

bool Enemy::isNearPlayer() {
    
    Point playerPosition = _playerReference->getNode()->getPosition();
    Point thisPosition = this->getNode()->getPosition();
    
    float xDifference = abs(playerPosition.x - thisPosition.x);
    float yDifference = abs(playerPosition.y - thisPosition.y);
    
    return (_sightRange == 0 || xDifference <= _sightRange || yDifference <= _sightRange);

}