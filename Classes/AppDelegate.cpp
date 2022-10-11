#include "AppDelegate.h"
#include "MainScene.h"
#include "AudioEngine.h"

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(800, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{

}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Rhythm", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("Rhythm");
#endif
        director->setOpenGLView(glview);
    }

    // ���� ���
    director->setDisplayStats(false);

    // FPS ����
    director->setAnimationInterval(1.0f / 60);

    // �ػ� ����
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
    auto frameSize = glview->getFrameSize();
    // ȭ�� ���̰� ���� ������� Ŭ ��
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // ȭ�� ���̰� ���� ������� Ŭ ��
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // ȭ�� ���̰� ���� ������� ���� ��
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	// ���� �� ����
    auto scene = MainScene::createScene();

    // ����
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

	AudioEngine::pauseAll();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

	AudioEngine::resumeAll();
}