#ifndef __NOTE_H__
#define __NOTE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene;

class Note
{
public:
	// �ΰ��� ����
	GameScene* gameScene;

	// ����Ʈ ����ü
	struct List
	{
		int beat;
		int lane;
		int kind;
		Sprite* spr;
		List* next;
	};

	// ����Ʈ ���� ����ü
	struct Lists
	{
		// �Ӹ� ����Ʈ ����
		List* head;

		// ���� ���� ����Ʈ ����
		List* create;

		// ȭ�� ��� ���� ����Ʈ ����
		List* began;

		// ȭ�� ��� �� ����Ʈ ����
		List* end;
	};

	// ����Ʈ ���� ����
	Lists list[4];

	// ��Ʈ ���� üũ �Լ�
	bool checkCreateNote(int _beat, int _count);

	// ��Ʈ ���� �Լ�
	Sprite* createNote(int _tag);

	// ����Ʈ ���� �Լ�
	Sprite* createEffect(int _tag, Sprite* _spr, bool _isHit);

private:
	Size visibleSize;
	Vec2 origin;

	// ����Ʈ ���� �Լ�
	void createList(char* _name);

	// ����Ʈ ���� �Լ�
	void deleteList();

	// ��������Ʈ ���� �Լ�
	Sprite* createNoteSprite(int _count, bool _isSub);

	// ��������Ʈ �̸� ��ȯ �Լ�
	char* getNoteName(int _kind);

	// ��������Ʈ ��ġ ���� �Լ�
	void setNotePosition(Vec2* _anchor, Vec2* _position, int _count, bool _isSub);

	// ��������Ʈ �׼� ���� �Լ�
	Sequence* createNoteAction();

	// ��Ʈ ��ġ ��ȯ �Լ�
	Vec2 getNotePosition(Sprite* _spr);

	// ��Ʈ ���� ���
	enum NoteKind
	{
		NOTE_NONE,
		NOTE_SINGLE,
		NOTE_HEAD,
		NOTE_BODY,
		NOTE_TAIL
	};

	// ����Ʈ ������ ���
	const int EFFECT_FRAME = 4;

public:
	// ������
	Note(char* _name);

	// �Ҹ���
	~Note();
};

#endif
