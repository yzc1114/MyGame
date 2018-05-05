#ifndef _GAME_LAYER_
#define _GAME_LAYER_
#include"cocos2d.h"
#include"GameMap.h"
USING_NS_CC;


class GameLayer : public Layer {
public:
	GameMap * map;

	Hero* hero;

	virtual bool init();

	void extraInit(int floor);

	static GameLayer* createGameLayer(int floor);

	void showTip();

	CREATE_FUNC(GameLayer);

};



#endif
