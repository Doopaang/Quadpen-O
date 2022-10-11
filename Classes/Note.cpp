#include "Note.h"
#include "GameScene.h"

Note::Note(char* _name) :
	list{ nullptr, nullptr, nullptr, nullptr }
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ����Ʈ ����
	createList(_name);
}

Note::~Note()
{
	// ����Ʈ ����
	deleteList();
}

void Note::createList(char* _name)
{
	// ���� ����
	FILE* fp = fopen(StringUtils::format("./sng/%s/%s", _name, _name).c_str(), "r");

	for (int count = NULL; count < 4; count++)
	{
		List* tail = NULL;

		int beat = NULL;
		int lane = NULL;
		int kind = NULL;

		// ���� �б�
		while (fscanf(fp, "%d %d %d", &beat, &lane, &kind) != -1)
		{
			// ���� ���θ� �Է�
			if (lane != count)
			{
				continue;
			}

			List* temp = new List;

			// ���� �Է�
			temp->beat = beat;
			temp->lane = lane;
			temp->kind = kind;
			temp->spr = nullptr;
			temp->next = nullptr;

			// ����Ʈ ����
			if (!list[count].head)
			{
				// �Ӹ� ����
				list[count].head = temp;

				// �ӽ� ���� ����
				tail = temp;

				// ����Ʈ ����
				list[count].create = list[count].head;
			}
			else
			{
				// ����Ʈ ����
				tail->next = temp;

				// �ӽ� ���� ����
				tail = temp;
			}
		}

		// �� ó������ �̵�
		rewind(fp);
	}

	// ���� �ݱ�
	fclose(fp);
}

void Note::deleteList()
{
	for (int count = NULL; count < 4; count++)
	{
		while (list[count].head)
		{
			List* temp = nullptr;

			// �Ӹ� ������ ���� ��
			if (list[count].head->next)
			{
				// ���� �Է�
				temp = list[count].head->next;

				// �Ӹ� ����
				delete list[count].head;
				list[count].head = nullptr;

				// �Ӹ� ����
				list[count].head = temp;
			}
			// �Ӹ� ���� ��
			else
			{
				// �Ӹ� ����
				delete list[count].head;
				list[count].head = nullptr;
			}
		}
	}
}

bool Note::checkCreateNote(int _beat, int _count)
{
	if (list[_count].create)
	{
		return _beat == list[_count].create->beat;
	}
	return false;
}

Sprite* Note::createNote(int _tag)
{
	bool isSub = list[_tag].create->beat % 2 ? true : false;

	// ��������Ʈ ����
	auto spr = createNoteSprite(_tag, isSub);
	
	// �׼� ����
	auto action = createNoteAction();

	// �׼� ����
	spr->runAction(action);

	// ��������Ʈ ����
	list[_tag].create->spr = spr;

	// ȭ�� ��� ���� ����Ʈ ����
	if (!list[_tag].began)
	{
		list[_tag].began = list[_tag].create;
	}

	// ȭ�� ��� �� ����Ʈ ����
	list[_tag].end = list[_tag].create;

	// ���� ���� ����Ʈ ����
	list[_tag].create = list[_tag].create->next;

	return spr;
}

Sprite* Note::createNoteSprite(int _count, bool _isSub)
{
	Vec2 anchor = Vec2::ZERO;
	Vec2 position = Vec2::ZERO;
	
	// ��������Ʈ �̸� ����
	char* name = getNoteName(list[_count].create->kind);

	// ��������Ʈ ��ġ ����
	setNotePosition(&anchor, &position, _count, _isSub);

	auto spr = Sprite::createWithSpriteFrameName(StringUtils::format("note_%s.png", name));
	spr->setAnchorPoint(anchor);
	spr->setPosition(position);
	spr->setTag(_count);
	if (_isSub)
	{
		spr->setRotation(90);
	}

	return spr;
}

char* Note::getNoteName(int _kind)
{
	switch (_kind)
	{
	case NoteKind::NOTE_SINGLE:
		return "single";

	case NoteKind::NOTE_HEAD:
		return "head";

	case NoteKind::NOTE_BODY:
		return "body";

	case NoteKind::NOTE_TAIL:
		return "tail";
	}
	return nullptr;
}

void Note::setNotePosition(Vec2* _anchor, Vec2* _position, int _count, bool _isSub)
{
	if (!_isSub)
	{
		switch (_count)
		{
		case 0:
			*_anchor = Point::ANCHOR_BOTTOM_RIGHT;
			*_position = origin + Vec2(0.0, visibleSize.height * 0.5);
			break;

		case 1:
			*_anchor = Point::ANCHOR_BOTTOM_LEFT;
			*_position = origin + Vec2(visibleSize.width, visibleSize.height * 0.5);
			break;

		case 2:
			*_anchor = Point::ANCHOR_TOP_RIGHT;
			*_position = origin + Vec2(0.0, visibleSize.height * 0.5);
			break;

		case 3:
			*_anchor = Point::ANCHOR_TOP_LEFT;
			*_position = origin + Vec2(visibleSize.width, visibleSize.height * 0.5);
			break;
		}
	}
	else
	{
		switch (_count)
		{
		case 0:
			*_anchor = Point::ANCHOR_TOP_RIGHT;
			*_position = origin + Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5 + visibleSize.width * 0.5);
			break;

		case 1:
			*_anchor = Point::ANCHOR_BOTTOM_RIGHT;
			*_position = origin + Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5 + visibleSize.width * 0.5);
			break;

		case 2:
			*_anchor = Point::ANCHOR_TOP_LEFT;
			*_position = origin + Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5 - visibleSize.width * 0.5);
			break;

		case 3:
			*_anchor = Point::ANCHOR_BOTTOM_LEFT;
			*_position = origin + Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5 - visibleSize.width * 0.5);
			break;
		}
	}
}

Sequence* Note::createNoteAction()
{
	auto move = MoveTo::create(0.8f, origin + visibleSize * 0.5);
	auto wait = DelayTime::create(0.08f);
	auto remove = RemoveSelf::create();
	auto call = CallFuncN::create(CC_CALLBACK_1(GameScene::missNote, gameScene));
	auto sequence = Sequence::create(move, wait, remove, call, NULL);

	return sequence;
}

Sprite* Note::createEffect(int _tag, Sprite* _spr, bool _isHit)
{
	// ��������Ʈ ����
	auto spr = Sprite::createWithSpriteFrameName("note_single.png");
	spr->setAnchorPoint(Point::ANCHOR_MIDDLE);
	spr->setPosition(getNotePosition(_spr));

	// �ִϸ��̼� ����
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.05f);
	if (_isHit)
	{
		for (int count = 1; count < EFFECT_FRAME; count++)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("note_effect_hit_%d.png", count));
			animation->addSpriteFrame(frame);
		}
	}
	else
	{
		for (int count = 1; count < EFFECT_FRAME; count++)
		{
			auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("note_effect_miss_%d.png", count));
			animation->addSpriteFrame(frame);
		}
	}

	// �׼� ����
	auto animate = Animate::create(animation);
	auto remove = RemoveSelf::create();
	auto sequence = Sequence::create(animate, remove, NULL);

	// �׼� ����
	spr->runAction(sequence);

	return spr;
}

Vec2 Note::getNotePosition(Sprite* _spr)
{
	Vec2 position = Vec2::ZERO;
	Size size = _spr->getContentSize() * 0.5;
	int tag = _spr->getTag();

	switch (tag)
	{
	case 0:
		position = Vec2(_spr->getPositionX() - size.width, _spr->getPositionY() + size.height);
		break;

	case 1:
		position = Vec2(_spr->getPositionX() + size.width, _spr->getPositionY() + size.height);
		break;

	case 2:
		position = Vec2(_spr->getPositionX() - size.width, _spr->getPositionY() - size.height);
		break;

	case 3:
		position = Vec2(_spr->getPositionX() + size.width, _spr->getPositionY() - size.height);
		break;
	}

	return position;
}