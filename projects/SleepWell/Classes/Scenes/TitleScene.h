#ifndef __SleepWell__Titlescene__
#define __SleepWell__Titlescene__

#include "../BaseLayer.h"

using namespace cocos2d;

class TitleScene : public BaseLayer
{
    
private:
    
	Menu *_menu;
    
	LayerColor *_help;
	bool _helpShown;
    
	LayerColor *_apneaExplanation;
    bool _apneaExplanationShown;
    
	LayerColor *_credits;
	bool _creditsShown;

public:

	virtual ~TitleScene();

	virtual bool init();
	static Scene* scene();
	CREATE_FUNC(TitleScene);
    
    virtual void registerWithTouchDispatcher();
    bool ccTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void ccTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void buttonAny(bool pressed);
    
    void enableMenus(bool enabled);
    
};

#endif // __SleepWell__Titlescene__