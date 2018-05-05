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
		|| collisionType == kenemy
		|| collisionType == kdoor
		|| collisionType == knpc)
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

	if (heroPosition.x < 0
		|| targetTileCoord.x > map->getMapSize().width - 1
		|| targetTileCoord.y < 0
		|| targetTileCoord.y > map->getMapSize().height - 1)
	{
		return kwall;
	}
}


void Hero::setFaceDirection(HeroDirection direction) {
	heroSprite->setTextureRect(Rect(0, 32 * direction, 32, 32));
}

void Hero::onMoveDone(Node* pTarget, void* data) {
	int direction = (int)data;
	setFaceDirection((HeroDirection)direction);
	isHeroMoving = false;

}