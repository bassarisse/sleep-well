
#ifndef __CampJam__CoffeeBar__
#define __CampJam__CoffeeBar__

#include "cocos2d.h"

using namespace cocos2d;

class HudPowerBar : public Layer
{
    
private:
    
    Sprite* _icon;
	LayerColor* _bar;
	Sprite* _gaugeSprite;
    
    
public:
    
    virtual bool init();
            
	void setLevel(float level);
    
	void blinkBar();
	void setPeaBerry(bool isPeaBerry);

};


#endif /* defined(__CampJam__BaseLayer__) */
