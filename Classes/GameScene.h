#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;
using namespace experimental;

class Note;

class GameScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(GameScene);

	// ��Ʈ ��ħ �ݹ� �Լ�
	void missNote(Ref* _sender);

private:
	Size visibleSize;
	Vec2 origin;

	// �� �̸� ����
	char* name;

	// ��ġ ����
	MenuItemSprite* touches[4];

	// �г� ����
	Sprite* pannals[4];

	// ���� �� ����
	Label* judgeLabel;

	// �޺� �� ����
	Label* comboLabel;

	// ��Ʈ ����
	Note* note;

	// �����ID ����
	int audioID;

	// �ð� ����
	float time;

	// ���� ����
	int beat;

	// �޺� ����
	int combo;

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

	// ��Ʈ ���� �Լ�
	void manageNote();

	// ��Ʈ �ε� �Լ�
	void loadNote();

	// ��� ���� �Լ�
	void createBackground();

	// ��ġ ���� �Լ�
	void createTouch();

	// �г� ���� �Լ�
	void createPannal();

	// ���� ���� �Լ�
	void createJudge();

	// �޺� ���� �Լ�
	void createCombo();

	// Ű���� �̺�Ʈ ������ ���� �Լ�
	void createKeyboardListener();

	// ��ġ �̺�Ʈ ������ ���� �Լ�
	void createTouchListener();

	// ��ġ ���� �Լ�
	void touch(int _tag, bool _touch);

	// ��ġ �̺�Ʈ �Լ�
	void touchEvent(const std::vector<Touch*>& _touches, bool _isTrue);

	// ���� �Լ�
	void judge(int _tag);

	// ���� ���� �Լ�
	void judgeAction(int _judge, Sprite* _spr);

	// ���� ���� ���� �Լ�
	void setJudgeLine(float* _pannalSize, float* _theSmall, float* _theBig, void* _temp);

	// ���� ��ȯ �Լ�
	int getJudge(float _pannalSize, float _theSmall, float _theBig);

	// ���� ���� �Լ�
	void setJudge(char* _judge);

	// ���� �׼� ��ȯ �Լ�
	Sequence* createJudgeAction();

	// �޺� ���� �Լ�
	void setCombo(bool _isUp);

	// �޺� �׼� ��ȯ �Լ�
	Sequence* createComboAction(bool _isUp);

	// ���� �׼� ��ȯ �Լ�
	Sequence* createStartAction();

	// Z���� ���
	enum ZOrder
	{
		Z_BACKGROUND,
		Z_TOUCH,
		Z_PANNAL,
		Z_NOTE,
		Z_LABEL
	};

	// ���� ���
	const float BEAT = 1.0f / 16;

	// ���� ���
	enum Judge
	{
		JUDGE_NONE,
		JUDGE_MISS,
		JUDGE_BAD,
		JUDGE_GOOD,
		JUDGE_GREAT,
		JUDGE_PERFECT
	};

public:
	// ������
	GameScene();

	// �Ҹ���
	~GameScene();
};

#endif
