#include "HelloWorldScene.h"
#include "Constants.h"
#include "LearnScene.h"
#include "TestScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !LayerColor::initWithColor(Constants::BACKGROUND_COLOR) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto learnBtn = ui::Button::create("BtnNormal", "BtnPressed", "", ui::Widget::TextureResType::PLIST);
    learnBtn->setScale9Enabled(true);
    learnBtn->setCapInsets(Rect(12, 12, 12, 12));
    learnBtn->setSize(Size(177, 58));
    learnBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 + 39));
    learnBtn->getTitleRenderer()->setBMFontFilePath(Constants::FONT_BOLD30);
    learnBtn->setTitleText("Learn");
    learnBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this));
    this->addChild(learnBtn, 1, 1);
    
    auto testBtn = ui::Button::create("BtnBlueNormal", "BtnBluePressed", "", ui::Widget::TextureResType::PLIST);
    testBtn->setScale9Enabled(true);
    testBtn->setCapInsets(Rect(12, 12, 12, 12));
    testBtn->setSize(Size(177, 58));
    testBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 - 39));
    testBtn->getTitleRenderer()->setBMFontFilePath(Constants::FONT_BOLD30);
    testBtn->setTitleText("Test");
    testBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this));
    this->addChild(testBtn, 2, 2);
    
    auto rateBtn = ui::Button::create("RateBtn", "", "", ui::Widget::TextureResType::PLIST);
    rateBtn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 30));
    rateBtn->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEvent, this));
    this->addChild(rateBtn, 3, 3);

    return true;
}

void HelloWorld::touchEvent(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch (type)
    {
        case ui::Widget::TouchEventType::BEGAN:
            break;
            
        case ui::Widget::TouchEventType::MOVED:
            break;
            
        case ui::Widget::TouchEventType::ENDED:
        {
            int tag = ((Node*)pSender)->getTag();
            if(tag == 1) {
                auto transition = TransitionSlideInR::create(0.25f, LearnScene::createScene());
                Director::getInstance()->replaceScene(transition);
            } else if(tag == 2) {
                auto transition = TransitionSlideInR::create(0.25f, TestScene::createScene());
                Director::getInstance()->replaceScene(transition);
            } else {
                Constants::openInAppStore();
            }
            break;
        }
            
        case ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}
