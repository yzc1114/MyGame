#include "Global.h"

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
{
	gameScene = NULL;

	gameLayer = NULL;

	controlLayer = NULL;

	gameMap = NULL;

	hero = NULL;

	chatRoom = NULL;

	//新游戏，当前地图层数为0
	currentLevel = 0;

	//勇士出生位置 初始化为第0层的出生位置
	heroSpawnTileCoord = Point(1, 11);

	highestStorey = 0;

	fullpath = "";
}

Global::~Global(void)
{

}

