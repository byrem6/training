//
//  PersonSprite.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-16.
//
//

#include "PersonSprite.h"
#include "GameHelper.h"

#define WAYPOINT_DIST_THRESHOLD 8.0f
#define TIMES_BLOCKED_FAIL 2

PersonSprite* PersonSprite::create()
{
    PersonSprite *person = new PersonSprite();
    if (person && person->init()) {
        person->autorelease();
        return person;
    }
    CC_SAFE_DELETE(person);
    return NULL;
}

PersonSprite::PersonSprite(void)
{
    _timesBlocked = 0;
    _direction = WalkingDirectionNone;
}

PersonSprite::~PersonSprite()
{
    CC_SAFE_RELEASE(_waypoints);
    CC_SAFE_RELEASE(m_pAnimates);
}

bool PersonSprite::init(void)
{
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("person.plist");
    
    if (CCSprite::initWithSpriteFrameName("_5_0001.png")) {
        
        _waypoints = new CCArray();
        
        CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        m_pAnimates = new CCArray(6);
        
        for (int i = 0; i < 6; i++) {
            CCAnimation *animation = CCAnimation::create();
            animation->setDelayPerUnit(1.0f/6.0f);
            if (i == 0) {
                CCSpriteFrame *frame = frameCache->spriteFrameByName("_5_0001.png");
                animation->addSpriteFrame(frame);
            } else {
                for (int j = 1; j<= 6; j++) {
                    CCString *file = CCString::createWithFormat("_%i_000%i.png", i, j);
                    CCSpriteFrame *frame = frameCache->spriteFrameByName(file->getCString());
                    animation->addSpriteFrame(frame);
                }
            }
            CCAnimate *animate = CCAnimate::create(animation);
            m_pAnimates->addObject(animate);
        }
        return true;
    }
    return false;
}

void PersonSprite::processWaypoints()
{
    bool removeFirstWaypoint = false;
	
	//Process waypoints
    CCObject *object;
    CCARRAY_FOREACH(_waypoints, object) {
        GameWaypoint *wp = (GameWaypoint *)object;
        CCPoint p2 = ccp(wp->getPosition().x, wp->getPosition().y);
		float distanceToNextPoint = GameHelper::distance(this->getPosition(), p2);
		
		//If we didn't make progress to the next point, increment timesBlocked
		if(distanceToNextPoint >= wp->getLastDistance()){
			_timesBlocked++;
			
			//Drop this waypoint if we failed to move a number of times
			if(_timesBlocked > TIMES_BLOCKED_FAIL){
				distanceToNextPoint = 0.0f;
			}
		}else{
			//If we are just starting toward this point we run our pre-callback
            wp->setLastDistance(distanceToNextPoint);
		}
        
		//If we are close enough to the waypoint we move onto the next one
		if(distanceToNextPoint <= WAYPOINT_DIST_THRESHOLD){
			removeFirstWaypoint = true;
            this->stopRunning();
		}else{
            CCPoint vector = ccp(wp->getPosition().x - this->getPosition().x, wp->getPosition().y - this->getPosition().y);
			this->runWithVector(vector);
			break;
		}
	}
	if(removeFirstWaypoint){
		_waypoints->removeObjectAtIndex(0);
		_timesBlocked = 0;
	}
}

void PersonSprite::animateActor(void)
{
    //CCLOG("animateActor");
    //WalkingDirection direction = WalkingDirectionRightUp;
    unsigned int index = 0;
    if (_direction < 6) {
        this->setFlipX(false);
        index = _direction;
    } else {
        this->setFlipX(true);
        index = WalkingDirectionAll - _direction + 1;
    }
    CCAnimate *animate = (CCAnimate *)m_pAnimates->objectAtIndex(index);
    
    //animateActor calls itself indefinitely
    CCCallFunc *animateCallFunc = CCCallFunc::create(this, callfunc_selector(PersonSprite::animateActor));
    this->runAction(CCSequence::create(animate, animateCallFunc, NULL));
}

void PersonSprite::runWithVector(CCPoint &v)
{
    //CCLog("runWithVector: (%i, %i)", (int)v.x, (int)v.y);
    //Change animation depending on angle
	float radians = GameHelper::vectorToRadians(v);
	float degrees = GameHelper::radiansToDegrees(radians);
    
	degrees += 90.0f;
    
	if(degrees >= 337.5f || degrees < 22.5f){
		_direction = WalkingDirectionLeft;
	}else if(degrees >= 22.5f && degrees < 67.5f){
		_direction = WalkingDirectionLeftUp;
	}else if(degrees >= 67.5f && degrees < 112.5f){
		_direction = WalkingDirectionUp;
	}else if(degrees >= 112.5f && degrees < 157.5f){
		_direction = WalkingDirectionRightUp;
	}else if(degrees >= 157.5f && degrees < 202.5f){
		_direction = WalkingDirectionRight;
	}else if(degrees >= 202.5f && degrees < 247.5f){
		_direction = WalkingDirectionRightDown;
	}else if(degrees >= 247.5f && degrees < 292.5f){
		_direction = WalkingDirectionDown;
	}else{
		_direction = WalkingDirectionLeftDown;
	}
    
    CCPoint position = this->getPosition();
    CCPoint offset = ccp(v.x/5, v.y/5);
    this->setPosition(ccpAdd(position, offset));
}

void PersonSprite::stopRunning()
{
    _direction = WalkingDirectionNone;
}
