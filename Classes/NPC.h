#ifndef __NPC_H__
#define __NPC_H__

#include"MagicTower.h"

USING_NS_CC;

class NPC : public Ref
{
public:
	//构造函数中要根据传递的属性表初始化各个变量
	NPC(const ValueMap &dict, int x, int y);
	~NPC(void);

	//用于显示npc的精灵
	Sprite *npcSprite;

	//保存在TileMap中配置的name项
	std::string npcId;

	//npc所在的TileMap坐标
	Point tileCoord;

	//图片纹理的文件路径
	std::string imagePath;

	//纹理的Rect
	Rect rect;

	//对应配置中的type项
	std::string type;
};

#endif