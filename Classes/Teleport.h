#ifndef __TELEPORT_H__
#define __TELEPORT_H__

#include"MagicTower.h"

USING_NS_CC;

class Teleport : public Ref
{
public:
	Teleport(const ValueMap &dict, int x, int y);

	~Teleport(void);

	//传送点所在位置
	Point tileCoord;

	//传送到目标层后，勇士所在坐标
	Point heroTileCoord;

	//目标地图的层数
	int targetMap;

	//唯一的ID
	int index;

	//图片纹理的文件路径
	std::string imagePath;

	Sprite *teleportSprite;
};
#endif