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
	map = GameMap::createMap(floor);

	addChild(map, 0);
	map->setPosition(0, 0);

	hero = Hero::create();
	hero->setPosition(GameMap::positionForTileCoord(Point(1, 11)));
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
