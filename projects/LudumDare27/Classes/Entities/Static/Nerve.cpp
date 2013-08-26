#include "Nerve.h"

#include "Player.h"
#include "../Abstract/Pulse.h"
#include "../../GameState.h"

bool Nerve::init(b2World *world, Dictionary *properties, Player *player) {
	
	this->setType(GameObjectTypeNerve);
    
    _player = player;
    _level = GameState::getInstance()->getActLevel();
    
    if (!GameObject::init(world, properties, false, false))
        return false;
    
    return true;
}

void Nerve::handleCollision(GameObject *gameObject) {
    
    if (gameObject->getType() == GameObjectTypePulse) {
        
        auto pulse = ((Pulse *)gameObject);
        
        if (pulse->getNerveHit())
            return;
        pulse->setNerveHit(true);
        
        float apnea = _player->getApneaLevel();
        float power = pulse->getPower();
        float multiplier = kPowerFinalDifference - _level * kPowerLevelDifference;
        
        apnea -= power * multiplier * 2;
        
        _player->setApneaLevel(apnea);
        _player->getGameScreen()->shakeScreen();
        _player->getGameScreen()->flashScreen();
        
    }
    
}