#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

static const float rotate225 = tanf(22.5);
static const float rotate675 = 1/rotate225;

class myFrameCache : public CCSpriteFrameCache
{
public:
    friend class HelloWorld;
};

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);
/*
	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
	CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Thonburi", 34);

	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	*/
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    myFrameCache *cache = (myFrameCache*)myFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("jiuwei.plist");
    
    m_hPlayer = new CCSprite();
    m_hPlayer->init();
    m_hPlayer->autorelease();
    
    m_hPlayer->setPosition(ccp(size.width/2,size.height/2));
    
//    player->setDisplayFrame(cache->spriteFrameByName("_3_0001.png"));
    
    CCMutableArray<CCSpriteFrame *> *frames[5];
    
    for(int i(0) ; i<5 ; i++)
    {
        frames[i] = new CCMutableArray<CCSpriteFrame *>(13);
        frames[i]->autorelease();
    }
    cache->m_pSpriteFrames->begin();
    
    for(int j=0 ; j<5 ; j++)
    {
        for( int i=0 ; i<13 ; i++)
        {
            CCSpriteFrame *frame = cache->m_pSpriteFrames->next();
            if(frame == NULL)
                break;
            frames[j]->addObject(frame);
        }
    }
    
    CCAnimation *animation[5];
    for(int i(0) ; i<5 ; i++)
    {
        animation[i] = CCAnimation::animationWithFrames(frames[i],0.1);
    }
    
//    CCAnimation *animation = CCAnimation::animationWithFrames(frames,0.1);
    CCAnimate *animate[5];
    for(int i(0) ; i<5 ; i++)
    {
        animate[i] = CCAnimate::actionWithAnimation(animation[i]);
    }
    
    
    for(int i(0) ; i<5 ; i++)
    {
        m_rhRepeatForever[i] = CCRepeatForever::actionWithAction(animate[i]);
        m_rhRepeatForever[i]->retain();
    }
    
    
    //动画组合的演示
    /*
    CCRepeat *repeat = CCRepeat::actionWithAction(animate, 1);
    
    CCMoveTo *move = CCMoveTo::actionWithDuration(5, ccp(0,0));
    
    CCSpawn *spawn = (CCSpawn*)CCSpawn::actions(repeat,move,NULL);
    
    CCSequence *sequence = (CCSequence*)CCSequence::actions(repeat,move,NULL);
    */
//    animate->setDuration(10.0);
    
    m_hPlayer->runAction(m_rhRepeatForever[0]);
    m_nWay = 0;
    addChild(m_hPlayer);
    
    this->setIsTouchEnabled(true);
    
	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    for(int i(0);i<5;i++)
    {
        m_rhRepeatForever[i]->release();
    }
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}



bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pt = pTouch->locationInView();
    pt = CCDirector::sharedDirector()->convertToGL(pt);
    changedAnimate(pt);
    return true;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pt = pTouch->locationInView();
    pt = CCDirector::sharedDirector()->convertToGL(pt);
    changedAnimate(pt);
}
void HelloWorld::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}
void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void HelloWorld::registerWithTouchDispatcher(void)
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

void HelloWorld::changedAnimate(cocos2d::CCPoint pt)
{
    CCSize sz = CCDirector::sharedDirector()->getWinSize();
    pt.x-=sz.width/2;
    pt.y-=sz.height/2;
    float temp = pt.x/pt.y;
    int tempway=0;
    if(temp>-rotate225 && temp<rotate225)
    {
        if(pt.y>0.00001)
            tempway = 0;
        else 
            tempway = 4;
    }
    else if(temp>rotate225 && temp<rotate675)
    {
        if(pt.y>0.00001)
            tempway = 1;
        else 
            tempway = 5;
            
    }
    else if(temp>rotate675 || temp<-rotate675)
    {
        if(pt.x>0.00001)
            tempway = 2;
        else 
            tempway = 6;
    }
    else if(temp>-rotate675 && temp<-rotate225)
    {
        if(pt.x>0.00001)
            tempway = 3;
        else 
            tempway = 7;
    }
    
    if(tempway == m_nWay) return;
    
    m_nWay = tempway;
    
    m_hPlayer->setFlipX(false);
    m_hPlayer->setFlipY(false);
    m_hPlayer->stopAllActions();
    if(m_nWay<5)
        m_hPlayer->runAction(m_rhRepeatForever[m_nWay]);
    else 
    {
        m_hPlayer->runAction(m_rhRepeatForever[8-m_nWay]);
        m_hPlayer->setFlipX(true);
    }

}




