#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene;

class Note
{
public:
	// 인게임 변수
	GameScene* gameScene;

	// 리스트 구조체
	struct List
	{
		int beat;
		int lane;
		int kind;
		Sprite* spr;
		List* next;
	};

	// 리스트 관련 구조체
	struct Lists
	{
		// 머리 리스트 변수
		List* head;

		// 다음 생성 리스트 변수
		List* create;

		// 화면 출력 시작 리스트 변수
		List* began;

		// 화면 출력 끝 리스트 변수
		List* end;
	};

	// 리스트 관련 변수
	Lists list[4];

	// 노트 생성 체크 함수
	bool checkCreateNote(int _beat, int _count);

	// 노트 생성 함수
	Sprite* createNote(int _tag);

	// 이펙트 생성 함수
	Sprite* createEffect(int _tag, Sprite* _spr, bool _isHit);

private:
	Size visibleSize;
	Vec2 origin;

	// 리스트 생성 함수
	void createList(char* _name);

	// 리스트 삭제 함수
	void deleteList();

	// 스프라이트 생성 함수
	Sprite* createNoteSprite(int _count, bool _isSub);

	// 스프라이트 이름 반환 함수
	char* getNoteName(int _kind);

	// 스프라이트 위치 설정 함수
	void setNotePosition(Vec2* _anchor, Vec2* _position, int _count, bool _isSub);

	// 스프라이트 액션 생성 함수
	Sequence* createNoteAction();

	// 노트 위치 반환 함수
	Vec2 getNotePosition(Sprite* _spr);

	// 노트 종류 상수
	enum NoteKind
	{
		NOTE_NONE,
		NOTE_SINGLE,
		NOTE_HEAD,
		NOTE_BODY,
		NOTE_TAIL
	};

	// 이펙트 프레임 상수
	const int EFFECT_FRAME = 4;

public:
	// 생성자
	Note(char* _name);

	// 소멸자
	~Note();
};

#endif
