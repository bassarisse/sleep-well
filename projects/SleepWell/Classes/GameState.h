
#ifndef __SleepWell__GameState__
#define __SleepWell__GameState__

#include "cocos2d.h"

class GameState
{
private:
    //Constructor
    GameState();
    
    //Instance of the singleton
    static GameState* m_theInstance;
    
    std::vector<float> _actTimes;
    
    
public:
    //Get instance of singleton
    static GameState* getInstance();
    
	CC_SYNTHESIZE(int, _actlevel, ActLevel);
    
    void clearActTimes();
    void addActTime(float time);
    std::vector<float> getActTimes();
    
};

#endif /* defined(__SleepWell__GameState__) */
