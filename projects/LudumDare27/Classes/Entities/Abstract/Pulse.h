
#ifndef __CampJam2013__Pulse__
#define __CampJam2013__Pulse__

#include "GameObject.h"

class Player;

class Pulse : public GameObject {
    
protected:
    
    Player *_player;
    
    virtual void addFixtures();
	
public:
    
    virtual bool init(b2World *world, Dictionary *properties, Player *player, float angle, float power);
    
    CC_SYNTHESIZE(bool, _nerveHit, NerveHit);
    CC_SYNTHESIZE(float, _power, Power);
    
};

#endif /* defined(__CampJam2013__Pulse__) */
