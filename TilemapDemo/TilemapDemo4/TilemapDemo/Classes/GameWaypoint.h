//
//  GameWaypoint.h
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-21.
//
//

#ifndef __TilemapDemo__GameWaypoint__
#define __TilemapDemo__GameWaypoint__

#include "cocos2d.h"

USING_NS_CC;

class GameWaypoint : public CCObject {
    CC_SYNTHESIZE_READONLY(CCPoint, _position, Position);
    CC_SYNTHESIZE(float, _lastDistance, LastDistance);
    
public:
    static GameWaypoint* createWithPosition(const CCPoint &p);
    GameWaypoint(const CCPoint &p);
};

#endif /* defined(__TilemapDemo__GameWaypoint__) */
