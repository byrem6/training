//
//  TileDemoScene.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-8.
//
//

#include "TileDemoScene.h"
#include "PersonSprite.h"
#include "CCString.h"
#include "SimpleAudioEngine.h"

CCScene* TileDemo::scene()
{
    CCScene *scene = CCScene::create();
    TileDemo *layer = TileDemo::create();
    scene->addChild(layer);
    return scene;
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
    
    PersonSprite *person = PersonSprite::create();
    person->setAnchorPoint(ccp(0, 0));
    person->setPosition(pos);
    _map->addChild(person);
    //person->walking(WalkingDirectionLeftFront);
    
    this->setTouchEnabled(true);
    return true;
}

void TileDemo::onEnter()
{
    CCLayer::onEnter();
    CCLog("onEnter");
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
    CCPoint tilePos = this->tilePosFromLocation(touchLocation, _map);
    CCLog("tilePosFromLocation: (%i, %i)", (int)tilePos.x, (int)tilePos.y);
    
    // Check if the touch was on water (e.g., tiles with isWater property)
    bool isTouchOnPath = false;
    CCTMXLayer* eventLayer = _map->layerNamed("GameEventLayer");
    int tileGID = eventLayer->tileGIDAt(tilePos);
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
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("alien-sfx.caf");
    }
}

CCPoint TileDemo::tilePosFromLocation(CCPoint location, CCTMXTiledMap *tileMap)
{
    // Tilemap position must be subtracted, in case the tilemap position is not at 0,0 due to scrolling
	CCPoint pos = ccpSub(location, tileMap->getPosition());
	
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
