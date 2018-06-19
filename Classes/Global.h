#ifndef _GLOBAL_H_
#define _GLOBAL_H_


#include"MagicTower.h"
USING_NS_CC;

class GameScene;
class GameLayer;
class ChatRoom;
class ControlLayer;
class GameMap;
class Hero;
class chat_client;


class Global : public Singleton<Global>
{
public:
	Global(void);
	~Global(void);
	//游戏主场景
	GameScene *gameScene;

	//游戏主图层
	GameLayer *gameLayer;

	//控制图层
	ControlLayer *controlLayer;

	//游戏地图
	GameMap *gameMap;

	//游戏地图数组
	std::map<int, GameMap*> GameMaps;

	//勇士
	Hero *hero;

	//聊天室
	ChatRoom* chatRoom;

	//目标地图的层数
	int currentLevel;

	//当前到达过的最高的楼层
	int highestStorey;

	//音量大小
	int per = 100;

	//勇士出现的起始位置
	Point heroSpawnTileCoord;

	std::string fullpath;
	//重置Global的全部变量为空
	void resetGlobal();
};



#endif

