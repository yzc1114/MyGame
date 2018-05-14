#include"ControlLayer.h"

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

	auto exit = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(ControlLayer::exitButtonCallBack, this));
	exitButton = Menu::create(exit, NULL);
	exitButton->setPosition(20, 416 - 20);
	addChild(exitButton, 0);


	directionControl = Sprite::create("btn_normal.png");
	TouchLeft = Sprite::create("btn_left.png");
	TouchRight = Sprite::create("btn_right.png");
	TouchUp = Sprite::create("btn_up.png");
	TouchDown = Sprite::create("btn_down.png");

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
	Director::getInstance()->replaceScene(StartScene::createStartScene());
}

bool ControlLayer::onTouchBegan(Touch * touch, Event * unused_Event)
{
	MouseGoesAway = false;

	if (UpPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(up)->setZOrder(100);
		return true;
	}
	if (DownPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(down)->setZOrder(100);		
		return true;
	}
	if (LeftPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(left)->setZOrder(100);		
		return true;
	}
	if (RightPart.containsPoint(touch->getStartLocation())) {
		getChildByTag(right)->setZOrder(100);		
		return true;
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
	if (keycode == EventKeyboard::KeyCode::KEY_S) {
		SaveControl::instance()->save(1);
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
