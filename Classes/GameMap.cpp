#include"GameMap.h"

GameMap::GameMap()
{
	Global::instance()->gameMap = this;
}

GameMap::~GameMap()
{
}

GameMap* GameMap::createMap(int floor)
{

	char cfloor[2];
	sprintf(cfloor, "%d", floor);
	std::string strFloor(cfloor);

	GameMap* map = new GameMap;
	if (map->initWithTMXFile("tile maps/" + strFloor + ".tmx"))
	{
		map->extraInit();
		map->autorelease();
		return map;
	}

	CC_SAFE_DELETE(map);
	return NULL;

}

void GameMap::extraInit()
{
	
}


Point GameMap::tileCoordForPosition(Point position)
{
	int x = position.x / this->getTileSize().width;
	int y = (((this->getMapSize().height - 1) * this->getTileSize().height) - position.y) / this->getTileSize().height;
	return Point(x, y);
}


Point GameMap::positionForTileCoord(Point tileCoord)
{
	Point pos = Point((tileCoord.x * this->getTileSize().width), ((this->getMapSize().height - tileCoord.y - 1) * this->getTileSize().height));
	return pos;
}