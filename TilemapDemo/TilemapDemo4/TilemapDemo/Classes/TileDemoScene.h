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
#include "AStarNode.h"
#include "PersonSprite.h"

USING_NS_CC;

class TileDemo : public CCLayer {
public:
    virtual ~TileDemo();
    virtual bool init();
    
    virtual void runGame();
    
    void step(float dt);
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    static CCScene* scene();
    
    CREATE_FUNC(TileDemo);
    
protected:
    virtual CCArray* waypointsWith(CCArray *foundPath);
    //virtual void personWalking(CCArray *path);
private:
    CCTMXTiledMap *_map;
    PersonSprite *_person;
    CCArray *_grid;
    bool _touchMoved;
    CCPoint tilePosFromLocation(CCPoint location, CCTMXTiledMap *tileMap);
    void addNeighborToGridNode(AStarNode* node, int x, int y);
};

#endif
