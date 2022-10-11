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

	// �� �̸� ����
	char* name;

	// ��ġ ����
	MenuItemSprite* touches[4];

	// �г� ����
	Sprite* pannals[4];

	// �����ID ����
	int audioID;

	// �ð� ����
	float time;

	// ������Ʈ �Լ�
	void update(float _delta);

	// Ű ���� �ݹ� �Լ�
	void onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event);

	// Ű �� �ݹ� �Լ�
	void onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event);

	// ��ġ ���� �ݹ� �Լ�
	virtual void onTouchesBegan(const std::vector<Touch*>& _touches, Event* _event);

	// ��ġ ������ �ݹ� �Լ�
	virtual void onTouchesMoved(const std::vector<Touch*>& _touches, Event* _event);

	// ��ġ ��� �ݹ� �Լ�
	virtual void onTouchesCancelled(const std::vector<Touch*>& _touches, Event* _event);

	// ��ġ ���� �ݹ� �Լ�
	virtual void onTouchesEnded(const std::vector<Touch*>& _touches, Event* _event);

	// ���� �ݹ� �Լ�
	void startCallback();

	// ��� ���� �Լ�
	void createBackground();

	// ��ġ ���� �Լ�
	void createTouch();

	// �г� ���� �Լ�
	void createPannal();

	// Ű���� �̺�Ʈ ������ ���� �Լ�
	void createKeyboardListener();

	// ��ġ �̺�Ʈ ������ ���� �Լ�
	void createTouchListener();

	// ���� ���� �׼� ��ȯ �Լ�
	Sequence* createStartAction();

	// ��ġ ���� �Լ�
	void touch(int _tag, bool _touch);

	// ��ġ �̺�Ʈ �Լ�
	void touchEvent(const std::vector<Touch*>& _touches, bool _isTrue);

	// Z���� ���
	enum ZOrder
	{
		Z_BACKGROUND, Z_TOUCH, Z_PANNAL
	};

public:
	// ������
	EditScene();

	// �Ҹ���
	~EditScene();
};

#endif
