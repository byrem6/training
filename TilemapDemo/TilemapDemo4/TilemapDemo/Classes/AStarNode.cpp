//
//  AStarNode.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-20.
//
//

#include "AStarNode.h"
#include "GameHelper.h"

AStarNode::AStarNode(void)
{
    _active = true;
    _neighbors = new CCArray();
    _costMultiplier = 1.0f;
}

AStarNode::~AStarNode(void)
{
    CC_SAFE_RELEASE_NULL(_neighbors);
}

bool AStarNode::init()
{
    return true;
}

/* Cost to node heuristic */
float AStarNode::costToNode(AStarNode *node)
{
    CCPoint src = ccp(_position.x, _position.y);
	CCPoint dst = ccp(node->getPosition().x, node->getPosition().y);
	float cost = GameHelper::distance(src, dst) * node->getCostMultiplier();
	return cost;
}

/* Helper method: Is a node in a given list? */
bool AStarNode::isNodeInList(AStarNode *a, CCArray *list)
{
    for(int i = 0; i < list->count(); i++){
		AStarNode *b = (AStarNode *)list->objectAtIndex(i);
		if(a->getPosition().x == b->getPosition().x
           && a->getPosition().y == b->getPosition().y){
			return true;
		}
	}
	return false;
}
