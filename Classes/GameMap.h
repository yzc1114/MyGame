#ifndef _GAMEMAP_
#define _GAMEMAP_

#include"cocos2d.h"
#include"Global.h"
USING_NS_CC;

class GameMap : public TMXTiledMap {
public:
	GameMap();
	~GameMap();

	static GameMap* createMap(int floor);

	void extraInit();

	Point tileCoordForPosition(Point position);

	Point positionForTileCoord(Point tileCoord);

private:

    TMXTiledMap * map;


};

#endif