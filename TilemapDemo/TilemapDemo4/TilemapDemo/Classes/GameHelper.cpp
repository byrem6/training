//
//  GameHelper.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-20.
//
//

#include "GameHelper.h"

float GameHelper::distance(CCPoint p1, CCPoint p2)
{
    return sqrt( pow( (p1.x-p2.x) ,2) + pow( (p1.y-p2.y) ,2) );
}

float GameHelper::radiansToDegrees(float r)
{
    return r * (180/PI_CONSTANT);
}

float GameHelper::vectorToRadians(CCPoint &vector)
{
    if(vector.y == 0){ vector.y = 0.000001f; }
	float baseRadians = atan(vector.x/vector.y);
	if(vector.y < 0){ baseRadians += PI_CONSTANT; }	//Adjust for -Y
	return baseRadians;
}
