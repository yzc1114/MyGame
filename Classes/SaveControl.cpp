#include "SaveControl.h"

DECLARE_SINGLETON_MEMBER(SaveControl);


SaveControl::SaveControl()
{
}


SaveControl::~SaveControl()
{
}

void SaveControl::save(int order)
{
	
	//sqlite3* pDataBase;
	//if ((sqlite3_open("Save.db", &pDataBase) != SQLITE_OK)) {
	//	assert("open_failed");
	//}
	//sqlite3_exec(pDataBase, "create table test( ID integer primary key autoincrement, name nvarchar(32) )", nullptr, nullptr, nullptr);
	//sqlite3_exec(pDataBase, "insert into MyTable_1(name) values(‘走路’)", nullptr, nullptr, nullptr);

	std::string order_str = std::to_string(order);
	std::string space = " ";
	auto Saver = UserDefault::getInstance();
	auto Hero = Global::instance()->hero;
	auto GameMaps = Global::instance()->GameMaps;

	//保存该存档确实存在
	Saver->setBoolForKey((order_str).c_str(), true);

	//保存英雄相关属性
	Saver->setIntegerForKey((order_str + space + "HP").c_str(), Hero->HP);
	Saver->setIntegerForKey((order_str + space + "ATK").c_str(), Hero->ATK);
	Saver->setIntegerForKey((order_str + space + "DEF").c_str(), Hero->DEF);
	Saver->setIntegerForKey((order_str + space + "coins").c_str(), Hero->coins);
	Saver->setIntegerForKey((order_str + space + "YellowKeys").c_str(), Hero->YellowKeys);
	Saver->setIntegerForKey((order_str + space + "BlueKeys").c_str(), Hero->BlueKeys);
	Saver->setIntegerForKey((order_str + space + "RedKeys").c_str(), Hero->RedKeys);
	Saver->setIntegerForKey((order_str + space + "currentlevel").c_str(), Global::instance()->currentLevel);
	Saver->setDoubleForKey((order_str + space + "HeroCocosPositionX").c_str(), Hero->getPositionX());
	Saver->setDoubleForKey((order_str + space + "HeroCocosPositionY").c_str(), Hero->getPositionY());

	
	for (auto iter = GameMaps.begin(); iter != GameMaps.end(); iter++) {
		int floor = iter->first;
		std::string floor_str = std::to_string(floor);
		GameMap* map = iter->second;
		auto DoorLayer = map->DoorLayer;
		auto ItemLayer = map->ItemLayer;
		auto enemyLayer = map->enemyLayer;
		Size s = DoorLayer->getLayerSize();

		for (int x = 0; x < s.width; x++) {   
			for (int y = 0; y < s.height; y++) {
				
				std::string x_str = std::to_string(x);
				std::string y_str = std::to_string(y);

				if (DoorLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //保存地图上的门 是否已经开启过
					Saver->setBoolForKey((order_str + space + "isDoorsClosed" + space + floor_str + space + x_str + space + y_str).c_str(), true);  //门还在 存储为true
				}

				if (ItemLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //保存地图上的物品 是否已经被拿走
					Saver->setBoolForKey((order_str + space + "isItemHaveNotBeenPickedUp" + space + floor_str + space + x_str + space + y_str).c_str(), true); //东西还在 存储为true
				}

				if (enemyLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //保存地图上的怪物 是否已经被击败
					Saver->setBoolForKey((order_str + space + "isEnemyExisted" + space + floor_str + space + x_str + space + y_str).c_str(), true); //怪物还在 存储为true
				}

			}
		}

	}

	Saver->flush();
	log(UserDefault::getXMLFilePath().c_str());
}

void SaveControl::load(int order)
{
	if (!checkIfTheSaveExisted(order)) {
		return;
	}
	std::string order_str = std::to_string(order);
	auto Saver = UserDefault::getInstance();
	auto Hero = Global::instance()->hero;
	auto GameMaps = Global::instance()->GameMaps;


	Hero->HP = Saver->getIntegerForKey((order_str + "HP").c_str());
	Hero->ATK = Saver->getIntegerForKey((order_str + "ATK").c_str());
	Hero->DEF = Saver->getIntegerForKey((order_str + "DEF").c_str());
	Hero->coins = Saver->getIntegerForKey((order_str + "coins").c_str());
	Hero->YellowKeys = Saver->getIntegerForKey((order_str + "YellowKeys").c_str());
	Hero->BlueKeys = Saver->getIntegerForKey((order_str + "BlueKeys").c_str());
	Hero->RedKeys = Saver->getIntegerForKey((order_str + "RedKeys").c_str());
	Global::instance()->currentLevel = Saver->getIntegerForKey((order_str + "currentlevel").c_str());
	int x = Saver->getFloatForKey((order_str + "HeroCocosPositionX").c_str());
	int y = Saver->getFloatForKey((order_str + "HeroCocosPositionY").c_str());
	Hero->setPosition(x, y);

	//将当前的map在gameLayer中去除
	Global::instance()->gameLayer->removeChildByTag(kZmap);  
	
	//遍历整个GameMaps数组 将每个原来保存的map释放 并保存新的地图在其中
	for (auto iter : GameMaps) { 
		iter.second->release();
		iter.second = nullptr;
		int floor = iter.first;
		std::string floor_str = std::to_string(floor);
		std::string space = " ";
		GameMap* map = GameMap::createMap(floor);
		TMXLayer* DoorLayer = map->DoorLayer;
		TMXLayer* enemyLayer = map->enemyLayer;
		TMXLayer* ItemLayer = map->ItemLayer;
		Size s = DoorLayer->getLayerSize();

		for (int x = 0; x < s.width; x++) {
			for (int y = 0; y < s.height; y++) {

				std::string x_str = std::to_string(x);
				std::string y_str = std::to_string(y);

				if (DoorLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //查看地图上的门 是否已经开启过
					   //有门 返回true 无门 返回false
					if (!  (Saver->getBoolForKey((order_str + space + "isDoorsClosed" + space + floor_str + space + x_str + space + y_str).c_str(), false) ) ) {
						DoorLayer->removeTileAt(GameMap::tileCoordForPosition(Vec2(x, y)));
					}
				}

				if (ItemLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //查看地图上的物品 是否已经被拿走
						//东西还在 返回true 不在 返回false
					if (!  (Saver->getBoolForKey((order_str + space + "isItemHaveNotBeenPickedUp" + space + floor_str + space + x_str + space + y_str).c_str(), false))) {
						ItemLayer->removeTileAt(GameMap::tileCoordForPosition(Vec2(x, y)));
					}
				}

				if (enemyLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //查看地图上的怪物 是否已经被击败
						//怪物还在 返回true 不在 返回false
					if (!(Saver->getBoolForKey((order_str + space + "isEnemyExisted" + space + floor_str + space + x_str + space + y_str).c_str(), false))) {
						enemyLayer->removeTileAt(GameMap::tileCoordForPosition(Vec2(x, y)));
					}
				}

			}
		}

		iter.second = map;

	}

	GameMap* currentMap = GameMaps.at(Global::instance()->currentLevel);
	Global::instance()->gameMap = currentMap;
	Global::instance()->gameLayer->addChild(currentMap,kZmap,kZmap);
	currentMap->setPosition(0, 0);

}

bool SaveControl::checkIfTheSaveExisted(int order)
{
	if (!UserDefault::isXMLFileExist()) {
		return false;
	}

	if (UserDefault::getInstance()->getBoolForKey(std::to_string(order).c_str(), false)) {
		return true;
	}
	else {
		return false;
	}
}


