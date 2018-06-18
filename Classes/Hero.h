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
	int YellowKeys; //黄钥匙
	int BlueKeys; //蓝钥匙
	int RedKeys; //红钥匙
	int coins; //金币
	bool HavingAxes; //是否有镐子
	CREATE_FUNC(Hero);
	bool init(); //初始化英雄
	Hero();
	~Hero();
	//向指定方向移动一格
	void move(HeroDirection direction); 
	//设置面部朝向
	void setFaceDirection(HeroDirection direction);
	//打架
	void fight();
	//判断敌人是否能被击败
	bool ifTheEnemyCanBeHurt(Enemy* enemy);
	// 捡东西
	void pickUpItem(); 
	// 开门
	void openDoor(int targetDoorGID); 
	//开门时更新图块的schedule函数
	void DoorOpeningUpdate(float dt);
	// 与npc互动
	void actWithNPC(); 
	 //传送
	void doTeleport(Teleport *teleport);
	//打架定时
	void FightingUpdate(float dt);
	//碰撞检测函数
	CollisionType checkCollision(Point heroPosition);
	//移动完成后的回调函数
	void onMoveDone(Node* pTarget, int data);
	//播放打架音效的定时器
	void MusicUpdate(float dt);
	//自动寻路至某点 coco坐标
	void moveToSomePointAutomatically(Vec2 TileCoord);
	//自动寻路中 检查是否可达
	bool ifReachable(Vec2 tilecoord);
	Enemy* enemy;//当前面对的敌人
	int targetDoorGID; //门的GID
	Point targetTileCoord; //临时保存的砖块坐标
	Point targetPosition;//临时保存的cocos坐标
	Sprite *heroSprite; //存储英雄的精灵
	Sprite* FightingSprite; //战斗图片的精灵
	bool isHeroMoving; // 标记是否在移动
	bool isHeroFighting; // 标记是否在打架
	bool isDoorOpening; // 标记是否在开门
	bool isTalking; //标记是否在对话
};


#endif
