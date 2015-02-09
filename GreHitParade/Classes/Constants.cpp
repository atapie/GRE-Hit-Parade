//
//  Constants.cpp
//
//  Created by Lâm Thái on 2/26/14.
//
//

#include "Constants.h"

USING_NS_CC;
using namespace std;

Value Constants::wordData;

// consts
const unsigned int Constants::NEW_WORD_PER_TIME = 10;
const unsigned int Constants::NEW_TEST_PER_TIME = 5;

// fonts
const string Constants::FONT_LIGHT = "fonts/OpenSans-Light.ttf";
const string Constants::FONT_LIGHT_ITALIC = "fonts/OpenSans-LightItalic.ttf";
const string Constants::FONT_REGULAR = "fonts/OpenSans-Regular.ttf";
const string Constants::FONT_ITALIC = "fonts/OpenSans-Italic.ttf";
const string Constants::FONT_MEDIUM = "fonts/OpenSans-Semibold.ttf";
const string Constants::FONT_MEDIUM_ITALIC = "fonts/OpenSans-SemiboldItalic.ttf";
const string Constants::FONT_BOLD = "fonts/OpenSans-Bold.ttf";
const string Constants::FONT_BOLD_ITALIC = "fonts/OpenSans-BoldItalic.ttf";
const string Constants::FONT_EXTRA_BOLD = "fonts/OpenSans-ExtraBold.ttf";
const string Constants::FONT_EXTRA_BOLD_ITALIC = "fonts/OpenSans-ExtraBoldItalic.ttf";

// colors
const Color4B Constants::BACKGROUND_COLOR(231, 240, 211, 255);

// static functions
string Constants::getNextWordToLearn()
{
    ValueMap& data = wordData.asValueMap();
    ValueVector& currJobVect = data["currentJob"].asValueVector();
    if(!currJobVect.empty()) {
        return currJobVect.at(rand() % currJobVect.size()).asString();
    } else {
        ValueVector& drillVect = data["drillJob"].asValueVector();
        if(!drillVect.empty()) {
            return drillVect.at(rand() % drillVect.size()).asString();
        } else { // push through the next 'day'
            bool hasNewWord = pullNewWordToLearn(); // try new words everyday
            int timeLapsed = hasNewWord ? 1 : dayUntilNextJob();
            ValueMap& learningMap = data["learningWords"].asValueMap();
            for(auto it = learningMap.begin(); it != learningMap.end(); ++it) {
                ValueMap& wData = it->second.asValueMap();
                wData["daysRemaining"] = wData["daysRemaining"].asInt() - timeLapsed;
                if(wData["daysRemaining"].asInt() <= 0) {
                    currJobVect.push_back(Value(it->first));
                }
            }
            return getNextWordToLearn();
        }
    }
}

string Constants::getNextWordToTest()
{
    ValueMap& data = wordData.asValueMap();
    ValueVector& currTestVect = data["currentTest"].asValueVector();
    if(!currTestVect.empty()) {
        return currTestVect.at(rand() % currTestVect.size()).asString();
    } else {
        ValueVector& drillVect = data["drillTest"].asValueVector();
        if(!drillVect.empty()) {
            return drillVect.at(rand() % drillVect.size()).asString();
        } else { // push through the next 'day'
            bool hasNewWord = pullNewWordToTest(); // try new words everyday
            int timeLapsed = hasNewWord ? 1 : dayUntilNextTest();
            ValueMap& testingMap = data["testingWords"].asValueMap();
            for(auto it = testingMap.begin(); it != testingMap.end(); ++it) {
                ValueMap& wData = it->second.asValueMap();
                wData["daysRemaining"] = wData["daysRemaining"].asInt() - timeLapsed;
                if(wData["daysRemaining"].asInt() <= 0) {
                    currTestVect.push_back(Value(it->first));
                }
            }
            return getNextWordToTest();
        }
    }
}

void Constants::handleWord(const std::string &w, int score)
{
    Value wValue = Value(w);
    ValueVector& drillVect = wordData.asValueMap()["drillJob"].asValueVector();
    auto it = find(drillVect.begin(), drillVect.end(), wValue);
    if(it != drillVect.end()) { // word in drill job
        if(score >= 3) { // remembered, let it go
            drillVect.erase(it);
        }
    } else {
        ValueMap& learningMap = wordData.asValueMap()["learningWords"].asValueMap();
        auto it2 = learningMap.find(w);
        if(it2 == learningMap.end()) {
            learningMap.insert(make_pair<string, Value>(string(w), Value(ValueMap())));
            it2 = learningMap.find(w);
        }
        ValueMap& wData = it2->second.asValueMap();
        if(wData.empty()) {
            wData.insert(make_pair("EF", Value(2.5f)));
            wData.insert(make_pair("repetitionVal", Value(0)));
            wData.insert(make_pair("repetitionCount", Value(0)));
            wData.insert(make_pair("daysRemaining", Value(0)));
        }
        
        // handle word score
        if (score < 3) { // not remember yet, move it to drill list and reset schedule
            drillVect.push_back(wValue);
            wData["repetitionVal"] = 1;
            wData["repetitionCount"] = 1;
            wData["daysRemaining"] = 1;
        } else { // update EF factor
            float newEF = wData["EF"].asFloat() + (0.1f - (5-score) * (0.08f+(5-score)*0.02f));
            if(newEF < 1.3f) newEF = 1.3f;
            if(newEF > 2.5f) newEF = 2.5f;
            wData["EF"] = newEF;
            
            int newRVal = wData["repetitionVal"].asInt();
            if(newRVal == 0) newRVal = 1;
            else if(newRVal == 1) newRVal = 6;
            else newRVal = round(newRVal * newEF);
            if(newRVal > 730) newRVal = 730;
            wData["repetitionVal"] = newRVal;
            wData["repetitionCount"] = wData["repetitionCount"].asInt() + 1;
            CCLOG("days remaining %d", wData["daysRemaining"].asInt());
            wData["daysRemaining"] = newRVal;
        }
        
        // remove it from current job list
        ValueVector& currJobVect = wordData.asValueMap()["currentJob"].asValueVector();
        it = find(currJobVect.begin(), currJobVect.end(), wValue);
        if(it != currJobVect.end()) {
            currJobVect.erase(it);
        }
    }
    
    // write to file
    std::string dataPath = FileUtils::getInstance()->getWritablePath().append("wordData.plist");
    FileUtils::getInstance()->writeToFile(wordData.asValueMap(), dataPath);
}

void Constants::handleTest(const std::string &w, int score)
{
    Value wValue = Value(w);
    ValueVector& drillVect = wordData.asValueMap()["drillTest"].asValueVector();
    auto it = find(drillVect.begin(), drillVect.end(), wValue);
    if(it != drillVect.end()) { // word in drill job
        if(score >= 3) { // remembered, let it go
            drillVect.erase(it);
        }
    } else {
        ValueMap& testingMap = wordData.asValueMap()["testingWords"].asValueMap();
        auto it2 = testingMap.find(w);
        if(it2 == testingMap.end()) {
            testingMap.insert(make_pair<string, Value>(string(w), Value(ValueMap())));
            it2 = testingMap.find(w);
        }
        ValueMap& wData = it2->second.asValueMap();
        if(wData.empty()) {
            wData.insert(make_pair("EF", Value(2.5f)));
            wData.insert(make_pair("repetitionVal", Value(0)));
            wData.insert(make_pair("repetitionCount", Value(0)));
            wData.insert(make_pair("daysRemaining", Value(0)));
        }
        
        // handle word score
        if (score < 3) { // not remember yet, move it to drill list and reset schedule
            drillVect.push_back(wValue);
            wData["repetitionVal"] = 1;
            wData["repetitionCount"] = 1;
            wData["daysRemaining"] = 1;
        } else { // update EF factor
            float newEF = wData["EF"].asFloat() + (0.1f - (5-score) * (0.08f+(5-score)*0.02f));
            if(newEF < 1.3f) newEF = 1.3f;
            if(newEF > 2.5f) newEF = 2.5f;
            wData["EF"] = newEF;
            
            int newRVal = wData["repetitionVal"].asInt();
            if(newRVal == 0) newRVal = 1;
            else if(newRVal == 1) newRVal = 6;
            else newRVal = round(newRVal * newEF);
            if(newRVal > 730) newRVal = 730;
            wData["repetitionVal"] = newRVal;
            wData["repetitionCount"] = wData["repetitionCount"].asInt() + 1;
            CCLOG("days remaining %d", wData["daysRemaining"].asInt());
            wData["daysRemaining"] = newRVal;
        }
        
        // remove it from current job list
        ValueVector& currTestVect = wordData.asValueMap()["currentTest"].asValueVector();
        it = find(currTestVect.begin(), currTestVect.end(), wValue);
        if(it != currTestVect.end()) {
            currTestVect.erase(it);
        }
    }
    
    // write to file
    std::string dataPath = FileUtils::getInstance()->getWritablePath().append("wordData.plist");
    FileUtils::getInstance()->writeToFile(wordData.asValueMap(), dataPath);
}

float Constants::getRetainedWordCount()
{
    float result = 0;
    ValueMap& learningMap = wordData.asValueMap()["learningWords"].asValueMap();
    for(auto it = learningMap.begin(); it != learningMap.end(); ++it) {
        int rpCount = it->second.asValueMap()["repetitionCount"].asInt();
        float retainPercent = rpCount * 0.25f;
        if(retainPercent < 0) retainPercent = 0;
        if(retainPercent > 1) retainPercent = 1;
        result += retainPercent;
    }
    return result;
}

float Constants::getTestedCount()
{
    float result = 0;
    ValueMap& testingMap = wordData.asValueMap()["testingWords"].asValueMap();
    for(auto it = testingMap.begin(); it != testingMap.end(); ++it) {
        int rpCount = it->second.asValueMap()["repetitionCount"].asInt();
        float retainPercent = rpCount * 0.25f;
        if(retainPercent < 0) retainPercent = 0;
        if(retainPercent > 1) retainPercent = 1;
        result += retainPercent;
    }
    return result;
}

size_t Constants::getTotalWordCount()
{
    const ValueMap& wordsConfig = Configuration::getInstance()->getValue("words").asValueMap();
    return wordsConfig.size();
}

size_t Constants::getTotalTestCount()
{
    return getTotalWordCount() * 2;
}

void Constants::initWordData()
{
    if(wordData.isNull()) { // init data
        auto fileUtils = FileUtils::getInstance();
        string dataPath = fileUtils->getWritablePath().append("wordData.plist");
        if(! fileUtils->isFileExist(dataPath) )
        {
            wordData = Value(ValueMap());
            
            // test
            wordData.asValueMap().insert(make_pair<string, Value>("currentTest", Value(ValueVector())));
            wordData.asValueMap().insert(make_pair<string, Value>("drillTest", Value(ValueVector())));
            wordData.asValueMap().insert(make_pair<string, Value>("poolTest", Value(ValueVector())));
            wordData.asValueMap().insert(make_pair<string, Value>("testingWords", Value(ValueMap())));
            
            // learn
            wordData.asValueMap().insert(make_pair<string, Value>("currentJob", Value(ValueVector())));
            wordData.asValueMap().insert(make_pair<string, Value>("drillJob", Value(ValueVector())));
            wordData.asValueMap().insert(make_pair<string, Value>("poolWords", Value(ValueVector())));
            wordData.asValueMap().insert(make_pair<string, Value>("learningWords", Value(ValueMap())));
            
            const ValueMap& wordsConfig = Configuration::getInstance()->getValue("words").asValueMap();
            ValueVector& poolVect = wordData.asValueMap()["poolWords"].asValueVector();
            ValueVector& poolTestVect = wordData.asValueMap()["poolTest"].asValueVector();
            for(auto it = wordsConfig.cbegin(); it != wordsConfig.cend(); ++it)
            { // put all words into pools
                poolVect.push_back(Value(it->first));
                poolTestVect.push_back(Value(it->first));
            }
            // First batch of new words
            pullNewWordToLearn();
            pullNewWordToTest();
        }
        else
        {
            wordData = Value(fileUtils->getValueMapFromFile(dataPath));
        }
    }
}

bool Constants::pullNewWordToLearn()
{
    ValueVector& poolVect = wordData.asValueMap()["poolWords"].asValueVector();
    ValueVector& currJobVect = wordData.asValueMap()["currentJob"].asValueVector();
    if(!poolVect.empty())
    {
        size_t newWordCount = MIN(NEW_WORD_PER_TIME, poolVect.size());
        move(poolVect.begin(), poolVect.begin()+newWordCount, back_inserter(currJobVect));
        poolVect.erase(poolVect.begin(), poolVect.begin()+newWordCount);
        return true;
    }
    return false;
}

bool Constants::pullNewWordToTest()
{
    ValueVector& poolTestVect = wordData.asValueMap()["poolTest"].asValueVector();
    ValueVector& currTestVect = wordData.asValueMap()["currentTest"].asValueVector();
    if(!poolTestVect.empty())
    {
        size_t newTestCount = MIN(NEW_TEST_PER_TIME, poolTestVect.size());
        for(auto it = poolTestVect.begin(); it != poolTestVect.begin()+newTestCount; ++it) {
            string s = it->asString();
            s.push_back('a');
            currTestVect.push_back(Value(s));
            s.pop_back();
            s.push_back('b');
            currTestVect.push_back(Value(s));
        }
        poolTestVect.erase(poolTestVect.begin(), poolTestVect.begin()+newTestCount);
        return true;
    }
    return false;
}

int Constants::dayUntilNextJob()
{
    int result = 9999;
    ValueMap& learningMap = wordData.asValueMap()["learningWords"].asValueMap();
    for(auto it = learningMap.begin(); it != learningMap.end(); ++it) {
        ValueMap& wData = it->second.asValueMap();
        if(result > wData["daysRemaining"].asInt()) result = wData["daysRemaining"].asInt();
    }
    return result;
}

int Constants::dayUntilNextTest()
{
    int result = 9999;
    ValueMap& testingMap = wordData.asValueMap()["testingWords"].asValueMap();
    for(auto it = testingMap.begin(); it != testingMap.end(); ++it) {
        ValueMap& wData = it->second.asValueMap();
        if(result > wData["daysRemaining"].asInt()) result = wData["daysRemaining"].asInt();
    }
    return result;
}
