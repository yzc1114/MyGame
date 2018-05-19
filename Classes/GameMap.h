#ifndef _GAMEMAP_
#define _GAMEMAP_

#include"MagicTower.h"
class NPC;
class Teleport;
class Enemy;

USING_NS_CC;

class GameMap : public TMXTiledMap {
public:
	GameMap();
	~GameMap();

	static GameMap* createMap(int floor);
	static GameMap* createNewMap(int floor);
	//附加init
	void extraInit();

	//cocos坐标到tiled map坐标
	static Point tileCoordForPosition(Point position);

	//tiled坐标到cocos坐标
	static Point positionForTileCoord(Point tileCoord);

	void initEnemy();

	void initObject();

	void updateEnemyAnimation(float time);

	Map<int, Teleport*> teleportDict;

	Map<int, NPC*> npcDict;

	TMXTiledMap * map;
	TMXLayer* WallLayer;
	TMXLayer* FloorLayer;
	TMXLayer* DoorLayer;
	TMXLayer* ItemLayer;
	TMXLayer* enemyLayer;
	Vector<Enemy*> enemyArray;

	void afterSwitchMap();
};

#endif