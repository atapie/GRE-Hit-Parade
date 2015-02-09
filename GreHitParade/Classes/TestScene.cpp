//
//  TestScene.cpp
//  GreHitParade
//
//  Created by Lâm Thái on 2/6/15.
//
//

#include "TestScene.h"
#include "Constants.h"
#include "HelloWorldScene.h"
#include <sstream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ObjCCalls.h"
#endif

USING_NS_CC;

Scene* TestScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = TestScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
    if ( !LayerColor::initWithColor(Constants::BACKGROUND_COLOR) )
    {
        return false;
    }
    
    std::string term = Constants::getNextWordToTest();
    this->setName(term);
    char type = term.back();
    term.pop_back();
    if(type == 'a') {
        return initFillIn(term);
    } else {
        int randNum = rand() % 100;
        if (randNum < 50) return initMultiChoice(term, randNum < 25);
        else return initTrueFalse(term);
    }
}

bool TestScene::initFillIn(const std::string& word)
{
    // Retrieve data
    const ValueMap& wInfo = Configuration::getInstance()->getValue("words").asValueMap().at(word).asValueMap();
    size_t testAvail = wInfo.at("ex").asValueMap().size() + 1;
    int randNum = rand() % testAvail;
    std::string text;
    if(randNum == 0) {
        auto it = wInfo.find("ldef");
        text = it != wInfo.end() ? it->second.asString() : "Missing definition";
    } else if (randNum == 1) {
        text = wInfo.at("ex").asValueMap().at("ex1").asString();
    } else if (randNum == 2) {
        text = wInfo.at("ex").asValueMap().at("ex2").asString();
    } else if (randNum == 3) {
        text = wInfo.at("ex").asValueMap().at("ex3").asString();
    }
    std::string textLower = text;
    std::transform(textLower.begin(), textLower.end(), textLower.begin(), ::tolower);
    size_t wordIdx = 0;
    while((wordIdx = textLower.find(word, wordIdx)) != textLower.npos) {
        text.replace(wordIdx, word.length(), word.length(), '_');
        wordIdx += word.length();
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Home btn
    auto btn = ui::Button::create("HomeBtn", "HomeBtn", "", ui::Widget::TextureResType::PLIST);
    btn->setPosition(Vec2(origin.x + visibleSize.width - 28, origin.y + visibleSize.height - 26));
    btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
    this->addChild(btn, 1, 1);
    
    // Progress txt
    float retainedPercent = round(Constants::getTestedCount() * 100 / Constants::getTotalTestCount());
    std::stringstream stm;
    stm << "Testing progress: " << retainedPercent << "%";
    auto label = Label::createWithTTF(stm.str(), Constants::FONT_MEDIUM, 12);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(origin.x + 14, origin.y + visibleSize.height - 28);
    this->addChild(label, 2, 2);
    
    // Progress bar
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
    sprite9->setContentSize(Size(visibleSize.width-28, visibleSize.height - 230));
    sprite9->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - 86 - sprite9->getContentSize().height / 2));
    this->addChild(sprite9, 4, 4);
    
    // input field
    auto inputFieldBg = ui::Scale9Sprite::createWithSpriteFrameName("InputFieldBg", Rect(5, 5, 5, 5));
    auto editBox = ui::EditBox::create(Size(visibleSize.width - 28, 36), inputFieldBg);
    editBox->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + 108));
    editBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    editBox->setPlaceHolder("Type in the term");
    editBox->setPlaceholderFont(Constants::FONT_LIGHT.c_str(), 12);
    editBox->setPlaceholderFontColor(Color3B(153, 153, 153));
    editBox->setFont(Constants::FONT_REGULAR.c_str(), 12);
    editBox->setFontColor(Color3B::BLACK);
    editBox->setMaxLength((int)word.length());
    editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    editBox->setDelegate(this);
    editBox->setName(word);
    this->addChild(editBox, 5, 5);
    
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
    btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
    this->addChild(btn, 99, 99);
    btn->setEnabled(false);
    btn->setBright(false);
    
    // definition
    label = Label::createWithTTF(text, Constants::FONT_REGULAR, 14);
    label->setColor(Color3B::BLACK);
    label->setDimensions(sprite9->getContentSize().width - 28, sprite9->getContentSize().height - 28);
    label->setPosition(sprite9->getContentSize().width/2, sprite9->getContentSize().height/2);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    sprite9->addChild(label, 1, 1);
    
    // init vars
    this->startTime = time(0);
    this->showNext = false;
    this->mode = TestMode::FILL_IN;
    
    return true;
}

bool TestScene::initMultiChoice(const std::string& word, bool showTerm)
{
    // Retrieve data
    const ValueMap& wordsConfig = Configuration::getInstance()->getValue("words").asValueMap();
    const ValueMap& wInfo = wordsConfig.at(word).asValueMap();
    size_t testAvail = wInfo.at("sdef").asValueMap().size();
    int randNum = rand() % testAvail;
    std::string text;
    if(randNum == 0) {
        text = wInfo.at("sdef").asValueMap().at("sdef1").asString();
    } else if (randNum == 1) {
        text = wInfo.at("sdef").asValueMap().at("sdef2").asString();
    } else if (randNum == 2) {
        text = wInfo.at("sdef").asValueMap().at("sdef3").asString();
    }
    this->correctAnswer = rand() % 4;
    
    // Generate wrong answers
    std::vector<std::string> wrongAnswers;
    while(wrongAnswers.size() < 3) {
        auto randIt = wordsConfig.cbegin();
        std::advance(randIt, rand() % wordsConfig.size());
        if(randIt->first == word) continue;
        wrongAnswers.push_back(showTerm ? randIt->first : randIt->second.asValueMap().at("sdef").asValueMap().at("sdef1").asString());
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Home btn
    auto btn = ui::Button::create("HomeBtn", "HomeBtn", "", ui::Widget::TextureResType::PLIST);
    btn->setPosition(Vec2(origin.x + visibleSize.width - 28, origin.y + visibleSize.height - 26));
    btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
    this->addChild(btn, 1, 1);
    
    // Progress txt
    float retainedPercent = round(Constants::getTestedCount() * 100 / Constants::getTotalTestCount());
    std::stringstream stm;
    stm << "Testing progress: " << retainedPercent << "%";
    auto label = Label::createWithTTF(stm.str(), Constants::FONT_MEDIUM, 12);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(origin.x + 14, origin.y + visibleSize.height - 28);
    this->addChild(label, 2, 2);
    
    // Progress bar
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
    
    label = Label::createWithTTF("Choose the correct answer", Constants::FONT_MEDIUM_ITALIC, 18);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(origin.x + 14, origin.y + visibleSize.height - 94);
    this->addChild(label, 4, 4);
    
    float startY = label->getPosition().y - label->getContentSize().height/2 - 14;
    label = Label::createWithTTF(showTerm ? text : word, Constants::FONT_BOLD, 12);
    label->setColor(Color3B::BLACK);
    label->setWidth(visibleSize.width - 28);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(origin.x + 14, startY);
    this->addChild(label, 5, 5);
    
    startY = label->getPosition().y - label->getContentSize().height - 14;
    for(int i = 0; i < 4; i++) { // answer choices
        auto btn = ui::Button::create("ChoiceBg", "ChoiceSelectedBg", "", ui::Widget::TextureResType::PLIST);
        btn->setScale9Enabled(true);
        btn->setCapInsets(Rect(5, 5, 5, 5));
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btn->setPosition(Vec2(origin.x + visibleSize.width/2, startY));
        btn->setTitleColor(Color3B::BLACK);
        btn->setTitleFontName(Constants::FONT_REGULAR);
        btn->setTitleFontSize(12);
        if(i == this->correctAnswer) {
            btn->setTitleText(showTerm ? word : text);
        } else {
            btn->setTitleText(wrongAnswers.back());
            wrongAnswers.pop_back();
        }
        btn->getTitleRenderer()->setWidth(visibleSize.width - 56);
        btn->getTitleRenderer()->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        int btnHeight = btn->getTitleRenderer()->getContentSize().height + 10;
        if(btnHeight < 30) btnHeight = 30;
        btn->setSize(Size(visibleSize.width-28, btnHeight));
        btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
        this->addChild(btn, 10+i, 10+i);
        startY -= btnHeight + 14;
    }
    
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
    btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
    this->addChild(btn, 99, 99);
    btn->setEnabled(false);
    btn->setBright(false);
    
    // init vars
    this->startTime = time(0);
    this->showNext = false;
    this->answerChoice = -1;
    this->mode = TestMode::MULTIPLE_CHOICE;
    
    return true;
}

bool TestScene::initTrueFalse(const std::string& word)
{
    // Retrieve data
    const ValueMap& wordsConfig = Configuration::getInstance()->getValue("words").asValueMap();
    this->correctAnswer = rand() % 2;
    
    std::string correctText;
    const ValueMap& wInfo = wordsConfig.at(word).asValueMap();
    size_t testAvail = wInfo.at("sdef").asValueMap().size();
    int randNum = rand() % testAvail;
    if(randNum == 0) {
        correctText = wInfo.at("sdef").asValueMap().at("sdef1").asString();
    } else if (randNum == 1) {
        correctText = wInfo.at("sdef").asValueMap().at("sdef2").asString();
    } else if (randNum == 2) {
        correctText = wInfo.at("sdef").asValueMap().at("sdef3").asString();
    }
    
    std::string incorrectText;
    if(correctAnswer == 1) { // false
        while(true) {
            auto randIt = wordsConfig.cbegin();
            std::advance(randIt, rand() % wordsConfig.size());
            if(randIt->first == word) continue;
            incorrectText = randIt->second.asValueMap().at("sdef").asValueMap().at("sdef1").asString();
            break;
        }
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Home btn
    auto btn = ui::Button::create("HomeBtn", "HomeBtn", "", ui::Widget::TextureResType::PLIST);
    btn->setPosition(Vec2(origin.x + visibleSize.width - 28, origin.y + visibleSize.height - 26));
    btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
    this->addChild(btn, 1, 1);
    
    // Progress txt
    float retainedPercent = round(Constants::getTestedCount() * 100 / Constants::getTotalTestCount());
    std::stringstream stm;
    stm << "Testing progress: " << retainedPercent << "%";
    auto label = Label::createWithTTF(stm.str(), Constants::FONT_MEDIUM, 12);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(origin.x + 14, origin.y + visibleSize.height - 28);
    this->addChild(label, 2, 2);
    
    // Progress bar
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
    
    label = Label::createWithTTF("Choose the correct answer", Constants::FONT_MEDIUM_ITALIC, 18);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setPosition(origin.x + 14, origin.y + visibleSize.height - 94);
    this->addChild(label, 4, 4);
    
    float startY = label->getPosition().y - label->getContentSize().height/2 - 14;
    label = Label::createWithTTF(word, Constants::FONT_BOLD, 12);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(origin.x + 14, startY);
    label->setName(correctText);
    this->addChild(label, 5, 5);
    
    startY = label->getPosition().y - label->getContentSize().height - 5;
    label = Label::createWithTTF(correctAnswer == 0 ? correctText : incorrectText, Constants::FONT_REGULAR, 12);
    label->setColor(Color3B::BLACK);
    label->setWidth(visibleSize.width - 28);
    label->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(origin.x + 14, startY);
    this->addChild(label, 6, 6);
    
    startY = label->getPosition().y - label->getContentSize().height - 14;
    for(int i = 0; i < 2; i++) { // answer choices
        auto btn = ui::Button::create("ChoiceBg", "ChoiceSelectedBg", "", ui::Widget::TextureResType::PLIST);
        btn->setScale9Enabled(true);
        btn->setCapInsets(Rect(5, 5, 5, 5));
        btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        btn->setPosition(Vec2(origin.x + visibleSize.width/2, startY));
        btn->setTitleColor(Color3B::BLACK);
        btn->setTitleFontName(Constants::FONT_REGULAR);
        btn->setTitleFontSize(12);
        btn->setTitleText(i == 0 ? "True" : "False");
        btn->getTitleRenderer()->setWidth(visibleSize.width - 56);
        btn->getTitleRenderer()->setAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
        int btnHeight = btn->getTitleRenderer()->getContentSize().height + 10;
        if(btnHeight < 30) btnHeight = 30;
        btn->setSize(Size(visibleSize.width-28, btnHeight));
        btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
        this->addChild(btn, 10+i, 10+i);
        startY -= btnHeight + 14;
    }
    
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
    btn->addTouchEventListener(CC_CALLBACK_2(TestScene::touchEvent, this));
    this->addChild(btn, 99, 99);
    btn->setEnabled(false);
    btn->setBright(false);
    
    // init vars
    this->startTime = time(0);
    this->showNext = false;
    this->answerChoice = -1;
    this->mode = TestMode::TRUE_FALSE;
    
    return true;
}

void TestScene::touchEvent(Ref *pSender, ui::Widget::TouchEventType type)
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
                auto transition = TransitionPageTurn::create(0.25f, HelloWorld::createScene(), true);
                Director::getInstance()->replaceScene(transition);
            } else if(tag >= 10 && tag <= 13) {
                int choice = tag % 10;
                if(choice == answerChoice) return;
                answerChoice = choice;
                
                // update GUI
                ((ui::Button*)this->getChildByTag(10))->loadTextureNormal(tag != 10 ? "ChoiceBg" : "ChoiceSelectedBg", ui::Widget::TextureResType::PLIST);
                ((ui::Button*)this->getChildByTag(11))->loadTextureNormal(tag != 11 ? "ChoiceBg" : "ChoiceSelectedBg", ui::Widget::TextureResType::PLIST);
                if(mode == TestMode::MULTIPLE_CHOICE) {
                    ((ui::Button*)this->getChildByTag(12))->loadTextureNormal(tag != 12 ? "ChoiceBg" : "ChoiceSelectedBg", ui::Widget::TextureResType::PLIST);
                    ((ui::Button*)this->getChildByTag(13))->loadTextureNormal(tag != 13 ? "ChoiceBg" : "ChoiceSelectedBg", ui::Widget::TextureResType::PLIST);
                }
                ((ui::Button*)this->getChildByTag(99))->setEnabled(true);
                ((ui::Button*)this->getChildByTag(99))->setBright(true);
            } else if(tag == 99) { // check
                if(showNext) {
                    auto transition = TransitionPageTurn::create(0.25f, TestScene::createScene(), false);
                    Director::getInstance()->replaceScene(transition);
                } else if(mode == TestMode::FILL_IN) {
                    // check answer
                    ui::EditBox* editBox = (ui::EditBox*)this->getChildByTag(5);
                    std::string word(editBox->getName());
                    std::string answer(editBox->getText());
                    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
                    
                    bool correct = word == answer;
                    time_t currTime = time(0);
                    int score = currTime - startTime <= 16 ? 5 : currTime - startTime <= 26 ? 4 : 3;
                    score = correct ? score : 5 - score;
                    Constants::handleTest(this->getName(), score);
                    
                    // show badge
                    auto badge = Node::create();
                    auto badgeBg = Sprite::createWithSpriteFrameName(correct? "CorrectBadge" : "IncorrectBadge");
                    badgeBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    badge->addChild(badgeBg);
                    auto label = Label::createWithTTF(correct? "You are correct" : "Oops, that's not correct", Constants::FONT_BOLD, correct? 24 : 16);
                    label->setColor(correct? Color3B(80,143,6) : Color3B(245,23,50));
                    label->setAnchorPoint(correct? Vec2::ANCHOR_MIDDLE_LEFT : Vec2::ANCHOR_BOTTOM_LEFT);
                    label->setPosition(Vec2(14, badgeBg->getContentSize().height/2));
                    badge->addChild(label);
                    if(!correct) {
                        label = Label::createWithTTF(word, Constants::FONT_REGULAR, 12);
                        label->setColor(Color3B(245,23,50));
                        label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                        label->setPosition(Vec2(14, badgeBg->getContentSize().height/2));
                        badge->addChild(label);
                    }
                    badge->setPosition(Director::getInstance()->getVisibleOrigin());
                    this->addChild(badge, 999);
                    badge->runAction(MoveBy::create(0.22f, Vec2(0, 136)));
                    
                    // Update GUI
                    editBox->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(99))->setTitleText("Continue");
                    showNext = true;
                } else if(mode == TestMode::MULTIPLE_CHOICE) {
                    bool correct = answerChoice == correctAnswer;
                    time_t currTime = time(0);
                    int score = currTime - startTime <= 16 ? 5 : currTime - startTime <= 26 ? 4 : 3;
                    score = correct ? score : 5 - score;
                    Constants::handleTest(this->getName(), score);
                    
                    // show badge
                    auto badge = Node::create();
                    auto badgeBg = Sprite::createWithSpriteFrameName(correct? "CorrectBadge" : "IncorrectBadge");
                    badgeBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    badge->addChild(badgeBg, 1, 1);
                    auto label = Label::createWithTTF(correct? "You are correct" : "Oops, that's not correct", Constants::FONT_BOLD, correct ? 24 : 16);
                    label->setColor(correct? Color3B(80,143,6) : Color3B(245,23,50));
                    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    label->setPosition(Vec2(14, badgeBg->getContentSize().height/2));
                    badge->addChild(label, 2, 2);
                    if(!correct) {
                        float titleHeight = label->getContentSize().height - 2;
                        const std::string& s = ((ui::Button*)this->getChildByTag(10+correctAnswer))->getTitleText();
                        label = Label::createWithTTF(s, Constants::FONT_REGULAR, 12);
                        label->setColor(Color3B(245,23,50));
                        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                        label->setPosition(Vec2(14, badgeBg->getContentSize().height/2 - titleHeight/2 + 2));
                        label->setWidth(200);
                        label->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
                        badge->addChild(label, 3, 3);
                        ((Node*)badge->getChildByTag(2))->setPositionY(label->getPositionY() + label->getContentSize().height/2 + titleHeight/2 + 2);
                    }
                    badge->setPosition(Director::getInstance()->getVisibleOrigin());
                    this->addChild(badge, 999);
                    badge->runAction(MoveBy::create(0.13f, Vec2(0, 80)));
                    
                    // Update GUI
                    ((ui::Button*)this->getChildByTag(10))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(11))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(12))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(13))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(10+correctAnswer))->loadTextureNormal("ChoiceCorrectBg", ui::Widget::TextureResType::PLIST);
                    ((ui::Button*)this->getChildByTag(99))->setTitleText("Continue");
                    showNext = true;
                } else if(mode == TestMode::TRUE_FALSE) {
                    bool correct = answerChoice == correctAnswer;
                    time_t currTime = time(0);
                    int score = currTime - startTime <= 6 ? 5 : currTime - startTime <= 16 ? 4 : 3;
                    score = correct ? score : 5 - score;
                    Constants::handleTest(this->getName(), score);
                    
                    // show badge
                    auto badge = Node::create();
                    auto badgeBg = Sprite::createWithSpriteFrameName(correct? "CorrectBadge" : "IncorrectBadge");
                    badgeBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    badge->addChild(badgeBg, 1, 1);
                    auto label = Label::createWithTTF(correct? "You are correct" : "Oops, that's not correct", Constants::FONT_BOLD, (correct && correctAnswer == 0) ? 24 : 16);
                    label->setColor(correct? Color3B(80,143,6) : Color3B(245,23,50));
                    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    label->setPosition(Vec2(14, badgeBg->getContentSize().height/2));
                    badge->addChild(label, 2, 2);
                    if(correctAnswer == 1) {
                        float titleHeight = label->getContentSize().height - 2;
                        std::string word = ((Label*)this->getChildByTag(5))->getString();
                        word.append(" - ").append(((Label*)this->getChildByTag(5))->getName());
                        label = Label::createWithTTF(word, Constants::FONT_REGULAR, 12);
                        label->setColor(correct? Color3B(80,143,6) : Color3B(245,23,50));
                        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                        label->setPosition(Vec2(14, badgeBg->getContentSize().height/2 - titleHeight/2 + 2));
                        label->setWidth(200);
                        label->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
                        badge->addChild(label, 3, 3);
                        ((Node*)badge->getChildByTag(2))->setPositionY(label->getPositionY() + label->getContentSize().height/2 + titleHeight/2 + 2);
                    }
                    badge->setPosition(Director::getInstance()->getVisibleOrigin());
                    this->addChild(badge, 999);
                    badge->runAction(MoveBy::create(0.13f, Vec2(0, 80)));
                    
                    // Update GUI
                    ((ui::Button*)this->getChildByTag(10))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(11))->setEnabled(false);
                    ((ui::Button*)this->getChildByTag(10+correctAnswer))->loadTextureNormal("ChoiceCorrectBg", ui::Widget::TextureResType::PLIST);
                    ((ui::Button*)this->getChildByTag(99))->setTitleText("Continue");
                    showNext = true;
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

void TestScene::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
    
}
void TestScene::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
    
}
void TestScene::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{

}
void TestScene::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
    std::string answer(editBox->getText());
    if(!answer.empty()) {
        ((ui::Button*)this->getChildByTag(99))->setEnabled(true);
        ((ui::Button*)this->getChildByTag(99))->setBright(true);
    } else {
        ((ui::Button*)this->getChildByTag(99))->setEnabled(false);
        ((ui::Button*)this->getChildByTag(99))->setBright(false);
    }
}
