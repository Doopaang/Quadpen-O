#ifndef __EDIT_SCENE_H__
#define __EDIT_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

class EditScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(EditScene);

private:
	Size visibleSize;
	Vec2 origin;

	// 곡 이름 변수
	char* name;

	// 터치 변수
	MenuItemSprite* touches[4];

	// 패널 변수
	Sprite* pannals[4];

	// 오디오ID 변수
	int audioID;

	// 시간 변수
	float time;

	// 업데이트 함수
	void update(float _delta);

	// 키 누름 콜백 함수
	void onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event);

	// 키 땜 콜백 함수
	void onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event);

	// 터치 누름 콜백 함수
	virtual void onTouchesBegan(const std::vector<Touch*>& _touches, Event* _event);

	// 터치 움직임 콜백 함수
	virtual void onTouchesMoved(const std::vector<Touch*>& _touches, Event* _event);

	// 터치 취소 콜백 함수
	virtual void onTouchesCancelled(const std::vector<Touch*>& _touches, Event* _event);

	// 터치 끝남 콜백 함수
	virtual void onTouchesEnded(const std::vector<Touch*>& _touches, Event* _event);

	// 시작 콜백 함수
	void startCallback();

	// 배경 생성 함수
	void createBackground();

	// 터치 생성 함수
	void createTouch();

	// 패널 생성 함수
	void createPannal();

	// 키보드 이벤트 리스너 생성 함수
	void createKeyboardListener();

	// 터치 이벤트 리스너 생성 함수
	void createTouchListener();

	// 게임 시작 액션 반환 함수
	Sequence* createStartAction();

	// 터치 동작 함수
	void touch(int _tag, bool _touch);

	// 터치 이벤트 함수
	void touchEvent(const std::vector<Touch*>& _touches, bool _isTrue);

	// Z오더 상수
	enum ZOrder
	{
		Z_BACKGROUND, Z_TOUCH, Z_PANNAL
	};

public:
	// 생성자
	EditScene();

	// 소멸자
	~EditScene();
};

#endif
