//
//  GameState.cpp
//  LudumDare27
//
//  Created by Bruno Assarisse on 25/08/13.
//
//

#include "GameState.h"

GameState* GameState::m_theInstance = nullptr;

GameState::GameState(): _actTimes()
{
    _actlevel = 1;
}

GameState* GameState::getInstance()
{
    //If the singleton has no instance yet, create one
    if(nullptr == m_theInstance)
    {
        //Create an instance to the singleton
        m_theInstance = new GameState();
    }
    
    //Return the singleton object
    return m_theInstance;
}

void GameState::clearActTimes() {
    _actlevel = 1;
    _actTimes.clear();
}

void GameState::addActTime(float time) {
    _actlevel++;
    _actTimes.push_back(time);
}

std::vector<float> GameState::getActTimes() {
    return _actTimes;
}