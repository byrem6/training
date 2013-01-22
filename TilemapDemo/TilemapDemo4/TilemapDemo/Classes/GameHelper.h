//
//  GameHelper.h
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-20.
//
//

#ifndef __TilemapDemo__GameHelper__
#define __TilemapDemo__GameHelper__

#include "cocos2d.h"

USING_NS_CC;

#define PI_CONSTANT 3.14159f

class GameHelper {
public:
    static float distance(CCPoint p1, CCPoint p2);
    static float radiansToDegrees(float r);
    static float vectorToRadians(CCPoint &vector);
};

#endif /* defined(__TilemapDemo__GameHelper__) */
