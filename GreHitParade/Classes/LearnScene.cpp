//
//  LearnScene.cpp
//  GreHitParade
//
//  Created by Lâm Thái on 2/2/15.
//
//

#include "LearnScene.h"
#include "Constants.h"
#include "HelloWorldScene.h"
#include <sstream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ObjCCalls.h"
#endif

USING_NS_CC;

Scene* LearnScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LearnScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LearnScene::init()
{
    if ( !LayerColor::initWithColor(Constants::BACKGROUND_COLOR) )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Home btn
    auto btn = ui::Button::create("HomeBtn", "HomeBtn", "", ui::Widget::TextureResType::PLIST);
    btn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    btn->setPosition(Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height));
    btn->addTouchEventListener(CC_CALLBACK_2(LearnScene::touchEvent, this));
    this->addChild(btn, 1, 1);
    
    // Learn progress txt
    float retainedPercent = round(Constants::getRetainedWordCount() * 100 / Constants::getTotalWordCount());
    std::stringstream stm;
    stm << "Learning progress: " << retainedPercent << "%";
    auto label = Label::createWithTTF(stm.str(), Constants::FONT_MEDIUM, 12);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(origin.x + 14, origin.y + visibleSize.height - 28);
    this->addChild(label, 2, 2);
    
    // Learn progress bar
    auto sprite9 = ui::Scale9Sprite::createWithSpriteFrameName("PbBg", Rect(2, 2, 2, 2));
    sprite9->setContentSize(Size(visibleSize.width - 28, 18));
    sprite9->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 60));
    this->addChild(sprite9, 3, 3);
    
    auto pb = ui::Scale9Sprite::createWithSpriteFrameName("PbProgress", Rect(2, 2, 2, 2));
    Size pbSize = sprite9->getContentSize();
    pbSize.width *= retainedPercent / 100;
    if(pbSize.width < 6) pbSize.width = 6;
    pb->setContentSize(pbSize);
    pb->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    sprite9->addChild(pb, 1, 1);
    
    // Text area
    sprite9 = ui::Scale9Sprite::createWithSpriteFrameName("TextArea", Rect(12, 12, 12, 12));
    sprite9->setContentSize(Size(visibleSize.width-28, visibleSize.height - 264));
    sprite9->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 86 - sprite9->getContentSize().height / 2));
    this->addChild(sprite9, 4, 4);
    
    // choices
    btn = ui::Button::create("ChoiceBg", "ChoiceSelectedBg", "", ui::Widget::TextureResType::PLIST);
    btn->setScale9Enabled(true);
    btn->setCapInsets(Rect(5, 5, 5, 5));
    btn->setSize(Size(visibleSize.width-28, 30));
    btn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 148));
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontName(Constants::FONT_REGULAR);
    btn->setTitleFontSize(12);
    btn->setTitleText("I know this word.");
    btn->getTitleRenderer()->setWidth(visibleSize.width - 56);
    btn->getTitleRenderer()->setAlignment(TextHAlignment::LEFT);
    btn->addTouchEventListener(CC_CALLBACK_2(LearnScene::touchEvent, this));
    this->addChild(btn, 5, 5);
    btn = ui::Button::create("ChoiceBg", "ChoiceSelectedBg", "", ui::Widget::TextureResType::PLIST);
    btn->setScale9Enabled(true);
    btn->setCapInsets(Rect(5, 5, 5, 5));
    btn->setSize(Size(visibleSize.width-28, 30));
    btn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 103));
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontName(Constants::FONT_REGULAR);
    btn->setTitleFontSize(12);
    btn->setTitleText("Not yet!");
    btn->getTitleRenderer()->setWidth(visibleSize.width - 56);
    btn->getTitleRenderer()->setAlignment(TextHAlignment::LEFT);
    btn->addTouchEventListener(CC_CALLBACK_2(LearnScene::touchEvent, this));
    this->addChild(btn, 6, 6);
    
    // check btn
    btn = ui::Button::create("BtnNormal", "BtnPressed", "BtnDisabled", ui::Widget::TextureResType::PLIST);
    btn->setScale9Enabled(true);
    btn->setCapInsets(Rect(12, 12, 12, 12));
    btn->setSize(Size(visibleSize.width-28, 58));
    btn->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 45));
    btn->setTitleColor(Color3B::WHITE);
    btn->setTitleFontName(Constants::FONT_BOLD);
    btn->setTitleFontSize(30);
    btn->setTitleText("Check");
    btn->addTouchEventListener(CC_CALLBACK_2(LearnScene::touchEvent, this));
    this->addChild(btn, 7, 7);
    btn->setEnabled(false);
    btn->setBright(false);
    
    // term
    std::string nextWord = Constants::getNextWordToLearn();
    label = Label::createWithTTF(nextWord, Constants::FONT_EXTRA_BOLD, 30);
    label->setColor(Color3B::BLACK);
    label->setPosition(sprite9->getContentSize().width/2, sprite9->getContentSize().height/2);
    sprite9->addChild(label, 1, 1);
    
    // definition
    const ValueMap& wInfo = Configuration::getInstance()->getValue("words").asValueMap().at(nextWord).asValueMap();
    auto it = wInfo.find("ldef");
    std::string ldef = it != wInfo.end() ? it->second.asString() : "Missing definition";
    label = Label::createWithTTF(ldef, Constants::FONT_REGULAR, 14);
    label->setColor(Color3B::BLACK);
    label->setDimensions(sprite9->getContentSize().width - 28, sprite9->getContentSize().height - 28);
    label->setPosition(sprite9->getContentSize().width/2, sprite9->getContentSize().height/2);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    sprite9->addChild(label, 2, 2);
    label->setOpacity(0);
    label->setVisible(false);
    
    // play sound
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ObjCCalls::playSound(nextWord.c_str());
#endif
    
    // init vars
    this->startTime = time(0);
    this->showNext = false;
    this->answerChoice = -1;
    
    return true;
}

void LearnScene::touchEvent(Ref *pSender, ui::Widget::TouchEventType type)
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
            if(tag == 1) { // home
                auto transition = TransitionSlideInL::create(0.25f, HelloWorld::createScene());
                Director::getInstance()->replaceScene(transition);
            } else if(tag == 5 || tag == 6) { // remembered || not yet
                int choice = tag == 5 ? 1 : 0;
                if(choice == answerChoice) return;
                answerChoice = choice;
                
                // update GUI
                ((ui::Button*)this->getChildByTag(5))->loadTextureNormal(tag != 5 ? "ChoiceBg" : "ChoiceSelectedBg", ui::Widget::TextureResType::PLIST);
                ((ui::Button*)this->getChildByTag(6))->loadTextureNormal(tag != 6 ? "ChoiceBg" : "ChoiceSelectedBg", ui::Widget::TextureResType::PLIST);
                ((ui::Button*)this->getChildByTag(7))->setEnabled(true);
                ((ui::Button*)this->getChildByTag(7))->setBright(true);
            } else if(tag == 7) { // check
                if(showNext) {
                    auto transition = TransitionSlideInR::create(0.25f, LearnScene::createScene());
                    Director::getInstance()->replaceScene(transition);
                } else {
                    auto termLabel = ((Label*)this->getChildByTag(4)->getChildByTag(1));
                    auto defLabel = ((Label*)this->getChildByTag(4)->getChildByTag(2));
                    const std::string& word = termLabel->getString();
                    time_t currTime = time(0);
                    int score = currTime - startTime <= 6 ? 5 : currTime - startTime <= 16 ? 4 : 3;
                    score = answerChoice == 1 ? score : 5 - score;
                    Constants::handleWord(word, score);
                    
                    // update GUI
                    termLabel->runAction(Sequence::create(FadeOut::create(0.25f), Hide::create(), NULL));
                    defLabel->runAction(Sequence::create(Show::create(), FadeIn::create(0.25f), NULL));
                    ((ui::Button*)this->getChildByTag(5))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(6))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(7))->setTitleText("Continue");
                    
                    // update vars
                    showNext = true;
                    Constants::showAd();
                }
            }
            break;
        }
            
        case ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}