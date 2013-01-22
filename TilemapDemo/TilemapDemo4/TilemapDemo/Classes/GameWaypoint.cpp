//
//  GameWaypoint.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-21.
//
//

#include "GameWaypoint.h"

GameWaypoint* GameWaypoint::createWithPosition(const CCPoint &p)
{
    GameWaypoint *waypoint = new GameWaypoint(p);
    waypoint->autorelease();
    return waypoint;
}

GameWaypoint::GameWaypoint(const CCPoint &p)
{
    _position = p;
    _lastDistance = 1000000.0f;
}
