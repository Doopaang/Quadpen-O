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

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./spr/game_sprites.plist");

	// �̸� �ҷ�����
	name = "sample";

	// �뷡 �ε�
	AudioEngine::preload(StringUtils::format("./sng/%s/%s.mp3", name, name));
}

GameScene::~GameScene()
{
	// ���� �Ҵ� ����
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

	// ��Ʈ �ε�
	loadNote();

	// ��� ����
	createBackground();

	// ��ġ ����
	createTouch();

	// �г� ����
	createPannal();

	// ���� ����
	createJudge();

	// �޺� ����
	createCombo();

    return true;
}

void GameScene::update(float _delta)
{
	// �ð� ����
	time += _delta;
	
	// �ð��� ���ڸ� ���� ��
	if (time >= BEAT)
	{
		// �ð� �ʱ�ȭ
		time = NULL;

		// ���� ����
		beat++;

		// ��Ʈ ����
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
	// �ð� ����
	time++;

	// �׼��� ��� ������ ��
	if (time >= 8)
	{
		// �ð� �ʱ�ȭ
		time = NULL;

		// Ű���� �̺�Ʈ ������ ����
		createKeyboardListener();

		// ��ġ �̺�Ʈ ������ ����
		createTouchListener();

		// ������Ʈ
		this->scheduleUpdate();

		// �뷡 ���
		audioID = AudioEngine::play2d(StringUtils::format("./sng/%s/%s.mp3", name, name));
	}
}

void GameScene::missNote(Ref* _sender)
{
	auto sender = (Sprite*)_sender;
	
	// �̽� ���� ����
	judgeAction(JUDGE_MISS, sender);
}

void GameScene::manageNote()
{
	for (int count = NULL; count < 4; count++)
	{
		// ��Ʈ ���� üũ
		if (note->checkCreateNote(beat, count))
		{
			// ��Ʈ ����
			auto spr = note->createNote(count);

			// ��Ʈ ���
			this->addChild(spr, ZOrder::Z_NOTE);
		}
	}
}

void GameScene::loadNote()
{
	// ��Ʈ ���� �Ҵ�
	note = new Note(name);

	// ��Ʈ�� �ΰ��� ����
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
	// �̺�Ʈ ������ ����
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	// �̺�Ʈ ������ ����
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::createTouchListener()
{
	// �̺�Ʈ ������ ����
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
	touchListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
	touchListener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesCancelled, this);
	touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);

	// �̺�Ʈ ������ ����
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::touch(int _tag, bool _touch)
{
	MenuItemSprite* touch = touches[_tag];
	Sprite* pannal = pannals[_tag];

	if (_touch)
	{
		// ��ġ ����
		touch->selected();

		// �г� ����
		pannal->setVisible(true);

		// ����
		judge(_tag);
	}
	else
	{
		// ��ġ ���� ����
		touch->unselected();

		// �г� ���� ����
		pannal->setVisible(false);
	}
}

void GameScene::touchEvent(const std::vector<Touch*>& _touches, bool _isTrue)
{
	for (int count = NULL; count < 4; count++)
	{
		// �� ���̶� ��ģ�ٸ�
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

	// ��Ʈ�� ���� ��
	// ��������Ʈ�� ���� ��
	if (!temp ||
		!temp->spr)
	{
		return;
	}

	float pannalSize = NULL;
	float theSmall = NULL;
	float theBig = NULL;

	// ������ ����
	setJudgeLine(&pannalSize, &theSmall, &theBig, temp);

	// ���� �޾ƿ���
	int judge = getJudge(pannalSize, theSmall, theBig);

	// ������ �ƴ� ��
	if (judge == Judge::JUDGE_NONE)
	{
		return;
	}

	// ���� ����
	judgeAction(judge, temp->spr);

	// ��Ʈ ����
	this->removeChild(temp->spr);
	temp->spr = nullptr;
}

void GameScene::judgeAction(int _judge, Sprite* _spr)
{
	int tag = _spr->getTag();

	// ȭ�� ��� ���� ����Ʈ ����
	note->list[tag].began = note->list[tag].began->next ? note->list[tag].began->next : nullptr;

	Sprite* effect = nullptr;

	// ����
	switch (_judge)
	{
	case JUDGE_MISS:
		// ���� ����
		setJudge("MISS");

		// �޺� ����
		setCombo(false);

		// ����Ʈ ����
		effect = note->createEffect(_tag, _spr, false);

		break;

	case JUDGE_BAD:
		// ���� ����
		setJudge("BAD");

		// �޺� ����
		setCombo(false);

		// ����Ʈ ����
		effect = note->createEffect(_tag, _spr, false);

		break;

	case JUDGE_GOOD:
		// ���� ����
		setJudge("GOOD");

		// �޺� ����
		setCombo(true);

		// ����Ʈ ����
		effect = note->createEffect(_tag, _spr, true);

		break;

	case JUDGE_GREAT:
		// ���� ����
		setJudge("GREAT");

		// �޺� ����
		setCombo(true);

		// ����Ʈ ����
		effect = note->createEffect(_tag, _spr, true);

		break;

	case JUDGE_PERFECT:
		// ���� ����
		setJudge("PERFECT");

		// �޺� ����
		setCombo(true);

		// ����Ʈ ����
		effect = note->createEffect(_tag, _spr, true);

		break;
	}

	// ����Ʈ ���
	this->addChild(effect);
}

void GameScene::setJudgeLine(float* _pannalSize, float* _theSmall, float* _theBig, void* _temp)
{
	Note::List* temp = (Note::List*)_temp;
	int tag = temp->lane;

	// ���� ���� ��
	if (temp->beat % 2 ? false : true)
	{
		*_pannalSize = pannals[tag]->getContentSize().width;

		switch (tag)
		{
			// ���� ������ ��
		case 0:
		case 2:
			*_theSmall = temp->spr->getPosition().x;
			*_theBig = pannals[tag]->getPosition().x;
			break;

			// ������ ������ ��
		case 1:
		case 3:
			*_theSmall = pannals[tag]->getPosition().x;
			*_theBig = temp->spr->getPosition().x;
			break;
		}
	}
	// ���� ���� ��
	else
	{
		*_pannalSize = pannals[tag]->getContentSize().width;

		switch (tag)
		{
			// ���� ������ ��
		case 0:
		case 1:
			*_theSmall = pannals[tag]->getPosition().y;
			*_theBig = temp->spr->getPosition().y;
			break;

			// �Ʒ��� ������ ��
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
		// ����
		return JUDGE_NONE;
	}
	// Miss
	else if (_theSmall < _theBig - _pannalSize * 2)
	{
		// ����
		return JUDGE_MISS;
	}
	// Bad
	else if (_theSmall < _theBig - _pannalSize * 1.5)
	{
		// ����
		return JUDGE_BAD;
	}
	// Good
	else if (_theSmall < _theBig - _pannalSize * 1)
	{
		// ����
		return JUDGE_GOOD;
	}
	// Great
	else if (_theSmall < _theBig - _pannalSize * 0.5)
	{
		// ����
		return JUDGE_GREAT;
	}
	// Perfect
	else
	{
		// ����
		return JUDGE_PERFECT;
	}
}

void GameScene::setJudge(char* _judge)
{
	// �׼� ����
	judgeLabel->stopAllActions();
	judgeLabel->setScale(1.0);

	// ���� ����
	judgeLabel->setString(StringUtils::format("%s", _judge));

	// �׼� ����
	auto action = createJudgeAction();

	// �׼� ����
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
	// �׼� ����
	comboLabel->stopAllActions();

	Action* action;

	if (_isUp)
	{
		// �׼� ����
		action = createComboAction(_isUp);

		// �޺� ����
		combo++;
	}
	else if (combo)
	{
		// �׼� ����
		action = createComboAction(_isUp);

		// �޺� ����
		combo = NULL;
	}
	else
	{
		// �׼� ����
		action = FadeOut::create(0.1f);
	}

	// �׼� ����
	comboLabel->runAction(action);

	// �� ����
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