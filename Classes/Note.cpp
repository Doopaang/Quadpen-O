#include "Note.h"
#include "GameScene.h"

Note::Note(char* _name) :
	list{ nullptr, nullptr, nullptr, nullptr }
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 리스트 생성
	createList(_name);
}

Note::~Note()
{
	// 리스트 삭제
	deleteList();
}

void Note::createList(char* _name)
{
	// 파일 열기
	FILE* fp = fopen(StringUtils::format("./sng/%s/%s", _name, _name).c_str(), "r");

	for (int count = NULL; count < 4; count++)
	{
		List* tail = NULL;

		int beat = NULL;
		int lane = NULL;
		int kind = NULL;

		// 파일 읽기
		while (fscanf(fp, "%d %d %d", &beat, &lane, &kind) != -1)
		{
			// 같은 레인만 입력
			if (lane != count)
			{
				continue;
			}

			List* temp = new List;

			// 정보 입력
			temp->beat = beat;
			temp->lane = lane;
			temp->kind = kind;
			temp->spr = nullptr;
			temp->next = nullptr;

			// 리스트 연결
			if (!list[count].head)
			{
				// 머리 설정
				list[count].head = temp;

				// 임시 꼬리 설정
				tail = temp;

				// 리스트 시작
				list[count].create = list[count].head;
			}
			else
			{
				// 리스트 연결
				tail->next = temp;

				// 임시 꼬리 설정
				tail = temp;
			}
		}

		// 맨 처음으로 이동
		rewind(fp);
	}

	// 파일 닫기
	fclose(fp);
}

void Note::deleteList()
{
	for (int count = NULL; count < 4; count++)
	{
		while (list[count].head)
		{
			List* temp = nullptr;

			// 머리 다음이 있을 때
			if (list[count].head->next)
			{
				// 정보 입력
				temp = list[count].head->next;

				// 머리 삭제
				delete list[count].head;
				list[count].head = nullptr;

				// 머리 설정
				list[count].head = temp;
			}
			// 머리 뿐일 때
			else
			{
				// 머리 삭제
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

	// 스프라이트 생성
	auto spr = createNoteSprite(_tag, isSub);
	
	// 액션 생성
	auto action = createNoteAction();

	// 액션 실행
	spr->runAction(action);

	// 스프라이트 저장
	list[_tag].create->spr = spr;

	// 화면 출력 시작 리스트 설정
	if (!list[_tag].began)
	{
		list[_tag].began = list[_tag].create;
	}

	// 화면 출력 끝 리스트 설정
	list[_tag].end = list[_tag].create;

	// 다음 생성 리스트 설정
	list[_tag].create = list[_tag].create->next;

	return spr;
}

Sprite* Note::createNoteSprite(int _count, bool _isSub)
{
	Vec2 anchor = Vec2::ZERO;
	Vec2 position = Vec2::ZERO;
	
	// 스프라이트 이름 설정
	char* name = getNoteName(list[_count].create->kind);

	// 스프라이트 위치 설정
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
	// 스프라이트 생성
	auto spr = Sprite::createWithSpriteFrameName("note_single.png");
	spr->setAnchorPoint(Point::ANCHOR_MIDDLE);
	spr->setPosition(getNotePosition(_spr));

	// 애니메이션 생성
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

	// 액션 생성
	auto animate = Animate::create(animation);
	auto remove = RemoveSelf::create();
	auto sequence = Sequence::create(animate, remove, NULL);

	// 액션 실행
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