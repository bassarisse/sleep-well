//
//  Common.h
//  CampJam
//
//  Created by Bruno Assarisse on 25/07/13.
//
//

#include "cocos2d.h"

using namespace cocos2d;

#ifndef CampJam_Common_h
#define CampJam_Common_h

#define PTM_RATIO 32.0f
#define kGravity -12
#define kImpulseYCut 0.72f
#define kWalkForce 6.0f

#define kDamageTime 0.3f
#define kPulseCooldownTime 0.3f
#define kDamageImpulse 20.0f
#define kPulseFactor 15.0f
#define kPulseForce 150.0f
#define kPulseWaveTime 0.75f
#define kEnemyRandomRange 600
#define kEnemyRandomTime 5.0f

#define kPowerChargeFactor 55.0f
#define kApneaFactor 9.0f

#define kWalkActionTag 500
#define kOverlayOpacity 210

#define greenLabelColor Color3B(150, 250, 150)
#define yellowLabelColor Color3B(250, 220, 100)
#define redLabelColor Color3B(240, 130, 120)

enum kDirection {
    kDirectionUp = 0,
    kDirectionDown,
    kDirectionLeft,
    kDirectionRight
};

enum MovingHorizontalState {
    MovingStateHorizontalStopped = 0,
    MovingStateLeft,
    MovingStateRight
};

enum MovingVerticalState {
    MovingStateVerticalStopped = 0,
    MovingStateUp,
    MovingStateDown
};

enum GameObjectState {
    GameObjectStateStanding = 0,
    GameObjectStateWalking,
    GameObjectStateTakingDamage,
    GameObjectStateAfterDamage,
	GameObjectStateDead,
	GameObjectStateDying
};

enum GameObjectType {
    GameObjectTypeUnknown = 0,
    GameObjectTypePlayer,
	GameObjectTypeBadNeuron,
	GameObjectTypeNerve,
	GameObjectTypePulse
};

enum SensorType {
    SensorTypeUnknown = 0
};

inline float floatValue(Object* o)
{
    String* string = dynamic_cast<String*>(o);
    if (string)
        return string->floatValue();
    return 0.0f;
}

inline const char* getDirectionName(kDirection direction) {
    switch (direction) {
        case kDirectionUp: return "up"; break;
        case kDirectionLeft: return "left"; break;
        case kDirectionRight: return "right"; break;
        default: return "down"; break;
    }
}

#endif
