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

	for (auto map : GameMaps) {
		while (map.second->getReferenceCount() > 1) {
			map.second->release();
		}
		map.second->release();
	}

	GameMaps.clear();

	//����Ϸ����ǰ��ͼ����Ϊ0
	currentLevel = 0;

	//��ʿ����λ�� ��ʼ��Ϊ��0��ĳ���λ��
	heroSpawnTileCoord = Point(1, 11);

	highestStorey = 0;
}

