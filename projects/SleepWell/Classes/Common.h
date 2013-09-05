
#include "cocos2d.h"

using namespace cocos2d;

#ifndef __SleepWell__Common__
#define __SleepWell__Common__

#define PTM_RATIO 32.0f
#define kGravity -12
#define kImpulseYCut 0.72f
#define kImpulseYCutLevelDifference 0.02f
#define kWalkForce 6.0f

#define kStartingPower 0.0f
#define kStartingApneaLevel 100.0f
#define kMaxLevel 10

#define kEnemyRandomRange 600
#define kEnemyRandomTime 5.0f
#define kEnemyDamageTime 0.6f

#define kDamageTime 0.3f
#define kDamageImpulse 20.0f

#define kPulseCooldownTime 0.4f
#define kPulseFactor 15.0f
#define kPulseForce 50.0f
#define kPulseForcePowerFactor 0.5f
#define kPulseWaveTime 0.85f

#define kPowerChargeFactor 55.0f
#define kPowerDamage 0.0f
#define kPowerFinalDifference 0.25f
#define kPowerLevelDifference 0.0125f

#define kApneaFactor 5.0f
#define kApneaLevelDifference 0.12f

#define kWalkActionTag 500
#define kOverlayOpacity 220

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

#endif // __SleepWell__Common__
