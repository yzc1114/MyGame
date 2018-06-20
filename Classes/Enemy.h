#ifndef _ENEMY_H_
#define _ENEMY_H_

#include"MagicTower.h"

USING_NS_CC;

class Enemy : public Ref
{
public:
	Enemy(void);
	~Enemy(void);

	Enemy(int gid);

	int HP;
	int ATK;
	int DEF;
	int coinsWorth;

	//������TileMap�ϵķ�λ
	Point position;

	//�����ʼ��ͼ��ID
	int startGID = 0;

	//�������ֵ��е�index
	int index;
};

#endif