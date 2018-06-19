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
	//��ʼ��������
	void initEnemy();
	//��ʼ�����ж����Ķ���
	void initObject();
	//���˶������º���
	void updateEnemyAnimation(float time);
	//���еĴ���¥���ֵ�
	Map<int, Teleport*> teleportDict;
	//���е�npc�ֵ�
	Map<int, NPC*> npcDict;
	//��ǰ��ͼ������Ϣ
	TMXTiledMap * map;
	TMXLayer* WallLayer;
	TMXLayer* FloorLayer;
	TMXLayer* DoorLayer;
	TMXLayer* ItemLayer;
	TMXLayer* enemyLayer;
	Vector<Enemy*> enemyArray;
	//�ڸ�����ͼ֮��ִ�еĺ���
	void afterSwitchMap();
};

#endif