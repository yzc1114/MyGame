#include"GameMap.h"

GameMap::GameMap()
{
	Global::instance()->gameMap = this;
}

GameMap::~GameMap()
{
}
//如果Global中的GameMaps字典已经存储了我想要创建的地图 则返回该地图 若没有存储 则返回一个全新的地图
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
//不以Global中存储的GameMaps为参照的创建一个全新的地图 用于读档时
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
	//初始化各层对象
	FloorLayer = this->getLayer("floor");
	WallLayer = this->getLayer("wall");
	ItemLayer = this->getLayer("item");
	DoorLayer = this->getLayer("door");
	//初始化敌人和对象
	initEnemy();
	initObject();
}

//根据cocos坐标获得砖块坐标
Point GameMap::tileCoordForPosition(Point position)
{
	auto map = Global::instance()->gameMap;

	int x = position.x / map->getTileSize().width;
	int y = (((map->getMapSize().height - 1) * map->getTileSize().height) - position.y) / map->getTileSize().height;
	return Point(x, y);
}

//根据砖块坐标获得cocos坐标
Point GameMap::positionForTileCoord(Point tileCoord)
{
	auto map = Global::instance()->gameMap;

	Point pos = Point((tileCoord.x * map->getTileSize().width), ((map->getMapSize().height - tileCoord.y - 1) * map->getTileSize().height));
	return pos;
}

void GameMap::initEnemy()
{
	//获取怪物层
	enemyLayer = this->getLayer("enemy");

	if (!enemyLayer) {
		return;
	}

	Size s = enemyLayer->getLayerSize();

	//遍历enemy层，将存在的怪物保存到数组中
	for (int x = 0; x < s.width; x++)
	{
		for (int y = 0; y < s.height; y++)
		{
			int gid = enemyLayer->getTileGIDAt(Point(x, y));
			if (gid != 0)
			{
				Enemy* enemy = new Enemy();

				//保存怪物坐标
				enemy->position = Point(x, y);

				//保存怪物起始的图块ID
				enemy->startGID = gid;

				//添加怪物对象到数组
				enemyArray.pushBack(enemy);
			}
		}
	}

	//用于更新敌人动画
	schedule(schedule_selector(GameMap::updateEnemyAnimation), 0.2f);
}

//更新怪物的图块
void GameMap::updateEnemyAnimation(float time)
{
	Vector<Enemy*>::iterator  iter;
	Enemy *enemy, *needRemove = NULL;

	//遍历保存所有怪物对象的数组
	for (iter = enemyArray.begin(); iter != enemyArray.end(); ++iter)
	{
		enemy = *iter;
		if (enemy != NULL)
		{
			//获取怪物当前的图块ID
			int gid = enemyLayer->getTileGIDAt(enemy->position);

			//如果怪物被删除了，需要把它在enemyArray中也删除
			if (gid == 0)
			{
				needRemove = enemy;
				continue;
			}
			gid++;

			//如果下一个图块ID 起始图块ID
			if (gid - enemy->startGID > 3)
			{
				gid = enemy->startGID;
			}

			//给怪物设置新的图块
			enemyLayer->setTileGID(gid, enemy->position);
		}
	}

	//删除被消灭的怪物
	if (needRemove != NULL)
	{
		enemyArray.eraseObject(needRemove);
	}
}



//初始化对象层
void GameMap::initObject()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("object");

	//获取对象层内的所有对象
	const ValueVector &objects = group->getObjects();

	//遍历所有对象
	for (ValueVector::const_iterator it = objects.begin();it != objects.end(); it++)
	{
		const ValueMap &dict = (*it).asValueMap();

		std::string key = "x";

		//获取x坐标
		int x = dict.at(key).asInt();
		key = "y";

		//获取y坐标
		int y = dict.at(key).asInt();
		Point tileCoord = tileCoordForPosition(Point(x, y));

		//计算唯一ID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;

		key = "type";

		//获取对象类别
		std::string type = dict.at(key).asString();


		
		//如果类型是传送门
		if (type == "teleport")
		{
			Teleport *teleport = new Teleport(dict, x, y);
			teleportDict.insert(index, teleport);
		}
		

		//如果类型是NPC对象
		if (type == "npc")
		{
			NPC *npc = new NPC(dict, x, y);
			npcDict.insert(index, npc);
		}
		
	}
}

void GameMap::afterSwitchMap()
{
	//获取对象层
	TMXObjectGroup* group = this->objectGroupNamed("object");

	//获取对象层内的所有对象
	const ValueVector &objects = group->getObjects();

	//遍历所有对象
	for (ValueVector::const_iterator it = objects.begin(); it != objects.end(); it++)
	{
		const ValueMap &dict = (*it).asValueMap();

		std::string key = "x";

		//获取x坐标
		int x = dict.at(key).asInt();
		key = "y";

		//获取y坐标
		int y = dict.at(key).asInt();
		Point tileCoord = tileCoordForPosition(Point(x, y));

		//计算唯一ID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;

		key = "type";

		//获取对象类别
		std::string type = dict.at(key).asString();




		//如果类型是NPC对象
		if (type == "npc")
		{
			auto npc = npcDict.at(index);
			//从动画管理器中根据npcId获取动画，开始永久播放
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