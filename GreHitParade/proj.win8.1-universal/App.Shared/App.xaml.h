#pragma once

#include "app.g.h"
#include "OpenGLES.h"
#include "openglespage.xaml.h"

namespace cocos2d
{
    ref class App sealed
    {
    public:
        App();
        virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
		void playSound(Platform::String ^text);
		static App^ getInstance();

    private:
        OpenGLESPage^ mPage;
        OpenGLES mOpenGLES;
		static App^ _instance;
    };
}
