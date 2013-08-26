
#ifndef __SleepWell__BadNeuron__
#define __SleepWell__BadNeuron__

#include "Enemy.h"

class BadNeuron : public Enemy {

protected:

	virtual void addFixtures();

public:

	virtual bool init(b2World *world, Dictionary *properties, Player* ref);

};


#endif /* defined(__SleepWell__BadNeuron__) */