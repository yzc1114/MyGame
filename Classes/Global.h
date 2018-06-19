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
	//��Ϸ������
	GameScene *gameScene;

	//��Ϸ��ͼ��
	GameLayer *gameLayer;

	//����ͼ��
	ControlLayer *controlLayer;

	//��Ϸ��ͼ
	GameMap *gameMap;

	//��Ϸ��ͼ����
	std::map<int, GameMap*> GameMaps;

	//��ʿ
	Hero *hero;

	//������
	ChatRoom* chatRoom;

	//Ŀ���ͼ�Ĳ���
	int currentLevel;

	//��ǰ���������ߵ�¥��
	int highestStorey;

	//������С
	int per = 100;

	//��ʿ���ֵ���ʼλ��
	Point heroSpawnTileCoord;

	std::string fullpath;
	//����Global��ȫ������Ϊ��
	void resetGlobal();
};



#endif

