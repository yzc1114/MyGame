#ifndef _HERO_
#define _HERO_

#include"cocos2d.h"
#include"Global.h"
#include"GameMap.h"
#include"AnimationControl.h"
USING_NS_CC;

class Hero : public Node {
	

public:
	int HP; //血量

	int ATK; //攻击力

	int DEF; //防御力

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

	//void pickUpItem(); // 捡东西

	//void openDoor(int targetDoorGID); // 开门

	//void actWithNPC(); // 与npc互动

	Sprite *heroSprite; //存储英雄的精灵

	Point targetTileCoord; //临时保存的砖块坐标

	Point targetPosition;//临时保存的cocos坐标

	int targetDoorGID; //门的GID
	
	CollisionType checkCollision(Point heroPosition); //碰撞检测函数

	void onMoveDone(Node* pTarget, void* data);//移动完成后的回调函数

};


#endif
