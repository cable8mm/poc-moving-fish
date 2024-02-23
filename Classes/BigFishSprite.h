//
//  BigFishSprite.h
//  NewNabuza
//
//  Created by Lee Samgu on 10/28/13.
//
//

#ifndef __NewNabuza__BigFishSprite__
#define __NewNabuza__BigFishSprite__

#include "cocos2d.h"
USING_NS_CC;

enum eBigFishKey
{
    kBIG_FISH_YELLOW    = 0
};

class BigFishSprite : public cocos2d::CCSprite
{
private:
    enum eBigFishStats {
        kBIG_FISH_READY,
        kBIG_FISH_EATING,
        kBIG_FISH_REMOVED
    };
    eBigFishKey _key;
    eBigFishStats  _state;
    int _ownPearlCount;  // 한번에 얻을 수 있는 진주 갯수
    void eatten();
    void onEnter();
    void close();
public:
    static BigFishSprite* createWithKey(eBigFishKey k);
    float radiusForGoal;
    void animating();
    void eating();
    bool isLiving();
    void flipX(bool b);
    void moveBy(CCPoint point);
};

#endif /* defined(__NewNabuza__BigFishSprite__) */
