#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "Constants.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // load config
    auto config = Configuration::getInstance();
    config->loadConfigFile("config/words.plist");
    
    // init word data
    Constants::initWordData();
    
    // random seed
    srand( unsigned( time(0) ) );
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    auto screenSize = glview->getFrameSize();
    auto designSize = Size(320, 480);
    
    std::vector<std::string> searchPaths;
    if (screenSize.height > 960)
    {
        searchPaths.push_back("HDR");
        searchPaths.push_back("HD");
        searchPaths.push_back("SD");
        director->setContentScaleFactor(4.0f);
    }
    else if (screenSize.height > 480)
    {
        searchPaths.push_back("HD");
        searchPaths.push_back("SD");
        director->setContentScaleFactor(2.0f);
    }
    else
    {
        searchPaths.push_back("SD");
        director->setContentScaleFactor(1.0f);
    }
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    auto cacher = SpriteFrameCache::getInstance();
    cacher->addSpriteFramesWithFile("assets.plist");
    
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
