
#include "Player.h"
#include "SimpleAudioEngine.h"
#include "../GameState.h"

using namespace CocosDenshion;

Player::~Player() {
}

void Player::addFixtures() {
    
    float width = _node->getContentSize().width * 0.32f;
    //float height = _node->getContentSize().height * 0.2f;
    
    this->addCircularFixture(width / 2);
    
}

bool Player::init(b2World *world, Dictionary *properties, GamePlay* gameScreen) {
    
    _level = GameState::getInstance()->getActLevel();
    _isFollowingPoint = false;
	_pointToFollow = Point(0, 0);
	_damageTime = 0.0f;
    _actionCooldownTime = 0.0f;
    _lastMovementAngle = -1;
    _node = Sprite::createWithSpriteFrameName("neuron_1.png");
    _power = 0.0f;
    _apneaLevel = 100.0f;
    
    _spriteFrameName = "neuron";
    
    this->setType(GameObjectTypePlayer);
    
	_gameScreen = gameScreen;

    if (!GameObject::init(world, properties))
        return false;
    
    _useIdleFrame = false;
    
    return true;
}

void Player::update(float dt) {
  
    GameObject::update(dt);
    
    _damageTime -= dt;
    if (_damageTime < 0)
        _damageTime = 0;
    
    _actionCooldownTime -= dt;
    if (_actionCooldownTime < 0)
        _actionCooldownTime = 0;
    
    int colorAdd = 100 + 155 * ((kDamageTime - _damageTime) / kDamageTime);
    Sprite *thisSprite = (Sprite *)this->getNode();
    thisSprite->setColor(Color3B(255, colorAdd, colorAdd));
    
    _power += dt * kPowerChargeFactor;
    if (_power > 100) _power = 100;
    
    _apneaLevel -= dt * (kApneaFactor - _level * kApneaLevelDifference);
    
    if (_apneaLevel < 0)
        _apneaLevel = 0;
    
    this->executeWalkAnimation();
    
}

float Player::getSpeed() {
    return _speedFactor;
}

void Player::handleCollision(GameObject *gameObject) {
    
    switch(gameObject->getType()) {
            
        case GameObjectTypeBadNeuron:
        {
            if (_damageTime > 0)
                return;
            _damageTime = kDamageTime;
            
			SimpleAudioEngine::getInstance()->playEffect("player_hit.wav");
            
            _power *= kPowerDamage;
            
            Point thisPosition = _node->getPosition();
            Point enemyPosition = gameObject->getNode()->getPosition();
            
            float angleFromEnemy = 180 + atan2(enemyPosition.y - thisPosition.y,
                                               enemyPosition.x - thisPosition.x) * 180 / M_PI;
            
            float angleFromPlayer = 180 + atan2(thisPosition.y - enemyPosition.y,
                                                thisPosition.x - enemyPosition.x) * 180 / M_PI;
            
            float playerImpulseX = kDamageImpulse * cos(angleFromEnemy * M_PI / 180.0f);
            float playerImpulseY = kDamageImpulse * sin(angleFromEnemy * M_PI / 180.0f);
            
            float enemyImpulseX = kDamageImpulse * 0.96f * cos(angleFromPlayer * M_PI / 180.0f);
            float enemyImpulseY = kDamageImpulse * 0.96f * sin(angleFromPlayer * M_PI / 180.0f);
            
            b2Vec2 playerImpulse = b2Vec2(playerImpulseX, playerImpulseY);
            b2Vec2 enemyImpulse = b2Vec2(enemyImpulseX, enemyImpulseY);
            
            gameObject->getBody()->ApplyLinearImpulse(enemyImpulse, gameObject->getBody()->GetWorldCenter());
            _body->ApplyLinearImpulse(playerImpulse, _body->GetWorldCenter());
            _gameScreen->shakeScreen();
            
        }
            break;
            
            /*
             * Collectables
             */
            
            /*
             * Static
             */
            
        case GameObjectTypeNerve:
            
            
            
            break;
            
        default:
            break;
    }
    
}

void Player::followPoint(Point point) {
    _isFollowingPoint = true;
    _pointToFollow = point;
}

void Player::stopFollowingPoint() {
    _isFollowingPoint = false;
}

void Player::handleMovement() {
    
    if (_isFollowingPoint) {
        this->handleMovement(this->getAngleForPoint(_pointToFollow));
        return;
    }
    
    GameObject::handleMovement();
}

void Player::handleMovement(float angle) {
    
    if (_damageTime > 0 || _actionCooldownTime > 0)
        return;
    
    if (angle >= 0)
        _lastMovementAngle = angle;
    
    GameObject::handleMovement(angle);
}

void Player::action() {
    
    if (_actionCooldownTime > 0 || _lastMovementAngle == -1)
        return;
    _actionCooldownTime = kPulseCooldownTime;
    
    float angle = _lastMovementAngle > 180 ? _lastMovementAngle - 180 : _lastMovementAngle + 180;
    
    float playerImpulseX = kPulseFactor * cos(angle * M_PI / 180.0f);
    float playerImpulseY = kPulseFactor * sin(angle * M_PI / 180.0f);
    
    _gameScreen->addPulse(_lastMovementAngle, _power);
    _power = 0;
    
    b2Vec2 playerImpulse = b2Vec2(playerImpulseX, playerImpulseY);
    
    _body->ApplyLinearImpulse(playerImpulse, _body->GetWorldCenter());
    
}
