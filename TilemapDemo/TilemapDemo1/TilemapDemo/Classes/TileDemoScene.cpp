//
//  TileDemoScene.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-8.
//
//

#include "TileDemoScene.h"

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
    CCSize s = _map->getContentSize();
    CCLOG("ContentSize: %f, %f", s.width,s.height);
    
    this->setTouchEnabled(true);
    return true;
}

void TileDemo::onEnter()
{
    CCLayer::onEnter();
    CCLog("onEnter");
}

void TileDemo::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint diff = touch->getDelta();
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
