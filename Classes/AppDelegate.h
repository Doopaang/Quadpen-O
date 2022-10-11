#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

class  AppDelegate : private cocos2d::Application
{
public:
	virtual void initGLContextAttrs();

	// 실행 함수
	virtual bool applicationDidFinishLaunching();

	// 창 내림 함수
	virtual void applicationDidEnterBackground();

	// 창 올림 함수
	virtual void applicationWillEnterForeground();

public:
	// 생성자
	AppDelegate();

	// 소멸자
	virtual ~AppDelegate();
};

#endif

