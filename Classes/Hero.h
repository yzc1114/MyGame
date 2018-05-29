#ifndef _HERO_
#define _HERO_

#include"MagicTower.h"
USING_NS_CC;

class Teleport;
class Enemy;
class Hero : public Node {
	

public:
	int HP; //血量

	int ATK; //攻击力

	int DEF; //防御力

	int YellowKeys;

	int BlueKeys;

	int RedKeys;

	int coins; //金币



	bool init(); //初始化英雄

	CREATE_FUNC(Hero);

	Hero();

	~Hero();

	void move(HeroDirection direction); //向指定方向移动一格

	void setFaceDirection(HeroDirection direction);//设置面部朝向

	void fight();//打架

	bool ifTheEnemyCanBeHurt(Enemy* enemy);//判断敌人是否能被击败

	Enemy* enemy;//当前面对的敌人

	bool isHeroMoving; // 标记是否在移动

	bool isHeroFighting; // 标记是否在打架

	bool isDoorOpening; // 标记是否在开门

	bool isTalking; //标记是否在对话

	void pickUpItem(); // 捡东西

	void openDoor(int targetDoorGID); // 开门

	void DoorOpeningUpdate(float dt); //开门时更新图块的schedule函数

	void contactMenuCALLBACK(Ref * psender);

	void actWithNPC(); // 与npc互动

	void doTeleport(Teleport *teleport); //传送

	Sprite *heroSprite; //存储英雄的精灵

	Sprite* FightingSprite; //战斗图片的精灵

	void FightingUpdate(float dt);//打架定时器

	Point targetTileCoord; //临时保存的砖块坐标

	Point targetPosition;//临时保存的cocos坐标

	int targetDoorGID; //门的GID
	
	CollisionType checkCollision(Point heroPosition); //碰撞检测函数

	void onMoveDone(Node* pTarget, int data);//移动完成后的回调函数

	void MusicUpdate(float dt);//播放打架音效的定时器

	//void contactMenuCALLBACK(Ref* psender);//跟NPC交流菜单的回调函数

};


#endif
