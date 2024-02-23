//
//  BigFishSprite.cpp
//  NewNabuza
//
//  Created by Lee Samgu on 10/28/13.
//
//

#include "BigFishSprite.h"

#define TEXTURE_WIDTH 126.
#define TEXTURE_HEIGHT 102.
#define MOUSE_POINT_X   72.
#define MOUSE_POINT_Y   91.

BigFishSprite* BigFishSprite::createWithKey(eBigFishKey k) {
    BigFishSprite *pob = new BigFishSprite();
    pob->_key  = k;
    pob->_state = kBIG_FISH_READY;
    
    pob->_ownPearlCount = 2;
    pob->radiusForGoal = 60.;
    
    if (pob->initWithFile("fish2.png", CCRect(k*TEXTURE_WIDTH, 0., TEXTURE_WIDTH, TEXTURE_HEIGHT)))
    {
        pob->autorelease();
        pob->setAnchorPoint(ccp(MOUSE_POINT_X/TEXTURE_WIDTH, MOUSE_POINT_Y/TEXTURE_HEIGHT));
    }
    else
    {
        delete pob;
        pob = NULL;
    }
    
    return pob;
}

void BigFishSprite::onEnter() {
    CCSprite::onEnter();
}

void BigFishSprite::moveBy(CCPoint point) {
    CCPoint prevPoint   = this->getPosition();
    CCPoint nextPoint   = ccp(this->getPositionX() + point.x, this->getPositionY() + point.y);

    CCPoint diffPoint = ccpSub(nextPoint, prevPoint);
//    CCPoint diffPoint = ccp(nextPoint.x - prevPoint.x, prevPoint.y - nextPoint.y);
    CCLog("diffPoint.x = %f, diffPoint.y = %f", diffPoint.x, diffPoint.y);
    if (diffPoint.x >= 0.) {
        this->flipX(true);
    } else {
        this->flipX(false);
    }
    
    float ninetyDegree  = CC_DEGREES_TO_RADIANS(90);
    
    float angleRadians;
    if (diffPoint.x < 0. && diffPoint.y > 0.) {   // 4사분면 (OK)
        angleRadians    = (-1)*atanf((-1)*diffPoint.x/diffPoint.y) + ninetyDegree;
    } else if (diffPoint.x > 0. && diffPoint.y > 0.) { // 1사분면 (OK)
        angleRadians    = atanf((float)diffPoint.x/(float)diffPoint.y) - ninetyDegree;
    } else if (diffPoint.x > 0. && diffPoint.y < 0.) { // 2사분면 (OK)
        angleRadians    = atanf((-1)*diffPoint.y/diffPoint.x);
    } else if (diffPoint.x < 0. && diffPoint.y < 0.) { // 3사분면
        angleRadians    = ninetyDegree*4 - atanf((float)diffPoint.y/(float)diffPoint.x);
    } else if (diffPoint.x == 0. && diffPoint.y > 0.) {
        angleRadians    = ninetyDegree;
    } else if(diffPoint.x == 0. && diffPoint.y < 0.) {
        angleRadians    = ninetyDegree * 3;
    } else if(diffPoint.x > 0. && diffPoint.y == 0.) {
        angleRadians    = 0;
    } else if(diffPoint.x < 0. && diffPoint.y == 0.) {
        angleRadians    = 0.;
    } else { // 원점
        angleRadians    = 0.;
    }
    
    this->setRotation(CC_RADIANS_TO_DEGREES(angleRadians));
    
    this->setPosition(ccp(this->getPositionX() + point.x, this->getPositionY() + point.y));
}

void BigFishSprite::animating() {
    CCTexture2D *bigFishTexture  = CCTextureCache::sharedTextureCache()->addImage("fish2.png");
    CCArray *bigFishFrames  = CCArray::create();
    bigFishFrames->retain();
    
    for (int i = 0; i < 4; i++) {
        CCSpriteFrame *bigFishFrame  = CCSpriteFrame::createWithTexture(bigFishTexture, CCRect(0., TEXTURE_HEIGHT, TEXTURE_WIDTH, TEXTURE_HEIGHT));
        bigFishFrames->addObject(bigFishFrame);
    }
    
    CCAnimation *bigFishAnimation  = CCAnimation::createWithSpriteFrames(bigFishFrames, .2);  // 1초
    
    this->_state    = kBIG_FISH_EATING;
    
    this->runAction(CCRepeatForever::create(
                                        CCAnimate::create(bigFishAnimation)
                                                        ));
    
    
    //    this->runAction(CCSequence::create(
//                                       CCMoveBy::create(3., ccp(TEXTURE_WIDTH*2+480., 0.))
//                                       ,CCDelayTime::create(2.)
//                                       ,CCCallFunc::create(this, callfunc_selector(BigFishSprite::close))
//                                       ,NULL
//                                       ));
}

void BigFishSprite::eating() {
    if (this->_state == kBIG_FISH_READY) {
        CCTexture2D *bigFishTexture  = CCTextureCache::sharedTextureCache()->addImage("fish2.png");
        CCArray *bigFishFrames  = CCArray::create();
        bigFishFrames->retain();
        
        CCRect offsetRect   = this->getTextureRect();
        
        for (int i = 0; i < 4; i++) {
            CCSpriteFrame *bigFishFrame  = CCSpriteFrame::createWithTexture(bigFishTexture, CCRect(offsetRect.origin.x, TEXTURE_HEIGHT*i, TEXTURE_WIDTH, TEXTURE_HEIGHT));
            bigFishFrames->addObject(bigFishFrame);
        }
        
        CCAnimation *bigFishAnimation  = CCAnimation::createWithSpriteFrames(bigFishFrames, .2);  // 1초
        
        this->_state    = kBIG_FISH_EATING;
        //        CCAnimation *blingBlingAnimation   = CCAnimationCache::sharedAnimationCache()->animationByName("clam_animation");
        this->runAction(CCSequence::create(
                                           CCAnimate::create(bigFishAnimation)
                                           ,CCCallFunc::create(this, callfunc_selector(BigFishSprite::eatten))
                                           ,NULL
                                           ));
    }
}

void BigFishSprite::eatten() {
    this->_state    = kBIG_FISH_READY;
}

bool BigFishSprite::isLiving() {
    return this->_state != kBIG_FISH_REMOVED;
}

void BigFishSprite::close() {
    this->_state    = kBIG_FISH_REMOVED;
    //    this->removeFromParentAndCleanup(true);
}

void BigFishSprite::flipX(bool b) {
    if (b == false) {
        this->setAnchorPoint(ccp(MOUSE_POINT_X/TEXTURE_WIDTH, MOUSE_POINT_Y/TEXTURE_HEIGHT));
    } else {
        this->setAnchorPoint(ccp((TEXTURE_WIDTH-MOUSE_POINT_X)/TEXTURE_WIDTH, MOUSE_POINT_Y/TEXTURE_HEIGHT));
    }
    this->setFlipX(b);
}