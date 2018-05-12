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
	GameMap* map = GameMap::createMap(floor);

	this->addChild(map, kZmap, kZmap);
	map->setPosition(0, 0);

	Hero* hero = Hero::create();
	hero->setPosition(GameMap::positionForTileCoord(Global::instance()->heroSpawnTileCoord));
	hero->setZOrder(kZhero);
	this->addChild(hero);


}

GameLayer* GameLayer::createGameLayer(int floor)
{
	GameLayer* layer = GameLayer::create();
	layer->extraInit(floor);
	return layer;
}

void GameLayer::showTip()
{

}

void GameLayer::switchMap(int floor)
{

    GameMap* gameMap = Global::instance()->gameMap;
	this->removeChildByTag(kZmap);
	gameMap = GameMap::createMap(floor);
	this->addChild(gameMap, kZmap, kZmap);
	Global::instance()->hero->setPosition(GameMap::positionForTileCoord(Global::instance()->heroSpawnTileCoord));
	Global::instance()->hero->setFaceDirection(kdown);

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
}
