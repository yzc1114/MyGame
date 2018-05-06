#include"Hero.h"

bool Hero::init()
{
	if (!Node::init())
	{
		return false;
	}

	heroSprite = Sprite::createWithSpriteFrame(AnimationControl::instance()->getAnimation(kdown)->getFrames().at(0)->getSpriteFrame());

	heroSprite->setAnchorPoint(Point::ZERO);

	this->addChild(heroSprite);

	ATK = DEF = 100;
	HP = 1000;
	YellowKeys = 5;
	BlueKeys = 1;
	RedKeys = 1;

	isHeroMoving = isHeroFighting = isDoorOpening = false;
	return true;
}

Hero::Hero()
{
	Global::instance()->hero = this;
}

Hero::~Hero()
{
}

void Hero::move(HeroDirection direction)
{
	if (isHeroMoving)
	{
		return;
	}

	//移动的距离
	Point moveByPosition;

	//根据方向计算移动的距离
	switch (direction)
	{
	case kdown:
		moveByPosition = Point(0, -32);
		break;
	case kleft:
		moveByPosition = Point(-32, 0);
		break;
	case kright:
		moveByPosition = Point(32, 0);
		break;
	case kup:
		moveByPosition = Point(0, 32);
		break;
	}

	//计算目标坐标，用当前勇士坐标加上移动距离
	targetPosition = this->getPosition() + moveByPosition;

	//调用checkCollision检测碰撞类型，如果是墙壁、怪物、门，则只需要设置勇士的朝向
	CollisionType collisionType = checkCollision(targetPosition);

	if (collisionType == kwall
		//|| collisionType == kenemy
		//|| collisionType == knpc
		|| collisionType == kdoor
		)
	{
		setFaceDirection((HeroDirection)direction);
		return;
	}

	//heroSprite仅播放行走动画
	heroSprite->runAction(AnimationControl::instance()->createAnimate(direction));

	//主体进行位移，结束时调用onMoveDone方法 把方向信息传递给onMoveDone方法
	Action *action = Sequence::create(
		MoveBy::create(0.20f, moveByPosition),
		CallFuncN::create(CC_CALLBACK_1(Hero::onMoveDone, this, (void*)direction)),
		NULL);

	this->runAction(action);

	isHeroMoving = true;
}

CollisionType Hero::checkCollision(Point heroPosition)
{
	auto map = Global::instance()->gameMap;
	targetTileCoord = map->tileCoordForPosition(heroPosition);

	//超出边界
	if (heroPosition.x < 0
		|| targetTileCoord.x > map->getMapSize().width - 1
		|| targetTileCoord.y < 0
		|| targetTileCoord.y > map->getMapSize().height - 1)
	{
		return kwall;
	}

	//获取墙壁层对应坐标的图块ID
	int targetTileGID = Global::instance()->gameMap->WallLayer->getTileGIDAt(targetTileCoord);

	//如果图块ID不为0，表示有墙
	if (targetTileGID)
	{
		return kwall;
	}

	//获得物品层对应坐标的图块ID
	targetTileGID = Global::instance()->gameMap->ItemLayer->getTileGIDAt(targetTileCoord);

	//如果图块ID不为0，表示有物品
	if (targetTileGID) {
		pickUpItem();
		return kitem;
	}

	//获得门层对应坐标的图块ID
	targetTileGID = Global::instance()->gameMap->DoorLayer->getTileGIDAt(targetTileCoord);
	
	//如果图块ID不为0，表示有门
	if (targetTileGID) {
		openDoor(targetTileGID);
		return kdoor;
	}


	//获得怪物层对应坐标的图块ID
	targetTileGID = Global::instance()->gameMap->enemyLayer->getTileGIDAt(targetTileCoord);

	//如果图块ID不为0，表示有敌人
	if (targetTileGID) {
		return kenemy;
	}



	int index = targetTileCoord.x + targetTileCoord.y * Global::instance()->gameMap->getMapSize().width;

	//从npc字典中查询
	NPC *npc = Global::instance()->gameMap->npcDict.at(index);
	if (npc != NULL)
	{
		actWithNPC();
		return knpc;
	}

	//从Teleport字典中查询
	Teleport *teleport = Global::instance()->gameMap->teleportDict.at(index);
	if (teleport != NULL)
	{
		doTeleport(teleport);
		return kteleport;
	}


	return knull;
}


void Hero::setFaceDirection(HeroDirection direction) {
	heroSprite->setTextureRect(Rect(0, 32 * direction, 32, 32));
}

void Hero::onMoveDone(Node* pTarget, void* data) {
	int direction = (int)data;
	setFaceDirection((HeroDirection)direction);
	isHeroMoving = false;

}

void Hero::doTeleport(Teleport *teleport) {

}

void Hero::actWithNPC() {

}

void Hero::openDoor(int gid) {
	if (isDoorOpening) {
		return;
	}

	targetDoorGID = gid;
	isDoorOpening = true;

	schedule(schedule_selector(Hero::DoorOpeningUpdate), 0.1f);
}

void Hero::pickUpItem() {

	Global::instance()->gameMap->ItemLayer->removeTileAt(targetTileCoord);

}

void Hero::DoorOpeningUpdate(float dt) {
	
	int NextGid = Global::instance()->gameMap->DoorLayer->getTileGIDAt(targetTileCoord) + 4;

	if (NextGid - targetDoorGID > 12) {
		Global::instance()->gameMap->DoorLayer->removeTileAt(targetTileCoord);
		unschedule(schedule_selector(Hero::DoorOpeningUpdate));
		isDoorOpening = false;
	}
	else {
		Global::instance()->gameMap->DoorLayer->setTileGID(NextGid,targetTileCoord);
	}
}
