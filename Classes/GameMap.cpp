#include"GameMap.h"

GameMap::GameMap()
{
	Global::instance()->gameMap = this;
}

GameMap::~GameMap()
{
}
//���Global�е�GameMaps�ֵ��Ѿ��洢������Ҫ�����ĵ�ͼ �򷵻ظõ�ͼ ��û�д洢 �򷵻�һ��ȫ�µĵ�ͼ
GameMap* GameMap::createMap(int floor)
{
	if (Global::instance()->GameMaps.count(floor) == 1 && Global::instance()->GameMaps.at(floor) != nullptr) {
		Global::instance()->gameMap = Global::instance()->GameMaps.at(floor);
		Global::instance()->GameMaps.at(floor)->afterSwitchMap();
		return Global::instance()->GameMaps.at(floor);
	}
	GameMap* map = new GameMap;
	if (map->initWithTMXFile("tile maps/" + std::to_string(floor) + ".tmx"))
	{
		map->extraInit();
		map->retain();
		Global::instance()->GameMaps.insert(std::pair<int, GameMap*>(floor, map));
		return map;
	}

	CC_SAFE_DELETE(map);
	return NULL;

}
//����Global�д洢��GameMapsΪ���յĴ���һ��ȫ�µĵ�ͼ ���ڶ���ʱ
GameMap * GameMap::createNewMap(int floor)
{
	GameMap* map = new GameMap;
	if (map->initWithTMXFile("tile maps/" + std::to_string(floor) + ".tmx"))
	{
		map->extraInit();
		map->retain();
		return map;
	}

	CC_SAFE_DELETE(map);
	return NULL;
}

void GameMap::extraInit()
{
	//��ʼ���������
	FloorLayer = this->getLayer("floor");
	WallLayer = this->getLayer("wall");
	ItemLayer = this->getLayer("item");
	DoorLayer = this->getLayer("door");
	//��ʼ�����˺Ͷ���
	initEnemy();
	initObject();
}

//����cocos������ש������
Point GameMap::tileCoordForPosition(Point position)
{
	auto map = Global::instance()->gameMap;

	int x = position.x / map->getTileSize().width;
	int y = (((map->getMapSize().height - 1) * map->getTileSize().height) - position.y) / map->getTileSize().height;
	return Point(x, y);
}

//����ש��������cocos����
Point GameMap::positionForTileCoord(Point tileCoord)
{
	auto map = Global::instance()->gameMap;

	Point pos = Point((tileCoord.x * map->getTileSize().width), ((map->getMapSize().height - tileCoord.y - 1) * map->getTileSize().height));
	return pos;
}

void GameMap::initEnemy()
{
	//��ȡ�����
	enemyLayer = this->getLayer("enemy");

	if (!enemyLayer) {
		return;
	}

	Size s = enemyLayer->getLayerSize();

	//����enemy�㣬�����ڵĹ��ﱣ�浽������
	for (int x = 0; x < s.width; x++)
	{
		for (int y = 0; y < s.height; y++)
		{
			int gid = enemyLayer->getTileGIDAt(Point(x, y));
			if (gid != 0)
			{
				Enemy* enemy = new Enemy();

				//�����������
				enemy->position = Point(x, y);

				//���������ʼ��ͼ��ID
				enemy->startGID = gid;

				//��ӹ����������
				enemyArray.pushBack(enemy);
			}
		}
	}

	//���ڸ��µ��˶���
	schedule(schedule_selector(GameMap::updateEnemyAnimation), 0.2f);
}

//���¹����ͼ��
void GameMap::updateEnemyAnimation(float time)
{
	Vector<Enemy*>::iterator  iter;
	Enemy *enemy, *needRemove = NULL;

	//�����������й�����������
	for (iter = enemyArray.begin(); iter != enemyArray.end(); ++iter)
	{
		enemy = *iter;
		if (enemy != NULL)
		{
			//��ȡ���ﵱǰ��ͼ��ID
			int gid = enemyLayer->getTileGIDAt(enemy->position);

			//������ﱻɾ���ˣ���Ҫ������enemyArray��Ҳɾ��
			if (gid == 0)
			{
				needRemove = enemy;
				continue;
			}
			gid++;

			//�����һ��ͼ��ID ��ʼͼ��ID
			if (gid - enemy->startGID > 3)
			{
				gid = enemy->startGID;
			}

			//�����������µ�ͼ��
			enemyLayer->setTileGID(gid, enemy->position);
		}
	}

	//ɾ��������Ĺ���
	if (needRemove != NULL)
	{
		enemyArray.eraseObject(needRemove);
	}
}



//��ʼ�������
void GameMap::initObject()
{
	//��ȡ�����
	TMXObjectGroup* group = this->objectGroupNamed("object");

	//��ȡ������ڵ����ж���
	const ValueVector &objects = group->getObjects();

	//�������ж���
	for (ValueVector::const_iterator it = objects.begin();it != objects.end(); it++)
	{
		const ValueMap &dict = (*it).asValueMap();

		std::string key = "x";

		//��ȡx����
		int x = dict.at(key).asInt();
		key = "y";

		//��ȡy����
		int y = dict.at(key).asInt();
		Point tileCoord = tileCoordForPosition(Point(x, y));

		//����ΨһID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;

		key = "type";

		//��ȡ�������
		std::string type = dict.at(key).asString();


		
		//��������Ǵ�����
		if (type == "teleport")
		{
			Teleport *teleport = new Teleport(dict, x, y);
			teleportDict.insert(index, teleport);
		}
		

		//���������NPC����
		if (type == "npc")
		{
			NPC *npc = new NPC(dict, x, y);
			npcDict.insert(index, npc);
		}
		
	}
}

void GameMap::afterSwitchMap()
{
	//��ȡ�����
	TMXObjectGroup* group = this->objectGroupNamed("object");

	//��ȡ������ڵ����ж���
	const ValueVector &objects = group->getObjects();

	//�������ж���
	for (ValueVector::const_iterator it = objects.begin(); it != objects.end(); it++)
	{
		const ValueMap &dict = (*it).asValueMap();

		std::string key = "x";

		//��ȡx����
		int x = dict.at(key).asInt();
		key = "y";

		//��ȡy����
		int y = dict.at(key).asInt();
		Point tileCoord = tileCoordForPosition(Point(x, y));

		//����ΨһID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;

		key = "type";

		//��ȡ�������
		std::string type = dict.at(key).asString();




		//���������NPC����
		if (type == "npc")
		{
			auto npc = npcDict.at(index);
			//�Ӷ����������и���npcId��ȡ��������ʼ���ò���
			Animate* animation = AnimationControl::instance()->createAnimate((npc->npcId).c_str());
			if (animation != NULL)
			{
				ActionInterval* action = RepeatForever::create(animation);
				npc->npcSprite->runAction(action);
			}
		}

	}
	schedule(schedule_selector(GameMap::updateEnemyAnimation), 0.2f);
}