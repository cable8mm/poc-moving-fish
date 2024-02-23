#include "HelloWorldScene.h"

#include "BigFishSprite.h"

#define TAG_BIG_FISH_SPRITE 1000

#define TERM_FISH_CROSS_SECOND 2
#define TERM_FISH_YEGGER_SECOND 1

#define BIG_FISH_MARGIN_X 110.

USING_NS_CC;

CCScene *HelloWorld::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!CCLayer::init()) {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    this->setTouchEnabled(true);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem =
        CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this,
                                menu_selector(HelloWorld::menuCloseCallback));

    pCloseItem->setPosition(ccp(
        origin.x + visibleSize.width - pCloseItem->getContentSize().width / 2,
        origin.y + pCloseItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    CCMenu *pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    BigFishSprite *bigFishSprite =
        BigFishSprite::createWithKey(kBIG_FISH_YELLOW);
    bigFishSprite->setPosition(
        ccp(visibleSize.width + 102., visibleSize.height / 2));
    //    bigFishSprite->setPosition(ccp(visibleSize.width/2,
    //    visibleSize.height/2));
    bigFishSprite->setTag(TAG_BIG_FISH_SPRITE);
    bigFishSprite->animating();
    this->addChild(bigFishSprite);

    this->isLeft = false;
    this->isYegger = false;
    this->heartbeat = 0;
    this->resetHeartbeat();
    this->deltaSum = 0.;
    this->displayPearls = CCArray::create();
    this->displayPearls->retain();

    this->scheduleUpdate();
    return true;
}

void HelloWorld::resetHeartbeat() {
    this->heartbeat = 0;
    this->prevHeartbeat = -100000;
}

void HelloWorld::update(float fDelta) {
#define COLLIPSE_RANGE 30.
    // Yegger

    if (this->displayPearls->count() > 0) {
        // 가까운 볼을 탐색한 후 먹자.
        BigFishSprite *bigFishSprite =
            (BigFishSprite *)this->getChildByTag(TAG_BIG_FISH_SPRITE);

        float distance = INT32_MAX;
        CCSprite *nearestPearSprite;
        CCArray *deletePearls = CCArray::create();
        deletePearls->retain();

        for (int i = 0; i < this->displayPearls->count(); i++) {
            CCSprite *pearlSprite =
                (CCSprite *)this->displayPearls->objectAtIndex(i);
            float tmpDistance = ccpDistance(bigFishSprite->getPosition(),
                                            pearlSprite->getPosition());

            // 충돌한 진주를 없애자.
            if (fabsf(tmpDistance) < COLLIPSE_RANGE) {
                deletePearls->addObject(pearlSprite);
            } else {
                // 화면 밖으로 넘어간 진주를 지우자
                if (pearlSprite->getPositionY() > 600.) {
                    deletePearls->addObject(pearlSprite);
                } else {
                    // 거리 중 가장 가까운 진주를 찾는다.
                    if (distance > tmpDistance) {
                        nearestPearSprite = pearlSprite;
                    }
                }
            }
        }

        //        // 먹었을 때...
        //        if (this->heartbeat - this->prevHeartbeat >= 1.) {
        //            this->prevHeartbeat = this->heartbeat;
        //
        //            this->goPoint2(nearestPearSprite->getPosition());
        //
        //            return;
        //        }
        //                    this->heartbeat  += fDelta;
        //        return;
        //        if (deletePearls->count() != 0) {
        //            for (int i=deletePearls->count()-1; i >= 0; i--) {
        //                BigFishSprite *enumBigFishSprite    =
        //                (BigFishSprite*)deletePearls->objectAtIndex(i);
        //                this->displayPearls->removeObject(enumBigFishSprite);
        //                enumBigFishSprite->removeFromParentAndCleanup(true);
        //            }
        //
        //            return;
        //        }
        //
        //        // 못먹고 쫓아 다닐 때...
        // #define YEGGER_SCALE    5.
        // #define MAX_TERM        20.
        // #define MIN_TERM        5.
        // #define MIN_TERM_Y_PLUS      10.
        //
        //        // 등속으로 쫓아가다가 거리가 아까워지면 거리 차이로
        ////        float anger
        //        float targetDistanceX    = nearestPearSprite->getPositionX() -
        //        bigFishSprite->getPositionX(); float targetDistanceY    =
        //        nearestPearSprite->getPositionY() -
        //        bigFishSprite->getPositionY();
        //
        //        float targetDistance    =
        //        nearestPearSprite->getPosition().getDistance(bigFishSprite->getPosition());
        //        float goX   = 0.;
        //        float goY   = 0.;
        //        if (fabsf(targetDistance) > COLLIPSE_RANGE) {
        //            goX   = targetDistanceX / 100. * YEGGER_SCALE;
        //            goY   = targetDistanceY / 100. * YEGGER_SCALE;
        //
        //            if (fabsf(goY) < MIN_TERM) {
        //                goY *= 3.;
        //            }
        //            if (goY > MAX_TERM) {
        //                goY = MAX_TERM;
        //            }
        //            if (goY < -1*MAX_TERM) {
        //                goY = -1*MAX_TERM;
        //            }
        //            if (fabsf(goY) < MIN_TERM) {
        //                goX *= 3.;
        //            }
        //        }
        //
        //        bigFishSprite->moveBy(ccp(goX, goY));
        //
        //        return;
        if (this->heartbeat - this->prevHeartbeat >= .5) {
            this->goPoint2(nearestPearSprite->getPosition());
            this->prevHeartbeat = this->heartbeat;
            return;
        }
    }
    this->heartbeat += fDelta;
}

void HelloWorld::callFish() {
    return;
    if (this->isYegger == true) {
        return;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    BigFishSprite *bigFishSprite =
        (BigFishSprite *)this->getChildByTag(TAG_BIG_FISH_SPRITE);
    bigFishSprite->setRotation(0);

    if (this->isLeft == false) {  // 왼쪽으로 갈 경우
        this->goPoint(ccp(-1 * BIG_FISH_MARGIN_X, visibleSize.height / 2),
                      false, (float)TERM_FISH_CROSS_SECOND);
    } else {
        this->goPoint(
            ccp(visibleSize.width + BIG_FISH_MARGIN_X, visibleSize.height / 2),
            false, (float)TERM_FISH_CROSS_SECOND);
    }
}

void HelloWorld::goPoint2(CCPoint pt) {
    BigFishSprite *bigFishSprite =
        (BigFishSprite *)this->getChildByTag(TAG_BIG_FISH_SPRITE);
    bigFishSprite->stopAllActions();
    bigFishSprite->runAction(CCMoveTo::create(.5, pt));
    float o = pt.x - bigFishSprite->getPosition().x;
    float a = pt.y - bigFishSprite->getPosition().y;
    float at = (float)CC_RADIANS_TO_DEGREES(atanf(o / a));

    if (a < 0) {
        if (o < 0) {
            at = 180 + fabs(at);
        } else {
            at = 180 - fabs(at);
        }
    }

    bigFishSprite->runAction(CCRotateTo::create(.2, at));
}

void HelloWorld::goPoint(CCPoint pt, bool isYegger, float termFishCrossSecond) {
    this->isYegger = isYegger;
    BigFishSprite *bigFishSprite =
        (BigFishSprite *)this->getChildByTag(TAG_BIG_FISH_SPRITE);
    bigFishSprite->stopActionByTag(239);

    CCPoint diffPoint = ccpSub(pt, bigFishSprite->getPosition());
    float length = ccpLength(diffPoint);

    termFishCrossSecond *= length / (480. + 102.2);

    if (diffPoint.x >= 0.) {
        bigFishSprite->flipX(true);
    } else {
        bigFishSprite->flipX(false);
    }

    float ninetyDegree = CC_DEGREES_TO_RADIANS(90);

    float angleRadians;
    if (diffPoint.x < 0. && diffPoint.y > 0.) {  // 4사분면 (OK)
        angleRadians =
            (-1) * atanf((-1) * diffPoint.x / diffPoint.y) + ninetyDegree;
    } else if (diffPoint.x > 0. && diffPoint.y > 0.) {  // 1사분면 (OK)
        angleRadians =
            atanf((float)diffPoint.x / (float)diffPoint.y) - ninetyDegree;
    } else if (diffPoint.x > 0. && diffPoint.y < 0.) {  // 2사분면 (OK)
        angleRadians = atanf((-1) * diffPoint.y / diffPoint.x);
    } else if (diffPoint.x < 0. && diffPoint.y < 0.) {  // 3사분면
        angleRadians =
            ninetyDegree * 4 - atanf((float)diffPoint.y / (float)diffPoint.x);
    } else if (diffPoint.x == 0. && diffPoint.y > 0.) {
        angleRadians = ninetyDegree;
    } else if (diffPoint.x == 0. && diffPoint.y < 0.) {
        angleRadians = ninetyDegree * 3;
    } else if (diffPoint.x > 0. && diffPoint.y == 0.) {
        angleRadians = 0;
    } else if (diffPoint.x < 0. && diffPoint.y == 0.) {
        angleRadians = 0.;
    } else {  // 원점
        angleRadians = 0.;
    }

    //    bigFishSprite->setRotation(CC_RADIANS_TO_DEGREES(angleRadians));

    CCAction *fishMovingAction = CCSequence::create(
        CCMoveTo::create(termFishCrossSecond, pt),
        CCCallFunc::create(this, callfunc_selector(HelloWorld::endYegger)),
        NULL);
    fishMovingAction->setTag(239);
    bigFishSprite->runAction(fishMovingAction);
}

void HelloWorld::endYegger() {
    this->isYegger = false;
    this->isLeft = !this->isLeft;
    this->deltaSum = 0.;
    this->resetHeartbeat();
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *touch,
                              cocos2d::CCEvent *pEvent) {
    CCPoint pt =
        CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

    //    this->goPoint(pt, true, (float)TERM_FISH_YEGGER_SECOND);
    //    this->goPoint2(pt);

    float randomX = (float)(rand() % 400);
    CCSprite *pearlSprite = CCSprite::create("pearl.png");
    pearlSprite->setPosition(ccp(randomX, 30.));
    this->addChild(pearlSprite);
    this->displayPearls->addObject(pearlSprite);
    pearlSprite->runAction(CCMoveBy::create(10., ccp(0., 1000.)));

    return true;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *touch,
                              cocos2d::CCEvent *event) {
    //    CCPoint pt    =
    //    CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
    //
    //    BigFishSprite *bigFishSprite    =
    //    (BigFishSprite*)this->getChildByTag(TAG_BIG_FISH_SPRITE);
    //    bigFishSprite->stopAllActions();
    //    float deltaAngle =
    //    pt.getAngle(bigFishSprite->getPosition())*180/3.141592+270.;
    //    CCLog("deltaAngle = %f, bigFishRotation = %f", deltaAngle,
    //    bigFishSprite->getRotation());
    //
    //    bigFishSprite->runAction(CCSpawn::create(
    //                                             CCRotateTo::create(.3,
    //                                             deltaAngle),
    //                                             CCMoveTo::create(.5, pt),
    //                                             NULL
    //                                             ));
}

void HelloWorld::ccTouchEnded(cocos2d::CCTouch *touch,
                              cocos2d::CCEvent *event) {
}

void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *touch,
                                  cocos2d::CCEvent *pEvent) {
}

void HelloWorld::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this, 0, true);
}

void HelloWorld::menuCloseCallback(CCObject *pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not "
                 "implement a close button.",
                 "Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
