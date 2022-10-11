#include "MainScene.h"
#include "GameScene.h"
#include "EditScene.h"

MainScene::MainScene() :
	select(M_NONE)
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 파일 불러오기
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./spr/main_sprites.plist");
}

MainScene::~MainScene()
{
	
}

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = MainScene::create();

    scene->addChild(layer);

    return scene;
}

bool MainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// 배경 생성
	createBackground();

	// 메뉴 생성
	createMenu();

	// 메뉴 버튼 생성
	createMenuButton();

	// 키보드 이벤트 리스너 생성
	createKeyboardListener();

    return true;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode _keyCode, Event* _event)
{
	switch (_keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		// 키를 처음 누를 때
		if (select == -1)
		{
			select = MenuKind::M_START;
			return;
		}

		menus[select]->unselected();
		menus[select]->setSelectedIndex(0);

		if (select > MenuKind::M_EDIT)
		{
			select -= 2;
		}
		else if (select == MenuKind::M_EDIT)
		{
			select = MenuKind::M_OPTION;
		}
		else
		{
			select = MenuKind::M_EXIT;
		}
		break;

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		// 키를 처음 누를 때
		if (select == -1)
		{
			select = MenuKind::M_START;
			return;
		}

		menus[select]->unselected();
		menus[select]->setSelectedIndex(0);

		if (select != MenuKind::M_START)
		{
			select--;
		}
		else
		{
			select = MenuKind::M_EXIT;
		}
		break;

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		// 키를 처음 누를 때
		if (select == -1)
		{
			select = MenuKind::M_START;
			return;
		}

		menus[select]->unselected();
		menus[select]->setSelectedIndex(0);

		if (select < MenuKind::M_OPTION)
		{
			select += 2;
		}
		else if (select == MenuKind::M_OPTION)
		{
			select = MenuKind::M_EDIT;
		}
		else
		{
			select = MenuKind::M_START;
		}
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		// 키를 처음 누를 때
		if (select == -1)
		{
			select = MenuKind::M_START;
			return;
		}

		menus[select]->unselected();
		menus[select]->setSelectedIndex(0);

		if (select != MenuKind::M_EXIT)
		{
			select++;
		}
		else
		{
			select = MenuKind::M_START;
		}
		break;

	case EventKeyboard::KeyCode::KEY_Q:
		select = MenuKind::M_START;
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
		select = MenuKind::M_EDIT;
		break;

	case EventKeyboard::KeyCode::KEY_Z:
		select = MenuKind::M_OPTION;
		break;

	case EventKeyboard::KeyCode::KEY_SLASH:
		select = MenuKind::M_EXIT;
		break;

	case EventKeyboard::KeyCode::KEY_SPACE:
	case EventKeyboard::KeyCode::KEY_ENTER:
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		select = MenuKind::M_EXIT;
		break;

	default:
		return;
	}

	menus[select]->selected();
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode _keyCode, Event* _event)
{
	// 실행
	switch (_keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_Q:
	case EventKeyboard::KeyCode::KEY_RIGHT_BRACKET:
	case EventKeyboard::KeyCode::KEY_Z:
	case EventKeyboard::KeyCode::KEY_SLASH:
	case EventKeyboard::KeyCode::KEY_SPACE:
	case EventKeyboard::KeyCode::KEY_ENTER:
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		break;

	default:
		return;
	}

	menus[select]->unselected();
	if (menus[select]->getSelectedIndex())
	{
		menus[select]->setSelectedIndex(0);
	}
	else
	{
		menus[select]->setSelectedIndex(1);
	}
	menuCallback(menus[select]);
}

void MainScene::menuCallback(Ref* _sender)
{
	auto sender = (MenuItemToggle*)_sender;
	int tag = sender->getTag();

	select = tag;

	for (int count = M_START; count < 4; count++)
	{
		// 선택한 메뉴일 때
		if (menus[count] == sender)
		{
			// 선택 안돼있을 때
			if (menus[count]->getSelectedIndex())
			{
				// 위로 올림
				menuLayer[count]->setLocalZOrder(ZOrder::Z_MENU);

				// 메뉴 버튼 설정
				setMenuButton(count);
			}
			// 선택 돼있을 때
			else
			{
				// 종료
				end(sender);
			}
		}
		// 선택 안한 메뉴일 때
		else
		{
			// 아래로 내림
			menuLayer[count]->setLocalZOrder(ZOrder::Z_BACK);

			// 선택 해제
			menus[count]->unselected();
			menus[count]->setSelectedIndex(0);

			// 선택 돼있을 때
			if (!menus[count]->getSelectedIndex())
			{
				int isLeft = count % 2 ? 1 : -1;
				Size size = menuBackground[count]->getContentSize();

				// 액션 중지
				menuBackground[count]->stopAllActions();
				menuItem[count]->stopAllActions();

				// 액션 실행
				menuBackground[count]->runAction(removeMenuAction(count, size));
				menuItem[count]->runAction(removeMenuAction(count, size));
			}
		}
	}
}

void MainScene::end(Ref* _sender)
{
	auto sender = (MenuItemToggle*)_sender;
	int tag = sender->getTag();

	Vector <Node*> &nodes = this->getChildren();

	// 이벤트 리스너 삭제
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->removeAllEventListeners();

	for (int count = M_START; count < 4; count++)
	{
		auto action = ScaleTo::create(1.0, 0.0);
		auto action_1 = EaseBackIn::create(action);
		auto action_2 = CallFunc::create(CC_CALLBACK_0(MainScene::changeScene, this));
		auto action_3 = Sequence::create(action_1, action_2, nullptr);

		// 비활성화
		menus[count]->setEnabled(false);
		menuItem[count]->setEnabled(false);

		// 액션 중지
		menus[count]->stopAllActions();

		// 액션 실행
		menus[count]->runAction(action_3);
	}

	if (select != M_NONE)
	{
		auto action = ScaleTo::create(1.0, 0.0);
		auto action_1 = EaseBackIn::create(action);

		// 액션 중지
		menuBackground[select]->stopAllActions();

		// 액션 실행
		menuBackground[select]->runAction(action_1);
	}

	if (select != M_NONE)
	{
		auto action = ScaleTo::create(1.0, 0.0);
		auto action_1 = EaseBackIn::create(action);

		// 액션 중지
		menuItem[select]->stopAllActions();

		// 액션 실행
		menuItem[select]->runAction(action_1);
	}

	if (select != M_NONE)
	{
		auto action = ScaleTo::create(1.0, 0.0);
		auto action_1 = EaseBackIn::create(action);

		// 액션 중지
		menuLayer[select]->stopAllActions();

		// 액션 실행
		menuLayer[select]->runAction(action_1);
	}
}

void MainScene::createBackground()
{
	auto background = Sprite::create("./img/white_background.png");
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(origin + visibleSize * 0.5);
	this->addChild(background, ZOrder::Z_BACKGROUND);
}

void MainScene::createMenu()
{
	for (int count = M_START; count < 4; count++)
	{
		char* menu;
		Vec2 position;
		switch (count)
		{
		case MenuKind::M_START:
			menu = "start";
			break;

		case MenuKind::M_EDIT:
			menu = "edit";
			break;

		case MenuKind::M_OPTION:
			menu = "option";
			break;

		case MenuKind::M_EXIT:
			menu = "exit";
			break;
		}

		auto normalNormal = Sprite::createWithSpriteFrameName(StringUtils::format("menu_%s_normal_normal.png", menu));
		auto normalSelect = Sprite::createWithSpriteFrameName(StringUtils::format("menu_%s_normal_select.png", menu));
		auto selectNormal = Sprite::createWithSpriteFrameName(StringUtils::format("menu_%s_select_normal.png", menu));
		auto selectSelect = Sprite::createWithSpriteFrameName(StringUtils::format("menu_%s_select_select.png", menu));

		auto itemNormal = MenuItemSprite::create(normalNormal, normalSelect);
		auto itemSelect = MenuItemSprite::create(selectNormal, selectSelect);

		switch (count)
		{
		case MenuKind::M_START:
			position = origin + visibleSize * 0.5 + Vec2(-itemNormal->getContentSize().width, itemNormal->getContentSize().height) * 0.5;
			break;

		case MenuKind::M_EDIT:
			position = origin + visibleSize * 0.5 + Vec2(itemNormal->getContentSize().width, itemNormal->getContentSize().height) * 0.5;
			break;

		case MenuKind::M_OPTION:
			position = origin + visibleSize * 0.5 + Vec2(-itemNormal->getContentSize().width, -itemNormal->getContentSize().height) * 0.5;
			break;

		case MenuKind::M_EXIT:
			position = origin + visibleSize * 0.5 + Vec2(itemNormal->getContentSize().width, -itemNormal->getContentSize().height) * 0.5;
			break;
		}

		auto menuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MainScene::menuCallback, this), itemNormal, itemSelect, nullptr);
		menuItem->setAnchorPoint(Point::ANCHOR_MIDDLE);
		menuItem->setPosition(position);
		menuItem->setTag(count);
		menuItem->setScale(0.0);

		menus[count] = menuItem;

		auto action = ScaleTo::create(1.0, 1.0);
		auto action_1 = EaseBackOut::create(action);

		menus[count]->runAction(action_1);
	}

	auto menu = Menu::create(menus[0], menus[1], menus[2], menus[3], nullptr);
	menu->setAnchorPoint(Point::ZERO);
	menu->setPosition(origin);
	this->addChild(menu, ZOrder::Z_TOP);
}

void MainScene::createMenuButton()
{
	for (int count = M_START; count < 4; count++)
	{
		int isLeft = count % 2 ? 1 : -1;

		// 메뉴 스프라이트 생성
		menuBackground[count] = Sprite::createWithSpriteFrameName("menu_background.png");
		menuBackground[count]->setAnchorPoint(Point::ANCHOR_MIDDLE);
		menuBackground[count]->setTag(count);

		Size size = menuBackground[count]->getContentSize();

		// 메뉴 라벨 생성
		auto menuLabel = Label::createWithTTF(" ", "./fnt/DXHwejun.ttf", 52, size, TextHAlignment::CENTER, TextVAlignment::CENTER);
		menuLabel->setColor(Color3B::WHITE);

		// 메뉴 아이템 생성
		menuItem[count] = MenuItemLabel::create(menuLabel, CC_CALLBACK_1(MainScene::end, this));
		menuItem[count]->setAnchorPoint(Point::ANCHOR_MIDDLE);
		menuItem[count]->setTag(count);

		// 메뉴 생성
		auto menu = Menu::createWithItem(menuItem[count]);
		menu->setPosition(Point::ZERO);

		// 스크롤 뷰 생성
		menuLayer[count] = ScrollView::create(size);
		this->addChild(menuLayer[count], ZOrder::Z_BACKGROUND);

		// 메뉴에 따라 설정
		switch (count)
		{
		case MenuKind::M_START:
			// 라벨 설정
			menuItem[count]->setString("Start");

			// 위치 설정
			menuBackground[count]->setPosition(Vec2(size.width * 1.5, size.height * 0.5));
			menuItem[count]->setPosition(Vec2(size.width * 1.5, size.height * 0.5));
			menuLayer[count]->setPosition(menus[count]->getPosition() + Vec2(isLeft * (menus[count]->getContentSize().width * 0.5 + size.width), size.height * -0.5));

			break;

		case MenuKind::M_EDIT:
			// 라벨 설정
			menuItem[count]->setString("Edit");

			// 위치 설정
			menuBackground[count]->setPosition(Vec2(size.width * -0.5, size.height * 0.5));
			menuItem[count]->setPosition(Vec2(size.width * -0.5, size.height * 0.5));
			menuLayer[count]->setPosition(menus[count]->getPosition() + Vec2(isLeft * menus[count]->getContentSize().width * 0.5, size.height * -0.5));

			break;

		case MenuKind::M_OPTION:
			// 라벨 설정
			menuItem[count]->setString("Option");

			// 위치 설정
			menuBackground[count]->setPosition(Vec2(size.width * 1.5, size.height * 0.5));
			menuItem[count]->setPosition(Vec2(size.width * 1.5, size.height * 0.5));
			menuLayer[count]->setPosition(menus[count]->getPosition() + Vec2(isLeft * (menus[count]->getContentSize().width * 0.5 + size.width), -size.height * 0.5));

			break;

		case MenuKind::M_EXIT:
			// 라벨 설정
			menuItem[count]->setString("Exit");

			// 위치 설정
			menuBackground[count]->setPosition(Vec2(size.width * -0.5, size.height * 0.5));
			menuItem[count]->setPosition(Vec2(size.width * -0.5, size.height * 0.5));
			menuLayer[count]->setPosition(menus[count]->getPosition() + Vec2(isLeft * menus[count]->getContentSize().width * 0.5, -size.height * 0.5));

			break;
		}

		// 메뉴 출력
		menuLayer[count]->addChild(menuBackground[count], ZOrder::Z_BACKGROUND);
		menuLayer[count]->addChild(menu, ZOrder::Z_MENU);
	}
}

void MainScene::createKeyboardListener()
{
	// 이벤트 리스너 생성
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);

	// 이벤트 디스패쳐 적용
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void MainScene::createTouchListener()
{
	// 이벤트 리스너 생성
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = CC_CALLBACK_2(MainScene::onTouchesBegan, this);
	touchListener->onTouchesMoved = CC_CALLBACK_2(MainScene::onTouchesMoved, this);
	touchListener->onTouchesCancelled = CC_CALLBACK_2(MainScene::onTouchesCancelled, this);
	touchListener->onTouchesEnded = CC_CALLBACK_2(MainScene::onTouchesEnded, this);

	// 이벤트 디스패쳐 적용
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainScene::setMenuButton(int _tag)
{
	int isLeft = _tag % 2 ? 1 : -1;
	Size size = menuBackground[_tag]->getContentSize();

	// 액션 정지
	menuBackground[_tag]->stopAllActions();
	menuItem[_tag]->stopAllActions();

	// 액션 실행
	menuBackground[_tag]->runAction(createMenuAction(_tag, size));
	menuItem[_tag]->runAction(createMenuAction(_tag, size));
}

EaseOut* MainScene::createMenuAction(int _tag, Size _size)
{
	MoveTo* action;

	switch (_tag)
	{
	case MenuKind::M_START:
		action = MoveTo::create(1.0, Point::ZERO + Vec2(_size.width * 0.5, _size.height * 0.5));
		break;

	case MenuKind::M_EDIT:
		action = MoveTo::create(1.0, Point::ZERO + Vec2(_size.width * 0.5, _size.height * 0.5));
		break;

	case MenuKind::M_OPTION:
		action = MoveTo::create(1.0, Point::ZERO + Vec2(_size.width * 0.5, _size.height * 0.5));
		break;

	case MenuKind::M_EXIT:
		action = MoveTo::create(1.0, Point::ZERO + Vec2(_size.width * 0.5, _size.height * 0.5));
		break;
	}

	auto action_1 = EaseOut::create(action, 5.0);

	return action_1;
}

EaseIn* MainScene::removeMenuAction(int _tag, Size _size)
{
	MoveTo* action;

	switch (_tag)
	{
	case MenuKind::M_START:
		action = MoveTo::create(0.2, Point::ZERO + Vec2(_size.width * 1.5, _size.height * 0.5));
		break;

	case MenuKind::M_EDIT:
		action = MoveTo::create(0.2, Point::ZERO + Vec2(_size.width * -0.5, _size.height * 0.5));
		break;

	case MenuKind::M_OPTION:
		action = MoveTo::create(0.2, Point::ZERO + Vec2(_size.width * 1.5, _size.height * 0.5));
		break;

	case MenuKind::M_EXIT:
		action = MoveTo::create(0.2, Point::ZERO + Vec2(_size.width * -0.5, _size.height * 0.5));
		break;
	}

	auto action_1 = EaseIn::create(action, 2.0);

	return action_1;
}

void MainScene::changeScene()
{
	switch (select)
	{
	case MenuKind::M_START:
		Director::getInstance()->replaceScene(GameScene::createScene());
		break;

	case MenuKind::M_EDIT:
		Director::getInstance()->replaceScene(EditScene::createScene());
		break;

	case MenuKind::M_OPTION:
		break;

	case MenuKind::M_EXIT:
		exit(0);
		break;
	}
}