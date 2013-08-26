#ifndef __LUDUMDARE_LEVELTRANSITION_H__
#define __LUDUMDARE_LEVELTRANSITION_H__

#include "../BaseLayer.h"

using namespace cocos2d;

class LevelTransition : public BaseLayer
{

public:
    
    CREATE_FUNC(LevelTransition);

	virtual bool init();
	static Scene* scene();
    virtual void onEnterTransitionDidFinish();

};

#endif // __LUDUMDARE_LEVELTRANSITION_H__