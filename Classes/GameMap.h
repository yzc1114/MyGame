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
	//����init
	void extraInit();

	//cocos���굽tiled map����
	static Point tileCoordForPosition(Point position);

	//tiled���굽cocos����
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