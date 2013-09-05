
#ifndef __SleepWell__CoffeeBar__
#define __SleepWell__CoffeeBar__

#include "cocos2d.h"

using namespace cocos2d;

class HudPowerBar : public LayerColor
{
    
private:
    
	LayerGradient* _bar;
    
    
public:
    
    virtual bool init();
            
	void setLevel(float level);
    
	void blinkBar();

};


#endif /* defined(__SleepWell__BaseLayer__) */
