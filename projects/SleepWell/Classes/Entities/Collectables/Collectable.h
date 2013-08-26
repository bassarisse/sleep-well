
#ifndef __SleepWell__Collectable__
#define __SleepWell__Collectable__

#include "GameObject.h"

class Collectable : public GameObject {
    
protected:
    
    virtual void addFixtures();
    
public:
    
    virtual bool init(b2World *world, Dictionary *properties);
    virtual void update(float dt);
    
};

#endif /* defined(__SleepWell__Collectable__) */
