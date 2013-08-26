#ifndef __SleepWell__Gameoverscene__
#define __SleepWell__Gameoverscene__

#include "../BaseLayer.h"

using namespace cocos2d;

class GameoverScene : public BaseLayer
{
    
private:
    
    Menu *_menu;

public:

    CREATE_FUNC(GameoverScene);

	virtual bool init();
	static Scene* scene();

    void enableMenus(bool enabled);

};

#endif // __SleepWell__Gameoverscene__