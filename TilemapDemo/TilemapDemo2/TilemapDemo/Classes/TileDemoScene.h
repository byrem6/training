//
//  TileDemoScene.h
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-8.
//
//

#ifndef TilemapDemo_TileDemoScene_h
#define TilemapDemo_TileDemoScene_h

#include "cocos2d.h"

USING_NS_CC;

class TileDemo : public CCLayer {
private:
    CCTMXTiledMap *_map;
public:
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    
    static CCScene* scene();
    
    CREATE_FUNC(TileDemo);
};

#endif
