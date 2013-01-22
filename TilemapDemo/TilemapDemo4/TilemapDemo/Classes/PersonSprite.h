//
//  PersonSprite.h
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-16.
//
//

#ifndef TilemapDemo_PersonSprite_h
#define TilemapDemo_PersonSprite_h

#include "cocos2d.h"
#include "GameWaypoint.h"

USING_NS_CC;

typedef enum {
    WalkingDirectionNone = 0,
    WalkingDirectionUp = 1,
    WalkingDirectionRightUp = 2,
    WalkingDirectionRight = 3,
    WalkingDirectionRightDown = 4,
    WalkingDirectionDown = 5,
    WalkingDirectionLeftDown = 6,
    WalkingDirectionLeft = 7,
    WalkingDirectionLeftUp = 8,
    WalkingDirectionAll = 9
} WalkingDirection;

class PersonSprite : public CCSprite {
    CC_SYNTHESIZE_RETAIN(CCArray*, _waypoints, Waypoints);
public:
    static PersonSprite* create();
public:
    PersonSprite(void);
    virtual ~PersonSprite(void);
    virtual bool init(void);
    virtual void processWaypoints(void);
    virtual void animateActor(void);
private:
    CCArray *m_pAnimates;
    int _timesBlocked;
    WalkingDirection _direction;
    void runWithVector(CCPoint &v);
    void stopRunning(void);
};

#endif
