//
//  AStarPathNode.h
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-20.
//
//

#ifndef __TilemapDemo__AStarPathNode__
#define __TilemapDemo__AStarPathNode__

#include "cocos2d.h"
#include "AStarNode.h"

USING_NS_CC;

class AStarPathNode : public CCObject {
    CC_SYNTHESIZE(AStarNode*, _node, Node);
    CC_SYNTHESIZE(AStarPathNode*, _previous, Previous);
    CC_SYNTHESIZE(float, _cost, Cost);
public:
    AStarPathNode(void);
    static AStarPathNode* createWithAStarNode(AStarNode *node);
    static CCArray* findPath(AStarNode* fromNode, AStarNode *toNode);
    static AStarPathNode* lowestCostNodeInArray(CCArray *a);
    static bool isPathNodeInList(AStarPathNode *a, CCArray *list);
};

#endif /* defined(__TilemapDemo__AStarPathNode__) */
