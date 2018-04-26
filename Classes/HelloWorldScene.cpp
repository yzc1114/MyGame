#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "HRocker.h"
USING_NS_CC;
#define MAP_SIZE 1280

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	map = TMXTiledMap::create("tile maps/1.tmx");
	meta = map->layerNamed("meta");
	if (meta) {
		log("ok");
	}
	addChild(map, 0);
	log("%f,%f", map->getPositionX(), map->getPositionY());
	//HRocker* rocker = HRocker::createHRocker("rocker.png", "rockerBG.png", Vec2(100, 100));
	//map->addChild(rocker,10);
	//rocker->startRocker(true);

   

	size = Director::getInstance()->getVisibleSize();
    
	//添加四个方向按钮
	auto pLeft = MenuItemImage::create("left.png", "left.png", this,menu_selector(HelloWorld::left));
	auto pRight = MenuItemImage::create("right.png", "right.png", this, menu_selector(HelloWorld::right));
	auto pUp = MenuItemImage::create("up.png", "up.png", this, menu_selector(HelloWorld::up));
	auto pDown = MenuItemImage::create("down.png", "down.png", this, menu_selector(HelloWorld::down));
	auto left = Menu::create(pLeft, NULL);
	auto right = Menu::create(pRight, NULL);
	auto up = Menu::create(pUp, NULL);
	auto down = Menu::create(pDown, NULL);
	left->setPosition(16 + 32, 16 + 32 * 2);
	right->setPosition(16 + 32 * 3, 16 + 32 * 2);
	up->setPosition(16 + 32 * 2, 16 + 32 * 3);
	down->setPosition(16 + 32 * 2, 16 + 32);
	addChild(left, 10);
	addChild(right, 10);
	addChild(down, 10);
	addChild(up, 10);
    //


	//添加键盘事件
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [=](EventKeyboard::KeyCode keycode, Event* unused_event) {
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: HelloWorld::up(this); this->schedule(schedule_selector(HelloWorld::keyup), 0.1f); break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: HelloWorld::down(this); this->schedule(schedule_selector(HelloWorld::keydown), 0.1f); break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: HelloWorld::left(this); this->schedule(schedule_selector(HelloWorld::keyleft), 0.1f); break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: HelloWorld::right(this); this->schedule(schedule_selector(HelloWorld::keyright), 0.1f); break;
		}
	};
	keyListener->onKeyReleased = [=](EventKeyboard::KeyCode keycode, Event* unused_event) {
		switch (keycode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW: this->unschedule(schedule_selector(HelloWorld::keyup)); break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW: this->unschedule(schedule_selector(HelloWorld::keydown)); break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW: this->unschedule(schedule_selector(HelloWorld::keyleft)); break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW: this->unschedule(schedule_selector(HelloWorld::keyright)); break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	//






	littleman = Sprite::create("contact.png");
	map->addChild(littleman, 9);
	littleman->setPosition(112, 144 + 4 * 32);

    
    return true;
}

void HelloWorld::up(Ref * psender)
{
	float y = littleman->getPositionY();
	if (y + 16 < MAP_SIZE && isCanReach(littleman->getPositionX(),y+32)  ) {
		auto action = MoveBy::create(0.08f, Vec2(0, 32));
		littleman->runAction(action);
		if ((y + map->getPositionY() > size.height / 2) && ((MAP_SIZE - y) > size.height / 2)) {
			auto action = MoveBy::create(0.08f, Vec2(0, -32));
			map->runAction(action);
		}
	}
}

void HelloWorld::left(Ref * psender)
{
	float x = littleman->getPositionX();
	if (x - 16 > 0 && isCanReach(x-32,littleman->getPositionY())  ){
		auto action = MoveBy::create(0.08f, Vec2(-32, 0));
		littleman->runAction(action);
		if ((x + map->getPositionX() < size.width / 2) && map->getPositionX() != 0) {
			auto action = MoveBy::create(0.08f, Vec2(32, 0));
			map->runAction(action);
		}
	}
}

void HelloWorld::down(Ref * psender)
{
	float y = littleman->getPositionY();
	if (y - 16 > 0 && isCanReach(littleman->getPositionX(), y - 32) ) {
		auto action = MoveBy::create(0.08f, Vec2(0, -32));
		littleman->runAction(action);
		if ((y + map->getPositionY() < size.height / 2) && map->getPositionY() != 0) {
			auto action = MoveBy::create(0.08f, Vec2(0, 32));
			map->runAction(action);
		}
	}
}

void HelloWorld::right(Ref * psender)
{
	float x = littleman->getPositionX();
	if (x + 16 < MAP_SIZE && isCanReach(x+32,littleman->getPositionY()) ) {
		auto action = MoveBy::create(0.08f, Vec2(32, 0));
		littleman->runAction(action);
		if ((x + map->getPositionX() > size.width / 2) && ((MAP_SIZE - x) > size.width / 2)) {
			auto action = MoveBy::create(0.08f, Vec2(-32, 0));
			map->runAction(action);
		}
	}
}

void HelloWorld::keyup(float dt)
{
	up(this);
}

void HelloWorld::keydown(float dt)
{
	down(this);
}

void HelloWorld::keyright(float dt)
{
	right(this);
}

void HelloWorld::keyleft(float dt)
{
	left(this);
}

bool HelloWorld::isCanReach(float x,float y)
{
	int mapX = (int)(x - 16) / 32;
	int mapY = (int)(39 - (y - 16) / 32);

	if (meta) {
		int tileGid = meta->tileGIDAt(Vec2(mapX, mapY));
			if (tileGid == 15) {
				return true;
			}
			else {
				return false;
			}
	}
	else {
		log("fuck");
	}
	
}


