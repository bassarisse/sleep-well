
#ifndef __CampJam__Man__
#define __CampJam__Man__

#include "Enemy.h"

class BadNeuron : public Enemy {

protected:

	virtual void addFixtures();

public:

	virtual bool init(b2World *world, Dictionary *properties, Player* ref);
    virtual void die();

};


#endif /* defined(__CampJam__Player__) */