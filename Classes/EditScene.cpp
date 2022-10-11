#include "EditScene.h"
#include "AudioEngine.h"

EditScene::EditScene() :
	touches{ nullptr },
	time(NULL)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// ���� �ҷ�����
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./spr/edit_sprites.plist");
}

EditScene::~EditScene()
{

}

Scene* EditScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = EditScene::create();

    scene->addChild(layer);

    return scene;
}

bool EditScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// ��� ����
	createBackground();

	// ��ġ ����
	createTouch();

	// �г� ����
	createPannal();

    return true;
}

void EditScene::update(float _delta)
{
	
}

void EditScene::onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event)
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

void EditScene::onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event)
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

void EditScene::onTouchesBegan(const std::vector<Touch*>& _touches, Event* _event)
{
	touchEvent(_touches, true);
}

void EditScene::onTouchesMoved(const std::vector<Touch*>& _touches, Event* _event)
{

}

void EditScene::onTouchesCancelled(const std::vector<Touch*>& _touches, Event* _event)
{
	onTouchesEnded(_touches, _event);
}

void EditScene::onTouchesEnded(const std::vector<Touch*>& _touches, Event* _event)
{
	touchEvent(_touches, false);
}

void EditScene::startCallback()
{
	// �ð� ����
	time++;

	// �׼��� ��� ������ ��
	if (time >= 4)
	{
		// �ð� �ʱ�ȭ
		time = NULL;

		// Ű���� �̺�Ʈ ������ ����
		createKeyboardListener();

		// ��ġ �̺�Ʈ ������ ����
		createTouchListener();

		// ������Ʈ
		this->scheduleUpdate();
	}
}

void EditScene::createBackground()
{
	auto background = Sprite::create("./img/white_background.png");
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(origin + visibleSize * 0.5);
	this->addChild(background, ZOrder::Z_BACKGROUND);
}

void EditScene::createTouch()
{
	for (int count = NULL; count < 8; count++)
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

void EditScene::createPannal()
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

void EditScene::createKeyboardListener()
{
	// �̺�Ʈ ������ ����
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(EditScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(EditScene::onKeyReleased, this);

	// �̺�Ʈ ������ ����
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void EditScene::createTouchListener()
{
	// �̺�Ʈ ������ ����
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = CC_CALLBACK_2(EditScene::onTouchesBegan, this);
	touchListener->onTouchesMoved = CC_CALLBACK_2(EditScene::onTouchesMoved, this);
	touchListener->onTouchesCancelled = CC_CALLBACK_2(EditScene::onTouchesCancelled, this);
	touchListener->onTouchesEnded = CC_CALLBACK_2(EditScene::onTouchesEnded, this);

	// �̺�Ʈ ������ ����
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

Sequence* EditScene::createStartAction()
{
	auto scaleTo = ScaleTo::create(1.0, 1.0);
	auto easeBack = EaseBackOut::create(scaleTo);
	auto callback = CallFunc::create(CC_CALLBACK_0(EditScene::startCallback, this));
	auto sequence = Sequence::create(easeBack, callback, nullptr);

	return sequence;
}

void EditScene::touch(int _tag, bool _touch)
{
	MenuItemSprite* touch = touches[_tag];
	Sprite* pannal = pannals[_tag];

	if (_touch)
	{
		// ��ġ ����
		touch->selected();

		// �г� ����
		pannal->setVisible(true);
	}
	else
	{
		// ��ġ ���� ����
		touch->unselected();

		// �г� ���� ����
		pannal->setVisible(false);
	}
}

void EditScene::touchEvent(const std::vector<Touch*>& _touches, bool _isTrue)
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
