#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    bool isLeft;
    bool isYegger;
    float heartbeat;
    float prevHeartbeat;
    unsigned int deltaSum;
    cocos2d::CCArray *displayPearls;
    cocos2d::CCPoint prevBigFishPoint;
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *pEvent);
    void registerWithTouchDispatcher();
    void goPoint(cocos2d::CCPoint pt, bool isYegger = false, float termFishCrossSecond = 1.);
    void goPoint2(cocos2d::CCPoint pt);
    void callFish();
    void endYegger();
    void update(float fDelta);
    void resetHeartbeat();
};

#endif // __HELLOWORLD_SCENE_H__
