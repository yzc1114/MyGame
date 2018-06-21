#ifndef _HERO_
#define _HERO_

#include"MagicTower.h"
#include<list>
#include<fstream>
#include<stack>
USING_NS_CC;

class Teleport;
class Enemy;

//�Զ�Ѱ·�� ʹ�õ���ö���� ��������Ƿ�ײ����ĳЩ����,�Ƿ���ִ���,�Ƿ������Զ��ƶ�
typedef enum {
	hitSomethingBad,badThingHappened,heroIsAutoMoving,moveCommondExecuted
}AutoMovingTest;

class Hero : public Node {
public:
	int HP; //Ѫ��
	int ATK; //������
	int DEF; //������
	int YellowKeys; //��Կ��
	int BlueKeys; //��Կ��
	int RedKeys; //��Կ��
	int coins; //���
	bool HavingAxes; //�Ƿ��и���
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
	// ��npc����
	void actWithNPC(); 
	 //����
	void doTeleport(Teleport *teleport);
	//��ܶ�ʱ
	void FightingUpdate(float dt);
	//��ײ��⺯��
	CollisionType checkCollision(Point heroPosition, bool isAutoMoving);
	//�ƶ���ɺ�Ļص�����
	void onMoveDone(Node* pTarget, int data);
	//���Ŵ����Ч�Ķ�ʱ��
	void MusicUpdate(float dt);
	//�Զ�Ѱ·��ĳ�� coco����
	void moveToSomePointAutomatically(Vec2 TileCoord);
	//�Զ�Ѱ·�� ����Ƿ�ɴ�
	bool ifReachable(Vec2 tilecoord);
	//�Զ�Ѱ·�� �ƶ���Ӣ����Χ��ĳһ�� �����κ����弴ֹͣ�ƶ�
	AutoMovingTest moveToPointAroundHeroWhenAutomaticallyMoving(Vec2 point);
	//�Զ�Ѱ·�õĶ�ʱ�� ���������˶�״̬
	void AutoMovingUpdate(float dt);
	
	
	Enemy* enemy;//��ǰ��Եĵ���
	int targetDoorGID; //�ŵ�GID
	Vec2 targetTileCoord; //��ʱ�����ש������
	Vec2 targetPosition;//��ʱ�����cocos���� ���ڴ洢��ͨ����ʱ��Ŀ��λ��
	Sprite *heroSprite; //�洢Ӣ�۵ľ���
	Sprite* FightingSprite; //ս��ͼƬ�ľ���
	bool isHeroMoving = false; // ����Ƿ����ƶ�
	bool isHeroFighting = false; // ����Ƿ��ڴ��
	bool isDoorOpening = false; // ����Ƿ��ڿ���
	bool isTalking = false;; //����Ƿ��ڶԻ�
	bool isCheckingAutomaticallyMovePath = false;//������ڲ���Ѱ·��·��
	bool isAutomaticallyMoving = false;//����Ƿ����Զ�����
	std::stack<Vec2> steps; //���ڴ洢�Զ�����ʱ�Ĳ���
	Vec2 EndPointOfAutoMoving;//���ڴ洢�Զ�����ʱ��Ŀ��λ��
	Vec2 NextPoint;//���ڴ洢�Զ�����ʱ ÿһ����Ŀ��λ��

};


#endif
