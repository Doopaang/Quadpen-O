#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Note;

class MainScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(MainScene);

private:
	Size visibleSize;
	Vec2 origin;

	// 메뉴 변수
	MenuItemToggle* menus[4];

	// 메뉴 배경 변수
	Sprite* menuBackground[4];

	// 메뉴 아이템 변수
	MenuItemLabel* menuItem[4];

	// 스크롤 뷰 변수
	ScrollView* menuLayer[4];

	// 선택 변수
	int select;

	// 키 누름 콜백 함수
	void onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event);

	// 키 땜 콜백 함수
	void onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event);

	// 메뉴 콜백 함수
	void menuCallback(Ref* _sender);

	// 종료 함수
	void end(Ref* _sender);

	// 배경 생성 함수
	void createBackground();

	// 메뉴 생성 함수
	void createMenu();

	// 메뉴 버튼 생성 함수
	void createMenuButton();

	// 키보드 이벤트 리스너 생성 함수
	void createKeyboardListener();

	// 터치 이벤트 리스너 생성 함수
	void createTouchListener();

	// 메뉴 버튼 설정 함수
	void setMenuButton(int _tag);

	// 메뉴 생성 액션 반환 함수
	EaseOut* createMenuAction(int _tag, Size _size);

	// 메뉴 삭제 액션 반환 함수
	EaseIn* removeMenuAction(int _tag, Size _size);

	// 화면 이동 함수
	void changeScene();

	// Z오더 상수
	enum ZOrder
	{
		Z_BACKGROUND,
		Z_BACK,
		Z_MENU,
		Z_TOP
	};

	// 메뉴 상수
	enum MenuKind
	{
		M_NONE = -1,
		M_START,
		M_EDIT,
		M_OPTION,
		M_EXIT
	};

public:
	// 생성자
	MainScene();

	// 소멸자
	~MainScene();
};

#endif
