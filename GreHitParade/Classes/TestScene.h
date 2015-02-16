//
//  TestScene.h
//  GreHitParade
//
//  Created by Lâm Thái on 2/6/15.
//
//

#ifndef __GreHitParade__TestScene__
#define __GreHitParade__TestScene__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum TestMode {
    FILL_IN,
    MULTIPLE_CHOICE,
    TRUE_FALSE
};

class TestScene : public cocos2d::LayerColor, public cocos2d::ui::EditBoxDelegate
{
private:
    TestMode mode;
    time_t startTime;
    bool showNext;
    int correctAnswer;
    int answerChoice;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    bool initFillIn(const std::string& word);
    bool initMultiChoice(const std::string& word, bool showTerm);
    bool initTrueFalse(const std::string& word);
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
    
    // a selector callback
    void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    
    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);
};

#endif /* defined(__GreHitParade__TestScene__) */
