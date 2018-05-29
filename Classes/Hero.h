#ifndef _HERO_
#define _HERO_

#include"MagicTower.h"
USING_NS_CC;

class Teleport;
class Enemy;
class Hero : public Node {
	

public:
	int HP; //Ѫ��

	int ATK; //������

	int DEF; //������

	int YellowKeys;

	int BlueKeys;

	int RedKeys;

	int coins; //���



	bool init(); //��ʼ��Ӣ��

	CREATE_FUNC(Hero);

	Hero();

	~Hero();

	void move(HeroDirection direction); //��ָ�������ƶ�һ��

	void setFaceDirection(HeroDirection direction);//�����沿����

	void fight();//���

	bool ifTheEnemyCanBeHurt(Enemy* enemy);//�жϵ����Ƿ��ܱ�����

	Enemy* enemy;//��ǰ��Եĵ���

	bool isHeroMoving; // ����Ƿ����ƶ�

	bool isHeroFighting; // ����Ƿ��ڴ��

	bool isDoorOpening; // ����Ƿ��ڿ���

	bool isTalking; //����Ƿ��ڶԻ�

	void pickUpItem(); // ����

	void openDoor(int targetDoorGID); // ����

	void DoorOpeningUpdate(float dt); //����ʱ����ͼ���schedule����

	void contactMenuCALLBACK(Ref * psender);

	void actWithNPC(); // ��npc����

	void doTeleport(Teleport *teleport); //����

	Sprite *heroSprite; //�洢Ӣ�۵ľ���

	Sprite* FightingSprite; //ս��ͼƬ�ľ���

	void FightingUpdate(float dt);//��ܶ�ʱ��

	Point targetTileCoord; //��ʱ�����ש������

	Point targetPosition;//��ʱ�����cocos����

	int targetDoorGID; //�ŵ�GID
	
	CollisionType checkCollision(Point heroPosition); //��ײ��⺯��

	void onMoveDone(Node* pTarget, int data);//�ƶ���ɺ�Ļص�����

	void MusicUpdate(float dt);//���Ŵ����Ч�Ķ�ʱ��

	//void contactMenuCALLBACK(Ref* psender);//��NPC�����˵��Ļص�����

};


#endif
