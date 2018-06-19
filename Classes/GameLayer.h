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
	//���ӳ�ʼ��
	void extraInit(int floor);
	//����GameLayer��̬����
	static GameLayer* createGameLayer(int floor);
	//�л���ͼ
	void switchMap(int floor);
	//Ĭ�ϴ�������
	CREATE_FUNC(GameLayer);
};



#endif
