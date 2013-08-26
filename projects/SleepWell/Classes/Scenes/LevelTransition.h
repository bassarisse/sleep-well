#ifndef __SleepWell__LevelTransition__
#define __SleepWell__LevelTransition__

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

#endif // __SleepWell__LevelTransition__