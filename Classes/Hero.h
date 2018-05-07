#ifndef _HERO_
#define _HERO_

#include"MagicTower.h"
USING_NS_CC;

class Teleport;
class Hero : public Node {
	

public:
	int HP; //血量

	int ATK; //攻击力

	int DEF; //防御力

	int YellowKeys;

	int BlueKeys;

	int RedKeys;



	bool init(); //初始化英雄

	CREATE_FUNC(Hero);

	Hero();

	~Hero();

	void move(HeroDirection direction); //向指定方向移动一格

	void setFaceDirection(HeroDirection direction);//设置面部朝向

	void fight();//打架

	bool isHeroMoving; // 标记是否在移动

	bool isHeroFighting; // 标记是否在打架

	bool isDoorOpening; // 标记是否在开门

	void pickUpItem(); // 捡东西

	void openDoor(int targetDoorGID); // 开门

	void DoorOpeningUpdate(float dt);

	void actWithNPC(); // 与npc互动

	void doTeleport(Teleport *teleport); //传送

	Sprite *heroSprite; //存储英雄的精灵

	Sprite* Fighting; //战斗图片的精灵

	Point targetTileCoord; //临时保存的砖块坐标

	Point targetPosition;//临时保存的cocos坐标

	int targetDoorGID; //门的GID
	
	CollisionType checkCollision(Point heroPosition); //碰撞检测函数

	void onMoveDone(Node* pTarget, int data);//移动完成后的回调函数

};


#endif
