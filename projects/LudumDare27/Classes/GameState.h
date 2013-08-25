//
//  GameState.h
//  LudumDare27
//
//  Created by Bruno Assarisse on 25/08/13.
//
//

#ifndef __LudumDare27__GameState__
#define __LudumDare27__GameState__

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

#endif /* defined(__LudumDare27__GameState__) */
