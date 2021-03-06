
#ifndef __SleepWell__Nerve__
#define __SleepWell__Nerve__

#include "GameObject.h"

class Player;

class Nerve : public GameObject {
    
protected:
    
    Player *_player;
    
    int _level;
	
public:
    
    virtual bool init(b2World *world, Dictionary *properties, Player *player);
    virtual void handleCollision(GameObject *gameObject);
    
};

#endif /* defined(__SleepWell__Nerve__) */
