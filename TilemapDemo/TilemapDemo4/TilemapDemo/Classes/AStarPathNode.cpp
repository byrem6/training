//
//  AStarPathNode.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-20.
//
//

#include "AStarPathNode.h"

AStarPathNode::AStarPathNode(void)
{
    _node = NULL;
    _previous = NULL;
    _cost = 0.0f;
}

AStarPathNode* AStarPathNode::createWithAStarNode(AStarNode *node)
{
    if(!node){	//Can't have a path node without a real node
		return NULL;
	}
    AStarPathNode *pathNode = new AStarPathNode();
    if (pathNode) {
        pathNode->autorelease();
        pathNode->setNode(node);
        return pathNode;
    }
	CC_SAFE_DELETE(pathNode);
    return NULL;
}

CCArray* AStarPathNode::findPath(AStarNode* fromNode, AStarNode *toNode)
{
	if(fromNode->getPosition().x == toNode->getPosition().x
       && fromNode->getPosition().y == toNode->getPosition().y){
		return NULL;
    }
    
    CCArray *foundPath = CCArray::create();//new CCArray();
	
	CCArray *openList = CCArray::create();
	CCArray *closedList = CCArray::create();
	
	AStarPathNode *currentNode = NULL;
	AStarPathNode *aNode = NULL;
	
	AStarPathNode *startNode = AStarPathNode::createWithAStarNode(fromNode);
	AStarPathNode *endNode = AStarPathNode::createWithAStarNode(toNode);
    openList->addObject(startNode);
    
	while(openList->count() > 0){
		currentNode = AStarPathNode::lowestCostNodeInArray(openList);
        
		if( currentNode->getNode()->getPosition().x == endNode->getNode()->getPosition().x &&
           currentNode->getNode()->getPosition().y == endNode->getNode()->getPosition().y){
			
			//Path Found!
			aNode = currentNode;
			while(aNode && aNode->getPrevious() != NULL){
				//Mark path
                CCPoint *positon = new CCPoint(aNode->getNode()->getPosition().x, aNode->getNode()->getPosition().y);
                positon->autorelease();
				foundPath->addObject(positon);
				aNode = aNode->getPrevious();
			}
            CCPoint *positon = new CCPoint(aNode->getNode()->getPosition().x, aNode->getNode()->getPosition().y);
            positon->autorelease();
			foundPath->addObject(positon);
			return foundPath;
		}else{
			//Still searching
			closedList->addObject(currentNode);
			openList->removeObject(currentNode);
			unsigned int count = currentNode->getNode()->getNeighbors()->count();
			for(int i = 0; i < count; i++){
                AStarNode *node = (AStarNode *)currentNode->getNode()->getNeighbors()->objectAtIndex(i);
				AStarPathNode *aNode = AStarPathNode::createWithAStarNode(node);
                float cost = currentNode->getCost() + currentNode->getNode()->costToNode(aNode->getNode()) + aNode->getNode()->costToNode(endNode->getNode());
				aNode->setCost(cost);
				aNode->setPrevious(currentNode);
				
				if(aNode->getNode()->getActive()
                   && !AStarPathNode::isPathNodeInList(aNode, openList)
                   && !AStarPathNode::isPathNodeInList(aNode, closedList)){
					openList->addObject(aNode);
				}
			}
		}
	}
	
	//No Path Found
	return NULL;
}

AStarPathNode* AStarPathNode::lowestCostNodeInArray(CCArray *a)
{
    AStarPathNode *lowest = NULL;
	for(int i = 0; i < a->count(); i++){
		AStarPathNode *node = (AStarPathNode *)a->objectAtIndex(i);
		if(!lowest || node->getCost() < lowest->getCost()){
			lowest = node;
		}
	}
	return lowest;
}

bool AStarPathNode::isPathNodeInList(AStarPathNode *a, CCArray *list)
{
    for(int i = 0; i < list->count(); i++){
		AStarPathNode *b = (AStarPathNode *)list->objectAtIndex(i);
		if(a->getNode()->getPosition().x == b->getNode()->getPosition().x
           && a->getNode()->getPosition().y == b->getNode()->getPosition().y){
			return true;
		}
	}
	return false;
}
