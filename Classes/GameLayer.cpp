#include"GameLayer.h"

GameLayer::GameLayer()
{
	Global::instance()->gameLayer = this;
}

GameLayer::~GameLayer()
{
}

bool GameLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	return true;
}

void GameLayer::extraInit(int floor)
{
	//若GameMaps字典中有地图 则应把所有存储的地图释放并清空字典
	if (!Global::instance()->GameMaps.empty()) {
		for (auto pair : Global::instance()->GameMaps) {
			while (pair.second->getReferenceCount() > 1) {
				pair.second->release();
			}
			pair.second->release();
		}
		Global::instance()->GameMaps.clear();
	}
	//创建新地图
	GameMap* map = GameMap::createMap(floor);
	this->addChild(map, kZmap, kZmap);
	map->setPosition(0, 0);
	//创建新英雄
	Hero* hero = Hero::create();
	hero->setPosition(GameMap::positionForTileCoord(Global::instance()->heroSpawnTileCoord));
	hero->setZOrder(kZhero);
	this->addChild(hero);
	//自动行走的触发函数 我写在这是因为我一开始以为鼠标获取坐标跟它点击的节点有关 后来我发现不管点在哪里我获得的坐标都是绝对的 但是我也懒得改了
	auto touchListenerForAutoWalking = EventListenerTouchOneByOne::create();
	touchListenerForAutoWalking->onTouchBegan = [&](Touch* touch, Event* eve) {
		Vec2 tilecoord = GameMap::tileCoordForPosition(Vec2(touch->getStartLocation().x - STATUSBARWIDTH , touch->getStartLocation().y - TIPBARHEIGHT - 32));
		Global::instance()->hero->moveToSomePointAutomatically(tilecoord);
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenerForAutoWalking, this);
}

GameLayer* GameLayer::createGameLayer(int floor)
{
	GameLayer* layer = GameLayer::create();
	layer->extraInit(floor);
	return layer;
}


//根据地图层数切换地图
void GameLayer::switchMap(int floor)
{
	if (floor > Global::instance()->highestStorey) {
		Global::instance()->highestStorey = floor;
	}
	//将目前地图子节点从gameLayer中删除
    GameMap* gameMap = Global::instance()->gameMap;
	this->removeChildByTag(kZmap);
	//创建新地图 如果已经创建过 则会获得已创建好的地图
	gameMap = GameMap::createMap(floor);
	this->addChild(gameMap, kZmap, kZmap);
	//将global参数更新
	Global::instance()->gameMap = gameMap;
	Global::instance()->hero->setPosition(GameMap::positionForTileCoord(Global::instance()->heroSpawnTileCoord));
	Global::instance()->hero->setFaceDirection(kdown);
	//使hero运行一个动作 使得刚切换地图时的人物在0.5秒内无法移动
	auto action = Sequence::create(CallFunc::create([&]() {
										Global::instance()->hero->isHeroMoving = true;
									}),
									DelayTime::create(0.2f),
								   CallFunc::create([&]() {
										Global::instance()->hero->isHeroMoving = false;
									}),
									NULL
	);
	Global::instance()->hero->runAction(action);
	//设置global参数 刷新状态栏
	Global::instance()->currentLevel = floor;
	Global::instance()->gameScene->refreshStatus(kZcurrentlevel);
}
