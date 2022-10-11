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

	// �޴� ����
	MenuItemToggle* menus[4];

	// �޴� ��� ����
	Sprite* menuBackground[4];

	// �޴� ������ ����
	MenuItemLabel* menuItem[4];

	// ��ũ�� �� ����
	ScrollView* menuLayer[4];

	// ���� ����
	int select;

	// Ű ���� �ݹ� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event);

	// Ű �� �ݹ� �Լ�
	void onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event);

	// �޴� �ݹ� �Լ�
	void menuCallback(Ref* _sender);

	// ���� �Լ�
	void end(Ref* _sender);

	// ��� ���� �Լ�
	void createBackground();

	// �޴� ���� �Լ�
	void createMenu();

	// �޴� ��ư ���� �Լ�
	void createMenuButton();

	// Ű���� �̺�Ʈ ������ ���� �Լ�
	void createKeyboardListener();

	// ��ġ �̺�Ʈ ������ ���� �Լ�
	void createTouchListener();

	// �޴� ��ư ���� �Լ�
	void setMenuButton(int _tag);

	// �޴� ���� �׼� ��ȯ �Լ�
	EaseOut* createMenuAction(int _tag, Size _size);

	// �޴� ���� �׼� ��ȯ �Լ�
	EaseIn* removeMenuAction(int _tag, Size _size);

	// ȭ�� �̵� �Լ�
	void changeScene();

	// Z���� ���
	enum ZOrder
	{
		Z_BACKGROUND,
		Z_BACK,
		Z_MENU,
		Z_TOP
	};

	// �޴� ���
	enum MenuKind
	{
		M_NONE = -1,
		M_START,
		M_EDIT,
		M_OPTION,
		M_EXIT
	};

public:
	// ������
	MainScene();

	// �Ҹ���
	~MainScene();
};

#endif
