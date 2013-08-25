
#ifndef __CampJam__HealthBar__
#define __CampJam__HealthBar__

#include "cocos2d.h"

using namespace cocos2d;

class HudApneaBar : public Layer
{
public:
    
    virtual bool init();
            
	void setLevel(float level);

	LayerColor* _bar;
    
	void blinkBar();

};


#endif /* defined(__CampJam__BaseLayer__) */
