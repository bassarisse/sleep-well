//
//  BAContactListener.cpp
//  CampJam
//
//  Created by Bruno Assarisse on 24/07/13.
//
//

#include "BAContactListener.h"

#include "GameObject.h"

BAContactListener::BAContactListener() : _contacts() {
}

BAContactListener::~BAContactListener() {
}

void BAContactListener::BeginContact(b2Contact* contact) {
    // We need to copy out the data because the b2Contact passed in
    // is reused.
    BAContact aContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(aContact);
    
    GameObject *gameObjectA = (GameObject *)aContact.fixtureA->GetUserData();
    GameObject *gameObjectB = (GameObject *)aContact.fixtureB->GetUserData();
    
    if (!gameObjectA || !gameObjectB)
        return;
    
    bool isPlayerContact = gameObjectA->getType() == GameObjectTypePlayer || gameObjectB->getType() == GameObjectTypePlayer;
    
    if (!isPlayerContact || gameObjectA->getType() == GameObjectTypePlayer)
        gameObjectA->addContact(gameObjectB);
    
    if (!isPlayerContact || gameObjectB->getType() == GameObjectTypePlayer)
        gameObjectB->addContact(gameObjectA);
}

void BAContactListener::EndContact(b2Contact* contact) {
    BAContact aContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    std::vector<BAContact>::iterator pos;
    pos = std::find(_contacts.begin(), _contacts.end(), aContact);
    if (pos != _contacts.end()) {
        /*
        GameObject *gameObjectA = (GameObject *)aContact.fixtureA->GetUserData();
        GameObject *gameObjectB = (GameObject *)aContact.fixtureB->GetUserData();
        
        if (gameObjectA)
            gameObjectA->removeContact(gameObjectB);
        
        if (gameObjectB)
            gameObjectB->removeContact(gameObjectA);
        */
        _contacts.erase(pos);
    }
}

void BAContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}

void BAContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}

std::vector<BAContact> BAContactListener::getContacts(GameObjectType gameObjectType) {
    
    std::vector<BAContact> foundContacts;
    
    for(std::vector<GameObject *>::size_type i = 0; i < _contacts.size(); i++) {
        BAContact aContact = _contacts[i];
        
        void *fixtureAUserData = aContact.fixtureA->GetUserData();
        void *fixtureBUserData = aContact.fixtureB->GetUserData();
        
        GameObject *gameObjectA = (GameObject *)fixtureAUserData;
        GameObject *gameObjectB = (GameObject *)fixtureBUserData;
        
        if ((gameObjectA && gameObjectA->getType() == gameObjectType) || (gameObjectB && gameObjectB->getType() == gameObjectType)) {
            foundContacts.push_back(aContact);
        }
        
    }
    
    return foundContacts;
    
}

std::vector<BAContact> BAContactListener::getContacts(GameObject *gameObject) {
    
    std::vector<BAContact> foundContacts;
    
    for(std::vector<GameObject *>::size_type i = 0; i < _contacts.size(); i++) {
        BAContact aContact = _contacts[i];
        
        void *fixtureAUserData = aContact.fixtureA->GetUserData();
        void *fixtureBUserData = aContact.fixtureB->GetUserData();
        
        GameObject *gameObjectA = (GameObject *)fixtureAUserData;
        GameObject *gameObjectB = (GameObject *)fixtureBUserData;
        
        if ((gameObjectA && gameObjectA == gameObject) || (gameObjectB && gameObjectB == gameObject)) {
            foundContacts.push_back(aContact);
        }
        
    }
    
    return foundContacts;
    
}

std::vector<BAContact> BAContactListener::getContacts(GameObject *gameObject, GameObjectType gameObjectType) {
    
    std::vector<BAContact> foundContacts;
    
    for(std::vector<GameObject *>::size_type i = 0; i < _contacts.size(); i++) {
        BAContact aContact = _contacts[i];
        
        void *fixtureAUserData = aContact.fixtureA->GetUserData();
        void *fixtureBUserData = aContact.fixtureB->GetUserData();
        
        GameObject *gameObjectA = (GameObject *)fixtureAUserData;
        GameObject *gameObjectB = (GameObject *)fixtureBUserData;
        
        if (gameObjectA && gameObjectB && ((gameObjectA == gameObject && gameObjectB->getType() == gameObjectType) || (gameObjectB == gameObject && gameObjectA->getType() == gameObjectType))) {
            foundContacts.push_back(aContact);
        }
        
    }
    
    return foundContacts;
    
}
