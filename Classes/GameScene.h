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

	// 노트 놓침 콜백 함수
	void missNote(Ref* _sender);

private:
	Size visibleSize;
	Vec2 origin;

	// 곡 이름 변수
	char* name;

	// 터치 변수
	MenuItemSprite* touches[4];

	// 패널 변수
	Sprite* pannals[4];

	// 판정 라벨 변수
	Label* judgeLabel;

	// 콤보 라벨 변수
	Label* comboLabel;

	// 노트 변수
	Note* note;

	// 오디오ID 변수
	int audioID;

	// 시간 변수
	float time;

	// 박자 변수
	int beat;

	// 콤보 변수
	int combo;

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

	// 노트 관리 함수
	void manageNote();

	// 노트 로드 함수
	void loadNote();

	// 배경 생성 함수
	void createBackground();

	// 터치 생성 함수
	void createTouch();

	// 패널 생성 함수
	void createPannal();

	// 판정 생성 함수
	void createJudge();

	// 콤보 생성 함수
	void createCombo();

	// 키보드 이벤트 리스너 생성 함수
	void createKeyboardListener();

	// 터치 이벤트 리스너 생성 함수
	void createTouchListener();

	// 터치 동작 함수
	void touch(int _tag, bool _touch);

	// 터치 이벤트 함수
	void touchEvent(const std::vector<Touch*>& _touches, bool _isTrue);

	// 판정 함수
	void judge(int _tag);

	// 판정 실행 함수
	void judgeAction(int _judge, Sprite* _spr);

	// 판정 라인 설정 함수
	void setJudgeLine(float* _pannalSize, float* _theSmall, float* _theBig, void* _temp);

	// 판정 반환 함수
	int getJudge(float _pannalSize, float _theSmall, float _theBig);

	// 판정 설정 함수
	void setJudge(char* _judge);

	// 판정 액션 반환 함수
	Sequence* createJudgeAction();

	// 콤보 설정 함수
	void setCombo(bool _isUp);

	// 콤보 액션 반환 함수
	Sequence* createComboAction(bool _isUp);

	// 시작 액션 반환 함수
	Sequence* createStartAction();

	// Z오더 상수
	enum ZOrder
	{
		Z_BACKGROUND,
		Z_TOUCH,
		Z_PANNAL,
		Z_NOTE,
		Z_LABEL
	};

	// 박자 상수
	const float BEAT = 1.0f / 16;

	// 판정 상수
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
	// 생성자
	GameScene();

	// 소멸자
	~GameScene();
};

#endif
