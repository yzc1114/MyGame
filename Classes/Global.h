#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#include"cocos2d.h"
#include"Singleton.h"
USING_NS_CC;

class GameScene;
class GameLayer;
class ControlLayer;
class GameMap;
class Hero;

class Global : public Singleton<Global>
{
public:
	Global(void);
	~Global(void);

	GameScene *gameScene;

	//游戏主图层
	GameLayer *gameLayer;

	ControlLayer *controlLayer;

	//游戏地图
	GameMap *gameMap;

	//勇士
	Hero *hero;

	//目标地图的层数
	int currentLevel;

	//勇士出现的起始位置
	Point heroSpawnTileCoord;
};



#endif

