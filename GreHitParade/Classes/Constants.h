//
//  Constants.h
//
//  Created by Lâm Thái on 2/26/14.
//
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include "cocos2d.h"

class Constants
{
public:
    // consts
    static const unsigned int NEW_WORD_PER_TIME;
    static const unsigned int NEW_TEST_PER_TIME;
    
    // fonts
    static const std::string FONT_REGULAR12;
    static const std::string FONT_REGULAR14;
    static const std::string FONT_BOLD12;
    static const std::string FONT_BOLD16;
    static const std::string FONT_BOLD24;
    static const std::string FONT_BOLD30;
    static const std::string FONT_SYSTEM;
    
    // colors
    static const cocos2d::Color4B BACKGROUND_COLOR;
    
    // show ad time
    static time_t lastShowAdTime;
    static time_t appEnterForegroundTime;
    
    // static functions
    static std::string getNextWordToLearn();
    static std::string getNextWordToTest();
    static void handleWord(const std::string& w, int score /*0->5*/);
    static void handleTest(const std::string& w, int score /*0->5*/);
    static float getRetainedWordCount();
    static float getTestedCount();
    static size_t getTotalWordCount();
    static size_t getTotalTestCount();
    static void initWordData();
    static void playText(const std::string& text);
    static bool showAd();
    static void openInAppStore();
    
private:
    static bool pullNewWordToLearn();
    static bool pullNewWordToTest();
    static int dayUntilNextJob();
    static int dayUntilNextTest();
    static cocos2d::Value wordData;
};

#endif
