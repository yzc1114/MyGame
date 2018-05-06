#include "NPC.h"

NPC::NPC(const ValueMap &dict, int x, int y)
{
	//获取名称
	std::string key = "name";
	npcId = dict.at(key).asString();

	//获取类型
	key = "type";
	type = dict.at(key).asString();

	//获取image项
	key = "image";
	imagePath = dict.at(key).asString();

	//获取rectX和rectY
	key = "rectX";
	int x1 = dict.at(key).asInt();

	key = "rectY";
	int y1 = dict.at(key).asInt();

	rect = Rect(x1, y1, 32, 32);

	//position为cocos2d-x坐标，tileCoord为TileMap坐标
	Point position = Point(x, y);
	tileCoord = Global::instance()->gameMap->tileCoordForPosition(position);

	//创建用于显示npc的精灵
	npcSprite = Sprite::create(imagePath, rect);
	npcSprite->setAnchorPoint(Point::ZERO);
	npcSprite->setPosition(position);

	Global::instance()->gameLayer->addChild(npcSprite, kZnpc);

	
	//从动画管理器中根据npcId获取动画，开始永久播放
	Animate* animation = AnimationControl::instance()->createAnimate(npcId.c_str());
	if (animation != NULL)
	{
		ActionInterval* action = RepeatForever::create(animation);
		npcSprite->runAction(action);
	}
	
}

NPC::~NPC(void)
{
}
