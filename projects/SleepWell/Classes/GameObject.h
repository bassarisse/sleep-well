
#ifndef __SleepWell__GameObject__
#define __SleepWell__GameObject__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "Common.h"
#include "BAContactListener.h"

using namespace cocos2d;

class SensorTypeContainer {
public:
    SensorType sensorType;
};

class GameObject : public Object {
    
protected:
    
    b2Body *_body;
    Node *_node;
    bool _hasNode;
    bool _isSensor;
    bool _isKinematic;
    bool _shouldFlipSprite;
    bool _useIdleFrame;
    std::vector<GameObject *> _contacts;
    kDirection _lastDirection;
    kDirection _lastVerticalDirection;
    kDirection _lastHorizontalDirection;
    float _speedFactor;
    const char* _spriteFrameName;
    
    virtual void setProperties(Dictionary *properties);
    virtual void addCircularFixture(float radius);
    virtual void addCircularFixture(float x, float y, float radius);
    virtual void addRectangularFixture(float width, float height);
    virtual void addRectangularFixture(float x, float y, float width, float height);
    virtual void createFixture(b2Shape *shape);
    virtual void addFixtures();
    virtual void createSensorFixture(b2Shape *shape, SensorTypeContainer *sensorTypeContainer);
    virtual float getSpeed();
    virtual void executeWalkAnimation();
    virtual void setIdleFrame();
	virtual void handleCollisions();
	virtual void handleCollision(GameObject *gameObject);
    virtual float getAngleForPoint(Point point);
    
public:
    
    GameObject();
    virtual ~GameObject();
    
    Node* getNode();
	b2Body* getBody();

    virtual void addContact(GameObject *contact);
    virtual void removeContact(GameObject *contact);
    
    virtual void addBodyToWorld(b2World *world);
    virtual bool init(b2World *world, Dictionary *properties);
    virtual bool init(b2World *world, Dictionary *properties, bool isSensor);
    virtual bool init(b2World *world, Dictionary *properties, bool isSensor, bool hasNode);
    virtual bool init(b2World *world, Dictionary *properties, bool isSensor, bool hasNode, bool isKinematic);
    virtual void update(float dt);
    virtual void updatePosition(Point position);
    virtual void handleMovement();
    virtual void handleMovement(float angle);
    virtual bool changeDirection(kDirection direction);

	virtual void destroyBody(b2World* world);

    CC_SYNTHESIZE(MovingHorizontalState, _movingHorizontalState, MovingHorizontalState);
    CC_SYNTHESIZE(MovingVerticalState, _movingVerticalState, MovingVerticalState);
    CC_SYNTHESIZE(GameObjectState, _state, State);
    CC_SYNTHESIZE(GameObjectType, _type, Type);
	CC_SYNTHESIZE(bool, _isDestroyed, IsDestroyed);
};

#endif /* defined(__SleepWell__GameObject__) */