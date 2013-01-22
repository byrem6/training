//
//  AStarNode.h
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-20.
//
//

#ifndef TilemapDemo_AStarNode_h
#define TilemapDemo_AStarNode_h

#include "cocos2d.h"

USING_NS_CC;

class AStarNode : public CCObject {
    /** The node's position on our map */
    CC_SYNTHESIZE_PASS_BY_REF(CCPoint, _position, Position);
    /** An array of neighbor AStarNode objects */
    CC_SYNTHESIZE_READONLY(CCArray*, _neighbors, Neighbors);
    /** Is this node active? */
    CC_SYNTHESIZE(bool, _active, Active);
    /** Use this to multiply the normal cost to reach this node. */
    CC_SYNTHESIZE(float, _costMultiplier, CostMultiplier);
public:
    float costToNode(AStarNode* node);
    static bool isNodeInList(AStarNode* a, CCArray* list);
public:
    AStarNode(void);
    virtual ~AStarNode(void);
    virtual bool init();
    CREATE_FUNC(AStarNode);
};

#endif
