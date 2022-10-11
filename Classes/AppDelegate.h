#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

class  AppDelegate : private cocos2d::Application
{
public:
	virtual void initGLContextAttrs();

	// ���� �Լ�
	virtual bool applicationDidFinishLaunching();

	// â ���� �Լ�
	virtual void applicationDidEnterBackground();

	// â �ø� �Լ�
	virtual void applicationWillEnterForeground();

public:
	// ������
	AppDelegate();

	// �Ҹ���
	virtual ~AppDelegate();
};

#endif

