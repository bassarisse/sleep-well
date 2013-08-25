#ifndef __CAMPJAM_GAMEOVER_H__
#define __CAMPJAM_GAMEOVER_H__

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

#endif // __CAMPJAM_SCENE_H__