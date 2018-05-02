#ifndef _GAMEMAP_
#define _GAMEMAP_

#include"cocos2d.h"
USING_NS_CC;

class GameMap : public TMXTiledMap {
public:

	static GameMap* createMap(int floor);

	void extraInit();

private:

    TMXTiledMap * map;
};

#endif