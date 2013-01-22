//
//  TileDemoScene.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-8.
//
//

#include "TileDemoScene.h"
#include "CCString.h"
#include "SimpleAudioEngine.h"
#include "AStarPathNode.h"

CCScene* TileDemo::scene()
{
    CCScene *scene = CCScene::create();
    TileDemo *layer = TileDemo::create();
    scene->addChild(layer);
    
    layer->runGame();
    
    return scene;
}

TileDemo::~TileDemo()
{
    CC_SAFE_RELEASE(_grid);
}

bool TileDemo::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    _map = CCTMXTiledMap::create("s400.tmx");
    this->addChild(_map);
    
    CCTMXLayer *eventLayer = _map->layerNamed("GameEventLayer");
    eventLayer->setVisible(false);
    
    CCSize s = _map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);
    
    CCPoint pos = eventLayer->positionAt(ccp(9, 99));
    CCLog("Position: %f, %f, ", pos.x, pos.y);
    
    _person = PersonSprite::create();
    _person->setAnchorPoint(ccp(0, 0));
    _person->setPosition(pos);
    _map->addChild(_person);
    //person->walking(WalkingDirectionLeftFront);
    
    this->setTouchEnabled(true);
    
    return true;
}

void TileDemo::runGame()
{
    //Shorter variable names
	float mw = _map->getMapSize().width;
	float mh = _map->getMapSize().height;
	float tw = _map->getTileSize().width;
	float th = _map->getTileSize().height;
    
    CCLog("mw: %i, mh: %i, tw: %i, th: %i", (int)mw, (int)mh, (int)tw, (int)th);
    
    //Create 2D array (grid)
	_grid = new CCArray((int)mw);
	for(int x=0; x<mw; x++){
        _grid->addObject(CCArray::createWithCapacity((int)mh));
	}
    
    //Create active and inactive nodes determined by the "GameEventLayer" TMX layer
	CCTMXLayer *eventLayer = _map->layerNamed("GameEventLayer");
	for(int x=0; x<mw; x++){
		for(int y=0; y<mh; y++){
            bool isPath = false;
			//Add a node
			AStarNode *node = AStarNode::create();
			node->setPosition(ccp(x*tw + tw/2, y*th + th/2));
            int tileGID = eventLayer->tileGIDAt(ccp(x,y));
            if (tileGID != 0) {
                CCDictionary *properties = _map->propertiesForGID(tileGID);
                if (properties) {
                    isPath = properties->valueForKey("isPath")->boolValue();
                }
            }
            node->setActive(isPath);
			CCArray *nodes = (CCArray *)_grid->objectAtIndex(x);
            nodes->addObject(node);
		}
	}
    
    //Add neighbors
	for(int x=0; x<mw; x++){
		for(int y=0; y<mh; y++){
			//Add a node
            CCArray *nodes = (CCArray *)_grid->objectAtIndex(x);
			AStarNode *node = (AStarNode *)nodes->objectAtIndex(y);
            
			//Add self as neighbor to neighboring nodes
			this->addNeighborToGridNode(node, x-1, y-1); //Top-Left
			this->addNeighborToGridNode(node, x-1, y); //Left
			this->addNeighborToGridNode(node, x-1, y+1); //Bottom-Left
			this->addNeighborToGridNode(node, x, y-1); //Top
			
			this->addNeighborToGridNode(node, x, y+1); //Bottom
			this->addNeighborToGridNode(node, x+1, y-1); //Top-Right
			this->addNeighborToGridNode(node, x+1, y); //Right
			this->addNeighborToGridNode(node, x+1, y+1); //Bottom-Right
		}
	}
    
    _person->animateActor();
    
    //Schedule our every tick method call
    this->schedule(schedule_selector(TileDemo::step));
}

void TileDemo::step(float dt)
{
    _person->processWaypoints();
}

void TileDemo::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    _touchMoved = false;
}

void TileDemo::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    _touchMoved = true;
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint diff = touch->getDelta();
    //CCLog("diff: (%f, %f)", diff.x, diff.y);
    CCPoint currentPos = _map->getPosition();
    CCSize size = _map->getContentSize();
    //CCLog("size width: %f, height: %f", size.width, size.height);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //CCLog("winSize width: %f, height: %f", winSize.width, winSize.height);
    float minX = winSize.width - size.width;
    float minY = winSize.height - size.height;
    CCPoint newPos = ccpAdd(currentPos, diff);
    //CCLog("newPos x: %f, y: %f", newPos.x, newPos.y);
    if (newPos.x > 0) {
        newPos.x = 0;
    } else if (newPos.x < minX) {
        newPos.x = minX;
    }
    if (newPos.y > 0) {
        newPos.y = 0;
    } else if (newPos.y < minY) {
        newPos.y = minY;
    }
    _map->setPosition(newPos);
}

void TileDemo::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if (_touchMoved) {
        return;
    }
    CCTouch *touch = (CCTouch *)pTouches->anyObject();
    CCPoint touchLocation = touch->getLocation();
    // Tilemap position must be subtracted, in case the tilemap position is not at 0,0 due to scrolling
	CCPoint endPos = ccpSub(touchLocation, _map->getPosition());
    CCPoint endTilePos = this->tilePosFromLocation(endPos, _map);
    
    // Check if the touch was on water (e.g., tiles with isWater property)
    bool isTouchOnPath = false;
    CCTMXLayer* eventLayer = _map->layerNamed("GameEventLayer");
    int tileGID = eventLayer->tileGIDAt(endTilePos);
    if (tileGID != 0) {
        CCDictionary *properties = _map->propertiesForGID(tileGID);
        if (properties) {
            isTouchOnPath = properties->valueForKey("isPath")->boolValue();
        }
    }
    // Decide what to do depending on where the touch was
    CCLog("isTouchOnPath: %i", isTouchOnPath);
    if (isTouchOnPath)
    {
        float tw = _map->getTileSize().width;
        float th = _map->getTileSize().height;
        CCPoint startPos = _person->getPosition();
        startPos.x = startPos.x + tw/2;
        startPos.y = startPos.y + th/2;        
        CCPoint startTilePos = this->tilePosFromLocation(startPos, _map);
        CCLog("startTilePos: (%i, %i)", (int)startTilePos.x, (int)startTilePos.y);
        CCLog("endTilePos: (%i, %i)", (int)endTilePos.x, (int)endTilePos.y);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("alien-sfx.caf");
        
        //Starting node
        CCArray *startNodes = (CCArray *)_grid->objectAtIndex((int)startTilePos.x);
        AStarNode *startNode = (AStarNode *)startNodes->objectAtIndex((int)startTilePos.y);
        //End node
        CCArray *endNodes = (CCArray *)_grid->objectAtIndex((int)endTilePos.x);
        AStarNode *endNode = (AStarNode *)endNodes->objectAtIndex((int)endTilePos.y);
        CCLog("startPos: (%i, %i)", (int)startNode->getPosition().x, (int)startNode->getPosition().y);
        CCLog("endPos: (%i, %i)", (int)endNode->getPosition().x, (int)endNode->getPosition().y);
        
        //Run the pathfinding algorithm
        CCArray *foundPath = AStarPathNode::findPath(startNode, endNode);
        if (foundPath) {
            CCLog("Found Path:");
            _person->setWaypoints(this->waypointsWith(foundPath));
            //CCObject *object;
            //foundPath->reverseObjects();
            //this->personWalking(foundPath);
            /*CCARRAY_FOREACH(foundPath, object) {
                CCPoint *positon = (CCPoint *)object;
                CCLOG("(%i, %i)", (int)positon->x, (int)positon->y);
            }*/
        } else {
            CCLOG("No Path Found");
        }
    }
}

CCPoint TileDemo::tilePosFromLocation(CCPoint location, CCTMXTiledMap *tileMap)
{
    CCPoint pos = location;
	// scaling tileSize to Retina display size
    CCSize tileSize = tileMap->getTileSize();
	float pointWidth = tileSize.width / CC_CONTENT_SCALE_FACTOR();
	float pointHeight = tileSize.height / CC_CONTENT_SCALE_FACTOR();
	
	// Cast to int makes sure that result is in whole numbers, tile coordinates will be used as array indices
    CCSize mapSize = tileMap->getMapSize();
	pos.x = (int)(pos.x / pointWidth);
	pos.y = (int)((mapSize.height * pointHeight - pos.y) / pointHeight);
	
	CCLOG("touch at (%.0f, %.0f) is at tileCoord (%i, %i)", location.x, location.y, (int)pos.x, (int)pos.y);
	if (pos.x < 0 || pos.y < 0 || pos.x >= mapSize.width || pos.y >= mapSize.height)
	{
		CCLOG("coordinates (%i, %i) out of bounds! Adjusting...", (int)pos.x, (int)pos.y);
	}
    
	pos.x = fmaxf(0, fminf(mapSize.width - 1, pos.x));
	pos.y = fmaxf(0, fminf(mapSize.height - 1, pos.y));
    
	return pos;
}

void TileDemo::addNeighborToGridNode(AStarNode *node, int x, int y)
{
    float mw = _map->getMapSize().width;
	float mh = _map->getMapSize().height;
	if(x >= 0 && y >= 0 && x < mw && y < mh){
        CCArray *nodes = (CCArray *)_grid->objectAtIndex(x);
		AStarNode *neighbor = (AStarNode *)nodes->objectAtIndex(y);
        node->getNeighbors()->addObject(neighbor);
	}
}

CCArray* TileDemo::waypointsWith(CCArray *foundPath)
{
    CCSize cs = _map->getContentSize();
	float tw = _map->getTileSize().width;
	float th = _map->getTileSize().height;
    
    CCArray *waypoints = CCArray::createWithCapacity(foundPath->count());
    for(int i=foundPath->count()-1; i>=0; i--){
        CCPoint *pathPoint = (CCPoint *)foundPath->objectAtIndex(i);
        float x = (pathPoint->x-tw/2)/CC_CONTENT_SCALE_FACTOR();
        float y = cs.height - (pathPoint->y+th/2)/CC_CONTENT_SCALE_FACTOR();
        CCLog("(%i, %i)", (int)x, (int)y);
        GameWaypoint *waypoint = GameWaypoint::createWithPosition(ccp(x, y));
        waypoints->addObject(waypoint);
    }
    return waypoints;
}
