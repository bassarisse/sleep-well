//
//  Enemy.h
//  CampJam13
//
//  Created by Bruno Assarisse - 30/07/13
//
//

#ifndef __CampJam__Collectable__
#define __CampJam__Collectable__

#include "GameObject.h"

class Collectable : public GameObject {
    
protected:
    
    virtual void addFixtures();
    
public:
    
    virtual bool init(b2World *world, Dictionary *properties);
    virtual void update(float dt);
    
};

#endif /* defined(__CampJam__Collectable__) */
