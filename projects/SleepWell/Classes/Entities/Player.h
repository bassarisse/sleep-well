
#ifndef __SleepWell__Player__
#define __SleepWell__Player__

#include "GameObject.h"
#include "../GamePlay.h"

class Player : public GameObject {
    
private:
        
protected:
    
    int _level;
    
    bool _isFollowingPoint;
    Point _pointToFollow;
    float _damageTime;
    float _actionCooldownTime;
    float _lastMovementAngle;
    
    virtual void addFixtures();
    virtual float getSpeed();
	virtual void handleCollision(GameObject *gameObject);
	virtual void handleMovement();
	virtual void handleMovement(float angle);
    
public:
    
    virtual ~Player();
    
    virtual bool init(b2World *world, Dictionary *properties, GamePlay* gameScreen);
    virtual void update(float dt);
    virtual void followPoint(Point point);
    virtual void stopFollowingPoint();
    virtual void action();
    
	CC_SYNTHESIZE(GamePlay*, _gameScreen, GameScreen);
	CC_SYNTHESIZE(float, _power, Power);
	CC_SYNTHESIZE(float, _apneaLevel, ApneaLevel);

};

#endif /* defined(__SleepWell__Player__) */
