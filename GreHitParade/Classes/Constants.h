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
    static const std::string FONT_LIGHT;
    static const std::string FONT_LIGHT_ITALIC;
    static const std::string FONT_REGULAR;
    static const std::string FONT_ITALIC;
    static const std::string FONT_MEDIUM;
    static const std::string FONT_MEDIUM_ITALIC;
    static const std::string FONT_BOLD;
    static const std::string FONT_BOLD_ITALIC;
    static const std::string FONT_EXTRA_BOLD;
    static const std::string FONT_EXTRA_BOLD_ITALIC;
    
    // colors
    static const cocos2d::Color4B BACKGROUND_COLOR;
    
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
    
private:
    static bool pullNewWordToLearn();
    static bool pullNewWordToTest();
    static int dayUntilNextJob();
    static int dayUntilNextTest();
    static cocos2d::Value wordData;
};

#endif
