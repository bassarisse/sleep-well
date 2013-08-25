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
        
        _player->setApneaLevel(_player->getApneaLevel() - ((Pulse *)gameObject)->getPower() * (0.5f - _level * 0.025f));
        
    }
    
}
