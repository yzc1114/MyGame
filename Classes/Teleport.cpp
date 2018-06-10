#include "Teleport.h"

Teleport::Teleport(const ValueMap &dict, int x, int y)
{
	Point position = Point(x, y);


	//传送点所在的TileMap位置
	tileCoord = Global::instance()->gameMap->tileCoordForPosition(Point(x, y));

	//得出勇士在目标层的起始位置
	std::string key = "heroTileCoordX";
	int x1 = dict.at(key).asInt();

	key = "heroTileCoordY";
	int y1 = dict.at(key).asInt();

	//计算index
	index = tileCoord.x + tileCoord.y * Global::instance()->gameMap->getMapSize().width;

	heroTileCoord = Point(x1, y1);

	//取得目标地图的层数
	key = "targetMap";
	targetMap = dict.at(key).asInt();

	//获取image项
	key = "image";
	imagePath = dict.at(key).asString();

	//创建用于显示Teleport的精灵
	teleportSprite = Sprite::create(imagePath);
	teleportSprite->setAnchorPoint(Point::ZERO);
	teleportSprite->setPosition(position);
	Global::instance()->gameMap->addChild(teleportSprite, kZteleport, index);
}

Teleport::~Teleport(void)
{
}