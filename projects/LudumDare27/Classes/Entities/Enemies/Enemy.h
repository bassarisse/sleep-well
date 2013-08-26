//
//  Enemy.h
//  CampJam13
//
//  Created by Murilo Clemente - 27/07/13
//
//

#ifndef __CampJam__Enemy__
#define __CampJam__Enemy__

#include "GameObject.h"
#include "../Player.h"

class Enemy : public GameObject {
    
protected:

	Player* _playerReference;
    
    float _randomMovingTime;
    bool _isRandomMoving;
    Point _walkingPoint;
    float _damageTime;
    float _currentMaxDamageTime;

public:
    
    virtual bool init(b2World *world, Dictionary *properties, Player* ref);
    virtual void update(float dt);
    
	virtual void handleCollision(GameObject *gameObject);
	virtual void handleMovement();
	virtual void handleMovement(float angle);
    virtual void die();
    virtual bool isNearPlayer();
    
    CC_SYNTHESIZE(float, _sightRange, SightRange);
    CC_SYNTHESIZE(bool, _randomMoveOnly, RandomMoveOnly);


};

#endif /* defined(__CampJam__Player__) */
