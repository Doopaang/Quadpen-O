#include "GameScene.h"
#include "Note.h"
#include "AudioEngine.h"

GameScene::GameScene() :
	touches{ nullptr },
	pannals{ nullptr },
	time(NULL),
	beat(NULL),
	combo(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 불러오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./spr/game_sprites.plist");

	// 이름 불러오기
	name = "sample";

	// 노래 로딩
	AudioEngine::preload(StringUtils::format("./sng/%s/%s.mp3", name, name));
}

GameScene::~GameScene()
{
	// 동적 할당 해제
	delete note;
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// 노트 로드
	loadNote();

	// 배경 생성
	createBackground();

	// 터치 생성
	createTouch();

	// 패널 생성
	createPannal();

	// 판정 생성
	createJudge();

	// 콤보 생성
	createCombo();

    return true;
}

void GameScene::update(float _delta)
{
	// 시간 증가
	time += _delta;
	
	// 시간이 박자를 넘을 때
	if (time >= BEAT)
	{
		// 시간 초기화
		time = NULL;

		// 박자 증가
		beat++;

		// 노트 관리
		manageNote();
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event)
{
	switch (_keyCode)
	{
	case EventKeyboard::KeyCode::KEY_Q:
		touch(0, true);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
		touch(1, true);
		break;

	case EventKeyboard::KeyCode::KEY_Z:
		touch(2, true);
		break;

	case EventKeyboard::KeyCode::KEY_SLASH:
		touch(3, true);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event)
{
	switch (_keyCode)
	{
	case EventKeyboard::KeyCode::KEY_Q:
		touch(0, false);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
		touch(1, false);
		break;

	case EventKeyboard::KeyCode::KEY_Z:
		touch(2, false);
		break;

	case EventKeyboard::KeyCode::KEY_SLASH:
		touch(3, false);
		break;
	}
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& _touches, Event* _event)
{
	touchEvent(_touches, true);
}

void GameScene::onTouchesMoved(const std::vector<Touch*>& _touches, Event* _event)
{

}

void GameScene::onTouchesCancelled(const std::vector<Touch*>& _touches, Event* _event)
{
	onTouchesEnded(_touches, _event);
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& _touches, Event* _event)
{
	touchEvent(_touches, false);
}

void GameScene::startCallback()
{
	// 시간 증가
	time++;

	// 액션이 모두 끝났을 때
	if (time >= 8)
	{
		// 시간 초기화
		time = NULL;

		// 키보드 이벤트 리스너 생성
		createKeyboardListener();

		// 터치 이벤트 리스너 생성
		createTouchListener();

		// 업데이트
		this->scheduleUpdate();

		// 노래 재생
		audioID = AudioEngine::play2d(StringUtils::format("./sng/%s/%s.mp3", name, name));
	}
}

void GameScene::missNote(Ref* _sender)
{
	auto sender = (Sprite*)_sender;
	
	// 미스 판정 실행
	judgeAction(JUDGE_MISS, sender);
}

void GameScene::manageNote()
{
	for (int count = NULL; count < 4; count++)
	{
		// 노트 생성 체크
		if (note->checkCreateNote(beat, count))
		{
			// 노트 생성
			auto spr = note->createNote(count);

			// 노트 출력
			this->addChild(spr, ZOrder::Z_NOTE);
		}
	}
}

void GameScene::loadNote()
{
	// 노트 동적 할당
	note = new Note(name);

	// 노트에 인게임 전달
	note->gameScene = this;
}

void GameScene::createBackground()
{
	auto background = Sprite::create("./img/white_background.png");
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(origin + visibleSize * 0.5);
	this->addChild(background, ZOrder::Z_BACKGROUND);
}

void GameScene::createTouch()
{
	for (int count = NULL; count < 4; count++)
	{
		auto normal = Sprite::createWithSpriteFrameName("touch_normal.png");
		auto touched = Sprite::createWithSpriteFrameName("touch_touch.png");

		auto touchItem = MenuItemSprite::create(normal, touched);
		Vec2 anchor;
		Vec2 position;
		switch (count)
		{
		case 0:
			anchor = Point::ANCHOR_TOP_LEFT;
			position = origin + Size(0.0, visibleSize.height);
			break;

		case 1:
			anchor = Point::ANCHOR_TOP_RIGHT;
			position = origin + visibleSize;
			break;

		case 2:
			anchor = Point::ANCHOR_BOTTOM_LEFT;
			position = origin;
			break;

		case 3:
			anchor = Point::ANCHOR_BOTTOM_RIGHT;
			position = origin + Size(visibleSize.width, 0.0);
			break;
		}
		touchItem->setAnchorPoint(anchor);
		touchItem->setPosition(position);
		touchItem->setEnabled(false);
		touchItem->setScale(0.0);

		touches[count] = touchItem;

		auto sequence = createStartAction();

		touches[count]->runAction(sequence);
	}
	
	auto touch = Menu::create(touches[0], touches[1], touches[2], touches[3], nullptr);
	touch->setAnchorPoint(Point::ZERO);
	touch->setPosition(origin);
	this->addChild(touch, ZOrder::Z_TOUCH);
}

void GameScene::createPannal()
{
	for (int count = NULL; count < 4; count++)
	{
		auto normal = Sprite::createWithSpriteFrameName("pannal_normal.png");
		Vec2 anchor;
		switch (count)
		{
		case 0:
			anchor = Point::ANCHOR_BOTTOM_RIGHT;
			break;

		case 1:
			anchor = Point::ANCHOR_BOTTOM_LEFT;
			break;

		case 2:
			anchor = Point::ANCHOR_TOP_RIGHT;
			break;

		case 3:
			anchor = Point::ANCHOR_TOP_LEFT;
			break;
		}
		normal->setAnchorPoint(anchor);
		normal->setPosition(origin + visibleSize * 0.5);
		normal->setScale(0.0);
		this->addChild(normal, ZOrder::Z_PANNAL);

		auto sequence = createStartAction();

		normal->runAction(sequence);

		auto touched = Sprite::createWithSpriteFrameName("pannal_touch.png");
		touched->setAnchorPoint(anchor);
		touched->setPosition(origin + visibleSize * 0.5);
		touched->setVisible(false);
		this->addChild(touched, ZOrder::Z_TOUCH);

		pannals[count] = touched;
	}
}

void GameScene::createJudge()
{
	judgeLabel = Label::createWithTTF("", "./fnt/DXHwejun.ttf", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	judgeLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	judgeLabel->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.2);
	judgeLabel->setColor(Color3B(244, 82, 137));
	judgeLabel->setOpacity(0);
	this->addChild(judgeLabel, ZOrder::Z_LABEL);
}

void GameScene::createCombo()
{
	comboLabel = Label::createWithTTF("", "./fnt/DXHwejun.ttf", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	comboLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
	comboLabel->setPosition(origin.x + visibleSize.width * 0.5, origin.y + visibleSize.height * 0.8);
	comboLabel->setColor(Color3B(244, 82, 137));
	comboLabel->setOpacity(0);
	this->addChild(comboLabel, ZOrder::Z_LABEL);
}

void GameScene::createKeyboardListener()
{
	// 이벤트 리스너 생성
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	// 이벤트 디스패쳐 적용
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::createTouchListener()
{
	// 이벤트 리스너 생성
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	touchListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
	touchListener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
	touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);

	// 이벤트 디스패쳐 적용
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::touch(int _tag, bool _touch)
{
	MenuItemSprite* touch = touches[_tag];
	Sprite* pannal = pannals[_tag];

	if (_touch)
	{
		// 터치 선택
		touch->selected();

		// 패널 선택
		pannal->setVisible(true);

		// 판정
		judge(_tag);
	}
	else
	{
		// 터치 선택 해제
		touch->unselected();

		// 패널 선택 해제
		pannal->setVisible(false);
	}
}

void GameScene::touchEvent(const std::vector<Touch*>& _touches, bool _isTrue)
{
	for (int count = NULL; count < 4; count++)
	{
		// 한 곳이라도 겹친다면
		for (auto iter = _touches.begin(); iter != _touches.end(); iter++)
		{
			Rect rect = touches[count]->getBoundingBox();
			Point point = (*iter)->getLocation();
			if (rect.containsPoint(point))
			{
				touch(count, _isTrue);
			}
		}
	}
}

void GameScene::judge(int _tag)
{
	Note::List* temp = note->list[_tag].began;

	// 노트가 없을 때
	// 스프라이트가 없을 때
	if (!temp ||
		!temp->spr)
	{
		return;
	}

	float pannalSize = NULL;
	float theSmall = NULL;
	float theBig = NULL;

	// 판정선 설정
	setJudgeLine(&pannalSize, &theSmall, &theBig, temp);

	// 판정 받아오기
	int judge = getJudge(pannalSize, theSmall, theBig);

	// 판정이 아닐 때
	if (judge == Judge::JUDGE_NONE)
	{
		return;
	}

	// 판정 실행
	judgeAction(judge, temp->spr);

	// 노트 삭제
	this->removeChild(temp->spr);
	temp->spr = nullptr;
}

void GameScene::judgeAction(int _judge, Sprite* _spr)
{
	int tag = _spr->getTag();

	// 화면 출력 시작 리스트 설정
	note->list[tag].began = note->list[tag].began->next ? note->list[tag].began->next : nullptr;

	Sprite* effect = nullptr;

	// 판정
	switch (_judge)
	{
	case JUDGE_MISS:
		// 판정 설정
		setJudge("MISS");

		// 콤보 설정
		setCombo(false);

		// 이펙트 생성
		effect = note->createEffect(_tag, _spr, false);

		break;

	case JUDGE_BAD:
		// 판정 설정
		setJudge("BAD");

		// 콤보 설정
		setCombo(false);

		// 이펙트 생성
		effect = note->createEffect(_tag, _spr, false);

		break;

	case JUDGE_GOOD:
		// 판정 설정
		setJudge("GOOD");

		// 콤보 설정
		setCombo(true);

		// 이펙트 생성
		effect = note->createEffect(_tag, _spr, true);

		break;

	case JUDGE_GREAT:
		// 판정 설정
		setJudge("GREAT");

		// 콤보 설정
		setCombo(true);

		// 이펙트 생성
		effect = note->createEffect(_tag, _spr, true);

		break;

	case JUDGE_PERFECT:
		// 판정 설정
		setJudge("PERFECT");

		// 콤보 설정
		setCombo(true);

		// 이펙트 생성
		effect = note->createEffect(_tag, _spr, true);

		break;
	}

	// 이펙트 출력
	this->addChild(effect);
}

void GameScene::setJudgeLine(float* _pannalSize, float* _theSmall, float* _theBig, void* _temp)
{
	Note::List* temp = (Note::List*)_temp;
	int tag = temp->lane;

	// 가로 줄일 때
	if (temp->beat % 2 ? false : true)
	{
		*_pannalSize = pannals[tag]->getContentSize().width;

		switch (tag)
		{
			// 왼쪽 레인일 때
		case 0:
		case 2:
			*_theSmall = temp->spr->getPosition().x;
			*_theBig = pannals[tag]->getPosition().x;
			break;

			// 오른쪽 레인일 때
		case 1:
		case 3:
			*_theSmall = pannals[tag]->getPosition().x;
			*_theBig = temp->spr->getPosition().x;
			break;
		}
	}
	// 세로 줄일 때
	else
	{
		*_pannalSize = pannals[tag]->getContentSize().width;

		switch (tag)
		{
			// 위쪽 레인일 때
		case 0:
		case 1:
			*_theSmall = pannals[tag]->getPosition().y;
			*_theBig = temp->spr->getPosition().y;
			break;

			// 아래쪽 레인일 때
		case 2:
		case 3:
			*_theSmall = temp->spr->getPosition().y;
			*_theBig = pannals[tag]->getPosition().y;
			break;
		}
	}
}

int GameScene::getJudge(float _pannalSize, float _theSmall, float _theBig)
{
	// None
	if (_theSmall < _theBig - _pannalSize * 3)
	{
		// 판정
		return JUDGE_NONE;
	}
	// Miss
	else if (_theSmall < _theBig - _pannalSize * 2)
	{
		// 판정
		return JUDGE_MISS;
	}
	// Bad
	else if (_theSmall < _theBig - _pannalSize * 1.5)
	{
		// 판정
		return JUDGE_BAD;
	}
	// Good
	else if (_theSmall < _theBig - _pannalSize * 1)
	{
		// 판정
		return JUDGE_GOOD;
	}
	// Great
	else if (_theSmall < _theBig - _pannalSize * 0.5)
	{
		// 판정
		return JUDGE_GREAT;
	}
	// Perfect
	else
	{
		// 판정
		return JUDGE_PERFECT;
	}
}

void GameScene::setJudge(char* _judge)
{
	// 액션 정지
	judgeLabel->stopAllActions();
	judgeLabel->setScale(1.0);

	// 판정 설정
	judgeLabel->setString(StringUtils::format("%s", _judge));

	// 액션 생성
	auto action = createJudgeAction();

	// 액션 실행
	judgeLabel->runAction(action);
}

Sequence* GameScene::createJudgeAction()
{
	auto fadeIn = FadeIn::create(0.0);
	auto scaleUp = ScaleTo::create(0.1f, 1.3f);
	auto scaleDown = ScaleTo::create(0.1f, 1.0);
	auto wait = DelayTime::create(0.2f);
	auto fadeOut = FadeOut::create(0.1f);
	auto sequence = Sequence::create(fadeIn, scaleUp, scaleDown, wait, fadeOut, NULL);

	return sequence;
}

void GameScene::setCombo(bool _isUp)
{
	// 액션 정지
	comboLabel->stopAllActions();

	Action* action;

	if (_isUp)
	{
		// 액션 생성
		action = createComboAction(_isUp);

		// 콤보 설정
		combo++;
	}
	else if (combo)
	{
		// 액션 생성
		action = createComboAction(_isUp);

		// 콤보 설정
		combo = NULL;
	}
	else
	{
		// 액션 생성
		action = FadeOut::create(0.1f);
	}

	// 액션 실행
	comboLabel->runAction(action);

	// 라벨 설정
	comboLabel->setString(StringUtils::format("%d", combo));
}

Sequence* GameScene::createComboAction(bool _isUp)
{
	if (_isUp)
	{
		auto fadeIn = FadeIn::create(0.0);
		auto scaleUp = ScaleTo::create(0.2f, 1.1f);
		auto scaleDown = ScaleTo::create(0.2f, 1.0);
		auto wait = DelayTime::create(1.0);
		auto fadeOut = FadeOut::create(0.1f);
		auto sequence = Sequence::create(fadeIn, scaleUp, scaleDown, wait, fadeOut, NULL);

		return sequence;
	}
	else
	{
		auto fadeIn = FadeIn::create(0.0);
		auto wait = DelayTime::create(0.5);
		auto fadeOut = FadeOut::create(0.1f);
		auto sequence = Sequence::create(fadeIn, wait, fadeOut, NULL);

		return sequence;
	}
}

Sequence* GameScene::createStartAction()
{
	auto scaleTo = ScaleTo::create(1.0, 1.0);
	auto easeBack = EaseBackOut::create(scaleTo);
	auto callback = CallFunc::create(CC_CALLBACK_0(GameScene::startCallback, this));
	auto sequence = Sequence::create(easeBack, callback, nullptr);

	return sequence;
}