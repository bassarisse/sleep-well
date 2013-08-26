
#ifndef __SleepWell__EnemySpawnPoint__
#define __SleepWell__EnemySpawnPoint__

#include "cocos2d.h"
#include "Common.h"

class GamePlay;
class GameObject;

using namespace cocos2d;

class EnemySpawnPoint : public Object {
    
private:
    GamePlay *_gamePlay;
    Dictionary *_properties;
    float _spawnTime;
    float _nextSpawnTime;
    int _maxObjects;
    std::vector<GameObject *> _gameObjects;
    
public:
    
    virtual ~EnemySpawnPoint();
    
    bool init(GamePlay *gamePlay, Dictionary *properties);
    void update(float dt);
    
};

#endif /* defined(__SleepWell__EnemySpawnPoint__) */
