#ifndef __NPC_H__
#define __NPC_H__

#include"MagicTower.h"

USING_NS_CC;

class NPC : public Ref
{
public:
	//���캯����Ҫ���ݴ��ݵ����Ա��ʼ����������
	NPC(const ValueMap &dict, int x, int y);
	~NPC(void);

	//������ʾnpc�ľ���
	Sprite *npcSprite;

	//������TileMap�����õ�name��
	std::string npcId;

	//npc���ڵ�TileMap����
	Point tileCoord;

	//ͼƬ������ļ�·��
	std::string imagePath;

	//�����Rect
	Rect rect;

	//��Ӧ�����е�type��
	std::string type;

	//���NPC�����˵��һЩƨ������message��
	std::string contactMessage;
};

#endif