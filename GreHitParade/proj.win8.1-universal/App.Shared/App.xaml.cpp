#include "App.xaml.h"
#include "OpenGLESPage.xaml.h"

using namespace cocos2d;

using namespace Concurrency;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::Media::SpeechSynthesis;

App^ App::_instance;
SpeechSynthesizer ^synth;
MediaElement ^media;

App::App()
{
    InitializeComponent();
	synth = ref new SpeechSynthesizer();
	media = ref new MediaElement();
	media->AutoPlay = true;
	_instance = this;
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    if (mPage == nullptr)
    {
        mPage = ref new OpenGLESPage(&mOpenGLES);
    }

    // Place the page in the current window and ensure that it is active.
    Windows::UI::Xaml::Window::Current->Content = mPage;
    Windows::UI::Xaml::Window::Current->Activate();
}

void App::playSound(Platform::String ^text)
{
	this->mPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([text]()
	{
		task<SpeechSynthesisStream ^> speakTask = create_task(synth->SynthesizeTextToStreamAsync(text));
		speakTask.then([](SpeechSynthesisStream ^speechStream)
		{
			media->SetSource(speechStream, speechStream->ContentType);
			media->Play();
		});
	}));
}

App^ App::getInstance()
{
	return _instance;
}