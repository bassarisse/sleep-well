#ifndef __CAMPJAM_SCENE_H__
#define __CAMPJAM_SCENE_H__

#include "BaseLayer.h"
#include "Box2D/Box2D.h"
#include "BAContactListener.h"
#include "HudPowerBar.h"
#include "HudApneaBar.h"

using namespace cocos2d;

class B2DebugDrawLayer;
class EnemySpawnPoint;
class CollectableSpawnPoint;
class GameObject;
class Player;

class GamePlay : public BaseLayer
{
private:
    
    bool _buttonLeftPressed;
    bool _buttonRightPressed;
    bool _buttonUpPressed;
    bool _buttonDownPressed;
    bool _buttonAPressed;
    
    float _gameTime;
    bool _didFinish;
    
public:
    virtual ~GamePlay();
    
    CREATE_FUNC(GamePlay);
    
    virtual bool init();
    static Scene* scene();
    
    // Variables/properties
    
    Size _winSize;
    B2DebugDrawLayer *_debugLayer;
    BAContactListener *_contactListener;
    Layer *_mainLayer;
    SpriteBatchNode *_mainBatchNode;
    TMXTiledMap *_tiledMap;
    LabelBMFont *_timeLabel;
    bool _isTouching;
    Point _touchLocation;
    
    std::vector<MovingHorizontalState> _movingHorizontalStates;
    std::vector<MovingVerticalState> _movingVerticalStates;
    std::vector<GameObject *> _gameObjects;
    std::vector<GameObject *> _staticObjects;
    std::vector<EnemySpawnPoint *> _enemySpawnPoints;
    std::vector<CollectableSpawnPoint *> _collectableSpawnPoints;
    Player *_player;
    
	CC_SYNTHESIZE(HudPowerBar*, _hudPowerBar, HudPowerBar);
	CC_SYNTHESIZE(HudApneaBar*, _hudApneaBar, HudApneaBar);
    
	LayerColor* _pauseLayer;
	bool _isPaused;

    // Box2D
    
    b2World *_world;
    
    // Methods
    
    void update(float dt);
    
    GameObject* createGameObject(GameObjectType type, Dictionary *properties);
	void removeObject(GameObject *deadObject);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void ccTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void ccTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void ccTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    
    virtual void handleKey(int keyCode, bool pressed);
    virtual void buttonLeft(bool pressed);
    virtual void buttonRight(bool pressed);
    virtual void buttonUp(bool pressed);
    virtual void buttonDown(bool pressed);
    virtual void buttonA(bool pressed);
	virtual void buttonB(bool pressed);

	virtual void onEnter();
    
	virtual void showScore(Point positionToShow, int scoreAmount);
	virtual void shakeScreen();
	virtual void flashScreen();
	virtual void addPulse(float angle, float power);
};

#endif // __CAMPJAM_SCENE_H__
