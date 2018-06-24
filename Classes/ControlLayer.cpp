#include"ControlLayer.h"
//class ChatRoom;

ControlLayer::ControlLayer()
{
	Global::instance()->controlLayer = this;
}

ControlLayer::~ControlLayer()
{
}

bool ControlLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	//退出按钮
	auto exit = MenuItemImage::create("Close/CloseNormal.png", "Close/CloseSelected.png", CC_CALLBACK_1(ControlLayer::exitButtonCallBack, this));
	exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(exit->getContentSize().width / 2,MAP_SIZE);
	addChild(exitButton, 0);

	//四个方向控制的按钮精灵
	directionControl = Sprite::create("DirectionButtons/btn_normal.png");
	TouchLeft = Sprite::create("DirectionButtons/btn_left.png");
	TouchRight = Sprite::create("DirectionButtons/btn_right.png");
	TouchUp = Sprite::create("DirectionButtons/btn_up.png");
	TouchDown = Sprite::create("DirectionButtons/btn_down.png");
	//把他们挨个放好位置
	directionControl->setPosition(32 * 3, 32 * 3);
	TouchLeft->setPosition(32 * 3, 32 * 3);
	TouchRight->setPosition(32 * 3, 32 * 3);
	TouchUp->setPosition(32 * 3, 32 * 3);
	TouchDown->setPosition(32 * 3, 32 * 3);

	this->addChild(directionControl, normal, normal);
	this->addChild(TouchLeft, left, left);
	this->addChild(TouchRight, right, right);
	this->addChild(TouchUp, up, up);
	this->addChild(TouchDown, down, down);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	
	//鼠标点击方向盘的上下左右四个区域
	auto rect = directionControl->getBoundingBox();
	UpPart = Rect(rect.getMinX() + rect.size.width / 3, rect.getMinY() + 2 * (rect.size.height / 3), rect.size.width / 3, rect.size.height / 3);
	DownPart = Rect(rect.getMinX() + rect.size.width / 3, rect.getMinY(), rect.size.width / 3, rect.size.height / 3);
	LeftPart = Rect(rect.getMinX(), rect.getMinY() + rect.size.height / 3, rect.size.width / 3, rect.size.height / 3);
	RightPart = Rect(rect.getMinX() + 2 * rect.size.width / 3, rect.getMinY() + rect.size.height / 3, rect.size.width / 3, rect.size.height / 3);


    touchListener->onTouchBegan = CC_CALLBACK_2(ControlLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(ControlLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(ControlLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


	auto keyBoardListener = EventListenerKeyboard::create();

	keyBoardListener->onKeyPressed = CC_CALLBACK_2(ControlLayer::onKeyPressed,this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(ControlLayer::onKeyReleased,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);

}



void ControlLayer::exitButtonCallBack(Ref* psender)
{
	Global::instance()->gameMap->removeFromParent();
	Global::instance()->resetGlobal();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
	Director::getInstance()->replaceScene(StartScene::createStartScene());
}

bool ControlLayer::onTouchBegan(Touch * touch, Event * unused_Event)
{
	//如果鼠标点击到对应位置后移开 则不执行移动
	MouseGoesAway = false;

	if (UpPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(up)->setZOrder(100);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
		return true;
	}
	if (DownPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(down)->setZOrder(100);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
		return true;
	}
	if (LeftPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(left)->setZOrder(100);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
		return true;
	}
	if (RightPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(right)->setZOrder(100);
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("music/ButtonBGS.mp3");
		return true;
	}
	
	Rect Help(0, 0, ADDED_WIDTH / 4, 25);
	Rect Save(ADDED_WIDTH / 4,0 , ADDED_WIDTH / 4, 25);
	Rect Chat(ADDED_WIDTH / 2, 0, ADDED_WIDTH / 4, 25);
	if (Help.containsPoint(touch->getStartLocation())) {
		if (!isHelping) {
			Global::instance()->gameScene->addChild(HelpLayer::create(), 200);
		}
	}
	if (Save.containsPoint(touch->getStartLocation())) {
		Global::instance()->gameScene->showSaveLayer();
	}
	if (Chat.containsPoint(touch->getStartLocation())) {
		Director::getInstance()->pushScene(ChatRoom::create());
	}

	return false;
	

}

void ControlLayer::onTouchMoved(Touch * touch, Event * unused_Event)
{
	

	if (UpPart.containsPoint(touch->getStartLocation())) {
		if (!UpPart.containsPoint(touch->getLocation())) {
			getChildByTag(up)->setZOrder(up);
			MouseGoesAway = true;
		}
	}
	if (DownPart.containsPoint(touch->getStartLocation())) {
		if (!DownPart.containsPoint(touch->getLocation())) {
			getChildByTag(down)->setZOrder(down);
			MouseGoesAway = true;
		}
	}
	if (LeftPart.containsPoint(touch->getStartLocation())) {
		if (!LeftPart.containsPoint(touch->getLocation())) {
			getChildByTag(left)->setZOrder(left);
			MouseGoesAway = true;
		}
	}
	if (RightPart.containsPoint(touch->getStartLocation())) {
		if (!RightPart.containsPoint(touch->getLocation())) {
			getChildByTag(right)->setZOrder(right);
			MouseGoesAway = true;
		}
	}

	
}

void ControlLayer::onTouchEnded(Touch * touch, Event * unused_Event)
{
	auto hero = Global::instance()->hero;

	if (UpPart.containsPoint(touch->getStartLocation()) && !MouseGoesAway) {
		hero->move(kup);
		getChildByTag(up)->setZOrder(up);
	}
	if (DownPart.containsPoint(touch->getStartLocation()) && !MouseGoesAway) {
		hero->move(kdown);
		getChildByTag(down)->setZOrder(down);
	}
	if (LeftPart.containsPoint(touch->getStartLocation()) && !MouseGoesAway) {
		hero->move(kleft);
		getChildByTag(left)->setZOrder(left);
	}
	if (RightPart.containsPoint(touch->getStartLocation()) && !MouseGoesAway) {
		hero->move(kright);
		getChildByTag(right)->setZOrder(right);
	}
}

void ControlLayer::onKeyPressed(EventKeyboard::KeyCode keycode, Event * unused_event)
{
	//行走的按键函数
	if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		schedule(schedule_selector(ControlLayer::moveUpUpdate));
	}
	if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		schedule(schedule_selector(ControlLayer::moveDownUpdate));
	}
	if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		schedule(schedule_selector(ControlLayer::moveLeftUpdate));
	}
	if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		schedule(schedule_selector(ControlLayer::moveRightUpdate));
	}
	//按下S进入存档界面
	if (keycode == EventKeyboard::KeyCode::KEY_S) {
		Global::instance()->gameScene->showSaveLayer();
	}
	//按下H进入帮助界面
	if (keycode == EventKeyboard::KeyCode::KEY_H) {
		if(!isHelping) {
			Global::instance()->gameScene->addChild(HelpLayer::create(), 200);
		}
		
	}
	if (keycode == EventKeyboard::KeyCode::KEY_C) {
		Director::getInstance()->pushScene(ChatRoom::create());
	}
	//快速上下楼 在楼梯旁边站着时 按PageUp和PageDown上下楼 只能去已经去过的楼层
	if (keycode == EventKeyboard::KeyCode::KEY_PG_UP) { 
		auto dict = Global::instance()->gameMap->teleportDict;
		for (auto teleport : dict) {
			Vec2 hero = GameMap::tileCoordForPosition(Global::instance()->hero->getPosition());
			Vec2 stair = teleport.second->tileCoord;
			if (abs((hero.getDistance(stair) - 1.0f)) <= 1e-6) {
				for (auto teleport : dict) {
					if (teleport.second->targetMap > Global::instance()->currentLevel && teleport.second->targetMap <= Global::instance()->highestStorey) {
						Global::instance()->hero->doTeleport(teleport.second);
					}
				}
				
			}
				
		}
	}
	if (keycode == EventKeyboard::KeyCode::KEY_F11) {
		auto dict = Global::instance()->gameMap->teleportDict;
		for (auto teleport : dict) {
			Vec2 hero = GameMap::tileCoordForPosition(Global::instance()->hero->getPosition());
			Vec2 stair = teleport.second->tileCoord;
			if (abs((hero.getDistance(stair) - 1.0f)) <= 1e-6) {
				for (auto teleport : dict) {
					if (teleport.second->targetMap > Global::instance()->currentLevel) {
						Global::instance()->hero->doTeleport(teleport.second);
					}
				}

			}

		}
	}
	if (keycode == EventKeyboard::KeyCode::KEY_PG_DOWN) {
		auto dict = Global::instance()->gameMap->teleportDict;
		for (auto teleport : dict) {
			Vec2 hero = GameMap::tileCoordForPosition(Global::instance()->hero->getPosition());
			Vec2 stair = teleport.second->tileCoord;
			if (abs((hero.getDistance(stair) - 1.0f)) <= 1e-6) {
				for (auto teleport : dict) {
					if (teleport.second->targetMap < Global::instance()->currentLevel && teleport.second->targetMap >= 0) {
						Global::instance()->hero->doTeleport(teleport.second);
					}
				}
			}	
		}
	}
	if (keycode == EventKeyboard::KeyCode::KEY_F12) {
		auto dict = Global::instance()->gameMap->teleportDict;
		for (auto teleport : dict) {
			Vec2 hero = GameMap::tileCoordForPosition(Global::instance()->hero->getPosition());
			Vec2 stair = teleport.second->tileCoord;
			if (abs((hero.getDistance(stair) - 1.0f)) <= 1e-6) {
				for (auto teleport : dict) {
					if (teleport.second->targetMap < Global::instance()->currentLevel && teleport.second->targetMap >= 0) {
						Global::instance()->hero->doTeleport(teleport.second);
					}
				}
			}
		}
	}
}

void ControlLayer::onKeyReleased(EventKeyboard::KeyCode keycode, Event * unused_event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		unschedule(schedule_selector(ControlLayer::moveUpUpdate));
	}
	if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		unschedule(schedule_selector(ControlLayer::moveDownUpdate));
	}
	if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		unschedule(schedule_selector(ControlLayer::moveLeftUpdate));
	}
	if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		unschedule(schedule_selector(ControlLayer::moveRightUpdate));
	}
}

void ControlLayer::moveUpUpdate(float dt)
{
	Global::instance()->hero->move(kup);
}

void ControlLayer::moveDownUpdate(float dt)
{
	Global::instance()->hero->move(kdown);
}

void ControlLayer::moveLeftUpdate(float dt)
{
	Global::instance()->hero->move(kleft);
}

void ControlLayer::moveRightUpdate(float dt)
{
	Global::instance()->hero->move(kright);
}
