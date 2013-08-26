
#ifndef __SleepWell__HealthBar__
#define __SleepWell__HealthBar__

#include "cocos2d.h"

using namespace cocos2d;

class HudApneaBar : public LayerColor
{
public:
    
    virtual bool init();
            
	void setLevel(float level);

	LayerColor* _bar;
    
	void blinkBar();

};


#endif /* defined(__SleepWell__BaseLayer__) */
