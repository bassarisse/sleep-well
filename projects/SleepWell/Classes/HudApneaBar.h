
#ifndef __SleepWell__HealthBar__
#define __SleepWell__HealthBar__

#include "cocos2d.h"

using namespace cocos2d;

class HudApneaBar : public LayerColor
{
    
private:
    
	LayerGradient* _bar;
    
    
public:
    
    virtual bool init();
    
	void setLevel(float level);
    
	void blinkBar();

};


#endif /* defined(__SleepWell__BaseLayer__) */
