#include "SaveControl.h"

DECLARE_SINGLETON_MEMBER(SaveControl);


SaveControl::SaveControl()
{
}


SaveControl::~SaveControl()
{
}

void SaveControl::initSaveControl()
{
	writablePath = FileUtils::getInstance()->getWritablePath();
	fullPath = writablePath + "text.xml";
	Global::instance()->fullpath = fullPath;
	root = Save[0] = Save[1] = Save[2] = Save[3] = Save[4] = nullptr;

	if (!FileUtils::getInstance()->isFileExist(fullPath)) {

		root = Dictionary::create();
		root->retain();

		for (int i = 0; i < 5; i++) {
			Save[i] = Dictionary::create();
			Save[i]->retain();
			Save[i]->setObject(Bool::create(false), "IfSaved");
		}

		root->setObject(Save[0], "Save0");
		root->setObject(Save[1], "Save1");
		root->setObject(Save[2], "Save2");
		root->setObject(Save[3], "Save3");
		root->setObject(Save[4], "Save4");
	}
	else {
		root = Dictionary::createWithContentsOfFile(fullPath.c_str());
		root->retain();
		(Save[0] = (Dictionary*)(root->objectForKey("Save0")))->retain();
		(Save[1] = (Dictionary*)(root->objectForKey("Save1")))->retain();
		(Save[2] = (Dictionary*)(root->objectForKey("Save2")))->retain();
		(Save[3] = (Dictionary*)(root->objectForKey("Save3")))->retain();
		(Save[4] = (Dictionary*)(root->objectForKey("Save4")))->retain();
	}
}

void SaveControl::save(int order)
{
	

	
	std::string space = " ";
	auto Hero = Global::instance()->hero;
	auto GameMaps = Global::instance()->GameMaps;

	

	auto Saver = Save[order];
	
	auto SaverOfHero = Dictionary::create();

	//�ô浵���д洢
	Saver->setObject(Bool::create(true), "IfSaved");
	Saver->setObject(Integer::create(Global::instance()->highestStorey), "highestStorey");
	//����Ӣ�������Ϣ
	SaverOfHero->setObject(Integer::create(Hero->HP), "HP");
	SaverOfHero->setObject(Integer::create(Hero->ATK), "ATK");
	SaverOfHero->setObject(Integer::create(Hero->DEF), "DEF");
	SaverOfHero->setObject(Integer::create(Hero->coins), "coins");
	SaverOfHero->setObject(Integer::create(Hero->YellowKeys), "YellowKeys");
	SaverOfHero->setObject(Integer::create(Hero->BlueKeys), "BlueKeys");
	SaverOfHero->setObject(Integer::create(Hero->RedKeys), "RedKeys");
	SaverOfHero->setObject(Integer::create(Global::instance()->currentLevel), "currentLevel");
	SaverOfHero->setObject(Integer::create(Hero->getPositionX()), "PositionX");
	SaverOfHero->setObject(Integer::create(Hero->getPositionY()), "PositionY");
	
	Saver->setObject(SaverOfHero, "SaverOfHero");

	Dictionary* SaverOfMap = Dictionary::create();

	for (auto iter = GameMaps.begin(); iter != GameMaps.end(); iter++) {
		int floor = iter->first;
		std::string floor_str = std::to_string(floor);
		GameMap* map = iter->second;
		auto DoorLayer = map->DoorLayer;
		auto ItemLayer = map->ItemLayer;
		auto enemyLayer = map->enemyLayer;
		Size s = DoorLayer->getLayerSize();

		
		String* temp;

		for (int x = 0; x < s.width; x++) {
			for (int y = 0; y < s.height; y++) {

				std::string x_str = std::to_string(x);
				std::string y_str = std::to_string(y);

				if (DoorLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //�����ͼ�ϵ��� �Ƿ��Ѿ�������
					SaverOfMap->setObject(Bool::create(true),"Door" + space + floor_str + space + x_str + space + y_str);//�Ż���
				}
				else {
					if (SaverOfMap->objectForKey("Door" + space + floor_str + space + x_str + space + y_str) != nullptr) {
						SaverOfMap->removeObjectForKey("Door" + space + floor_str + space + x_str + space + y_str);
					}
				}
				

				if (ItemLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //�����ͼ�ϵ���Ʒ �Ƿ��Ѿ�������
					SaverOfMap->setObject(Bool::create(true),"Item" + space + floor_str + space + x_str + space + y_str); //��������
				}
				else {
					if (SaverOfMap->objectForKey("Item" + space + floor_str + space + x_str + space + y_str) != nullptr) {
						SaverOfMap->removeObjectForKey("Item" + space + floor_str + space + x_str + space + y_str);
					}
				}
				

				if (enemyLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //�����ͼ�ϵĹ��� �Ƿ��Ѿ�������
					SaverOfMap->setObject(Bool::create(true),"Enemy" + space + floor_str + space + x_str + space + y_str); //���ﻹ�� �洢Ϊtrue
				}
				else {
					if (SaverOfMap->objectForKey("Enemy" + space + floor_str + space + x_str + space + y_str) != nullptr) {
						SaverOfMap->removeObjectForKey("Enemy" + space + floor_str + space + x_str + space + y_str);
					}
				}
				

			}
		}
		

	}

	Saver->setObject(SaverOfMap, "SaverOfMap");

	root->writeToFile(fullPath.c_str());


}

void SaveControl::load(int order)
{
	if (!checkIfTheSaveExisted(order)) {
		assert("Loading Save Doesn't Existed");
		return;
	}
	
	//���ȫ���ļ���Ϣ��ValueMap
	auto valueMap = FileUtils::getInstance()->getValueMapFromFile(fullPath);
	//���Ŀ��浵��Dictֵ
	auto Saver = valueMap.at("Save" + std::to_string(order)).asValueMap();
	//ͨ��ValueMap��key�ֱ���Ӣ�ۺ͵�ͼ���ֵ�
	auto SaverOfHero = Saver.at("SaverOfHero").asValueMap();
	auto SaverOfMap = Saver.at("SaverOfMap").asValueMap();
	//��ȡȫ�ֵ��������Hero��GameMaps�ֵ�
	auto Hero = Global::instance()->hero;
	auto GameMaps = Global::instance()->GameMaps;

	
	
	//����Ӣ��ȫ����Ϣ
	Hero->HP = SaverOfHero.at("HP").asInt();
	Hero->ATK = SaverOfHero.at("ATK").asInt();
	Hero->DEF = SaverOfHero.at("DEF").asInt();
	Hero->coins = SaverOfHero.at("coins").asInt();
	Hero->YellowKeys = SaverOfHero.at("YellowKeys").asInt();
	Hero->BlueKeys = SaverOfHero.at("BlueKeys").asInt();
	Hero->RedKeys = SaverOfHero.at("RedKeys").asInt();
	Global::instance()->currentLevel = SaverOfHero.at("currentLevel").asInt();
	int x = SaverOfHero.at("PositionX").asInt();
	int y = SaverOfHero.at("PositionY").asInt();
	Hero->setPosition(x, y);

	//����ǰ��map��gameLayer��ȥ��
	Global::instance()->gameLayer->removeChildByTag(kZmap);
	Global::instance()->gameMap = nullptr;
	
	std::map<int, GameMap*> newGameMaps;

	

	//��������GameMaps���� ��ÿ��ԭ�������map�ͷ� �������µĵ�ͼ������
	for (auto iter : GameMaps) {
		do {
			iter.second->release();
		} while (iter.second->getReferenceCount() != 1);
		iter.second->release();
	}

	auto highestStorey = Saver.at("highestStorey").asInt();
		
	for(int floor = 0 ; floor <= highestStorey ; floor++){
		
		std::string floor_str = std::to_string(floor);
		std::string space = " ";
		GameMap* map = GameMap::createNewMap(floor);

		TMXLayer* DoorLayer = map->DoorLayer;
		TMXLayer* enemyLayer = map->enemyLayer;
		TMXLayer* ItemLayer = map->ItemLayer;
		
		Size s = DoorLayer->getLayerSize();

		newGameMaps.insert(std::pair<int, GameMap*>(floor,map));


		for (int x = 0; x < s.width; x++) {
			for (int y = 0; y < s.height; y++) {

				std::string x_str = std::to_string(x);
				std::string y_str = std::to_string(y);

				if (DoorLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //�鿴��ͼ�ϵ��� �Ƿ��Ѿ�������
																   
					if (SaverOfMap.count("Door" + space + floor_str + space + x_str + space + y_str) != 1){
						DoorLayer->removeTileAt(Vec2(x, y));
					}
				}

				if (ItemLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //�鿴��ͼ�ϵ���Ʒ �Ƿ��Ѿ�������
																   
					if (SaverOfMap.count("Item" + space + floor_str + space + x_str + space + y_str) != 1) {
						ItemLayer->removeTileAt(Vec2(x, y));
					}
				}

				if (enemyLayer->getTileGIDAt(Vec2(x, y)) != 0) {    //�鿴��ͼ�ϵĹ��� �Ƿ��Ѿ�������
																	
					if (SaverOfMap.count("Enemy" + space + floor_str + space + x_str + space + y_str) != 1) {
						enemyLayer->removeTileAt(Vec2(x, y));
					}
				}

			}
		}

		
	}


	Global::instance()->GameMaps = newGameMaps;

	if (Global::instance()->GameMaps.count(Global::instance()->currentLevel) == 1) {

		GameMap* currentMap = Global::instance()->GameMaps.at(Global::instance()->currentLevel);
		Global::instance()->gameMap = currentMap;
		Global::instance()->gameLayer->addChild(currentMap, kZmap, kZmap);
		currentMap->setPosition(0, 0);
	}
	else {
		exit(0);
	}
	
	
}

void SaveControl::deleteSave(int order)
{
	if (!checkIfTheSaveExisted(order)) {
		assert("Loading Save Doesn't Existed");
		return;
	}
	auto Saver = Save[order];
	Saver->setObject(Bool::create(false), "IfSaved");
	root->writeToFile(fullPath.c_str());
}

bool SaveControl::checkIfTheSaveExisted(int order)
{
	if (!FileUtils::getInstance()->isFileExist(fullPath)) {
		return false;
	}
	else {
		//���ȫ���ļ���Ϣ��ValueMap
			auto valueMap = FileUtils::getInstance()->getValueMapFromFile(fullPath);
			//���Ŀ��浵��Dictֵ
			auto Saver = valueMap.at("Save" + std::to_string(order)).asValueMap();
	
			if (Saver.at("IfSaved").asBool()) {
				return true;
			}
			else {
				return false;
			}
		}
	
}


