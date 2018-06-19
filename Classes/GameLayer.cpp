#include"GameLayer.h"

GameLayer::GameLayer()
{
	Global::instance()->gameLayer = this;
}

GameLayer::~GameLayer()
{
}

bool GameLayer::init()
{
	if (!Layer::init()) {
		return false;
	}
	return true;
}

void GameLayer::extraInit(int floor)
{
	//��GameMaps�ֵ����е�ͼ ��Ӧ�����д洢�ĵ�ͼ�ͷŲ�����ֵ�
	if (!Global::instance()->GameMaps.empty()) {
		for (auto pair : Global::instance()->GameMaps) {
			while (pair.second->getReferenceCount() > 1) {
				pair.second->release();
			}
			pair.second->release();
		}
		Global::instance()->GameMaps.clear();
	}
	//�����µ�ͼ
	GameMap* map = GameMap::createMap(floor);
	this->addChild(map, kZmap, kZmap);
	map->setPosition(0, 0);
	//������Ӣ��
	Hero* hero = Hero::create();
	hero->setPosition(GameMap::positionForTileCoord(Global::instance()->heroSpawnTileCoord));
	hero->setZOrder(kZhero);
	this->addChild(hero);
	//�Զ����ߵĴ������� ��д��������Ϊ��һ��ʼ��Ϊ����ȡ�����������Ľڵ��й� �����ҷ��ֲ��ܵ��������һ�õ����궼�Ǿ��Ե� ������Ҳ���ø���
	auto touchListenerForAutoWalking = EventListenerTouchOneByOne::create();
	touchListenerForAutoWalking->onTouchBegan = [&](Touch* touch, Event* eve) {
		Vec2 tilecoord = GameMap::tileCoordForPosition(Vec2(touch->getStartLocation().x - STATUSBARWIDTH , touch->getStartLocation().y - TIPBARHEIGHT - 32));
		Global::instance()->hero->moveToSomePointAutomatically(tilecoord);
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenerForAutoWalking, this);
}

GameLayer* GameLayer::createGameLayer(int floor)
{
	GameLayer* layer = GameLayer::create();
	layer->extraInit(floor);
	return layer;
}


//���ݵ�ͼ�����л���ͼ
void GameLayer::switchMap(int floor)
{
	if (floor > Global::instance()->highestStorey) {
		Global::instance()->highestStorey = floor;
	}
	//��Ŀǰ��ͼ�ӽڵ��gameLayer��ɾ��
    GameMap* gameMap = Global::instance()->gameMap;
	this->removeChildByTag(kZmap);
	//�����µ�ͼ ����Ѿ������� ������Ѵ����õĵ�ͼ
	gameMap = GameMap::createMap(floor);
	this->addChild(gameMap, kZmap, kZmap);
	//��global��������
	Global::instance()->gameMap = gameMap;
	Global::instance()->hero->setPosition(GameMap::positionForTileCoord(Global::instance()->heroSpawnTileCoord));
	Global::instance()->hero->setFaceDirection(kdown);
	//ʹhero����һ������ ʹ�ø��л���ͼʱ��������0.5�����޷��ƶ�
	auto action = Sequence::create(CallFunc::create([&]() {
										Global::instance()->hero->isHeroMoving = true;
									}),
									DelayTime::create(0.2f),
								   CallFunc::create([&]() {
										Global::instance()->hero->isHeroMoving = false;
									}),
									NULL
	);
	Global::instance()->hero->runAction(action);
	//����global���� ˢ��״̬��
	Global::instance()->currentLevel = floor;
	Global::instance()->gameScene->refreshStatus(kZcurrentlevel);
}
