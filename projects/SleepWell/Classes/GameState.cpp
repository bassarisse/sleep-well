
#include "GameState.h"

GameState* GameState::m_theInstance = nullptr;

GameState::GameState(): _actTimes()
{
    _actlevel = 1;
    
    _hours = 1 + rand() % 7;
    _minutes = rand() % 60;
    _seconds = rand() % 60;
    
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
    
    _hours = 1 + rand() % 7;
    _minutes = rand() % 60;
    _seconds = rand() % 60;
    
    _actTimes.clear();
}

void GameState::addActTime(float time) {
    _actlevel++;
    _actTimes.push_back(time);
}

std::vector<float> GameState::getActTimes() {
    return _actTimes;
}

float GameState::getTotalTime() {
    
    float totalTime = 0.0f;
    auto maxSize = _actTimes.size();
    
    for (auto i = maxSize - maxSize; i < maxSize; i++) {
        totalTime += _actTimes[i];
    }
    
    return totalTime;
}