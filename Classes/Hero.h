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
	int YellowKeys; //��Կ��
	int BlueKeys; //��Կ��
	int RedKeys; //��Կ��
	int coins; //���
	CREATE_FUNC(Hero);
	bool init(); //��ʼ��Ӣ��
	Hero();
	~Hero();
	//��ָ�������ƶ�һ��
	void move(HeroDirection direction); 
	//�����沿����
	void setFaceDirection(HeroDirection direction);
	//���
	void fight();
	//�жϵ����Ƿ��ܱ�����
	bool ifTheEnemyCanBeHurt(Enemy* enemy);
	// ����
	void pickUpItem(); 
	// ����
	void openDoor(int targetDoorGID); 
	//����ʱ����ͼ���schedule����
	void DoorOpeningUpdate(float dt);
	//
	void contactMenuCALLBACK(Ref * psender);
	// ��npc����
	void actWithNPC(); 
	 //����
	void doTeleport(Teleport *teleport);
	//��ܶ�ʱ
	void FightingUpdate(float dt);
	//��ײ��⺯��
	CollisionType checkCollision(Point heroPosition);
	//�ƶ���ɺ�Ļص�����
	void onMoveDone(Node* pTarget, int data);
	//���Ŵ����Ч�Ķ�ʱ��
	void MusicUpdate(float dt);
	//��NPC�����˵��Ļص�����
	//void contactMenuCALLBACK(Ref* psender);
//private:
	Enemy* enemy;//��ǰ��Եĵ���
	int targetDoorGID; //�ŵ�GID
	Point targetTileCoord; //��ʱ�����ש������
	Point targetPosition;//��ʱ�����cocos����
	Sprite *heroSprite; //�洢Ӣ�۵ľ���
	Sprite* FightingSprite; //ս��ͼƬ�ľ���
	bool isHeroMoving; // ����Ƿ����ƶ�
	bool isHeroFighting; // ����Ƿ��ڴ��
	bool isDoorOpening; // ����Ƿ��ڿ���
	bool isTalking; //����Ƿ��ڶԻ�
};


#endif
