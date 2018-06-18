#include "Global.h"

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
{
	resetGlobal();
	fullpath = "";
}

Global::~Global(void)
{

}

void Global::resetGlobal()
{
	gameScene = NULL;

	gameLayer = NULL;

	controlLayer = NULL;

	gameMap = NULL;

	hero = NULL;

	chatRoom = NULL;

	//����Ϸ����ǰ��ͼ����Ϊ0
	currentLevel = 0;

	//��ʿ����λ�� ��ʼ��Ϊ��0��ĳ���λ��
	heroSpawnTileCoord = Point(1, 11);

	highestStorey = 0;
}

