//
//  PersonSprite.cpp
//  TilemapDemo
//
//  Created by 方俊锋 on 13-1-16.
//
//

#include "PersonSprite.h"

using namespace cocos2d;

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
    //
}

PersonSprite::~PersonSprite()
{
    CC_SAFE_RELEASE(m_pAnimates);
}

bool PersonSprite::init(void)
{
    if (CCSprite::initWithSpriteFrameName("_5_0001.png")) {
        CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        m_pAnimates = new CCArray(5);
        for (int i = 1; i <= 5; i++) {
            CCArray *frames = CCArray::createWithCapacity(6);
            for (int j = 1; j<= 6; j++) {
                CCString *file = CCString::createWithFormat("_%i_000%i.png", i, j);
                //CCLog(file->getCString());
                CCSpriteFrame *frame = frameCache->spriteFrameByName(file->getCString());
                frames->addObject(frame);
            }
            CCAnimation *animation = CCAnimation::createWithSpriteFrames(frames);
            animation->setDelayPerUnit(1.0f/6.0f);
            CCAnimate *animate = CCAnimate::create(animation);
            m_pAnimates->addObject(animate);
        }
        return true;
    }
    return false;
}

void PersonSprite::walking(WalkingDirection direction)
{
    this->stopAllActions();
    unsigned int index = 0;
    if (direction <= 5) {
        this->setFlipX(false);
        index = direction - 1;
    } else {
        this->setFlipX(true);
        index = 8 - (direction - 1);
    }
    CCAnimate *animate = (CCAnimate *)m_pAnimates->objectAtIndex(index);
    CCRepeatForever *repeat = CCRepeatForever::create(animate);
    this->runAction(repeat);
    //this->setFlipX(true);
}
