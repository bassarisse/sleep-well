
#include "GameObject.h"

#include "GameState.h"

GameObject::GameObject() {
    
}

GameObject::~GameObject() {
    
}

void GameObject::setProperties(Dictionary* properties)
{
    if (properties && _hasNode) {
        float x = floatValue(properties->objectForKey("x"));
        float y = floatValue(properties->objectForKey("y"));
        float halfwidth = floatValue(properties->objectForKey("width")) / 2.0f;
        float halfheight = floatValue(properties->objectForKey("height")) / 2.0f;
        
        x += halfwidth;
        y += halfheight;
        
        _node->setPosition(x, y);
    }
}

void GameObject::addCircularFixture(float radius) {
    
    b2CircleShape shape;
    shape.m_radius = radius * _node->getScale() / PTM_RATIO;
    this->createFixture(&shape);
    
}

void GameObject::addCircularFixture(float x, float y, float radius) {
    
    b2CircleShape shape;
    shape.m_p = b2Vec2(x * _node->getScale() / PTM_RATIO, y * _node->getScale() / PTM_RATIO);
    shape.m_radius = radius * _node->getScale() / PTM_RATIO;
    this->createFixture(&shape);
    
}

void GameObject::addRectangularFixture(float width, float height) {
    
    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f * _node->getScale() / PTM_RATIO, height / 2.0f * _node->getScale() / PTM_RATIO);
    this->createFixture(&shape);
    
}

void GameObject::addRectangularFixture(float x, float y, float width, float height) {
    
    b2PolygonShape shape;
    shape.SetAsBox(width / 2.0f * _node->getScale() / PTM_RATIO,
                   height / 2.0f * _node->getScale() / PTM_RATIO,
                   b2Vec2(x * _node->getScale() / PTM_RATIO, y * _node->getScale() / PTM_RATIO),
                   0);
    this->createFixture(&shape);
    
}

void GameObject::createFixture(b2Shape *shape) {
    
    // some properties can be modified later by iterating on body's fixtures (e.g. fixture->SetFriction())
    
    b2FixtureDef fixtureDef;
    
    fixtureDef.shape = shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData = this;
    fixtureDef.isSensor = _isSensor;
    
    _body->CreateFixture(&fixtureDef);
    
}

void GameObject::addFixtures() {
    
}

void GameObject::createSensorFixture(b2Shape *shape, SensorTypeContainer *sensorTypeContainer) {
    
    //fixture definition
    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.density = 1;
    fixtureDef.isSensor = true;
    b2Fixture* footSensorFixture = _body->CreateFixture(&fixtureDef);
    
    footSensorFixture->SetUserData( sensorTypeContainer );
    
}

float GameObject::getSpeed() {
    return _speedFactor;
}

void GameObject::handleCollisions() {
    
	for(std::vector<GameObject*>::size_type i = 0; i < _contacts.size(); i++)
	{
		GameObject* collisionObject = _contacts[i];
		if(!collisionObject || collisionObject->getState() == GameObjectStateDead)
			continue;
		
        this->handleCollision(collisionObject);
		
	}
    
}

void GameObject::handleCollision(GameObject *gameObject) {
    
}

Node* GameObject::getNode() {
    return _node;
}

b2Body* GameObject::getBody() {
	return _body;
}

void GameObject::addContact(GameObject *contact) {
    _contacts.push_back(contact);
}

void GameObject::removeContact(GameObject *contact) {
    
    std::vector<GameObject *>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), contact);
    if (pos != _contacts.end()) {
        _contacts.erase(pos);
    }
    
}

void GameObject::addBodyToWorld(b2World *world) {
    
    if (!_hasNode)
        return;
    
    b2BodyDef bodyDef;
    
    bodyDef.type = _isKinematic ? b2_kinematicBody : b2_dynamicBody;
    bodyDef.position.Set(_node->getPositionX() / PTM_RATIO, _node->getPositionY() / PTM_RATIO);
    bodyDef.userData = this;
    bodyDef.fixedRotation = true;
    
    _body = world->CreateBody(&bodyDef);
    
}

bool GameObject::init(b2World *world, Dictionary *properties) {
    return this->init(world, properties, false);
}

bool GameObject::init(b2World *world, Dictionary *properties, bool isSensor) {
    return this->init(world, properties, isSensor, true);
}

bool GameObject::init(b2World *world, Dictionary *properties, bool isSensor, bool hasNode) {
    return this->init(world, properties, isSensor, hasNode, false);
}

bool GameObject::init(b2World *world, Dictionary *properties, bool isSensor, bool hasNode, bool isKinematic) {
    
    _speedFactor = 0.0f;
    _hasNode = hasNode;
    _isSensor = isSensor;
    _isKinematic = isKinematic;
    _shouldFlipSprite = true;
    _useIdleFrame = true;
    _lastDirection = kDirectionDown;
    _lastVerticalDirection = kDirectionDown;
    _lastHorizontalDirection = kDirectionRight;
	_IsDestroyed = false;
    
    this->setMovingVerticalState(MovingStateVerticalStopped);
    this->setMovingHorizontalState(MovingStateHorizontalStopped);
    this->setProperties(properties);
    this->addBodyToWorld(world);
    this->addFixtures();
    
    return true;
}

void GameObject::update(float dt) {
    
	if (_contacts.size() > 0) {
		this->handleCollisions();
		_contacts.clear();
	}
    
    if (!_hasNode)
        return;
    
    b2Vec2 position = _body->GetPosition();
    
    int x = position.x * PTM_RATIO;
    int y = position.y * PTM_RATIO;
    
    this->updatePosition(Point(x, y));
    
    int zOrder = - 10 - y + (_node->getContentSize().height * _node->getScaleY()) / 2;
    
    _node->getParent()->reorderChild(_node, zOrder);
    //_node->setVertexZ(- 10 - y);
    
    this->handleMovement();
    
    if (_shouldFlipSprite) {
        auto sprite = ((Sprite *)_node);
        sprite->setFlipX(_lastHorizontalDirection == kDirectionRight);
        sprite->setFlipY(_lastVerticalDirection == kDirectionUp);
    }
    
}

void GameObject::updatePosition(Point position) {
    _node->setPosition(position);
}

void GameObject::handleMovement() {
    
    float angle = 0;
    
    switch (this->getMovingHorizontalState())
    {
        case MovingStateLeft:
            switch (this->getMovingVerticalState())
        {
            case MovingStateDown: angle = 225; break;
            case MovingStateVerticalStopped: angle = 180; break;
            case MovingStateUp: angle = 135; break;
        }
            break;
        case MovingStateHorizontalStopped:
            switch (this->getMovingVerticalState())
        {
            case MovingStateDown: angle = 270; break;
            case MovingStateVerticalStopped: angle = -1; break;
            case MovingStateUp: angle = 90; break;
        }
            break;
        case MovingStateRight:
            switch (this->getMovingVerticalState())
        {
            case MovingStateDown: angle = 315; break;
            case MovingStateVerticalStopped: angle = 0; break;
            case MovingStateUp: angle = 45; break;
        }
            break;
    }
    
    this->handleMovement(angle);
    
}

void GameObject::handleMovement(float angle) {
    
    if (this->getState() == GameObjectStateDead)
        return;
    
    this->setState(this->getMovingHorizontalState() == MovingStateHorizontalStopped && this->getMovingVerticalState() == MovingStateVerticalStopped ? GameObjectStateStanding : GameObjectStateWalking);
    
    float x = (kWalkForce + this->getSpeed()) * cos(angle * M_PI / 180.0f);
    float y = (kWalkForce + this->getSpeed()) * sin(angle * M_PI / 180.0f);
    
    if (y > 0) {
        float multiplier = kImpulseYCut - kImpulseYCutLevelDifference * GameState::getInstance()->getActLevel();
        float difference = 1.0f - multiplier;
        y *= 1.0f - difference * _body->GetGravityScale();
    }
    
    float desiredXVel = 0;
    float desiredYVel = 0;
    
    b2Vec2 vel = _body->GetLinearVelocity();
    
    switch (this->getMovingHorizontalState())
    {
        case MovingStateLeft:     desiredXVel = b2Max( vel.x - (abs(x)/3.0f), x); break;
        case MovingStateHorizontalStopped:  desiredXVel = vel.x * 0.9f; break;
        case MovingStateRight:    desiredXVel = b2Min( vel.x + (abs(x)/3.0f), x); break;
    }
    
    switch (this->getMovingVerticalState())
    {
        case MovingStateDown:     desiredYVel = b2Max( vel.y - (abs(y)/3.0f), y); break;
        case MovingStateVerticalStopped:  desiredYVel = vel.y * 0.9f; break;
        case MovingStateUp:    desiredYVel = b2Min( vel.y + (abs(y)/3.0f), y); break;
    }
    
    float xVelChange = desiredXVel - vel.x;
    float yVelChange = desiredYVel - vel.y;
    float xImpulse = _body->GetMass() * xVelChange;
    float yImpulse = _body->GetMass  () * yVelChange;
    
    _body->ApplyLinearImpulse( b2Vec2(xImpulse, yImpulse), _body->GetWorldCenter() );
    
}

float GameObject::getAngleForPoint(Point point) {
    
	Point thisPosition = _node->getPosition();
	
	float angle = 180 + atan2(thisPosition.y - point.y,
                              thisPosition.x - point.x) * 180 / M_PI; //degree conversion
    
	if(angle == 0 || angle == 360)
	{
		this->setMovingHorizontalState(MovingStateRight);
		this->setMovingVerticalState(MovingStateVerticalStopped);
	} else if(angle > 0 && angle < 90)
	{
		this->setMovingHorizontalState(MovingStateRight);
		this->setMovingVerticalState(MovingStateUp);
	} else if(angle == 90)
	{
		this->setMovingHorizontalState(MovingStateHorizontalStopped);
		this->setMovingVerticalState(MovingStateUp);
	} else if(angle > 90 && angle < 180)
	{
		this->setMovingHorizontalState(MovingStateLeft);
		this->setMovingVerticalState(MovingStateUp);
	} else if(angle == 180)
	{
		this->setMovingHorizontalState(MovingStateLeft);
		this->setMovingVerticalState(MovingStateVerticalStopped);
	} else if(angle > 180 && angle < 270)
	{
		this->setMovingHorizontalState(MovingStateLeft);
		this->setMovingVerticalState(MovingStateDown);
	} else if(angle == 270)
	{
		this->setMovingHorizontalState(MovingStateHorizontalStopped);
		this->setMovingVerticalState(MovingStateDown);
	} else if(angle > 270 && angle < 360)
	{
		this->setMovingHorizontalState(MovingStateRight);
		this->setMovingVerticalState(MovingStateDown);
	}
    
    kDirection newHorizontalDirection = kDirectionLeft;
    kDirection newVerticalDirection = kDirectionUp;
    
    if (this->getMovingHorizontalState() == MovingStateRight)
        newHorizontalDirection = kDirectionRight;
    else if (this->getMovingHorizontalState() == MovingStateLeft)
        newHorizontalDirection = kDirectionLeft;
    
    if (this->getMovingVerticalState() == MovingStateUp) {
        newVerticalDirection = kDirectionUp;
    } else if (this->getMovingVerticalState() == MovingStateDown)
        newVerticalDirection = kDirectionDown;
    
    if (_lastVerticalDirection != newVerticalDirection)
        this->getNode()->stopActionByTag(kWalkActionTag);
    
    _lastHorizontalDirection = newHorizontalDirection;
    _lastVerticalDirection = newVerticalDirection;
	
    return angle;
}

bool GameObject::changeDirection(kDirection direction) {
    
    if (_lastDirection == direction)
        return false;
    
    _lastDirection = direction;
    
    if (direction == kDirectionUp || direction == kDirectionDown)
        _lastVerticalDirection = direction;
    if (direction == kDirectionLeft || direction == kDirectionRight)
        _lastHorizontalDirection = direction;
    
    if (_node->getActionByTag(kWalkActionTag)) {
        _node->stopActionByTag(kWalkActionTag);
        this->setIdleFrame();
    }
    
    return true;
}

void GameObject::executeWalkAnimation() {
    
    if (!_useIdleFrame || this->getState() == GameObjectStateWalking) {
        
        float speed = (abs(kWalkForce + this->getSpeed())) / 1.9f;
        
        Speed *walkAction = dynamic_cast<Speed*>(_node->getActionByTag(kWalkActionTag));
        
        if (!walkAction) {
            
            SpriteFrameCache *spriteCache = SpriteFrameCache::getInstance();
            
            Animation *anim = Animation::create();
            anim->setDelayPerUnit(0.7f);
            anim->setRestoreOriginalFrame(true);
            
            anim->addSpriteFrame(spriteCache->getSpriteFrameByName(String::createWithFormat("%s_1.png", _spriteFrameName)->getCString()));
            anim->addSpriteFrame(spriteCache->getSpriteFrameByName(String::createWithFormat("%s_2.png", _spriteFrameName)->getCString()));
            anim->addSpriteFrame(spriteCache->getSpriteFrameByName(String::createWithFormat("%s_3.png", _spriteFrameName)->getCString()));
            anim->addSpriteFrame(spriteCache->getSpriteFrameByName(String::createWithFormat("%s_2.png", _spriteFrameName)->getCString()));
            
            walkAction = Speed::create(RepeatForever::create(Animate::create(anim)), speed);
            walkAction->setTag(kWalkActionTag);
            
            _node->stopAllActions();
            _node->runAction(walkAction);
            
        } else {
            
            walkAction->setSpeed(speed);
            
        }
        
    } else if (this->getState() == GameObjectStateStanding) {
        
        _node->stopAllActions();
        this->setIdleFrame();
        
    }
}

void GameObject::setIdleFrame () {
    
    ((Sprite *)_node)->setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("%s_1.png", _spriteFrameName)->getCString()));
    
}

void GameObject::destroyBody(b2World* world) {
	world->DestroyBody(this->getBody());
	this->_IsDestroyed = true;
}