#include "NPC.h"

NPC::NPC(const ValueMap &dict, int x, int y)
{
	//��ȡ����
	std::string key = "name";
	npcId = dict.at(key).asString();

	//��ȡ����
	key = "type";
	type = dict.at(key).asString();

	//��ȡimage��
	key = "image";
	imagePath = dict.at(key).asString();

	//��ȡmessage��
	key = "message";
	contactMessage = dict.at(key).asString();


	//��ȡrectX��rectY
	key = "rectX";
	int x1 = dict.at(key).asInt();

	key = "rectY";
	int y1 = dict.at(key).asInt();

	rect = Rect(x1, y1, 32, 32);

	//positionΪcocos2d-x���꣬tileCoordΪTileMap����
	Point position = Point(x, y);
	tileCoord = Global::instance()->gameMap->tileCoordForPosition(position);

	//����������ʾnpc�ľ���
	npcSprite = Sprite::create(imagePath, rect);
	npcSprite->setAnchorPoint(Point::ZERO);
	npcSprite->setPosition(position);


	//����ΨһID
	int index = tileCoord.x + tileCoord.y * Global::instance()->gameMap->getMapSize().width;

	Global::instance()->gameMap->addChild(npcSprite, kZnpc, index);

	
	//�Ӷ����������и���npcId��ȡ��������ʼ���ò���
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
