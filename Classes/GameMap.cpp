#include"GameMap.h"

GameMap* GameMap::createMap(int floor)
{

	char cfloor[2];
	sprintf(cfloor, "%d", floor);
	std::string strFloor(cfloor);

	GameMap* map = new GameMap;
	if (map->initWithTMXFile("tile maps/" + strFloor + ".tmx"))
	{
		//调用额外的init方法
		map->extraInit();

		//将实例放入autorelease池，统一由引擎控制对象的生命周期
		map->autorelease();
		return map;
	}

	CC_SAFE_DELETE(map);
	return NULL;

}

void GameMap::extraInit()
{
	
}
