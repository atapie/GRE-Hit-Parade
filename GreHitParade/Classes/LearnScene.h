//
//  LearnScene.h
//  GreHitParade
//
//  Created by Lâm Thái on 2/2/15.
//
//

#ifndef __GreHitParade__LearnScene__
#define __GreHitParade__LearnScene__

#include "cocos2d.h"
#include "CocosGUI.h"

class LearnScene : public cocos2d::LayerColor
{
private:
    time_t startTime;
    bool showNext;
    int answerChoice;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(LearnScene);
};

#endif /* defined(__GreHitParade__LearnScene__) */
