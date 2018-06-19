#ifndef _GAME_LAYER_
#define _GAME_LAYER_
#include"MagicTower.h"
USING_NS_CC;

class GameMap;
class Hero;

class GameLayer : public Layer {
public:
	GameLayer();
	
	~GameLayer();



	virtual bool init();
	//附加初始化
	void extraInit(int floor);
	//创建GameLayer静态方法
	static GameLayer* createGameLayer(int floor);
	//切换地图
	void switchMap(int floor);
	//默认创建函数
	CREATE_FUNC(GameLayer);
};



#endif
