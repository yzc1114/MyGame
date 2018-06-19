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
	//初始化敌人们
	void initEnemy();
	//初始化所有对象层的对象
	void initObject();
	//敌人动画更新函数
	void updateEnemyAnimation(float time);
	//所有的传送楼梯字典
	Map<int, Teleport*> teleportDict;
	//所有的npc字典
	Map<int, NPC*> npcDict;
	//当前地图各类信息
	TMXTiledMap * map;
	TMXLayer* WallLayer;
	TMXLayer* FloorLayer;
	TMXLayer* DoorLayer;
	TMXLayer* ItemLayer;
	TMXLayer* enemyLayer;
	Vector<Enemy*> enemyArray;
	//在更换地图之后执行的函数
	void afterSwitchMap();
};

#endif