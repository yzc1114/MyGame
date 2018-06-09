#include "Global.h"

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
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

	fullpath = "";
}

Global::~Global(void)
{

}

