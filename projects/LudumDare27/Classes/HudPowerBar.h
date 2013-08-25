
#ifndef __CampJam__CoffeeBar__
#define __CampJam__CoffeeBar__

#include "cocos2d.h"

using namespace cocos2d;

class HudPowerBar : public Layer
{
    
private:
    
	LayerColor* _bar;
    
    
public:
    
    virtual bool init();
            
	void setLevel(float level);
    
	void blinkBar();

};


#endif /* defined(__CampJam__BaseLayer__) */
