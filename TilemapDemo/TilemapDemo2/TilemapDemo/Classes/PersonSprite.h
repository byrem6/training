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

typedef enum {
    WalkingDirectionBack = 1,
    WalkingDirectionRightBack = 2,
    WalkingDirectionRight = 3,
    WalkingDirectionRightFront = 4,
    WalkingDirectionFront = 5,
    WalkingDirectionLeftFront = 6,
    WalkingDirectionLeft = 7,
    WalkingDirectionLeftBack = 8
} WalkingDirection;

class PersonSprite : public cocos2d::CCSprite {
public:
    static PersonSprite* create();
public:
    PersonSprite(void);
    virtual ~PersonSprite(void);
    virtual bool init(void);
    virtual void walking(WalkingDirection direction);
private:
    cocos2d::CCArray *m_pAnimates;
};

#endif
